/*
 *  ======== imuLoop ========
 *  This sketch simply reads the Zumo IMU sensors at 20Hz and prints
 *  the current value once per second to the UART.
 */

#include <Wire.h>

#include "L3G.h"
#include "LSM303.h"

#include "app.h"

/* Pololu IMU data instance objects */
LSM303 imuCompass; /* acceleration and magnetometer */
L3G    imuGyro;    /* gyro data */

/*
 *  ======== imuSetup ========
 */
__extern void imuSetup()
{
    Serial.begin(9600);
    Serial.println("imuSetup ...");
    Wire.begin();

    /* initialize Zumo accelerometer and magnetometer */
    imuCompass.init();
    imuCompass.enableDefault();

    /* initialize Zumo gyro */
    if (!imuGyro.init()) {
        Serial.print("Failed to autodetect gyro type!");
        delay(1000);
    }
    imuGyro.enableDefault();
    Serial.println("imuSetup done.");
}

/*
 *  ======== imuLoop ========
 */
__extern void imuLoop()
{
    /* update IMU data every 50 ms (200 Hz) */
    imuGyro.read();
    imuCompass.read();
    delay(50);
}
