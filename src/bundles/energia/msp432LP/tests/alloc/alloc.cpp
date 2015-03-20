/* BIOS Header files */
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Error.h>

extern void printMem(IHeap_Handle heap);

/*
 *  ======== main task ========
 */
void alloc_task(xdc_UArg arg0, xdc_UArg arg1)
{
    int i;
    xdc_Ptr ptr;
    int total;
    static Error_Block eb;
    
    System_printf("alloc_task(%p, %p) ...\n", arg0, arg1);
    System_flush();

    for (total = i = 0; ; i++) {
        ptr = Memory_alloc(NULL, 1024, 1, &eb);
        total += 1024;
        System_printf("%d: alloc: %p (total %ld)\n", i, ptr, total);
        if (ptr == NULL) {
            break;
        }
        if (i & 0x1) {
            Memory_free(NULL, ptr, 1024);
            total -= 1024;
        }
        printMem(NULL);
        System_flush();
        Task_sleep(1000);
    }

    printMem(NULL);
    System_printf("alloc: all done.\n");
    System_flush();
}
