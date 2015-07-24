/**
 * RedBear Zumo Bot interactive control with real-time acceleration display
 * 
 * Before running this program, start the zumo bot and connect to its
 * WiFi network.
 */

import processing.net.Client;

Client client;           /* data socket connected to zumo IMU data server */

/* IMU data from zumo */
char[] imuBuffer = new char[72];

int    MAX_FPS = 30;     /* max frames/sec, i.e., rate of calls to draw() */
int    MAX_XVALS = 256;  /* max # of samples in the x-axis */
int    MAX_PENDING = 6;  /* max # of outstanding commands to send */

String LOG_NAME  = "zumo_log.txt";

/* IMU data graph object */
Graph graph1 = new Graph(150, 80, 400, 200, color (200, 20, 20));

/* IMU data values to graph */
float[] time = { 0 };       
float[] ax   = { 0 }; 
float[] ay   = { 0 };
float[] az   = { 0 };

char curGraph = 'A';
int curGraphOffset = 1;
long startMillis = 0;
float curTime = 0;
boolean scaleData = true;

PrintWriter log = null;  /* optional data log */

/*
 *  ======== setup ========
 */
void setup() 
{
    /* create minimal text output window for acc data */
    size(650, 350); 
   
    graph1.xLabel = " Time (s)";
    graph1.yLabel = "Acceleration (m/s^2)";
    graph1.Title  = " Acceleration: (x, y, z) vs. t ";  
    graph1.yMin   = 0;
    graph1.yMax   = 0;

    /* slow the draw() rate down to MAX_FPS frames/sec */
    frameRate(MAX_FPS);
        
    /* Connect to zumo's command server IP address and port */
    client = new Client(this, "192.168.1.1", 8080);
}

/*
 *  ======== draw ========
 */
boolean cont = false;  /* continuously send previous command to get IMU data */
String  pcmd = " \n";  /* previous command */
int     pending = 0;   /* number of outstanding commands sent to zumo bot */

void draw() 
{
    /* send server commands based on keyboard input */
    if (keyPressed || cont) {
        if (startMillis == 0) {
            /* capture initial start time */
            startMillis = java.lang.System.currentTimeMillis();
        }
        String cmd = pcmd;
        if  (key == ' ' || key == 'w' || key == 'a' || key == 's' || key == 'd') {
            cmd = key + "\n";
        }
        else if (key == 'x') cont = false;
        else if (key == 'c') cont = true;
        else if (key == 'L') {
            if (log == null) {
                log = createWriter(LOG_NAME);
                println("logging started ...");
            }
        }
        else if (key == 'l') {
            if (log != null) {
                log.flush();
                log.close();
                log = null;
                println("logging stopped.");
            }
        }
        else if (key == 'G') {
            if (curGraph != 'G') {
                curGraph = 'G';
                curGraphOffset = 5;
                graph1.yLabel = "Rotational Speed (deg/s)";
                graph1.Title = " Gyro: (x, y, z) vs. t ";  
                graph1.yMin = 0;
                graph1.yMax = 0;
                ax = new float [] { 0 }; 
                ay = new float [] { 0 };
                az = new float [] { 0 };
                time = new float [] { curTime }; 
            }
        }
        else if (key == 'A') {
            if (curGraph != 'A') {
                curGraph = 'A';
                curGraphOffset = 1;
                graph1.yLabel = "Acceleration (m/s^2)";
                graph1.Title = " Acceleration: (x, y, z) vs. t ";  
                graph1.yMin = 0;
                graph1.yMax = 0;
                ax = new float [] { 0 }; 
                ay = new float [] { 0 };
                az = new float [] { 0 };
                time = new float [] { curTime }; 
            }
        }

        /* stop sending commands if too many commands are outstanding */
        if (pending < MAX_PENDING) {
            client.write(cmd);
            pcmd = cmd;
            pending++;
        }
    }

    /* Read IMU data from server and display it */
    if (client.available() >= imuBuffer.length) {
        /* read IMU data */
        for (int i = 0; i < imuBuffer.length; i++) {
            imuBuffer[i] = client.readChar();
        }

        /* decrement pending count so we can send more commands */
        if (--pending < 0) {
            println("Yikes! pending = " + pending);
            pending = 0;
        }
       
        /* parse IMU data and display it */
        String input = new String(imuBuffer);
        String [] tokens = splitTokens(input, " ");
        if (tokens.length > (2 + curGraphOffset)) {
            newPoint(tokens[0 + curGraphOffset],  /* x value */
                     tokens[1 + curGraphOffset],  /* y value */
                     tokens[2 + curGraphOffset]); /* z value */
        }
    }
}

/*
 *  ======== newPoint ========
 *  add new point to data arrays and update graph
 */
void newPoint(String x, String y, String z)
{
    /* get next values */
    curTime = (java.lang.System.currentTimeMillis() - startMillis) / 1000.0;
    if (log != null) {
        log.println("A: " + x + " " + y + " " + z);
    }
    
    /* update all data arrays */
    time = pushv(time, curTime);
    ax = pushv(ax, scale(curGraph, x));
    ay = pushv(ay, scale(curGraph, y));
    az = pushv(az, scale(curGraph, z));

    /* update display */
    updatePlots();
}

/*
 *  ======== pushv ========
 *  append new data sample to arr[] and return new array
 */
float[] pushv(float [] arr, float val)
{
    float [] tmp = arr;
    
    if (arr.length < MAX_XVALS) {
        tmp = append(arr, val);
    }
    else {
        /* shift data within arr to make room for new value */
        int i;
        for (i = 1; i < MAX_XVALS; i++) {
            arr[i - 1] = arr[i];
        }
        arr[MAX_XVALS - 1] = val;
    }

    return (tmp);
}

/*
 *  ======== updatePlots ========
 *  redraw IMU graph
 */
void updatePlots()
{
    background(255);

    graph1.xMax = max(time);
    graph1.xMin = min(time);
    graph1.yMax = max(max(max(az), max(ax), max(ay)), graph1.yMax);
    graph1.yMin = min(min(min(az), min(ax), min(ay)), graph1.yMin);

    graph1.DrawAxis();
      
    graph1.GraphColor = color(200, 40, 40);  
    graph1.LineGraph(time, ax);      
      
    graph1.GraphColor = color(40, 200, 40);   
    graph1.LineGraph(time, ay);

    graph1.GraphColor = color(40, 40, 200);   
    graph1.LineGraph(time, az);
}

/*
 *  ======== scale =========
 */
float scale(char type, String value)
{
    if (scaleData) {
        switch (type) {
            case 'A':
                return (float(value) * (2 * 9.80665) / float(32768)); /* acc data is +-2G */

            case 'G':
                return (float(value) * 250 / float(32768));           /* gyro data is +-250 deg/sec */
        }
    }
    return (float(value));
}
