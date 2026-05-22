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
11. Appendix  

## 1. Introduction

TODO: write as last 

## 2. Research Question and Sub-Questions

## Main Question

**How can embedded-backend communication in smart city systems be made secure and reliable?**

## Sub-Questions

1. **What is the real-world problem behind embedded-backend communication in smart city systems?**
2. **What security risks can occur when embedded devices communicate with backend systems?**
3. **What reliability risks can occur when embedded devices depend on backend systems?**
4. **What requirements are needed to make embedded-backend communication secure and reliable?**

## 3. Real-World Problem Context

The growth of the Internet of Things (IoT) has placed small computing devices, known as embedded systems, at the heart of modern cities. These devices manage important tasks like monitoring traffic and environment sensors. However, as these systems move from simple prototypes to real-world use, they face a "dependability gap". This means they are not yet safe, reliable, or secure enough for professional city services.

Security Risks and the CIA Triad
Connected devices are often targets for hackers because they expand the attack surface of a network. These risks are usually measured using the CIA triad (Confidentiality, Integrity, and Availability):

- Confidentiality (Privacy): Many devices use unencrypted communication. This allows hackers to "eavesdrop" and steal private data. 
- Integrity (Accuracy): Through Man-in-the-Middle (MitM) attacks, a hacker can change data packets. This leads to the backend receiving fake or wrong sensor data. 
- Availability (Service): Systems are vulnerable to network attacks or software crashes that can stop the device from working.

## 4. Embedded-Backend Communication in Smart City Systems

[Explain how embedded-backend communication normally works.]

Possible structure:

Embedded device  
→ network connection  
→ API endpoint / message broker  
→ backend service  
→ database  
→ dashboard / monitoring / control system  

Possible things to explain:

- Embedded devices send JSON or sensor values.
- Backend validates incoming data.
- Backend stores data in a database.
- Backend may provide the data to dashboards.
- Backend may later send commands back to devices.
- Communication can be one-way or two-way.

## 5. Security Risks in Embedded-Backend Communication

### 5.1 Unencrypted Communication

[Explain the risk of HTTP/plain communication.]

Possible content:

- HTTP does not encrypt data.
- Data can potentially be read or modified in transit.
- HTTPS/TLS helps protect confidentiality and integrity.
- Even sensor data can be sensitive when combined over time.

### 5.2 Fake or Manipulated Data

[Explain that a backend may receive fake requests.]

Possible content:

- If endpoints are open, other devices/users may send fake data.
- Fake data can pollute the database.
- Fake sensor data can lead to wrong decisions.
- Input validation is needed.

### 5.3 Weak Authentication or No Authentication

[Explain the risk of not knowing which device sent data.]

Possible content:

- Backend should know which device is allowed to send data.
- Without authentication, any device may pretend to be a sensor.
- Later improvements could include API keys, tokens or device certificates.

### 5.4 Exposed Backend Services

[Explain risk of exposing too much.]

Possible content:

- Database should not be directly reachable from outside.
- API docs may reveal available endpoints.
- Only necessary ports should be exposed.
- Internal services should stay inside the Docker network.

### 5.5 Secrets and Configuration

[Explain risk of hardcoded passwords.]

Possible content:

- Passwords and tokens should not be in source code.
- Use environment variables.
- Use `.env.example` for structure, but not real secrets.
- `.env` should not be committed.

## 6. Reliability Risks in Embedded-Backend Communication

### 6.1 Backend Service Failure

[Explain what happens if the backend is down.]

Possible content:

- Embedded devices cannot send data.
- Data may be lost.
- Dashboards may stop updating.
- Commands cannot be sent back to devices.

### 6.2 Database Failure

[Explain what happens if the database is unavailable.]

Possible content:

- Backend may receive requests but fail to store them.
- Historical data can be lost.
- Application may crash if database dependency is not handled.

### 6.3 Data Loss

[Explain why persistent storage matters.]

Possible content:

- Containers are temporary.
- Database data should be stored in persistent volumes.
- Backups are needed for recovery.
- Without persistence, data can disappear after rebuilds or failures.

### 6.4 Network Instability

[Explain embedded devices may disconnect.]

Possible content:

- WiFi can be unstable.
- Devices may send repeated or delayed data.
- Backend should handle missing, duplicate or late messages.

### 6.5 Single Point of Failure

[Explain if everything depends on one server.]

Possible content:

- If one Raspberry Pi/server fails, the whole system may stop.
- A recovery plan is needed.
- Future improvements could include backups, monitoring or fallback deployment.

---

## 7. Requirements for Secure and Reliable Embedded-Backend Communication

[Create requirements based on your analysis.]

## 8. Relevance to the City Sim Project

[Only here you connect it to your own project.]

## 9. Conclusion

[Answer the main question.]

## 10. References

[Add APA-style references here.]

Use this format:

Author/Organisation. (Year). Title. Website or publisher. URL

## 11. Appendix

### Appendix A — Notes

[Add notes from your own project here.]

### Appendix B — Evidence

[Add screenshots or command outputs later if needed.]

Possible evidence:

- Current backend access
- Docker Compose structure
- Health endpoint
- Swagger docs
- Current HTTP access
