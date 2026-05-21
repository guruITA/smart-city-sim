# Sprint 4, Learning Goal 1 - Professionalizing the traffic-light tile by transferring the breadboard setup to a permanent perfboard circuit

## Learning Question

How can I professionalize my traffic-light project by transferring the working breadboard circuit to a permanent perfboard setup that can be mounted in the city tile, while keeping the wiring safe, clear, testable, and reliable?

## S — Situation

At the start of this learning goal, I already have a working four-way traffic-light system. The traffic lights are controlled by an ESP32-S3, an MCP23017 I/O expander, two ULN2803 driver chips, and external LED power. The system has already been tested on a breadboard and can switch the traffic lights in a safe sequence.

This means the functional prototype already works. The main problem is no longer whether the traffic-light logic works, but whether the physical electronics are suitable for a more finished project setup. At the moment, the circuit is still built on a breadboard with loose jumper wires. That is useful for testing and debugging, but it is not a professional or reliable final structure for a city tile.

A breadboard setup is temporary. Wires can come loose, connections can be unclear, and the circuit is harder to mount neatly inside or underneath the tile. This becomes a problem when the project needs to be moved, demonstrated, tested repeatedly, or combined with the 3D printed traffic-light models.

The feedback on my previous learning goal also showed that simply hiding the breadboard or hiding the wires is not strong enough as an independent Embedded and Robotics learning goal. That is more suitable as part of finalizing the project or as part of the Definition of Done. Therefore, I will focus this learning goal on professionalizing the physical electronics.

For this sprint, I want to investigate how I can transfer the working breadboard circuit to a more permanent perfboard setup. This includes deciding how the ESP32-S3, MCP23017, ULN2803 chips, resistors, power lines, ground lines, connectors, and traffic-light wiring should be arranged so the setup becomes more reliable and easier to use in the city tile.

The city tile integration is still important, but it is not the main learning goal by itself. It becomes part of the practical result: the professionalized circuit must be suitable for mounting in or under the tile, and the wires to the traffic-light models must be routed safely and clearly.

## T — Task

My task in this learning goal is to turn the current breadboard-based traffic-light electronics into a more permanent and professional physical setup.

The main goal is to investigate and realise how the circuit can be transferred from a temporary breadboard to a perfboard while keeping the existing traffic-light system working correctly. This means I must not redesign the full traffic-light logic. Instead, I build on the working breadboard version and focus on making the physical electronics more reliable, structured, and suitable for the city tile.

To do this, I will first analyse what is wrong with keeping the project on a breadboard and what requirements a permanent perfboard version must meet. After that, I will design how the circuit should be placed on the perfboard, how the power and ground lines should be routed, how the MCP23017 and ULN2803 chips should be connected, and how the outgoing wires to the traffic-light models should be organized.

I will also think about practical details such as connectors, wire strain relief, separation between signal wiring and LED power wiring, test points, and how the circuit can still be checked or repaired after it is mounted. These choices are important because a permanent circuit should not only work once, but should remain understandable and testable afterwards.

After the design, I will build the perfboard version and test it step by step. I will check whether the ESP32-S3 can still communicate with the MCP23017, whether the ULN2803 drivers still switch the LED channels correctly, whether all traffic lights still follow the correct sequence, and whether the physical wiring is stable enough for use in the tile.

The result I want is a working traffic-light tile setup in which the electronics are no longer a loose breadboard prototype, but a more permanent perfboard circuit that can be mounted neatly and safely. The traffic-light models, hidden wiring, and holes in the tile are part of the final integration, but the main learning is about making the embedded hardware setup more professional and reliable.

## Deliverables

### 1. Analysis document

In this document, I explain why the current breadboard setup is not suitable as a permanent solution for the city tile. I describe the limitations of using a breadboard for a project that must be mounted, moved, demonstrated, and tested repeatedly.

I also analyse what the permanent setup must be able to do. This includes keeping the existing traffic-light functionality, supporting the ESP32-S3, MCP23017, ULN2803 chips, external LED power, common ground, current-limiting resistors, and outgoing wires to the traffic-light models.

The analysis also includes the risks of transferring the circuit to perfboard, such as wrong connections, unclear wiring, weak solder joints, short circuits, difficult debugging, and poor cable organization. I define when the perfboard version can be considered successful.

### 2. Design document

In this document, I show how I will transfer the breadboard circuit to a perfboard design. I describe the placement of the main components, the routing of power and ground, the connection between the ESP32-S3 and MCP23017, the connection between the MCP23017 and ULN2803 chips, and the outgoing wiring to the traffic-light LEDs.

I also include a clear mapping table so the physical perfboard wiring can be traced back to the existing traffic-light channels. The design should show how the circuit remains understandable, testable, and safe after the transfer.

The design also explains how the perfboard will fit into the city tile setup. This includes where the wires leave the board, how the wires go toward the traffic-light models, how the wiring can be hidden under the tile, and how the LED connections are kept separate so they do not interfere with each other.

### 3. Realisation document

In this document, I record how I build the perfboard version of the traffic-light circuit. I describe how I transfer the circuit from the breadboard to the perfboard, how I solder and connect the components, how I organize the wiring, and how I connect the perfboard to the traffic-light models in the tile.

I also document the tests I use during the realisation. These tests should not only check the final result, but also help verify the circuit step by step. For example, I test power and ground first, then I2C communication, then separate output channels, and finally the complete traffic-light sequence.

The realisation document must prove that the perfboard setup works reliably and that the circuit is suitable for use as a more permanent part of the city tile.

### 4. Advice document

In this document, I evaluate whether transferring the breadboard setup to perfboard was a good way to professionalize the traffic-light tile.

I describe what worked well, what problems I found, and what I would improve in a next version. This can include improvements to soldering, wire routing, connector use, component placement, repairability, or the way the perfboard is mounted in the tile.

The advice document should also explain whether the setup is now good enough to continue with future smart traffic-light features, such as more sensors, backend communication, or improved physical integration.

## A — Action

## R — Result

## R — Reflection

## T — Transfer

## Appendix

## References