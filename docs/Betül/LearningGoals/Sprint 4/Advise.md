# Advise Report — HTTPS/TLS Integration for the City Sim Backend

| Document information |                                                                |
| -------------------- | -------------------------------------------------------------- |
| Title                | Advise Report — HTTPS/TLS Integration for the City Sim Backend |
| Author               | Betül Aydin                                                    |
| Date                 | 1 June 2026                                                    |
| Version              | 1.0                                                            |
| Classification       | Internal                                                       |
| Mayor                | Mats                                                           |
| Company              | Amsterdam University of Applied Sciences                       |
| Learning outcome     | Advise                                                         |
| Sprint               | Sprint 4                                                       |

## Table of Contents

1. [Introduction](#1-introduction)
2. [Main- and Sub-Questions](#2-advice-question-and-sub-questions)
3. [Evaluation of the Realisation](#3-evaluation-of-the-realisation)
4. [Recommended Integration Approach](#4-recommended-integration-approach)
5. [Advice to the Backend Team](#5-advice-to-the-backend-team)
6. [Limitations](#6-limitations)
7. [Conclusion](#7-conclusion)
8. [References](#8-references)


## 1. Introduction

This advise report gives a recommendation for how HTTPS/TLS should be integrated into the City Sim backend after the local HTTPS/TLS prototype test.

In the Realisation phase, HTTPS/TLS was tested locally with a self-signed certificate and a separate Docker Compose test setup. The test confirmed that the FastAPI backend can respond through (About HTTPS - FastAPI, z.d.) HTTPS. However, the prototype was not deployed to the shared Raspberry Pi environment, because that backend is actively used by the team, ESP32 devices and dashboard.

This advice is mainly written for the backend team, especially the backend infrastructure side, because future HTTPS/TLS integration should be done in a controlled way without breaking the current deployment.


## 2. Advice Question and Sub-Questions

### Main Question

What is the best next step for integrating HTTPS/TLS into the shared City Sim backend deployment?

### Sub-Questions

1. What did the local HTTPS/TLS prototype prove?
2. Why should the prototype not be deployed directly to the shared Raspberry Pi?
3. How should HTTPS/TLS be integrated in the future?

## 3. Evaluation of the Realisation

The local HTTPS/TLS prototype was successful. The backend responded correctly through:

```text
https://127.0.0.1:8443/health
```

with the response:

```json
{"status":"ok"}
```

This proves that HTTPS/TLS can technically be added to the Docker-based FastAPI backend.

The prototype used a self-signed certificate. This is acceptable for local testing, but it is not suitable as a final deployment solution because browsers and devices do not trust self-signed certificates by default.


![](/docs/Betül/images/healthcheck-certificaat-ok.png)
Figure 1. Successful HTTPS/TLS health check using the local test environment. The screenshot shows that the FastAPI backend responds correctly over an encrypted HTTPS connection.


The prototype was also intentionally kept separate from the normal backend. This was a good decision because the existing backend was not replaced or disrupted.

![](/docs/Betül/images/dockercomposeps.png)
Figure 2. Docker Compose environment used during testing. The screenshot shows the original backend services running alongside the separate HTTPS test container and database.

## 4. Recommended Integration Approach

The local HTTPS/TLS prototype should not be deployed directly as the final solution. For future development or handover, HTTPS/TLS should preferably be added through the existing NGINX-based backend stack. (NGINX Reverse Proxy | NGINX Documentation, 2026).

During the sprint, a teammate created a clustered backend setup with NGINX in front of multiple API replicas. This stack is mainly focused on reliability, load balancing and recovery behaviour. Because NGINX already acts as the entry point in that setup, it is the most suitable place to handle HTTPS/TLS termination (NGINX Reverse Proxy | NGINX Documentation, 2026).

The recommended future architecture is:

```text
ESP32 devices / browser
        ↓ HTTPS
NGINX reverse proxy
        ↓ internal Docker network
FastAPI API replicas
        ↓ internal Docker network
PostgreSQL database
```

In this structure, NGINX handles the secure external connection, while the FastAPI services continue to run internally. This separates certificate management from the application code and fits better with the team’s clustered backend design. (NGINX Reverse Proxy | NGINX Documentation, 2026)

![](/docs/Betül/images/docker-compose-cluster.png)
Figure 3. Clustered backend configuration. The screenshot shows the NGINX service defined in the Docker Compose cluster setup, acting as the entry point for incoming traffic.


![](/docs/Betül/images/nginx.conf.png)
Figure 4. NGINX reverse proxy configuration. The screenshot shows how requests are forwarded from NGINX to the backend API containers within the Docker network.


## 5. Advice to the Backend Team

My advice to the backend team is:

1. Keep the current Raspberry Pi backend stable until the final product delivery.
2. Do not replace the shared backend with the local HTTPS/TLS prototype.
3. Use the local prototype as proof that HTTPS/TLS is technically feasible.
4. Treat HTTPS/TLS on the Raspberry Pi as a future development or handover recommendation.
5. Add HTTPS/TLS through the existing NGINX-based stack if the project is continued.
6. Keep private keys and certificate files out of Git.
7. Coordinate any deployment changes with the backend infrastructure owner before changing the shared Raspberry Pi setup.

This approach avoids unnecessary risk close to the final presentation. The local prototype still provides useful evidence, while the shared backend remains stable for the final delivery.


## 6. Limitations

The local HTTPS/TLS prototype has some limitations:

| Limitation               | Explanation                                                                                                     |
| ------------------------ | --------------------------------------------------------------------------------------------------------------- |
| Local test only          | The test was performed locally, not on the Raspberry Pi.                                                        |
| Self-signed certificate  | The certificate is not trusted by browsers or devices by default.                                               |
| Not final architecture   | Direct HTTPS testing with Uvicorn is useful for proof of concept, but NGINX is preferred for shared deployment. |
| Team coordination needed | The Raspberry Pi backend is shared, so deployment changes must be planned with the team.                        |

These limitations do not make the prototype invalid. They show that the prototype should be used as evidence of feasibility, while the final deployment should be handled more carefully.

## 7. Conclusion

The local HTTPS/TLS prototype proved that encrypted communication can be added to the Docker-based FastAPI backend. However, it should not be deployed directly to the shared Raspberry Pi environment shortly before the final delivery, because the active backend is used by the team, ESP32 devices and dashboard.

The recommended approach is to keep the current Raspberry Pi deployment stable and document HTTPS/TLS as a future improvement. If the project is continued, HTTPS/TLS should preferably be added through the existing NGINX-based backend stack. This is safer and cleaner than configuring HTTPS directly inside every FastAPI container, because NGINX can handle external secure traffic while the FastAPI services continue running internally.

This advice keeps the final sprint delivery stable while still giving the team a clear technical direction for improving backend security in future development.


## 8. References

*About HTTPS - FastAPI*. (z.d.). FastApi. https://fastapi.tiangolo.com/deployment/https/

NGINX Reverse Proxy | NGINX Documentation. (2026, 4 mei). https://docs.nginx.com/nginx/admin-guide/web-server/reverse-proxy/ 

Uvicorn. (z.d.). *Deployment - Uvicorn*. https://uvicorn.dev/deployment/
