
//  _   _                   ____  _   _ __  __ __  __ 
// | \ | | __ _  __ _ _   _| __ )| | | |  \/  |  \/  |
// |  \| |/ _` |/ _` | | | |  _ \| | | | |\/| | |\/| |
// | |\  | (_| | (_| | |_| | |_) | |_| | |  | | |  | |
// |_| \_|\__,_|\__, |\__, |____/ \___/|_|  |_|_|  |_|
//              |___/ |___/                           
//

/**
 * @brief Airsoft rendezvényre kellék, megoldható rejtvénypanel vezérlő szoftver.
 *
 */

#define DEMO // Ha DEMO definiálva van akkor csak az én birtokomban lévő arduino mega konfigurációra buildel a program
			 //  vegyük ki a DEMO sort itt ha nem arra akarunk fordítani
#define DEBUG // comment this line out if you dont want log, also can reduce programsize
			  // ha van log akkor kiépül a usb kommunikáció ami resetelheti az arduinót, de csak abban az esetben ha az géphez van kötve


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

//______                              _                
//| ___ \                            | |               
//| |_/ /_ _ _ __ __ _ _ __ ___   ___| |_ ___ _ __ ___ 
//|  __/ _` | '__/ _` | '_ ` _ \ / _ \ __/ _ \ '__/ __|
//| | | (_| | | | (_| | | | | | |  __/ ||  __/ |  \__ \
//\_|  \__,_|_|  \__,_|_| |_| |_|\___|\__\___|_|  |___/
                                                     

#define _20_day_in_sec 1728000


// Innentől változtathatsz paramétereket

#ifdef DEMO
#define penalty_time_barrier 3599 // 59 perc 59 másodperc
#else
#define penalty_time_barrier three_day_in_sec // Ez az időintervallum alatt nem vonuk le büntetőidőt.
#endif


//#define	TIME_OF_DETONATION	120000   // ez a paramétert a setv var programban állítsd
#define DETENTION_TIME  	30*60
#define TIME_PENALTY		60*60
#define	MORNIG_START		8
#define	NIGHT_START			20
#define	FIRST_PASS			"0000"
#define	FINAL_PASS			"1111"


//innentől ne változtass a kódon

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#define MOTION_SENSOR 47 // mozgásérzékelő pin
#define PIROTECH 48		 // bomba (pirotechnikai eszköz) aktiváló pin //ezt a pint különösen figyelni hogy ne definiáljuk újra

#define BUZZER 49 // piezo buzzer

// kártyaolvasó module
#define card_reader_input_pin 3	   // ez veszi a jelet
#define card_reader_activate_pin 2 // ez ad áramot

/*
További pin kiosztás
22-33 ig a minijátékok 3as ával  -> activate, jo megoldás, rosz megoldás pin

34-39 LCD kijelző 2x16 //TODO pontos pinek sorrendje

50-53 sorrendben játék állapotot jelző (zöld) ledek

A0-A8 ig mp3 modul vezérlő pinek

20-21 gyroscope module megfelelő pinjei

6,5,4 óramodul //TODO melyik pinek

7-13 elvileg a pinpad lesz itt de nem tudtuk kipróbálni //TODO

14-16 led 7 segment kijelző //TODO lehet át kéne rakni másik digitális pinre

*/

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


#include <EEPROM.h>

#include "log.h"
#include "clock.h"
#include "display.h"

enum State
{
	Night = 1,
	Detention = 6,
	Wait_card = 2,
	Wait_pin = 3,
	Day = 4,
	Wait_final_pin = 5,
	error = 404
	// Night="night", Detention="detention", Wait_card="card", Wait_pin="pin", Day="day", error="error"
};
void switch_state(State next);
State state;					  // milyen helyzetben vagyunk
State enter_detention_from_state; // hol voltunk mielött büntibe kerültünk
State enter_night_from_state;	  // hol voltunk mielött este lett

uint32_t detention_time = DETENTION_TIME; // in second //fél óra
uint32_t penalty_time = TIME_PENALTY;	// in second this will subtract from time 1 hour //1 óra
uint32_t time_left_at_start = _20_day_in_sec;	// THIS IS FOR CLOCK ERROR CHECK

uint32_t time_of_detonation=-1; 		// time of detonation, save it in eeprom can't change here

Clock clock;
const int time_morning = MORNIG_START; 	// mikor kezdődjön a reggel
const int time_night = NIGHT_START;  		// meddig tartson a nap

const String first_pass = FIRST_PASS; // jelszo kartya olvasás után
const String final_pass = FINAL_PASS; // utolsó jelszó
String input_password = "";		  // don't change, itt tárolom az eddig beírt karaktereket

byte progress = 0; // itt tároljuk az előrehaladást
//	1bit popszeg 1 bit detention,2 bit sate, 4 bit games,
//	10100101
//	00000100 1<<2
//	00000100
//	progress & (1<<2)

// 2 bit empty
// 2 bit card and pin
// 4 bit minigames

LED leds[4];			 // ledek
Mini_game mini_games[4]; // játékok
Gyoscope gyro;			 // giroszóp
uint8_t active_game_idx = 0;

#ifdef DEMO
#define NO_KEY '\0'
struct Keypad
{
	int next = 0;
	int count = 0;
	char getKey()
	{
		if (millis() > next)
		{
			count++;
			next = millis() + 1000;
			if (count == 5)
			{
				count = 0;
				return '#';
			}
			return '0';
		}
		return NO_KEY;
	}
} keypad;
#else
#include <Keypad.h>
const byte ROWS = 4, COLS = 3;
char keys[ROWS][COLS] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}};
byte rowPins[ROWS] = {10, 11, 12, 13}; // 2-7-6-4 LÁBAK a D10-D11-D12-D13 PORTRA
byte colPins[COLS] = {7, 8, 9};		   // 3-1-5 LÁBAK a D7-D8-D9 PORTRA
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
#endif

void setup()
{
#ifdef DEBUG
	Serial.begin(9600);
#endif

	// ez a kikommentelt rész felel azért hogy az elején lementsük az adatokat
	// erre a célra készült egy másik kód is ami csak ezt tartalmazza
	// elkészült az a kód -> setvar/setvar.ino

	log("Program starting.");
	lcd_init();
	lcd_write("Booting", "   .......");
	led_segment_init();
	led_segment(0); // may remove

	gyro.init();

	init_music_player();

	pinMode(MOTION_SENSOR, INPUT); 		// may need resistor 10k
	pinMode(PIROTECH, OUTPUT);	   		// bomba
	pinMode(card_reader_input_pin, INPUT);
	pinMode(card_reader_activate_pin, OUTPUT);	// HIGH is accept
	digitalWrite(card_reader_activate_pin, LOW);

	digitalWrite(PIROTECH, LOW);	//may give it a resistor

	pinMode(BUZZER, OUTPUT);
	noTone(BUZZER);
	

	beep(0);
	delay(500);
	beep(0);
	delay(500);
	beep(1);
	delay(200);

	//ledek
	leds[0] = LED(50);
	leds[1] = LED(51);
	leds[2] = LED(52);
	leds[3] = LED(53);

	log("INIT: leds,bomb,motion sensore,card reader initialized");
	// load eeprom saved data
	
	progress = EEPROM[0];
	time_of_detonation = ((uint32_t)EEPROM[1]);
	time_of_detonation |= ((uint32_t)EEPROM[2]) << 8;
	time_of_detonation |= ((uint32_t)EEPROM[3]) << 16;
	time_of_detonation |= ((uint32_t)EEPROM[4]) << 24;

	detention_end = ((uint32_t)EEPROM[5]);
	detention_end |= ((uint32_t)EEPROM[6]) << 8;
	detention_end |= ((uint32_t)EEPROM[7]) << 16;
	detention_end |= ((uint32_t)EEPROM[8]) << 24;

	//progress = 0b10000000;
	for (int i = 0; i < 4; i++)
	{
		if (progress & 1 << i)
			active_game_idx++;
		else
			break;
	}

	log("Progress: " + String(progress, 2));
	lcd_write("Progress: ", "   "+String(progress, 2));

	//log("next game to play: " + String(active_game_idx));

	//log("detonation time: " + time_to_string(time_of_detonation));
	log("loaded detonation time: " + String(time_of_detonation));
	//log(String(time_of_detonation, 2));
	log("loaded detention: " + time_to_string(detention_end));

	log("Start program");
	if (!(progress & 1 << 7)) // it the first start
	{
		log("Wait for first start");
		uint32_t t=millis()+60000;
		while(t>millis())
		{
		lcd_write(String((t-millis())/1000)+" mp", " elso inditas.");
		delay(1000);
		}
		// first start
		// kell idő a beüzemelésre , lecsavarozásra és elhagyásra
		// 1 órát várunk ilyenkor
		
		progress |= 1 << 7;
		save_progress();

		// delay(3600000);// 1 hour delay
		// delay(60000);// 1 min delay
		uint32_t end_bolting=millis()+3600000;
		//uint32_t end_bolting = millis() + 60000;
		while (millis() < end_bolting)
		{
			beep(2);
			lcd_write("Telepites!", time_to_string((end_bolting - millis()) / 1000));
			delay(1000);
		}
	}

	state = State::Wait_card;

	if (progress & (1 << 4))
	{
		log("Card is done by save");
		state = State::Wait_pin;
	}
	if (progress & (1 << 5))
	{
		log("Pin is done by save");
		state = State::Day;
	}

	switch_state(State::Detention);

	clock.refresh();

	if(!clock.is_disconected) // save the valid left till explosion time
		time_left_at_start = 60 + time_of_detonation-clock.sec;
	//time_left_at_start/=2;//REMOVE THIS
	
	
	log(String(time_left_at_start));
	// detention_end=clock.sec+detention_time;

	leds[0].turn(progress & (1 << 0));
	leds[1].turn(progress & (1 << 1));
	leds[2].turn(progress & (1 << 2));
	leds[3].turn(progress & (1 << 3));

	mini_games[0] = Mini_game{22, 23, 24, progress & (1 << 0)};
	mini_games[1] = Mini_game{25, 26, 27, progress & (1 << 1)};
	mini_games[2] = Mini_game{28, 29, 30, progress & (1 << 2)};
	mini_games[3] = Mini_game{31, 32, 33, progress & (1 << 3)};

	for (int i = 0; i < 4; i++)
	{
		mini_games[i].init();
		// mini_games[i].activate();
	}
	log("Next game to play: " + String(active_game_idx));

	// if(state != State::Detention)
	//	switch_state(state);
}

void detention_loop();
void day_loop();
void night_loop();
void card_loop();
void pinpad_loop();
void final_pinpad_loop();

void explosion_loop();

uint32_t next_music_play_time = 180000; // 3 min

uint32_t next_log_time = 0;




void loop()
{
	// state independent tasks
	clock.refresh();
	if(clock.is_disconected)
	{
		log("ERROR: Clock is disconected.");
		lcd_write("ERROR: Clock is "," disconected!!",1);
		beep(3);
		delay(1000);
		lcd_write("GAME STOPPED"," CALL HELP!!",1);
		delay(1000);
		return;
	}
	if(time_of_detonation - clock.sec > time_left_at_start)
	{
		log("ERROR: Clock is reseted.");
		lcd_write("ERROR: CLOCK IS"," OUT OF SYNC",1);
		beep(3);
		delay(1000);
		lcd_write("GAME STOPPED"," CALL HELP!!",1);
		delay(1000);
		return;
	}



	lcd_update();
	// log("Time is: "+clock.get_time());
	if (next_log_time <= millis())
	{
		next_log_time = millis() + 1000; // TODO change to 500
		led_segment(time_of_detonation - clock.sec);
		// log("det time :"+String(time_of_detonation));
		//log("Time: "+clock.get_time()+"     T-"+time_to_string(time_of_detonation-clock.sec));
	}
	if (time_of_detonation <= clock.sec)
	{
		//Serial.println(clock.get_time());
		//Serial.println(String(time_of_detonation)+"  "+String(clock.sec));
		// well boom
		explosion_loop();
	}

	if (gyro.did_gyroscope_move())
	{
		log("Gyroscope motion detected");
		add_penalty();
		switch_state(State::Detention);
		play_music(m_move);
		lcd_write("Ne mozgass!!","   Ne mozgass!!",6);
	}

	if (clock.isDay() && detect_motion())
	{

		if (state == State::Detention)
		{
			// play_music(m_cant_play);
		}
		else
		{
			log("Motion sensore trigered");
			play_music(m_welcome);
			lcd_write("Udvozollek!"," Stalker",10);
			next_music_play_time = millis() + 180000;
		}
	}

	if (clock.isDay() && state != State::Detention)
	{
		if (next_music_play_time < millis())
		{
			play_music(m_music);
			next_music_play_time = millis() + 300000;
		}
	}

	// state switch
	switch (state)
	{
	case State::Day:
		day_loop();
		break;
	case State::Night:
		night_loop();
		break;
	case State::Wait_card:
		card_loop();
		break;
	case State::Wait_pin:
		pinpad_loop();
		break;
	case State::Wait_final_pin:
		final_pinpad_loop();
		break;
	case State::Detention:
		detention_loop();
		break;
	default:
		break;
	}
	delay(10);
}

void switch_state(State next)
{
	if (next == state)
	{
		log("SAME STATE");
		// return;
	}

	// leave state events
	if (state == State::Detention && next == State::Detention)
	{
		return;
	}
	if (state == State::Night && next == State::Detention)
	{
		return;
	}

	if (state == State::Day)
	{
		for (int i = 0; i < 4; i++)
			mini_games[i].deactivate();
	}
	if (state == State::Wait_card)
	{
		digitalWrite(card_reader_activate_pin, LOW);
	}

	State from = state;
	state = next;

	// enter state events

	switch (state)
	{
	case State::Day:
		log("Entering Day");
		lcd_write("Udvozollek! *_* ", " "+String(active_game_idx+1)+". modul aktiv");
		mini_games[active_game_idx].activate();
		// for(int i=0;i<4;i++)
		//	mini_games[i].activate();
		break;
	case State::Night:
		log("Entering Night");
		lcd_write("Modulok Zarolva", "Gyere vissza 800");
		enter_night_from_state = from;
		break;
	case State::Wait_card:
		log("Entering Wait_card");
		lcd_write("Add meg a", " kartyat");
		digitalWrite(card_reader_activate_pin, HIGH);
		break;
	case State::Wait_pin:
		input_password = "";
		log("Entering Wait_pin");
		lcd_write("Add meg a", " pinkodot");
		break;
	case State::Wait_final_pin:
		input_password = "";
		lcd_write("Add meg a", " vegso pinkodot.");
		log("Entering Wait_final_pin");
		break;
	case State::Detention:
		log("Entering Detention");
		enter_detention_from_state = from;
		break;
	}
};

void day_loop()
{
	// log("Day");

	// lcd_write("Day  "+clock.get_time(),"T-"+time_to_string(time_of_detonation-clock.sec));

	if (!clock.isDay())
	{
		log("pack for the night");
		// pack for the night
		switch_state(State::Night);
		return;
	}

	if (mini_games[active_game_idx].solved)
	{
		log("WARNING INVALID GAME STATE");
		mini_games[active_game_idx].deactivate();
		active_game_idx++;
		lcd_write("Udvozollek! *_* ", " "+String(active_game_idx+1)+". modul aktiv");
		if (active_game_idx < 4)
		{
			mini_games[active_game_idx].activate();
		}
	}

	// for(int i=0;i<4;i++)
	{
		auto t_state = mini_games[active_game_idx].get_state();

		if (t_state == Mini_game_state::Unknown)
		{
			// log("game state is unknown");
		}
		else if (t_state == Mini_game_state::Good)
		{
			log("minigame "+String(active_game_idx)+" passed");
			leds[active_game_idx].turn(1);
			progress = progress | 1 << active_game_idx;
			save_progress();
			active_game_idx++; // go to next game
			play_music(m_passed_attempt);
			lcd_write("Udvozollek! *_* ", " "+String(active_game_idx+1)+". modul aktiv");
			lcd_write("Modul "+String(active_game_idx),"  megoldva  *_*",5);
			if (active_game_idx < 4)
			{
				mini_games[active_game_idx].activate();
			}
		}
		else if (t_state == Mini_game_state::Bad)
		{
			log("minigame "+String(active_game_idx)+" failed attempt");
			add_penalty();
			switch_state(State::Detention);
			play_music(m_failed_attempt);
			lcd_write("Ezt meg"," gyakorolni kell",5); //TODO change szöveg
			return;
		}
		else
		{
			//do nothing
			//log("minigame unhandled state");
		}
	}
	// ellenörzés
	bool check = true;
	for (int i = 0; i < 4; i++)
	{
		check = check && mini_games[i].solved;
	}
	if (check)
	{
		// all game is passed
		log("all game is passed");
		play_music(m_all_game_passed);
		switch_state(State::Wait_final_pin);
		lcd_write("Minden modul ", "  Sikeres!!",10); // TODO
	}
	// switch_state(State::Night);
}
void night_loop()
{
	// log("Night");
	if (clock.isDay())
	{
		log("it is morning, let's wake up");
		// wake up the system
		switch_state(enter_night_from_state);
		return;
	}
	// switch_state(State::Detention);
}
void card_loop()
{
	if (!clock.isDay())
	{
		log("pack for the night");
		// pack for the night
		switch_state(State::Night);
		return;
	}

	if (digitalRead(card_reader_input_pin))
	{
		// all good, save state and next phase
		log("card is good");
		play_music(m_card_accept);
		progress = progress | 1 << 4;
		save_progress();
		switch_state(State::Wait_pin);
	}
}

void pinpad_loop()
{
	if (!clock.isDay())
	{
		log("pack for the night");
		// pack for the night
		switch_state(State::Night);
		return;
	}

	char key = keypad.getKey();
	if (key)
	{
		log("KEY: "+key);
		beep(0);
		if (key == '*')
		{ // clear pass
			input_password = "";
			// cursor
		}
		else if (key == '#')
		{
			// enter
			if (input_password == first_pass)
			{
				input_password = "";
				play_music(m_pass_good);
				switch_state(State::Day);
				progress = progress | 1 << 5;
				save_progress();
			}
			else
			{
				input_password = "";
				play_music(m_failed_attempt);
				add_penalty();
				switch_state(State::Detention);
				lcd_write("Hibas pin","     *_*",5); //TODO change szöveg
			}
		}
		else
		{
			input_password += key;
			lcd_write("Kerem a jelszot:", input_password);
			log("INPUT FIELD: "+input_password);
		}
	}
}
void final_pinpad_loop()
{
	if (!clock.isDay())
	{
		log("pack for the night");
		// pack for the night
		switch_state(State::Night);
		return;
	}

	char key = keypad.getKey();
	if (key)
	{
		log("KEY: "+key);
		beep(0);
		if (key == '*')
		{ // clear pass
			input_password = "";
			// cursor
		}
		else if (key == '#')
		{
			// enter
			if (input_password == final_pass)
			{
				input_password = "";
				play_music(m_defusing_complete);
				progress = 255;
				save_progress();
				lcd_write("Gratulalunk!", " Hatastalanitva!");
				while (1);
				// switch_state(State::Day);
			}
			else
			{
				input_password = "";
				play_music(m_failed_attempt);
				add_penalty();
				switch_state(State::Detention);
				lcd_write("Hibas pin","     *_*",5); //TODO change szöveg
			}
		}
		else
		{
			input_password += key;
			lcd_write("Kerem a jelszot:", input_password);
			log("INPUT FIELD: "+input_password);
		}
	}
}

void add_penalty()
{
	detention_end = clock.sec + detention_time;

	// log(time_of_detonation-clock.sec);
	// log("####");
	// log(penalty_time);
	if (time_of_detonation - clock.sec > penalty_time_barrier)
	{
		time_of_detonation -= penalty_time;
		save_detonation_time();
	}
	// save this time in eeprom
	save_detention_end_time();
	// progress |= 1<<6;
	//save_progress();
}

uint32_t detention_log_time = 0;

void detention_loop()
{
	if (detention_log_time <= millis())
	{
		detention_log_time = millis() + 1000;
		lcd_write("Eszkoz zarolva!", time_to_string(detention_end - clock.sec));
	}
	// log("you are in Detention");
	// log("time left "+String(detention_end-clock.sec));

	// check time for penalty
	if (detention_end <= clock.sec)
	{
		// end of detention
		// progress &=  ~(1<<6); //deactivate detention
		// save_progress();
		switch_state(enter_detention_from_state);
	}
}

uint32_t next_motion_detect_time = 0;
bool detect_motion()
{
	if (digitalRead(MOTION_SENSOR))
	{
		if (next_motion_detect_time < millis())
		{
			next_motion_detect_time = millis() + 900000;
			return true;
		}
	}
	return false;
}

void explosion_loop()
{
	log("LOG: FIREWORKS ACTIVATED");
	// turn on petarda
	digitalWrite(PIROTECH, HIGH);
	lcd_isdelay=false;
	while (true)
	{
		lcd_write("BOOM A Bomba", " felrobbant!!!");
		delay(1000);
		lcd_write("################", "################");
		delay(1000);
	}
}

#include <EEPROM.h>
// i byte
void save_progress()
{
	log("SAVE: progress "+String(progress,2));
	EEPROM.write(0, progress);
}
// 4 byte
void save_detonation_time()
{
	log("SAVE: time of detonation "+String(time_of_detonation));
	EEPROM.write(1, time_of_detonation);
	EEPROM.write(2, time_of_detonation >> 8);
	EEPROM.write(3, time_of_detonation >> 16);
	EEPROM.write(4, time_of_detonation >> 24);
}
// 4byte
void save_detention_end_time()
{
	log("SAVE: detention end time "+String(detention_end));
	EEPROM.write(5, detention_end);
	EEPROM.write(6, detention_end >> 8);
	EEPROM.write(7, detention_end >> 16);
	EEPROM.write(8, detention_end >> 24);
}
