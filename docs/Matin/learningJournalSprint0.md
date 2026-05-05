Learning question
What tools and technologies do I need to learn for backend development in an IoT Smart City project, and how do they work together?

S — Situation
This is the start of our Smart City project "The Embedded Alliance". Our team of 5 students (Betul, Gurpreet, Thijmen, Wesley and me) just started Sprint 0. We had our first team meetings where we discussed project goals and started planning our Smart City.

What we had done at this point:

GitLab repository created

index.md written with project overview

City layout drawn on paper as a team

No code written yet

No development environment set up

My role is Backend Developer. I will be working on the server side that receives and stores sensor data from ESP32 microcontrollers. However, at this point I had no clear idea which technologies to use. I am a fullstack developer with experience in JavaScript and Python, but I have never built a backend specifically for IoT sensor data.

T — Task
My main objective for Sprint 0 was to understand what a backend for IoT looks like and figure out which technologies I should use.

Concrete goals:

Research what technologies are commonly used for IoT backends

Understand how ESP32 sensors connect to a backend server

Choose a tech stack that fits our project needs and my skill level

Document my findings for the team

Create a learning plan for Sprint 1

A — Action
Week 1: Team meetings and city design
We had a kickoff meeting on Monday with coaches Mats Otten and Gerald Stap. We discussed the requirements and divided roles. On Tuesday we did a brainstorming session as a team and drew our Smart City layout. The embedded team (Gurpreet, Thijmen, Wesley) will handle the ESP32 sensors, and the backend team (Betul and me) handles the server and database.

Research phase: What does a backend developer do in an IoT project?
I started by researching what a backend developer actually does in this context. I watched YouTube videos (Fireship, TechWorld with Nana), read articles about REST APIs and IoT architectures, and used AI tools to help me understand concepts.

What I learned about the backend role:

Build an API that receives sensor data from ESP32 devices over WiFi

Validate incoming data (is it in the correct format?)

Store data in a database for historical analysis

Serve data to a frontend/dashboard

Ensure the system runs reliably

Technology exploration: What should I use?
This was the hardest part because I genuinely did not know what to pick. I explored several options:

Option 1: Java Spring Boot + MySQL The coaches mentioned Java Spring Boot in early sessions, and MySQL is what I learned in previous semesters. But I realized Spring Boot has a steep setup curve, requires a lot of boilerplate code, and I would spend more time fighting the framework than building features. I am not confident enough in Java to move fast.

Option 2: Node.js + Express + MongoDB Familiar from previous projects. But MongoDB felt wrong for structured sensor data (timestamp, value, unit) where every reading has the same shape. A relational database makes more sense here.

Option 3: Python FastAPI + PostgreSQL I discovered FastAPI through online research. It immediately clicked because Python is the language I am most comfortable with, FastAPI auto-generates API documentation (Swagger UI), it has built-in data validation with Pydantic, and PostgreSQL is more robust than MySQL for production use. I had not used FastAPI before but the learning curve seemed manageable.

Docker: Regardless of which stack I chose, Docker was always going to be part of it. It solves the "works on my machine" problem and means team members do not need to install PostgreSQL locally.

Key insight: How ESP32 connects to a backend
The most important thing I learned was the communication flow:

ESP32 reads a sensor value (e.g. distance in cm)

ESP32 connects to WiFi

ESP32 sends an HTTP POST request with JSON data to the API

Backend validates and stores the data

Dashboard reads data from the backend via HTTP GET

This is essentially the same as any web application, just with a microcontroller as the client instead of a browser.

Team collaboration
I created the index.md with the project overview on GitLab. I discussed the architecture with Betul (who is also assigned to backend) and agreed that I would focus on the API and she would support with Scrum Master duties. I talked with the embedded team about how we would test the connection between ESP32 and backend in Sprint 1.

R — Result
By the end of Sprint 0 I had:

A clear understanding of what an IoT backend needs to do

Chosen my tech stack: FastAPI (Python) + PostgreSQL + Docker Compose

Understanding of how ESP32 devices communicate with a backend via HTTP

A GitLab repository with basic project structure and documentation

A learning plan for Sprint 1: build the API, set up Docker, create the first endpoints

What I did NOT have yet:

Any working code

A database schema

Experience with FastAPI (never used it before)

A clear picture of what endpoints each tile would need

R — Reflection
The biggest lesson from Sprint 0 is that choosing tools is harder than using them. I spent most of my time going back and forth between options instead of just picking one and starting. In hindsight, the decision to go with FastAPI over Spring Boot was correct because speed of development matters more than framework popularity in a 17-week student project.

I also realized that I learn better by doing than by researching. I watched many videos and read many articles, but the real understanding only came when I started thinking about concrete things: what JSON will the ESP32 send? What tables do I need? What endpoints does the dashboard call?

A weakness I noticed: I tend to work alone and figure things out by myself instead of asking the team or coaches for input on technology choices. In Sprint 1, I want to involve Betul more in the backend decisions.

T — Transfer
For Sprint 1, I will apply what I learned by:

Building the FastAPI backend with Docker Compose

Creating a generic API endpoint that all tiles can use

Writing the database schema for sensor readings

Documenting the API so team members can integrate their ESP32 devices

Setting up a basic dashboard to visualize the data

The pattern of "research options, pick one, build a prototype, iterate" is transferable to any technology decision I will face in future projects.

References
FastAPI documentation. https://fastapi.tiangolo.com/ (accessed 2026-02-09)

SQLAlchemy 2.0 documentation. https://docs.sqlalchemy.org/ (accessed 2026-02-09)

Docker documentation: Compose. https://docs.docker.com/compose/ (accessed 2026-02-09)

Arduino HTTPClient library. https://docs.arduino.cc/libraries/httpclient/ (accessed 2026-02-09)

Fireship. (2023). "Backend web development - a complete overview". YouTube. https://www.youtube.com/watch?v=XBu54nfzxAQ (accessed 2026-02-08)
