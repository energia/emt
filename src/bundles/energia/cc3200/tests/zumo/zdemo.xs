/*
 * ======== zdemo.xs ========
 *  Open a socket to the Zumo, drive it, and display/log IMU telemetry data
 *
 *  usage: xs -c zdemo.xs [log_file]
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

    /* open a IMU data log (if one's specified on the command line) */
    if (args.length > 0) {
        log = new java.io.FileWriter(args[0], true);
    }

    /* run a test */
    drive(STOP,     15, "------------- calibrating ------------");
    drive(FORWARD,  30, "------------- driving forward ------------");
    drive(RIGHT,    51, "------------- turning right ------------");
    drive(BACKWARD, 30, "------------- driving backward ------------");
    drive(LEFT,     51, "------------- turning left ------------");
    drive(STOP,     15, "------------- stopping ------------");

    /* close the socket and flush IMU data to the log */
    socket.close();

    if (log) {
        printLine("-------------- done ---------------");
        log.flush();
        log.close();
    }
}

/*
 *  ======== drive ======== 
 */
function drive(direction, duration, msg)
{
    if (msg) printLine(msg);
    
    for (var i = 0; i < duration; i++) {
	cmd.println(direction);            /* send command to zumo */
	reply.readFully(line);             /* read results */
	printLine(java.lang.String(line)); /* print/log results */
        //java.lang.Thread.sleep(5);
    }
}

/*
 *  ======== printLine ========
 */
function printLine(line)
{
    if (log) {
        log.write(line + "\n");
    }
    else {
        print(line);
    }
}
