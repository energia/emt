#include <Energia.h>

#define BUTTON1 10
#define BUTTON2 6
#define BUZZER  5
#define LED2    9

uint16_t buzzerState = 0;
uint16_t buzzerPeriod = 1;
int16_t buzzerDuration = 2000;
uint16_t ledState = 0;
uint16_t ledBrightness = 0;

void button1()
{
    ledState ^= 1;
    if (ledState == 0) {
	digitalWrite(LED2, 0);
    }
}

void button2()
{
    buzzerState ^= 1;
}

void setup5()
{
    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);

    attachInterrupt(BUTTON1, button1, RISING);
    attachInterrupt(BUTTON2, button2, RISING);
}

void loop5()
{
    if (buzzerState) {
        delay(buzzerPeriod);
        digitalWrite(BUZZER, 1);
        delay(buzzerPeriod);
        digitalWrite(BUZZER, 0);
	if ((buzzerDuration -= 2*buzzerPeriod) <= 0) {
	    buzzerPeriod = (buzzerPeriod + 1) % 5;
	    if (buzzerPeriod == 0) buzzerPeriod = 1;
	    buzzerDuration = 2000; /* two seconds */
	}
    }
    else if (ledState) {
	analogWrite(LED2, ledBrightness);
	ledBrightness = (ledBrightness + 1) % 256;
	delay(4);
    }
}
