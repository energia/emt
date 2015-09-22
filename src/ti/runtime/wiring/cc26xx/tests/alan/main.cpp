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
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/family/arm/lm4/Timer.h>
#include <xdc/runtime/System.h>

#include <Energia.h>

/* magic insertion point 769d20fcd7a0eedaf64270f591438b01 */
extern void setup0(), loop0();
extern void setup1(), loop1();
extern void setup2(), loop2();
extern void setup3(), loop3();
extern void setup4(), loop4();
extern void setup5(), loop5();
extern void setup6(), loop6();
extern void setup7(), loop7();


void (*func_ptr[][2])(void) = {
//	{setup0, loop0},
//	{setup1, loop1},
	{setup2, loop2},
//	{setup3, loop3},
//	{setup4, loop4},
//	{setup5, loop5},
//	{setup6, loop6},
//	{setup7, loop7}
};

const char *taskNames[] = {
//	"loop0",
//	"loop1",
	"loop2",
//	"loop3",
//	"loop4",
//	"loop5",
//	"loop6",
//	"loop7"
};

#define NUM_SKETCHES sizeof(taskNames)/4

/*
 *  ======== main task ========
 */
Void the_task(UArg _task_setup, UArg _task_loop)
{
    /* Call setup once */
    (*(void(*)()) _task_setup)();

    /* Call loop repeatedly */
    for(;;) {
        (*(void(*)()) _task_loop)();
        System_flush();
        Task_yield();
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

    /* initialize taskParams and set to default */
    Task_Params_init(&taskParams);

    /* All tasks have the same priority */
    taskParams.priority = Task_numPriorities - 1;
    taskParams.stackSize = 0x800;

    uint8_t i = 0;
    for (i = 0; i < NUM_SKETCHES; i++) {
        /* Set arg0 to setup() */
        taskParams.arg0 = (UArg)func_ptr[i][0];
        /* Set ar1 to loop */
        taskParams.arg1 = (UArg)func_ptr[i][1];
        /* Set the task name */
        taskParams.instance->name = (xdc_String) taskNames[i];
        /* Create the task */
        Task_create(the_task, &taskParams, NULL);
    }

    /* does not return */
    BIOS_start();

    return (0); /* should never get here, but just in case ... */
}
