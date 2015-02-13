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
        boardName:      "MSP-EXP432P401R",
        boardFamily:    "MSP432_Launch_Pad",
        boardRevision:  "1.0"
    };

instance:

    /*
     *  This platform supports MSP432 devices with Cortex M4F cores.
     */
    override config xdc.platform.IExeContext.Cpu CPU = {
        id:             "0",
        clockRate:      1.0,
        catalogName:    "ti.catalog.arm.cortexm4",
        deviceName:     "MSP432",
        revision:       "",
    };

    override config string codeMemory = null;

    override config string dataMemory = null;

    override config string stackMemory = null;
}

