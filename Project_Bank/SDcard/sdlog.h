#ifndef SD_CARD_LOGGER
#define SD_CARD_LOGGER
#include <SPI.h>
#include <SD.h>

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
		Serial.println("SD Card initialization failed!");
		return false;
		//while (1);
	}
	Serial.println("SD Card initialization done.");
}


bool Log2file(String filename,char* buffer,size_t len)
{
	Serial.println(len);

  File myFile = SD.open(filename, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("logging ");
    myFile.write(buffer,len);
    // close the file:
    myFile.close();
    Serial.println("done.");
	return true;
  } else {
    // if the file didn't open, print an error:
    Serial.println("...,error opening file");
	return false;
  }


}


//TODO read


#endif
