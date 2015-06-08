/*
 *  ======== readmem.xs ========
 *  Open a socket to the CC3200 read it memory
 *
 *  usage: xs -f readmem.xs [log_file]
 *
 *  If log_file is specified, IMU data is appended to the specified file;
 *  otherwise, this data is printed to the console.
 */

var word = java.lang.reflect.Array.newInstance(java.lang.Byte.TYPE, 128);
var line = java.lang.reflect.Array.newInstance(java.lang.Byte.TYPE, 75);
var log  = null;

/*
 *  ======== main ========
 */
function main(args)
{
    var host = "192.168.1.1";
    var port = 8080;

    for (;;) {
        if (java.lang.System.in.available() > 1) return;

        /* open a socket to the zumo */
        var socket = java.net.Socket(host, port);
        var out = java.io.PrintWriter(socket.getOutputStream(), true);
        var ins = java.io.DataInputStream(socket.getInputStream());

        /* open an IMU data log (if specified on the command line) */
        if (args.length > 0) {
            log = new java.io.FileWriter(args[0], true);
        }

        /* run a test */
        testROV(ins, out, 0x20004000, 30);

        /* close the socket and flush IMU data to the log */
        socket.close();
    }
    if (log) {
        logLine("-------------- done ---------------");
        log.flush();
        log.close();
    }
}

main(arguments);

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
 *  ======== readmem ========
 */
function readmem(ins, out, start, data)
{
    cmd = start.toString(16) + "," + data.length;
    out.println(cmd);
    ins.readFully(data);
    var result = "";
    for (j = 0; j < data.length; j++) {
        var b = (0xff & data[j]).toString(16);
	result += b + " ";
    }
    print(start.toString(16) + ": " + result);
}

/*
 *  ======== testROV ========
 */
function testROV (ins, out, start, count)
{
    var addr = start;
    for (var i = 0; i < count; i++) {
        readmem(ins, out, addr, word);
        addr += word.length;
    }
}
