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
	libs = libs.replace(/\/\/+/g, '/');
        print("    WARNING: patching " + this.$name 
	      + " getLibs to link: " + libs);
    }

    return (libs);
}
