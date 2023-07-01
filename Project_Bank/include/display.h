#ifndef S_DISPLAY
#define S_DISPLAY

#include <Wire.h>
#include "pitches.h"


#include "log.h"

#define BUZZER 8


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd0(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#ifdef DUAL_LCD_SCREEN
// secondary display if needed
LiquidCrystal_I2C lcd1(0x26, 16, 2); // You need to short the A0 pins on your i2c_lcd panel to change the addres to 0x26
#endif

void lcd_init()
{
	lcd0.init();
	lcd0.backlight();
	lcd0.setCursor(0, 0);
	lcd0.print("");
	lcd0.setCursor(0, 1);
	lcd0.print("");
	logn("LCD initilaized");
	#ifdef DUAL_LCD_SCREEN
	lcd1.init();
	lcd1.backlight();
	lcd1.setCursor(0, 0);
	lcd1.print("");
	lcd1.setCursor(0, 1);
	lcd1.print("");
	logn("Secoundary LCD initilaized");
	#endif
}

void lcd_write(const char *line0_, const char *line1_ , LiquidCrystal_I2C &lcd = lcd0)
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(line0_);
	lcd.setCursor(0, 1);
	lcd.print(line1_);
}


void lcd_char(char c, short row, short col, LiquidCrystal_I2C &lcd = lcd0)
{
	lcd.setCursor(col, row);
	lcd.print(c);
	lcd.flush();
}

#define on true
#define off false

struct LED
{
	uint8_t pin;
	bool isOn = false;
	void turn(bool b)
	{
		isOn = b;
		digitalWrite(pin, b);
	};
	void togle()
	{
		if (isOn)
			turn(off);
		else
			turn(on);
	}
	LED(int pin_)
	{
		pin = pin_;
		pinMode(pin, OUTPUT);
	};
	LED() { pin = -1; };
};

void beep(uint8_t i)
{
	if (i == 0)
		tone(BUZZER, 2000, 100);
	else if (i == 1)
		tone(BUZZER, 1000, 200);
	else if (i == 2)
		tone(BUZZER, 808, 100);
	else if (i == 3)
		tone(BUZZER, 404, 500);
	else if (i == 4)
		tone(BUZZER, 808, 1000);
	else if (i == 5)
	{
		tone(BUZZER, NOTE_A5, 200);
		delay(200);
		tone(BUZZER, NOTE_A4, 200);
		delay(200);
		tone(BUZZER, NOTE_C6, 400);
		//delay(400);
	}
		else if (i == 6)
	{
		tone(BUZZER, NOTE_A4, 100);
		delay(200);
		tone(BUZZER, NOTE_A4, 100);
		delay(200);
		tone(BUZZER, NOTE_C6, 200);
		//delay(200);
	}
}

#endif
