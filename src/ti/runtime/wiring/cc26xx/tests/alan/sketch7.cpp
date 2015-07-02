#include <Energia.h>

void buttonFxn0()
{
    static bool off = true;

    if (off) {
    	off = false;
    	digitalWrite(GREEN_LED, 1);
    }
    else {
    	off = true;
    	digitalWrite(GREEN_LED, 0);
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
//    	digitalWrite(GREEN_LED, 1);
    }
    else {
//    	digitalWrite(GREEN_LED, 0);
    }

    if (state & 0x4) {
//    	digitalWrite(YELLOW_LED, 1);
    }
    else {
//    	digitalWrite(YELLOW_LED, 0);
    }
}

void setup7()
{
    pinMode(PUSH1, INPUT_PULLUP);
    pinMode(PUSH2, INPUT_PULLUP);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    attachInterrupt(PUSH1, buttonFxn0, RISING);
    attachInterrupt(PUSH2, buttonFxn1, RISING);
}

void loop7()
{
//    digitalWrite(RED_LED, 1);
//    digitalWrite(GREEN_LED, 0);
    delay(1000);
//    digitalWrite(RED_LED, 0);
//    digitalWrite(GREEN_LED, 1);
    delay(1000);
}
