# Advise Report — HTTPS/TLS Integration for the City Sim Backend

| Document information |                                                                |
| -------------------- | -------------------------------------------------------------- |
| Title                | Advise Report — HTTPS/TLS Integration for the City Sim Backend |
| Author               | Betül Aydin                                                    |
| Date                 | 1 June 2026                                                    |
| Version              | 2                                                              |
| Classification       | Internal                                                       |
| Mayor                | Mats                                                           |
| Company              | Amsterdam University of Applied Sciences                       |
| Learning outcome     | Advise                                                         |
| Sprint               | Sprint 4                                                       |

## Table of Contents

1. [Introduction](#1-introduction)
2. [Main- and Sub-Questions](#2-advice-question-and-sub-questions)
3. [Local HTTPS/TLS Test Setup and Results](#3-local-https/tls-test-setup-and-results)
4. [Recommended Integration Approach](#4-recommended-integration-approach)
5. [Advice to the Backend Team](#5-advice-to-the-backend-team)
6. [Limitations and ESP32 Considerations](#6-limitations-and-esp32-considerations)
7. [Conclusion](#7-conclusion)
8. [References](#8-references)


## 1. Introduction

This advise report gives a recommendation for how HTTPS/TLS should be integrated into the City Sim backend. City Sim is a smart city prototype in which embedded devices, such as ESP32-based traffic lights and sensors, send data to a central backend. The backend receives, validates and stores this data so it can be used for monitoring, dashboard updates and future control decisions.

HTTPS/TLS was investigated because the current embedded-backend communication needs to become more secure and reliable before it can be treated as a more professional smart city system. In the Realisation phase, HTTPS/TLS was tested locally with a self-signed certificate and a separate Docker Compose test setup. The test confirmed that the FastAPI backend can respond over HTTPS (About HTTPS - FastAPI, z.d.).

This advice is mainly written for the backend team, especially the backend infrastructure side. However, it is also relevant for the embedded team, because ESP32 devices must eventually be able to communicate with the backend through the chosen secure connection. The goal of this document is therefore to advise how HTTPS/TLS can be integrated safely without disrupting the current shared Raspberry Pi backend.


## 2. Advice Question and Sub-Questions

This section defines the focus of the advice. The advice is based on the local HTTPS/TLS test and on the fact that the shared backend is actively used by the team, ESP32 devices and dashboard.

### Main Question

What is the best next step for integrating HTTPS/TLS into the shared City Sim backend deployment?

### Sub-Questions

1. What did the local HTTPS/TLS prototype prove?
2. Why should the prototype not be deployed directly to the shared Raspberry Pi?
3. How should HTTPS/TLS be integrated in the future?
4. What should the team consider for ESP32 communication with HTTPS?

## 3. Local HTTPS/TLS Test Setup and Results

This chapter evaluates the local HTTPS/TLS test setup and explains what the result means for the City Sim backend. The purpose of this test was not to replace the shared backend directly, but to prove whether HTTPS/TLS could technically work with the FastAPI backend.

The local HTTPS/TLS prototype was successful. The backend responded correctly through:

```text
https://127.0.0.1:8443/health
```

with the response:

```json
{"status":"ok"}
```
The result of the health check is shown in Figure 1. This figure supports the conclusion that the backend can respond over an encrypted HTTPS connection in the local test environment.


![](/docs/Betül/images/healthcheck-certificaat-ok.png) Figure 1. Successful HTTPS/TLS health check using the local test environment. The screenshot shows that the FastAPI backend responds correctly over an encrypted HTTPS connection.

This proves that HTTPS/TLS can technically be added to the Docker-based FastAPI backend.

The prototype used a self-signed certificate. This is acceptable for local testing, but it is not suitable as a final deployment solution because browsers and devices do not trust self-signed certificates by default.


The prototype was intentionally kept separate from the normal backend, as shown in Figure 2. The main reason for this separation was to prevent any risk of disrupting or breaking the shared Raspberry Pi backend, especially in the final days before the sprint deadline. Running the HTTPS/TLS test in a separate environment ensured that the team’s existing backend remained stable and unaffected.

![](/docs/Betül/images/dockercomposeps.png)
Figure 2. Docker Compose environment used during testing. The screenshot shows the original backend services running alongside the separate HTTPS test container and database.

## 4. Recommended Integration Approach

The local HTTPS/TLS prototype should not be deployed directly as the final solution. It proves technical feasibility, but the final integration should be handled through the existing backend architecture in a controlled way.

For future development or handover, HTTPS/TLS should preferably be added through the existing NGINX-based backend stack (NGINX Reverse Proxy | NGINX Documentation, 2026). During the project, a teammate created a clustered backend setup with NGINX in front of multiple API replicas. This stack is mainly focused on reliability, load balancing and recovery behaviour. Because NGINX already acts as the entry point in that setup, it is the most suitable place to handle HTTPS/TLS termination.

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

Figure 4 shows how NGINX forwards incoming requests to the backend API containers. This supports the recommendation to let NGINX handle HTTPS externally while the API containers communicate internally through the Docker network.


![](/docs/Betül/images/nginx.conf.png)
Figure 4. NGINX reverse proxy configuration. The screenshot shows how requests are forwarded from NGINX to the backend API containers within the Docker network.


## 5. Advice to the Backend Team

My advice to the backend team is to keep the current Raspberry Pi backend stable for the final delivery and not replace it with the local HTTPS/TLS prototype. The local prototype should be used as evidence that HTTPS/TLS is technically feasible, not as the final deployment architecture.

For future development or handover, HTTPS/TLS should be added through the existing NGINX-based stack. Private keys and certificate files should stay out of Git, and deployment changes should be coordinated with the backend infrastructure owner before changing the shared Raspberry Pi setup.

This approach avoids unnecessary risk close to the final presentation. The local prototype still provides useful evidence, while the shared backend remains stable for the final delivery.


## 6. Limitations and ESP32 Considerations

The local HTTPS/TLS prototype has some limitations:

| **Limitation**                 | Explanation                                                                                                                                                                                                   |
| ------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Local test only                | The test was performed locally, not on the Raspberry Pi.                                                                                                                                                      |
| Self-signed certificate        | The certificate is not trusted by browsers or devices by default.                                                                                                                                             |
| Not final architecture         | Direct HTTPS testing with Uvicorn is useful for proof of concept, but NGINX is preferred for shared deployment.                                                                                               |
| Team coordination needed       | The Raspberry Pi backend is shared, so deployment changes must be planned with the backend and embedded teams.                                                                                                |
| ESP32 limitations              | ESP32 devices have limited memory and processing power compared to normal computers. HTTPS/TLS can require certificate handling, extra memory and a TLS handshake before data is sent.                        |
| ESP32 compatibility testing    | The embedded team should test whether the ESP32 devices can connect reliably to the HTTPS endpoint before HTTPS becomes mandatory.                                                                            |
| Temporary HTTP and HTTPS ports | Running HTTP and HTTPS on separate ports could be used temporarily during testing. This would allow backend/browser testing over HTTPS while ESP32 compatibility is checked step by step.                     |
| Not a permanent solution       | Running HTTP and HTTPS side by side should only be temporary. The final direction should still be secure communication, preferably through HTTPS/TLS via NGINX, if the ESP32 devices can support it reliably. |

These limitations do not make the prototype invalid. They show that the prototype should be used as evidence of feasibility, while the final deployment should be handled carefully with both backend and embedded teams involved.

## 7. Conclusion

The local HTTPS/TLS prototype proved that encrypted communication can be added to the Docker-based FastAPI backend. However, it should not be deployed directly to the shared Raspberry Pi environment shortly before the final delivery, because the active backend is used by the team, ESP32 devices and dashboard.

The recommended approach is to keep the current Raspberry Pi deployment stable and document HTTPS/TLS as a future improvement. If the project is continued, HTTPS/TLS should preferably be added through the existing NGINX-based backend stack. This is safer and cleaner than configuring HTTPS directly inside every FastAPI container, because NGINX can handle external secure traffic while the FastAPI services continue running internally.

Before HTTPS/TLS becomes mandatory, the team should also test ESP32 compatibility. Certificate handling, memory usage and connection overhead may influence whether ESP32 devices can use the secure endpoint reliably. This advice keeps the final delivery stable while still giving the team a clear technical direction for improving backend security in future development.


## 8. References

*About HTTPS - FastAPI*. (z.d.). FastApi. https://fastapi.tiangolo.com/deployment/https/

NGINX Reverse Proxy | NGINX Documentation. (2026, 4 mei). https://docs.nginx.com/nginx/admin-guide/web-server/reverse-proxy/ 

Uvicorn. (z.d.). *Deployment - Uvicorn*. https://uvicorn.dev/deployment/

HTTPS Server - ESP32 -  — ESP-IDF Programming Guide v6.0.1 documentation. (z.d.). https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/esp_https_server.html