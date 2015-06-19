/*
 *  ======== readmem.xs ========
 *  Open a socket to the CC3200 read it memory
 *
 *  usage: xs -f readmem.xs [[read_len] [log_file]]
 *
 *  If read_len is specified, each read request to the server is of length 
 *  read_len; otherwise, data is read 4-bytes at a time.
 *
 *  If log_file is specified, memory data is appended to the specified file;
 *  otherwise, this data is printed to the console.
 */

var word;
var log  = null;

/*
 *  ======== main ========
 */
function main(args)
{
    var host = "192.168.1.1";
    var port = 8080;
    var readLen = 4;

    if (args.length > 0) {
	readLen = args[0] - 0;
        if (!readLen) {
	    readLen = 4;
	}
    }

    /* create read reply buffer */
    word = java.lang.reflect.Array.newInstance(java.lang.Byte.TYPE, readLen);

    for (;;) {
        if (java.lang.System.in.available() > 1) break;

        /* open a socket to the zumo */
        print("\nconnecting to port " + port);
        var socket = java.net.Socket(host, port);
        var out = java.io.PrintWriter(socket.getOutputStream(), true);
        var ins = java.io.DataInputStream(socket.getInputStream());

        /* open an read data log (if specified on the command line) */
        if (args.length > 1) {
            log = new java.io.FileWriter(args[1], true);
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
