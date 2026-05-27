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
5. Proposed Backend Design  
6. HTTPS/TLS Design  
7. Reliability Design Choices  
8. Scope and Limitations  
9. Conclusion  
10. References  


## 1. Introduction

[Explain shortly what this document is about.]

## 2. Design Question and Sub-Questions

### Main Question

How should the City Sim backend be designed with Docker containers and HTTPS/TLS to improve security and reliability?

### Sub-Questions

1. How should the backend services be structured with Docker containers?
2. How can HTTPS/TLS improve backend communication security?
3. Which reliability measures should be included in the backend design?

## 3. Design Requirements
| **ID** | **Requirement** | **Priority** | **How the design responds** |
| --- | --- | --- | --- |
| **R1** | **Input validation** | Must | Use FastAPI/Pydantic schemas for incoming payloads to prevent **malformed or invalid data**. |
| **R2** | **Secret management** | Must | Use environment variables and keep real **`.env`** files out of Git. |
| **R3** | **Persistent storage** | Must | Store PostgreSQL data in a **Docker volume** to reduce data loss risks during container restarts. |
| **R4** | **Basic documentation** | Must | Document steps for starting, stopping, checking logs, and basic recovery. |
| **R5** | **Transport encryption** | Should | Add **HTTPS/TLS** through a reverse proxy where feasible to protect data in transit. |
| **R6** | **Service recovery** | Should | Use **restart policies** and health checks to **recover automatically where possible**. |
| **R7** | **Network isolation** | Should | Keep the database internal to the Docker network, exposing only the reverse proxy. |

## 4. Current Backend Situation

The current City Sim backend runs on a Raspberry Pi using FastAPI and PostgreSQL. While functional, it represents a state where backend services may become unavailable if the main process or hardware fails. Communication currently happens over unencrypted HTTP, making it vulnerable to interception. The backend is already functional, but the deployment can be improved by separating responsibilities more clearly and documenting recovery behaviour.

## 5. Proposed Backend Design

[Describe your improved structure.]

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