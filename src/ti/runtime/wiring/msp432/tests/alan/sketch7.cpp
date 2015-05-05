#include <Energia.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

void buttonFxn0()
{
    static bool off = true;

    if (off) {
    	off = false;
    	digitalWrite(YELLOW_LED, 1);
    }
    else {
    	off = true;
    	digitalWrite(YELLOW_LED, 0);
    }
}

void buttonFxn1()
{
    static int state = 0;

    state = (state + 1) & 0x7;

    if (state & 0x1) {
    	digitalWrite(RED_LED, 1);
    }
    else {
    	digitalWrite(RED_LED, 0);
    }

    if (state & 0x2) {
    	digitalWrite(GREEN_LED, 1);
    }
    else {
    	digitalWrite(GREEN_LED, 0);
    }

    if (state & 0x4) {
    	digitalWrite(BLUE_LED, 1);
    }
    else {
    	digitalWrite(BLUE_LED, 0);
    }
}

void setup7()
{
    pinMode(PUSH1, INPUT_PULLUP);
    pinMode(PUSH2, INPUT_PULLUP);

    attachInterrupt(PUSH1, buttonFxn0, RISING);
    attachInterrupt(PUSH2, buttonFxn1, RISING);
}

void loop7()
{
    delay(2500);
//    digitalWrite(RED_LED, 0);
    digitalWrite(RED_LED, 1);
//    digitalWrite(RED_LED, 0);
    delay(2500);
    digitalWrite(RED_LED, 0);
//    delay(2499);
//    digitalWrite(RED_LED, 1);
//    digitalWrite(RED_LED, 0);
//    delay(2501);
//    digitalWrite(RED_LED, 1);
//    setDelayResolution(250);
}
