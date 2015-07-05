/* SYS/BIOS Header files */
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/System.h>

/*
 *  ======== hello_task ========
 */
void hello_task(xdc_UArg arg0, xdc_UArg arg1)
{
    int i;
    
    System_printf("hello_task(%p, %p) ...\n", arg0, arg1);
    System_flush();

    for (i = 0; ; i++) {
        System_printf("%d: hello world\n", i);
        System_flush();
        Task_sleep(1000);
    }
}
