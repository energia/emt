/* 
 * This package's libraries are added to the link line by the
 * ti.runtime.wiring.cc26xx package's getLibs() function based in the 
 * specified board name.  So we add the getLibs() below to ensure our 
 * libraries are not supplied a second time.
 * 
 * ti.runtime.wiring.cc26xx dynamically adds our libraries to:
 *     o ensure that pins.obj always occurs _after_ the wiring library 
 *       (which references the definition in pins.c)
 *     o ensure Board_init.obj occurs _before_ the ti.drivers libraries
 *     o avoid a static dependency on any board-specific packages
 */
function getLibs(prog)
{
    return (null);  
}
