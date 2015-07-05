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

/* application/sketch setup/loop functions */
#include "app.h"

typedef void (*SFxn)(void);

typedef struct Sketch {
    SFxn    setup;
    SFxn    loop;
    CString name;
    Uns     stackSize;
    Int     priority;
} Sketch;

Sketch sketchTab[] = {
    {imuSetup,   imuLoop,   "imu",   0xc00, 2},
    {motorSetup, motorLoop, "motor", 0xc00, 2},
    {apSetup,    apLoop,    "ap",    0x1000, 2},
    //    {shellSetup, shellLoop, "shell", 0x1000},
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

    Int i = 0;
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
