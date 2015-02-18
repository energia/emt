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

/* TI-RTOS Board support */
#include <ti/drivers/bsp/Board.h>

extern Void hello_task(UArg a0, UArg arg1);

/*
 *  ======== main ========
 */
int main()
{
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
    taskParams.instance->name = (String)"hello";

    /* Create the task */
    Task_create(hello_task, &taskParams, NULL);

    /* does not return */
    BIOS_start();

    return (0); /* should never get here, but just in case ... */
}

