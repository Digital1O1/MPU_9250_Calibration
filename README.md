# Main Motivation

The purpose of this repo is to serve as a guideline of how to properly use the calibration functions in the  @bolderflight MPU9250 library.

Credit also rightfully goes to @GiovanniCmpaner for clarifying how to properly use the calibrateAccel() function. A reference link is provided where the discussion was brought up on how to use the calibrateAccel() function properly.

Reference links :

- [MPU9250 library](https://github.com/bolderflight/MPU9250)

- [Proper usage of calibrateAccel() function](https://github.com/bolderflight/MPU9250/issues/80)

This repository has two versions of the same code for those who prefer using the Arduino IDE, and for those who prefer using Visual Code with the Platform IO extension.

### [PLEASE KEEP IN MIND]

If you're using the Arduino IDE and the Teensy 4, you'll have to DECREASE the clock speed by going to :

- Tools --> CPU Speed --> 150 Mhz

For some reason if you're using VS Code with the Platform IO extension, you'll HAVE to include the following in the platformio.ini file :

- monitor_speed = 115200

- board_build.f_cpu = 60000000L

---

# Wire Hook Up

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

NCS is just a different way of notating that particular pin on the MPU 9250 is the Chip Select (CS) pin. When the NCS/CS pin recieves a LOW signal from the Teensy, communication between the two devices is initialized and the IMU data is sent TO the Teensy board.

If multiple MPU 9250's were to be used (Or any sensor that's using SPI in general) , the NCS/CS pins from each MPU 9250 would have to be 'attached' to a different digital pin on the Teensy.Specifying the digital pin to be used to send the LOW signal to the MPU 9250 is done through the SPI object declaration, which by "default" in this program is pin 10 as shown in the code snippet below.

```c++
MPU9250 IMU(SPI, 10);
```

The other pins such as the SCL,SDA,ADO can be attached to the same breadboard nodes with the other MPU 9250's

![image](https://user-images.githubusercontent.com/39348633/106200539-c0000d80-617c-11eb-84a7-ed4812c3a171.png)

---

# Overview of calibration program

A series of print statements are used throughout the program to notify the user where they are in the calibration process.

After serial communication is established, the function IMU.begin() should return TRUE if nothing is wrong, which if that's the case, the serial monitor or terminal you're using will prompt the user to press ENTER to start calibrating the acceleromotor.

![image](https://user-images.githubusercontent.com/39348633/106092802-aca95f80-60f4-11eb-85af-9a7cc119fbe6.png)

---

# Overview of calibration functions

### IMU.calibrateAccel();

Calibrating the acceleromotor involves placing the sensor in six different stationary positions similar when looking at the six different faces of a single dice.

After pressing ENTER a countdown will start.

Once the calibration for that iteration is done, the user will be prompted to change the sensor orientation and notified which iteration (out of 6) they're on.

After all six iterations are done, the user will be prompted both the bias and scale factors which are then applied.

---

### IMU.calibrateGyro();

The user will be prompted again to press ENTER to initiate the gyroscopic calibration (which happens A LOT faster and is only done once).

Once the gyro calibratoin is done, both the gyro scale and bias factors will be applied to the sensor.

---

### IMU.calibrate();

During the magnetometer calibration, the MPU 9250 MUST be continuously and slowly moved in a figure 8 motion. 

Once enough data is collected, both the bias and scale factors for the magnetometer will then be applied. 

---

Pictured below is a high level diagram of what's going on while the program is running.

![image](https://user-images.githubusercontent.com/39348633/106216711-60642b00-6199-11eb-8dcb-70a30699cc86.png)

# Repository Folder Contents

In the Basic_IMU_Ouput folder are two subfolders:
- Basic_IMU_Output_Arduino
- Basic_IMU_Output_Arduino_VS_Code

The main purpose of these programs is to provide a quick way to check if your SPI wiring is correct.

# So...Did the calibration even worked?

Looking at the output data alone, it's extremely hard to tell if the calibration worked or not.

So to determine if it did or not, I :

- Uploaded either the Basic_IMU_Output_Arduino/Basic_IMU_Output_VS_Code to the Teensy.

- Took the MPU 9250 and made circular/figure 8 motions/movements as shown in the gif below.

![](IMU_Demo.gif)
![](figure8.gif)

- Took the uncalibrated gryoscopic/acceleromotor/magnetometer data that was displayed on the serial monitor/terminal copy and pasted the values into a txt file.

- Imported the data from the txt to an Excel spreadsheet.

- Created a line graph with the data provided.

- Repeated the same steps with after calibrating the MPU 9250.

And the results were as followed.

![image](https://user-images.githubusercontent.com/39348633/106338798-581ff480-625a-11eb-9332-00ea4a303028.png)

![image](https://user-images.githubusercontent.com/39348633/106338806-5d7d3f00-625a-11eb-9d8b-80703babe251.png)

![image](https://user-images.githubusercontent.com/39348633/106337914-1ee68500-6258-11eb-97b2-d42a825d9254.png)

![image](https://user-images.githubusercontent.com/39348633/106337963-3de51700-6258-11eb-99f5-680a72526e53.png)

---

# Conclusion

Judging by the generated Excel sheets along with my limited knowledge and experience with Internal Measurement Units (IMUs) like the MPU 9250, it's extremely hard to tell if the calibration is working or not.

But circling back to my original point of creating this repository was to provide a guide/framework for those who are using bolderflight's library and whatever IMU/MPU sensor the user has on hand.

As a academically trained Biomedical engineer, for those who have comments/suggestions/corrections, I would love to hear back from y'all as I'm still learning alot myself as I dive deeper into the Embedded engineering world.
