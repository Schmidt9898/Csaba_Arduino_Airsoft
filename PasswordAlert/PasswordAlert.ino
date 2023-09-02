/**
 * @brief 
 * 
 */

#include "log.h"
#include "pitches.h"
#include <Keypad.h>
//#include  <virtuabotixRTC.h>

#define INSTALL_PIN 11
#define INSTALL_TIME 10L * 60L * 1000L
#define START_WAIT_BEFORE_MOTION 5L * 1000L
#define WAIT_BEFORE_START_ALERT 15L * 1000L
#define WAIT_BEFORE_NEXT_START_CYCLE 10L*60L*1000L


#define HANG_1 A0
#define HANG_2 A1
#define HANG_3 A2
#define HANG_4 A3
#define HANG_N 4

#define MOTION_SENSOR A4
#define BUZZER A5

#define LED_GREEN A6
#define LED_RED A7

#define BEEP_CLICK 0
#define BEEP_START_MELODY 1
#define BEEP_INSTALL_TICK 2
#define BEEP_WRONG_PASS 3
#define BEEP_GOOD_PASS 4

void beep(uint8_t i);

void init_audio();
/**
 * @brief Playing audio need atlest 200 ms active low to activate
 * 
 * @param i audio pin to play
 */
void play_audio(int i);


#define ON true
#define OFF false
struct LED {
	short pin;
	bool isOn=ON;
	void turn(bool b){isOn=b;digitalWrite(pin,b);};
	void toggle(){turn(!isOn);}
	LED(int pin_):pin(pin_) {pinMode(pin, OUTPUT);};
	LED(){pin=-1;};
};


void install_loop();
/**
 * @brief Gets new key input and returns -1 if wrong pass was entered
 * 1 if valid pass was entered
 * 0 otherwise
 * 
 * @return int 
 */
int check_password();

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {3, 4, 5, 6}; //2-7-6-4 LÁBAK a D3-D4-D5-D6 PORTRA
byte colPins[COLS] = {7, 8, 9}; //3-1-5 LÁBAK a D7-D8-D9 PORTRA

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const String secret_pass = "789"; 
String input_password = "";

LED LED_G(LED_GREEN);
LED LED_R(LED_RED);

void setup()
{
	init_log; // Initialize serial communications with the PC, cn be turned off by define
	init_audio();
	pinMode(BUZZER, OUTPUT);

	pinMode(MOTION_SENSOR, INPUT);
	pinMode(INSTALL_PIN, INPUT);
	
	install_loop();

	LED_G.turn(ON);
	LED_R.turn(OFF);

	//program start signal
}

void loop()
{
	//Start program
	logn("Program start!");
	beep(BEEP_START_MELODY); // 2 sec
	LED_G.turn(ON);
	LED_R.turn(OFF);
	play_audio(HANG_1);

	delay(START_WAIT_BEFORE_MOTION);

	while(!digitalRead(MOTION_SENSOR)){delay(50);}; // Wait for motion

	bool is_alert_active = false;
	uint32_t next_alert_time = millis() + WAIT_BEFORE_START_ALERT;
	uint32_t next_led_togle = millis();

	while(true)
	{
		int val = check_password(); 
		if( val == -1) // wrong pass
		{
			//start HANG_4 alert
			beep(BEEP_WRONG_PASS);
			next_alert_time = millis();
		}else if (val == 1)
		{
			beep(BEEP_GOOD_PASS);
			break;
		}
		if(next_alert_time <= millis())
		{
			LED_G.turn(OFF); // REMOVABLE just for show
			is_alert_active = true;
			next_alert_time += millis() + 29 * 1000;
			play_audio(HANG_4);
		}
		if(is_alert_active)
		{
			if(next_led_togle <= millis()){
				LED_R.toggle();
				next_led_togle = millis() + 675;
			}
		}
		delay(50);
	}
	// Pass is good 
	play_audio(HANG_3);
	LED_G.turn(ON);
	LED_R.turn(OFF);
	delay(WAIT_BEFORE_NEXT_START_CYCLE);
}

void init_audio(){
	for(int i = HANG_1;i<HANG_1+HANG_N;i++)
	{
		pinMode(i, OUTPUT);
		digitalWrite(i,HIGH); // set to 5 volt 
	}
	logn("Audio player initialized!");
}
void play_audio(int i){
	log("play audio: "+String(i-54));
	digitalWrite(i,LOW);
	delay(200);
	digitalWrite(i,HIGH);
}

void install_loop()
{
	if(digitalRead(INSTALL_PIN) != LOW)return;

							//	 min  sec  millis
	uint32_t end_time = millis() + INSTALL_TIME;

	logn("Install cycle!");
	LED_R.turn(ON);

	while(millis()<end_time)
	{
		//wait do some ticking
		delay(200);
		beep(BEEP_INSTALL_TICK);
		LED_G.toggle();
		LED_R.toggle();
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

void beep(uint8_t i)
{
if (i == BEEP_CLICK){
	tone(BUZZER, 1000, 100);
}
else if (i == BEEP_START_MELODY){
	tone( BUZZER,NOTE_C5,400);delay(400); 
	tone( BUZZER,NOTE_D5,400);delay(400); 
	tone( BUZZER,NOTE_E5,400);delay(400); 
	tone( BUZZER,NOTE_G5,400);delay(400); 
	tone( BUZZER,NOTE_F5,400);delay(400); 
	tone( BUZZER,NOTE_E5,400);//delay(400); 
}
else if (i == BEEP_INSTALL_TICK){
	tone(BUZZER, 808, 100);
}
else if (i == BEEP_WRONG_PASS){
	tone( BUZZER,NOTE_E2,100);delay(100);
	tone( BUZZER,NOTE_D2,100);delay(100);
	tone( BUZZER,NOTE_C2,500);//delay(500);
}
else if (i == BEEP_GOOD_PASS){
	tone( BUZZER,NOTE_C6,100);delay(100);
	tone( BUZZER,NOTE_D6,100);delay(100);
	tone( BUZZER,NOTE_E6,500);//delay(500);
}

/* 	if (i == 0)
	else if (i == 1)
		tone(BUZZER, 1000, 200);
	else if (i == 2)
		tone(BUZZER, 808, 100);
	else if (i == 3)
		tone(BUZZER, 404, 500);
	else if (i == 4)
		tone(BUZZER, 808, 1000); */
}
