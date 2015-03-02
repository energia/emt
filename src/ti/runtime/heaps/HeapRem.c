/*
 *  ======== HeapRem.c ========
 */
#include <xdc/std.h>
 
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/IHeap.h>

#include <ti/sysbios/heaps/HeapMem.h>

#include "package/internal/HeapRem.xdc.h"

static HeapMem_Struct heapMemObj; /* the "real" heap instance object */

/* ======== HeapRem_begin ======== */
Void HeapRem_begin(void)
{
    HeapMem_Params params;

    /* create the "real" heap used to do all allocations */
    HeapMem_Params_init(&params);
    params.buf = HeapRem_start;
    params.size = (UArg)HeapRem_end - (UArg)HeapRem_start;
    HeapMem_construct(&heapMemObj, &params);

    module->heap = HeapMem_handle(&heapMemObj);
}

/* ======== HeapRem_Instance_init ======== */
Void HeapRem_Instance_init(HeapRem_Object *obj, 
    const HeapRem_Params *params)
{
    obj->count = 0;
}

/* ======== HeapRem_Module_startup ======== */
Int HeapRem_Module_startup(Int state)
{
    if (!Startup_rtsDone() || !HeapMem_Module_startupDone()) {
        return (Startup_NOTDONE);
    }
 
    HeapRem_begin();
    
    return (Startup_DONE);
}

/* ======== HeapRem_alloc ======== */
Ptr HeapRem_alloc(HeapRem_Object *obj, SizeT size, 
    SizeT align, Error_Block *eb)
{
    if (module->heap == NULL) {
        return (NULL);
    }
    
    return (HeapMem_alloc(module->heap, size, align, eb));
}
 
/* ======== HeapRem_free ======== */
Void HeapRem_free(HeapRem_Object *obj, Ptr block, SizeT size)
{
    HeapMem_free(module->heap, block, size);
}
 
/* ======== HeapRem_isBlocking ======== */
Bool HeapRem_isBlocking(HeapRem_Object *obj)
{
    return (Memory_query((IHeap_Handle)module->heap, Memory_Q_BLOCKING));
}
 
/* ======== HeapRem_getStats ======== */
Void HeapRem_getStats(HeapRem_Object *obj, Memory_Stats *stats)
{
    HeapMem_getStats(module->heap, stats);
}
