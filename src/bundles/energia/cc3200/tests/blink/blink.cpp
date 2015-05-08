#include <Energia.h> // Energia Wiring API

// Pin   Function
//  11 - push button SW 2
//   3 - push button SW 3
//  10 - green  LED 05
//   9 - yellow LED 06
//  29 - red    LED 07
int ledPin = RED_LED;

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
