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

    switch(state) {
	case 0: 
	case 4: 
	    analogWrite(RED_LED, 0);
	    break;
	case 1:
	case 5: 
	    analogWrite(RED_LED, 10);
	    break;
	case 2:
	case 6: 
	    analogWrite(RED_LED, 100);
	    break;
	case 3:
	case 7: 
//	    analogWrite(RED_LED, 255);
	    digitalWrite(RED_LED, 1);
	    break;
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

//    SPI.begin();
//    SPI.transfer(100);
}

void loop7()
{
//    digitalWrite(RED_LED, 1);
//    digitalWrite(GREEN_LED, 0);
    delay(1000);
//    digitalWrite(RED_LED, 0);
//    digitalWrite(GREEN_LED, 1);
    delay(1000);
//    analogWrite(RED_LED, 100);
//    analogWrite(GREEN_LED, 100);
//    analogRead(18);
//    analogRead(20);
//    digitalWrite(20,0);
}
