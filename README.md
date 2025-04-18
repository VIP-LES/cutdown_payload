# Cutdown Payload

## Overview

This repository contains the payload-side firmware for our remote cutdown and FCC-compliant telemetry system. It was designed for a high-altitude balloon mission in Spring 2025.

The payload features an Adafruit Feather M0 RFM9x LoRa board transmitting live GPS, temperature, altitude, and pressure data at distances of 2km or more. While this code is not intended for reuse, it serves as a working proof-of-concept (PoC) of the cutdown system.

A notable success: the nichrome-based cutdown mechanism used a four-point tether to the parachute and balloon, which effectively severed fishing line to release the payload—freeing it from over 50 ft up in a tree.

## Build Instructions

> ⚠️ The Arduino IDE is known for its unique build system. All source files must be either in the top-level sketch directory or inside the `~/Arduino/libraries` folder.

To build this project:

1. Clone this repository:
   ```bash
   git clone <repo-url>
   cd <repo>
   ```

2. Initialize submodules:
   ```bash
   git submodule update --init --recursive
   ```

3. Generate C source files from the protobuf definitions:
   ```bash
   python nanopb/generator/nanopb_generator.py cutdown-packets/messages.proto
   mv cutdown-packets/messages.pb* ./
   ```

4. Open the main `.ino` file in the Arduino IDE and upload it to the Feather board.

5. Hope and pray.

## Code Structure

All code follows a modular task-based system defined in `Job.h`. Each **job** is represented by a class implementing the following functions:

- `initialize()`: Setup logic run once at boot.
- `ready()`: Determines whether the job should run now (e.g., a timer expired).
- `execute()`: The main logic that runs when the job is triggered.
- `poll()`: Optional lightweight checks run regularly.

Jobs are triggered by events such as periodic timers, incoming data (e.g., GPS), or completed messages on serial interfaces. This architecture provides clean separation of responsibilities and simplifies future expansion or debugging.

## What Works

- ✅ **MS8607 Sensor**: Accurately measured temperature, pressure, and humidity.
- ✅ **LoRa Radio (RFM9x)**: Successfully transmitted telemetry packets and received remote commands. Some functions were incomplete and are noted in comments, but the critical cutdown related parts performed just fine.
- ⚠️ **SD Card Logging**: `CSVLogger` and `DataLogger` classes were not fully implemented, but outline the intended data logging design.
- ⚠️ **TelemetryJob**: Sent periodic telemetry packets but failed to correctly include GPS data.