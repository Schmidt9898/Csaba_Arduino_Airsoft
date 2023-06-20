#ifndef S_LOGGER
#define S_LOGGER


#define USESERIAL

#ifdef USESERIAL
#define init_log Serial.begin(9600)
#define logn(str) Serial.println(str)
#define log(str) Serial.print(str)
#define loghex(str) Serial.print(str,HEX)
#else
#define init_log
#define logn(str) 
#define log(str) 
#define loghex(str)
#endif

/*
void printHex(unsigned int numberToPrint)
{
  if (numberToPrint >= 16)
    printHex(numberToPrint / 16);
  Serial.print("0123456789ABCDEF"[numberToPrint % 16]);
}
*/


#endif
