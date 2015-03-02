/*
 *  ======== main.cpp ========
 *  TI-RTOS Task startup
 */
#include <stddef.h>

/* XDC Header files */
#include <xdc/cfg/global.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/IHeap.h>

/* TI-RTOS Board support */
#include <ti/drivers/bsp/Board.h>

extern Void alloc_task(UArg a0, UArg arg1);

Void printMem(IHeap_Handle heap);

/*
 *  ======== main ========
 */
int main()
{
    Ptr ptr;
    
    printMem(NULL);
    ptr = Memory_alloc(NULL, 1024, 1, NULL);
    printMem(NULL);
    
    /* initialize all device/board specific peripherals */
    Board_init();  /* this function is generated as part of TI-RTOS config */

    Task_Params taskParams;

    System_printf("Startup\n");
    System_flush();

    /* initialize taskParams and set to default */
    Task_Params_init(&taskParams);

    /* All tasks have the same priority */
    taskParams.priority = Task_numPriorities - 1;
    taskParams.stackSize = 0x800;

    /* Set the task name */
    taskParams.instance->name = (String)"alloc";

    /* Create the task */
    Task_create(alloc_task, &taskParams, NULL);

    printMem(NULL);

    /* does not return */
    BIOS_start();

    return (0); /* should never get here, but just in case ... */
}

/*
 *  ======== printMem ========
 */
Void printMem(IHeap_Handle heap)
{
    Memory_Stats stats;
    Memory_getStats(heap, &stats);
    System_printf("heap size: 0x%x, free: 0x%x, max free block: 0x%x\n",
                  stats.totalSize, stats.totalFreeSize, stats.largestFreeSize);
}
