#include "CardService.h"

CardService cardservice;

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
State *READ_WRITE = nullptr;
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
		Serial.println("1 Card continuity check");
		Serial.println("2 manage card content");
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
			if (cm == "2")
			{
				return READ_WRITE;
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
						// status = mfrc522.PICC_HaltA();
						status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &factory_key, &(mfrc522.uid));
						if (status != MFRC522::STATUS_OK)
						{
							Serial.print(F("PCD_Authenticate() failed: "));
							Serial.println(mfrc522.GetStatusCodeName(status));
							break;
						}
						else
						{
							Serial.print("Connected ");
							Serial.println(mfrc522.GetStatusCodeName(status));
						}
					}
				}
			}
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
		Serial.println("Continous card check test");
		Serial.println("commands:");
		Serial.println("q for quit");
		Serial.println("A to send it to sleep for quit");
		String cm;
		while ((cm = GetStr()) != "q")
		{
			if (cardservice.Check_card())
			{
				Serial.print("Y");
				tone(22, 440, 100);
			}
			else if (cardservice.Get_new_card())
			{
				Serial.println("New card is present.");
			}
			else
			{
				Serial.println("No card near");
				Serial.println(aut_counter);
				noTone(22);
			}

			delay(100);
		}
		Serial.println("quiting");
		return MENU;
	}
};

class Read_write : public State
{
public:
	Profile testprofile;

	Read_write()
	{

	};


	virtual State *main()
	{
		Serial.println("Read_write test");
		Serial.println("commands:");
		Serial.println("q for quit");
		Serial.println("");
   cardservice.check_module_connection();
		String cm;
		while (true)
		{
			while (cardservice.Check_card());
			while (!cardservice.Get_new_card())
				delay(300);
			tone(22, 440, 100);
			while (cardservice.Check_card())
			{
				cardservice.Read_profile_from_card(&testprofile);
				testprofile.print();
				sprintf(testprofile.name, "card C \0");
				testprofile.balance++;
				testprofile.transaction_count = 1234;
				testprofile.uid = 3;
				cardservice.Write_profile_to_card(&testprofile,true);
				delay(100);
				break;
			}
			break;
		}
		Serial.println("quiting");
		return MENU;
	}
};

Menu menu_class;
Listdetect listdetect_class;
Cardnear cardnear_class;
Read_write read_write_class;

//*****************************************************************************************//
void setup()
{
	MENU = &menu_class;
	LISTDETECT = &listdetect_class;
	CARDNEAR = &cardnear_class;
	READ_WRITE = &read_write_class;
	state = MENU;

	Serial.begin(9600); // Initialize serial communications with the PC
	cardservice.init();

	Serial.setTimeout(100);
	Serial.print("factory_key: ");
	printkey(factory_key);
	Serial.println();
	Serial.print("Prod_key: ");
	printkey(Prod_key);
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
