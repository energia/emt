/*
 *  ======== main.cpp ========
 *  Basic TI-RTOS Task startup (no Energia MT references)
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

extern void hello_task(xdc_UArg a0, xdc_UArg arg1);

/*
 *  ======== main ========
 */
int main()
{
    /* initialize all device/board specific peripherals */
    Board_init();

    Task_Params taskParams;

    System_printf("Startup\n");
    System_flush();

    /* initialize taskParams to the defaults */
    Task_Params_init(&taskParams);

    taskParams.priority = Task_numPriorities - 1;
    taskParams.stackSize = 0x800;

    /* Set the task name */
    taskParams.instance->name = (xdc_String)"hello";

    /* Create the task */
    Task_create(hello_task, &taskParams, NULL);

    /* does not return */
    BIOS_start();

    return (0); /* should never get here, but just in case ... */
}

