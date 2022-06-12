

/*#define DEMO*/
#define DEBUG  //comment this line out if you dont want log, also can reduce programsize

#ifdef DEMO
#define penalty_time_barrier 3599 
#else
//#define penalty_time_barrier 3599
#define penalty_time_barrier three_day_in_sec  
#endif

#define MOTION_SENSOR A2
#define PIROTECH 48

#define BUZZER 49

#define card_reader_input_pin 3
#define card_reader_activate_pin 2

#include <EEPROM.h>

#include "log.h"
#include "clock.h"
#include "display.h"

enum State{
	Night=1, Detention=6, Wait_card=2, Wait_pin=3, Day=4, Wait_final_pin=5,error=404
	//Night="night", Detention="detention", Wait_card="card", Wait_pin="pin", Day="day", error="error"
};
void switch_state(State next);
State state;
State enter_detention_from_state;
State enter_night_from_state;

uint32_t detention_time = 1800; //in second
uint32_t penalty_time = 3600; //in second this will subtract from time 1 hour
// 2022.07.15 20:00:00 prepare to meet your doom
uint32_t time_of_detonation; // time of detonation, save it in eeprom can't change here
//uint32_t time_of_detonation = 1657908000; // time of detonation, save it in eeprom do not change 

Clock clock;
int time_morning=8;
int time_night=20;

const String first_pass="0000";
const String final_pass="1111";
String input_password=""; //don't change


byte progress=0;
//	1bit popszeg 1 bit detention,2 bit sate, 4 bit games, 
//	10100101
//	00000100 1<<2
//	00000100
//	progress & (1<<2)

//2 bit card and pin
//2 bit last state 


LED leds[4];
Mini_game mini_games[4];
Gyoscope gyro;



#ifdef DEMO
#define NO_KEY  '\0'
struct Keypad{
	int next=0;
	int count=0;
	char getKey(){
		if(millis()>next){
			count++;
			next=millis()+1000;
			if(count==5)
			{
				count=0;
				return '#';
			}
			return 'a';
		}
		return NO_KEY;
	}
}keypad;
#else
#include <Keypad.h>

const byte ROWS = 4, COLS = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {10,11,12,13}; //2-7-6-4 LÁBAK a D10-D11-D12-D13 PORTRA
byte colPins[COLS] = {7, 8, 9}; //3-1-5 LÁBAK a D7-D8-D9 PORTRA
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
#endif



void setup()
{
	#ifdef DEBUG
	Serial.begin(9600);
	#endif
	//log("detonation time before save: "+time_to_string(time_of_detonation));
	/*
	{
		time_of_detonation = 1655142221; // ide írd az időt
		log(String(time_of_detonation,2));
		detention_end=0;
		progress=0b00000000;
		save_detention_end_time();
		save_detonation_time();
		save_progress();
		beep(1);
		delay(100000);
	}
	*/



	//load eeprom data
	//if some pin is low reset saved state to default
	//eeprom has a lifecycle of 100,000 write/delete 
	//save only on reset, day,night,penalty state
	//save on milestone comlete
	//save every 1 hour and penalty time


	lcd_init();
 //lcd_write("ez egy szoveg","ez a masik");
 //delay(100000);
	lcd_write("initializing"," .........");
	led_segment_init();
	led_segment(0);//may remove

	gyro.init();

	init_music_player();	

  //play_music(4);
 



	pinMode(MOTION_SENSOR, INPUT);//may need resistor 10k
	pinMode(PIROTECH, OUTPUT);//bomba
	pinMode(card_reader_input_pin, INPUT);
	pinMode(card_reader_activate_pin, OUTPUT); //HIGH is accept
	digitalWrite(card_reader_activate_pin,LOW);

	digitalWrite(PIROTECH,LOW);
	
	pinMode(BUZZER, OUTPUT);
	noTone(BUZZER);

	beep(0);
	delay(500);
	beep(0);
	delay(500);
	beep(1);
	delay(200);
	
		


	leds[0]=LED(50);
	leds[1]=LED(51);
	leds[2]=LED(52);
	leds[3]=LED(53);
	
	//load eeprom saved data
	progress=EEPROM[0];
	time_of_detonation = ((uint32_t)EEPROM[1]);
	time_of_detonation|= ((uint32_t)EEPROM[2]) << 8;
	time_of_detonation|= ((uint32_t)EEPROM[3]) << 16;
	time_of_detonation|= ((uint32_t)EEPROM[4]) << 24;

	detention_end = ((uint32_t)EEPROM[5]);
	detention_end|= ((uint32_t)EEPROM[6]) << 8;
	detention_end|= ((uint32_t)EEPROM[7]) << 16;
	detention_end|= ((uint32_t)EEPROM[8]) << 24;

	log("progress: "+String(progress,2));
	lcd_write("progress: ",String(progress,2));

	log("detonation time: "+time_to_string(time_of_detonation));
	log("detonation time: "+String(time_of_detonation));
	log(String(time_of_detonation,2));
	log("loaded detention: "+time_to_string(detention_end));

log("Start");
if(!(progress & 1<<7))
{
	log("wait for first start");
  lcd_write("1 min to save");
	delay(60000);
	//first start 
	//kell idő a beüzemelésre , lecsavarozásra és elhagyásra
	//1 órát várunk ilyenkor
	progress|= 1<<7;
	save_progress();
	log("It is saved");

	//delay(3600000);// 1 hour delay
	//delay(60000);// 1 min delay
	//uint32_t end_bolting=millis()+3600000;
	uint32_t end_bolting=millis()+60000;
	while(millis()<end_bolting){
		beep(2);
		lcd_write("Telepites!",time_to_string((end_bolting-millis())/1000));
		delay(1000);
	}

}


	/*
	leds[0].turn(1);
	delay(1000);
	leds[1].turn(1);
	delay(1000);
	leds[2].turn(1);
	delay(1000);
	leds[3].turn(1);
	delay(2000);
	leds[0].turn(0);
	leds[1].turn(0);
	leds[2].turn(0);
	leds[3].turn(0);

	//check everithing is working
	//if not warning sound
	//check what time it is
	

	//lcd_write("Üdvozöllek"," Stalker");
	//play_music(0);
	//delay(1000);
	//lcd_write("aaaaaa","bbbbbbbb");

	*/

	state=State::Wait_card;

	if(progress & (1<<4)){
		log("card is done by save");
		state=State::Wait_pin;
	}
	if(progress & (1<<5)){
		log("pin is done by save");
		state=State::Day;
	}
	
	switch_state(State::Detention);

	clock.refresh();
	//detention_end=clock.sec+detention_time;

	leds[0].turn(progress & (1<<0));
	leds[1].turn(progress & (1<<1));
	leds[2].turn(progress & (1<<2));
	leds[3].turn(progress & (1<<3));


	mini_games[0]=Mini_game{22,23,24,progress & (1<<0)};
	mini_games[1]=Mini_game{25,26,27,progress & (1<<1)};
	mini_games[2]=Mini_game{28,29,30,progress & (1<<2)};
	mini_games[3]=Mini_game{31,32,33,progress & (1<<3)};

	for(int i=0;i<4;i++)
	{
		mini_games[i].init();
		//mini_games[i].activate();
	}

	//if(state != State::Detention)
	//	switch_state(state);
}


void detention_loop();
void day_loop();
void night_loop();
void card_loop();
void pinpad_loop();
void final_pinpad_loop();

void explosion_loop();





uint32_t next_music_play_time=180000;//3 min


uint32_t next_log_time=0;

void loop()
{
// state independent tasks
clock.refresh();
	//log("Time is: "+clock.get_time());
	if(next_log_time<=millis())
	{
		next_log_time=millis()+1000; //TODO change to 500
		led_segment(time_of_detonation-clock.sec);
		//log("det time :"+String(time_of_detonation));
		//log("time:"+String(clock.sec)+"     T-"+time_to_string(time_of_detonation-clock.sec));
	}
if(time_of_detonation<=clock.sec)
{
	//well boom
	explosion_loop();
}


	if(gyro.did_gyroscope_move())
	{
		log("elmozdultam");
		add_penalty();
		switch_state(State::Detention);
   play_music(m_move);
	}

if(clock.isDay() && detect_motion())
{

	if(state==State::Detention)
		{
    //play_music(m_cant_play);
		  }
	else{
		play_music(m_welcome);
		lcd_write("Udvozollek!");
		next_music_play_time=millis()+180000;
	}
}

if(clock.isDay() && state!= State::Detention)
{
	if(next_music_play_time<millis())
	{
		play_music(m_music);
		next_music_play_time=millis()+300000;
	}
}


//state switch
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
/*
if(digitalRead(MOTION_SENSOR))
	lcd_write("most");
	else
	lcd_write("most nem");
*/
	delay(100);
	//tone(52, 349,200);
}



void switch_state(State next){
	if(next == state)
		{
			log("SAME STATE");      
		//return;
		}

	//leave state events
    if(state==State::Detention && next == State::Detention)
    {
      return;
    }


  
	if(state==State::Day)
	{
		for(int i=0;i<4;i++)
			mini_games[i].deactivate();
	}
	if(state==State::Wait_card)
	{
		digitalWrite(card_reader_activate_pin,LOW);
	}

	
	State from = state;
	state=next;

	//enter state events

	switch (state)
		{
			case State::Day: 
				log("Entering Day");
				lcd_write("Udvozollek! *_* "," Modulok aktivak");
				for(int i=0;i<4;i++)
					mini_games[i].activate();
				break;
			case State::Night: 
				log("Entering Night");
				lcd_write("Modulok Zarolva","Gyere vissza 800");
				enter_night_from_state=from;
				break;
			case State::Wait_card: 
				log("Entering Wait_card");
				lcd_write("Add meg a"," kartyat");
				digitalWrite(card_reader_activate_pin,HIGH);
				break;
			case State::Wait_pin: 
				input_password="";
				log("Entering Wait_pin");
				lcd_write("Add meg a"," pinkodot");
				break;
			case State::Wait_final_pin: 
				input_password="";
				lcd_write("Add meg a"," pinkodot");
				log("Entering Wait_final_pin");
				break;
			case State::Detention: 
				log("Entering Detention");
				enter_detention_from_state=from;
				break;
		}
};




void day_loop(){
		//log("Day");
		
		
		//lcd_write("Day  "+clock.get_time(),"T-"+time_to_string(time_of_detonation-clock.sec));


		if(!clock.isDay())
			{
				log("pack for the night");
				//pack for the night
				switch_state(State::Night);
				return;
			}
		for(int i=0;i<4;i++)
		{
			auto t_state=mini_games[i].get_state();

			if(t_state==Mini_game_state::Unknown){
				//log("game state is unknown");

			}
			else if (t_state==Mini_game_state::Good){
				log("game state is good");
				leds[i].turn(1);
				progress = progress | 1<<i;
				save_progress();
			} 
			else if (t_state==Mini_game_state::Bad)
			{
				log("game state is bad");
				add_penalty();
				switch_state(State::Detention);
       play_music(m_failed_attempt);
				return;
			}
			else{
				//log("what?????????");

			}
		}
		//ellenörzés
		bool check=true;
		for(int i=0;i<4;i++)
		{
			check = check && mini_games[i].solved;
		}
		if(check)
		{
			//all game is passed
			log("all game is passed");
			lcd_write("Minden modul ","  Sikeres!!");//TODO
			play_music(m_all_game_passed);
			switch_state(State::Wait_final_pin);
		}
		//switch_state(State::Night);
}
void night_loop(){
		//log("Night");
		if(clock.isDay())
			{
				log("it is morning, let's wake up");
				//wake up the system
				switch_state( enter_night_from_state );
				return;
			}
		//switch_state(State::Detention);
}
void card_loop(){
		if(!clock.isDay())
			{
				log("pack for the night");
				//pack for the night
				switch_state(State::Night);
				return;
			}
		

		if(digitalRead(card_reader_input_pin))
		{
		//all good, save state and next phase
		log("card is good");
		play_music(m_card_accept);
		progress = progress | 1<<4;
		save_progress();
		switch_state(State::Wait_pin);
		}


}


void pinpad_loop(){
		if(!clock.isDay())
			{
				log("pack for the night");
				//pack for the night
				switch_state(State::Night);
				return;
			}


		char key = keypad.getKey();
		if(key)
		{
			beep(0);
			if (key == '*') {//clear pass
      			input_password = "";
				//cursor

			}
    		else if (key == '#') {
				//enter
				if(input_password==first_pass)
				{
					input_password="";
					play_music(m_pass_good);
					switch_state(State::Day);
					progress = progress | 1<<5;
					save_progress();
				}else{
					input_password="";
					play_music(m_failed_attempt);
					add_penalty();
					switch_state(State::Detention);
				}
			}else
			{
				input_password+=key;
				lcd_write("Kerem a jelszot:",input_password);
			}
		}
}
void final_pinpad_loop(){
		if(!clock.isDay())
			{
				log("pack for the night");
				//pack for the night
				switch_state(State::Night);
				return;
			}


		char key = keypad.getKey();
		if(key)
		{
			beep(0);
			if (key == '*') {//clear pass
      			input_password = "";
				//cursor

			}
    		else if (key == '#') {
				//enter
				if(input_password==final_pass)
				{
					input_password="";
					play_music( m_defusing_complete);
					progress = 255;
					save_progress();
					lcd_write("Gratulalunk!"," Hatastalanitva!");
					while(1);
					//switch_state(State::Day);
				}else{
					input_password="";
					play_music(m_failed_attempt);
					add_penalty();
					switch_state(State::Detention);
				}
			}else
			{
				input_password+=key;
				lcd_write("Kerem a jelszot:",input_password);
			}
		}

}


void add_penalty(){
	detention_end=clock.sec+detention_time;
 //log(time_of_detonation-clock.sec);
 //log("####");
 //log(penalty_time);
	if(time_of_detonation-clock.sec > penalty_time_barrier)
	{
		time_of_detonation-=penalty_time;
		save_detonation_time();
	}
	//save this time in eeprom
	//save_detention_end_time();
	//progress |= 1<<6;
	save_progress();

	
}

uint32_t detention_log_time=0;


void detention_loop(){
	if(detention_log_time<=millis())
	{
		detention_log_time=millis()+1000;
		lcd_write("Eszkoz zarolva!",time_to_string(detention_end-clock.sec));
	}
	//log("you are in Detention");
	//log("time left "+String(detention_end-clock.sec));
	
	//check time for penalty
	if(detention_end<= clock.sec)
	{
	//end of detention
	//progress &=  ~(1<<6); //deactivate detention
	//save_progress();
	switch_state(enter_detention_from_state);
	}
}



uint32_t next_motion_detect_time=0;
bool detect_motion()
{
	if(digitalRead(MOTION_SENSOR))
	{
		if(next_motion_detect_time<millis())
		{
		next_motion_detect_time=millis()+900000;
		return true;
		}
	}
	return false;
}




void explosion_loop(){
	//turn on petarda
	digitalWrite(PIROTECH,HIGH);
	while (true)
	{
		lcd_write("BOOM A Bomba"," felrobbant!!!");
		delay(1000);
		lcd_write("##############","############");
		delay(1000);
	}
	
}


#include <EEPROM.h>
//i byte
void save_progress(){

EEPROM.write(0, progress);

}
//4 byte
void save_detonation_time()
{
EEPROM.write(1, time_of_detonation);
EEPROM.write(2, time_of_detonation >> 8);
EEPROM.write(3, time_of_detonation >> 16);
EEPROM.write(4, time_of_detonation >> 24);
}
//4byte
void save_detention_end_time()
{
EEPROM.write(5, detention_end);
EEPROM.write(6, detention_end >> 8);
EEPROM.write(7, detention_end >> 16);
EEPROM.write(8, detention_end >> 24);

}
