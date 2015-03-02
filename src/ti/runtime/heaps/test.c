#include "HeapRem.h"
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

char __UNUSED_SRAM_end__[1];
char __UNUSED_SRAM_start__[16 + 1024];

/*
 *  ======== main ========
 */
int main(int argc, char *argv[])
{
    Ptr ptr = HeapRem_alloc(myHeap, 1, 1, NULL);
    System_printf("ptr = %p\n", ptr);

    return (0);
}
