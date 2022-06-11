#ifndef S_CLOCK
#define S_CLOCK

#include <Wire.h> 

#define five_day_in_sec 432000

long detention_end;

extern int time_morning;
extern int time_night;



String time_to_string(uint32_t sec)
{
	uint32_t min =sec/60; 
	uint32_t hour =min/60;	
	return String(hour)+":"+String(min%60)+":"+String(sec%60);
}

struct	Clock
{
	//long time = 0; //in sec
	uint32_t sec=0;
	Clock(int data,int reset)
	{
		//TODO
	}
	
	
	
	void refresh(){
		#ifdef DEMO
		sec = 28790+millis()/1000;
		#else
		sec=0;
		#endif
	};
	String get_time(){
		//some offset
		uint32_t min =sec/60; 
		uint32_t hour =min/60; 
		//uint32_t day =min/60; 
		//uint32_t	=min/60; 
		return String(hour%24)+":"+String(min%60)+":"+String(sec%60);
		//return min/60*1000+min%60;
	};
	bool isDay(){
		uint32_t min =sec/60; 
		uint32_t hour =min/60; 
		return hour>=time_morning && hour<time_night;
	}

};
//TODO
//time till date
//time left
//save time


enum Mini_game_state{
	Unknown=0, Good=1, Bad=2,Already_done=3
};


struct Mini_game{
	int act;		//pin dont change
	int good_pin;	//pin dont change
	int bad_pin;	//pin dont change
	bool solved;		//true if it is solved
	//bool isactivated;		//true if is activated

	void init(){
		pinMode(act,OUTPUT);
		digitalWrite(act,LOW);

		pinMode(isReadenable,INPUT);
		pinMode(signal,INPUT);
	}


	Mini_game_state get_state(){
		if(solved)
			return Mini_game_state::Already_done;
		
		if(digitalRead(good_pin)){
			solved=true;
			deactivate();
			return Mini_game_state::Good;
		}else if(digitalRead(bad_pin))
		{
			return Mini_game_state::Bad;
		}else
		{
			return Mini_game_state::Unknown;
		}
	};
	void activate(){
		if(!solved)
			digitalWrite(act,HIGH);
	}
	void deactivate(){
		digitalWrite(act,LOW);
	}


};


struct Gyoscope{

const int MPU = 0x68;
//int Tmp, AcX, AcY, AcZ, GyX, GyY, GyZ, ujX, ujY;
int AcX, AcY, ujX, ujY;
bool first=true;
long stop_detection=0;

void init(){
	//Wire.begin();
	Wire.beginTransmission(MPU);
	Wire.write(0x6B);
	Wire.write(0);
	Wire.endTransmission(true);
	#ifdef DEMO
	//pinMode(2, INPUT_PULLUP);
	#else
	#endif

}

bool did_gyroscope_move(){
	if(millis()<stop_detection)
	{
		//first=true;
		return false;
	}



	#ifdef DEMO

	/*if(!digitalRead(2))
	{
		stop_detection=millis()+6000;
		return true;
	}else*/
	{
	return false;
	}



	#else
		Wire.beginTransmission(MPU);
		Wire.write(0x3B);
		Wire.endTransmission(false);
		Wire.requestFrom(MPU, 12, true);
		ujX = Wire.read() << 8 | Wire.read();
		ujY = Wire.read() << 8 | Wire.read();
		Wire.read() << 8 | Wire.read();
		Wire.read() << 8 | Wire.read();
		Wire.read() << 8 | Wire.read();
		Wire.read() << 8 | Wire.read();

		if(first){
			AcX=ujX;
			AcY=ujY;
			first=false;
			return false;
		}
		bool val=true;

		if (AcX - 1000 < ujX and ujX < AcX + 1000 and AcY - 1000 < ujY and ujY < AcY + 1000 )
		{
		val=false;
		}else{
			first=true;
			stop_detection=millis()+6000;
		}
		AcX=ujX;
		AcY=ujY;

		return val;
	#endif
};


};











#endif