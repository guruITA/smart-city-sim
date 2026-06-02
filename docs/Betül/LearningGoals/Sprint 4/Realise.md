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
