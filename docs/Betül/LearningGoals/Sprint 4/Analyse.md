# Analyse

| Document information | |
|---|---|
| Title | Analysis Report — Secure and Reliable Embedded-Backend Communication in Smart City Systems |
| Author | Betül Aydin |
| Date | 1 June 2026 |
| Version | 1.1 |
| Classification | Internal |
| Mayor | Mats |
| Company | Amsterdam University of Applied Sciences |
| Learning outcome | Analysis |
| Sprint | Sprint 4 |

## Table of Contents

1. [Introduction](#1-introduction)  
2. [Research Question and Sub-Questions](#2-research-question-and-sub-questions)  
3. [Embedded-Backend Communication in Smart City Systems](#3-embedded-backend-communication-in-smart-city-systems)  
4. [Security Risks in Embedded-Backend Communication](#4-security-risks-in-embedded-backend-communication)  
5. [Reliability Risks in Embedded-Backend Communication](#5-reliability-risks-in-embedded-backend-communication)  
6. [Requirements for Secure and Reliable Embedded-Backend Communication](#6-requirements-for-secure-and-reliable-embedded-backend-communication)  
7. [Conclusion](#7-conclusion)  
8. [References](#8-references)

## 1. Introduction

This analysis report investigates how embedded-backend communication in smart city systems can be made more secure and reliable. In these systems, embedded devices such as sensors, traffic lights, streetlights and parking systems collect data from the physical environment and send it to backend systems for validation, storage, monitoring and future control decisions.

As IoT systems move from prototypes toward real-world use, a working prototype is not automatically secure or reliable enough for professional city services (Zhou et al., 2023; Solouki et al., 2024). Unencrypted communication, manipulated requests or backend failure can affect data quality, monitoring and future control functions.

This is relevant to the City Sim project because the prototype uses embedded devices, a central backend, a Raspberry Pi and Docker-based backend services. Therefore, the project benefits from professional design choices such as secure communication, service separation, persistent storage and recovery planning.

This report explains the communication structure, analyses the main security and reliability risks, and translates these risks into requirements for the Design phase.

## 2. Research Question and Sub-Questions

This section defines the focus of the analysis. The main question describes the overall problem, while the sub-questions divide the analysis into communication structure, security risks, reliability risks and practical requirements.

### Main Question

**How can embedded-backend communication in smart city systems be made secure and reliable?**

### Sub-Questions

1. **How does embedded-backend communication work in smart city systems?**
2. **What security risks can occur when embedded devices communicate with backend systems?**
3. **What reliability risks can occur when embedded devices depend on backend systems?**
4. **What requirements are needed to make embedded-backend communication secure and reliable?**

## 3. Embedded-Backend Communication in Smart City Systems

This chapter explains the basic communication structure between embedded devices and backend systems. This is needed before analysing risks, because security and reliability problems depend on how data moves through the system and which components process it.

In a smart city system, embedded devices such as ESP32 boards collect sensor data from the physical environment. Because embedded hardware has limited resources, this data is often sent as a lightweight JSON or binary payload. The data is then transmitted over a network such as Wi-Fi, Ethernet or cellular communication. For this project context, HTTPS/TLS is the relevant communication method because it protects data while it is being sent.

The backend receives the request through an API endpoint, validates the incoming data and can apply authentication mechanisms such as API keys or tokens if required (Apriorit, 2025). After validation, a backend service such as FastAPI processes the data and stores it in a database such as PostgreSQL for monitoring or future analysis.

Although different IoT protocols exist, this analysis does not focus on choosing between HTTPS and MQTT. HTTPS has already been chosen as the relevant direction for this project. HTTP/HTTPS is suitable for web APIs, while MQTT is often used for event-driven IoT communication with a publish/subscribe model (Power by akacia, 阿卡希亞(股)公司, www.akacia.com.tw, 2026; Vikram, 2026b). The rest of this analysis therefore focuses on making the chosen HTTPS/TLS direction more secure and reliable.

Backend reliability is also part of embedded-backend communication. A tool such as Docker Compose can support this by separating services into containers, such as the API, database and reverse proxy. Restart policies, health checks and persistent volumes can improve recovery behaviour and reduce data loss during container restarts or recreations (Abdelzaher et al., 2025).

Together, HTTPS/TLS and containerization support secure and reliable communication. HTTPS/TLS protects data in transit, while Docker Compose supports service separation, recovery behaviour and persistent data storage.

## 4. Security Risks in Embedded-Backend Communication

This chapter analyses the main security risks when embedded devices communicate with backend services. These risks matter because smart city systems process operational data and may later support control functions. If communication or backend access is not protected, the system can receive manipulated data, expose sensitive information or become easier to attack (Mktg & Mktg, 2023).

### 4.1 Unencrypted Communication

Using unencrypted protocols, such as standard HTTP or plain MQTT, creates risks for confidentiality and integrity. Standard HTTP sends data without encryption, which means unauthorized actors may be able to read or change the information in transit. This also creates a risk of Man-in-the-Middle attacks (Sowa, 2025).

Even simple sensor values can become sensitive when collected over time, because they can reveal operational patterns. HTTPS/TLS is therefore important because it encrypts data in transit and reduces the risk of interception or modification (Bosch, z.d.).

### 4.2 Fake or Manipulated Data

Backend services that do not validate incoming data are vulnerable to fake or manipulated requests. If an API endpoint is open and unprotected, unauthorized devices can send false sensor readings or status updates (Mktg & Mktg, 2023b). This can pollute historical data and make monitoring or future analysis unreliable.

The backend must therefore handle malformed payloads, invalid data types and unexpected values. Validation with FastAPI and Pydantic schemas can help prevent incorrect data from being processed or stored (Sowa, 2025b).

### 4.3 Weak Authentication or No Authentication

A common IoT risk is that the backend cannot verify whether a connecting device is legitimate. Without authentication, another device could pretend to be a sensor and send data to the backend. This becomes especially risky if future control functions depend on incoming data (Apriorit, 2025; Emq, 2024).

Prototype systems can reduce this risk with simpler mechanisms such as API keys or unique tokens. These help the backend distinguish trusted devices from unauthorized requests (Bosch, z.d.).

### 4.4 Exposed Backend Services

Exposing unnecessary backend services increases the attack surface. Internal components, such as the database, should not be directly reachable from outside the server environment. Container-based networking can help isolate internal communication between services, such as the API and database (HiveMQ Team, 2026).

Only necessary API endpoints should be exposed. Public debug ports, unnecessary service ports or overly visible API documentation can reveal internal structure and make attacks easier (Sowa, 2025c).

### 4.5 Secrets and Configuration

Sensitive credentials, such as database passwords and API tokens, are a frequent source of security problems. Hardcoded credentials in firmware or source code are risky because they can be extracted through static analysis or physical access (Mktg & Mktg, 2023c).

A safer approach is to manage secrets through environment variables instead of storing them in version control (Apriorit, 2025). Configuration templates such as `.env.example` can still be used without exposing real passwords or tokens.

## 5. Reliability Risks in Embedded-Backend Communication

This chapter analyses reliability risks. Reliability, or dependability, means that the system can continue delivering its service even when faults occur. In a smart city context, this is important because backend, database or network failures can affect monitoring and future control functions (Solouki et al., 2024; Admin, 2025).

### 5.1 Backend Service Failure

If the backend service, such as a FastAPI application, crashes or becomes unresponsive, embedded devices lose their target endpoint. They may still collect data locally, but they can no longer send it to the backend.

This can stop dashboard updates and leave operators with outdated information. In a later stage, it could also prevent the backend from sending commands back to devices that require real-time adjustment (Admin, 2025).

### 5.2 Database Failure

A backend can remain online but still fail to communicate with its database because of configuration errors, resource exhaustion or database container failure. Since the database stores the latest reliable system state, a database failure can cause the system to lose its operational context (Solouki et al., 2024).

Database failure can also create gaps in historical data. If the backend receives requests but cannot store them, monitoring and analysis records become incomplete. Poor error handling can also cause the backend service itself to crash (Admin, 2025).

### 5.3 Data Loss and Lack of Persistence

Data loss is a major risk when persistent storage is not configured correctly. Containers are temporary by nature, so data can be lost when a container is removed, recreated or rebuilt.

To improve reliability, database files must be mapped to persistent storage on the host machine (Solouki et al., 2024). However, persistent volumes are not enough on their own. Backups are still needed to recover from hardware failure or storage corruption.

### 5.4 Network Instability

Smart city sensors often rely on wireless connections that can be affected by interference, distance or temporary signal loss. This can cause delayed, missing, duplicate or out-of-order data packets (Solouki et al., 2024).

The backend should therefore handle network-related inconsistencies without crashing or creating unreliable database records.

### 5.5 Single Point of Failure

Relying on one physical host, such as a Raspberry Pi or server, creates a single point of failure. Power loss, SD-card corruption or hardware malfunction can make the full backend infrastructure unavailable (Admin, 2025).

Docker Compose can reduce this risk by making the backend easier to redeploy on alternative hardware. However, this only helps if the necessary data, configuration files and documentation are available (W, 2026). 

## 6. Requirements for Secure and Reliable Embedded-Backend Communication

This chapter translates the security and reliability risks from the previous chapters into practical design goals. The MoSCoW method is used to keep the scope realistic for the current prototype phase. These requirements form the bridge between the Analysis phase and the Design phase.

### 6.1 Must Have Requirements

**R1: Input Validation**  
The backend must validate incoming data payloads, for example with Pydantic schemas, to prevent malformed, invalid or unexpected data from being processed.

**R2: Secret Management**  
Credentials such as database passwords and API tokens must be managed through environment variables instead of being hardcoded in firmware or source code.

**R3: Persistent Storage**  
Database files must be stored in persistent volumes so that data remains available when containers are removed, recreated or rebuilt.

**R4: Basic Deployment Documentation**  
The backend deployment steps, environment configuration and basic recovery steps must be documented so the system can be maintained or redeployed by the team.

### 6.2 Should Have Requirements

**R5: Transport Encryption**  
Communication between embedded devices and the backend should use HTTPS/TLS where possible to protect data in transit from interception or modification.

**R6: Automated Service Recovery**  
The backend should use restart policies and health checks to detect failures and recover services where possible.

**R7: Network Isolation**  
Internal services, such as the database, should stay inside a private network. Only necessary API endpoints should be exposed, preferably through a reverse proxy.

**R8: Backup and Recovery Strategy**  
A backup and recovery process should be described to support recovery from hardware failure, storage corruption or data loss.

### 6.3 Could Have Requirements

**R9: Device Authentication**  
The backend could use API keys or tokens to verify connecting devices and reduce the risk of unauthorized spoofing.

**R10: Future Advanced Improvements**  
Advanced measures such as network resilience handling, deployment portability, hardware-backed security, signed firmware updates, monitoring tools or cloud fallback could be considered in future iterations, but they are outside the current prototype scope.

## 7. Conclusion

This analysis answered the main question: **How can embedded-backend communication in smart city systems be made secure and reliable?**

Embedded-backend communication can be made more secure by protecting data in transit, validating incoming data, managing secrets safely and limiting unnecessary network exposure. HTTPS/TLS helps reduce the risk of eavesdropping and Man-in-the-Middle attacks, while input validation and secret management help prevent unreliable data and credential leaks.

Reliability can be improved by focusing on service availability, persistent storage, recovery behaviour and deployment documentation. Risks such as backend failure, database failure, data loss, network instability and single-server dependency show that a working prototype still needs clear reliability measures.

For the City Sim project, these findings are relevant because the backend is deployed on a Raspberry Pi and uses Docker-based backend services. This creates a realistic context for applying the requirements from this analysis. HTTPS/TLS can improve communication security, persistent volumes can reduce the risk of data loss, and restart policies or health checks can improve service recovery.

The most important requirements for a realistic prototype are input validation, secret management, persistent storage and basic deployment documentation. These form the foundation for the Design phase, where the requirements can be applied to the City Sim backend using Docker containers, persistent volumes and an HTTPS/TLS approach where feasible.

## 8. References

Patidar, R. (2026, 1 mei). How Embedded Software is Powering the Future of Smart Devices. EvinceDev Blog. https://evincedev.com/blog/embedded-software-development-guide/

Abdelzaher, T., Hu, Y., Kara, D., Kimura, T., Misra, A., Ramani, V., Tardieu, O., Wang, T., Wigness, M., & Youssef, A. (2025). The bottlenecks of AI: challenges for embedded and real-time research in a data-centric age. Real-Time Systems, 61(2), 185–236. https://doi.org/10.1007/s11241-025-09452-w

Power by akacia, 阿卡希亞(股)公司, www.akacia.com.tw. (2026, 10 februari). IoT Communication Protocol selection: MQTT, HTTP, and COAP from a system architecture perspective - InnoComm. Innocomm. https://www.innocomm.com/en/news/news/content/iot-communication-protocol-selection

Vikram. (2026b, februari 24). MQTT vs. HTTP: Choosing the Right Protocol for Your IoT Project. MQTTfy. https://mqttfy.com/resources/mqtt-vs-http-for-iot

Craggs, I. (2026, 19 februari). MQTT vs. HTTP for IoT. MQTT Vs. HTTP for IoT. https://www.hivemq.com/blog/mqtt-vs-http-protocols-in-iot-iiot/

Mktg, S., & Mktg, S. (2023c, december 1). The OWASP IoT top 10 vulnerabilities and how to mitigate them. SISA. https://www.sisainfosec.com/blogs/the-owasp-iot-top-10-vulnerabilities-and-how-to-mitigate-them/

Sowa, A. (2025c, juli 14). Cybersecurity for Embedded Systems. Somco Software. https://somcosoftware.com/en/blog/cybersecurity-for-embedded-systems#:~:text=A%20single%20security%20flaw%20can%20open%20the%20door,data%E2%80%94functions%20where%20compromise%20could%20lead%20to%20real-world%20harm.

HiveMQ Team. (2026, 19 februari). Securing MQTT Systems - MQTT Security Fundamentals. Securing MQTT Systems - MQTT Security Fundamentals. https://www.hivemq.com/blog/mqtt-security-fundamentals-securing-mqtt-systems/

Emq. (2024, 12 november). Exploring MQTT-SN: A Comprehensive guide. www.emqx.com. https://www.emqx.com/en/blog/connecting-mqtt-sn-devices-using-emqx

Apriorit. (2025, 31 maart). 10 Best Practices to Ensure Embedded System Security. https://www.apriorit.com/dev-blog/690-embedded-systems-attacks

Solouki, M. A., Angizi, S., & Violante, M. (2024). Dependability in Embedded Systems: A Survey of Fault Tolerance Methods and Software-Based Mitigation Techniques. https://arxiv.org/html/2404.10509v1#S3

Admin. (2025, 4 augustus). How to Design Fail-Safe Systems for Critical Embedded Applications - Inspiro. Inspiro. https://www.inspiro.nl/en/how-to-design-fail-safe-systems-for-critical-embedded-applications/

W, S. (2026, 25 april). Risk Management in Embedded Projects & Approaches and Best Practices. https://www.linkedin.com/pulse/risk-management-embedded-projects-approaches-best-practices-veber-dce4c/

Zhou, X., Wang, P., Zhou, L., Xun, P., & Lu, K. (2023). A Survey of the Security Analysis of Embedded Devices. Sensors, 23(22), 9221. https://doi.org/10.3390/s23229221
