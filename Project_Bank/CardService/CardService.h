#ifndef CARDSERVICE
#define CARDSERVICE

#include <SPI.h>
#include <MFRC522.h>

//data to be stored

MFRC522::MIFARE_Key key;
//TODO init this with the key
//for (byte i = 0; i < 6; i++) key.keyByte[i] = i+1;
key.keyByte = {0x01,0x02,0x03,0x04,0x05,0x06}; //Secret password




struct Profile
{
String name = "noname";
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
#define BANKDATATRAILER 7

//#define PERSON_NAME0 8
//#define PERSON_NAME1 9
//#define PERSON_BALANCE 10



bool Write_profile_to_card(Profile* profile,Uid *uid)
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


//2x16 for name

//void Convert2CardData(Profile* profile)










byte Targetblockaddr;
byte len;
  MFRC522::StatusCode status;


#endif