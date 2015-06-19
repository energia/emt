/**
 * Simple RedBear Zumo Bot interactive control
 * 
 * Before running this program, start the zumo bot and connect to its
 * WiFi network.
 */

import processing.net.Client;

Client client;

char[] charBuffer = new char[75];

/*
 *  ======== setup ========
 */
void setup() 
{
    /* create minimal text output window for acc data */
    size(280, 120);
    background(204);
    stroke(0);
    textSize(20);
    fill(50);

    /* slow down the draw() rate to N frames/sec */
    frameRate(30);

    /* Connect to zumo's command server IP address and port */
    client = new Client(this, "192.168.1.1", 8080);
}

/*
 *  ======== draw ========
 */
int MAX_PENDING = 1;
int pending = 0;
boolean cont = false;
void draw() 
{
    /* send server commands based on keyboard input */
    if (keyPressed || cont) {
        String cmd = " \n";
        if  (key == 'w' || key == 'a' || key == 's' || key == 'd') {
            cmd = key + "\n";
        }
        else if (key == 'c') cont = false;
        else if (key == 'x') cont = true;

        if (pending < MAX_PENDING) {
            client.write(cmd);
            pending++;
        }
    }

    /* Read IMU data from server and display it */
    if (client.available() >= charBuffer.length) {

        /* read IMU data and convert it to a string */
        for (int i = 0; i < charBuffer.length; i++) {
            charBuffer[i] = client.readChar();
        }
        String input = new String(charBuffer);
        pending--;

        /* parse IMU data and display it */
        String [] tokens = splitTokens(input, " ");
        if (tokens.length > 3) {
            background(204);
            text(tokens[1] + ", " + tokens[2] + ", " + tokens[3], 10, 60);
        }
    }
}
