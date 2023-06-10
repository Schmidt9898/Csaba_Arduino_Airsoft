#ifndef SD_CARD_LOGGER
#define SD_CARD_LOGGER
#include <SPI.h>
#include <SD.h>
#include "log.h"

/**
 * @brief initializing the sd card reader
 * 
 * @return true all good card is ready
 * @return false there was an error card cannot be read
 */
bool init_sdcard(int cs = 4)
{
	if (!SD.begin(4))
	{
		logn("SD Card initialization failed!");
		return false;
		//while (1);
	}
	logn("SD Card initialization done.");
}


bool Log2file(const char* filename,char* buffer)
{
	logn(strlen(buffer));

  File myFile = SD.open(filename, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    log("logging ");
    myFile.write(buffer,strlen(buffer));
    // close the file:
    myFile.close();
    logn("done.");
	return true;
  } else {
    // if the file didn't open, print an error:
    logn("...,error opening file");
	return false;
  }


}


//TODO read


#endif
