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
     *  ======== variantPath ========
     *  Optional path prefix to use to locate variant libraries
     *
     *  In order to support multiple board variants with a single
     *  closure, the library name provided ti.runtime.wiring.<device>
     *  has a variant-independent name (because it's used in a
     *  linker command file common to all variants.)  In order to use
     *  this linker command file, it's necessary for the user to
     *  supply a variant-specific library search path to ensure that
     *  the right variant library is used.
     *
     *  This config parameter is only used to enable the creation of 
     *  longer variant-independent name for the library.  For example,
     *  setting `variantPath` to "variants/" will cause the the
     *  following line to appear in the generated linker command
     *  file:
     *  @p(code)
     *      variant/lib/board.<target suffix>.lib
     *  @p
     *  rather than the default of just:
     *  @p(code)
     *      lib/board.<target suffix>.lib
     *  @p
     */
    config String variantPath = "";
    
    /*!
     *  ======== driverLibPattern ========
     *  A pattern used to locate DriverLib along the package path
     *
     *  DriverLib is located by looking for a directory formed by
     *  appending the pattern plus "/inc" to each directory of the
     *  package path.
     *
     *  The pattern may contain a single '*' character which is used
     *  to denote any sequence of characters except the directory
     *  separation character '/'.
     *
     *  Suppose, for example, the directory `/ti/msp430` on the
     *  package path contains `MSP430Ware_1_97_00_47`.  The pattern
     *  "MSP43* /msp430ware/driverlib" will identify the directory
     *  "/ti/msp430/MSP430Ware_1_97_00_47/msp430ware/driverlib" as the 
     *  location of DriverLib.
     */
    config String driverLibPattern;
    
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

    config Bool generateSources = false;

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
     *  Automatically include a device-specific linker command file
     *  
     *  This flag specifies whether the platform should include a default
     *  device-specific linker command file.
     *
     *  By default, a user is responsible for supplying a device and
     *  toolchain-specific linker command file to the linker. However, if
     *  this flag is set to `true`, this platform will include an appropriate
     *  default linker command file for the selected device and toolchain.
     */
    config Bool includeLinkCmdFile = false;

    /*!
     *  ======== variant ========
     *  Name of the board variant to use
     *
     *  Board variants are small libraries of code that customize
     *  TI-RTOS and Energia for a specific hardware platform.  This library
     *  contains the TI-RTOS "board.c" driver configuration, pin mux setup
     *  code, as well as Energia pin map tables.
     *
     *  Variant names are used to create a path name to a library whose
     *  basename is variant-independent, such as "board.lib".  For example,
     *  variant libraries are typically contained in a directory whose name
     *  is the variant name.  Suppose that `variant` is set to
     *  "CC3200_LAUNCHXL", the full path name of the variant library named
     *  "board.lib" is
     *  @p(code)
     *      <some_path>/CC3200_LAUNCHXL/lib/board.lib
     *  @p
     */
    config String variant;
}
