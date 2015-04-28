/*
 *  ======== Platform.xs ========
 *  Platform support for the CC32xx Launch Pad
 */

/*
 * ======== module$meta$init ========
 */
function module$meta$init()
{
    this.$super.module$meta$init();
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
 */
function instance$meta$init(name)
{
    this.includeLinkCmdFile = true;

    /* We use 'revision' to pass 'the real device name' to the generic
     * MSP432 device.
     */
    this.deviceName = "CC3200";
    this.CPU.revision = this.deviceName;
    this.CPU.catalogName = "ti.catalog.arm.cortexm4";
    this.CPU.clockRate = 80;

    if (Program.build.target.$name.match(/gnu/)) {
        this.codeMemory = "REGION_TEXT";
        this.dataMemory = "REGION_DATA";
        this.stackMemory = "REGION_STACK";
    }
}
