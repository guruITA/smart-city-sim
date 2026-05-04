# Advice Document

## Table of Contents

1. [Purpose of This Advice](#1-purpose-of-this-advice)  
2. [Evaluation of the Chosen Software Structure](#2-evaluation-of-the-chosen-software-structure)  
   2.1 [Suitability for the Train Crossing Tile](#21-suitability-for-the-train-crossing-tile)  
   2.2 [Reliability of Data Communication](#22-reliability-of-data-communication)  
   2.3 [Integration Within the Shared ESP32-S3 Project](#23-integration-within-the-shared-esp32-s3-project)  
3. [Reflection on Challenges Encountered](#3-reflection-on-challenges-encountered)  
   3.1 [Structuring as a Library](#31-structuring-as-a-library)  
   3.2 [Ensuring Consistent Backend Communication](#32-ensuring-consistent-backend-communication)  
   3.3 [Integrating Team Components](#33-integrating-team-components)  
4. [Assessment of Reusability, Maintainability, and Collaboration](#4-assessment-of-reusability-maintainability-and-collaboration)  
   4.1 [Reusability](#41-reusability)  
   4.2 [Maintainability](#42-maintainability)  
   4.3 [Collaboration](#43-collaboration)  
5. [Recommendations for the Next Iteration](#5-recommendations-for-the-next-iteration)  
   5.1 [Improve Communication Reliability](#51-improve-communication-reliability)  
   5.2 [Refine the Modular Structure](#52-refine-the-modular-structure)  
   5.3 [Enhance Scalability](#53-enhance-scalability)  
6. [Advice for Improving the Shared Embedded Software Structure](#6-advice-for-improving-the-shared-embedded-software-structure)  

## 1. Purpose of This Advice

In this advice document, I evaluate whether the chosen software structure is suitable for the train crossing tile and whether it supports reliable data communication and integration within the shared ESP32-S3 project.

The evaluation is based on the implemented structure in the shared `embedded/city-sim` codebase and the realization results from Sprint 2.

---

## 2. Evaluation of the Chosen Software Structure

### 2.1 Suitability for the Train Crossing Tile

The current structure is suitable for the train crossing tile because the implementation is encapsulated in the `TrainPredictionSignal` library and integrated through a simple `begin()` and `update()` lifecycle.

Strengths:

- The crossing logic is grouped in one reusable class with configurable constructor parameters.
- Responsibilities are split into focused implementation files (button handling, outputs, and network communication).
- The tile can run in the shared loop together with other components.

Limitations:

- Network behavior is still tightly coupled to train signal logic in the same library instead of a fully separate communication service.
- Some response parsing is manual and therefore less robust.

### 2.2 Reliability of Data Communication

The backend communication flow (create train, update second sensor, update crossed) is present and functional. The implementation also includes graceful behavior when WiFi or backend calls fail, which is important in a shared embedded environment.

Strengths:

- Central network utility (`NetworkController`) avoids duplicated HTTP setup.
- Non-successful requests do not block local crossing behavior.
- URL building is centralized, reducing endpoint formatting mistakes.

Limitations:

- No retry/backoff strategy for transient failures.
- Manual JSON parsing increases risk of parsing errors if response format changes.
- Communication is synchronous, which can impact timing in heavier workloads.

### 2.3 Integration Within the Shared ESP32-S3 Project

The structure supports integration with other team members' tiles because each tile has a clear update cycle and a configurable setup.

Strengths:

- Mergeability improved compared to a standalone sketch.
- The train tile coexists with streetlight and speed camera code in one firmware.
- Hardware behavior remains deterministic through state-based logic.

Limitations:

- Shared resource planning (timers, pins, and communication bandwidth) is not yet formalized for future scale.
- No common event bus yet for cross-tile coordination.

---

## 3. Reflection on Challenges Encountered

### 3.1 Structuring as a Library

Moving from a standalone prototype to library form required clear boundaries around state, hardware outputs, and API calls. This increased maintainability, but it also revealed that architectural separation can still be improved further.

### 3.2 Ensuring Consistent Backend Communication

It was challenging to keep communication reliable while preserving local behavior. The current implementation chooses resilience (continue simulation on failure), which is correct for demonstration continuity, but reliability mechanisms are still basic.

### 3.3 Integrating Team Components

Combining multiple tiles in one shared project introduced practical integration concerns:

- Naming and structure consistency
- Shared loop timing behavior
- Shared network dependencies

The current structure handles this reasonably well, but scaling to more tiles will require stricter shared conventions.

---

## 4. Assessment of Reusability, Maintainability, and Collaboration

### 4.1 Reusability

Assessment: **Good**

The train crossing code is reusable through configurable constructor parameters and isolated behavior methods. It can be instantiated in other contexts with different pin mappings and timing values.

### 4.2 Maintainability

Assessment: **Good, with clear improvement opportunities**

The split across dedicated source files improves readability and ownership. However, maintainability is reduced by manual response parsing and limited error-handling strategy.

### 4.3 Collaboration

Assessment: **Good**

The library-based structure supports team collaboration better than monolithic sketches. Integration in a shared loop is straightforward, but collaboration quality would improve further with shared standards for interfaces, naming, and integration testing.

---

## 5. Recommendations for the Next Iteration

### 5.1 Improve Communication Reliability

Recommended actions:

- Add retry with backoff for failed requests.
- Add request timeout and clearer failure categorization.
- Introduce optional local buffering/queueing for events during temporary network loss.

### 5.2 Refine the Modular Structure

Recommended actions:

- Extract backend protocol handling into a dedicated train communication class.
- Replace manual JSON parsing with a structured JSON library.
- Introduce clearer interfaces between tile logic and communication logic.

### 5.3 Enhance Scalability

Recommended actions:

- Define a shared resource strategy for timers, pins, and communication usage.
- Introduce lightweight event-based coordination between tiles.
- Standardize lifecycle and initialization patterns across all tile libraries.

---

## 6. Advice for Improving the Shared Embedded Software Structure

To better support future expansion of the city simulation, the shared software structure should evolve toward a more explicit platform architecture:

1. Define a common tile contract (setup, update, health status, optional diagnostics).
2. Introduce a shared communication abstraction with reusable request/response handling.
3. Add integration tests that run multi-tile scenarios in one build.
4. Add coding standards for folder structure, API conventions, and naming.
5. Introduce observability hooks (uniform logging and error reporting per tile).

This direction will keep the project mergeable and maintainable as the number of tiles and contributors grows.
