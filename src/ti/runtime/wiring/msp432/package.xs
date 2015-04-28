/*
 *  ======== Package.getLibs ========
 */
function getLibs(prog)
{
    /* find board-specific variant library */
    var prefix = this.$name.replace(/\./g, "/");

    var platform = prog.platform.$orig;
    if (! "variant" in platform) {
	this.$logError("platform: " + platform.$name 
            + " does not supply a variant name required for ti.runtime.wiring",
            this);
	return (null);
    }
	
    var vbase = xdc.findFile(prefix + "/variants/" + platform.variant);

    if (vbase) {
	var vlib = vbase + "/lib/" + platform.variant + ".lib";
	return ("lib/" + this.$name + ".lib;" + vlib);
    }

    this.$logWarning("unknown board variant: " + platform.variant, this);
    return (null);
}
