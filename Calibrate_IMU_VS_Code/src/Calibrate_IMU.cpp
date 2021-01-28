#include <Arduino.h>
#include <Wire.h>
#include <MPU9250.h>
int status;
float axb, ayb, azb, axs, ays, azs,
      ax, ay, az, gx, gy, gz,
      gxb, gyb, gzb = 0;
MPU9250 IMU(SPI, 10);

//---------------------------------------------------- [ DISPLAY FUNCTIONS ] ----------------------------------------------------//

void display_values_table()
{
  Serial.println("--------------- ACCELEROMOTOR DATA --------           --------------- GYROSCOPE DATA ---------------");
  Serial.print("aX : "); Serial.print(ax); Serial.print(' ');
  Serial.print("\taY : "); Serial.print(ay); Serial.print(' ');
  Serial.print("\taZ : "); Serial.print(az);
  Serial.print("\t\tgX : "); Serial.print(gx); Serial.print(' ');
  Serial.print("\tgY : "); Serial.print(gy); Serial.print(' ');
  Serial.print("\tgZ : "); Serial.println(gz);
}
void display_values_serial()
{
  Serial.print(ax); Serial.print(',');
  Serial.print(ay); Serial.print(',');
  Serial.print(az); Serial.print(',');
  Serial.print(gx); Serial.print(',');
  Serial.print(gy); Serial.print(',');
  Serial.println(gz);
}
void setup()
{
  //---------------------------------------------------- [ ESTABLISH SERIAL COMMUNICATION AND CHECK IMU ] ----------------------------------------------------//
  Serial.begin(115200);
  Serial.println("-------------[ CHECKING IMU STATUS ]-------------\n");

  // If something is wrong with the IMU, IMU.begin() will return a -1 and an error message will pop up on the serial monitor
  status = IMU.begin();
  if (status < 0)
  {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1) {}
  }
  else
  {
    delay(1200);
    Serial.println("-------------[ IMU IS OPERATIONAL ]-------------\n");
  }

  //---------------------------------------------------- [ CALIBRATE ACCELEROMOTOR ] ----------------------------------------------------//
  Serial.println("========================================================================================================================");
  Serial.println("\n-------------[ CALIBRATING ACCELEROMOTOR ]-------------\n");
  Serial.println("To initiate ACCELEROMOTOR calibration press [ ENTER ]...");

  // Calibration won't start until user inputs 'ENTER' in the serial monitor
  while (true)
  {
    // Checks serial monitor for incoming data
    if (Serial.available() > 0)
    {
      // If user presses 'ENTER' break out of while loop
      if (Serial.read() == '\n')
      {
        Serial.println("\n\nCALIBRATION IN PROGRESS");
        break;
      }
    }
    Serial.print(".");
    delay(1000);
  }
  delay(1200);
  Serial.println("\nOrientate IMU to desiered INITAL position to calibrate all SIX axes");

  // This is where the actual calibration takes place. Orientate the IMU sensor in SIX different directions AND MAKE SURE IT'S STABLE
  for (int n = 1; n <= 6; n++)
  {
    Serial.print("\nNumber of calibration iterations : ");
    Serial.print(n); Serial.println(" \\ 6\n");

    // Countdown timer
    for (int i = 5; i > 0; i--)
    {
      Serial.print(i);
      Serial.println("...");
      delay(1000);
    }
    Serial.println("\n-------------[ CALIBRATION IN PROGRESS ]-------------");

    IMU.calibrateAccel();

    Serial.println("\n-------------[ Change IMU posistion ]-------------");
  }
  Serial.println("\n-------------[ Calibrating accelerometer COMPLETE ]-------------\n");

  // Get Scale factor values
  // Scale factors are the ratio between the measured output and the change in the 'sense' input
  // More info about scale factors can be found here : https://www.edn.com/evaluating-inertial-measurement-units/
  axs = IMU.getAccelScaleFactorX();
  ays = IMU.getAccelScaleFactorY();
  azs = IMU.getAccelScaleFactorZ();

  // Biased Factors
  axb = IMU.getAccelBiasX_mss();
  ayb = IMU.getAccelBiasY_mss();
  azb = IMU.getAccelBiasZ_mss();

  // Display scale/bias values
  Serial.println("\n-------------[ SCALE FACTORS ]-------------\n");

  Serial.print("X_Scale Factor : "); Serial.print(axs);
  Serial.print(" Y_Scale Factor : "); Serial.print(ays);
  Serial.print(" Z_Scale Factor : "); Serial.println(azs);

  Serial.println("\n-------------[ BIAS VALUES]-------------\n");

  Serial.print("X_Biased : "); Serial.print(axb);
  Serial.print(" Y_Biased : "); Serial.print(ayb);
  Serial.print(" Z_Biased : "); Serial.println(azb);

  // Store scale/bias values into an array
  const float accelBias[3] = {axb, ayb, azb};
  const float accelFactor[3] = {axs, ays, azs};

  // Set the scale/bias values accordingly
  Serial.println("\n-------------[ IMU VALUES SET TO]-------------\n");
  IMU.setAccelCalX(accelBias[0], accelFactor[0]);
  IMU.setAccelCalY(accelBias[1], accelFactor[1]);
  IMU.setAccelCalZ(accelBias[2], accelFactor[2]);

  // Display
  Serial.print("Ax_Bias : "); Serial.print(accelBias[0]);  Serial.print(" | Ax_Accel : "); Serial.println(accelFactor[0]);
  Serial.print("Ay_Bias : "); Serial.print(accelBias[1]);  Serial.print(" | Ay_Accel : "); Serial.println(accelFactor[1]);
  Serial.print("Az_Bias : "); Serial.print(accelBias[2]);  Serial.print(" | Az_Accel : "); Serial.println(accelFactor[2]);
  delay(3000);

  Serial.println("\n========================================================================================================================");
  delay(2000);

  //---------------------------------------------------- [ CALIBRATE GYRO ] ----------------------------------------------------//
  Serial.println("\n-------------[ CALIBRATING GYROSCOPE ]-------------");
  Serial.println("\nPress [ ENTER ] to continue...");

  // Calibration won't start until user inputs 'ENTER' in the serial monitor
  while (true)
  {
    // Checks serial monitor for incoming data
    if (Serial.available() > 0)
    {
      // If user presses 'ENTER' break out of while loop
      if (Serial.read() == '\n')
      {
        Serial.println("\n\nPlace IMU sensor on solid surface and DON'T TOUCH IT");
        delay(2000);

        Serial.println("\nCALIBRATION IN PROGRESS\n");
        break;
      }
    }
    Serial.print(".");
    delay(1000);
  }
  
  // Countdown timer
  for (int i = 5; i > 0; i--)
  {
    Serial.print(i);
    Serial.println("...");
    delay(1000);
  }

  // Where the actual Gyro calibration takes place
  if (IMU.calibrateGyro() == true)
  {
    Serial.println("\nCALIBRATION COMPLETE\n");

    // "Grab" bias values
    gxb = IMU.getGyroBiasX_rads();
    gyb = IMU.getGyroBiasY_rads();
    gzb = IMU.getGyroBiasZ_rads();

    // Set bias values
    IMU.setGyroBiasX_rads(gxb);
    IMU.setGyroBiasY_rads(gyb);
    IMU.setGyroBiasZ_rads(gzb);

    // Print out bias values
    Serial.println("---- GYRO BIAS VALUES SET ----\n");
    Serial.print("GXB : "); Serial.print(gxb); Serial.print(',');
    Serial.print(" GYB : "); Serial.print(gyb); Serial.print(',');
    Serial.print(" GZB : "); Serial.println(gzb);
  }

  Serial.println("\nPress [ ENTER ] to display IMU data...");

  // Calibration won't start until user inputs 'ENTER' in the serial monitor
  while (true)
  {
    // Checks serial monitor for incoming data
    if (Serial.available() > 0)
    {
      // If user presses 'ENTER' break out of while loop
      if (Serial.read() == '\n')
      {
        break;
      }
    }
    Serial.print(".");
    delay(1000);
  }

  // Uncomment if you're exporting the IMU data to something like Excel
  //Serial.println("ax,ay,az,gx,gy,gz");

}

void loop()
{
  // Reads IMU data from sensor
  IMU.readSensor();
  ax = (IMU.getAccelX_mss());
  ay = (IMU.getAccelY_mss());
  az = (IMU.getAccelZ_mss());
  gx = (IMU.getGyroX_rads());
  gy = (IMU.getGyroY_rads());
  gz = (IMU.getGyroZ_rads());

  display_values_table();
  display_values_serial();
  delay(100);
}