/*
 *  ======== apLoop ========
 *  This sketch starts a network and listens on port PORTNUM for
 *  command that can control the zumo motors.
 *
 *  The name and password of the network and the port number of the server
 *  (always at IP address 192.168.1.1) can be changed below.
 */

#include <cc3200/libraries/WiFi/WiFi.h>
#include <ti/sysbios/knl/Task.h>

#include <string.h>

#include "app.h"

/* name of the network and its password */
static const char ssid[] = "zumoAP";
static const char wifipw[] = "password";

/* the port number of the server listening for command at 192.168.1.1 */
#define PORTNUM 8080

/* create data server on port PORTNUM */
static WiFiServer server(PORTNUM);

static void doWASD(char wasd, WiFiClient client);

/*
 *  ======== apSetup ========
 */
__extern void apSetup()
{
    Serial.begin(9600);

    /* set priority of this task to be lower than other tasks */
    Task_setPri(Task_self(), 1);

    /* startup a new network and get the first IP address: 192.168.1.1 */
    Serial.print("Starting a new network: "); Serial.println(ssid);
    WiFi.beginNetwork((char *)ssid, (char *)wifipw);
    while (WiFi.localIP() == INADDR_NONE) {
        Serial.print(".");
        delay(300);
    }

    /* startup the command server on port PORTNUM */
    server.begin();
    Serial.print("dataserver started on port "); Serial.println(PORTNUM);
}

/*
 *  ======== apLoop ========
 */
__extern void apLoop()
{
    /* Did a client connect/disconnect since the last time we checked? */
    if (WiFi.getTotalDevices() > 0) {

        /* listen for incoming clients */
        WiFiClient client = server.available();
        if (client) {

            /* if there's a client, read and process commands */
            static char buffer[64] = {0};
            int bufLen = 0;

            /* while connected to the client, read commands and send results */
            while (client.connected()) {
                /* if there's a byte to read from the client .. */
                if (client.available()) {
                    /* copy it to the command buffer, byte at a time */
                    char c = client.read();

                    /* ignore bogus characters */
                    if (c == '\0' || c == '\r') continue;
                    
                    /* never overrun the command buffer */
                    if (bufLen >= (int)(sizeof (buffer))) { 
                        bufLen = sizeof (buffer) - 1;
                    }
                    buffer[bufLen++] = c;

                    /* if there's a new line, we have a complete command */
                    if (c == '\n') {
                        doWASD(buffer[0], client);
                        /* reset command buffer index to get next command */
                        bufLen = 0;
                    }
                }
            }

            /* client disconnected or timed out, close the connection */
            client.flush();
            client.stop();

            /* disconnect => implicitly stop the motor */
            motorWASD = ' ';
        }
    }

    /* check for new connections 2 times per second */
    delay(500);
}

/*
 *  ======== doWASD ========
 */
static void doWASD(char wasd, WiFiClient client)
{
    static char report[80];

    /* set new motor command */
    motorWASD = wasd;

    /* send current IMU data */
    System_snprintf(report, sizeof(report),
        "A: %6d %6d %6d G: %6d %6d %6d M: %6d %6d %6d",
                    imuCompass.a.x, imuCompass.a.y, imuCompass.a.z,
                    imuGyro.g.x,    imuGyro.g.y,    imuGyro.g.z, 
                    imuCompass.m.x, imuCompass.m.y, imuCompass.m.z);
    if (client.write((unsigned char *)report, 72) != 72) {
        Serial.println("Error: reply failed, status != 72");
    }
}
