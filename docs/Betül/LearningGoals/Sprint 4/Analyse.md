# Analyse Template

| Document information | |
|---|---|
| Title | Analysis Report — Secure and Reliable Embedded-Backend Communication in Smart City Systems |
| Author | Betül Aydin |
| Date | 21 May 2026 |
| Version | 1.0 |
| Classification | Internal |
| Mayor | Mats |
| Company | Amsterdam University of Applied Sciences |
| Learning outcome | Analysis |
| Sprint | Sprint 4 |

## Table of Contents

1. Introduction  
2. Research Question and Sub-Questions  
3. Real-World Problem Context  
4. Embedded-Backend Communication in Smart City Systems  
5. Security Risks in Embedded-Backend Communication  
6. Reliability Risks in Embedded-Backend Communication  
7. Requirements for a Secure and Reliable Backend Deployment  
8. Relevance to the City Sim Project  
9. Conclusion  
10. References  

## 1. Introduction

This analysis report investigates how embedded-backend communication in smart city systems can be made more secure and reliable. In smart city environments, embedded devices such as sensors, traffic lights, streetlights and parking systems collect data from the physical environment and send this data to backend systems. These backend systems can validate, store and process the data for monitoring, analysis or future control decisions.

The reason for this analysis is that a basic working prototype is not automatically suitable for a more professional smart city environment. A prototype may function correctly, but it can still be vulnerable if communication is unencrypted, backend services are not separated, data is not stored persistently or recovery steps are unclear.

This report first explains the real-world problem behind embedded-backend communication. It then analyses security risks, reliability risks and the requirements needed to make the communication more secure and dependable. The findings provide the foundation for the Design phase, where the requirements can be applied to the City Sim backend context using Docker containers and HTTPS/TLS.

## 2. Research Question and Sub-Questions

## Main Question

**How can embedded-backend communication in smart city systems be made secure and reliable?**

## Sub-Questions

1. **What is the real-world problem behind embedded-backend communication in smart city systems?**
2. **What security risks can occur when embedded devices communicate with backend systems?**
3. **What reliability risks can occur when embedded devices depend on backend systems?**
4. **What requirements are needed to make embedded-backend communication secure and reliable?**

## 3. Real-World Problem Context

The growth of the Internet of Things (IoT) has placed small computing devices, known as embedded systems, at the heart of modern cities. These devices manage important tasks like monitoring traffic and environment sensors (Zhou et al., 2023). However, as these systems move from simple prototypes to real-world use, they face a "dependability gap". This means they are not yet safe, reliable, or secure enough for professional city services (Solouki et al., 2024).

### Security Risks and the CIA Triad  
Connected devices are often targets for hackers because they expand the attack surface of a network. These risks are usually measured using the CIA triad (Confidentiality, Integrity, and Availability) (Securing Connected Embedded Systems From Cyber Threats | HCLTech, z.d.):

- Confidentiality (Privacy): Many devices use unencrypted communication. This allows hackers to "eavesdrop" and steal private data. 
- Integrity (Accuracy): Through Man-in-the-Middle (MitM) attacks, a hacker can change data packets. This leads to the backend receiving fake or wrong sensor data (Sowa, 2025d). 
- Availability (Service): Systems are vulnerable to network attacks or software crashes that can stop the device from working.

### Reliability and the Single Point of Failure  
Besides hackers, basic systems often fail because of how they are designed. In many prototype setups, the backend acts as a single point of failure. This means that if the main backend service or the hardware (like a Raspberry Pi) crashes, the entire smart city system stops working.
These failures can be caused by simple hardware errors or mistakes in the software code (Admin, 2025b). 

### The Hardware Constraint Challenge          
Fixing these problems is difficult because of the limitations of embedded hardware. Devices like the ESP32 or Raspberry Pi have limited power and memory. Because of these limits, it is hard to run heavy security software or complex monitoring tools without making the device too slow.

The main problem is that standard communication between devices and backends is often too fragile for real-world city operations. A system that does not protect its data or separate its services is not yet suitable for professional use. To reach a professional standard, the architecture must be improved to ensure that data is accurate and services keep running even if a crash occurs (Apriorit, 2025).

## 4. Embedded-Backend Communication in Smart City Systems

In a Smart City ecosystem, the communication between embedded devices and the backend is the foundational "connective tissue" that supports important urban functions. This architecture must balance the resource constraints of embedded hardware with the need for secure transport and backend reliability (Patidar, 2026).

### The Communication Flow: A Layered Architecture

Professional smart city systems often follow a layered design to manage the flow of data from the physical environment to the user interface:
- **Embedded Device:** A microcontroller (such as an ESP32) collects data from sensors—monitoring variables like traffic flow or air quality. Due to resource constraints, data is often formatted as lightweight JSON or binary payloads.
- **Secure Transport (HTTPS/TLS):** Data is transmitted over a network (Wi-Fi, Ethernet, or cellular). To ensure data integrity and confidentiality, HTTPS/TLS is used to encrypt data in transit. While TLS secures the communication channel, database security and device authentication are managed as separate layers.
- **API Endpoint / Message Broker:** This serves as the formal entry point. While the TLS layer secures the connection, the backend validates the incoming data and can apply additional authentication mechanisms if required, such as API keys or tokens(Mieruński, 2026).
- **Backend Service & Database:** A service (e.g., FastAPI) processes the data and stores it in a database (e.g., PostgreSQL) for historical analysis.

### Protocol Comparison: HTTP/HTTPS vs. MQTT

The choice of protocol dictates the system's responsiveness and efficiency.
- **HTTP/HTTPS (Request-Response):** This is the standard for web APIs and is effective for sending data to web APIs or configuration updates. However, because it is client-initiated, it is less suitable for direct server-initiated commands, as the server cannot spontaneously send data to the device without the device "polling" first (Power by akacia, 阿卡希亞(股)公司, www.akacia.com.tw, 2026).
- **MQTT (Publish/Subscribe):** Designed specifically for the IoT, MQTT is highly efficient due to its low overhead. It is better suited for two-way, event-driven communication because a broker can push messages to devices over a persistent connection. Reliability depends on the configured Quality of Service (QoS) level and network stability (Vikram, 2026b).

### Backend Reliability and Containerization

For a smart city system, the backend must remain available so that incoming sensor data can still be received, processed and stored. A container-based deployment tool such as Docker Compose can support backend reliability by separating services and making them easier to manage.

- **Service Isolation and Maintenance:** Components such as the API, database, reverse proxy or message broker can run in separate containers. This makes it easier to isolate, restart and maintain individual services.

- **Restart Policies and Health Checks:** Docker Compose can use restart policies and health checks to help services recover after crashes or reboots and to detect whether a container is still functioning correctly.

- **Persistent Data Management:** Persistent volumes reduce the risk of data loss during container restarts or recreations. However, stronger data protection also requires regular database backups and proper storage management (Abdelzaher et al., 2025).

Together, HTTPS/TLS and containerization support secure and reliable embedded-backend communication. HTTPS/TLS protects data in transit, while Docker Compose supports service separation, recovery behaviour and persistent data storage.

## 5. Security Risks in Embedded-Backend Communication

The interaction between embedded hardware and backend services introduces specific attack vectors that can compromise the functionality of a Smart City system. Because these systems often manage operational or sensitive data, addressing security risks is essential to maintain the integrity of urban monitoring and future control functions (Mktg & Mktg, 2023).

### 5.1 Unencrypted Communication
Using unencrypted protocols, such as standard HTTP or plain MQTT, creates significant vulnerabilities regarding data confidentiality and integrity.  
- **Plain Text Transmission:** Standard HTTP transmits data without encryption, allowing unauthorized actors on the network path to read the information.  
- **Eavesdropping and MitM:** Without transport-level security, systems are vulnerable to Man-in-the-Middle (MitM) attacks, where an attacker intercepts or modifies traffic in transit (Sowa, 2025).  
- **Data Aggregation Risks:** While individual sensor values may seem minor, the accumulation of such data over time can reveal sensitive operational patterns about city infrastructure.  
- **Protection via TLS:** Implementing HTTPS/TLS is a common method for encrypting data in transit, significantly reducing the risk of tampering during transmission (Bosch, z.d.).

### 5.2 Fake or Manipulated Data
Backend services that do not properly validate incoming data are susceptible to data pollution and logic errors.
- **Unauthorized Requests:** If API endpoints are left open and unprotected, malicious actors can send fake sensor readings or status updates (Mktg & Mktg, 2023b).
- **Database Pollution:** Injected fake data can corrupt historical records, making long-term analysis and monitoring unreliable.
- **Improper Input Validation:** The backend must be able to handle malformed payloads, invalid data types, or unexpected values. Without robust validation (e.g., via FastAPI/Pydantic schemas), these inputs could lead to application errors or incorrect automated decisions (Sowa, 2025b).

### 5.3 Weak Authentication or No Authentication
A common risk in IoT systems is the inability of the backend to verify the identity of a connecting device.
- **Identity Verification:** The backend needs a mechanism to distinguish between a legitimate sensor (like an ESP32) and an unauthorized device (Apriorit, 2025).
- **Device Spoofing:** Without authentication, any device can "pretend" to be a sensor and push data to a specific endpoint, potentially triggering unauthorized actions in future control scenarios (Emq, 2024).
- **Implementation Options:** While advanced systems may use X.509 digital certificates, prototype implementations often utilize API keys or unique tokens to authorize device access (Bosch, z.d.).

### 5.4 Exposed Backend Services
Providing external or unnecessary network access to internal backend components significantly increases the system's attack surface.
- **Network Isolation:** Services should utilize container-based networking to ensure that internal communication, such as the link between the API and the database is isolated from unauthorized network segments (HiveMQ Team, 2026).
- **Access Control:** Internal services and databases should generally not be directly reachable from outside the server environment.
- **Endpoint Visibility:** Publicly available API documentation or exposed debug ports can reveal the internal structure of the system, making it easier for attackers to find entry points (Sowa, 2025c).

### 5.5 Secrets and Configuration
The management of sensitive credentials, such as database passwords and API tokens, is a frequent point of failure.
- **Hardcoded Credentials:** Embedding fixed passwords or keys directly into the firmware or source code is a major security flaw, as these can be extracted via static analysis or physical access (Mktg & Mktg, 2023c).
- **Environment Variables:** A common best practice is managing secrets through environment variables rather than storing them in version control (Apriorit, 2025).
- **Configuration Security:** Utilizing templates (e.g., .env.example) allows for a consistent structure across development environments without exposing actual secrets in shared repositories.

## 6. Reliability Risks in Embedded-Backend Communication

Reliability, often referred to as **dependability**, is the system's ability to maintain its service delivery even when internal faults occur. In a Smart City context, a **failure** happens when the system's actual behavior diverges from its expected operation (Solouki et al., 2024). Identifying these risks is essential for creating a resilient design that ensures important urban functions remain operational despite hardware or software issues (Admin, 2025).

### 6.1 Backend Service Failure

If the backend service (such as a FastAPI application) crashes or becomes unresponsive, the **link** between the city's sensors and the management logic **is interrupted**.

- **Interruption of Data Reception:** While embedded devices might still collect data locally, they lose their target endpoint and can no longer transmit information to the backend.
- **Impact on Future Control Functions:** The backend becomes unable to send commands back to the devices, which is a significant risk for **future control functions** that require real-time adjustment based on sensor input (Admin, 2025).
- **Frozen Monitoring:** Dashboards stop updating, leaving operators with outdated information that does not reflect the current state of the city.

### 6.2 Database Failure

A backend may remain online but become unable to communicate with its database, often due to configuration errors or resource exhaustion.

- **Loss of Latest System State:** The database often serves as the **"stable memory"** that stores the latest fault-free state of the system. If the database fails, the system loses its current operational context or "checkpoint". Without this saved state, a subsequent restart of the backend would mean the system has no record of the city's most recent status (e.g., the current state of traffic controllers) (Solouki et al., 2024).
- **Gaps in Historical Data:** The backend may still receive incoming requests, but the inability to store them leads to gaps in the records used for **monitoring and analysis**.
- **Application Instability:** If the software does not properly handle database connection errors through robust exception handling, a database failure can cause the entire backend service to crash (Admin, 2025).

### 6.3 Data Loss and Lack of Persistence

Data loss is a high-impact risk when the system lacks a robust strategy for **persistent storage**.

- **Ephemeral Container Storage:** Containers are temporary by nature; while data might survive a simple restart, it is typically **lost when a container is removed, recreated, or rebuilt** unless explicitly managed.
- **Persistent Storage Mapping:** To ensure reliability, database files must be mapped to **persistent storage** on the host machine, ensuring information remains available across these lifecycle events (Solouki et al., 2024).
- **The Role of Backups:** While persistent storage protects data during service updates, regular **backups** are still required to recover the system in the event of major hardware failures.

### 6.4 Network Instability

Smart City sensors often rely on wireless connections that are susceptible to environmental influences and interference.

- **Transient Faults:** Signal issues can cause temporary disruptions, resulting in delayed or missing data packets.
- **Duplicate Data:** When a device re-executes a transmission to compensate for a weak signal, it can lead to the backend receiving duplicate or out-of-order messages (Solouki et al., 2024).
- **Backend Resilience:** The backend must be designed to handle these network-induced inconsistencies without crashing or creating duplicate records in the database.

### 6.5 Single Point of Failure

Relying on a single physical host (like one Raspberry Pi or server) to run the entire backend infrastructure creates a **Single Point of Failure**.

- **Hardware Vulnerability:** The entire system is vulnerable to common hardware issues such as **power loss, storage failure (e.g., SD-card corruption), or general hardware malfunctions** (Admin, 2025).
- **Lack of Redundancy:** Without a standby system or a clear recovery plan, a single hardware fault **can result in service downtime** for the city’s monitoring infrastructure (W, 2026).
- **Portability as Mitigation:** Utilizing **Docker Compose** helps mitigate this risk by making the infrastructure portable, allowing for faster redeployment on alternative hardware. However, this is only effective if the **necessary data, configuration files, and documentation** are also readily available.


## 7. Requirements for Secure and Reliable Embedded-Backend Communication

The following requirements translate the security and reliability risks from the analysis into practical design goals. The MoSCoW method is used to keep the scope realistic for the current prototype phase.

### 7.1 Must Have Requirements

**R1: Input Validation**

The backend must validate incoming data payloads, for example with Pydantic schemas, to prevent malformed, invalid or unexpected data from being processed.

**R2: Secret Management**

Credentials such as database passwords and API tokens must be managed through environment variables instead of being hardcoded in firmware or source code.

**R3: Persistent Storage**

Database files must be stored in persistent volumes so that data remains available when containers are removed, recreated or rebuilt.

**R4: Basic Deployment Documentation**

The backend deployment steps, environment configuration and basic recovery steps must be documented so the system can be maintained or redeployed by the team.

### 7.2 Should Have Requirements

**R5: Transport Encryption**

Communication between embedded devices and the backend should use HTTPS/TLS where possible to protect data in transit from interception or modification.

**R6: Automated Service Recovery**

The backend should use restart policies and health checks to detect failures and recover services where possible.

**R7: Network Isolation**

Internal services, such as the database, should stay inside a private network. Only necessary API endpoints should be exposed, preferably through a reverse proxy.

**R8: Backup and Recovery Strategy**

A backup and recovery process should be described to support recovery from hardware failure, storage corruption or data loss.

### 7.3 Could Have Requirements

**R9: Device Authentication**

The backend could use API keys or tokens to verify connecting devices and reduce the risk of unauthorized spoofing.

**R10: Future Advanced Improvements**

Advanced measures such as network resilience handling, deployment portability, hardware-backed security, signed firmware updates, monitoring tools or cloud fallback could be considered in future iterations, but they are outside the current prototype scope. 

## 8. Relevance to the City Sim Project

The City Sim project is a small-scale version of the real-world problem described in this analysis. In the project, embedded devices communicate with a central backend that receives, stores and processes sensor data. Even though the system is a prototype, it still benefits from professional design choices such as secure communication, service separation, persistent storage and recovery planning.

The analysis is relevant to City Sim because the backend is deployed on a Raspberry Pi and uses Docker-based backend services. This creates a realistic context for applying the requirements from Chapter 7. For example, HTTPS/TLS can improve communication security, persistent volumes can reduce the risk of data loss, and restart policies or health checks can improve service recovery.

The Design phase should therefore apply these requirements to the City Sim backend context. The design should focus on a realistic Docker-based structure with separated services, persistent storage and an HTTPS/TLS approach where feasible within the sprint scope.

## 9. Conclusion

This analysis answered the main question: **How can embedded-backend communication in smart city systems be made secure and reliable?**

Embedded-backend communication can be made more secure by protecting data in transit, validating incoming data, managing secrets safely and limiting unnecessary network exposure. HTTPS/TLS helps reduce the risk of eavesdropping and Man-in-the-Middle attacks, while input validation and secret management help prevent unreliable data and credential leaks.

Reliability can be improved by focusing on service availability, persistent storage, recovery behaviour and deployment documentation. Risks such as backend failure, database failure, data loss, network instability and single-server dependency show that a working prototype still needs clear reliability measures.

The most important requirements for a realistic prototype are input validation, secret management, persistent storage and basic deployment documentation. These form the foundation for the Design phase, where the requirements can be applied to the City Sim backend using Docker containers, persistent volumes and an HTTPS/TLS approach where feasible.

## 10. References

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