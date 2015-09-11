#include <Energia.h>

void buttonFxnX()
{
    static bool off = true;

    if (off) {
    	off = false;
//    	digitalWrite(YELLOW_LED, 1);
    }
    else {
    	off = true;
//    	digitalWrite(YELLOW_LED, 0);
    }
}

void buttonFxn0()
{
    SPI.transfer(2,0);
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

    attachInterrupt(PUSH1, buttonFxn0, RISING);
    attachInterrupt(PUSH2, buttonFxn1, RISING);
}

void loop7()
{
//    pinMode(YELLOW_LED, INPUT_PULLUP);
    digitalWrite(YELLOW_LED, 1);
    delay(5000);
//    pinMode(YELLOW_LED, INPUT_PULLUP);
    digitalWrite(YELLOW_LED, 0);
    delay(5000);
}
