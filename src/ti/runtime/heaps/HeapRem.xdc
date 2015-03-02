import xdc.runtime.Error;
import ti.sysbios.heaps.HeapMem;
 
/*!
 *  ======== HeapRem ========
 *  Variable sized first-fit allocation heap manager
 *  
 *  `HeapRem` manages all remaining (unused) memory as defined by the two
 *  configuration parameters: `start` and `end`.  These config parameters are
 *  typically initialized to symbols defined by the linker.
 *  
 *  To support variable sized allocations, `HeapRem` delegates to an instance
 *  of `ti.sysbios.heaps.HeapMem` created at startup, .  This module can be
 *  eliminated when/if `HeapMem` supports the definition of a heap via linker
 *  defined symbols; currently (2015) HeapMem only supports statically
 *  configurable heap sizes.
 */
@ModuleStartup  /* this module has a startup fxn to be called prior to main */
module HeapRem inherits xdc.runtime.IHeap
{
    /*!
     *  ======== start ========
     *  First valid address of memory that can be dynamically allocated
     *
     *  `HeapRem` manages memory between the values specified by the config
     *  parameters `start` and `end`; valid memory includes bytes (actually
     *  MAUs) in the range [`start`, `end`).
     */
    config Char *start = "&__UNUSED_SRAM_start__";

    /*!
     *  ======== end ========
     *  One more than the largest possible return value from Memory_alloc
     *
     *  @see start
     */
    config Char *end = "&__UNUSED_SRAM_end__";

internal:
    struct Module_State {
        HeapMem.Handle heap;
    };

    struct Instance_State {
        Int count;
    };
}
