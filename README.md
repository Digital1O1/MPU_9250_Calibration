## Main Motivation

The purpose of this repo is to serve as a guideline of how to properly use the calibration functions in the  @bolderflight MPU9250 library.

Credit also rightfully goes to @GiovanniCmpaner for clarifying how to properly use the calibrateAccel() function and a reference link is provided where the discussion was brought up on how to use the calibrateAccel() function properly.

Reference links :

- [MPU9250 library](https://github.com/bolderflight/MPU9250)

- [Proper usage of calibrateAccel() function](https://github.com/bolderflight/MPU9250/issues/80)

---

## Wire Hook Up

**KEEP IN MIND THE MPU9250 IS A 3.3V TOLERANT BREAKOUT BOARD**

The wiring table below was based on the pinout from a Teensy 4 board. 

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

The SPI communication protocol was used between the Teensy board and the MPU 9250.

NCS is just a different way of notating that particular pin is the Chip Select (CS) pin.When it recieves a LOW signal from the Teensy, communication between the two devices is initialized and the IMU data is sent TO the Teensy board.

If multiple MPU 9250's were to be used (Or any sensor that's using SPI) , the NCS/CS pins from each MPU 9250 would have to be 'attached' to a different digital pin on the Teensy.Specifying the digital pin to be used to send the LOW signal to the MPU 9250 is done through the SPI object declaration, which by "default" in this program is pin 10.

The other pins such as the SCL,SDA,ADO can be attached to the same breadboard nodes with the other MPU 9250's

![image](https://user-images.githubusercontent.com/39348633/106200539-c0000d80-617c-11eb-84a7-ed4812c3a171.png)

---

## Overview of calibration program

A series of print statements are used throughout the program to notify the user where they are in the calibration process.

### Establish serial communication and IMU.begin()

After serial communication is established, the function IMU.begin() should return TRUE if nothing is wrong, which if that's the case, the serial monitor or terminal you're using will prompt the user to press ENTER to start calibrating the acceleromotor.

![image](https://user-images.githubusercontent.com/39348633/106092802-aca95f80-60f4-11eb-85af-9a7cc119fbe6.png)

Calibrating the acceleromotor involves the sensor being placed stationary in six different positions much like placing a dice on it's different faces. After pressing ENTER a countdown will start and the user will be prompted when to change the IMU orientation and what calibration iteration they're on.

After calibrating all six sides, the user will be prompted again to press ENTER to initiate the gyroscopic calibration (which happens A LOT faster and is only done once).

As of 1/27/2021, the magometer calibration has yet to be added, but will be in the near future

Pictured below is a high level diaghram of what's going on while the program is running.

![image](https://user-images.githubusercontent.com/39348633/106155224-0d14bd00-6146-11eb-8ebd-6f9f6a96cf07.png)
