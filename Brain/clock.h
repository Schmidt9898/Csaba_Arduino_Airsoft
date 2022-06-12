#ifndef S_CLOCK
#define S_CLOCK

#include <Wire.h> 

#define five_day_in_sec 432000

uint32_t detention_end=0;

extern int time_morning;
extern int time_night;



String time_to_string(uint32_t sec)
{
	uint32_t min =sec/60; 
	uint32_t hour =min/60;	
	return String(hour)+":"+String(min%60)+":"+String(sec%60);
}

#ifdef DEMO
#else
#include  <virtuabotixRTC.h>  //Library used
virtuabotixRTC myRTC(10, 11, 12); //If you change the wiring change the pins here also
#endif

struct	Clock
{
	//uint32_t time = 0; //in sec
	uint32_t sec=0;
	/*Clock(int data,int reset)
	{
		
	}*/
	
	
	
	void refresh(){
		#ifdef DEMO
		sec = 28790+millis()/1000;
		#else
 		myRTC.updateTime();

		
  		Serial.print(myRTC.year);             //You can switch between day and month if you're using American system
  		Serial.print("/");
  		Serial.print(myRTC.month);
  		Serial.print("/");
  		Serial.print(myRTC.dayofmonth);
  		Serial.print(" ");
  		Serial.print(myRTC.hours);
  		Serial.print(":");
  		Serial.print(myRTC.minutes);
  		Serial.print(":");
  		Serial.println(myRTC.seconds);

/*
time_t makeTime(const tmElements_t &tm){   
// assemble time elements into time_t 
// note year argument is offset from 1970 (see macros in time.h to convert to other formats)
// previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9
  
  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds= tm.Year*(SECS_PER_DAY * 365);
  for (i = 0; i < tm.Year; i++) {
    if (LEAP_YEAR(i)) {
      seconds += SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < tm.Month; i++) {
    if ( (i == 2) && LEAP_YEAR(tm.Year)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
    }
  }
  seconds+= (tm.Day-1) * SECS_PER_DAY;
  seconds+= tm.Hour * SECS_PER_HOUR;
  seconds+= tm.Minute * SECS_PER_MIN;
  seconds+= tm.Second;
  return (time_t)seconds; 
}
*/






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

		pinMode(good_pin,INPUT);
		pinMode(bad_pin,INPUT);
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
uint32_t stop_detection=0;

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