/*
  WiFi Web Server
 
 A simple web server to control the outputs of several
 CC3200 development boards. 
 
 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 created 19 August 2015 
 */

#include <cc3200/libraries/WiFi/WiFi.h>
#include <cc3200/libraries/WiFi/WiFiClient.h>
#include <cc3200/libraries/WiFi/WiFiServer.h>
#include <ti/sysbios/family/arm/m3/TimestampProvider.h>

/* 
 * The following BOARD_xxx defines should be auto-provided as -D's in the compile line
 */

//#define BOARD_RedBearLab_WiFi_Mini
//#define BOARD_RedBearLab_CC3200
//#define BOARD_CC3200_LAUNCHXL
//#define BOARD_CC3200STK_WIFI

//#define ACCESS_POINT_MODE

#ifdef ACCESS_POINT_MODE
// Access Point network name also called SSID
char ssid[] = "Energia_CC3200_AP";
// your network password
char password[] = "password";
#else
// your network name also called SSID
char ssid[] = "TINK-NET";
// your network password
char password[] = "";
#endif

WiFiServer server(80);

String HTTP_req;          // stores the HTTP request
boolean Red_LED_status = 0;   // state of LED, off by default
boolean Yellow_LED_status = 0;   // state of LED, off by default
boolean Green_LED_status = 0;   // state of LED, off by default

void ProcessCheckboxes(WiFiClient cl);
void printWifiStatus();
void listNetworks();
void printEncryptionType(int thisType);

void setup2() {
//    Serial.begin(115200);      // initialize serial communication
    pinMode(RED_LED, OUTPUT);      // set the LED pin mode

//    WiFi.init();

//    listNetworks();

#ifdef ACCESS_POINT_MODE
//    Serial.print("Setting up Access Point named: ");
//    Serial.println(ssid);
//    Serial.print("AP uses WPA and password is: ");
//    Serial.println(password);

    // Create a WPA/WPA2 network. Change this line if using open or WEP network:
    WiFi.beginNetwork((char *)ssid, (char *)password);
#else
//    Serial.print("Connecting to network: ");
//    Serial.print(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
//    WiFi.begin((char *)ssid, (char *)password);
    WiFi.begin((char *)ssid);
    while ( WiFi.status() != WL_CONNECTED) {
        // print dots while we wait to connect
//        Serial.print(".");
        delay(300);
    }

//    Serial.println("\nConnected!");
#endif

//    Serial.print("Waiting for an ip address");

    while (WiFi.localIP() == INADDR_NONE) {
        // print dots while we wait for an ip addresss
//        Serial.print(".");
        delay(300);
    }

//    Serial.println();

    // you're connected now, so print out the status  
//    printWifiStatus();

//    Serial.println("Starting webserver on port 80");
    server.begin();                           // start the web server on port 80
//    Serial.println("Webserver started!");
}

void loop2() {
    uint32_t startTime;
    // listen for incoming clients
    WiFiClient client = server.available();
    if (client) {
//        Serial.println("new client");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                HTTP_req += c;  // save the HTTP request 1 char at a time
                //        Serial.write(c);
                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // send web page
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<meta name=\"viewport\" content =\"width=device-width,initial-scale=1.1,user-scalable=yes\" />");
                    client.println("<title>Energia CC3200 Control</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>Digital Output Tests</h1>");
                    client.println("<p>Click to turn pins on and off.</p>");
                    client.println("<form method=\"get\">");
//                    startTime = TimestampProvider_get32();            
                    ProcessCheckboxes(client);
//                    Serial.print((float)(TimestampProvider_get32()-startTime) / 80000.0);
//                    Serial.println("ms");
                    client.println("</form>");
                    client.println("</body>");
                    client.println("</html>");
//                    Serial.print(HTTP_req);
                    HTTP_req = "";    // finished with request, empty string
                    break;
                }
                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                }
                else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        // give the web browser time to receive the data
        delay(10);

        // close the connection:
        client.stop();
//        Serial.println("client disonnected");
    }
    else {
      delay(5000);
    }
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("Network Name: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

#ifdef BOARD_RedBearLab_WiFi_Mini
#define NUM_COLUMNS 2
const char *pinNames[] = {
    "GND",   "D7",
    "3v3",   "D8",
    "D6",    "D9",
    "D5",    "D10",
    "D4",    "D11",
    "D3",    "D12",
    "D2",    "D13",
    "D1-Tx", "D14",
    "D0-Rx", "D15",
    "A3",    "D20",
    "A2",    "D21",
    "A1",    "D22",
    "NRST",  "D23",
    "3v3",   "D24",
    "GND",   "3v3",
    "VIN",   "GND"
};

int8_t pinNum[] = {
    -1, 7,
    -1, 8,
    6, 9,
    5, 10,
    4, 11,
    3, 12,
    2, 13,
    -1, 14,
    -1, 15,
    19, 20,
    18, 21,
    17, 22,
    -1, 23,
    -1, 24,
    -1, -1,
    -1, -1
};
#endif

#ifdef BOARD_RedBearLab_CC3200
#define NUM_COLUMNS 2
const char *pinNames[] = {
    "NC",    "D15",
    "NC",    "D14",
    "NC",    "NC",
    "NC",    "GND",
    "NC",    "D13",
    "IOREF", "D12",
    "NRST",  "D11",
    "3v3",   "D10",
    "5v",    "D9",
    "GND",   "D8",
    "GND",   "D7",
    "VIN",   "D6",
    "A0",    "D5",
    "A1",    "D4",
    "A2",    "D3",
    "A3",    "D2",
    "D20",   "D1-Tx",
    "D21",   "D0-Rx"
};

int8_t pinNum[] = {
    -1, 15,
    -1, 14,
    -1, -1,
    -1, -1,
    -1, 13,
    -1, 12,
    -1, 11,
    -1, 10,
    -1, 9,
    -1, 8,
    -1, 7,
    -1, 6,
    16, 5,
    17, 4,
    18, 3,
    19, 2,
    20, -1,
    21, -1
};
#endif

#ifdef BOARD_CC3200_LAUNCHXL
#define NUM_COLUMNS 4
const char *pinNames[] = {
    "3v3",      "5v",       "P40/10",   "GND",
    "P2/25",    "GND",      "P39/9",    "P19/35-J",
    "P3-Rx-S3", "P23-Rx",   "P38/31-J", "P18",
    "P4-Tx",    "P24/33",   "P37/29",   "P17",
    "P5",       "P25/2",    "P36/13",   "RST",
    "P6/26",    "P26/6",    "P35/19-J", "P15",
    "P7",       "P27",      "P34/8",    "P14",
    "P8/34",    "P28",      "P33/24",   "P13/36",
    "P9-LY",    "P29-LR",   "P32-J",    "P12-Tx",
    "P10-LG",   "P30",      "P31/38-J", "P11-S2"
};

int8_t pinNum[] = {
    -1, -1, -1, -1,
     2, -1, -1, -1,
    -1, -1, -1, 18,
    -1, 24, -1, 17,
     5, -1, -1, -1,
     6, -1, -1, 15,
     7, 27, -1, 14,
     8, 28, -1, 13,
     9, 29, -1, -1,
    10, 30, -1, -1
};
#endif

#ifdef BOARD_CC3200STK_WIFI
#define NUM_COLUMNS 3
const char *pinNames[] = {
    "VDD",      "GND",      "P21",
    "P3-SCL",   "P4-SDA",   "P22-BATTMON",
    "P5-DP12",  "P6-DP7",   "P23-BUZZ",
    "P7-DP11",  "P8-DP6",   "P24-REED",
    "VDD",      "P10-Tx",   "P25-BUTT1",
    "P11-MOSI", "P12-Rx",   "P26-TMP",
    "P13-MISO", "P14-DP3",  "P27-LED1",
    "P15-SCLK", "P16-DP2",  "P28-MPUINT",
    "P17-DPID", "P18-BUTT2","P29-MICPWR",
    "P19-PWRG", "P20-LED2", "P30-JTTMS"
};

int8_t pinNum[] = {
    -1, -1, 21,
     3,  4, -1,
     5,  6, 23,
     7,  8, -1,
    -1, -1, -1,
    11, -1, -1,
    13, 14, 27,
    15, 16, -1,
    17, -1, -1,
    -1, 20, -1
};
#endif
extern uint32_t wakeupcalled;
extern void printUtilization();
// switch LEDs and send back HTML for LED checkboxes
void ProcessCheckboxes(WiFiClient cl)
{
    uint8_t i;
    String pinName, pinSetStr, pinCheckBox, checkBoxes;
    uint8_t getIndex = HTTP_req.indexOf("GET");
    uint8_t httpIndex = HTTP_req.indexOf("HTTP/1.1");
    uint8_t pinIndex;
    bool lineEnd;
    checkBoxes = "";

    for (i=0; i < sizeof(pinNum); i++) {
        pinName = "pin";
        pinName += pinNum[i];
        pinSetStr = pinName + "=1";
        pinIndex = HTTP_req.indexOf(pinSetStr);
        lineEnd = false;
        pinCheckBox = "<label style=\"position: absolute; color: #00F; left: ";
#if (NUM_COLUMNS == 2)
        switch (i%2) {
            case 0:
                pinCheckBox += "10";
                break;
            case 1:
                pinCheckBox += "100";
                lineEnd = true;
                break;
        }
#elif (NUM_COLUMNS == 3)
        switch (i%3) {
            case 0:
                pinCheckBox += "10";
                break;
            case 1:
                pinCheckBox += "110";
                break;
            case 2:
                pinCheckBox += "210";
                lineEnd = true;
                break;
        }
#elif (NUM_COLUMNS == 4)
        switch (i%4) {
            case 0:
                pinCheckBox += "10";
                break;
            case 1:
                pinCheckBox += "100";
                break;
            case 2:
                pinCheckBox += "180";
                break;
            case 3:
                pinCheckBox += "260";
                lineEnd = true;
                break;
        }
#endif
        pinCheckBox += "px; \">";
        pinCheckBox += "<input type=\"checkbox\" value=\"1\" name=\"";
        pinCheckBox += pinName;
        pinCheckBox += "\" ";
        if (pinNum[i] == -1) {
            pinCheckBox += "disabled";
        }
        else {
            if (pinIndex > getIndex && pinIndex < httpIndex) {  // see if checkbox was clicked
                pinCheckBox += "checked onclick=\"submit();\"";
                digitalWrite(pinNum[i], 1);
            }
            else {
                pinCheckBox += "onclick=\"submit();\"";
                digitalWrite(pinNum[i], 0);
            }
        }
        pinCheckBox += "/> ";
        pinCheckBox += pinNames[i];
        pinCheckBox += "</label>";

        if (lineEnd == true) {
            pinCheckBox += "<br>";
        }

        pinCheckBox += "\r\n";

        /* add each check box to the html checkbox form */
        checkBoxes += pinCheckBox;
//        Serial.println(pinCheckBox);
    }
    /* print entire check box form at once */
//Serial.println("1");
    cl.print (checkBoxes);
//Serial.println("2");
//Serial.println(wakeupcalled);
//    Serial.println (checkBoxes);
}

void listNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
  case ENC_TYPE_WEP:
    Serial.println("WEP");
    break;
  case ENC_TYPE_TKIP:
    Serial.println("WPA");
    break;
  case ENC_TYPE_CCMP:
    Serial.println("WPA2");
    break;
  case ENC_TYPE_NONE:
    Serial.println("None");
    break;
  case ENC_TYPE_AUTO:
    Serial.println("Auto");
    break;
  }
}








