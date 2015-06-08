/*
 *  ======== apLoop.cpp ========
 *  This sketch starts a network and listens on port PORTNUM for
 *  commands to read memory.
 *
 *  The name and password of the network and the port number of the server
 *  (always at IP address 192.168.1.1) can be changed below.
 */

#include <cc3200/libraries/WiFi/WiFi.h>
#include <string.h>

#include "app.h"

/* name of the network and its password */
static const char ssid[] = "tink3AP";
static const char wifipw[] = "password";

/* the port number of the server listening for command at 192.168.1.1 */
#define PORTNUM 8080

static WiFiServer server(PORTNUM); // create data server on port PORTNUM

static int num_clients = 0;
static int num_sockets = 0;
  
static void doCommand(char *buffer, int len, WiFiClient client);
static void printWifiData();
static void getAddrCnt(char *buf, int bcnt, long *addr, int *cnt);

/*
 *  ======== apSetup ========
 */
__extern void apSetup()
{
    Serial.begin(9600);

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
    Serial.println();
    printWifiData();
    Serial.println("AP active.");
  
    /* start a data server on port number PORTNUM */
    Serial.print("Starting dataserver on port: "); Serial.println(PORTNUM);
    server.begin();
    Serial.println("dataserver started!");
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
            Serial.println("Client connected! All clients:");
            for (i = 0; i < a; i++) {
                Serial.print("Client #");
                Serial.print(i);
                Serial.print(" at IP address = ");
                Serial.print(WiFi.deviceIpAddress(i));
                Serial.print(", MAC = ");
                Serial.println(WiFi.deviceMacAddress(i));
            }
        } 
        else {                  // a Client disconnected
            digitalWrite(MAIN_LED_PIN, !digitalRead(MAIN_LED_PIN));
            Serial.println("Client disconnected.");
        }    
        num_clients = a;
    }

    if (num_clients > 0) {
        /* listen for incoming clients */
        WiFiClient client = server.available();

        if (client) {
            /* if there's a client, read and process commands */
            Serial.print("new client socket: ");
            Serial.println(num_sockets++);

            static char buffer[256] = {0};
            int bufLen = 0;

            /* while connected to the client, read commands and send results */
            while (client.connected()) {

                /* if there's a byte to read from the client .. */
                if (client.available()) {    

                    /* copy it to the command buffer, byte at a time */
                    char c = client.read();

                    /* never overrun the command buffer */
                    if (bufLen >= (int)(sizeof (buffer))) { 
                        bufLen = sizeof (buffer) - 1;
                    }
                    buffer[bufLen++] = c;

                    /* if there's a new line, we have a complete command */
                    if (c == '\n') {
                        doCommand(buffer, bufLen, client);
                        /* reset command buffer index to get next command */
                        bufLen = 0;
                    }
                }
            }

            /* client disconnected, close the connection */
            client.stop();
            Serial.println("client socket disconnected");
        }
    }

    delay(100);
}

/*
 *  ======== doCommand ========
 *  execute the command in buffer and send results to client
 */
static void doCommand(char *buffer, int len, WiFiClient client)
{
    long int addr = 0;
    int cnt = 0;
    char *ptr;
    int status;

    /* get the address and count from the command */
    getAddrCnt(buffer, len, &addr, &cnt);

    /* send client cnt bytes starting from addr */
    ptr = (char *)addr;
    while (cnt-- > 0) {
        status = client.write(*ptr);
        ptr++;
    }
    if (status != 1) {
        Serial.println("doCommand write failure!");
    }
}

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
    Serial.print("read("); Serial.print(*addr);
    Serial.print(", "); Serial.println(*cnt);
}

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
