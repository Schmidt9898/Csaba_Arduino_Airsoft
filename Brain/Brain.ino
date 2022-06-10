

#define DEMO
#define DEBUG  //comment this line out if you dont want log, also can reduce programsize

#define MOTION_SENSOR 51

#include <EEPROM.h>

#include "log.h"
#include "clock.h"
#include "display.h"

enum State{
	None=0, Night=1, Detention=5, Wait_card=2, Wait_pin=3, Day=4, error=404
};
void switch_state(State next);
State state= State::None;

unsigned int detention_time = 10; //in second
unsigned int penalty_time = 10; //in second this will subtract from time

//int //some game timer til boom 
Clock clock;
//these wariables need saave to eeprom
uint32_t countdown_time=200;
byte progress=0;//TODO save
//	10100101
//	00000100 1<<2
//	00000100
//	progress & (1<<2)


LED leds[4];
Mini_game mini_games[4]{
Mini_game{22,23,24,progress & (1<<2)},
Mini_game{25,26,27,progress & (1<<3)},
Mini_game{28,29,30,progress & (1<<4)},
Mini_game{31,32,33,progress & (1<<5)},
};


void setup()
{
	#ifdef DEBUG
	Serial.begin(9600);
	#endif

	//load eeprom data
	//if some pin is high reset saved state to default
	//eeprom has a lifecycle of 100,000 write/delete 
	//save only on reset, day,night,penalty state
	//save on milestone comlete
	//save every 1 hour and penalty time


	lcd_init();

	leds[0]=LED(52);
	leds[0].turn(1);

	lcd_write("ez mas uzenet");
	delay(1000);
	lcd_write("aaaaaa","bbbbbbbb");

	leds[0].turn(0);

	//check everithing is working
	//if not warning sound
	//check what time it is
	state=State::Wait_card;

	pinMode(MOTION_SENSOR, INPUT);//may need resistor 10k

	init_music_player(40,1);
	play_music(0);

	init_gyroscope();

for(int i=0;i<4;i++)
{
	mini_games[i].init();
	mini_games[i].activate();
}
//EEPROM.write(0,)


	//did_gyroscope_move(); //not here

	//tone(52, 1000,200);


}

void loop()
{
// state independent tasks
clock.refresh();
	log(clock.get_time());
	did_gyroscope_move();

//state switch
switch (state)
		{
		case State::Day: 
		//log("Day");
		
		for(int i=0;i<4;i++)
		{
			auto t_state=mini_games[i].get_state();
			if(t_state==Mini_game_state::Unknown)
				log("game state is unknown");
			else if (t_state==Mini_game_state::Good)
				log("game state is good");
			else if (t_state==Mini_game_state::Bad)
				log("game state is bad");
			else
				log("what?????????");
		}
		//switch_state(State::Night);
		break;
		case State::Night: 
		log("Night");
		switch_state(State::Detention);
		break;
		case State::Wait_card: 
		log("Wait_card");//TODO
		switch_state(State::Wait_pin);
		break;
		case State::Wait_pin: 
		log("Wait_pin");//TODO
		switch_state(State::Day);
		break;
		case State::Detention: 
		log("Detention");
		break;
		case State::None:
		{

			break;
		}
		default:
			break;
		}

if(digitalRead(MOTION_SENSOR))
	lcd_write("most");
	else
	lcd_write("most nem");

	delay(500);
	//tone(52, 349,200);
}



void switch_state(State next){
	if(next == state)
		return;
	
	//leave state events

	state=next;

	//enter state events

	switch (state)
		{
			case State::Day: 
				log("Entering Day");
				break;
			case State::Night: 
				log("Entering Night");
				break;
			case State::Wait_card: 
				log("Entering Wait_card");
				break;
			case State::Wait_pin: 
				log("Entering Wait_pin");
				break;
			case State::Detention: 
				log("Entering Detention");
				break;
		}
};








