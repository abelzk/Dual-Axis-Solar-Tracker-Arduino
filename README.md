# Dual Axis Solar Tracker Arduino
![Logo](https://socialify.git.ci/abelzk/Dual-Axis-Solar-Tracker-Arduino/image?language=1&owner=1&name=1&stargazers=1&theme=Light)

This project is an implementation of a dual-axis solar tracker using an Arduino. The tracker continuously adjusts the position of a solar panel in two axes (horizontal and vertical) to ensure optimal alignment with the sun. This maximizes the panel's exposure to sunlight, thereby increasing its efficiency.

## Table of Contents
- [Overview](#overview)
- [Components Used](#components-used)
- [Features](#features)
- [Circuit Diagram](#circuit-diagram)
- [Code Overview](#code-overview)
- [Setup Instructions](#setup-instructions)
- [Usage](#usage)
- [Future Improvements](#future-improvements)
- [Author](#author)

## Overview
The dual-axis solar tracker uses Light Dependent Resistors (LDRs) to detect the intensity of sunlight in different directions. Based on these readings, the system controls two servos to adjust the position of the solar panel, ensuring it always faces the direction of maximum sunlight.

Additionally, the system monitors the voltage, current, and power output of the solar panel, displaying this information on an LCD screen and logging it via the Arduino's serial interface.

## Components Used
- **Arduino Uno**: The microcontroller that controls the system.
- **4x LDRs (Light Dependent Resistors)**: Sensors used to detect light intensity.
- **2x Servos (Horizontal and Vertical)**: Motors that adjust the solar panel's position.
- **LiquidCrystal Display (LCD)**: Displays real-time voltage, current, and power readings.
- **Resistors**: Used in voltage division and current sensing circuits.
- **Miscellaneous**: Jumper wires, breadboard, power supply, etc.

## Features
- **Automatic Sun Tracking**: Adjusts the solar panel's orientation in real-time to maximize sunlight exposure.
- **Real-Time Monitoring**: Displays voltage, current, and power output on an LCD screen.
- **Data Logging**: Outputs real-time data to the Arduino IDE's serial monitor for analysis.
- **Servo Limits**: Protects the servos from over-rotation by setting configurable limits.

## Circuit Diagram
_(Include a circuit diagram image here, or describe how the components are connected if a diagram is not available.)_

## Code Overview
The project code is written in Arduino C/C++ and utilizes the following libraries:
- **`Wire.h`**: For I2C communication (if needed for additional components).
- **`LiquidCrystal.h`**: For controlling the LCD display.
- **`Servo.h`**: For controlling the horizontal and vertical servos.

### Key Code Sections
- **Servo Control**:
  - `servoh` and `servov`: Variables that store the current positions of the horizontal and vertical servos.
  - `servohLimitHigh/Low` and `servovLimitHigh/Low`: Set the maximum and minimum angles the servos can move to, preventing over-rotation.
  
- **LDR Connections**:
  - `ldrlt`, `ldrrt`, `ldrld`, `ldrrd`: Analog pins connected to LDRs that measure light intensity in the four quadrants around the solar panel.

- **LCD Display**:
  - Displays real-time voltage, current, and power readings based on sensor data.

### Setup Instructions
1. **Hardware Setup**:
   - Connect the LDRs, servos, and LCD to the Arduino according to the pin assignments in the code.
   - Ensure proper power supply and grounding for all components.

2. **Code Upload**:
   - Open the Arduino IDE and load the provided code.
   - Upload the code to your Arduino Uno.

3. **Calibration**:
   - If needed, adjust the servo limits (`servohLimitHigh/Low` and `servovLimitHigh/Low`) to match the physical constraints of your setup.

### Loop Logic
- The `loop()` function reads the light intensity values from the LDRs and calculates the necessary adjustments for the servos.
- It also monitors the system's voltage, current, and power, updating the LCD and logging data to the serial monitor at regular intervals.

## Usage
1. **Power Up**:
   - Power up the system and watch the solar panel align itself with the brightest light source (i.e., the sun).

2. **Monitor Performance**:
   - Observe real-time performance metrics on the LCD display.
   - Use the serial monitor in the Arduino IDE for detailed data logging.

3. **Adjustments**:
   - If the panel's movement seems off, recalibrate the LDRs or adjust the servo limits in the code.

## Future Improvements
- **Integration with IoT**: Enable remote monitoring and control via an IoT platform.
- **Weatherproofing**: Design an enclosure to protect the system from environmental factors.
- **Battery Storage Integration**: Add a battery storage system to store the generated power.

## Author
**Abel Zecharias**  
Email: [abelzeki24@gmail.com](mailto:abelzeki24@gmail.com)

---

Feel free to reach out if you have any questions or suggestions for improvements!
