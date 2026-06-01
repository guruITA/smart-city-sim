# Design Report — Improving the City Sim Backend Deployment with Docker Containers and HTTPS/TLS

| Document information | |
|---|---|
| Title | Design Report — Improving the City Sim Backend Deployment with Docker Containers and HTTPS/TLS |
| Author | Betül Aydin |
| Date | [25 May 2026] |
| Version | 1.0 |
| Classification | Internal |
| Mayor | Mats |
| Company | Amsterdam University of Applied Sciences |
| Learning outcome | Design |
| Sprint | Sprint 4 |


## Table of Contents

1. Introduction  
2. Design Question  
3. Design Requirements  
4. Current Backend Situation  
5. Proposed Security Design  
6. HTTPS/TLS Design  
7. Supporting Reliability Design Choices 
8. Scope and Limitations  
9. Conclusion  
10. References  


## 1. Introduction

This design report describes how secure communication can be improved for the existing City Sim backend deployment. The design is based on the risks and requirements identified in the Analysis report, especially the risks related to unencrypted HTTP communication, exposed backend access and backend reliability.

The current project already contains a Docker Compose setup where the FastAPI backend and PostgreSQL database run as separate services. A health check has also already been added by a teammate. Because of this, this design does not focus on redesigning the Docker container structure from scratch. Instead, the main focus is on designing how HTTPS/TLS can be added to the existing backend deployment in a realistic way.

Docker-related reliability choices, such as persistent storage, restart behaviour, health check usage and secret management, are included as supporting design concerns. They are important because secure communication is only useful if the backend also remains reachable, maintainable and recoverable.

## 2. Design Question and Sub-Questions

### Main Question

How should HTTPS/TLS be designed for the existing City Sim backend deployment to improve secure communication, while keeping the current Docker-based setup reliable and realistic for the sprint?


### Sub-Questions

1. How can HTTPS/TLS improve communication security for the existing City Sim backend?
2. Which HTTPS/TLS approach fits the current Raspberry Pi and Docker-based deployment best?
3. Which existing Docker reliability features should be verified or documented to support the HTTPS/TLS design?


## 3. Design Requirements

The design uses the requirements from the Analysis report, but the focus is narrowed to what fits the current sprint. Since the API and database containers already exist, the main design focus is secure communication with HTTPS/TLS. Docker-related requirements are treated as supporting reliability checks, not as a full redesign of the container structure.

| ID | Requirement | Priority | Design response |
| --- | --- | --- | --- |
| R1 | Transport encryption | Must | Design an HTTPS/TLS approach for the existing backend deployment. |
| R2 | Secret management | Must | Keep credentials and certificate-related values out of source code. |
| R3 | Network exposure | Must | Expose only necessary backend access and avoid direct database exposure. |
| R4 | Input validation | Should | Keep or use FastAPI/Pydantic validation for incoming payloads. |
| R5 | Health check usage | Should | Use the existing health check to verify backend availability. |
| R6 | Persistent storage | Should | Verify that PostgreSQL data is stored in a persistent Docker volume. |
| R7 | Restart behaviour | Should | Verify or recommend Docker restart policies. |


## 4. Current Backend Situation

The current City Sim backend already has a Docker-based deployment. The FastAPI backend and PostgreSQL database run as separate Docker services. This means the most important container separation is already present.

A simplified version of the current structure is:

```
ESP32 devices / browser
        ↓ HTTP
FastAPI backend container
        ↓ internal Docker network
PostgreSQL database container
```

The current setup is functional, but the Analysis showed that communication over HTTP is a security risk because data is not encrypted in transit. This can make the system more vulnerable to eavesdropping or Man-in-the-Middle attacks.

The current Docker setup also supports reliability because the API and database are already separated. However, this design still includes reliability checks as supporting concerns. These include health check usage, persistent storage, restart behaviour, secret management and network isolation.


## 5. Proposed Security Design

The proposed design focuses on adding secure communication to the existing City Sim backend deployment. The current API and database containers already provide the basic Docker service separation, so the design does not add unnecessary extra backend services.

The current communication flow is:

```
ESP32 devices / browser
        ↓ HTTP
FastAPI backend container
        ↓ internal Docker network
PostgreSQL database container
```

The target communication flow is:

```
ESP32 devices / browser
        ↓ HTTPS/TLS where feasible
FastAPI backend container
        ↓ internal Docker network
PostgreSQL database container
```

This target design improves the security of communication between clients and the backend while keeping the existing Docker deployment structure.

If HTTPS/TLS cannot be configured directly on the backend in a clean way, a reverse proxy can be considered as a future extension:

```
ESP32 devices / browser
        ↓ HTTPS
Reverse proxy
        ↓ internal Docker network
FastAPI backend container
        ↓ internal Docker network
PostgreSQL database container
```

The reverse proxy option is more production-like because it separates TLS handling from the application code. However, it is not required as a full implementation for this sprint. For this sprint, the design priority is to define the HTTPS/TLS approach and implement or document it where feasible.

## 6. HTTPS/TLS Design

HTTPS/TLS is the main security improvement in this design. The current backend communication uses HTTP, which does not encrypt data in transit. HTTPS/TLS reduces the risk of eavesdropping and Man-in-the-Middle attacks by encrypting the connection between clients and the backend.

### 6.1 Direct HTTPS/TLS Option

The first option is to configure HTTPS/TLS directly for the backend endpoint. This keeps the design simple and avoids adding another container. This option may be suitable for a prototype if certificate setup is manageable within the current Raspberry Pi and network environment.

The limitation is that certificate handling becomes part of the backend deployment. This may be less clean than using a reverse proxy, but it can be more realistic for the sprint because it avoids adding another service.

### 6.2 Reverse Proxy Option

The second option is to place a reverse proxy, such as Nginx or Caddy, in front of the FastAPI backend. The reverse proxy handles HTTPS/TLS and forwards requests to the FastAPI container over the internal Docker network.

This is a cleaner production-like design because TLS is separated from the application code. However, it also adds extra configuration and may be too large for the current sprint.

### 6.3 Selected Sprint Design

For this sprint, the selected design is to treat HTTPS/TLS as the main security improvement and implement it only if feasible. The design first documents the direct HTTPS/TLS option because it is simpler and closer to the current setup.

The reverse proxy option is included as the recommended future extension if the team wants a cleaner production-like setup later. This keeps the design realistic while still directly addressing the main security risk from the Analysis report: unencrypted backend communication.

If full HTTPS/TLS implementation is not feasible during the sprint, the remaining steps should be documented in the Advise document.

## 7. Supporting Reliability Design Choices

The focus of this design is HTTPS/TLS, but reliability still matters because secure communication is only useful when the backend remains available and maintainable. The Docker setup already separates the API and database, so this section describes which existing reliability features should support the security design.

### 7.1 Health Check Usage

A health check has already been added by a teammate. This design uses the health check as a supporting reliability feature because it gives the team a simple way to verify backend availability.

Example test:

```bash
curl http://<backend-url>/health
```

During Realisation, this health check can be tested and documented as evidence that the backend is reachable.

### 7.2 Persistent Storage

The design requires PostgreSQL data to be stored in a persistent Docker volume. This prevents the database from depending only on the temporary container filesystem and reduces the risk of data loss when containers are removed, recreated or rebuilt.

This is not the main security improvement, but it supports reliability because sensor data and system state should survive container lifecycle events.

### 7.3 Restart Behaviour

Important backend services should restart after crashes or Raspberry Pi reboot where possible. This can be supported with Docker Compose restart policies such as:

```
restart: unless-stopped
```

or:

```
restart: always
```

Restart behaviour is included as a supporting reliability measure. It helps the backend recover from simple service failures without requiring immediate manual intervention.

### 7.4 Secret Management

Secrets such as database passwords, API tokens or certificate-related values should not be hardcoded in the source code or committed to Git. They should be loaded from environment variables.

The project should use:

- `.env` for real local or deployment values;
- `.env.example` as a safe template;
- `.gitignore` to prevent real secrets from being committed.

This supports the HTTPS/TLS design because certificate paths, tokens or other sensitive configuration values should also be managed safely.

### 7.5 Network Isolation

The database should remain internal to the Docker network. External clients should communicate with the API, not directly with PostgreSQL.

This reduces unnecessary exposure and supports a cleaner backend architecture. It also ensures that adding HTTPS/TLS focuses on securing the public backend access, while internal database communication remains private inside the Docker network.


## 8. Scope and Limitations

This design focuses on improving secure communication for the existing backend deployment. It does not redesign the Docker Compose setup from scratch because the API and database containers already exist.

### In Scope

- designing an HTTPS/TLS approach for the existing backend;
- comparing direct HTTPS/TLS with a reverse proxy option;
- choosing a realistic sprint approach for HTTPS/TLS;
- using the existing Docker API/database separation as the baseline;
- including the existing health check as a supporting reliability feature;
- describing persistent storage as a supporting reliability requirement;
- describing restart behaviour as a supporting reliability requirement;
- describing secret management for credentials and certificate-related values;
- explaining network isolation between the API and database.

### Out of Scope

- recreating the existing Docker Compose setup from scratch;
- claiming ownership of Docker Compose work already completed by a teammate;
- claiming ownership of the existing health check implementation;
- adding unnecessary extra containers only for the sake of splitting services;
- fully implementing production HTTPS/TLS if certificate setup is not feasible;
- fully implementing a reverse proxy if it becomes too large for the sprint;
- designing or implementing the backup and recovery plan, because this was already handled by a teammate during the sprint;
- implementing Kubernetes;
- implementing cloud fallback;
- implementing a full monitoring stack;
- implementing hardware-backed security;
- implementing signed firmware updates.

### Team Responsibility Note

The Docker Compose setup, API/database separation, health check and backup/recovery plan were already handled within the team. This design builds on those existing team contributions and focuses on secure communication through HTTPS/TLS, supported by reliability checks such as health check usage, persistent storage, restart behaviour, secret management and network isolation.

## 9. Conclusion

This design focuses on improving the security of the existing City Sim backend deployment by designing an HTTPS/TLS approach for backend communication. Since the API and database already run as separate Docker services, the design does not recreate the Docker structure from scratch.

The main design decision is to protect communication in transit with HTTPS/TLS where feasible. A direct HTTPS/TLS setup is the simpler sprint option, while a reverse proxy is described as a cleaner future extension. Supporting reliability concerns, such as health check usage, persistent storage, restart behaviour and secret management, are included because they help keep the existing backend maintainable and recoverable.

The next Realisation phase should focus on testing what is feasible within the sprint: verifying the existing health check, checking secrets and storage, documenting the current deployment and attempting or documenting the HTTPS/TLS setup.

## 10. References

[My Analyse report](Analyse.md)

Kirkland, W. (2026, 1 mei). The essential guide to SSL/TLS security & certificate automation (2026). Urllo. https://www.urllo.com/resources/learn/ssl-tls-security-guide

Pasemko, S. (2026, 1 mei). Redirect HTTP to HTTPS: complete setup guide. Urllo. https://www.urllo.com/resources/learn/redirect-http-to-https
