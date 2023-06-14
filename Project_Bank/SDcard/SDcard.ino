#include "sdlog.h"



const int chipSelect = 4;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

init_sdcard(4);

String filename = "log.txt";

char buff[32] = "logmetofile   hahaha\n"; 

Log2file(filename,buff,strlen(buff));
Log2file(filename,buff,strlen(buff));
Log2file(filename,buff,strlen(buff));
Log2file(filename,buff,strlen(buff));

}

void loop(void) {
}