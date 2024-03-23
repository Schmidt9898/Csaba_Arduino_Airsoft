/*
Kézi eszköz terminál,
1 kijelző 
beolvasáskor kiírja a kijelzőre a nevet és a rajta lévő összeget,
2 gomb
12 gombos 
lcd
*/

//#define DUAL_LCD_SCREEN

#include "include/input.h"
#include "include/display.h"
#include "include/CardService.h"

CardService cardservice;

Profile temp_profile;

// DO NOT GIVE BIGGER NAME THAN THIS, IT WILL CAUSE THE SD CARD TO GO KERNEL PANIC
#define LOGFILE_NAME "HAND0.log"

#define button_pin_enter A0
#define button_pin_cancel A1

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

const char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
const byte rowPins[ROWS] = {3, 4, 5, 6}; //2-7-6-4 LÁBAK a D3-D4-D5-D6 PORTRA
const byte colPins[COLS] = {7, 8, 9}; //3-1-5 LÁBAK a D7-D8-D9 PORTRA

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// TODO add keypad


// #define zold_pin A0
// #define sarga_pin A1
// #define piros_pin 7

// LED led_zold(zold_pin);
// LED led_sarga(sarga_pin);
// LED led_piros(piros_pin);

Button btn_p1(button_pin_p1);
Button btn_p2(button_pin_p2);
Button btn_p3(button_pin_p3);
Button btn_m1(button_pin_m1);
Button btn_m2(button_pin_m2);
Button btn_m3(button_pin_m3);
Button btn_enter(button_pin_enter);
Button btn_cancel(button_pin_cancel);

#define value_1 100
#define value_2 200
#define value_3 500

void Error_loop()
{
	lcd_write("Syst. init fail", "Restart device!");
	// led_zold.turn(on);
	// led_sarga.turn(off);
	// led_piros.turn(off);
	while (1)
	{
		// led_zold.togle();
		// led_sarga.togle();
		// led_piros.togle();
		delay(200);
	}
}

int check_password()
{
	char key = keypad.getKey();
	if (key)
	{
		logn("KEY: " + String(key));
		
		beep(BEEP_CLICK);

		if (key == '*')
		{ // clear pass
			input_password = "";
		}
		else if (key == '#')// enter
		{
			if (input_password == secret_pass)
			{
				input_password = "";
				return 1;
			}
			else
			{
				input_password = "";
				return -1;
			}
		}
		else
		{
			input_password += key;
			log("INPUT FIELD: " + input_password);
		}
	}
	return 0;
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

	// led_zold.turn(on); // if all good green led turned on
	logn("Connected!");

	pinMode(BUZZER, OUTPUT); // Set buzzer - pin 8 as an output //MOVE TO DISPLAY
	// beep(3);

	beep(5);

	// delay(1000);

	// tone(BUZZER, 808, 1000);
}

uint32_t next_check_time = 0;

void loop()
{

clear_card:
	// led_sarga.turn(off);
	// led_piros.turn(off);
	// led_zold.turn(on);

	logn("Waiting for Card.");
	lcd_write("Kartya", "  Kereses");

	short offset = 0;

	while (true)
	{

		if (next_check_time <= millis())
		{
			if (cardservice.Get_new_card())
				break;
			if (cardservice.Check_card())
				break;
			next_check_time = millis() + 1000;
		}
		delay(500);

		lcd1.clear();
		lcd1.setCursor(0, 0);
		lcd1.print("Hello Stalker!");
		lcd1.setCursor(0, 1);
		lcd1.print(&"Kerlek helyezd be a kartyat"[offset %= 28]);
		offset++;
	}
	cardservice.Read_profile_from_card(&temp_profile);
	temp_profile.print();

	int32_t cost = 0;
	bool update = true;

button_pressing:
	/////////////////////
	/// Kártya behelyezve/
	/////////////////////

	// led_sarga.turn(on);

	while (true)
	{
		if (next_check_time <= millis())
		{
			// logn("check");
			if (!cardservice.Check_card())
				return;
			next_check_time = millis() + 200;
		}

		if (btn_p1.GetState() == ButtonState::Pressed)
		{
			cost += value_1;
			beep(0);
			update = true;
		}
		if (btn_p2.GetState() == ButtonState::Pressed)
		{
			cost += value_2;
			beep(0);
			update = true;
		}
		if (btn_p3.GetState() == ButtonState::Pressed)
		{
			cost += value_3;
			beep(0);
			update = true;
		}
		if (btn_m1.GetState() == ButtonState::Pressed)
		{
			cost += -value_1;
			beep(0);
			update = true;
		}
		if (btn_m2.GetState() == ButtonState::Pressed)
		{
			cost += -value_2;
			beep(0);
			update = true;
		}
		if (btn_m3.GetState() == ButtonState::Pressed)
		{
			cost += -value_3;
			beep(0);
			update = true;
		}
		if (btn_enter.GetState() == ButtonState::Pressed)
		{
			update = true;
			goto transaction;
		}
		if (btn_cancel.GetState() == ButtonState::Pressed)
		{
			cost = 0;
			update = true;
			beep(1);
		}

		if (update) // update 2 lcd screen with new data
		{
			update = false;

			lcd0.clear();
			lcd1.clear();
			lcd0.setCursor(0, 0);
			lcd1.setCursor(0, 0);

			static char buff[17];
			sprintf(buff, "Egyn: %ld", temp_profile.balance);
			lcd0.print(buff);
			lcd1.print(buff);

			sprintf(buff, "Terh: %ld", cost);
			lcd0.setCursor(0, 1);
			lcd1.setCursor(0, 1);
			lcd0.print(buff);
			lcd1.print(buff);
		}

		delay(10);
	}

transaction:

	if (cost == 0)
	{
		goto button_pressing;
	}

	// pénz kell a kártyára írni
	// led_piros.turn(on);
	logn("Transaction.");

	int32_t previous_balance = temp_profile.balance;

	auto result = temp_profile.add_to_balance(cost);

	// ellenörzés, hogy biztos megcsinálható e
	if (result < 0)
	{ // nem mehetünk minuszba
		beep(4);
		// TDO write to lcd
		lcd_write("Hiba! Nincs eleg", " fedezet!", lcd0);
		lcd_write("Hiba! Nincs eleg", " fedezet!", lcd1);
		delay(5000);
		// led_piros.turn(off);
		goto button_pressing;
	}

	// try to write it to the card
	if (!cardservice.Write_profile_to_card(&temp_profile))
	{
		temp_profile.balance = previous_balance;
		temp_profile.transaction_count--;
		// profile save failure, something went wrong
		lcd_write("Transaction fail", "Contact IT");
		delay(5000);
		lcd_write("Device will", "Restart");
		delay(5000);
		return;
	}
	// everything is fine, card write is done and succesfull

	beep(6);
	delay(100);

	if (result == TransactionResult::MAXEDOUT)
	{
		beep(6); // TODO new beep
		lcd_write("Kartya", "   megtelt");
		delay(1000);
	}

	if (!cardservice.Check_card())
	{
		return; // start from the begining
	}

	temp_profile.print();

	{
#ifdef USESERIAL
		// 16 + 1 +
		static char str[80];
		//  log to card
		//  name,uid,transaction_cunt,previous,added,new_balance
		// 4294967296
		//"aaaaaaaaaaaaaaaa,4294967296,4294967296,-2147483648,-2147483648,-2147483648R"
		logn(temp_profile.balance);
		sprintf(str, "%s,%lu,%lu,%ld,%ld,%ld\n", temp_profile.name, temp_profile.uid, temp_profile.transaction_count, previous_balance, cost, temp_profile.balance); // csv format
		logn(String(str));
// Log2file(LOGFILE_NAME, str);
#endif
	}
}
