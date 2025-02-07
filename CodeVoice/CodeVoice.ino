#include <EEPROM.h>

#define BTN_0 2
#define BTN_1 3
#define BTN_2 4
#define BTN_3 5
#define BTN_RESET 6
#define BTN_ENTER 7

#define BTN_MEM_CLEAR BTN_0

#define LED_STATUS 10

#define BUZZER 9

#define TRAC_A1 A1
#define TRAC_A2 A2
#define TRAC_A3 A3
#define TRAC_A4 A4
#define TRAC_A5 A5
#define TRAC_A6 A6
#define TRAC_A7 A7
#define TRAC_A8 11
#define TRAC_A9 12

#define TRAC_LEN_MS_A1 1000   // code 1
#define TRAC_LEN_MS_A2 1000   // code 2
#define TRAC_LEN_MS_A3 1000   // code 3
#define TRAC_LEN_MS_A4 1000   // code 4
#define TRAC_LEN_MS_A5 1000   // code 5
#define TRAC_LEN_MS_A6 300000 // code 6 //5perc, 300mp, 300000 milisec
#define TRAC_LEN_MS_A7 4000   // welcome
#define TRAC_LEN_MS_A8 1000   // code error
#define TRAC_LEN_MS_A9 1000   // startup penalty

#define CODE_LEN 6

#define PENALTY_TIME_MIN 1

uint8_t code1[CODE_LEN] = {1,2,4,2,3,3};
uint8_t code2[CODE_LEN] = {1,2,3,4,2,2};
uint8_t code3[CODE_LEN] = {4,3,1,2,4,2};
uint8_t code4[CODE_LEN] = {0,0,0,0,0,0};
uint8_t code5[CODE_LEN] = {0,0,0,0,0,0};
uint8_t code6[CODE_LEN] = {1,1,1,1,1,1};


/*
----------------------------------------------------------------
----------------NO CHANGE AFTER THIS LINE-----------------------
----------------------------------------------------------------
*/

void beep()
{
  tone(BUZZER, 500, 100);
}

void play_audio(unsigned int pin,unsigned int duration){
  digitalWrite(pin, LOW);
  delay(100);
  digitalWrite(pin, HIGH);
  delay(duration);
}

uint8_t penalty_time_left = 0;

uint8_t cur_code[CODE_LEN];
uint8_t cur_code_idx = 0;

void print_code(uint8_t *arr)
{
  for(uint8_t i = 0; i < CODE_LEN; i++)
  {
    Serial.print(arr[i]);
  }
  Serial.print("\n");
}

void setup() {

  Serial.begin(9600);
  Serial.println("Init..");
  
  noTone(BUZZER);
  beep();
  delay(200);

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
  

  if(!digitalRead(BTN_MEM_CLEAR)){
    EEPROM.write(0, 0);
    Serial.println("Penalty cleared");
  } 
  EEPROM.get(0,penalty_time_left);
  if (penalty_time_left != 0)
  {
    // startup penalty
    play_audio(TRAC_A9,TRAC_LEN_MS_A9);
    penalty_state();
  }

  for(short i = 0; i < 2; i++)
  {
    delay(500);
    digitalWrite(LED_STATUS, 0);
    delay(500);
    digitalWrite(LED_STATUS, 1);
  }
  
  play_audio(TRAC_A7,TRAC_LEN_MS_A7);
}

void penalty_state()
{
  Serial.println("Entering penalty state.");
  for(penalty_time_left; penalty_time_left > 0;penalty_time_left--)
  {
    Serial.print(penalty_time_left);
    Serial.println(" minutes of penalry left");
    delay(60000);
    EEPROM.write(0, penalty_time_left);
  }
  Serial.println("Leaving penalty state.");
}

void code_error(){
  Serial.println("code_error");
  play_audio(TRAC_A8,TRAC_LEN_MS_A8);
  penalty_time_left = PENALTY_TIME_MIN;
  EEPROM.write(0, penalty_time_left);
  Serial.print(PENALTY_TIME_MIN);
  Serial.println(" minutes added to penalty");
  penalty_state();
}

bool isChange = false;

void loop() {

if (cur_code_idx < CODE_LEN)
{
if(!digitalRead(BTN_0)){
  cur_code[cur_code_idx++] = 1;
  beep();
  isChange = true;
}
if(!digitalRead(BTN_1)){
  cur_code[cur_code_idx++] = 2;
  beep();
  isChange = true;
}
if(!digitalRead(BTN_2)){
  cur_code[cur_code_idx++] = 3;
  beep();
  isChange = true;
}
if(!digitalRead(BTN_3)){
  cur_code[cur_code_idx++] = 4;
  beep();
  isChange = true;
}
}



if(!digitalRead(BTN_RESET)){
  cur_code_idx = 0;
  beep();
  isChange = true;
}
if(!digitalRead(BTN_ENTER)){
  beep();
  if (cur_code_idx < CODE_LEN)
  {
    code_error();return;
  }
  
  if (memcmp(cur_code, code1, sizeof(code1)) == 0){
    play_audio(TRAC_A1,TRAC_LEN_MS_A1);
  }else if(memcmp(cur_code, code2, sizeof(code2)) == 0){
    play_audio(TRAC_A2,TRAC_LEN_MS_A2);
  }else if(memcmp(cur_code, code3, sizeof(code3)) == 0){
    play_audio(TRAC_A3,TRAC_LEN_MS_A3);
  }else if(memcmp(cur_code, code4, sizeof(code4)) == 0){
    play_audio(TRAC_A4,TRAC_LEN_MS_A4);
  }else if(memcmp(cur_code, code5, sizeof(code5)) == 0){
    play_audio(TRAC_A5,TRAC_LEN_MS_A5);
  }else if(memcmp(cur_code, code6, sizeof(code6)) == 0){
    play_audio(TRAC_A6,TRAC_LEN_MS_A6);
  }
  else{
    code_error();
  }
  cur_code_idx = 0;
  isChange = true;
  return;
}


if (isChange)
{
  isChange = false;
  print_code(cur_code);
}

delay(500);
}
