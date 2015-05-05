/*
 *  ======== Platform.xdc ========
 */

/*
 *  ======== Platform ========
 */
metaonly module Platform inherits ti.platforms.launchpad.IPlatform
{
    override config xdc.platform.IPlatform.Board BOARD = {
        id:             "0",
        boardName:      "CC3200-LAUNCHXL",
        boardFamily:    "CC3200_Launch_Pad",
        boardRevision:  "3.2"
    };

    /*!
     *  ======== driverLibPattern ========
     *  The pattern used to locate CC3200 DriverLib
     *
     *  The default setting below locates driverLib relative to the TI-RTOS
     *  repo which must be on the package path in order to access reentrant
     *  SYS/BIOS peripheral drivers.
     *
     *  Note: this default may be dynamically changed in this module's
     *  module$meta$init() startup function (defined in Platform.xs).
     *
     *  @see ti.platforms.launchpad.IPlatform#driverLibPattern
     */
    override config String driverLibPattern = "../products/CC3200_driverlib_*";

instance:

    /*
     *  This platform supports CC32xx devices with Cortex M4 cores.
     */
    override config xdc.platform.IExeContext.Cpu CPU = {
        id:             "0",
        clockRate:      80.0,
        catalogName:    "ti.catalog.arm.cortexm4",
        deviceName:     "CC32xx",
        revision:       "",
    };

    override config String variant = "CC3200_LAUNCHXL";
}

