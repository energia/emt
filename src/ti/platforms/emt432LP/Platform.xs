/*
 *  ======== Platform.xs ========
 *  Platform support for the MSP432 Launch Pad
 */

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
    this.deviceName = "MSP432P401R";

    this.includeLinkCmdFile = true;

    if (Program.build.target.$name.match(/gnu/)) {
        this.codeMemory = "REGION_TEXT";
        this.dataMemory = "REGION_DATA";
        this.stackMemory = "REGION_STACK";
    }

    /* We use 'revision' to pass 'the real device name' to the generic
     * MSP432 device.
     */
    this.CPU.revision = this.deviceName;
    this.CPU.catalogName = "ti.catalog.arm.cortexm4";
    this.CPU.clockRate = 48;
}

