#include <Energia.h> // Energia Wiring API
#include <WString.h> // Energia Wiring String API
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>

// Pin   Function
//  73 - push button 1
//  74 - push button 2
//  75 - red tri-color LED 02
//  76 - green tri-color LED 02
//  77 - blue tri-color LED 02
//  78 - yellow LED
int ledPin = 77;

String stringOne = String("Hello: ");

__extern void stringSetup(void)
{
    System_printf("stringSetup() ...");
    System_flush();

    System_printf("len: %d\n", (stringOne + " String").length());

    pinMode(ledPin, OUTPUT);    // set ledPin pin as output
}

__extern void stringLoop(void)
{
    Memory_Stats stats;
    static int count = 1;

    digitalWrite(ledPin, HIGH); // set the LED on

    System_printf("%s", (stringOne + count + '\n').c_str());
    count++;
    
    Memory_getStats(NULL, &stats);
    System_printf("heap size: 0x%x, free: 0x%x, max free block: 0x%x\n",
                  stats.totalSize, stats.totalFreeSize, stats.largestFreeSize);
    System_flush();

    delay(50);                  // wait for 1/20 second
    digitalWrite(ledPin, LOW);  // set the LED off
    delay(950);                 // wait for (1 - 1/20) second
}
