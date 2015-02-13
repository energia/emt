/*
 *  ======== IPlatform.xdc ========
 */

/*!
 *  ======== IPlatform ========
 *  A generic platform that supports any MSP432 device
 *
 *  The device to be used by this platform is passed as the platform instance
 *  name. On the `xdc.tools.configuro` command line, it is done in the
 *  following way:
 *  @p(code)
 *  xs xdc.tools.configuro ... -p "ti.platforms.emt432"
 *  @p
 *
 *  In package.bld, the platform instance is selected as in:
 *  @p(code)
 *  Pkg.addExecutable("test", target, "ti.platforms.emt432");
 *  @p
 */
@Template ("ti/platforms/launchpad/Platform.xdt")
metaonly interface IPlatform inherits xdc.platform.IPlatform
{
    config xdc.platform.IPlatform.Board BOARD;

    /*!
     *  ======== addDriverLibs ========
     *  Automatically add DriverLib libraries to linker command line.
     *
     *  Setting this option to `false`, requires that you must
     *  explicitly add the appropriate `driverlib.a` to your
     *  application's link line.
     */
    config Bool addDriverLibs = true;

    /*!
     *  ======== findDriverLib ========
     *  Return an absolute path to the device DriverLib library
     *
     *  This library is required by TI-RTOS drivers
     */
    function findDriverLib();

instance:

    /*
     *  This platform supports MSP432 devices with Cortex M4F cores.
     *  The corresponding "actual" catalogName is overwritten
     *  in Platform.instance$meta$init() based on the device name
     */
    config xdc.platform.IExeContext.Cpu CPU;

    /*!
     *  ======== deviceName ========
     *  The name of an `ICpuDataSheet` module for the device
     *
     *  This parameter is required, but it does not have to be set explicitly;
     *  it can be encoded in the instance's name.
     */
    config String deviceName;

    /*!
     *  ======== clockRate ========
     *  The clock rate for this device.
     */
    config Double clockRate;

    override config String codeMemory = null;

    override config String dataMemory = null;

    override config String stackMemory = null;

    /*!
     *  ======== includeLinkCmdFile ========
     *  The flag that specifies if the platform should include a linker command
     *  file.
     *
     *  By default, a user is responsible for adding a linker command file to
     *  the project, or to the linker command line. However, if this flag is
     *  set, this platform will include a default linker command file for the
     *  selected device.
     */
    config Bool includeLinkCmdFile = false;
}
