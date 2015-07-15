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

int num_clients = 0;
static int num_sockets = 0;
  
static void doCommand(char *buffer, int len, WiFiClient client);
static void doWASD(char wasd, WiFiClient client);
static void printWifiData();

/*
 *  ======== apSetup ========
 */
__extern void apSetup()
{
    Serial.begin(9600);

    /* set priority of this task to be lower than other tasks */
    Task_setPri(Task_self(), 1);

    /* toggle LED when clients connect/disconnect */
    pinMode(MAIN_LED_PIN, OUTPUT);  
    digitalWrite(MAIN_LED_PIN, LOW);
  
    System_printf("  apSetup.\r\n");

    Serial.print("Setting up Access Point named: ");
    Serial.print(ssid);
    Serial.print(" with password: ");
    Serial.println(wifipw);

    /* startup a new network and get the first IP address: 192.168.1.1 */
    WiFi.beginNetwork((char *)ssid, (char *)wifipw);
    while (WiFi.localIP() == INADDR_NONE) {
        /* print dots while we wait for the AP config to complete */
        Serial.print('.');
        delay(300);
    }
    Serial.println('.');
    printWifiData();
  
    /*
     * Expected device firmware versions from servicepack_1.0.0.10.0.bin:
     *   NWP version: 2.4.0.2
     *   MAC version: 1.3.0.1
     *   PHY version: 1.0.3.34
     *
     *   RedBear output was: 2.2.0.1.31.1.2.0.2.1.0.3.23
     *           and now is: 2.4.0.2.31.1.3.0.1.1.0.3.34
     */
    Serial.print(" firmware version: ");
    Serial.println(WiFi.firmwareVersion());
    Serial.println("AP active.");

    /* startup the command server on port PORTNUM */
    Serial.print("Starting dataserver ... "); 
    server.begin();
    Serial.print("dataserver started on port "); Serial.println(PORTNUM);
}

/*
 *  ======== apLoop ========
 */
__extern void apLoop()
{
    /* Did a client connect/disconnect since the last time we checked? */
    int a = WiFi.getTotalDevices();
    if (a != num_clients) {
        if (a > num_clients) {  // new Client connected
            int i;
            digitalWrite(MAIN_LED_PIN, !digitalRead(MAIN_LED_PIN));
            /* display all clients on the network */
            Serial.println("New client connected. All clients:");
            for (i = 0; i < a; i++) {
                Serial.print("  Client #");
                Serial.print(i);
                Serial.print(" at IP address = ");
                Serial.print(WiFi.deviceIpAddress(i));
                Serial.print(", MAC = ");
                Serial.println(WiFi.deviceMacAddress(i));
            }
        } 
        else {                  // a Client disconnected
            digitalWrite(MAIN_LED_PIN, !digitalRead(MAIN_LED_PIN));
            Serial.print("Client disconnected, ");
            Serial.print(a);
            Serial.println(" clients remaining.");
        }    
        num_clients = a;
    }

    if (num_clients > 0) {
        /* listen for incoming clients */
        WiFiClient client = server.available();

        if (client) {
            /* if there's a client, read and process commands */
            Serial.print("new client socket: ");
            Serial.println(++num_sockets);

            static char buffer[64] = {0};
            int bufLen = 0;

            /* while connected to the client, read commands and send results */
            unsigned int lapse = 0;
            unsigned int start = millis();
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
                    //Serial.print(num_sockets); Serial.print(": ");
                    //Serial.println((int)c);

                    /* if there's a new line, we have a complete command */
                    if (c == '\n') {
                        doCommand(buffer, bufLen, client);
                        /* reset command buffer index to get next command */
                        bufLen = 0;
                    }

                    lapse = 0;
                    start = millis();
                }
                else {
                    lapse = millis() - start;
                }
            }

            Serial.print("client socket disconnected: lapse = ");
            Serial.println(lapse);

            /* client disconnected or timed out, close the connection */
            client.stop();

            /* disconnect => implicitly stop the motor */
            motorWASD = ' ';
        }
    }

    /* check for new connections 2 times per second */
    delay(500);
}

/*
 *  ======== doCommand ========
 *  execute the command in buffer and send results to client
 */
static void doCommand(char *buffer, int len, WiFiClient client)
{
    if (buffer[1] == '\n' || buffer[1] == '\r') {
        doWASD(buffer[0], client);
    }
    else {
#if 0
        long int addr = 0;
        int cnt = 0;
        char *ptr;

        /* get the address and count from the command */
        getAddrCnt(buffer, len, &addr, &cnt);

        /* send client cnt bytes starting from addr */
        ptr = (char *)addr;
        while (cnt-- > 0) {
            int status;
            if ((status = client.write(*ptr)) != 1) {
                return;
            }
            ptr++;
        }
#else
        doWASD(' ', client);
#endif
    }
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

#if 0
/*
 *  ======== getAddrCnt ========
 */
static void getAddrCnt(char *buf, int bcnt, long *addr, int *cnt)
{
    static const char hex[] = "0123456789abcdef";
    int i = 0;

    *addr = 0;
    *cnt = 0;
    
    for (i = 0; i < bcnt;  i++) {
        unsigned char c = buf[i];
        char *cp = strchr(hex, c);
        if (cp == NULL) {
            break;
        }
        *addr = (*addr * 16) + (cp - hex);
    }
    
    i++;
    for (; i < bcnt;  i++) {
        if (buf[i] == '\n' || buf[i] == '\r') {
            break;
        }
        *cnt = (*cnt * 10) + (buf[i] - '0');
    }
}
#endif

/*
 *  ======== printWifiData ========
 */
static void printWifiData() 
{
    int i;
  
    /* print our WiFi IP address: */
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    /* print our MAC address: */
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC address: ");
    for (i = 5; i >= 1; i--) {
        Serial.print(mac[i], HEX);
        Serial.print(":");
    }
    Serial.println(mac[0], HEX);
}
