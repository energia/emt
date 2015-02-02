#include "Energia.h"

#include <xdc/runtime/System.h>

#include "GLCDI2C.h"
#include "Wire.h"

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
}

void loop1()
{
	lcd.setColor(BLACK);
	lcd.clear();
	lcd.fontType(FONT_LARGE);
	lcd.setColor(WHITE);
	lcd.cursor(5,15);
	lcd.print("Sw0 = ");
//	lcd.fontType(FONT_BIG);
	lcd.setColor(GREEN);
//	lcd.cursor(20,35);
	lcd.print(digitalRead(41));
	lcd.cursor(5,40);
	lcd.setColor(WHITE);
	lcd.print("Sw1 = ");
//	lcd.fontType(FONT_BIG);
	lcd.setColor(GREEN);
//	lcd.cursor(20,35);
	lcd.print(digitalRead(42));
	delay(500);
}
