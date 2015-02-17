function getLibs(prog)
{
    /* get Platform module */
    var Platform = xdc.om[this.$name + ".Platform"];

    /* get the output directory */
    var outDir = Platform.$private.src.getGenSourceDir();

    /* convert to an absolute path since it's usually _not_ in this package */
    outDir = String(java.io.File(outDir).getAbsolutePath());

    /* construct appropriate lib file sequence to link with */
    var libs = outDir + "/Board_init.obj";

    /* get required DriverLib libraries (if requested) */
    var dlib = Platform.addDriverLibs ? Platform.findDriverLib() : null;
    if (dlib != null) {
	libs = libs + ';' 
	    + dlib + "/driverlib/ccs/Release/driverlib.a";
    }

    /* '!' tells XDCtools the file won't exist until _after_ generation */
    return ('!' + libs);
}
