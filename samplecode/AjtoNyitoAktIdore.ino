#include <Keypad.h>
#include  <virtuabotixRTC.h>

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

const String password_1 = "1234"; // change your password here
const String password_2 = "4321"; // change your password here
const String password_3 = "123456789876543210"; // change your password here
String input_password = "";

const int buzzer = 2;
virtuabotixRTC myRTC(10, 11, 12);


int joLED = A2;
int rosszLED = A1;
int nyito = A3;

void setup() {
  pinMode(joLED, OUTPUT);
  pinMode(rosszLED, OUTPUT);
  pinMode(nyito, OUTPUT);
  Serial.begin(9600);
  tone(buzzer, 500);
  delay(200);
  noTone(buzzer);
  delay(200);
  tone(buzzer, 500);
  delay(200);
  noTone(buzzer);
}

void loop() {
  myRTC.updateTime();
  digitalWrite(joLED, LOW);
  digitalWrite(rosszLED, LOW);  
  digitalWrite(nyito, LOW);
  char key = keypad.getKey();// Read the key

  if (key) {
    Serial.println(key);

    char buf [4];
    sprintf(buf, "%d%02d", myRTC.hours, myRTC.minutes);
    int ido = myRTC.hours * 100 + myRTC.minutes;
    Serial.println(ido);

    if (key == '*') {
      input_password = ""; // reset the input password
    } else if (key == '#') {
      if ( input_password.toInt() == ido)
      {
        Serial.println("JÓ MEGOLDÁS");
        tone(buzzer, 900);
        delay(100);
        noTone(buzzer);
        delay(100);
        tone(buzzer, 700);
        delay(100);
        noTone(buzzer);
        delay(100);
        tone(buzzer, 900);
        delay(100);
        noTone(buzzer);
        tone(buzzer, 700);
        delay(100);
        noTone(buzzer);
        tone(buzzer, 900);
        delay(100);
        noTone(buzzer);
        digitalWrite(joLED, HIGH);
        digitalWrite(nyito, HIGH);
        delay(5000);
        digitalWrite(joLED, LOW);        
        digitalWrite(nyito, LOW);
      }
      else
      {
        Serial.println("ROSSZ MEGOLDÁS");
        tone(buzzer, 200);
        delay(200);
        noTone(buzzer);
        delay(200);
        tone(buzzer, 200);
        delay(200);
        noTone(buzzer);
        digitalWrite(rosszLED, HIGH);
        delay(5000);
        digitalWrite(rosszLED, LOW);
      }

      input_password = ""; // reset the input password
    } else {
      input_password += key; // append new character to input password string
    }
  }

}
