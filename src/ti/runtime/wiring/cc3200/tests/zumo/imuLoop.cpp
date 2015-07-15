/*
 *  ======== imuLoop ========
 *  This sketch simply reads the Zumo IMU sensors at 20Hz and prints
 *  the current value once per second to the UART.
 */

#include <Wire.h>
#include <L3G.h>
#include <LSM303.h>

#include <xdc/runtime/System.h>

#include "app.h"

/* Pololu IMU data instance objects */
LSM303 imuCompass; /* acceleration and magnetometer */
L3G    imuGyro;    /* gyro data */

/*
 *  ======== imuSetup ========
 */
__extern void imuSetup()
{
    System_printf("  imuSetup\r\n");

    Serial.begin(9600);

    Wire.begin();

    /* initialize Zumo accelerometer and magnetometer */
    imuCompass.init();
    imuCompass.enableDefault();
    System_printf("  imuSetup: compass done\r\n");

    /* initialize Zumo gyro */
    if (!imuGyro.init()) {
        System_printf("Failed to autodetect gyro type!");
        delay(1000);
    }

    imuGyro.enableDefault();

    System_printf("  imuSetup.\r\n");
}

/*
 *  ======== imuLoop ========
 */
__extern void imuLoop()
{
    static int count = 0;
    static char report[120];

    imuGyro.read();
    imuCompass.read();

    if (++count >= 40) {
        count = 0;
        System_snprintf(report, sizeof(report),
                        "A: %6d %6d %6d   G: %6d %6d %6d   M: %6d %6d %6d",
                        imuCompass.a.x, imuCompass.a.y, imuCompass.a.z,
                        imuGyro.g.x,    imuGyro.g.y,    imuGyro.g.z, 
                        imuCompass.m.x, imuCompass.m.y, imuCompass.m.z);

        Serial.println(report);
    }

    /* update IMU data every 50 ms (200 Hz) */
    delay(50);
}
