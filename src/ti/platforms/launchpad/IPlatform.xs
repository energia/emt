/*
 *  ======== Platform.xs ========
 *  Generic platform support for EMT boards
 */

var Utils = xdc.useModule('xdc.platform.Utils');

/*
 *  ======== Platform.getCpuDataSheet ========
 *  This Platform's implementation xdc.platform.IPlatform.getCpuDataSheet.
 *
 *  Return the xdc.platform.ICpuDataSheet.Instance object that corresponds
 *  to the device that runs executables loaded on the specified cpuId.
 */
function getCpuDataSheet(cpuId)
{
    var cpuAttrs = this.CPU.$copy();

    cpuAttrs.deviceName = this.deviceName;

    return (Utils.getCpuDataSheet(cpuAttrs));
}

/*
 *  ======== Platform.getExeContext ========
 *  This Platform's implementation xdc.platform.IPlatform.getExeContext.
 *
 *  Return the xdc.platform.IExeContext.Instance object that will run the
 *  specified program prog.
 */
function getExeContext(prog)
{
    /* create a default ExeContext execution context */
    var ExeContext = xdc.useModule('xdc.platform.ExeContext');

    var cpu = ExeContext.create(this.CPU, this.$module.BOARD);
    var revision = this.CPU.revision;

    /* This is where we have peripherals available, so we can get their base
     * addresses and other info.
     */

    cpu.memoryMap = Utils.assembleMemoryMap(cpu, this);
    cpu.deviceName = revision;
    cpu.revision = "";

    if (prog.build.target.$name.match(/gnu/)) {
        if (this.codeMemory == undefined) {
            this.codeMemory = "REGION_TEXT";
        }
        if (this.dataMemory == undefined) {
            this.dataMemory = "REGION_DATA";
        }
        if (this.stackMemory == undefined) {
            this.stackMemory = "REGION_STACK";
        }
    }
    /* check for the overlap in the memory map */
    var overlap = Utils.checkOverlap(cpu.memoryMap);

    if (overlap != null) {
        this.$module.$logError("Memory objects " + overlap + " overlap", this,
            overlap);
    }

    this.$seal();
    return (cpu);
}

/*
 *  ======== Platform.getExecCmd ========
 *  This Platform's implementation xdc.platform.IPlatform.getExecCmd
 */
function getExecCmd(prog, packagePath)
{
    var os = environment["xdc.hostOS"];
    var doNotKnow = "@$(ECHO) " + prog.platform.$package.$name 
	+ " platform package cannot execute " 
	+ prog.name + " on " + os + "\n";
    return (doNotKnow);
}

/*
 *  ======== Platform.getLinkTemplate ========
 *  This Platform's implementation xdc.platform.IPlatform.getLinkTemplate
 */
function getLinkTemplate(prog)
{
    /* hack to get inherited Platform package name */
    var tname = this.$super.$type;
    var iname = tname.substring(0, tname.lastIndexOf('.'));
    var prefix = xdc.om[iname].$package.$name.replace(/\./g, '/');

    if (prog.build.target.$name.match(/gnu/)) {
        return (prefix + "/linkcmd_gnu.xdt");
    }
    else if (prog.build.target.$name.match(/iar/)) {
        return (prefix + "/linkcmd_iar.xdt");
    }
    else {
        return (prefix + "/linkcmd.xdt");
    }
}

/*
 *  ======== Platform.instance$meta$init ========
 *  This function is called to initialize a newly created instance of a
 *  platform.  Platform instances are created just prior to running
 *  program configuration scripts.
 *
 *  Platform instances may also be created in the build domain.
 *
 *  @param(name)        the name used to identify this instance (without
 *                      the package name prefix)
 *
 *  For this platform, 'name' must identify a device. Besides the catalog
 *  module, this parameter may encode values for other configuration
 *  parameters, as defined by `nameFormat`. The values are separated by ':'.
 */
function instance$meta$init(name)
{
    var maxParamsLength = this.$module.nameFormat.split(":").length;
    var nameParams = name.split(":");

    if (nameParams.length > maxParamsLength) {
        this.$module.$logWarning("This platform accepts "
            + "only " + maxParamsLength + " instance parameters in its name. "
            + "The supplied string '" + name + "' contains additional values, "
            + "which will be ignored.", this, this.$module.nameFormat);
    }

    if (nameParams[0] != "" && this.deviceName != null
        && this.deviceName != nameParams[0]) {
        this.$module.$logError(this.$package.$name
            + " was passed two different device names: '" + nameParams[0]
            + "' and '" + this.deviceName + "'.", this, this.deviceName);
    }
    else if (nameParams[0] != "") {
        this.deviceName = nameParams[0];
    }

    if (this.deviceName == null) {
        this.$module.$logFatal("A device name must be specified for the "
            + "platform '" + this.$package.$name + "'. A device name can be "
            + "specified in the platform name, as "
            + "'" + this.$package.$name + ":MSP432XYZ', where MSP432XYZ is the name "
            + "of the selected device.", this, this.deviceName);
    }

    if (nameParams[1] != "false" && Boolean(nameParams[1]).valueOf()) {
        this.includeLinkCmdFile = true;
    }

    if (nameParams[2] != undefined && !isNaN(parseFloat(nameParams[2]))) {
        this.clockRate = parseFloat(nameParams[2]);
    }

    if (this.externalMemoryMap.length != 0) {
        this.$module.$logWarning(this.$package.$name + " does not support "
            + "external memory map, and the parameter 'externalMemoryMap' is "
            + "ignored.", this, this.externalMemoryMap);
    }

    if (Program.build.target.$name.match(/gnu/)) {
        this.codeMemory = "REGION_TEXT";
        this.dataMemory = "REGION_DATA";
        this.stackMemory = "REGION_STACK";
    }

    /* if in the configuration model ... */
    if (xdc.om.$name == "cfg") {
        /* We use 'revision' to pass 'the real device name' to the generic
         * device.
         */
        this.CPU.revision = this.deviceName;

        if (this.deviceName.match(/^MSP432/)) {
            this.CPU.catalogName = "ti.catalog.arm.cortexm4";
            this.CPU.clockRate = 48;
        }
    }
}

/*
 * ======== module$meta$init ========
 */
function module$meta$init()
{
    if (this.generateSources) {
	/* get source directory for generated files */
	var SourceDir = xdc.useModule("xdc.cfg.SourceDir");

	/* compute unique name for the platform sources */
	var k = this.$package.$name.lastIndexOf('.');
	var sdir = this.$package.$name.substring(k + 1);
	this.$private.src = SourceDir.create(sdir);
    }
}

/*
 *  ======== findDriverLib ========
 */
function findDriverLib()
{
    var file = _wildcard(this.driverLibPattern + "/inc");
    if (file == null) {
	throw new Error("can't find DriverLib along the path '"
		       + xdc.curPath() + "' using the pattern '"
		       + this.driverLibPattern + "/inc'");
    }
    file = file.substring(0, file.lastIndexOf('/inc'));

    return (file);
}

/*
 *  ======== _wildcard ========
 */
function _wildcard(pat)
{
    var dirs = xdc.curPath().split(';');
    var pa = pat.split('*');

    var k = pa[0].lastIndexOf('/');
    var prefix;
    var middle;
    if (k >= 0) {
        prefix = '/' + pa[0].substring(0, k);
        middle = pa[0].substring(k + 1);
    }
    var suffix = pa[1] ? (pa[1] + '/') : null;
    
    for (var i = 0; i < dirs.length; i++) {
        var root = dirs[i];
	if (root != "") {
            root += prefix;
	    var file = _find(root, middle);
	    if (file != null) {
                if (suffix == null) {
                    return file;
                }
                file += suffix;
                if (java.io.File(file).exists()) {
                    return (file);
                }
	    }
	}
    }

    return (null);
}

/*
 *  ======== _find ========
 *  Find the directory <dirName>/prefix* 
 */
function _find(dirName, prefix) {
    //print("    looking for " + dirName + '/' + prefix + '*');
    /* create a file name filter */
    function match(dir, name) {
        if (String(name).indexOf(prefix) == 0) {
            return (true);
        }
        return (false);
    }
    var filter = java.io.FilenameFilter({accept: match});

    /* get files matching the filter */
    var files = java.io.File(dirName).list(filter);

    /* if a match is found, return full path name; otherwise null */
    if (files != null && files.length > 0) {
        //print("    found: " + dirName + "/" + files[0]);
        return (dirName + "/" + files[0]);
    }
    return (null);
}
