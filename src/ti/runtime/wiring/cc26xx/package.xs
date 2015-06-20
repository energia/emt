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
	
    /* select board-specific variant package directory */
    var prefix = this.$name.replace(/\./g, "/");
    var ext = "." + prog.build.target.suffix + ".lib";
    var vname = prefix + "/variants/" + platform.variant;
    var vbase = xdc.findFile(vname);
    if (vbase == null) {
	prog.$logWarning("unknown board variant '" + platform.variant 
			 + "': expected to find board support files in "
			 + vname, this);
	return (null);
    }

    /* return our lib + the pre-compiled board-support library */
    var lib = this.$name.substring(this.$name.lastIndexOf('.') + 1) + ext;
    var vlib = vbase + "/lib/" + platform.variant + ext;
    if (xdc.findFile(vlib) == null) {
	prog.$logWarning("can't find variant library '" + vlib 
			 + "': expected to find board support files in "
			 + vname, this);
	return ("lib/" + lib);
    }
    return ("lib/" + lib + ";" + vlib);
}
