
## Overview
This document explains how to set up and use the Wokwi extension in VS Code for ESP32-based projects. It covers the installation and license activation steps, the required project configuration files, and how to create and use a `diagram.json` file to start simulations.

## Table of Contents

* [Overview](#overview)
* [Prerequisites](#prerequisites)
* [1. Installation Wokwi in VS Code](#1-installation-wokwi-in-vs-code)
  * [1.1 install extension](#11-install-extension)
  * [1.2 Activate the license](#12-activate-the-license)
* [2. Project Configuration](#2-project-configuration)
  * [2.1 Create `wokwi.toml`](#21-create-wokwitoml)
  * [2.2 Create `diagram.json`](#22-create-diagramjson)
* [3. Example: Arduino CLI project for ESP32-S3](#3-example-arduino-cli-project-for-esp32-s3)
  * [3.1 Compile the project](#31-compile-the-project)
  * [3.2 Configure `wokwi.toml` for the project](#32-configure-wokwitoml-for-the-project)
* [4. Creating the circuit in Wokwi](#4-creating-the-circuit-in-wokwi)
* [5. Starting the simulation in VS Code](#5-starting-the-simulation-in-vs-code)
  * [Expected result](#expected-result)
* [6. Reopening `diagram.json` as text](#6-reopening-diagramjson-as-text)
* [7. Troubleshooting](#7-troubleshooting)
  * [License activation does not complete](#license-activation-does-not-complete)
  * [The simulator opens, but nothing runs](#the-simulator-opens-but-nothing-runs)
  * [The wrong file path is used](#the-wrong-file-path-is-used)
  * [`diagram.json` always opens as a simulation](#diagramjson-always-opens-as-a-simulation)
* [References](#references)

## Introduction
Wokwi is a simulator for embedded projects. In this workflow, it is used to test ESP32 hardware setups and firmware behavior in VS Code before deploying to physical hardware.

## Prerequisites
Before you begin, make sure you have: VS Code installed, a Wokwi account, internet access for license activation, Arduino CLI installed, and ESP32 board support configured.

For local simulation in VS Code, Wokwi needs compiled firmware output from your project. In addition, the VS Code setup uses two configuration files in the project root: `wokwi.toml` and `diagram.json`.

## 1. Installation Wokwi in VS Code
To use Wokwi in VS Code, first install the extension and then activate the license.

#### 1.1 install extension
- Open VS Code.
- Open the Extensions view in VS Code.
- Search for `Wokwi Simulator` 
- Click install.

After installation, the extension becomes available in VS Code and can be configured for your local project.

### 1.2 Activate the license
- Press `F1` 
- Run the command `Wokwi: Request a new License` 
- When VS Code asks to open the Wokwi website, click`Open`
- Sign in to your Wokwi account, or create an account if needed.
- In the browser, confirm that the license should be sent to VS Code.
- Return to VS Code and confirm the activation if prompted again.
- Verify that the message `License activated for [your name]` appears.


## 2. Project Configuration
To run a project in Wokwi for VS Code, two files must be placed in the root directory of the workspace:
`wokwi.toml`, which points to the compiled firmware and ELF output, and `diagram.json`, which defines the circuit used by the simulator.

### 2.1 Create `wokwi.toml`
A basic wokwi.toml file looks like this:
```toml
[wokwi]
version = 1
firmware = 'path-to-your-firmware.hex'
elf = 'path-to-your-firmware.elf'
```
Use paths that are relative to the project root. For ESP32-family boards, Wokwi supports firmware files such as .bin, .uf2, .elf, and flasher_args.json, depending on the build system being used. The elf entry is optional, but it can improve the simulation experience in some cases. Use forward slashes in paths so the project remains portable across operating systems.

### 2.2 Create `diagram.json`
The `diagram.json` file defines the simulated hardware layout. It contains the board, connected parts, and electrical connections between components. A minimal structure looks like this:

```json
{
  "version": 1,
  "author": "Your Name",
  "editor": "wokwi",
  "parts": [],
  "connections": [],
  "dependencies": {}
}
```
In practice, this file is usually created in the Wokwi web editor and then copied into the local project.

## 3. Example: Arduino CLI project for ESP32-S3
This section shows one possible workflow for a project built with Arduino CLI.

### 3.1 Compile the project

From the project project folder, compile the firmware:
```bash
arduino-cli compile -v -b esp32:esp32:esp32s3 --output-dir ./build_wokwi
```
This command generates the build output that Wokwi needs to start the simulation. The exact filenames may differ depending on the project and toolchain, so always verify the files that are actually produced in the build_wokwi directory.

### 3.2 Configure `wokwi.toml` for the project
After compiling, update wokwi.toml so that it points to the generated build files. For example:

```toml
[wokwi]
version = 1
firmware = "./build_wokwi/temp.ino.merged.bin"
elf = "./build_wokwi/temp.ino.elf"
```

## 4. Creating the circuit in Wokwi
To create diagram.json, use the Wokwi web editor:

 1. Open a new ESP32 project in the Wokwi web interface.
 2. Select the correct board, for example an ESP32-S3 if that matches your project.
 3. Add and connect the required components, such as sensors, LEDs, or other peripherals.
 4. Open the diagram.json tab in the Wokwi editor.
 5. Copy the JSON content.
 6. Paste that content into the diagram.json file in the root of the local VS Code project.
At this stage, the firmware does not need to be finalized. The purpose of this step is to define the simulated hardware layout so the project can later be tested in VS Code.

## 5. Starting the simulation in VS Code
After wokwi.toml and diagram.json have been added to the project root and the firmware has been compiled, the simulation can be opened in VS Code.
 1. Open the diagram.json file in VS Code.
 2. The Wokwi diagram view opens inside the editor.
 3. Start the simulation from that view.

### Expected result

If the setup is correct:
- the circuit appears in the Wokwi panel inside VS Code
- the simulator starts without configuration errors
- the compiled firmware is loaded into the simulated board
- the project behaves as expected in the virtual circuit
Because Wokwi runs the compiled output, the project must be built before starting the simulator.


## 6. Reopening `diagram.json` as text
Sometimes `diagram.json` opens directly in the simulator view, while you may want to edit the JSON source manually. In that case:

1. Right-click the open diagram.json tab.
2. Select Reopen Editor With.
3. Choose Text Editor.

This reopens the file as plain JSON instead of the simulator view.
    
## 7. Troubleshooting

### License activation does not complete

Check whether you are signed in to your Wokwi account and confirm every browser and VS Code prompt during activation. The process may require confirmation in both places before the license is delivered to the extension.

### The simulator opens, but nothing runs

Make sure the firmware has been compiled and that the paths in `wokwi.toml` are correct. Wokwi for VS Code requires compiled build artifacts before a simulation can start successfully

### The wrong file path is used

Do not use absolute or user-specific paths unless there is a very specific reason to do so. Use relative paths from the root of the project and prefer forward slashes instead of backslashes.

### `diagram.json` always opens as a simulation

Use **Reopen Editor With -> Text Editor** to switch back to editing the raw JSON file. This is useful when you want to adjust parts or connections manually.


## References
_Getting started with Wokwi for VS Code_. (n.d.). Wokwi. https://docs.wokwi.com/vscode/getting-started
_Configuring your project (wokwi.toml)_. (n.d.). Wokwi. https://docs.wokwi.com/vscode/project-config
_Diagram.json file format_. (n.d.). Wokwi. https://docs.wokwi.com/diagram-format


