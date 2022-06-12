

#include <EEPROM.h>





uint32_t detention_time = 1800; //in second
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
	 Serial.print("saving");
	{
		time_of_detonation = 1655142221; // ide írd az időt
		Serial.print(String(time_of_detonation,2));
		detention_end=0;
		progress=0b00000000;
		save_detention_end_time();
		save_detonation_time();
		save_progress();
		//beep(1);
		delay(100000);
	}
	Serial.print("done");
	
}
void loop() { 

}
