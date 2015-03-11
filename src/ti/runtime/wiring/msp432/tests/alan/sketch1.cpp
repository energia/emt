#include <Energia.h>

#include <xdc/runtime/System.h>

#include "GLCDI2C.h"
#include <Wire.h>

#define	BLACK           0x0000
#define	GREEN           0x07E0
#define WHITE           0xFFFF

unsigned int maxX = 128;  // LCD x-resolution
unsigned int maxY = 128;  // LCD y-resolution
unsigned char i2cAddress = 0x46;  // LCD module I2C address

GLCDI2C lcd = GLCDI2C(maxX, maxY, i2cAddress);

void setup1()
{
    Wire.begin(0);
    lcd.init();
    delay(100);
    lcd.setColor(BLACK);
    lcd.clear();
    delay(100);
    lcd.fontType(FONT_MEDIUM);
    delay(2000);
}

void loop1()
{
    static int sw1 = 0;
    static int sw2 = 0;
    static int pin30 = 0;

    delay(10);

    lcd.setColor(WHITE);
    lcd.cursor(5,15);
    lcd.print("Sw1 = ");
    lcd.setColor(BLACK);
    lcd.print(sw1);

    delay(10);

    lcd.setColor(WHITE);
    lcd.cursor(5,15);
    lcd.print("Sw1 = ");
    lcd.setColor(GREEN);
    lcd.print(sw1 = digitalRead(PUSH1));

    delay(10);

    lcd.setColor(WHITE);
    lcd.cursor(5,40);
    lcd.print("Sw2 = ");
    lcd.setColor(BLACK);
    lcd.print(sw2);

    delay(10);

    lcd.setColor(WHITE);
    lcd.cursor(5,40);
    lcd.print("Sw2 = ");
    lcd.setColor(GREEN);
    lcd.print(sw2 = digitalRead(PUSH2));

    delay(10);

    lcd.setColor(WHITE);
    lcd.cursor(5,65);
    lcd.print("pin30 = ");
    lcd.setColor(BLACK);
    lcd.print((pin30 * 2.5) / 1023.0);

    delay(10);

    lcd.setColor(WHITE);
    lcd.cursor(5,65);
    lcd.print("pin30 = ");
    lcd.setColor(GREEN);
    pin30 = analogRead(30);
    lcd.print((pin30 * 2.5) / 1023.0);
}
