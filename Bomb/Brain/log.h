#ifndef S_LOGGER
#define S_LOGGER


#ifdef DEBUG
#define log(str) Serial.println(str)
#else
#define log(str) 
#endif
/*
void _log(const char* a){
	Serial.println(a);
};*/

#endif