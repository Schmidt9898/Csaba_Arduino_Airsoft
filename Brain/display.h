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


String line0="",line1="";
bool lcd_isdelay=false; // did we write it back, so we dont write it rapidly 
uint32_t write_back_time=0;
void lcd_init()
{
	line0="";
	line1="";
	#ifdef DEMO
	lcd.init(); 
	lcd.backlight();
	//lcd.setCursor(0,0);
	//lcd.print("elso sor");
	//lcd.setCursor(0,1);
	//lcd.print("masodik sor");
	#else
	lcd.begin(16, 2);
	lcd.clear();
	#endif
	log("LCD initilaized");
}


void lcd_write(String line0_,String line1_,uint32_t sec_=0)
{
	if(sec_ == 0){
		line0=line0_;
		line1=line1_;
	}else
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print(line0_);
		lcd.setCursor(0,1);
		lcd.print(line1_);	
		write_back_time=millis()+sec_*1000;
		lcd_isdelay=true;
	}
	if(!lcd_isdelay)
	{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print(line0);
		lcd.setCursor(0,1);
		lcd.print(line1);		
	}
}
void lcd_update()
{
	if(lcd_isdelay)
	{
		if(write_back_time<=millis())
		{
			lcd_isdelay=false;
				lcd.clear();
				lcd.setCursor(0,0);
				lcd.print(line0);
				lcd.setCursor(0,1);
				lcd.print(line1);
		}
	}
}

/*

void lcd_write(String line,int n=0)
{
	//lcd.clear();
	lcd.setCursor(0,n);
	lcd.print(line);
}

*/

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

	log("led segment display initialized");
	#endif
}

void led_segment(uint32_t sec)
{
	uint32_t min =sec/60; 
	uint32_t hour =min/60; //3 digit
	min%=60;	//2 digit
	sec%=60;	//2 digit	

	#ifdef DEMO
	#else
	//log("T-"+String(hour)+":"+String(min)+":"+String(sec));
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
//A0 = 54
#define m_welcome A0
#define m_card_accept A1
#define m_pass_good A2
#define m_passed_attempt A3
#define m_failed_attempt A4
#define m_all_game_passed A5
#define m_defusing_complete A6
#define m_music A7
#define m_move A8
//#define m_cant_play A9


void init_music_player(){
	for(int i = PIN_A0;i<PIN_A0+9;i++)
	{
		pinMode(i, OUTPUT);
		digitalWrite(i,HIGH);
	}
	/*
	pinMode(A0, OUTPUT);
	digitalWrite(A0,HIGH);
	pinMode(A1, OUTPUT);
	digitalWrite(A1,HIGH);
	pinMode(A2, OUTPUT);
	digitalWrite(A2,HIGH);
	pinMode(A3, OUTPUT);
	digitalWrite(A3,HIGH);
	pinMode(A4, OUTPUT);
	digitalWrite(A4,HIGH);
	pinMode(A5, OUTPUT);
	digitalWrite(A5,HIGH);
	pinMode(A6, OUTPUT);
	digitalWrite(A6,HIGH);
	pinMode(A7, OUTPUT);
	digitalWrite(A7,HIGH);
	pinMode(A8, OUTPUT);
	digitalWrite(A8,HIGH);
	*/
	log("music player initialized");
}
void play_music(int i){
	log("play audio: "+String(i-54));
	digitalWrite(i,LOW);
	delay(500);
	digitalWrite(i,HIGH);
}
//may add stop pin function

void beep(int i){
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
