#include <Energia.h> // Energia Wiring API

// Pin   Function
//  43 - red tri-color LED 02
//  44 - green tri-color LED 02
//  45 - blue tri-color LED 02
//  46 - red LED 01
int ledPin = 45;

__extern void blinkSetup(void)
{
    pinMode(ledPin, OUTPUT);  // set ledPin pin as output
}

__extern void blinkLoop(void)
{
    digitalWrite(ledPin, HIGH);  // set the LED on
    delay(1000);                 // wait for a second
    digitalWrite(ledPin, LOW);   // set the LED off
    delay(1000);                 // wait for a second
}
