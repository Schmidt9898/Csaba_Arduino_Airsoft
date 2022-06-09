


#define DEBUG  //comment this line out if you dont want log, also can reduce programsize

#include "log.h"
#include "clock.h"

enum State{
	None=0, Night=1, Detention=400, Wait_card=2, Wait_pin=3, Day=4
};
void switch_state(State next);
State state= State::None;

unsigned int detention_time = 10; //in second
//int //some game timer til boom 
Clock clock;




void setup()
{
	#ifdef DEBUG
	Serial.begin(9600);
	#endif

	//check everithing is working
	//if not warning sound
	//check what time it is
	state=State::Wait_card;
	//tone(52, 1000,200);


}

void loop()
{
// state independent tasks
	log(clock.get_time());
//state switch
switch (state)
		{
		log("Day");
		case State::Day: 
		switch_state(State::Night);
		break;
		case State::Night: 
		log("Night");
		switch_state(State::Detention);
		break;
		case State::Wait_card: 
		log("Wait_card");
		switch_state(State::Wait_pin);
		break;
		case State::Wait_pin: 
		log("Wait_pin");
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








