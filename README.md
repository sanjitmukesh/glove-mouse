# Glove Mouse

A wearable glove that replaces the traditional computer mouse using an Arduino Pro Micro and MPU-6050 sensor. It detects hand motion and orientation to move the cursor, with buttons for clicks and a re-center function.

## Overview
The Glove Mouse translates 3D hand motion into 2D cursor movement. It uses gyroscope data to detect rotation and an algorithm to stabilize drift. Four mechanical switches simulate mouse clicks, and an extra button lets the user re-center the cursor.

## Components
- Arduino Pro Micro (ATmega32U4)
- MPU-6050 IMU sensor
- 4 mechanical switches
- Glove
- Flexible wiring, hot glue

## Features
- 3D motion tracking → smooth, drift-free cursor movement  
- Left-click, right-click, re-center, and scroll buttons  
- USB HID mouse functionality (no extra software required)  
- Comfortable wearable design
