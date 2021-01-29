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
  IMU.readSensor();
  ax = (IMU.getAccelX_mss());
  ay = (IMU.getAccelY_mss());
  az = (IMU.getAccelZ_mss());
  gx = (IMU.getGyroX_rads());
  gy = (IMU.getGyroY_rads());
  gz = (IMU.getGyroZ_rads());
  mx = IMU.getMagX_uT();
  my = IMU.getMagY_uT();
  mz = IMU.getMagZ_uT();
  Serial.print(ax); Serial.print(',');
  Serial.print(ay); Serial.print(',');
  Serial.print(az); Serial.print(',');
  Serial.print(gx); Serial.print(',');
  Serial.print(gy); Serial.print(',');
  Serial.print(gz); Serial.print(',');
  Serial.print(mx); Serial.print(',');
  Serial.print(my); Serial.print(',');
  Serial.println(mz);
  delay(100);
}
