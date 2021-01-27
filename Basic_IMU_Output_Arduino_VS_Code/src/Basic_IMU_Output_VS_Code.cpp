#include <Arduino.h>
#include "MPU9250.h"
/*
  Make sure the following are included in the platformio.ini file
    * board_build.f_cpu = 60000000L
    * lib_deps = bolderflight/Bolder Flight Systems MPU9250@^1.0.2
    * monitor_speed = 115200
*/

// an MPU9250 object with the MPU-9250 sensor on SPI bus 0 and chip select pin 10
MPU9250 IMU(SPI, 10);
int status;

void setup()
{
  // serial to display data
  Serial.begin(115200);
  while (!Serial)
  {
  }

  // start communication with IMU
  status = IMU.begin();
  if (status < 0)
  {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1)
    {
    }
  }
}

void loop()
{
  // read the sensor
  IMU.readSensor();
  // display the data
  Serial.print(IMU.getAccelX_mss(), 6);
  Serial.print("\t");
  Serial.print(IMU.getAccelY_mss(), 6);
  Serial.print("\t");
  Serial.print(IMU.getAccelZ_mss(), 6);
  Serial.print("\t");
  Serial.print(IMU.getGyroX_rads(), 6);
  Serial.print("\t");
  Serial.print(IMU.getGyroY_rads(), 6);
  Serial.print("\t");
  Serial.print(IMU.getGyroZ_rads(), 6);
  Serial.print("\t");
  Serial.print(IMU.getMagX_uT(), 6);
  Serial.print("\t");
  Serial.print(IMU.getMagY_uT(), 6);
  Serial.print("\t");
  Serial.print(IMU.getMagZ_uT(), 6);
  Serial.print("\t");
  Serial.println(IMU.getTemperature_C(), 6);
  delay(100);
}