#include "Energia.h"

void buttonFxn0()
{
	static bool off = true;

    if (off) {
    	off = false;
    	digitalWrite(46, 1);
    }
    else {
    	off = true;
    	digitalWrite(46, 0);
    }
}

void buttonFxn1()
{
	static int state = 0;

	state = (state + 1) & 0x7;

	if (state & 0x1) {
    	digitalWrite(43, 1);
	}
	else {
    	digitalWrite(43, 0);
	}

	if (state & 0x2) {
    	digitalWrite(44, 1);
	}
	else {
    	digitalWrite(44, 0);
	}

	if (state & 0x4) {
    	digitalWrite(45, 1);
	}
	else {
    	digitalWrite(45, 0);
	}
}

void setup7()
{
    pinMode(41, INPUT_PULLUP);
    pinMode(42, INPUT_PULLUP);

    attachInterrupt(41, buttonFxn0, RISING);
    attachInterrupt(42, buttonFxn1, RISING);
}

void loop7()
{
	delay(1000);
}
