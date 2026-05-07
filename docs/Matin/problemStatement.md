# Problem statement: Parking search traffic in Amsterdam

## The problem

Drivers in Amsterdam spend an average of 10 to 20 minutes looking for a free parking spot in busy neighborhoods. Studies estimate that up to 30% of inner city traffic in busy areas consists of cars circling for parking (Shoup, 2017). This search traffic produces unnecessary CO2 emissions, increases noise in residential streets, and adds to fine particle pollution. Amsterdam targets emission free traffic in the city center by 2030, so reducing unnecessary driving matters.

## Why this matters

Residents in dense neighborhoods like De Pijp, Oost, and Centrum deal with cars slowly circling their streets at all hours. Drivers waste fuel and time on something that should be simple: knowing where a spot is free. The municipality spends money on traffic management that could be avoided if parking availability data was accessible in real time.

## What I am building

I am building a smart parking detection system as a proof of concept for the City Sim project. Each parking spot has an ESP32 microcontroller with an HC-SR04 ultrasonic sensor that measures whether a car is present. The sensor data is sent over WiFi to a shared FastAPI backend running on a Raspberry Pi on the HvA network. The backend stores every reading in PostgreSQL and exposes a REST API. A web dashboard shows all parking spots with their current status (occupied or free) updating every 2 seconds. Drivers can check the dashboard before leaving home or while approaching a neighborhood, instead of circling once they arrive. The idea is straightforward: if drivers can see which spots are free before they start driving, they do not need to circle the block.

## What I am NOT solving

- Parking policy or pricing. This system detects occupancy, it does not manage permits or tariffs.
- Payment integration. There is no connection to parking meters or payment apps.
- Enforcement. The system does not detect illegal parking or overstaying.
- Scale. This is a proof of concept on a miniature city model with 3 parking spots, not a citywide deployment.

## Connection to learning outcomes

This problem touches all four learning outcomes. Analysis: researching how parking search traffic affects Amsterdam residents and what data is needed to reduce it. Design: choosing the right sensor, communication protocol, and database schema for real time parking data. Realise: building the working system from ESP32 firmware to backend API to dashboard. Advise: designing the shared backend so all team members can integrate their tiles, and advising on deployment and hardware choices.
