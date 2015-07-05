#include <Energia.h> // Energia Wiring API

int period = 1;

extern "C" void switchToRTCTimer(void);

__extern void blinkSetup(void)
{
    pinMode(RED_LED, OUTPUT);    // set ledPin pin as output
    //switchToRTCTimer();
}

__extern void blinkLoop(void)
{
    digitalWrite(RED_LED, HIGH); // set the LED on
    delay(period);               // wait for period ms
    digitalWrite(RED_LED, LOW);  // set the LED off
    delay(1000 - period);        // wait for remainder of 1 second
}
