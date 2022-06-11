#ifndef S_DISPLAY
#define S_DISPLAY


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

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
	//CSABA
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
#define m_pin_accept 2
#define m_passed_game 3
#define m_failed_game 4
#define m_all_game_passed 5
#define m_defusing_complete 6
#define m_music 7
#define m_move 8
#define m_game_start 9


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







#endif
