#ifndef CARDSERVICE
#define CARDSERVICE

//TODO remove printf-s


#include <SPI.h>
#include <MFRC522.h>

#ifdef ARDUINO_AVR_MEGA2560
#define RST_PIN 5 // Configurable, see typical pin layout above
#define SS_PIN 53 // Configurable, see typical pin layout above
#endif
#ifdef ARDUINO_AVR_NANO
#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above
#endif

//data to be stored

MFRC522::MIFARE_Key Prod_key;
MFRC522::MIFARE_Key factory_key;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
MFRC522::StatusCode status;


//TODO init this with the key
key.keyByte = {0x01,0x02,0x03,0x04,0x05,0x06}; //Secret password
factory_key.keyByte = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; //Factory password


struct Profile
{
String name = "NONAME";
int32_t balance = 0;
uint32_t transaction_count = 0;
/*
bool remove(int32_t amount)
{
	if(balance - amount < 0)//cant afford it
		return false;
	
	balance-=amount;
}
bool add(uint32_t amount)
{

}
*/
}

#define BANKDATABLOCK 4
//#define AUTDATABLOCK 4

//#define PERSON_NAME0 8
//#define PERSON_NAME1 9
//#define PERSON_BALANCE 10

bool is_equal(byte* a,byte*b,byte size)
{
	for(byte i=0;i<size;i++)
	{
		if(a[i] != b[i])
			return false;
	}
	return true;
}

/**
 * @brief Returns the trailer addres from the corresponding addres
 * 
 * @param blockAddr 
 * @return byte trailer addres
 */
byte Get_trailer(byte blockAddr)
{
	return (blockAddr / 4 + 1) * 4 - 1  
}


class CardService
{
	MFRC522::MIFARE_Key* key; //selected key

	int timeout_ms = 2*1000; //time before we signals that card has been removed
	unsigned long endtime = -1;//millis() + timeout*1000;

	byte selected_uid[10] = {0};
	bool is_card_present = false; // signals if a card is in the reader, turns false if card is not there and  timeout expires
	bool is_card_active = false; // signals that the card was there at the last check this is always uptodate, no timeout



	
	bool Get_new_card(); //check for new card, non blocking, returns true if card is in the reader, it also returns true if called again on the same card

	bool Check_card(); //returns true if card is still present

	bool Write_profile_to_card(Profile* profile);
	bool Read_profile_from_card(Profile* profile);

	bool Write_bytes_to_block(byte blockAddr,byte* bytes,byte len);

	bool is_block_factory(byte blockAddr); //returns true if the block is coded with factory key
	bool set_block_key(MFRC522::MIFARE_Key* key); //sets block key

}



///CPP PARTS


bool CardService::Write_profile_to_card(Profile* profile,Uid *uid)
{
	//write secret data
	status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, BANKDATATRAILER, &key, uid);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    // Write data to the block
    //Serial.print(F("Writing data into block ")); Serial.print(blockAddr);

    //dump_byte_array(dataBlock, 16); Serial.println();

    status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(BANKDATABLOCK, dataBlock, 16);

	//TODO create check write macro


    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.println();
}


bool CardService::Get_new_card(){
	
	if (is_card_present)
	{
		//TODO check timeout
		//if()
		{
			return true;
		}
	}

	if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
	{
		// new card is in the reader and ready state
		// get the id
		is_card_present = is_card_active = true;
		memcpy(selected_uid, mfrc522.uid.uidByte, mfrc522.uid.size);
	}
}
bool CardService::Check_card(){
	if (!is_card_present)
	{
		return false;
	}
	if(is_card_active)
	{
		status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Get_trailer(BANKDATABLOCK) , key, &(mfrc522.uid));
		if (status != MFRC522::STATUS_OK)
		{
			Serial.print(F("PCD_Authenticate() failed: "));
			Serial.println(mfrc522.GetStatusCodeName(status));
			is_card_active = false;
			continue;
		}
		else
		{
			Serial.print("Connection is good.");
			Serial.println(mfrc522.GetStatusCodeName(status));
			endtime = millis() + timeout_ms;
		}
	}
	if(!is_card_active) //note to self this is not an else statement because same loop next card check
	{
		if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
		{
			if (is_equal(mfrc522.uid.uidByte, selected_uid, mfrc522.uid.size))
			{
				is_card_active = true;
				endtime = millis() + timeout_ms;
			}
			else
			{
				Serial.println("not equal card id");
			}
		}
	}
	if(is_card_present)
	{
		if(millis()>endtime)
		{
			//timeout
			Serial.println("Timeout");
			is_card_present=false;
		}
	}





}
bool CardService::Read_profile_from_card(Profile* profile){
	//TODO implement
}
bool CardService::Write_bytes_to_block(byte blockAddr,byte* bytes,byte len){
	//TODO implement
}
bool CardService::is_block_factory(byte blockAddr){
	//TODO implement
} 
bool CardService::set_block_key(MFRC522::MIFARE_Key* key){
	//TODO implement
} 



	bool Keep_card(int timeout = 1)//timeout in sec
	{
		int timeout_ms = timeout*1000;
		unsigned long endtime = millis() + timeout*1000;

		byte selected_uid[10] = {0};
		bool is_empty = true;
		bool is_active = false;

		byte blockAddr = 11;
  		byte trailerBlock = 11;


		while(endtime > millis()) //until timeout is not reached, timeout restart when card is detected
		{
			if(!is_active)
			{

			if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
			{
				//new card is in the reader and ready state
				//get the id
				if(is_empty)
				{
					is_empty = false;
					memcpy(selected_uid,mfrc522.uid.uidByte,mfrc522.uid.size);
				}else
				{
					if(is_equal(mfrc522.uid.uidByte,selected_uid,mfrc522.uid.size))
						{
							is_active = true;
						}else
						{
							Serial.println("not equal card id");
						}
				}
			}
			}else
			{
				status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &factorykey, &(mfrc522.uid));
				if (status != MFRC522::STATUS_OK) {
					Serial.print(F("PCD_Authenticate() failed: "));
					Serial.println(mfrc522.GetStatusCodeName(status));
					is_active = false;
					continue;
				}else
				{
					Serial.print("Connected ");
					Serial.println(mfrc522.GetStatusCodeName(status));
					endtime = millis() + timeout*1000;
				}
			}


		}
			Serial.println("Card keep timeout");
			return false;



	}


//2x16 for name

//void Convert2CardData(Profile* profile)












#endif