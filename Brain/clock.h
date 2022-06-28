#ifndef S_CLOCK
#define S_CLOCK

#include <Wire.h> 

#define three_day_in_sec 259200
//#define three_day_in_sec 3600

uint32_t detention_end=0;

extern const int time_morning;
extern const int time_night;



String time_to_string(uint32_t sec)
{
	char buff[10]="";
	uint32_t min =sec/60; 
	uint32_t hour =min/60;
	min%=60;
	sec%=60;
	sprintf(buff,"%03d:%02d:%02d\0",(int)hour,(int)min,(int)sec);
	return String(buff);
	//return String(hour)+":"+String(min%60)+":"+String(sec%60);
}

#ifdef DEMO
#else
#include  <virtuabotixRTC.h>  //Library used
virtuabotixRTC myRTC(6,5,4);  //If you change the wiring change the pins here also
					//clk data reset
					//6,5,4

//short month[12]={31,28,31,30 ,31 ,30 ,31 ,31 ,30 ,31 ,30 ,31};
uint16_t month[12]={31,59,90,120,151,181,211,242,272,303,333,364};
					
#endif

struct	Clock
{
	//uint32_t time = 0; //in sec
	uint32_t sec=0;
	bool is_disconected=false;
	/*Clock(int data,int reset)
	{
		
	}*/
	
	
	
	void refresh(){
		is_disconected=false;
		#ifdef DEMO
		//is_disconected=true;
		//sec = -1;
		sec = 28800+millis()/1000;
		#else
 		myRTC.updateTime();
/*
	Serial.print("Jelenlegi dátum / Idő: ");
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
  */
  		
//log("--->: "+String(myRTC.month));
//log("......: "+String(month[(myRTC.month-2)]));
		if(myRTC.month==0)
		{
			log("WARNING Clock is disconected.");
			sec=0;
			is_disconected=true;
		}
		if(myRTC.month>1)
		sec = month[(myRTC.month-2)];
		if(myRTC.dayofmonth>1)
		sec+=myRTC.dayofmonth-1;
		
   //log("napok: "+String(sec));

		sec=sec*24;//hours
    //log("orak: "+String(sec));
		sec+=myRTC.hours;
		sec=sec*60 + myRTC.minutes;
		sec=sec*60 + myRTC.seconds;
    //log("just sec: "+String(sec));
    sec+= 1640995200;
    //log("all sec "+String(sec));
		//2022 = 1640995200
		#endif
	};
	String get_time(){
#ifdef DEMO
		uint32_t min =sec/60; 
		uint32_t hour =min/60; 
		return String(hour%24)+":"+String(min%60)+":"+String(sec%60);
#else
		return String(myRTC.hours)+":"+String(myRTC.minutes)+":"+String(myRTC.seconds);
#endif


	};
	bool isDay(){
		#ifdef DEMO
		uint32_t min =sec/60; 
		uint32_t hour =min/60; 
		#else
		//uint32_t min =myRTC.hours; 
		uint32_t hour =myRTC.hours; 
    //log("hour "+String(myRTC.hours));
		#endif
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
	Wire.begin();
	Wire.beginTransmission(MPU);
	Wire.write(0x6B);
	Wire.write(0);
	Wire.endTransmission(true);
	#ifdef DEMO
	//pinMode(2, INPUT_PULLUP);
	#else
	log("gyroscope initialized");
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
		if (AcX - 5000 < ujX and ujX < AcX + 5000 and AcY - 5000 < ujY and ujY < AcY + 5000 )
		{
		val=false;
		}else{
			first=true;
			stop_detection=millis()+6000;
		}
//log("gyro val: "+String(val)+" , "+String(AcX)+" , "+String(AcY)+" , "+String(ujX)+" , "+String(ujY));
		AcX=ujX;
		AcY=ujY;

		return val;
	#endif
};


};











#endif
