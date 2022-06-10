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












#endif