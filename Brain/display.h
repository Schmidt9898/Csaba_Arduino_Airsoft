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
