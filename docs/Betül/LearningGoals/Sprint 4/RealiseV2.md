# Realisation Report — Local HTTPS/TLS Prototype Test for the City Sim Backend

| Document information |  |
| --- | --- |
| Title | Realisation Report — Local HTTPS/TLS Prototype Test for the City Sim Backend |
| Author | Betül Aydin |
| Date | 4 June 2026 |
| Version | 2.0 |
| Classification | Internal |
| Mayor | Mats |
| Company | Amsterdam University of Applied Sciences |
| Learning outcome | Realise |
| Sprint | Sprint 4 |


## Table of Contents

1. [Introduction](#1-introduction)
2. [Main- and Sub-Questions](#2-main--and-sub-questions)
3. [Implementation Context](#3-implementation-context)
4. [Step-by-Step Implementation](#4-step-by-step-implementation)
5. [Testing Process and Expected Results](#5-testing-process-and-expected-results)
6. [Result Evaluation](#6-result-evaluation)
7. [Conclusion](#7-conclusion)
8. [References](#8-references)



## 1. Introduction

This realisation report describes how a local HTTPS/TLS prototype was implemented and tested for the City Sim backend. The goal was to test whether encrypted HTTPS communication could be added to the existing Docker-based FastAPI backend without disrupting the current working HTTP backend.

HTTPS is relevant because it uses TLS certificates to secure the connection between a client and a server. FastAPI explains that HTTPS depends on certificates and that the encryption happens before the HTTP communication itself starts (About HTTPS - FastAPI, z.d.). This means that the backend application can still be a FastAPI application, but the server running the application must be started with certificate and key files.

The test was performed locally instead of directly on the shared Raspberry Pi deployment. This was a deliberate choice because the Raspberry Pi backend is used by the team, ESP32 devices and dashboard. Changing the shared deployment during the final sprint could create unnecessary risk. Therefore, the HTTPS/TLS prototype was implemented as a separate local test service.

The purpose of this realisation is not to create a production-ready HTTPS deployment. The purpose is to prove that the FastAPI backend can respond correctly over HTTPS/TLS while the original HTTP backend remains available.


## 2. Main- and Sub-Questions

### Main Question

How can HTTPS/TLS be tested on the existing City Sim backend without disrupting the current Docker-based deployment?

### Sub-Questions

1. How can a self-signed certificate be used to test HTTPS/TLS locally?
2. How can the HTTPS/TLS prototype be added without replacing the existing HTTP backend?
3. How can the implementation be tested so another student can reproduce the same result?

## 3. Implementation Context

The City Sim backend already runs with Docker Compose. The existing backend was kept available on:

```text
http://localhost:80
```

The local HTTPS/TLS prototype was tested separately on:

```text
https://127.0.0.1:8443
```

This means that the normal HTTP backend and the HTTPS prototype could run at the same time.

This approach was chosen for three reasons:

1. The existing HTTP backend should not be replaced during the test.
2. The HTTPS prototype should be isolated from the shared team deployment.
3. Another student should be able to reproduce the test locally.

Docker Compose was used because it can build, create and start containers for the application services (Docker, z.d.-a). The container status was checked with `docker compose ps`, because this command shows which Compose services are currently running (Docker, z.d.-b).


## 4. Step-by-Step Implementation

### 4.1 Step 1 — Open a terminal in the backend project

The first step is to open a terminal in the backend folder of the City Sim project.

Example in PowerShell:

```powershell
cd "C:\path\to\city-sim-project\backend"
```

The exact path depends on where the project is stored locally.

This step is needed because the Docker Compose file and backend configuration are located inside the backend project folder. Running the commands from the correct folder ensures that Docker Compose uses the correct configuration.

### 4.2 Step 2 — Create a certificate folder

A folder was created to store the local certificate files:

```powershell
mkdir certs
```

This creates a folder called:

```text
certs/
```

This folder is used to store the certificate and private key that are needed for HTTPS/TLS.

Expected result:

```text
A certs folder exists inside the backend project.
```

The folder should look like this after the certificate files are created:

```text
certs/
  cert.pem
  key.pem
```

![](/docs/Betül/images/dircerts.png)

Figure 1. Local certificate directory used during the HTTPS/TLS prototype test. The screenshot shows the generated `cert.pem` and `key.pem` files, which were required to start the FastAPI backend with SSL enabled.

### 4.3 Step 3 — Generate a self-signed certificate and private key

For this local prototype, a self-signed certificate was used. This is suitable for local testing because the goal is only to test whether HTTPS/TLS works technically. It is not meant as a trusted production certificate.

OpenSSL can be used to create certificate requests and self-signed certificates (OpenSSL, z.d.). The following command creates both the certificate and the private key:

```powershell
openssl req -x509 -newkey rsa:4096 -keyout certs/key.pem -out certs/cert.pem -days 365 -nodes
```

Explanation of the command:

| Part                    | Meaning                                                                                     |
| ----------------------- | ------------------------------------------------------------------------------------------- |
| `openssl req`           | Starts the OpenSSL certificate request command                                              |
| `-x509`                 | Creates a self-signed certificate                                                           |
| `-newkey rsa:4096`      | Creates a new RSA private key                                                               |
| `-keyout certs/key.pem` | Saves the private key in the certs folder                                                   |
| `-out certs/cert.pem`   | Saves the certificate in the certs folder                                                   |
| `-days 365`             | Makes the certificate valid for 365 days                                                    |
| `-nodes`                | Creates the private key without a password, so the container can start without manual input |

During this command, OpenSSL may ask for information such as country, organisation or common name. For this local test, these values are not important for the technical result. The important part is that `cert.pem` and `key.pem` are created.

Expected result:

```text
certs/cert.pem exists
certs/key.pem exists
```

Why this step is needed:

Uvicorn needs a certificate file and a private key file to serve the FastAPI application over HTTPS. Without these files, the HTTPS test container cannot start with TLS enabled.

### 4.4 Step 4 — Exclude certificate files from Git

The certificate files were added locally, but they should not be committed to Git. Especially the private key should not be shared in the repository.

The following rule was added to `.gitignore`:

```gitignore
certs/*.pem
```

This means that all `.pem` files inside the `certs` folder are ignored by Git.

Expected result:

```text
Git does not track cert.pem or key.pem.
```

To check this, the following command can be used:

```powershell
git status
```

Expected result:

```text
certs/cert.pem and certs/key.pem are not shown as files to commit.
```

Why this step is needed:

The HTTPS prototype needs certificate files locally, but private keys should not be stored in the shared project repository. This keeps the test setup safer and avoids accidentally sharing secret key material.

![](/docs/Betül/images/gitignore.png)

Figure 2. Git ignore configuration for certificate files. The screenshot shows that all `.pem` files inside the `certs` directory were excluded from version control.

### 4.5 Step 5 — Add a separate HTTPS test service to Docker Compose

The existing HTTP backend was not replaced. Instead, a separate HTTPS test service was added to the Docker Compose configuration.

The HTTPS test service starts the same FastAPI backend, but with Uvicorn SSL options. Uvicorn supports SSL configuration with options such as `--ssl-keyfile` and `--ssl-certfile` (Uvicorn, z.d.).

The important part of the service is that Uvicorn is started with:

```bash
--ssl-keyfile /certs/key.pem
--ssl-certfile /certs/cert.pem
```

These options tell Uvicorn:

| Option                           | Meaning                                 |
| -------------------------------- | --------------------------------------- |
| `--ssl-keyfile /certs/key.pem`   | Use this private key file for HTTPS/TLS |
| `--ssl-certfile /certs/cert.pem` | Use this certificate file for HTTPS/TLS |

The HTTPS test service uses port `8443`. This keeps it separate from the normal HTTP backend on port `80`.

Expected result:

```text
The normal backend keeps running on http://localhost:80.
The HTTPS test backend runs on https://127.0.0.1:8443.
```

Why this step is needed:

This keeps the HTTPS/TLS prototype isolated. If the HTTPS service does not work, the normal backend is still available. This was important because the project was in the final sprint and the shared backend should not be disrupted.

![](/docs/Betül/images/docker-compose.https-test.png)

Figure 3. Docker Compose configuration for the HTTPS test service. The screenshot shows the separate HTTPS container that was added for the prototype. This service starts Uvicorn with SSL options and runs independently from the existing HTTP backend.

### 4.6 Step 6 — Start the Docker containers

After the HTTPS test service was added, the containers were started with Docker Compose:

```powershell
docker compose up --build
```

This command builds and starts the services from the Docker Compose configuration. Docker explains that `docker compose up` builds, creates, starts and attaches to containers for the configured services (Docker, z.d.-a).

Expected result:

```text
The API container starts.
The database container starts.
The HTTPS test container starts.
```

If the terminal shows that the containers are running without errors, the implementation can be tested.

Why this step is needed:

The HTTPS/TLS prototype runs inside Docker. Therefore, the backend should be tested through the Docker Compose environment and not only as a local Python process.

### 4.7 Step 7 — Check if the containers are running

Before testing the endpoints, the running containers were checked with:

```powershell
docker compose ps
```

Expected result:

```text
The normal API container is running.
The HTTPS test container is running.
The PostgreSQL database container is running.
```

Why this step is needed:

This confirms that the HTTPS test service was added next to the existing backend. It also confirms that the original backend was not replaced.

![](/docs/Betül/images/dockercomposeps.png)

Figure 4. Docker Compose environment during the HTTPS/TLS prototype test. The screenshot shows the normal API container, the separate HTTPS test container and the PostgreSQL database container running at the same time.

## 5. Testing Process and Expected Results

### 5.1 Test 1 — Test the HTTPS health endpoint with curl

The HTTPS endpoint was tested from PowerShell with:

```powershell
curl.exe -k https://127.0.0.1:8443/health
```

The `-k` option is needed because the certificate is self-signed. The operating system does not automatically trust this certificate. Without `-k`, curl may block the request because it cannot verify the certificate.

Expected result:

```json
{"status":"ok"}
```

Actual result:

```json
{"status":"ok"}
```

This result shows that the FastAPI backend responded correctly through HTTPS/TLS.

Why this test matters:

This is the most important functional test. It proves that:

1. The HTTPS test container is running.
2. Uvicorn loaded the certificate and private key correctly.
3. The FastAPI `/health` endpoint can be reached through HTTPS.
4. The backend still returns the expected JSON response.

![](/docs/Betül/images/healthcheck-certificaat-ok.png)

Figure 5. Successful HTTPS/TLS health check using the local test environment. The screenshot shows the `curl -k` command returning `{"status":"ok"}`, confirming that the FastAPI backend responded correctly over HTTPS using the self-signed certificate.

---

### 5.2 Test 2 — Check the HTTPS endpoint in the browser

The HTTPS endpoint was also opened in the browser:

```text
https://127.0.0.1:8443/health
```

Expected result:

```text
The browser shows a certificate warning.
```

The browser showed:

```text
NET::ERR_CERT_AUTHORITY_INVALID
```

This warning is expected because the certificate is self-signed. The browser recognises that HTTPS is being used, but it does not trust the certificate because it was not issued by a trusted certificate authority.

Why this test matters:

This confirms that HTTPS/TLS is active. The warning does not mean the prototype failed. It means the local certificate is not trusted by the browser. For a production deployment, a trusted certificate would be needed. For this prototype, the warning is acceptable because the goal is only to test the technical HTTPS/TLS setup.

![](/docs/Betül/images/healthcheck-certificaat.png)

Figure 6. Browser certificate warning during the HTTPS/TLS prototype test. The screenshot shows the expected `NET::ERR_CERT_AUTHORITY_INVALID` message, which occurs when using a self-signed certificate.

---

### 5.3 Test 3 — Check that the original HTTP backend still works

After testing HTTPS, the original HTTP backend was tested again:

```powershell
curl.exe http://localhost:80/health
```

Expected result:

```json
{"status":"ok"}
```

The result showed that the original HTTP backend was still available.

Why this test matters:

The main requirement of this realisation was that the HTTPS/TLS prototype should not disrupt the existing backend. This test proves that the original backend still worked after adding the HTTPS test service.

![](/docs/Betül/images/healthcheck.png)

Figure 7. Original HTTP backend running alongside the HTTPS prototype. The screenshot shows that the normal backend on `http://localhost:80/health` continued functioning during the test.

## 6. Result Evaluation

The implementation was successful because all expected results were reached.

| Test              | Expected result                                               | Actual result                               | Status |
| ----------------- | ------------------------------------------------------------- | ------------------------------------------- | ------ |
| Container status  | HTTP backend, HTTPS test service and database are running     | Containers were running at the same time    | Passed |
| HTTPS curl test   | `/health` returns `{"status":"ok"}` over HTTPS                | `{"status":"ok"}` was returned              | Passed |
| Browser test      | Browser shows certificate warning for self-signed certificate | `NET::ERR_CERT_AUTHORITY_INVALID` was shown | Passed |
| HTTP backend test | Original backend still responds on HTTP                       | HTTP `/health` still worked                 | Passed |

The most important result is that the HTTPS test service worked without replacing or breaking the existing HTTP backend. This means the prototype achieved its goal.

The browser warning was also expected. Because the certificate was self-signed, the browser did not trust it. This does not make the prototype unsuccessful. It only shows that the prototype is not production-ready yet.

## 7. Conclusion

The local HTTPS/TLS prototype test was successful. A separate HTTPS test service was added to the Docker-based FastAPI backend. The service used a local self-signed certificate and Uvicorn SSL options to serve the backend over HTTPS.

The backend responded correctly through:

```text
https://127.0.0.1:8443/health
```

with the response:

```json
{"status":"ok"}
```

The original HTTP backend also remained available through:

```text
http://localhost:80/health
```

This proves that HTTPS/TLS can technically be added to the Docker-based FastAPI backend without disrupting the existing HTTP backend.

The implementation should be treated as a feasibility prototype, not as a production-ready deployment. The reason is that the prototype uses a self-signed certificate, which causes browser trust warnings. For a future shared deployment, HTTPS/TLS should be added in a controlled team moment with a proper trusted certificate setup.


## 8. References

*About HTTPS - FastAPI*. (z.d.). FastApi. https://fastapi.tiangolo.com/deployment/https/

Docker. (z.d.-a). docker compose up. Docker Documentation. https://docs.docker.com/reference/cli/docker/compose/up/

Docker. (z.d.-b). docker compose ps. Docker Documentation. https://docs.docker.com/reference/cli/docker/compose/ps/

openssl-req - OpenSSL Documentation. (z.d.). https://docs.openssl.org/3.5/man1/openssl-req/

*Security/Server Side TLS*. (z.d.). https://wiki.mozilla.org/Security/Server_Side_TLS

Uvicorn. (z.d.). *Deployment - Uvicorn*. https://uvicorn.dev/deployment/