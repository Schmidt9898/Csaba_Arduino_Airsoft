#ifndef S_LOGGER
#define S_LOGGER


#define USESERIAL

#ifdef USESERIAL
#define logn(str) Serial.println(str)
#define log(str) Serial.print(str)
#define loghex(str) Serial.print(str,HEX)
#else
#define logn(str) 
#define log(str) 
#define loghex(str)
#endif
/*
void _log(const char* a){
	Serial.println(a);
};*/

#endif