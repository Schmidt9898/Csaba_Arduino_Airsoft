#ifndef S_CLOCK
#define S_CLOCK


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

struct  Clock
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
		//uint32_t  =min/60; 
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


//init
//get state
void init_gyroscope(){

	#ifdef DEMO
	#else
	#endif


};

bool did_gyroscope_move(){
	#ifdef DEMO
	return false;
	#else
	#endif
};

enum Mini_game_state{
	Unknown=0, Good=1, Bad=2,Already_done=3
};


struct Mini_game{
	int act;		//pin dont change
	int isReadenable;	//pin dont change
	int signal;			//pin dont change
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
		
		if(digitalRead(isReadenable)){
			if(digitalRead(signal)){
				solved=true;
				deactivate();
				return Mini_game_state::Good;
			}else
				return Mini_game_state::Bad;
		}else{
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











#endif