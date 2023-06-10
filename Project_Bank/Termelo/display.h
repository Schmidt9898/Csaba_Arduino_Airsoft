#ifndef S_DISPLAY
#define S_DISPLAY


#include <Wire.h> 

#include "log.h"

#define BUZZER 8

#define I2C_BUMM

#ifdef I2C_BUMM
#include <LiquidCrystal_I2C.h>
	LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#else
	//CSABA
	
	#include <LiquidCrystal.h>
	LiquidCrystal lcd(34, 35, 36, 37, 38, 39);
	
#endif

/*
#include "LedControl.h"
LedControl lc=LedControl(14,16,15,1);
*/


void lcd_init()
{
	lcd.init(); 
	lcd.backlight();
	lcd.setCursor(0,0);
	lcd.print("");
	lcd.setCursor(0,1);
	lcd.print("");
	log("LCD initilaized");
}


void lcd_write(const char* line0_,const char* line1_)
{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print(line0_);
		lcd.setCursor(0,1);
		lcd.print(line1_);	
}

#define on true
#define off false


struct LED {
	uint8_t pin;
	bool isOn=false;
	void turn(bool b){
		isOn=b;
		digitalWrite(pin,b);
	};
	void togle()
	{
		if(isOn)
			turn(off);
		else
			turn(on);
	}
	LED(int pin_){
		pin=pin_;
		pinMode(pin, OUTPUT);
	};
	LED(){pin=-1;};

};


void beep(uint8_t i){
	if(i==0)
	tone(BUZZER,2000,100);
	else if (i==1)
	tone(BUZZER,1000,200);
	else if(i==2)
	tone(BUZZER,808,100);	
	else if(i==3)
	tone(BUZZER,404,500);
}





#endif
