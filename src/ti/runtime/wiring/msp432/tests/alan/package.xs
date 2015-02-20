function validate()
{
    var pkgType = xdc.om["ti.tirtos.Package"];
    if (pkgType != null) {
//        pkgType.$patch('getLibs', newGetLibs);
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
