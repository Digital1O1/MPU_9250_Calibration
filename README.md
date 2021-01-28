## Main Motivation

The purpose of this repo is to serve as a guideline of how to properly use the calibration functions in the  @bolderflight MPU9250 library.

Credit also rightfully goes to @GiovanniCmpaner for clarifying how to properly use the calibrateAccel() function and a reference link is provided where the discussion was brought up on how to use the calibrateAccel() function properly.

Reference links :

- [MPU9250 library](https://github.com/bolderflight/MPU9250)

- [Proper usage of calibrateAccel() function](https://github.com/bolderflight/MPU9250/issues/80)

---

## Wire Hook Up

**KEEP IN MIND THE MPU9250 IS A 3.3V TOLERANT BREAKOUT BOARD**

The wiring table below was based on the pinout from a Teensy 4 board. The SPI communication protocol was used between the Teensy board and the MPU 9250.

NCS is also the Chip Select (CS) pin that's used to send a LOW signal to initiate communication between the MPU 9250 (Or any sensor that's using SPI) and the Teensy 4.

| MPU Breakout Board Pin | Teensy Pin |
| :-------------: | :-------------:|
| VCC | 3.3 V|
| GND | GND |
| SCL | 13  | 
| SDA | 11  | 
| EDA | N/A |
| ECL | N/A | 
| ADO | 12  |
| INT | GND |
| NCS (CS) | 10  |
| FSYNC | GND |

---

## Overview of calibration program

A series of print statements are used throughout the program to notify the user where they are in the calibration process.

After serial communication is established and the IMU.begin() function returns a 1 that indicates the IMU is operational, the user is then prompted via the serial monitor or terminal to press ENTER to initalize the calibration of the acceleromotor. 

![image](https://user-images.githubusercontent.com/39348633/106092802-aca95f80-60f4-11eb-85af-9a7cc119fbe6.png)

Throughout the acceleromotor calibration, the MPU 9250 has to be placed on six different positions, much like placing a dice on it's different faces.

Once that's done, the program will indicate to the user to yet again, press ENTER to initiate the gyroscopic calibration (which happens A LOT faster and is only done once).

As of 1/27/2021, the magometer calibration has yet to be added, but will be in the near future

To calibrate the acceleromotor, the MPU 9250 has to be stationary on six different 'faces', kinda like placing a dice on each face.

![image](https://user-images.githubusercontent.com/39348633/106089503-acf22c80-60ed-11eb-85ac-bd43b0fef243.png)
