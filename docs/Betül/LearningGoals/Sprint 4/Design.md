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
| R8 | Backup and recovery | Could | Include backup and recovery as team-level recommendations. |


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


## 5. Proposed Backend Design


### 5.1 Reverse Proxy Container


### 5.2 FastAPI Backend Container


### 5.3 PostgreSQL Database Container

## 6. HTTPS/TLS Design

[Explain HTTPS/TLS shortly.]

## 7. Reliability Design Choices

### 7.1 Persistent Storage


### 7.2 Restart Policies

### 7.3 Health Checks

### 7.4 Deployment Documentation

Deployment steps should be documented clearly. This includes:

- starting the backend;
- stopping the backend;
- checking logs;
- testing the health endpoint;
- redeploying after code changes;
- basic recovery steps.

## 8. Scope and Limitations

### In scope

### Out of scope / future work


## 9. Conclusion

## 10. References

[Use sources from your Analysis.]