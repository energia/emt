/******************************************************************************
 *
 * Default Linker Command file for the Texas Instruments CC3200
 *
 *****************************************************************************/

--retain=g_pfnVectors

MEMORY
{
    SRAM (RWX) : origin = 0x20004000, length = 0x00040000 - 0x4000
}

/* The following command line options are set as part of the CCS project.    */
/* If you are building using the command line, or for some reason want to    */
/* define them here, you can uncomment and modify these lines as needed.     */
/* If you are using CCS for building, it is probably better to make any such */
/* modifications in your CCS project and leave this file alone.              */
/*                                                                           */
/* --heap_size=0                                                             */
/* --stack_size=256                                                          */
/* --library=rtsv7M4_T_le_eabi.lib                                           */

/* Section allocation in memory */

SECTIONS
{
    .intvecs:   > 0x20004000
    .text   :   > SRAM
    .const  :   > SRAM
    .cinit  :   > SRAM
    .pinit  :   > SRAM
    .init_array : > SRAM

    .vtable :   > 0x20004000

    /* start stack at the highest physical address (since it grows to lower
     * addresses.
     *
     * start heap at the end of all RAM data and extend downward wward the
     * stack top
     *
     * define __UNUSED_SRAM_start__ and __UNUSED_SRAM_end__ to identify
     * unused memory to be used as {extra heap space, stack overrun check
     * boundary, ...}
     */
    GROUP {
        .data
        .bss
        .sysmem
        empty: {
            __UNUSED_SRAM_start__ = .;
            __UNUSED_SRAM_end__ = __stack;
            __SRAM_LENGTH__ = size(SRAM);
        }
    } > SRAM

    .stack  : > SRAM (HIGH)
}

__STACK_TOP = __stack + 512;
