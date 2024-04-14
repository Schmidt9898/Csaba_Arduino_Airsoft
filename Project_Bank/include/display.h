#ifndef S_DISPLAY
#define S_DISPLAY

#include <Wire.h>
#include "pitches.h"


#include "log.h"

#ifndef BUZZER
#define BUZZER 8
#endif


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

#define AUDIO_KEYPRESS_1 0
#define AUDIO_KEYPRESS_2 1
#define AUDIO_KEYPRESS_3 2
#define AUDIO_KEYPRESS_4 10
#define AUDIO_KEYPRESS AUDIO_KEYPRESS_1
#define AUDIO_ERROR 3
#define AUDIO_ERROR2 4
#define AUDIO_CONNECTED 5
#define AUDIO_DISCONNECTED 6
#define AUDIO_WELCOME_MELODY 7
#define AUDIO_WARNING 8
#define AUDIO_CHECK 9


void beep(uint8_t i)
{
	if (i == 0)
		tone(BUZZER, 2000, 100);
	else if (i == 1)
		tone(BUZZER, 1000, 200);
	else if (i == 10)
		tone(BUZZER, 300, 200);
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
	else if (i == 7)
	{

		//Abc cba cde
		tone(BUZZER, NOTE_A5, 200); delay(200);
		tone(BUZZER, NOTE_B5, 200); delay(200);
		tone(BUZZER, NOTE_C5, 200); delay(400);

		tone(BUZZER, NOTE_C5, 200); delay(200);
		tone(BUZZER, NOTE_B5, 200); delay(200);
		tone(BUZZER, NOTE_A5, 200); delay(400);

		tone(BUZZER, NOTE_C5, 200); delay(200);
		tone(BUZZER, NOTE_D5, 200); delay(200);
		tone(BUZZER, NOTE_E5, 200); delay(400);
	}
	else if (i == 8){
		for(int i = 0;i<4;i++)
		{
		tone(BUZZER, NOTE_A4, 200);delay(200);
		tone(BUZZER, NOTE_AS4, 200);delay(200);
		}
	}
	else if (i == 9){
		tone(BUZZER, 808, 200);delay(200);
		tone(BUZZER, 1000, 200);
	}
}

#endif
