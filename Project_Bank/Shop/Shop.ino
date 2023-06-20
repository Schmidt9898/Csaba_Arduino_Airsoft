/*
1. eszköz bolt,
2 kijelző (egy eladó egy ügyfélnek) ugyanazt írja ki, | this need modification on the i2c board
beolvasáskor kiírja a kijelzőkre a nevet és a rajta lévő összeget,
a bolt oldalán 6 gomb, +100,+200,+500 és -100,-200,-500
*/

#define DUAL_LCD_SCREEN

#include "include/input.h"
#include "include/display.h"
#include "include/CardService.h"

CardService cardservice; 

Profile temp_profile;

// DO NOT GIVE BIGGER NAME THAN THIS, IT WILL CAUSE THE SD CARD TO GO KERNEL PANIC
// DO NOT GIVE BIGGER NAME THAN THIS, IT WILL CAUSE THE SD CARD TO GO KERNEL PANIC
// DO NOT GIVE BIGGER NAME THAN THIS, IT WILL CAUSE THE SD CARD TO GO KERNEL PANIC
#define LOGFILE_NAME "TERM0.log"

#define button_pin_p1 A3
#define button_pin_p2 A4
#define button_pin_p5 A5
#define button_pin_m1 A6
#define button_pin_m2 A7
#define button_pin_m3 A8
#define button_pin_enter A9

#define zold_pin A0
#define sarga_pin A1
#define piros_pin A2

LED led_zold(zold_pin);
LED led_sarga(sarga_pin);
LED led_piros(piros_pin);

Button btn_p1(button_pin_p1);
Button btn_p2(button_pin_p2);
Button btn_p5(button_pin_p5);
Button btn_m1(button_pin_m1);
Button btn_m2(button_pin_m2);
Button btn_m3(button_pin_m3);
Button btn_enter(button_pin_enter);


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

	led_zold.turn(on); // if all good green led turned on
	logn("Connected!");
	
	pinMode(BUZZER, OUTPUT); // Set buzzer - pin 8 as an output //MOVE TO DISPLAY
	beep(3);
  
}

void loop()
{
	
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
	lcd_write(temp_profile.name, "Termeles inditas");
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

	// pénz kell a kártyára írni
	led_piros.turn(on);
	logn("Transaction.");
 
	int32_t previous_balance = temp_profile.balance;
	temp_profile.balance += termeles_jutalom;
	temp_profile.transaction_count += 1;

	if (!cardservice.Write_profile_to_card(&temp_profile))
	{
		// profile save failure, something went wrong
		lcd_write("Transaction fail", "Contact IT");
		delay(5000);
		lcd_write("Device will", "Restart");
		delay(5000);
		return;
	}
 	delay(1000);
 
	if (!cardservice.Check_card())
	{
		return;
	}

	temp_profile.print();

	// 16 + 1 +
	static char str[80];
	//  log to card
	//  name,uid,transaction_cunt,previous,added,new_balance

	// 4294967296

	//"aaaaaaaaaaaaaaaa,4294967296,4294967296,-2147483648,-2147483648,-2147483648R"

	sprintf(str,"%s,%lu,%lu,%ld,%d,%ld\n",temp_profile.name,temp_profile.uid,temp_profile.transaction_count,previous_balance,termeles_jutalom,temp_profile.balance); // csv format

	logn(String(str));
	Log2file(LOGFILE_NAME, str);
}
