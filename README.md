# Arduino Automatic Door Control System with Environmental Monitoring

## Overview
This project is an automated door control system that uses an Arduino to operate a motorized door based on ambient light conditions. The system also monitors temperature, humidity, and pressure using a BME280 sensor and displays all information on a 0.96" OLED display. The door automatically opens when it's bright outside and closes when it gets dark, making it ideal for chicken coops, greenhouses, pet doors, or any application requiring light-based automation with environmental monitoring.

## Features
- **Light-Based Operation**: Opens the door when brightness exceeds threshold, closes when darkness is detected
- **Environmental Monitoring**: Tracks temperature, humidity, and barometric pressure
- **Real-Time Display**: Shows system status and sensor readings on OLED display
- **Confirmation Check**: Double-checks light readings before closing to prevent accidental operation
- **Status Logging**: Provides detailed serial output for monitoring and debugging
- **Configurable Parameters**: Easy adjustment of light thresholds and timing values

## Circuit Diagram

```
                     +---------------------+
                     |                     |
                     |     ARDUINO UNO     |
                     |                     |
+----------------+   |                     |
|                |   |                     |
| Light Sensor   |---| A1             D2  |--- Motor Driver (Down)
| (Photoresistor)|   |                     |
|                |   |                     |
+----------------+   |                D3  |--- Motor Driver (Up)
                     |                     |
                     |                     |
                     |                     |
+----------------+   |                     |
|                |   |     SDA (A4)        |
|  BME280        |---|                     |
|  Temp/Humid/   |   |     SCL (A5)        |
|  Pressure      |---|                     |
|                |   |                     |
+----------------+   |                     |
                     |                     |
+----------------+   |                     |
|                |   |     SDA (A4)        |
|  0.96" OLED    |---|                     |
|  Display       |   |     SCL (A5)        |
|  (SSD1306)     |---|                     |
|                |   |                     |
+----------------+   |                     |
                     +---------------------+
                              |
                              | Serial (9600 baud)
                              |
                     +---------------------+
                     |                     |
                     |    Serial Monitor   |
                     |                     |
                     +---------------------+
```

## Hardware Requirements
- Arduino board (Uno, Nano, or similar)
- Motor driver or relay module (to control door motor)
- DC motor for door operation
- Photoresistor/light sensor connected to analog pin A1
- 10kΩ resistor (for photoresistor voltage divider)
- BME280 temperature/humidity/pressure sensor
- 0.96" OLED display (SSD1306, I2C interface)
- Power supply appropriate for your motor
- Door mechanism (cables, pulleys, etc.)

## Pin Configuration
- **A1**: Light sensor input (analog)
- **A4/SDA**: I2C data line (for BME280 and OLED)
- **A5/SCL**: I2C clock line (for BME280 and OLED)
- **D2**: Door down motor control
- **D3**: Door up motor control

## I2C Addresses
- OLED Display: 0x3C (default, may be 0x3D on some modules)
- BME280 Sensor: 0x76 (default, may be 0x77 on some modules)

## Required Libraries
- Adafruit SSD1306 (for OLED display)
- Adafruit GFX (dependency for SSD1306)
- Adafruit BME280 (for temperature/humidity/pressure sensor)
- Wire (built-in I2C library)

To install libraries in Arduino IDE:
1. Go to Tools > Manage Libraries
2. Search for each library name
3. Click Install

## Installation and Setup
1. Connect the hardware according to the circuit diagram
2. Install the required libraries
3. Upload the provided Arduino sketch to your board
4. Calibrate the light thresholds if necessary
5. Mount the light sensor where it can accurately detect outdoor light conditions
6. Place the BME280 sensor in a suitable location for environmental monitoring
7. Connect the motor to your door mechanism

## Code Configuration
You can adjust the following parameters in the code to match your specific requirements:

```cpp
const int BRIGHTNESS_HIGH_THRESHOLD = 400;  // Threshold for raising door
const int BRIGHTNESS_LOW_THRESHOLD = 100;   // Threshold for lowering door
const int DOOR_MOVEMENT_DELAY = 11000;      // Time required for door movement (milliseconds)
const int SENSOR_READ_DELAY = 2000;         // Delay between sensor readings (milliseconds)
const int DISPLAY_REFRESH_INTERVAL = 5000;  // Refresh display every 5 seconds
```

## Display Information
The OLED display shows:
- System status (ready, opening, closing)
- Current temperature and humidity
- Light level reading
- Door position (open/closed)

## Operation Logic
1. The system reads the brightness value from the light sensor and environmental data from BME280
2. It displays this information on the OLED screen
3. If brightness is above the high threshold and the door is closed, it opens the door
4. If brightness is below the low threshold and the door is open, it waits and confirms the reading before closing the door
5. The system continuously monitors light levels and maintains door position when no action is needed
6. Environmental data and door status are regularly updated on the display

## Troubleshooting
- **Door timing issues**: Adjust `DOOR_MOVEMENT_DELAY` to match your specific door and motor
- **Inconsistent operation**: Check light sensor placement and adjust thresholds
- **Motor not moving**: Verify connections and power to the motor driver
- **BME280 not detected**: Check I2C address (try 0x77 if 0x76 doesn't work)
- **OLED not working**: Verify I2C address and connections

## Future Improvements
- Add limit switches to detect when door is fully open/closed
- Create a mobile app for remote monitoring and control
- Add manual override button


