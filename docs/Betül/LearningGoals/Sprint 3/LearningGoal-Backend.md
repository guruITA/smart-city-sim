# **S — Situation**

At the start of Sprint 3, our team is working on a backend system that processes incoming data from external components. The backend runs inside a Docker environment and is intended to be deployed on a Raspberry Pi.

Within the team, the **Docker setup and Raspberry Pi deployment are handled by a teammate**, who is responsible for configuring and maintaining the infrastructure. I did not contribute to setting up this environment, but I do use it locally by running the Docker containers on my own laptop.

During this sprint, **my main contribution was focused on embedded tasks**, where I worked on the hardware-related aspects of the system. As a result, I have **not yet actively worked on the API or FastAPI endpoints**.

Although I have basic experience with running the backend in Docker, my understanding of how the backend, Docker, and Raspberry Pi environment connect is still limited. I also have not yet developed API endpoints myself.

Because of this, there is currently a **clear separation in team contributions**, but this has not been explicitly documented before. This made it unclear who was responsible for which parts of the system.

---

## **T — Task**

For this learning goal, my task is to **shift my focus from embedded work to backend development** in the next sprint.

This means I will:

- Gain a solid understanding of **FastAPI**
- Learn how **data flows through the backend system**
- Understand how the **Docker environment and Raspberry Pi setup interact with the backend**
- Start developing and testing **API endpoints that process and filter incoming data**

Additionally, I will **clearly document the division of responsibilities within the team**, so it is transparent who is working on infrastructure (Docker/Raspberry Pi) and who is working on backend logic (API development).

The goal is not only to build working endpoints, but also to **develop a clear understanding of the full backend system and my role within it**.

---

## **Deliverables**

### **Analysis document**

- Explanation of how the backend system works within Docker
- Description of how data flows through the system
- Reflection on my current knowledge gap (from embedded → backend)

### **Design document**

- Design of API endpoints (input, processing, output)
- Clear structure of filtering logic
- Explanation of how the backend connects to the Docker/Raspberry Pi environment

### **Realisation document**

- Implementation of FastAPI endpoints
- Explanation of how I run and test the backend locally using Docker
- Evidence of testing (e.g., curl requests)
- Reflection on learning progress from not knowing → implementing

### **Advise document**

- Evaluation of backend design and implementation
- Reflection on team role separation (embedded vs backend vs infrastructure)
- Suggestions for improving collaboration and system structure

---

## **A — Action**

### **Analysis**

- I will analyze how the backend system runs inside Docker
- I will investigate how data flows from embedded components to the backend
- I will identify what I do not yet understand about FastAPI and the system architecture

### **Advise**

- I will reflect on the division of responsibilities within the team
- I will explain how this division impacts collaboration and clarity
- I will suggest improvements for clearer task ownership

### **Design**

- I will design API endpoints before implementing them
- I will define input, processing, and output clearly
- I will structure filtering logic in a scalable way

### **Realise**

- I will learn the basics of FastAPI
- I will implement my first API endpoints
- I will run the backend locally using Docker
- I will test endpoints using curl in PowerShell
- I will iteratively improve my implementation

### **Manage & Control**

- I will clearly document **who did what within the team**
- I will track my transition from embedded to backend tasks
- I will test whether my endpoints meet requirements
- I will communicate progress during daily standups

### **Personal Leadership**

- I will take initiative in learning backend development
- I will actively close my knowledge gap in FastAPI and Docker
- I will reflect on my learning process
- I will ask for feedback and apply it
