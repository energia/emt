/**
 * RedBear Zumo Bot interactive control with real-time acceleration display
 * 
 * Before running this program, start the zumo bot and connect to its
 * WiFi network.
 */

import processing.net.Client;

Client client;

char[] charBuffer = new char[72];

int    NUM_XVALS = 80;
String LOG_NAME  = "acc.txt";

Graph graph1 = new Graph(150, 80, 400, 200, color (200, 20, 20));

float[] time = { 0 };       
float[] ax = { 0 }; 
float[] ay = { 0 };
float[] az = { 0 };

char curGraph = 'A';
int curGraphOffset = 1;
float curTime = 0;

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
    graph1.Title = " Acceleration: (x, y, z) vs. t ";  
    graph1.yMin = 0;

    /* slow the draw() rate down to 10 frames/sec */
    frameRate(40);
        
    /* Connect to zumo's command server IP address and port */
    client = new Client(this, "192.168.1.1", 8080);
}

/*
 *  ======== draw ========
 */
boolean cont = false;  /* continuously send previous command to get IMU data */
String  pcmd = " \n";  /* previous command */
int pending = 0;
int MAX_PENDING = 6;

void draw() 
{
    /* send server commands based on keyboard input */
    if (keyPressed || cont) {
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
                graph1.yLabel = "Rotational Speed (r/s)";
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

        if (pending < MAX_PENDING) {
            client.write(cmd);
            pcmd = cmd;
            pending++;
        }
    }

    /* Read IMU data from server and display it */
    if (client.available() >= charBuffer.length) {
        /* read IMU data */
        for (int i = 0; i < charBuffer.length; i++) {
            charBuffer[i] = client.readChar();
        }
        if (--pending < 0) {
            println("Yikes! pending = " + pending);
            pending = 0;
        }
       
        /* parse IMU data and display it */
        String input = new String(charBuffer);
        String [] tokens = splitTokens(input, " ");
        if (tokens.length > (2 + curGraphOffset)) {
            newPoint(tokens[curGraphOffset], tokens[1 + curGraphOffset], tokens[2 + curGraphOffset]);
        }
    }
}

void newPoint(String x, String y, String z)
{
    /* get next values */
    curTime += 1;
    if (log != null) {
        log.println("A: " + x + " " + y + " " + z);
    }
    
    /* update all data arrays */
    time = pushv(time, curTime);
    ax = pushv(ax, float(x));
    ay = pushv(ay, float(y));
    az = pushv(az, float(z));

    /* update display */
    updatePlots();
}

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
 *  ======== pushv ========
 */
float[] pushv(float [] arr, float val)
{
    float [] tmp = arr;
    
    if (arr.length < NUM_XVALS) {
        tmp = append(arr, val);
    }
    else {
        /* shift data within arr to make room for new value */
        int i;
        for (i = 1; i < NUM_XVALS; i++) {
            arr[i - 1] = arr[i];
        }
        arr[NUM_XVALS - 1] = val;
    }

    return (tmp);
}
