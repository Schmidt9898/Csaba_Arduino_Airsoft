

#define DEMO
#define DEBUG  //comment this line out if you dont want log, also can reduce programsize

#define MOTION_SENSOR 51



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
unsigned int countdown_time=200;

LED leds[4];

void setup()
{
	#ifdef DEBUG
	Serial.begin(9600);
	#endif

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
	//did_gyroscope_move(); //not here

	//tone(52, 1000,200);


}

void loop()
{
// state independent tasks
clock.refresh();
	log(clock.get_time());
//state switch
switch (state)
		{
		case State::Day: 
		log("Day");
		switch_state(State::Night);
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








