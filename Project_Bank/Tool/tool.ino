

#include <SPI.h>
#include <MFRC522.h>



void printkey(MFRC522::MIFARE_Key &key)
{
	for (byte i = 0; i < 6; i++)
	{
		Serial.print(key.keyByte[i], HEX);
		Serial.print(" ");
	}
}

class Menu;
class Listdetect;
class Selected_keep;

class State
{
public:
	virtual State *main()
	{
		return this;
	};
};

State *MENU = nullptr;
State *LISTDETECT = nullptr;
State *CARDNEAR = nullptr;
State *state = nullptr; // MENU;

String WFS();
String GetStr();

class Menu : public State
{
public:
	virtual State *main()
	{
		Serial.println("RFC debug tool:");
		Serial.println("commands:");
		Serial.println("0 is card, present loop");
		Serial.println("1 is card keep");
		String cm;
		while ((cm = WFS()) != "q")
		{
			Serial.println(cm);

			if (cm == "0")
			{
				return LISTDETECT;
			}
			if (cm == "1")
			{
				return CARDNEAR;
			}
		}
		return this;
	}
};
class Listdetect : public State
{
public:
	virtual State *main()
	{
		Serial.println("RFC List tool:");
		Serial.println("commands:");
		Serial.println("q for quit");
		String cm;
		while ((cm = GetStr()) != "q")
		{
			if (cm != "")
			{
				Serial.println(cm);
				if (cm == "0")
				{
					return LISTDETECT;
				}
				if (cm == "w")
				{
					Serial.print("Wake UP ");
					byte atqa_answer[2];
					byte atqa_size = 2;
					status = mfrc522.PICC_WakeupA(atqa_answer, &atqa_size);
					Serial.println(mfrc522.GetStatusCodeName(status));
				}
				if (cm == "r")
				{
					Serial.print("RequestA ");
					byte atqa_answer[2];
					byte atqa_size = 2;
					status = mfrc522.PICC_RequestA(atqa_answer, &atqa_size);
					Serial.println(mfrc522.GetStatusCodeName(status));
				}					
				if (cm == "d")
				{
					mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
				}
				if (cm == "p")
				{
					Serial.print("picc selected: ");
					Serial.println(mfrc522.PICC_ReadCardSerial());
					mfrc522.PICC_DumpDetailsToSerial(&mfrc522.uid);
				}
				if (cm == "h")
				{
					status = mfrc522.PICC_HaltA();
					Serial.print("HALT ");
					Serial.println(mfrc522.GetStatusCodeName(status));
				}
				if (cm == "c")
				{
					byte blockAddr = 11;
  					byte trailerBlock = 11;
					while ((cm = GetStr()) != "q")
					{
						//status = mfrc522.PICC_HaltA();
						status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &factorykey, &(mfrc522.uid));
						if (status != MFRC522::STATUS_OK) {
							Serial.print(F("PCD_Authenticate() failed: "));
							Serial.println(mfrc522.GetStatusCodeName(status));
							break;
						}else
						{
						Serial.print("Connected ");
						Serial.println(mfrc522.GetStatusCodeName(status));
						}
					}
				}


			}


			//Serial.println(mfrc522.PICC_IsNewCardPresent());
			//mfrc522.PICC_ReadCardSerial();
			//Serial.println(mfrc522.uid);
			//mfrc522.PICC_DumpDetailsToSerial(&mfrc522.uid);
			
			/*

			// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
			if ( ! mfrc522.PICC_IsNewCardPresent()) {
				continue;
			}

			// Select one of the cards
			if ( ! mfrc522.PICC_ReadCardSerial()) {
				continue;
			}
			mfrc522.PICC_DumpDetailsToSerial(&mfrc522.uid);

			//PICC_Select
			status = mfrc522.PICC_HaltA();

			Serial.println(mfrc522.GetStatusCodeName(status));
			mfrc522.PCD_StopCrypto1();
			*/
			
			

		}
		Serial.println("quiting");
		return MENU;
	}
};

class Cardnear : public State
{
public:
	virtual State *main()
	{
		Serial.println("Card near");
		Serial.println("commands:");
		Serial.println("q for quit");
		String cm;
		while ((cm = GetStr()) != "q")
		{
			if (cm != "")
			{
				if(cm == "s")
				{
					Keep_card(2);
				}

			}
		}
		Serial.println("quiting");
		return MENU;
	}

	bool is_equal(byte* a,byte*b,byte size)
	{
		for(byte i=0;i<size;i++)
		{
			if(a[i] != b[i])
				return false;
		}
		return true;
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






};



Menu menu_class;
Listdetect listdetect_class;
Cardnear cardnear_class;

//*****************************************************************************************//
void setup()
{

	MENU = &menu_class;
	LISTDETECT = &listdetect_class;
	CARDNEAR = &cardnear_class;
	state = MENU;

	Serial.begin(9600); // Initialize serial communications with the PC
	SPI.begin();		// Init SPI bus
	mfrc522.PCD_Init(); // Init MFRC522 card
	Serial.setTimeout(100);
	Serial.println(F("Read PICC:")); // shows in serial that it is ready to read

	Serial.print("factory key: ");
	printkey(factorykey);
	Serial.println();
	Serial.print("mykey: ");
	printkey(mykey);
	Serial.println();
}

//*****************************************************************************************//

void loop()
{
	state = state->main();
	delay(100);
}
//*****************************************************************************************//

String WFS() // Wait for string
{
	while (!Serial.available())
		delay(100);
	String s = Serial.readString();
	s.trim();
	return s;
};
String GetStr() // Get string, if not avail return "" empty string
{
	if (Serial.available())
	{
		String s = Serial.readString();
		s.trim();
		return s;
	}
	else
		return "";
};
