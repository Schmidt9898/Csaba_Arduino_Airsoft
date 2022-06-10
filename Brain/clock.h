#ifndef S_CLOCK
#define S_CLOCK

struct  Clock
{
	//long time = 0; //in sec
	unsigned int sec=0;
	void refresh(){
		#ifdef DEMO
		sec =(millis()+72000000)/1000;
		#else
		sec=0;
		#endif
	};
	int get_time(){
		//some offset
		int min =sec/60; 
		return min/60*100+min%60;
	};
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
	Unknown=0, Good=1, Bad=5
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
			return Mini_game_state::Good;
		
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