# Environmental Monitoring and Control System

## Overview
This project monitors ambient temperature and light levels using sensors and displays the data on a 16x2 LCD. Users can set thresholds and control a DC motor via buttons. A LED simulates a light, turning on when the ambient light falls below the set threshold. UART interface allows testing and data reading from a PC.

## Components
- ATmega2560 microcontroller  
- LM35 temperature sensor (analog)  
- Light-dependent resistor (LDR)  
- 16x2 LCD with I2C interface  
- 3 push-buttons (navigation, select)  
- Status LED  
- DC motor controlled via PWM  
- UART interface for testing and monitoring  

## Features
- **Interactive LCD menu**: navigate with buttons to set temperature/light thresholds and motor status/speed.  
- **Automatic control**:  
  - Motor turns on if temperature exceeds threshold.  
  - LED lights up when light level is below threshold.  
- **UART commands** for testing and data monitoring:  
  - `normal_mode` → stream sensor data to PC  
  - `test_mode` → stop streaming and test components (`test_led`, `test_temp`, `test_lcd`, `test_motor`)  

## Sensor Readings
- LM35 → temperature in °C  
- LDR → ambient light level  

## Motor Control
- PWM signal via hardware timer  
- Motor speed adjustable from menu  

## LED Control
- Lights up when LDR value is below threshold  
- Can be tested via UART  

## Diagrams
