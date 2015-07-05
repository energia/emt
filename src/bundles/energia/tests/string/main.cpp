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

/* TI-RTOS Board support */
#include <ti/drivers/bsp/Board.h>

typedef void (*SFxn)(void);

typedef struct Sketch {
    SFxn         setup;
    SFxn         loop;
    const char   *name;
    unsigned int stackSize;
    int          priority;
} Sketch;
__extern void stringSetup(void);
__extern void stringLoop(void);

Sketch sketchTab[] = {
    {stringSetup,    stringLoop,    "stringLoop",    0x1000, 1},
};

#define NUM_SKETCHES (sizeof(sketchTab) / sizeof (Sketch))

/*
 *  ======== sketchTask ========
 */
void sketchTask(xdc_UArg _task_setup, xdc_UArg _task_loop)
{
    /* Call setup once */
    (*(void(*)())_task_setup)();

    /* Call loop repeatedly */
    for (;;) {
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
    Board_init();

    Task_Params taskParams;

    /* initialize taskParams to the defaults */
    Task_Params_init(&taskParams);

    int i = 0;
    for (i = 0; i < NUM_SKETCHES; i++) {
        taskParams.stackSize = sketchTab[i].stackSize;
        taskParams.priority =  sketchTab[i].priority;
        
        taskParams.arg0 = (xdc_UArg)sketchTab[i].setup;
        taskParams.arg1 = (xdc_UArg)sketchTab[i].loop;
        taskParams.instance->name = (xdc_String)sketchTab[i].name;

        /* create the sketch task */
        Task_create(sketchTask, &taskParams, NULL);
    }

    /* enable interrupts and start all threads */
    BIOS_start(); /* does not return */

    return (0);   /* should never get here, but just in case ... */
}
