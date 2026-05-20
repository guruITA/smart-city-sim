# Learning Goal - Sprint 4

**Improving the Security and Reliability of the City Sim Backend Using Docker Containers and HTTPS/SSL**

## Learning Question

**How can the deployed City Sim backend be improved with Docker containers and HTTPS/SSL to make the system more secure, reliable and professional for a smart city environment?**

## S — Situation

During Sprint 4, the City Sim project needs to become more professional, smart and reliable. The backend is already deployed on a Raspberry Pi, but the current setup still needs improvement to better support a realistic smart city environment. The mayor/teacher gave feedback that the city should not only work as a basic prototype, but should show a more challenging and professional solution.

One important point of feedback was the question of what happens when the Raspberry Pi or backend service crashes. This means that the backend should not only receive and process data, but should also be designed with security, reliability and recovery in mind. Gerald also mentioned possible technical improvements such as using multiple Docker containers and adding HTTPS/SSL for a more secure API.

Because of this, the focus of this learning goal is to investigate and improve the backend deployment by using Docker containers and HTTPS/SSL. This connects to the backend profile and supports the City Sim by making the system more secure, structured and production-like.

## T — Task

The task is to analyse the current backend deployment and design an improved setup that uses Docker containers and HTTPS/SSL. The goal is to make the backend more secure and reliable while keeping the solution realistic for the sprint.

The backend should be structured in a way that separates responsibilities between services, such as the backend application, database and possibly a reverse proxy. The system should also use HTTPS/SSL so that API communication becomes more secure than plain HTTP. In addition, the design should consider reliability measures such as persistent volumes, restart policies and possible future backup or fallback strategies.

The final evidence should be written as a structured technical report using Gerald’s method. This means the evidence should include a main question, sub-questions, analysis, design, realisation, conclusion, recommendations and references.

## A — Action

To achieve this learning goal, the first step is to analyse the current Raspberry Pi deployment and identify risks related to security and reliability. This includes checking how the backend currently runs, which services are used, how data is stored and whether the API is exposed over HTTP or HTTPS.

After the analysis, a new deployment design will be created. This design will describe how Docker containers can be used to separate backend services and how HTTPS/SSL can be added to secure the API. The design will include diagrams or explanations of the container structure, data flow and deployment setup.

The implementation will focus on applying the designed improvements where possible within the sprint. This may include updating the Docker Compose configuration, adding or improving container separation, configuring HTTPS/SSL, testing the deployed backend and documenting the results. If full implementation of every reliability feature is not possible, the remaining improvements will be included as recommendations.
