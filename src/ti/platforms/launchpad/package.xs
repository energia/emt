function getLibs(prog)
{
    /* find the Platform module that inherits from our IPlatform interface */
    var IPlatform = xdc.om[this.$name + ".IPlatform.Module"];
    var mods = xdc.om.$modules;
    for (var i = 0; i < mods.length; i++) {
        var mod = mods[i];
	if (mod instanceof IPlatform) {
            // print("==== " + mod.$name + " is an " + IPlatform.$name);
        }
    }

    return (null);
}
