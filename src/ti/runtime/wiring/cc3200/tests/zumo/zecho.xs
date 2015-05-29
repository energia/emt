/*
 *  ======== zecho.xs ========
 *  Open a socket to the Zumo, drive it, and display/log IMU telemetry data
 *
 *  usage: xs -f zecho.xs [log_file]
 *
 *  If log_file is specified, IMU data is appended to the specified file;
 *  otherwise, this data is printed to the console.
 */

var word = java.lang.reflect.Array.newInstance(java.lang.Byte.TYPE, 4);
var line = java.lang.reflect.Array.newInstance(java.lang.Byte.TYPE, 75);
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
    var out = java.io.PrintWriter(socket.getOutputStream(), true);
    var ins = java.io.DataInputStream(socket.getInputStream());

    /* open an IMU data log (if specified on the command line) */
    if (args.length > 0) {
        log = new java.io.FileWriter(args[0], true);
    }

    /* run a test */
    //    testWASD(ins, out);
    testDrive(ins, out);

    /* close the socket and flush IMU data to the log */
    socket.close();
    if (log) {
        logLine("-------------- done ---------------");
        log.flush();
        log.close();
    }
}

main(arguments);

/*
 *  ======== getIMU ========
 */
function getIMU(wasd, ins, out)
{
    /* send command to zumo */
    out.println(wasd);

    /* read results */
    ins.readFully(line);

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
function testDrive(ins, out)
{
    logLine("------------- driving forward ------------");
    for (var i = 0; i < 35; i++) {
	getIMU("w", ins, out);
        java.lang.Thread.sleep(5);
    }

    logLine("------------- turning right ------------");
    for (var i = 0; i < 55; i++) {
	getIMU("d", ins, out);
        java.lang.Thread.sleep(5);
    }

    logLine("------------- driving backward ------------");
    for (var i = 0; i < 35; i++) {
	getIMU("s", ins, out);
        java.lang.Thread.sleep(5);
    }

    logLine("------------- turning left ------------");
    for (var i = 0; i < 55; i++) {
	getIMU("a", ins, out);
        java.lang.Thread.sleep(5);
    }

    logLine("------------- stopping ------------");
    getIMU(" ", ins, out);
}

/*
 *  ======== testROV ========
 */
function testROV(ins, out)
{
    for (var i = 10; i < 5; i++) {
        print("reading word " + i + " ...");
	out.println("20004000,4");
	ins.readFully(word);
    }
}

/*
 *  ======== testWASD ========
 */
function testWASD(ins, out)
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
        getIMU(wasd, ins, out);

        /* pause a bit */
        java.lang.Thread.sleep(1);
    }
}
