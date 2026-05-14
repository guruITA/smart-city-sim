# Analysis document
## Table of Contents

1. [Context of Sprint 2 Development](#context-of-sprint-2-development)  
2. [Analysis of Reusable and Mergeable Code](#analysis-of-reusable-and-mergeable-code)  
   2.1 [Current Code Structure Analysis](#current-code-structure-analysis)  
   2.2 [Recommended Modular Breakdown](#recommended-modular-breakdown)  
3. [Requirements for Code Structure](#requirements-for-code-structure)  
   3.1 [Backend Integration Requirements](#backend-integration-requirements)  
   3.2 [Multi-Tile Integration Requirements](#multi-tile-integration-requirements)  
   3.3 [Hardware Abstraction Requirements](#hardware-abstraction-requirements)  
4. [Risks Analysis](#risks-analysis)  
   4.1 [Unclear Responsibilities](#unclear-responsibilities)  
   4.2 [Code Duplication](#code-duplication)  
   4.3 [Inconsistent Communication](#inconsistent-communication)  
   4.4 [Merge Conflicts](#merge-conflicts)  
5. [Acceptance Criteria](#acceptance-criteria)  
   5.1 [Modularity Criteria](#modularity-criteria)  
   5.2 [Reusability Criteria](#reusability-criteria)  
   5.3 [Scalability Criteria](#scalability-criteria)  
   5.4 [Integration Criteria](#integration-criteria)  
   5.5 [Testability Criteria](#testability-criteria)  
   5.6 [Maintainability Criteria](#maintainability-criteria)  

---

## Context of Sprint 2 Development

Sprint 2 focuses on transforming the standalone train crossing prototype into a reusable component suitable for integration within the shared ESP32-S3 city simulation project.

The City Simulation Learning Group aims to create a collaborative platform where multiple "tiles" (representing different city infrastructure components like traffic lights, parking sensors, and railroad crossings) can coexist on shared ESP32-S3 hardware. The current train crossing implementation is not suitable for this integration because it assumes exclusive control over hardware resources and lacks the modularity required for shared environments.

---

## Analysis of Reusable and Mergeable Code

In the context of the City Simulation Learning Group, reusable code refers to components that can be instantiated multiple times with different configurations, shared across different tiles, and easily integrated into larger systems without conflicts. Mergeable code means components that can be combined with other tiles' code without causing resource conflicts, naming collisions, or architectural incompatibilities.

### Current Code Structure Analysis

The existing `TrainPredictionSignal` class combines multiple responsibilities:
- Hardware control (LEDs, buzzer, servo, button)
- State management (IDLE, MEASURING, WAITING)
- Timing calculations
- HTTP communication
- Button debouncing

### Recommended Modular Breakdown

The train crossing functionality should be divided into the following separate modules/classes:

#### 1. TrainDetector Component
**Responsibilities**: Handle sensor inputs, timing calculations, train state tracking  
**Interface**: Provide train detection events and predicted arrival times  
**Configurable**: Pin assignments, distance measurements, timing parameters  
**Reusable**: Can be used by other tiles that need train detection (e.g., traffic lights that respond to approaching trains)  

#### 2. SignalController Component
**Responsibilities**: Control visual and audible signals (LEDs, buzzer, barrier servo)  
**Interface**: Accept signal state commands (idle, warning, active)  
**Configurable**: Pin assignments, timing intervals, signal patterns  
**Reusable**: Signal patterns can be adapted for different types of crossings or warning systems  

#### 3. CommunicationHandler Component
**Responsibilities**: Manage backend communication, data   serialization  
**Interface**: Send train events, receive configuration updates  
**Configurable**: API endpoints, retry logic, data formats  
**Reusable**: Shared communication layer for all tiles requiring backend integration

#### 4. CrossingController Component
**Responsibilities**: Orchestrate the interaction between detector, signals, and communication  
**Interface**: High-level control (start/stop monitoring, emergency override)  
**Configurable**: Integration parameters, safety margins  
**Reusable**: Template for other infrastructure controllers  

---

## Requirements for Code Structure

To support backend data sending and integration with other tiles, the code structure must meet the following requirements:

### Backend Integration Requirements
1. **Asynchronous Communication**: HTTP requests should not block other tile operations
2. **Error Handling**: Robust retry mechanisms and fallback behavior when backend is unavailable
3. **Data Standardization**: Consistent JSON schemas for train events and sensor data
4. **Configuration Management**: Runtime configuration updates from backend
5. **Resource Efficiency**: Minimal memory footprint to allow coexistence with other tiles

### Multi-Tile Integration Requirements
1. **Resource Sharing**: No exclusive hardware resource claims (pins, timers, WiFi)
2. **Event-Driven Architecture**: Publish/subscribe pattern for inter-tile communication
3. **Configuration Isolation**: Each tile instance maintains separate configuration
4. **Conflict Resolution**: Mechanisms to handle pin conflicts or resource contention
5. **Modular Initialization**: Independent setup and teardown of components

### Hardware Abstraction Requirements
1. **Pin Multiplexing**: Support for shared or dynamically assigned pins
2. **Timer Management**: Shared timer resources with priority-based allocation
3. **Interrupt Handling**: Non-blocking interrupt service routines
4. **Power Management**: Support for sleep modes and power-saving states

---

## Risks Analysis

### Unclear Responsibilities
**Risk**: Components with overlapping concerns lead to maintenance confusion  
**Impact**: Difficult debugging, inconsistent behavior, maintenance overhead  
**Mitigation**: Clear separation of concerns with well-defined interfaces  

### Code Duplication
**Risk**: Similar logic (e.g., button debouncing, LED blinking) replicated across tiles  
**Impact**: Increased maintenance burden, potential for inconsistent implementations  
**Mitigation**: Shared utility libraries and common base classes

### Inconsistent Communication
**Risk**: Different tiles using varying API patterns or data formats  
**Impact**: Backend integration complexity, data integrity issues  
**Mitigation**: Standardized communication protocols and shared client libraries  

### Merge Conflicts
**Risk**: Hardcoded pin assignments conflict with other tiles' requirements  
**Impact**: Compilation failures, runtime hardware conflicts  
**Mitigation**: Configuration-driven pin assignment and resource negotiation  

---

## Acceptance Criteria

A code structure suitable for the design and realization phase must satisfy the following acceptance criteria:

### Modularity Criteria
- Each component has a single, well-defined responsibility
- Components can be instantiated independently with configuration parameters
- Clear interfaces between components with minimal coupling
- No hardcoded values; all configuration through parameters or external files

### Reusability Criteria
- Components can be reused across different tile types
- Configuration allows for different hardware setups (pin assignments, parameters)
- Code is platform-agnostic within ESP32-S3 constraints
- Documentation includes usage examples for different scenarios

### Scalability Criteria
- Memory usage scales linearly with component complexity
- CPU usage remains bounded under normal operation
- Support for multiple instances of the same component type
- Graceful degradation when resources are constrained

### Integration Criteria
- Successful compilation when merged with other tile codebases
- No hardware resource conflicts with existing tiles
- Compatible with shared WiFi and backend communication infrastructure
- Event-driven communication with other tiles when required

### Testability Criteria
- Unit tests for individual components without hardware dependencies
- Integration tests for component interaction
- Mock implementations for external dependencies (backend, hardware)
- Automated testing pipeline integration

### Maintainability Criteria
- Clear code documentation and API references
- Consistent coding standards across all components
- Version control friendly (no binary dependencies)
- Error logging and debugging support
