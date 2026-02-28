# dslrbooth360 Automation

## Overview
This repository contains the necessary scripts and Arduino firmware to automate a 360 photo booth, integrating it directly with dslrBooth software. The system uses a Windows batch script triggered by dslrBooth to send a serial command to an Arduino, which then controls the booth's motor for a set duration.

## Features
- **dslrBooth Integration:** Automatically starts the 360 platform when the dslrBooth countdown begins.
- **Arduino Control:** Uses an Arduino for reliable, timed control of the booth's motor via a relay.
- **Hardware Kill Switch:** Includes a physical button to immediately and safely stop the booth's operation at any time.
- **Simple Communication:** Leverages serial communication for a robust link between the PC and the Arduino.

## Components
### `sketch_360_wKillSwitch.ino`
This is the firmware for the Arduino microcontroller.
- **Functionality:** It listens for a `BOOTH_ON` command over the serial port. Upon receiving the command, it initiates a sequence:
    1. Sends a 200ms pulse to start the booth's motor (via a relay).
    2. Waits for 10 seconds.
    3. Sends another 200ms pulse to stop the motor.
- **Hardware Pins:**
    - `Pin 2`: Output to the relay module that controls the motor.
    - `Pin 3`: Input for the hardware kill switch button (wired with an internal pull-up).
- **Kill Switch Logic:** If the button connected to `Pin 3` is pressed (grounded), the sequence is immediately aborted, and a stop pulse is sent to the relay.

### `BoothON.bat`
A Windows batch script that acts as the bridge between dslrBooth and the Arduino.
- **Functionality:** This script is designed to be executed by dslrBooth's "Triggers" feature. When called, it uses a command-line serial utility (`serialsend.exe`) to send the `BOOTH_ON` string to the Arduino connected to COM port 4 at a 9600 baud rate.

## Prerequisites
### Hardware
- An Arduino-compatible board (e.g., Arduino Uno, Nano).
- A 5V relay module.
- A momentary push button.
- A 360 photo booth platform with a motor controllable by a relay.
- Wires for connecting the components.

### Software
- dslrBooth Professional for Windows.
- Arduino IDE.
- A command-line serial utility for Windows. The batch script is configured for `serialsend.exe`, which is not included in this repository but can be found here: https://batchloaf.wordpress.com/serialsend/.

## Setup and Usage
1.  **Hardware Wiring:**
    - Connect the relay module's signal pin to **Pin 2** on the Arduino.
    - Connect the momentary push button between **Pin 3** and **GND** on the Arduino.
    - Wire your 360 booth's motor control through the relay according to the relay module's specifications.

2.  **Arduino Setup:**
    - Open `sketch_360_wKillSwitch.ino` in the Arduino IDE.
    - Connect your Arduino to the PC.
    - Select the correct board and COM Port from the `Tools` menu.
    - Click **Upload** to flash the firmware to the Arduino.

3.  **Windows PC Setup:**
    - Create a folder on your PC (e.g., `C:\dslrBooth\scripts`).
    - Place `BoothON.bat` and your chosen serial utility (e.g., `serialsend.exe`) into this folder.
    - **Important:** If your Arduino is not on COM4, edit `BoothON.bat` and change the `/devnum 4` parameter to match the correct COM port number.
    - Open dslrBooth and navigate to `Settings > Triggers`.
    - Click `+` to add a new trigger.
    - Set the **Event** to **Countdown Start**.
    - Set the **Action** to **Run Executable**.
    - For the **Executable Path**, browse to and select the `BoothON.bat` file.
    - Save the settings.

## Workflow
1. A user starts a session in dslrBooth.
2. The recording countdown begins, triggering the `Countdown Start` event.
3. dslrBooth executes `BoothON.bat`.
4. The batch script sends the `BOOTH_ON` command to the Arduino over the serial port.
5. The Arduino receives the command and runs its sequence, starting and then stopping the 360 platform after 10 seconds.
6. The operator can press the hardware kill switch at any point to immediately stop the platform.
