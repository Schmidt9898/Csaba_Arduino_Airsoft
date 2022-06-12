#ifndef S_DISPLAY
#define S_DISPLAY


#include <Wire.h> 




#ifdef DEMO
#include <LiquidCrystal_I2C.h>
	LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#else
	//CSABA
	#include "LedControl.h"
	#include <LiquidCrystal.h>
	LiquidCrystal lcd(34, 35, 36, 37, 38, 39);
	LedControl lc=LedControl(14,16,15,1);
#endif





void lcd_init()
{
	#ifdef DEMO
	lcd.init(); 
	lcd.backlight();
	//lcd.setCursor(0,0);
	//lcd.print("elso sor");
	//lcd.setCursor(0,1);
	//lcd.print("masodik sor");
	#else
	#endif
}


void lcd_write(String line0,String line1="")
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(line0);
	lcd.setCursor(0,1);
	lcd.print(line1);
}

void led_segment_init()
{
	#ifdef DEMO
	#else
	/*
	 The MAX72XX is in power-saving mode on startup,
	 we have to do a wakeup call
	 */
	lc.shutdown(0,false);
	/* Set the brightness to a medium values */
	lc.setIntensity(0,8);
	/* and clear the display */
	lc.clearDisplay(0);
	#endif
}

void led_segment(uint32_t sec)
{
	uint32_t min =sec/60; 
	uint32_t hour =min/60; //3 digit
	min%=60;	//2 digit
	sec%=60;	//2 digit	

	#ifdef DEMO
	//log("T-"+String(hour)+":"+String(min)+":"+String(sec));
	#else
	lc.clearDisplay(0);

	lc.setDigit(0,7, hour/100 % 10	,false);//balról
    lc.setDigit(0,6, hour/10 % 10	,false);
    lc.setDigit(0,5, hour%10 		,false);

    //lc.setDigit(0,4,4,false);
    //minute
	lc.setDigit(0,3,min/10%10,false);
    lc.setDigit(0,2,min%10,true);
    //sec
	lc.setDigit(0,1,sec/10%10,false);
    lc.setDigit(0,0,sec%10,false);
	#endif 
}




struct LED {
	int pin;
	bool isOn=false;
	void turn(bool b){
		isOn=b;
		digitalWrite(pin,b);
	};
	LED(int pin_){
		pin=pin_;
		pinMode(pin, OUTPUT);
	};
	LED(){pin=-1;};

};



//üdvözlés,
//kártya elfogadva
//kód elfogadva indulnak a játékok
//sikeres megoldás
//sikertelen megoldás
//feladatok kész kérem a végső kódot
//végső kód kész
//zene
//mozgas
//most lehet játszani

#define m_welcome 0
#define m_card_accept 1
#define m_pass_good 2
#define m_passed_attempt 3
#define m_failed_attempt 4
#define m_all_game_passed 5
#define m_defusing_complete 6
#define m_music 7
#define m_move 8
#define m_cant_play 9


int music_start_pin;
void init_music_player(int start_pin,int n){
	music_start_pin=start_pin;
	for (int i=start_pin;i<start_pin+n;i++)
	{
		pinMode(i, OUTPUT);
		digitalWrite(i,LOW);
	}
}
void play_music(int i){
	i+=music_start_pin;
	digitalWrite(i,HIGH);
	delay(100);
	digitalWrite(i,LOW);
}
//may add stop pin function

void beep(int i){
	if(i==0)
	tone(BUZZER,2000,100);
	else if (i==1)
	tone(BUZZER,1000,200);
	else if(i=2)
	tone(BUZZER,808,100);
}





#endif
