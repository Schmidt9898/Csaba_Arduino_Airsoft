

#include <EEPROM.h>


#define BUZZER 49 // piezo buzzer


uint32_t detention_end = 0; //in second
uint32_t time_of_detonation;
byte progress=0;




//i byte
void save_progress(){

EEPROM.write(0, progress);

}
//4 byte
void save_detonation_time()
{
EEPROM.write(1, time_of_detonation);
EEPROM.write(2, time_of_detonation >> 8);
EEPROM.write(3, time_of_detonation >> 16);
EEPROM.write(4, time_of_detonation >> 24);
}
//4byte
void save_detention_end_time()
{
EEPROM.write(5, detention_end);
EEPROM.write(6, detention_end >> 8);
EEPROM.write(7, detention_end >> 16);
EEPROM.write(8, detention_end >> 24);

}


void setup() {
	 Serial.begin(9600);
	pinMode(BUZZER, OUTPUT);
	noTone(BUZZER);
	 Serial.print("Saving data to EEPROM");
	{
		time_of_detonation = 1655142221; // ide írd az időt
		Serial.print("Detonation time:\nIN BINARY: "+String(time_of_detonation,2));
		Serial.print("UNIXEPOCH: "+String(time_of_detonation));
		detention_end=0;
		//first time bit | 1 empty bit | pincode bit | keycard bit | 4,3,2,1 game bit  
		progress=0b00110000;
		Serial.print("PROGRESS: "+String(progress,2));
		save_detention_end_time();
		tone(BUZZER,500,100);
		delay(500);
		save_detonation_time();
		tone(BUZZER,1000,100);
		delay(500);
		save_progress();
		tone(BUZZER,2000,100);
		delay(500);
	}
	Serial.print("Programing is done!");
	
}
void loop() { 
	delay(1000);
}
