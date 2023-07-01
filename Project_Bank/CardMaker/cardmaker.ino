
#include "include/input.h"
#include "include/CardService.h"

CardService cardservice;
// String WFS();
// String GetStr();

Profile temp_profile;

#define button_pin_enter A0
Button btn_enter(button_pin_enter);

void setup()
{
	Serial.begin(9600); // Initialize serial communications with the PC

	cardservice.init();

	// TODO cardreader

	Serial.println("Connected all good!");
}
void loop()
{
	Serial.println("Waiting for profile");

	while (Serial.available() < 32)
	{
		if (btn_enter.GetState() == ButtonState::Pressed)
		{
			//Serial.println("Waiting for profile");
			return;
		}
	}

	// get profile data

	// change key if needed

	// upload profile

	// read profile

	// compare

	// return status with uid

	unsigned char buffer[16];
	if (Serial.available() >= 32)
	{
		// Name
		Serial.readBytes(buffer, 16);
		dump_byte_array(buffer, 16);
		memcpy(&temp_profile.name, buffer, 16);
		// Data
		Serial.readBytes(buffer, 16);
		dump_byte_array(buffer, 16);
		// Serial.write(buffer,16);
		// Serial.flush();
		Serial.println(""); // for the C# code part.
		memcpy(&temp_profile.balance, buffer, 4);
		memcpy(&temp_profile.transaction_count, &buffer[4], 4);

		temp_profile.print();

		//Serial.println("here");

		while (cardservice.Check_card())
		{
		//Serial.println("here2");

			if (btn_enter.GetState() == ButtonState::Pressed)
			{
				//Serial.println("Waiting for profile");
			return;

			}
		}
		while (!cardservice.Get_new_card())
		{
		//Serial.println("here3");

			if (btn_enter.GetState() == ButtonState::Pressed)
			{
				//Serial.println("Waiting for profile");
			return;

			}
			delay(100);
		}
		tone(8, 440, 100);
		memcpy(&temp_profile.uid, &cardservice.selected_uid, 4);

		// make card private key if it is not
		cardservice.MakeCardProd();

		cardservice.Write_profile_to_card(&temp_profile, true);
		delay(100);

		// extra added information

		temp_profile.print();
		// for the C# code part.
		Serial.print("Profile UID :");
		dump_byte_array((unsigned char *)&temp_profile.uid, 4);
		Serial.println("");
		// Serial.println(temp_profile.uid);
		Serial.println("--END--");
	}
}

// Serial.readBytes()

/*
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
*/
