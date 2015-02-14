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
	    + dlib + "/MSP432P4xx/ccs/msp432p4xx_driverlib.lib";
    }

    /* '!' tells XDCtools the file won't exist until _after_ generation */
    return ('!' + libs);
}

/*
 *  ======== validate ========
 */
function validate()
{
    return;

    /* patch TI-RTOS getLibs() bug */
    if ("ti.tirtos.Package" in xdc.om) {
	var pkgType = xdc.om["ti.tirtos.Package"];
        pkgType.$patch('getLibs', _newGetLibs);
    }
}

/*
 *  ======== _newGetLibs ========
 *  TI-RTOS getLibs patch
 */
function _newGetLibs(prog)
{
    var libs = this.$oldFxn(prog); /* call previous getLibs() function */
    if (libs != null) {
        print("    " + this.$name + " wants to link: " + libs);
	libs = "../drivers/lib/drivers_MSP432P401R.aem4f;../drivers/ports/tirtos/lib/tirtosport.aem4f";
        print("    WARNING: patching " + this.$name + " getLibs to link: " + libs);
    }

    return (libs);
}
