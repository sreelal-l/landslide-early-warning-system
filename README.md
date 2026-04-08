# Landslide Early Warning System (IoT Based)

## Overview
This project is a low-cost IoT-based landslide detection system using ESP32.

## Features
- Soil moisture monitoring (2 layers)
- Tilt detection using MPU6050
- Vibration detection
- Cloud monitoring using ThingSpeak
- Buzzer alert system

## Hardware Used
- ESP32
- Soil Moisture Sensors
- MPU6050
- Vibration Sensor
- Buzzer

## Working
Sensors collect data → ESP32 processes → Data sent to ThingSpeak → Alert triggered if danger detected.

## Results
System successfully detects changes in soil moisture, tilt, and vibration.

## Future Scope
- AI prediction
- Mobile alerts
- Solar power integration