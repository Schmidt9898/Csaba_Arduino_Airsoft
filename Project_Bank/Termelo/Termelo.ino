/*
2. eszköz termelő állomás
áramot kap az eszköz, zöld ledet világít
behelyezik a kártyát, kijelzőn megjelenik a termelés indítása felirat
megnyomják a start gombot "termelés indul(esetleg egy betöltő csík mehetne a kijelzőn) 30 percig "tölt"
közbe sárga led világít (villog?),
ha lejárt a kártyára +500(hasra ütöttem az összeget) felíródik
a kártyára írás közben piros led és ne vegye ki a kártyát felirat,
majd vissza kezdő állapotba, zöld led, startra vár
*/

#include "include/display.h"
#include "include/CardService.h"

CardService cardservice; //  9,10 pin

Profile temp_profile;

// DO NOT GIVE BIGGER NAME THAN THIS, IT WILL CAUSE THE SD CARD TO GO KERNEL PANIC
// DO NOT GIVE BIGGER NAME THAN THIS, IT WILL CAUSE THE SD CARD TO GO KERNEL PANIC
// DO NOT GIVE BIGGER NAME THAN THIS, IT WILL CAUSE THE SD CARD TO GO KERNEL PANIC
#define LOGFILE_NAME "TERM0.log"

#define button_pin A0

#define zold_pin A1
#define sarga_pin A2
#define piros_pin A3

#define termeles_jutalom 500

bool isbuttonDown = false;

uint32_t termeles_time = 60*30;

LED led_zold(zold_pin);
LED led_sarga(sarga_pin);
LED led_piros(piros_pin);

void Error_loop()
{
	lcd_write("Syst. init fail", "Restart device!");
	led_zold.turn(on);
	led_sarga.turn(off);
	led_piros.turn(off);
	while (1)
	{
		led_zold.togle();
		led_sarga.togle();
		led_piros.togle();
		delay(200);
	}
}

void setup()
{
	init_log; // Initialize serial communications with the PC, cn be turned off by define

	lcd_init(); // initialize the 2x16 lcd display i2c

	bool succes = cardservice.init(); // init SPI rfid and sd card system

	if (!succes)
	{
		Error_loop();
	}

	pinMode(button_pin, INPUT_PULLUP); // active low button

	led_zold.turn(on); // if all good green led turned on
	logn("Connected!");
	
	pinMode(BUZZER, OUTPUT); // Set buzzer - pin 8 as an output //MOVE TO DISPLAY
	beep(3);
  
}

void loop()
{
	/**
	 * @brief After start or succesfull termeles the device swiched to idle state
	 * Waiting for card to be inserted.
	 *
	 */
clear_card:
	led_sarga.turn(off);
	led_piros.turn(off);
	led_zold.turn(on);
	logn("Waiting for Card.");
	lcd_write("Helyezd be", "      a kartyat.");
	while (true)
	{
		if (cardservice.Get_new_card())
			break;
		if (cardservice.Check_card())
			break;
		delay(1000);
	}
	cardservice.Read_profile_from_card(&temp_profile);
	temp_profile.print();
	// megvárjuk a gombnyomást
	logn("Waiting for Button press.");
	lcd_write(temp_profile.name, "Felt. inditas");
	led_sarga.turn(on);
	while (true)
	{
		if (cardservice.Check_card())
		{
			if (!digitalRead(button_pin))
			{
				if (!isbuttonDown)
				{
					isbuttonDown = true; // ha leragasztják a gombot akkor ez meggátolja a további elfogadást
					logn("pressed");
					delay(100); //  button transient noise canceling delay.
					goto termeles;
				}
			}
			else
			{
				isbuttonDown = false;
			}
			delay(200);
			log(".");
		}
		else
		{
			// we lost the card go to idle
			return; // same as goto clear_card
					// goto clear_card;
		}
	}
termeles:
	logn("termeles...");
 
	led_sarga.turn(on);
	uint32_t end_time = millis() + 1000 * termeles_time;
	lcd_write("Feltoltes:", "");
	while (millis() < end_time)
	{
		int segment = int(16 - 16 * (float(end_time - millis()) / float(1000 * termeles_time)));
		if (cardservice.Check_card())
		{
			// ez jó de nem kell csinálni semmit
		}
		else
		{
			lcd_write("Kartya elveszett", "Termelesi hiba.");
			delay(2000);
			return; // same as goto clear_card
					// goto clear_card;
		}

		lcd_char(' ', 1, segment);
		delay(500);
		lcd_char('#', 1, segment);
		led_sarga.togle();
		delay(500);
		beep(3);
		// tune(100)
	}
	led_sarga.turn(off);

	// termelés sikeres pénz kell a kártyára írni
	led_piros.turn(on);
	logn("Transaction.");
 
	int32_t previous_balance = temp_profile.balance;
	temp_profile.balance += termeles_jutalom;
	temp_profile.transaction_count += 1;

	if (!cardservice.Write_profile_to_card(&temp_profile))
	{
		// profile save failure, something went wrong
		logn("Transaction fail.");
		lcd_write("Transaction fail", "Contact IT");
		delay(5000);
		lcd_write("Device will", "Restart");
		delay(5000);
		return;
	}
 
	if (!cardservice.Check_card())
	{
		return;
	}

	temp_profile.print();




#ifdef USESERIAL
	static char str[80]; //more than enough
	//  log to card
	//  name,uid,transaction_cunt,previous,added,new_balance
	//longest string is
	//"aaaaaaaaaaaaaaaa,4294967296,4294967296,-2147483648,-2147483648,-2147483648R"
	sprintf(str,"%s,%lu,%lu,%ld,%ld,%ld\n",temp_profile.name,temp_profile.uid,temp_profile.transaction_count,previous_balance,termeles_jutalom,temp_profile.balance); // csv format
	logn(String(str));
#endif
	//Log2file(LOGFILE_NAME, str);
}
