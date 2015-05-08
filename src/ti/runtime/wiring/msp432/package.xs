/*
 *  ======== Package.getLibs ========
 */
function getLibs(prog)
{
    /* ensure platform supports specification of board variants */
    var platform = prog.platform.$orig;
    if ((!"variant" in platform) || platform.variant == null) {
	prog.$logError("platform: " + platform.$name 
            + " does not supply a variant name required for ti.runtime.wiring",
            this);
	return (null);
    }
	
    /* find a compatible target library suffix */
    var suffix = prog.build.target.findSuffix(this);
    if (suffix == null) {
	prog.$logWarning("unsupported target '" + prog.build.target.$name 
	    + "': the target's suffix '" + prog.build.target.suffix
            + "' isn't compatible with any library provided by this pack",
	    this);
	return (null);
    }
    var ext = "." + suffix + ".lib";

    /* select board-specific variant package directory */
    var vbase = "!/${variant}/";
    if (platform.$module.addDriverLibs) {
	var prefix = this.$name.replace(/\./g, "/");
	var vname = prefix + "/variants/" + platform.variant;
	vbase = xdc.findFile(vname);
	if (vbase == null) {
	    prog.$logWarning("unknown board variant '" + platform.variant 
			     + "': expected to find board support files in "
			     + vname, this);
	    return (null);
	}
	vbase += '/';
    }

    /* return our lib + the pre-compiled board-support library */
    var lib = this.$name.substring(this.$name.lastIndexOf('.') + 1) + ext;
    var vlib = vbase + "lib/board" + ext;
    return ("lib/" + lib + ";" + vlib);
}
