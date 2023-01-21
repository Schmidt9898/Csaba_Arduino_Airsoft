

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}


bool card_pesent = false;
bool read_serial = false;

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.

//Serial.println("Present:"+String(mfrc522.PICC_IsNewCardPresent()));
//if(mfrc522.PICC_IsNewCardPresent())
{
}
Serial.println("Serial:"+String(mfrc522.PICC_ReadCardSerial()));


	
/*

	card_pesent ||= mfrc522.PICC_IsNewCardPresent();
	if (card_pesent && mfrc522.PICC_ReadCardSerial()) {

		Serial.println("New Card");

		while(true)
		{
			delay(100);
			if(mfrc522.PICC_IsNewCardPresent())
			{
				int val = int(mfrc522.PICC_ReadCardSerial()) + int(mfrc522.PICC_ReadCardSerial());
				 Serial.println(val);
				if(val == 1){
					//Good
				}else
				{
					break;
				}
			}
		}
		Serial.println("Removed");

	}
*/
	/*
	// Select one of the cards
	if (mfrc522.PICC_ReadCardSerial()) {
		Serial.println("1");
	}else
	{
		Serial.println("0");
	}
	*/
	delay(500);
}
