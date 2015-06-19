/*
 *  ======== ti.runtime.wiring.cc3200.tests.wifi ========
 *  Simple WiFi stress test
 *
 *  The executable establishes an Access Point (named tink3AP) and a
 *  "memory read" server running on port 8080 which accepts commands to
 *  read memory and replies with the content of memory at the specified
 *  address.
 *
 *  HW: cc3200 launchpad and a WiFi client that can run XDCtools xs
 *  
 *  To run the test:
 *      o load and start wifi.xm4g using CCS, optionally open the
 *        launchpad's USB serial port (to monitor progress of wifi.xm4g)
 *      o on the WiFi client connect to the tink3AP network (started by
 *        wifi.xm4g)
 *      o run readmem.xs in the WiFi client:
 *            xs -f readmem.xs 64
 *      o stop readmem.xs by typing <ret><ret>
 *      o it should be possible to re-run and stop readmem.xs repeatedly
 *      
 *  Expected results:
 *      o wifi.xm4g echo the read commands it resives on the serial port, and
 *      o readmem.xs displayes the values of memory received from wifi.xm4g
 */
package ti.runtime.wiring.cc3200.tests.wifi {
}
