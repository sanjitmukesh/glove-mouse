# Glove Mouse

A wearable glove that replaces the traditional computer mouse using an Arduino Pro Micro and MPU-6050 sensor. It detects hand motion and orientation to move the cursor, with buttons for clicks and a re-center function.

## Overview
The Glove Mouse translates 3D hand motion into 2D cursor movement. It uses gyroscope data to detect rotation and an accelerometer to stabilize drift. Mechanical switches simulate mouse clicks, and an extra button lets the user re-center the cursor.

## Components
- Arduino Pro Micro (ATmega32U4)
- MPU-6050 IMU sensor
- 3 mechanical switches
- Glove
- Flexible wiring, Gorilla Glue, bolts, optional 3D-printed shell

## Features
- 3D motion tracking → smooth, drift-free cursor movement  
- Left/right click + re-center button  
- USB HID mouse functionality (no extra software required)  
- Comfortable wearable design
