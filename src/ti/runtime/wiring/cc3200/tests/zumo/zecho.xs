/*
 *  ======== zecho.xs ========
 *  Open a socket to the Zumo, drive it, and display/log IMU telemetry data
 *
 *  usage: xs -f zecho.xs [log_file]
 *
 *  If log_file is specified, IMU data is appended to the specified file;
 *  otherwise, this data is printed to the console.
 */
 
/* driving direction options */
var FORWARD  = 'w';
var RIGHT    = 'd';
var LEFT     = 'a';
var BACKWARD = 's';
var STOP     = ' ';

/* data arrays returned from Zumo */
var word = java.lang.reflect.Array.newInstance(java.lang.Byte.TYPE, 4);
var line = java.lang.reflect.Array.newInstance(java.lang.Byte.TYPE, 72);

/* command and reply streams to/from Zumo */
var cmd;
var reply;

/* optional IMU data log */
var log  = null;

/*
 *  ======== main ========
 */
function main(args)
{
    var host = "192.168.1.1";
    var port = 8080;

    /* open a socket to the zumo */
    var socket = java.net.Socket(host, port);

    /* create command stream to the Zumo and data stream from the Zumo */
    cmd = java.io.PrintWriter(socket.getOutputStream(), true);
    reply = java.io.DataInputStream(socket.getInputStream());

    /* open an IMU data log (if specified on the command line) */
    if (args.length > 0) {
        log = new java.io.FileWriter(args[0], true);
    }

    /* run a test */
    testDrive();

    /* close the socket and flush IMU data to the log */
    socket.close();

    if (log) {
        logLine("-------------- done ---------------");
        log.flush();
        log.close();
    }
}

/*
 *  ======== drive ========
 */
function drive(direction, duration, msg)
{
    if (msg) logLine(msg);
    
    for (var i = 0; i < duration; i++) {
	getIMU(direction);
        java.lang.Thread.sleep(5);
    }
}

/*
 *  ======== getIMU ========
 */
function getIMU(wasd)
{
    /* send command to zumo */
    cmd.println(wasd);

    /* read results */
    reply.readFully(line);

    /* print/log results */
    logLine(java.lang.String(line));
}

/*
 *  ======== logLine ========
 */
function logLine(line)
{
    if (log) {
        log.write(line + "\n");
    }
    else {
        print(line);
    }
}

/*
 *  ======== testDrive ========
 */
function testDrive()
{
    do {
	drive(STOP,     15, "------------- calibrating ------------");
	drive(FORWARD,  15, "------------- driving forward ------------");
	drive(RIGHT,    55, "------------- turning right ------------");
	drive(BACKWARD, 15, "------------- driving backward ------------");
	drive(LEFT,     55, "------------- turning left ------------");
	drive(STOP,     15, "------------- stopping ------------");
    } while (java.lang.System.in.available() == 0);

    while (java.lang.System.in.available() > 0) {
	java.lang.System.in.read();
    }
}

/*
 *  ======== testROV ========
 */
function testROV()
{
    for (var i = 10; i < 5; i++) {
        print("reading word " + i + " ...");
	cmd.println("20004000,4");
	reply.readFully(word);
    }
}

/*
 *  ======== testWASD ========
 */
function testWASD()
{
    var wasd = " ";
    var reader = java.io.BufferedReader(
        java.io.InputStreamReader(java.lang.System.in));

    for (;;) {
        /* check for console command input */
        while (reader.ready()) {
            /* get command line input */
            var tmp = reader.readLine();
            if (tmp != null) {
                wasd = String(tmp)[0];
                if (wasd != "w" && wasd != "a" && wasd != "s" && wasd != "d") {
                    wasd = " "; /* ignore non-WASD commands */
                }
            }
        }

        /* repeat latest command, get and display IMU data */
        getIMU(wasd);

        /* pause a bit */
        java.lang.Thread.sleep(1);
    }
}
