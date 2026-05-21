# Learning Goal - Sprint 4

Improving the Security and Reliability of Embedded-Backend Communication Using Docker Containers and HTTPS/SSL

## Learning Question

How can embedded-backend communication in smart city systems be made more secure and reliable, and how can these findings be applied to the City Sim backend deployment using Docker containers and HTTPS/SSL?

## S — Situation

During Sprint 4, the City Sim project needs to become more professional, smart and reliable. The backend is already deployed on a Raspberry Pi, but the current setup still needs improvement to better support a realistic smart city environment. The mayor/teacher gave feedback that the city should not only work as a basic prototype, but should show a more challenging and professional solution.

In real-world smart city systems, embedded devices communicate with backend systems to send sensor data, status updates or control information. This communication needs to be secure and reliable, because backend systems may store data, monitor behaviour or support decisions. If the communication is not secure, data can be intercepted, manipulated or faked. If the communication is not reliable, data can be lost or services can become unavailable.

One important point of feedback was the question of what happens when the Raspberry Pi or backend service crashes. This means that the backend should not only receive and process data, but should also be designed with security, reliability and recovery in mind. Gerald also mentioned possible technical improvements such as using multiple Docker containers and adding HTTPS/SSL for a more secure API.

Because of this, the focus of this learning goal is to first investigate the real-world problem of secure and reliable embedded-backend communication, and then apply these findings to the City Sim backend deployment using Docker containers and HTTPS/SSL. This connects to the backend profile and supports the City Sim by making the system more secure, structured and production-like.

## T — Task

The task is to analyse how embedded-backend communication can be made more secure and reliable in a smart city context. The goal is to research common risks and requirements first, and then apply this to the current City Sim backend deployment.

The backend should be structured in a way that separates responsibilities between services, such as the backend application, database and possibly a reverse proxy. The system should also use HTTPS/SSL where possible so that API communication becomes more secure than plain HTTP. In addition, the design should consider reliability measures such as persistent volumes, restart policies, health checks and possible future backup or fallback strategies.

The final evidence should be written as a structured technical report using Gerald’s method. This means the evidence should include a title page with title, author, date, version, classification, client and company, followed by a table of contents, main question, sub-questions, analysis, design, realisation, conclusion, recommendations and references.

The report should also include APA-style references at the end and in-text citations inside the text.

## A — Action

To achieve this learning goal, the first step is to analyse the real-world problem of secure and reliable embedded-backend communication. This includes researching risks such as unencrypted HTTP communication, fake or invalid sensor data, exposed API endpoints, backend failure, database failure, data loss and missing recovery strategies.

After the analysis, the findings will be applied to the City Sim project context. A new deployment design will be created for our own team setup. This design will describe how Docker containers can be used to separate backend services and how HTTPS/SSL can be added to secure API communication. The design will include diagrams or explanations of the container structure, data flow and deployment setup.

The implementation will focus on applying or testing the designed improvements where possible within the sprint. This may include updating or checking the Docker Compose configuration, improving container separation, checking restart policies, testing health endpoints, researching or configuring HTTPS/SSL, testing the deployed backend and documenting the results in the Raspberry Pi context.

If full implementation of every reliability or security feature is not possible, the remaining improvements will be included as recommendations.

For the Advise evidence, the advice will be written for my co-backend developer, Matin, as the target audience. The advice will explain what should be improved, maintained or finished in the backend deployment.

I will also ask another teacher for feedback on my evidence before final submission, preferably Raymond, to improve the structure, technical depth, references and learning outcome alignment.

## Deliverables

### Analysis document

- Explain the real-world problem of secure and reliable embedded-backend communication
- Research security risks, such as HTTP instead of HTTPS, fake data and exposed API endpoints
- Research reliability risks, such as backend/container/database/Raspberry Pi failure
- Explain why these risks matter in smart city or IoT systems
- Define requirements for improving embedded-backend communication with Docker containers and HTTPS/SSL
- Use Gerald’s document format
- Include APA-style references and in-text citations

### Design document

- Design an improved Docker-based backend structure for our own City Sim team context
- Explain the role of each container, such as backend, database and reverse proxy
- Design how HTTPS/SSL can secure API communication
- Explain how the improved setup connects to the Raspberry Pi, FastAPI backend, PostgreSQL database and ESP32 tiles
- Include reliability choices such as persistent volumes, restart policies and health checks

### Realisation document

- Implement or configure multiple Docker containers where possible
- Implement, configure or test HTTPS/SSL where possible
- Test the improved backend deployment in the City Sim Raspberry Pi context
- Provide evidence of testing with Swagger, Postman, curl or browser requests
- Show that the backend remains reachable and functional after the deployment changes
- Explain what worked, what did not work and what still needs to be improved

### Advise document

- Write advice for Matin as my co-backend developer
- Evaluate the improved backend deployment
- Evaluate the Docker and HTTPS/SSL solution
- Explain how the changes improve security and reliability
- Describe limitations and unfinished parts
- Advise backup/fallback options if the Raspberry Pi crashes
- Suggest future improvements such as database backups, monitoring, cloud fallback or Kubernetes