#include <EEPROM.h>

#define BTN_0 A1
#define BTN_1 A1
#define BTN_2 A1
#define BTN_3 A1
#define BTN_RESET A1
#define BTN_ENTER A1

#define LED_STATUS A0

#define BUZZER A2

#define TRAC_A1 A0
#define TRAC_A2 A0
#define TRAC_A3 A0
#define TRAC_A4 A0
#define TRAC_A5 A0
#define TRAC_A6 A0
#define TRAC_A7 A0
#define TRAC_A8 A0
#define TRAC_A9 A0

#define TRAC_LEN_MS_A1 4000
#define TRAC_LEN_MS_A2 1000
#define TRAC_LEN_MS_A3 1000
#define TRAC_LEN_MS_A4 1000
#define TRAC_LEN_MS_A5 1000
#define TRAC_LEN_MS_A6 1000
#define TRAC_LEN_MS_A7 1000
#define TRAC_LEN_MS_A8 1000
#define TRAC_LEN_MS_A9 4000

#define CODE_LEN 6

#define PENALTY_TIME_MIN 0

uint8_t penalty_time_left = 0;

void beep()
{
  tone(BUZZER, 500, 100);
  noTone(BUZZER);
}


void play_audio(unsigned int pin,unsigned int duration){
  digitalWrite(pin, LOW);
  delay(50);
  digitalWrite(pin, HIGH);
  delay(duration);
}


uint8_t cur_code[CODE_LEN];
uint8_t cur_code_idx = 0;


uint8_t code1[CODE_LEN] = {0,0,0,0,0,0};
uint8_t code2[CODE_LEN] = {0,0,0,0,0,0};
uint8_t code3[CODE_LEN] = {0,0,0,0,0,0};
uint8_t code4[CODE_LEN] = {0,0,0,0,0,0};
uint8_t code5[CODE_LEN] = {0,0,0,0,0,0};
uint8_t code6[CODE_LEN] = {0,0,0,0,0,0};
uint8_t code7[CODE_LEN] = {0,0,0,0,0,0};




void setup() {

  EEPROM.get(0,penalty_time_left);
  penalty_state();

  Serial.begin(9600);

	pinMode(BUZZER, OUTPUT);
	noTone(BUZZER);

  pinMode(BTN_0, INPUT_PULLUP);
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
  pinMode(BTN_3, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(BTN_ENTER, INPUT_PULLUP);


  pinMode(TRAC_A1, OUTPUT);
  pinMode(TRAC_A2, OUTPUT);
  pinMode(TRAC_A3, OUTPUT);
  pinMode(TRAC_A4, OUTPUT);
  pinMode(TRAC_A5, OUTPUT);
  pinMode(TRAC_A6, OUTPUT);
  pinMode(TRAC_A7, OUTPUT);
  pinMode(TRAC_A8, OUTPUT);
  pinMode(TRAC_A9, OUTPUT);
  digitalWrite(TRAC_A1, HIGH);
  digitalWrite(TRAC_A2, HIGH);
  digitalWrite(TRAC_A3, HIGH);
  digitalWrite(TRAC_A4, HIGH);
  digitalWrite(TRAC_A5, HIGH);
  digitalWrite(TRAC_A6, HIGH);
  digitalWrite(TRAC_A7, HIGH);
  digitalWrite(TRAC_A8, HIGH);
  digitalWrite(TRAC_A9, HIGH);

  pinMode(LED_STATUS, OUTPUT);


  digitalWrite(LED_STATUS, 1);
  for(short i = 0; i < 2; i++)
  {
    delay(500);
    digitalWrite(LED_STATUS, 0);
    delay(500);
    digitalWrite(LED_STATUS, 1);
  }

  play_audio(TRAC_A1,TRAC_LEN_MS_A1);
}

void penalty_state()
{
  Serial.println("penalty_state");
  for(penalty_time_left; penalty_time_left > 0;penalty_time_left--)
  {
    delay(60000);
    EEPROM.write(0, penalty_time_left);
  }
}

void code_error(){
  Serial.println("code_error");
  play_audio(TRAC_A2,TRAC_LEN_MS_A2);
  penalty_time_left = PENALTY_TIME_MIN;
  EEPROM.write(0, penalty_time_left);
  penalty_state();
}


void loop() {

if (cur_code_idx < CODE_LEN)
{
if(!digitalRead(BTN_0)){
  cur_code[cur_code_idx++] = 0;
  beep();
}
if(!digitalRead(BTN_1)){
  cur_code[cur_code_idx++] = 1;
  beep();
}
if(!digitalRead(BTN_2)){
  cur_code[cur_code_idx++] = 2;
  beep();
}
if(!digitalRead(BTN_3)){
  cur_code[cur_code_idx++] = 3;
  beep();
}
}



if(!digitalRead(BTN_RESET)){
  cur_code_idx = 0;
  beep();
}
if(!digitalRead(BTN_ENTER)){
  beep();

  if (cur_code_idx < CODE_LEN)
  {
    code_error();return;
  }

  if (memcmp(cur_code, code1, sizeof(code1)) == 0){
    play_audio(TRAC_A3,TRAC_LEN_MS_A3);
  }else if(memcmp(cur_code, code1, sizeof(code2)) == 0){
    play_audio(TRAC_A4,TRAC_LEN_MS_A4);
  }else if(memcmp(cur_code, code1, sizeof(code3)) == 0){
    play_audio(TRAC_A5,TRAC_LEN_MS_A5);
  }else if(memcmp(cur_code, code1, sizeof(code4)) == 0){
    play_audio(TRAC_A6,TRAC_LEN_MS_A6);
  }else if(memcmp(cur_code, code1, sizeof(code5)) == 0){
    play_audio(TRAC_A7,TRAC_LEN_MS_A7);
  }else if(memcmp(cur_code, code1, sizeof(code6)) == 0){
    play_audio(TRAC_A8,TRAC_LEN_MS_A8);
  }else if(memcmp(cur_code, code1, sizeof(code7)) == 0){
    play_audio(TRAC_A9,TRAC_LEN_MS_A9);
  }else{
    code_error();
  }
  cur_code_idx = 0;
  return;
}
delay(100);
}
