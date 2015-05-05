/*
 *  ======== Package.init ========
 *  Called at initialization time (before user config script runs)
 */
function init()
{
    if (xdc.om.$name != 'cfg') {
        return;
    }

    if (Program.sectMap[".bootVecs"] === undefined) {
        Program.sectMap[".bootVecs"] = new Program.SectionSpec();
        Program.sectMap[".bootVecs"].loadAddress = 0x20004000;
    }
}

/*
 *  ======== Package.getLibs ========
 *  Called during generation to get libraries to link with
 */
function getLibs(prog)
{
    /* get Platform module */
    var Platform = xdc.om[this.$name + ".Platform"];

    var libs;
    if (Platform.$private.src) {
	/* get the output directory */
	var outDir = Platform.$private.src.getGenSourceDir();

	/* convert to absolute path since it's usually _not_ in this package */
	outDir = String(java.io.File(outDir).getAbsolutePath());

	/* construct appropriate lib file sequence to link with */
	libs = outDir + "/Board_init.obj";
    }

    /* get required DriverLib libraries (if requested) */
    var dlib = Platform.addDriverLibs ? Platform.findDriverLib() : null;
    if (dlib != null) {
	libs = (libs ? (libs + ';') : "")
	    + dlib + "/driverlib/ccs/Release/driverlib.a";
    }

    /* '!' tells XDCtools the file won't exist until _after_ generation */
    return (libs ? ('!' + libs) : null);
}
