#include <Arduino.h>
#include <Wire.h>
#include <MPU9250.h>
int status;
float axb, ayb, azb, axs, ays, azs, ax, ay, az, gx, gy, gz,
      gxb, gyb, gzb, hxb, hyb, hzb, set_gxb, set_gyb, set_gzb,
      hxs, hys, hzs, mx, my, mz = 0;

// Declare SPI object and CS pin here
MPU9250 IMU(SPI, 10);

//---------------------------------------------------- [ DISPLAY FUNCTIONS ] ----------------------------------------------------//

void display_values_table()
{
  Serial.println("--------------- ACCELEROMOTOR DATA --------           --------------- GYROSCOPE DATA ---------------           --------------- MAGOMETER DATA ---------------");
  Serial.print("aX : "); Serial.print(ax); Serial.print(' ');
  Serial.print("\taY : "); Serial.print(ay); Serial.print(' ');
  Serial.print("\taZ : "); Serial.print(az);

  Serial.print("\t\tgX : "); Serial.print(gx); Serial.print(' ');
  Serial.print("\tgY : "); Serial.print(gy); Serial.print(' ');
  Serial.print("\tgZ : "); Serial.print(gz);

  Serial.print("\t\tmX : "); Serial.print(mx); Serial.print(' ');
  Serial.print("\tmY : "); Serial.print(my); Serial.print(' ');
  Serial.print("\tmZ : "); Serial.println(mz);

}
void display_values_serial()
{
  Serial.print(ax); Serial.print(',');
  Serial.print(ay); Serial.print(',');
  Serial.print(az); Serial.print(',');
  Serial.print(gx); Serial.print(',');
  Serial.print(gy); Serial.print(',');
  Serial.print(gz); Serial.print(',');
  Serial.print(mx); Serial.print(',');
  Serial.print(my); Serial.print(',');
  Serial.println(mz);

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
  delay(1200);
  Serial.println("Orientate IMU to desiered INITAL position to calibrate all SIX axes\n");
  delay(1200);
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
        Serial.println("\nCALIBRATION STARTING IN...");
        break;
      }
    }
    Serial.print(".");
    delay(1000);
  }
  delay(1200);

  // This is where the actual calibration takes place. Orientate the IMU sensor in SIX different directions AND MAKE SURE IT'S STABLE
  for (int n = 1; n <= 6; n++)
  {
    Serial.print("\nCurrent calibration iteration(s) : ");
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

        Serial.println("\nCALIBRATION STARTING IN...\n");
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



  //---------------------------------------------------- [ CALIBRATE MAGNETOMETER ] ----------------------------------------------------//
  Serial.println("\nPress [ ENTER ] to calibrate megnetometer...");

  // Calibration won't start until user inputs 'ENTER' in the serial monitor
  while (true)
  {
    // Checks serial monitor for incoming data
    if (Serial.available() > 0)
    {
      // If user presses 'ENTER' break out of while loop
      if (Serial.read() == '\n')
      {
        Serial.println("\n-------------[ CALIBRATING MAGNETOMETER WILL TAKE ~60-80 SECONDS TO COMPLETE...]-------------");
        delay(1500);
        Serial.println("\n-------------[ SLOWLY AND CONTINUOUSLY MAKE A FIGURE 8 MOTION DURING CALIBRATION ]-------------");
        delay(2000);

        Serial.println("\nCALIBRATION STARTING IN...\n");
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

  if (IMU.calibrateMag() == true)
  {
    // Gather magnetometer bias/scale factors
    hxb = IMU.getMagBiasX_uT();
    hyb = IMU.getMagBiasY_uT();
    hzb = IMU.getMagBiasZ_uT();
    hxs = IMU.getMagScaleFactorX();
    hys = IMU.getMagScaleFactorY();
    hzs = IMU.getMagScaleFactorZ();

    // Display bias/scale factors
    Serial.println("\nCALIBRATION COMPLETE\n");
    Serial.println("---- MAG BIAS VALUES ----\n");
    Serial.print("X_Bias : "); Serial.print(hxb); Serial.print(',');
    Serial.print(" Y_Bias : "); Serial.print(hyb); Serial.print(',');
    Serial.print(" Z_Bias : "); Serial.println(hzb);

    Serial.println("\n---- MAG SCALE VALUES ----\n");
    Serial.print("X_Scale : "); Serial.print(hxs); Serial.print(',');
    Serial.print("Y_Scale : "); Serial.print(hys); Serial.print(',');
    Serial.print("Z_Scale : "); Serial.println(hzs);

    // Set biased/scale factors
    IMU.setMagCalX(hxb, hxs);
    IMU.setMagCalY(hyb, hys);
    IMU.setMagCalZ(hzb, hzs);
    Serial.println("\n---- MAGNOMETER BIAS/SCALE FACTOR SET TO ----\n");

    Serial.print("Mag X Bias : "); Serial.print(hxb); Serial.print("\tMag X Scale Factor : "); Serial.println(hxs);
    Serial.print("Mag Y Bias : "); Serial.print(hyb); Serial.print("\tMag Y Scale Factor : "); Serial.println(hys);
    Serial.print("Mag Z Bias : "); Serial.print(hzb); Serial.print("\tMag Z Scale Factor : "); Serial.println(hzs);

  }

  //---------------------------------------------------- [ CALIBRATION STUFF DONE ] ----------------------------------------------------//
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
  //Serial.println("ax,ay,az,gx,gy,gz,mx,my,mz");

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
  mx = IMU.getMagX_uT();
  my = IMU.getMagY_uT();
  mz = IMU.getMagZ_uT();

  // Display data
  //display_values_table();
  display_values_serial();
  delay(100);
}
