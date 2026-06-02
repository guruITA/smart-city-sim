# Realisation Report — Local HTTPS/TLS Prototype Test for the City Sim Backend

| Document information |  |
| --- | --- |
| Title | Realisation Report — Local HTTPS/TLS Prototype Test for the City Sim Backend |
| Author | Betül Aydin |
| Date | 1 June 2026 |
| Version | 1.0 |
| Classification | Internal |
| Mayor | Mats |
| Company | Amsterdam University of Applied Sciences |
| Learning outcome | Realise |
| Sprint | Sprint 4 |


## Table of Contents

1. Introduction
2. Main- and Sub-Questions
3. Implementation Context
4. HTTPS/TLS Prototype Setup
5. Test Results
6. Conclusion
7. References


## 1. Introduction

This realisation report describes a local HTTPS/TLS prototype test for the City Sim backend. The goal was to test whether encrypted HTTPS communication can be added to the existing Docker-based FastAPI backend without disrupting the current working HTTP backend.

The test was performed locally instead of directly on the shared Raspberry Pi deployment. This was a deliberate scope decision, because the Raspberry Pi backend is used by the team, ESP32 devices and dashboard. Changing the shared deployment during the final sprint could create unnecessary risk.


## 2. Main- and Sub-Questions

### Main Question

How can HTTPS/TLS be tested on the existing City Sim backend without disrupting the current Docker-based deployment?

### Sub-Questions

1. How can a self-signed certificate be used to test HTTPS/TLS locally?
2. How can the HTTPS/TLS prototype be added without replacing the existing HTTP backend?
3. What test results show that the HTTPS/TLS prototype works?

## 3. Implementation Context

The existing backend already runs with Docker Compose. The normal HTTP backend remained available during the test, while HTTPS/TLS was tested through a separate local prototype setup (Uvicorn, z.d.).

The existing backend stayed available on:

```
http://localhost:80
```

The local HTTPS/TLS prototype was tested on:

```
https://127.0.0.1:8443
```

This approach made it possible to test HTTPS/TLS safely without changing the active backend flow 
(*About HTTPS - FastAPI*, z.d.).


## 4. HTTPS/TLS Prototype Setup

A separate Docker Compose HTTPS test setup was created. The existing HTTP backend was not replaced.

The HTTPS prototype used:

- a self-signed certificate (*Security/Server Side TLS*, z.d.);
- a private key;
- a separate HTTPS test service;
- Uvicorn SSL options (Uvicorn, z.d.);
- port `8443` for the local HTTPS test.

The certificate files were stored locally:

```
certs/cert.pem
certs/key.pem
```

The private key was excluded from Git using:

```
certs/*.pem
```

![](/docs/Betül/images/dircerts.png)

![](/docs/Betül/images/gitignore.png)

The HTTPS test service starts Uvicorn with:

```bash
--ssl-keyfile /certs/key.pem
--ssl-certfile /certs/cert.pem
```

This allowed the FastAPI backend to serve HTTPS traffic during the prototype test 
(*About HTTPS - FastAPI*, z.d.).


![](/docs/Betül/images/docker-compose.https-test.png)

## 5. Test Results

### 5.1 Container status

Before testing the HTTPS endpoint, the running containers were checked with:

```powershell
docker compose ps
```
![](/docs/Betül/images/dockercomposeps.png)

The output shows that the normal API container, the HTTPS test container and the database container were running at the same time. This confirms that the HTTPS prototype was added alongside the existing backend instead of replacing it.


### 5.2 Health check
The HTTPS endpoint was tested with:

```powershell
curl.exe -k https://127.0.0.1:8443/health
```

The result was:

```json
{"status":"ok"}
```

The `-k` option was required because the certificate was self-signed and therefore not trusted by the operating system (*Security/Server Side TLS*, z.d.).

![](/docs/Betül/images/healthcheck-certificaat-ok.png)

The browser also showed the following certificate warning:

```
NET::ERR_CERT_AUTHORITY_INVALID
```

This warning is expected for a self-signed certificate. It confirms that HTTPS/TLS was active, but that the certificate was not trusted by an official certificate authority 
(*About HTTPS - FastAPI*, z.d.).


![](/docs/Betül/images/healthcheck-certificaat.png)


The original backend was kept available during the prototype test. This shows that the HTTPS/TLS test did not replace or break the normal HTTP backend.


![](/docs/Betül/images/healthcheck.png)

## 6. Conclusion

The local HTTPS/TLS prototype test was successful. The backend responded correctly through:

```
https://127.0.0.1:8443/health
```

with the response:

```json
{"status":"ok"}
```

This proves that HTTPS/TLS can technically be added to the Docker-based FastAPI backend (Uvicorn, z.d.).

Because the prototype uses a self-signed certificate, it should be treated as a feasibility test and not as a production-ready HTTPS deployment. The prototype was not deployed to the shared Raspberry Pi environment. This was a deliberate decision to avoid disrupting the active team backend during the final sprint. For the shared deployment, HTTPS/TLS should be added later in a controlled team moment, preferably through the existing NGINX-based backend stack (*Security/Server Side TLS*, z.d.).


## 7. References

*About HTTPS - FastAPI*. (z.d.). FastApi. https://fastapi.tiangolo.com/deployment/https/

*Security/Server Side TLS*. (z.d.). https://wiki.mozilla.org/Security/Server_Side_TLS

Uvicorn. (z.d.). *Deployment - Uvicorn*. https://uvicorn.dev/deployment/
