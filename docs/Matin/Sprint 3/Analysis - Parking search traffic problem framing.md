# Analysis — Parking search traffic problem framing

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | May 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group — HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document analyzes the city problem behind my parking tile and identifies what was missing in my Sprint 1-2 evidence. Mats' feedback forced me to answer the question I had been avoiding: why does this matter?

### Main question

What city problem does my parking tile address, and why was this missing from my earlier evidence?

### Sub-questions

1. What is the parking search traffic problem?
2. What was wrong with my Sprint 1-2 framing?

---

## Chapter 1 — The parking search traffic problem

### Context

I build a parking occupancy detection system on a miniature city tile. But I never wrote down why.

### Findings

Up to 30% of inner-city traffic in busy areas consists of cars circling for a free parking spot (Shoup, 2017). In Amsterdam, this contributes to CO2 emissions, noise, and congestion. Amsterdam targets emission-free traffic in the city center by 2030 (Gemeente Amsterdam, 2019).

Real-time parking occupancy data could reduce this search traffic. If a driver knows which spots are free before they drive, they do not need to circle.

My tile demonstrates this with 3 miniature parking spots, an ultrasonic sensor, and a backend that tracks occupancy in real time. The dashboard shows which spots are free. The speed camera tile (Learning Goal 1) extends the story: a smart city that detects both where you can park and whether you are driving safely.

### Limitations

- Shoup's 30% figure is from US cities. Amsterdam-specific data is limited.
- My proof of concept uses 3 spots and a toy car. Scaling conclusions are speculative.
- No user testing. No interviews with drivers or residents.

### Sub-conclusion

The problem is real and documented in literature. My tile addresses it at proof-of-concept scale. The biggest gap is that I have no primary research to validate stakeholder needs.

---

## Chapter 2 — What was wrong with my Sprint 1-2 framing

### Context

Mats gave me 1/4 on all learning goals in Sprint 2. His feedback: "Choose a human-centered problem from the city and describe what you are going to do to address it."

### Findings

My Sprint 1-2 evidence focused on "I built this":
- "I set up FastAPI with PostgreSQL"
- "I designed a generic sensor_readings table"
- "I deployed Docker on the Pi"

None of this answered "why". Assessors saw code without a story. Every teammate had a problem analysis and learning goals. I had none.

What was missing:
- No problem statement connecting code to a city issue
- No learning goals in "I want to learn how to..." format
- No recorded expert feedback
- No stakeholder analysis

### Sub-conclusion

The technical work was fine. The framing was absent. Assessors cannot grade what they cannot see. Building without documenting the "why" looks like building without thinking.

---

## Conclusion

The parking search traffic problem is real: up to 30% of inner-city traffic in busy areas (Shoup, 2017). My tile addresses it at PoC scale. The reason Mats gave 1/4 was not bad code but missing framing. Sprint 3 fixes this with a problem statement (`problemStatement.md`, commit `dbb1bc8`) and restructured Portflow evidence.

---

## References

- Shoup, D. (2017). *The High Cost of Free Parking*. Routledge.
- Gemeente Amsterdam. (2019). *Agenda Amsterdam Autoluw.*
- Mats Otten. (2026). Sprint 2 Progress Review feedback. Verbal.
