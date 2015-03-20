#include <Energia.h> // Energia Wiring API
#include <WString.h> // Energia Wiring API
#include <xdc/runtime/System.h>

// Pin   Function
//  43 - red tri-color LED 02
//  44 - green tri-color LED 02
//  45 - blue tri-color LED 02
//  46 - red LED 01
int ledPin = 45;

__extern void stringSetup(void)
{
    System_printf("stringSetup() ...");
    System_flush();

    String stringOne = String("Hello");
    Serial.println((stringOne + " String").length());

    pinMode(ledPin, OUTPUT);  // set ledPin pin as output
}

__extern void stringLoop(void)
{
    digitalWrite(ledPin, HIGH);  // set the LED on
    delay(1000);                 // wait for a second
    digitalWrite(ledPin, LOW);   // set the LED off
    delay(1000);                 // wait for a second
}
