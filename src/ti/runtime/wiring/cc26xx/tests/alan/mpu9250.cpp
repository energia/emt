/*
 *  ======== mpu9250.cpp ========
 *  This code initially came from http://www.lucidarme.me/?p=5057
 */

#include <Energia.h> // Energia Wiring API
#include <Wire.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>
 
//#include "app.h"

#include "MPU6500.h"

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C
 
#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18
 
#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18
 
#define LED RED_LED
#define BUZZER 23

TwoWire Wire1(1);

/*
 *  ======== I2Cread ========
 *  This function read Nbytes bytes from I2C device at address Address.
 *  Put read bytes starting at register Register in the Data array. 
 */
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
    // Set register address
    Wire1.beginTransmission(Address);
    Wire1.write(Register);
    Wire1.endTransmission();
 
    // Read Nbytes
    Wire1.requestFrom(Address, Nbytes); 
    uint8_t index = 0;
    while (Wire1.available()) {
        Data[index++] = Wire1.read();
    }
}
 
/*
 *  ======== I2CwriteByte ========
 *  Write a byte (Data) in device (Address) at register (Register)
 */
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
    // Set register address
    Wire1.beginTransmission(Address);
    Wire1.write(Register);
    Wire1.write(Data);
    Wire1.endTransmission();
}
 
/*
 *  ======== blinkSetup ========
 */
void blinkSetup()
{
    // Arduino initializations
    Wire1.begin();
    Serial.begin(9600);
    pinMode(LED, OUTPUT);

    for (int i = 0; i < 2; i++) {
        digitalWrite(LED, 1);
        delay(250);
        digitalWrite(LED, 0);
        delay(250);
    }
 
    // reset the device
    I2CwriteByte(MPU9250_ADDRESS, MPU6500_RA_PWR_MGMT_1, 0x80);

    delay(100); // page 42 - delay 100ms

    // ensure all axis of acc and gyro are enabled
    I2CwriteByte(MPU9250_ADDRESS, MPU6500_RA_PWR_MGMT_2, 0x00);

    // reset gyro, accel, temp 
    I2CwriteByte(MPU9250_ADDRESS, MPU6500_RA_SIGNAL_PATH_RESET, 0x07);

    delay(100); // page 42 - delay 100ms

    uint8_t id;
    I2Cread(MPU9250_ADDRESS, MPU6500_RA_WHO_AM_I, 1, &id);
    Serial.print("id should be 0x71: ");
    Serial.println(id, HEX);

    // Set DLPF_CFG to 1: 1kHz Gyro sampling, 184Hz bandwidth
    I2CwriteByte(MPU9250_ADDRESS, MPU6500_RA_CONFIG, 0x01);

    // Default: 1kHz Accel sampling, 480Hz cutoff

    // enable temperature, gyro, and accelerometer output
    I2CwriteByte(MPU9250_ADDRESS, MPU6500_RA_FIFO_EN, 
        MPU6500_FIFO_EN_ACC | MPU6500_FIFO_EN_TEMP | MPU6500_FIFO_EN_GYRO);

    // Configure gyroscope range
    I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_250_DPS);

    // Configure accelerometers range
    I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);

    // Set bypass mode for magnetometer (host can directly address it on I2C)
    I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02);
 
    // Put magnetometer (AK8963) into single measurement mode
    I2CwriteByte(MAG_ADDRESS, 0x0B, 0x01); /* soft reset */
    I2CwriteByte(MAG_ADDRESS, 0x0A, 0x01);
}


/*
 *  ======== blinkLoop ========
 *  Main loop, read and display data
 */
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

/*
 *  ======== blinkLoop ========
 */
void blinkLoop()
{
    static uint32_t newTone, oldTone;

    // ____________________________________
    // :::  accelerometer and gyroscope ::: 
 
    // Read accelerometer and gyroscope
    uint8_t Buf[14];
    I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
 
    // Create 16 bits values from 8 bits data
 
    // Accelerometer
    ax = Buf[0] << 8 | Buf[1];
    ay = Buf[2] << 8 | Buf[3];
    az = Buf[4] << 8 | Buf[5];

    // Gyroscope
    gx = Buf[8] << 8 | Buf[9];
    gy = Buf[10] << 8 | Buf[11];
    gz = Buf[12] << 8 | Buf[13];
 
    // Display values
 
#if 0
    // Accelerometer
    Serial.print("Axyz: ");
    Serial.print(ax, DEC); 
    Serial.print("\t");
    Serial.print(ay, DEC);
    Serial.print("\t");
    Serial.print(az, DEC);  
    Serial.print("\n");
 
    // Gyroscope
    Serial.print("Gxyz: ");
    Serial.print(gx, DEC); 
    Serial.print("\t");
    Serial.print(gy, DEC);
    Serial.print("\t");
    Serial.print(gz, DEC);  
    Serial.print("\n");

    // _____________________
    // :::  Magnetometer ::: 
 
    // Read register Status 1 and wait for the DRDY: Data Ready
    uint8_t ST1;
    do  {
        I2Cread(MAG_ADDRESS, 0x02, 1, &ST1);
    } while (!(ST1 & 0x01));
 
    // Read magnetometer data  
    uint8_t Mag[6];  
    I2Cread(MAG_ADDRESS, 0x03, 6, Mag);
 
    // Request next magnetometer single measurement
    I2CwriteByte(MAG_ADDRESS, 0x0A, 0x01);
 
    // Create 16 bits values from 8 bits data
 
    // Magnetometer
    mx = Mag[1] << 8 | Mag[0];
    my = Mag[3] << 8 | Mag[2];
    mz = Mag[5] << 8 | Mag[4];
 
    // Magnetometer
    Serial.print(mx, DEC); 
    Serial.print("\t");
    Serial.print(my, DEC);
    Serial.print("\t");
    Serial.print(mz, DEC);  
    Serial.print("\t");

    // End of line
    Serial.println("");
#endif

    newTone = ax/2 + 2000;
    if (abs(newTone - oldTone) > 20) {
        tone(BUZZER, newTone, 0);
        oldTone = newTone;
    }
    delay(100);    /* otherwise get data 100 times per sec */
}
