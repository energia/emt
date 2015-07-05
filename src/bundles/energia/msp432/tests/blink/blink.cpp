#include <Energia.h> // Energia Wiring API
#include <ti/drivers/Power.h>

// Pin   Function
//  73 - push button 1
//  74 - push button 2
//  75 - red tri-color LED 02
//  76 - green tri-color LED 02
//  77 - blue tri-color LED 02
//  78 - yellow LED
int ledPin = 77;

int period = 0;
int status = 0;

__extern void blinkSetup(void)
{
    pinMode(ledPin, OUTPUT);    // set ledPin pin as output
    //    status = Power_setPerformanceLevel(0);
}

volatile int i;

__extern void blinkLoop(void)
{
    digitalWrite(ledPin, HIGH); // set the LED on
    for (i = 0; i < 1000; i++) {
        ;
    }
    //delay(period);              // wait for period ms
    digitalWrite(ledPin, LOW);  // set the LED off
    delay(1000 - period);       // wait for remainder of 1 second
}
