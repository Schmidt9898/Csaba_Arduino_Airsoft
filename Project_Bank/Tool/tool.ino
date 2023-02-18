

#include <SPI.h>
#include <MFRC522.h>

#define MEGA

#ifdef MEGA
#define RST_PIN 5 // Configurable, see typical pin layout above
#define SS_PIN 53 // Configurable, see typical pin layout above

#else

#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above

#endif

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

// Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
MFRC522::MIFARE_Key factorykey{{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
MFRC522::MIFARE_Key mykey{{0x00, 0x01, 0x03, 0x05, 0x07, 0x09}};

MFRC522::StatusCode status;

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
		String cm;
		while ((cm = WFS()) != "q")
		{
			Serial.println(cm);

			if (cm == "0")
			{
				return LISTDETECT;
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
					Serial.print("Wake UP");
					byte atqa_answer[2];
					byte atqa_size = 2;
					status = mfrc522.PICC_WakeupA(atqa_answer, &atqa_size);
					Serial.println(mfrc522.GetStatusCodeName(status));
				}
				if (cm == "r")
				{
					Serial.print("Request A ");
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
			

			// Dump debug info about the card; PICC_HaltA() is automatically called
			//mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
			

		}
		Serial.println("quiting");
		return MENU;
	}
};

Menu menu_class;
Listdetect listdetect_class;

//*****************************************************************************************//
void setup()
{

	MENU = &menu_class;
	LISTDETECT = &listdetect_class;
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
