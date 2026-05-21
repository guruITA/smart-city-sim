# Analyse Template

| Document information | |
|---|---|
| Title | Analysis Report — Secure and Reliable Embedded-Backend Communication in Smart City Systems |
| Author | Betül Aydin |
| Date | [DD Month YYYY] |
| Version | 1.0 |
| Classification | Internal |
| Client | City Sim Learning Group / The Embedded Alliance |
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

[Introduce the purpose of this analysis report.]

[Explain that this report investigates the real-world problem of secure and reliable communication between embedded devices and backend systems.]

[Explain that smart city systems often use embedded devices such as sensors, traffic lights, parking sensors and streetlights to send data to backend systems.]

[Explain that this communication must be secure and reliable because the backend may store data, monitor systems, support decisions or control parts of the city.]

[Briefly mention that the City Sim project will be used as a practical case context later in the report.]

## 2. Research Question and Sub-Questions

## Main Question

How can embedded-backend communication in smart city systems be made secure and reliable?

## Sub-Questions

1. What is the real-world problem behind embedded-backend communication in smart city systems?
2. What security risks exist when embedded devices communicate with backend systems?
3. What reliability risks exist when embedded devices depend on backend systems?
4. What technical requirements are needed for secure and reliable embedded-backend communication?
5. How can these findings be applied to the City Sim backend deployment?

## 3. Real-World Problem Context

[Focus on the general real-world problem here, not yet too much on City Sim.]

Possible content:

- Smart cities use many small embedded devices.
- These devices collect data from the physical world.
- Examples: traffic lights, parking sensors, street lighting, air quality sensors, waste bins, public transport displays.
- These devices often send data to a backend through HTTP, MQTT or another protocol.
- The backend stores, processes and sometimes sends commands back.
- If communication fails, the system may lose data or become unreliable.
- If communication is not secure, attackers may read, change or fake data.
- This can affect trust, safety and decision-making.

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
