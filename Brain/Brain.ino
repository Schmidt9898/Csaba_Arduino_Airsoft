

#define DEMO
#define DEBUG  //comment this line out if you dont want log, also can reduce programsize

#ifdef DEMO
#define penalty_time_barrier 3599 
#else
#define penalty_time_barrier 432000 
#endif

#define MOTION_SENSOR 6
#define PIROTECH A0

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

long detention_time = 10; //in second
long penalty_time = 3600; //in second this will subtract from time 1 hour
// 2022.07.15 20:00:00 prepare to meet your doom
long time_of_detonation = 39800; // time of detonation, save it in eeprom do not change
//long time_of_detonation = 1657908000; // time of detonation, save it in eeprom do not change 
//int //some game timer til boom 
Clock clock(4,5);
int time_morning=8;
int time_night=20;


//these wariables need saave to eeprom
//uint32_t countdown_time=200;
byte progress=0;//TODO saveMOTION_SENSOR
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

void setup()
{
	#ifdef DEBUG
	Serial.begin(9600);
	#endif

	//load eeprom data
	//if some pin is low reset saved state to default
	//eeprom has a lifecycle of 100,000 write/delete 
	//save only on reset, day,night,penalty state
	//save on milestone comlete
	//save every 1 hour and penalty time


	lcd_init();
	gyro.init();

	init_music_player(40,1);		
	pinMode(MOTION_SENSOR, INPUT);//may need resistor 10k
	pinMode(PIROTECH, OUTPUT);//bomba
	pinMode(card_reader_input_pin, INPUT);
	pinMode(card_reader_activate_pin, OUTPUT); //HIGH is accept
	digitalWrite(card_reader_activate_pin,LOW);

	digitalWrite(PIROTECH,LOW);
	

	
	
		


	leds[0]=LED(50);
	leds[1]=LED(51);
	leds[2]=LED(52);
	leds[3]=LED(53);
	
	progress=0b00000000;

	//log("progress: "+String(progress,2));

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
	if(progress & (1<<6))
		switch_state(State::Detention);

	clock.refresh();
	//detention_end=clock.sec+detention_time;

	mini_games[0]=Mini_game{22,23,24,progress & (1<<0)};
	mini_games[1]=Mini_game{25,26,27,progress & (1<<1)};
	mini_games[2]=Mini_game{28,29,30,progress & (1<<2)};
	mini_games[3]=Mini_game{31,32,33,progress & (1<<3)};

	for(int i=0;i<4;i++)
	{
		mini_games[i].init();
		//mini_games[i].activate();
	}
//EEPROM.write(0,)


	//did_gyroscope_move(); //not here

	//tone(52, 1000,200);

	if(state != State::Detention)
		switch_state(state);
}


void detention_loop();
void day_loop();
void night_loop();
void card_loop();
void pinpad_loop();
void final_pinpad_loop();

void explosion_loop();





long next_music_play_time=180000;//3 min


void loop()
{
// state independent tasks
clock.refresh();
	//log("Time is: "+clock.get_time());
	log("T-"+time_to_string(time_of_detonation-clock.sec));
if(time_of_detonation<=clock.sec)
{
	//well boom
	explosion_loop();
}


	if(gyro.did_gyroscope_move())
	{
		//TODO
		log("elmozdultam");
		add_penalty();
		switch_state(State::Detention);


	}

if(clock.isDay() && detect_motion())
{

	if(state==State::Detention)
		play_music(m_cant_play);
	else{
		play_music(m_welcome);
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
	delay(1000);
	//tone(52, 349,200);
}



void switch_state(State next){
	if(next == state)
		{
			log("SAME STATE");
		//return;
		}

	//leave state events
	if(state==State::Day)
	{
		for(int i=0;i<4;i++)
			mini_games[i].deactivate();
	}
	if(state==State::Wait_card)
	{
		digitalWrite(card_reader_activate_pin,LOW);
	}

	
	State from = state;//save last state
	state=next;

	//enter state events

	switch (state)
		{
			case State::Day: 
				log("Entering Day");
				//lcd_write("Day");
				for(int i=0;i<4;i++)
					mini_games[i].activate();
				break;
			case State::Night: 
				log("Entering Night");
				lcd_write("Night");
				enter_night_from_state=from;
				break;
			case State::Wait_card: 
				log("Entering Wait_card");
				digitalWrite(card_reader_activate_pin,HIGH);
				break;
			case State::Wait_pin: 
				log("Entering Wait_pin");
				break;
			case State::Detention: 
				log("Entering Detention");
				enter_detention_from_state=from;
				break;
		}
};




void day_loop(){
		log("Day");
		
		
		lcd_write("Day  "+clock.get_time(),"T-"+time_to_string(time_of_detonation-clock.sec));


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

			if(t_state==Mini_game_state::Unknown)
				log("game state is unknown");
			else if (t_state==Mini_game_state::Good){
				log("game state is good");
				leds[i].turn(1);
				progress = progress | 1<<i;
				//TODO save progress eeprom
			} 
			else if (t_state==Mini_game_state::Bad)
			{
				log("game state is bad");
				add_penalty();
				switch_state(State::Detention);
				return;
			}
			else{
				//log("what?????????");

			}
		}
		//switch_state(State::Night);
}
void night_loop(){
		log("Night");
		if(clock.isDay())
			{
				log("let's wake up");
				//wake up the system
				switch_state( enter_night_from_state );
				return;
			}
		//switch_state(State::Detention);
}
void card_loop(){
		log("Wait_card");//TODO
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
		switch_state(State::Wait_pin);
		progress = progress | 1<<4;//todo save eeprom
		}


}
void pinpad_loop(){
		log("Wait_pin");//TODO
		if(!clock.isDay())
			{
				log("pack for the night");
				//pack for the night
				switch_state(State::Night);
				return;
			}
		switch_state(State::Day);
		progress = progress | 1<<5;//todo save eeprom
}
void final_pinpad_loop(){
		log("final pinpad loop");//TODO
		if(!clock.isDay())
			{
				log("pack for the night");
				//pack for the night
				switch_state(State::Night);
				return;
			}
		//switch_state(State::Day);
		//progress = 255;//todo save eeprom

}


//TODO add penalty
void add_penalty(){
	detention_end=clock.sec+detention_time;
	if(time_of_detonation-clock.sec > penalty_time_barrier)
	{
		time_of_detonation-=penalty_time;
	}

	
	//save this time in eeprom
}
void detention_loop(){
	log("you are in Detention");
	lcd_write("time left",String(detention_end-clock.sec));
	log("time left "+String(detention_end-clock.sec));
	
	//check time for penalty
	if(detention_end<= clock.sec)
	{
	switch_state(enter_detention_from_state);
	}



}



long next_motion_detect_time=0;
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



