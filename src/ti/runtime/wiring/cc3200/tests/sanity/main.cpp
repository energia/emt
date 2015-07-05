/*
 *  ======== main.cpp ========
 *  MT wiring Task framework
 */
#include <stddef.h>

/* XDC Header files */
#include <xdc/cfg/global.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/System.h>

/* Board Support Header files (from configuration closure) */
#include <Energia.h>

__extern void blinkSetup(void);
__extern void blinkLoop(void);

void (*func_ptr[][2])(void) = {
    {blinkSetup, blinkLoop}
};

const char *taskNames[] = {
    "blinkLoop"
};

#define NUM_SKETCHES (sizeof(taskNames) / 4)

static int gi;
static int sj;

/*
 *  ======== main task ========
 */
void the_task(xdc_UArg _task_setup, xdc_UArg _task_loop)
{
    System_printf("%p", &sj, &gi);
    
    /* Call setup once */
    (*(void(*)()) _task_setup)();

    /* Call loop repeatedly */
    for(;;) {
        Task_yield();
        (*(void(*)()) _task_loop)();
        System_flush();
    }
}

/*
 *  ======== main ========
 */
int main()
{
    /* initialize all device/board specific peripherals */
    Board_init();  /* this function is generated as part of TI-RTOS config */

    Task_Params taskParams;

    /* initialize taskParams to the defaults */
    Task_Params_init(&taskParams);

    /* All tasks have the same priority */
    taskParams.priority = Task_numPriorities - 1;
    taskParams.stackSize = 0x800;

    uint8_t i = 0;
    for (i = 0; i < NUM_SKETCHES; i++) {
        /* Set arg0 to setup() */
        taskParams.arg0 = (xdc_UArg)func_ptr[i][0];
        /* Set ar1 to loop */
        taskParams.arg1 = (xdc_UArg)func_ptr[i][1];
        /* Set the task name */
        taskParams.instance->name = (xdc_String)taskNames[i];
        /* Create the task */
        Task_create(the_task, &taskParams, NULL);
    }

    /* does not return */
    BIOS_start();

    return (0); /* should never get here, but just in case ... */
}

