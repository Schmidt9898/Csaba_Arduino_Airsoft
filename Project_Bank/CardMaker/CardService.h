#ifndef CARDSERVICE
#define CARDSERVICE

// TODO remove printf-s

#define DEBUG

#ifdef DEBUG
#define Log(str) Serial.println(str)
#elif
#define Log(str)
#endif

void log()
{
}

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

// data to be stored

MFRC522::MIFARE_Key Prod_key{{0x00, 0x01, 0x03, 0x05, 0x07, 0x09}};
MFRC522::MIFARE_Key factory_key{{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
MFRC522::StatusCode status;

struct Profile
{
	char name[16] = "NONAMENO\0"; // max 16 or ends with \0 terminator
	int32_t balance = 0; //4 byte 
	uint32_t transaction_count = 0;// 4 byte
	uint32_t uid = 0; // 4 byte from the uid the card
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
	void print(){
		Serial.println("Profile data:");
		Serial.print("name: ");Serial.println(name);
		Serial.print("balance = ");Serial.println(balance);
		Serial.print("transaction count = ");Serial.println(transaction_count);
		Serial.print("uid ");Serial.println(uid);
	};
};

// This is sector 1 DO NOT USE SECTOR 0 !!!!!
#define BANKDATABLOCK 4 
#define NAMEDATABLOCK 5

/**
 * @brief Print mfrckey to serial
 *
 * @param key
 */
void printkey(MFRC522::MIFARE_Key &key)
{
	for (byte i = 0; i < 6; i++)
	{
		Serial.print(key.keyByte[i], HEX);
		Serial.println(" ");
	}
}

bool is_equal(byte *a, byte *b, byte size)
{
	for (byte i = 0; i < size; i++)
	{
		if (a[i] != b[i])
			return false;
	}
	return true;
}

/**
 * @brief Print buffer to Serial, use for debug
 *
 * @param buffer
 * @param bufferSize
 */
void dump_byte_array(byte *buffer, byte bufferSize)
{
	for (byte i = 0; i < bufferSize; i++)
	{
		Serial.print(buffer[i] < 0x10 ? " 0" : " ");
		Serial.print(buffer[i], HEX);
	}
	Serial.println("");

}

/**
 * @brief Returns the trailer addres from the corresponding addres
 *
 * @param blockAddr
 * @return byte trailer addres
 */
byte Get_trailer(byte blockAddr)
{
	return (blockAddr / 4 + 1) * 4 - 1;
}

class CardService
{
  public:
	MFRC522::MIFARE_Key *key; // selected key

	size_t timeout_ms = 500;	// time before we signals that card has been removed
	unsigned long endtime = -1; // millis() + timeout*1000;

	byte selected_uid[10] = {0};
	bool is_card_present = false; // signals if a card is in the reader, turns false if card is not there and  timeout expires
	bool is_card_active = false;  // signals that the card was there at the last check this is always uptodate, no timeout

	byte buffer[18] = {1};

public:
	CardService()
	{
		key = &Prod_key;
	}

	void init()
	{
		SPI.begin();					   // Init SPI bus
		mfrc522.PCD_Init();				   // Init MFRC522 card
		delay(4);						   // Optional delay. Some board do need more time after init to be ready, see Readme
		mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
		check_module_connection();
		Serial.println("Constructor 112");
	}

	bool check_module_connection()
	{
		byte v = mfrc522.PCD_ReadRegister(MFRC522::PCD_Register::VersionReg);
		// When 0x00 or 0xFF is returned, communication probably failed
		if ((v == 0x00) || (v == 0xFF))
		{
			Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
			return false;
		}
		return true;
	}

	bool Get_new_card(); // check for new card, non blocking, returns true if card is in the reader, it also returns true if called again on the same card

	bool Check_card(); // returns true if card is still present

	bool Write_profile_to_card(Profile *profile,bool override_name = false);
	bool Read_profile_from_card(Profile *profile);

	bool Write_bytes_to_block(byte blockAddr, byte *bytes, byte len);
	bool Read_bytes_from_block(byte blockAddr, byte *bytes, byte len);
	bool Authenticate_block(byte blockAddr);
	bool Authenticate_block(byte blockAddr,MFRC522::MIFARE_Key* key_);

	void MakeCardProd();


	bool is_block_factory(byte blockAddr);		  // returns true if the block is coded with factory key
	bool set_block_key(MFRC522::MIFARE_Key *key); // sets block key


};

/// CPP PARTS

bool CardService::Write_profile_to_card(Profile *profile,bool override_name)
{
	byte blockAddr = BANKDATABLOCK;
	// TODO dump data into buffer
	// buffer =
	// authenticate block
	if (!Authenticate_block(blockAddr))
	{
		return false;
	}
	// Write data to the block
	memcpy(buffer,&profile->balance, 4);
	memcpy(&buffer[4],&profile->transaction_count, 4);
	memcpy(&buffer[8],&profile->uid, 4);

	if(!Write_bytes_to_block(blockAddr, buffer, 16))
	{
		return false;
	}

	if (override_name)
	{
		memcpy(buffer,&profile->name, 16);
		blockAddr = NAMEDATABLOCK;
		if(!Write_bytes_to_block(blockAddr, buffer, 16))
			{
				return false;
			}
	}
	
	return true;




}

bool CardService::Get_new_card()
{

	if (is_card_present) // we dont need new card
	{
		{
			// Serial.println("175 returns false");
			return false;
		}
	}
	//Serial.println("Cheking for new card...");
	if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
	{
		// new card is in the reader and ready state
		// get the id
		is_card_present = is_card_active = true;
		memcpy(selected_uid, mfrc522.uid.uidByte, mfrc522.uid.size);
	//	Serial.println("new card selected");
		return true;
	}
	return false;
}

//size_t aut_counter = 0;
bool CardService::Check_card()
{
	if (!is_card_present)
	{
		return false;
	}
	if (is_card_active)
	{
		//aut_counter++;
		status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Get_trailer(0), &factory_key, &(mfrc522.uid));
		if (status != MFRC522::STATUS_OK)
		{
			Serial.print(F("PCD_Authenticate() failed: "));
			Serial.println(mfrc522.GetStatusCodeName(status));
			is_card_active = false;
			// return false;
		}
		else
		{
			//Serial.print("Connection is good.");
			//Serial.println(mfrc522.GetStatusCodeName(status));
			endtime = millis() + timeout_ms;
		}
	}
	if (!is_card_active) // note to self this is not an else statement because same loop next card check
	{
		if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
		{
			Serial.println("New potential card");
			if (is_equal(mfrc522.uid.uidByte, selected_uid, mfrc522.uid.size))
			{
				Serial.println("Same card");
				is_card_active = true;
				endtime = millis() + timeout_ms;
			}
			else
			{
				Serial.println("not equal card id");
			}
		}
	}
	//Serial.println(endtime);
	if (is_card_present)
	{
		if (millis() > endtime)
		{
			// timeout
			Serial.println("Timeout");
			is_card_present = false;
		}
		return true;
	}
	return false;
}
bool CardService::Read_profile_from_card(Profile *profile)
{
	byte blockAddr = BANKDATABLOCK;
	// authenticate block
	if (!Authenticate_block(blockAddr))
	{
		return false;
	}
	// 

	Read_bytes_from_block(blockAddr, buffer, 18);

	memcpy(&profile->balance, buffer, 4);
	memcpy(&profile->transaction_count, &buffer[4], 4);
	memcpy(&profile->uid, &buffer[8], 4);

	blockAddr = NAMEDATABLOCK;
	Read_bytes_from_block(blockAddr, buffer, 18);
	memcpy(&profile->name, buffer, 16);
	
	return true;


}
bool CardService::Write_bytes_to_block(byte blockAddr, byte *bytes, byte len)
{
	// Write data to the block
	Serial.print(F("Writing data into block "));
	Serial.print(blockAddr);
	Serial.println(F(" ..."));
	dump_byte_array(buffer, 16);
	Serial.println();
	status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, buffer, len);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("MIFARE_Write() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		is_card_active = false;
		return false;
	}
	return true;
}
bool CardService::Read_bytes_from_block(byte blockAddr, byte *bytes, byte len)
{
	Serial.println("Reading block:");
	Serial.println(blockAddr);
	status = mfrc522.MIFARE_Read(blockAddr, bytes, &len);
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("MIFARE_Read() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return false;
	}
	Serial.println("Data read:");
	Serial.println(len);
	dump_byte_array(bytes, 16);
	Serial.println();
	return true;
}


bool CardService::Authenticate_block(byte blockAddr)
{
	return Authenticate_block(blockAddr,this->key);
}

bool CardService::Authenticate_block(byte blockAddr,MFRC522::MIFARE_Key* key_)
{
	status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, Get_trailer(blockAddr), key_, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK)
	{
		Serial.print(F("PCD_Authenticate() failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		is_card_active = false;
		return false;
	}
	return true;
}

void CardService::MakeCardProd(){

Serial.println("aut prod");
	if(Authenticate_block(BANKDATABLOCK,&Prod_key))
		return; // nothing to do here
	else
	{
		// now the card is not answearing

		while(Check_card()){
			if (is_card_active)
			{
				goto make_card_prod;
			}
		}
		Serial.println("Card lost: FAILED");
		return;

	}
	make_card_prod:
	// card is factory need rewrite datablock
Serial.println("aut factory");
	if(Authenticate_block(BANKDATABLOCK,&factory_key))
	{
Serial.println("read");
		bool val = Read_bytes_from_block(Get_trailer(BANKDATABLOCK), buffer, 18);
		dump_byte_array(buffer,16);
		memcpy(buffer,&Prod_key.keyByte,MFRC522::MIFARE_Misc::MF_KEY_SIZE);
		dump_byte_array(buffer,16);
		val = val && Write_bytes_to_block(Get_trailer(BANKDATABLOCK), buffer, 16);
		//Serial.println("Huge error");
		if(!val)
			Serial.println("Make card prof failed!");
	}
	else
	{
		//Serial.println("Huge error");
	}

}




bool CardService::is_block_factory(byte blockAddr)
{
	// TODO implement
}
bool CardService::set_block_key(MFRC522::MIFARE_Key *key)
{
	// TODO implement
}

// 2x16 for name

// void Convert2CardData(Profile* profile)

#endif
