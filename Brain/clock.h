#ifndef S_CLOCK
#define S_CLOCK

struct  Clock
{
	//long time = 0; //in sec
	int get_time(){
		//some offset
		int min =(millis()+72000000)/1000/60; 
		return min/60*100+min%60;
	};
};



#endif