# Feedback - Mats and Gerald (Sprint 4)

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | May 2026 |
| **Purpose** | Capture all teacher feedback, map it to the right deliverable, submit per outcome in Portflow |
| **Sources** | Mats Otten (mayor/assessor), Gerald Stap (mayor/teacher) |

This document collects the feedback Mats and Gerald gave on my Sprint 4 learning goals and on my Analysis, Advise, Design, and Realise documents. It also maps each point to the deliverable it belongs to, so I can submit the right feedback under the right outcome in Portflow.

Note: Gerald gave most of his feedback while reading my Sprint 3 Realise document (speed camera). It applies to how I write all Realise and deliverable documents from now on.

---

## Mats (mayor / assessor)

### On learning goals + Analysis + Advise

- Make it sustainable: show how I make sure the solution lasts (`hoe kan ik verzorgen dat het duurzaam is`).
- Do real research: how do the failures happen, how do we prevent them, how do we catch them.

### On Design

- Show how to set up a system that catches our problems automatically.

### On learning goals (second feedback moment)

- LG3 surprise feature is too unclear. Better not do it as-is. Pick a surprise that ties back to feedback from a previous sprint.
- If a document relates to a previous sprint, make that link clear in the new STARRT.

### On document quality (second feedback moment)

- Introduction must give full context.
- Define the target audience clearly. Not "Gemeente Amsterdam" in general, but which department, technical or not.
- These documents are not written for my teachers.
- (My own note) We work as if we are in a real company. The docs must match that: lots of detail.
- Structure must tell a story. Target audience clear from the start.

---

## Gerald (mayor / teacher)

### On the main question and research process

- The main question for a Design must be a design question, not a research question. For example "How do I design the data model?" or "What design choices and which methods do I use?".
- A choice does not belong in the question. The "why yes / why no" belongs in the body.
- Follow the process order: Analysis > Advise > Design > Realise.
- Make the main question sharper. For backend, something like "How can we demonstrate that the backend does X?".

### On requirements

- Include the requirements. What are the HvA requirements for the Smart City project.
- Each mayor delivery added requirements. Write down clearly which requirements I took along from each delivery.
- From camera data > feedback requirements > mayor requirements: carry them through.
- Stick to the requirements and translate them into the design.
- Test against the requirements in the Realise.

### On reasoning and language

- Justify my choices.
- Conclusions must be better.
- Always use "we", not "I".
- Each challenge appears twice. Show how I reuse it.
- Include everything and test it in the Realise.
- If something is tested, show the test results.

### On document structure and metadata (second feedback)

- Versions before feedback are 0.x. After feedback or a mayor delivery, bump to 1.1, 1.2, etc.
- Classification: make it clear for myself what it means.
- Author: "Matin" is not enough. Say who Matin is, for example backend developer (junior).
- Pick a clear standard and stick to it.
- A document is written for a purpose and a target audience. Make that clear. It is not for the teacher. It is for my group, or the municipality, or an employer.
- Client and target audience are separate things. The client is not "we" or "the mayors". Look at the project: Gemeente Amsterdam, a department. If Mats as mayor is the client, that is fine, but still name the client and keep it separate from the target audience.
- Put the target audience in the introduction (for example "head of ...").
- Introduction must give good context and say who/what everything is. For example: who is Gurpreet? Teammate, senior embedded / robotics.
- Document standard: story > situation (what did not work, why) > conclusion (what it delivers).

### On the Realise specifically

- Explain the code conventions: which ones I use, which I follow, the linter. Reference it in APA.
- Name the type of performance test: stress test, load test, soak test. Explain what each one does, for example soak test to catch a memory leak.
- End with a user test.
- Make the handover to management (beheer) clear in the recommendation, like a pilot / proof of concept.

### On references and conclusion

- Use APA style for references.
- State clearly whether a reference is online or offline.
- References must be clearer overall.
- Put recommendations in their own section.
- No new information in the conclusion or in places it does not belong.

### General writing (final feedback)

- Explain more. Do not assume the reader knows; spell things out.
- Justify more. Back up every claim and choice with a reason.
- Refer more. Point to sources, requirements, earlier documents, and mayor feedback that support what I write.

---

## Project standard (decided for Sprint 4)

These are fixed across every Sprint 4 deliverable so the documents stay consistent.

- **Client**: Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR). This department owns parking and traffic policy, which is exactly what our tiles address. The mayor (Mats Otten) acts as the client's representative / stakeholder in the project.
- **Target audience**: technical readers. Concretely the development team (embedded + backend engineers) and the technical lead at the client side. These are backend documents, so the audience is technical, not a general public.
- **Author**: Matin Khajehfard, Backend Developer (junior).
- **Classification scale (high to low sensitivity)**:
  1. **Restricted** - secrets, credentials, security internals. Never shared outside the core team.
  2. **Confidential** - internal architecture details not for outside parties.
  3. **Internal** - for the team and the client, not public. (Default for these deliverables.)
  4. **Public** - safe to publish.
  I pick the level per document. Most Sprint 4 deliverables are **Internal**.
- **Versioning**: 0.x while the document is in draft / before feedback. After feedback or a mayor delivery, bump to 1.1, 1.2, etc.

---

## Mapping: which feedback goes where

| # | Feedback (short) | From | Deliverable / outcome |
|---|------------------|------|------------------------|
| 1 | Show the solution is sustainable | Mats | Analysis + Advise |
| 2 | Research how failures happen, prevent, catch | Mats | Analysis |
| 3 | System that catches our problems automatically | Mats | Design |
| 4 | LG3 surprise: drop the unclear one, pick one tied to a previous sprint | Mats | Learning Goal 3 |
| 5 | Link to previous sprints in the new STARRT | Mats | Learning Goal (all STARRT docs) |
| 6 | Intro: full context, clear target audience, tell a story | Mats + Gerald | All deliverables (intro) |
| 7 | Docs written as if in a real company, lots of detail | Mats | All deliverables |
| 8 | Design main question must be a design question, not research | Gerald | Design |
| 9 | Choice not in the question; why/why-not in the body | Gerald | Design (+ all main questions) |
| 10 | Follow process Analysis > Advise > Design > Realise | Gerald | Learning Goal (structure) |
| 11 | Sharper main question ("how can we demonstrate X") | Gerald | All deliverables (main question) |
| 12 | Include HvA Smart City requirements | Gerald | Analysis + Design |
| 13 | Track requirements added per mayor delivery | Gerald | Analysis + Design |
| 14 | Carry requirements: camera data > feedback req > mayor req | Gerald | Design |
| 15 | Stick to requirements and translate to design | Gerald | Design |
| 16 | Test against requirements | Gerald | Realise |
| 17 | Justify my choices | Gerald | Advise + Design |
| 18 | Better conclusions | Gerald | All deliverables (conclusion) |
| 19 | Use "we", not "I" | Gerald | All deliverables |
| 20 | Each challenge appears twice, show reuse | Gerald | Learning Goal (structure) |
| 21 | Show test results when tested | Gerald | Realise |
| 22 | Versioning: 0.x before feedback, 1.1/1.2 after | Gerald | All deliverables (metadata) |
| 23 | Classification clear for myself | Gerald | All deliverables (metadata) |
| 24 | Author: say who Matin is (role + level) | Gerald | All deliverables (metadata) |
| 25 | Pick a clear standard and stick to it | Gerald | All deliverables |
| 26 | Client vs target audience separate; name the real client | Gerald | All deliverables (metadata + intro) |
| 27 | Target audience in the intro (e.g. "head of ...") | Gerald | All deliverables (intro) |
| 28 | Intro explains who/what everything is (e.g. who is Gurpreet) | Gerald | All deliverables (intro) |
| 29 | Doc standard: story > situation > conclusion | Gerald | All deliverables |
| 30 | Explain code conventions + linter, APA reference | Gerald | Realise |
| 31 | Name performance test types (stress/load/soak) and purpose | Gerald | Realise |
| 32 | End with a user test | Gerald | Realise |
| 33 | Handover to management in recommendation (pilot/PoC) | Gerald | Realise (recommendation) |
| 34 | APA references, state online/offline | Gerald | All deliverables (references) |
| 35 | Recommendations in own section | Gerald | All deliverables (recommendation) |
| 36 | No new info in conclusion | Gerald | All deliverables (conclusion) |
| 37 | Explain more, do not assume reader knows | Gerald | All deliverables |
| 38 | Justify more, reason behind every claim/choice | Gerald | All deliverables |
| 39 | Refer more, cite sources/requirements/earlier docs/mayor feedback | Gerald | All deliverables |

---

## How I will submit this in Portflow

Per learning group review (Analysis / Advise / Design / Realise), I attach the feedback rows that map to that outcome:

- **Analysis**: rows 1, 2, 6, 7, 12, 13, 18, 19, 22-29, 34-36
- **Advise**: rows 1, 6, 7, 17, 18, 19, 22-29, 34-36
- **Design**: rows 3, 6, 7, 8, 9, 11, 12, 13, 14, 15, 17, 18, 19, 22-29, 34-36
- **Realise**: rows 6, 7, 16, 18, 19, 21, 22-36
- **Learning Goal (STARRT)**: rows 4, 5, 10, 20
