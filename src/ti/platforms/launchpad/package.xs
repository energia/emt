function getLibs(prog)
{
    /* find the Platform module that inherits from out IPlatform interface */
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

function validate()
{
    return;

    if ("ti.tirtos.Package" in xdc.om) {
        var pkgType = xdc.om["ti.tirtos.Package"];
        pkgType.$patch('getLibs', newGetLibs);
    }
}

function newGetLibs(prog)
{
    var libs = this.$oldFxn(prog); /* call previous getLibs() function */
    if (libs != null) {
        print("    " + this.$name + " wants to link: " + libs);
        libs = "../drivers/lib/drivers_MSP432P401R.aem4f;../drivers/ports/tirtos/lib/tirtosport.aem4f";
        print("    WARNING: patching " + this.$name + " getLibs to link: " + libs);
    }

    return (libs);
}
