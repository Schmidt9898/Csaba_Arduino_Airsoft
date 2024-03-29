#include <TrueRandom.h> //https://github.com/sirleech/TrueRandom

#define red 11
#define yellow 10
#define green 9
#define blue 3
int colori[] = {11, 10, 9, 3};

#define btn_red 7
#define btn_yellow 6
#define btn_green 5
#define btn_blue 4

#define JoMegoldas 8
#define RosszMegoldas 12
const int buzzer = 2;

const int MAX_LEVEL = 10;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int level = 1;

int velocity = 500;
int bounce = 500;
int brightness = 10;

void setup() {
  pinMode(btn_red, INPUT_PULLUP);
  pinMode(btn_green, INPUT_PULLUP);
  pinMode(btn_blue, INPUT_PULLUP);
  pinMode(btn_yellow, INPUT_PULLUP);


  pinMode(JoMegoldas, OUTPUT);
  pinMode(RosszMegoldas, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  for (int i = 0; i < 5; i++)
  {
    pinMode(colori[i], OUTPUT);
  }
  start();
  delay(30);
  start();
  delay(30);
}

void loop()
{
  if (level == 1)
    generate_sequence(); //generate a sequence;
  show_sequence();     //show the sequence
  get_sequence();      //wait for your sequence

}


void show_sequence()
{
  for (int i = 0; i < 5; i++) {
    digitalWrite(colori[i], LOW);
  }
  for (int i = 0; i < level; i++)
  {
    analogWrite(colori[sequence[i]], brightness);
    delay(velocity);
    digitalWrite(colori[sequence[i]], 0);
    delay(velocity / 2);
  }
}

void get_sequence()
{
  int flag = 0; //this flag indicates if the sequence is correct

  for (int i = 0; i < level; i++)
  {
    flag = 0;
    while (flag == 0)
    {
      if (digitalRead(btn_green) == LOW)
      {
        tone(buzzer, 200);
        delay(200);
        noTone(buzzer);
        delay(200);
        analogWrite(green, brightness);
        your_sequence[i] = green;
        flag = 1;
        delay(bounce);
        if (your_sequence[i] != colori[sequence[i]])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(green, 0);
      }

      if (digitalRead(btn_yellow) == LOW)
      {
        tone(buzzer, 200);
        delay(200);
        noTone(buzzer);
        delay(200);
        analogWrite(yellow, brightness);
        your_sequence[i] = yellow;
        flag = 1;
        delay(bounce);
        if (your_sequence[i] != colori[sequence[i]])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(yellow, 0);
      }

      if (digitalRead(btn_blue) == LOW)
      {
        tone(buzzer, 200);
        delay(200);
        noTone(buzzer);
        delay(200);
        analogWrite(blue, brightness);
        your_sequence[i] = blue;
        flag = 1;
        delay(bounce);
        if (your_sequence[i] != colori[sequence[i]])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(blue, 0);
      }

      if (digitalRead(btn_red) == LOW)
      {
        tone(buzzer, 200);
        delay(200);
        noTone(buzzer);
        delay(200);
        analogWrite(red, brightness);
        your_sequence[i] = red;
        flag = 1;
        delay(bounce);
        if (your_sequence[i] != colori[sequence[i]])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(red, 0);
      }
    }
  }
  right_sequence();
}

void generate_sequence()
{
  for (int i = 0; i < MAX_LEVEL; i++)
  {
    sequence[i] = TrueRandom.random(0, 3);
    delay(15);
  }
  for (int i = 0; i < MAX_LEVEL; i++)
  {
    delay(5);
    Serial.print("Seq_");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(sequence[i]);
    Serial.print("\n");
  }
}

void wrong_sequence()
{
  digitalWrite(RosszMegoldas, HIGH);
  tone(buzzer, 500);
  delay(500);
  noTone(buzzer);
  delay(500);
  tone(buzzer, 500);
  delay(500);
  noTone(buzzer);
  delay(500);
  tone(buzzer, 500);
  delay(500);
  noTone(buzzer);
  delay(500);

  delay(5000);
  digitalWrite(RosszMegoldas, LOW);
  /*

     Hibás jel kidása és várakoztatni 1 órát
  */

  for (int i = 0; i < 60; i++)
  {
    analogWrite(red, i);
    analogWrite(green, i);
    analogWrite(blue, i);
    analogWrite(yellow, i);
    delay(20);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(colori[i], LOW);
    delay(200);
  }
  level = 1;
  velocity = 500;
}

void right_sequence()
{
  for (int i = 0; i < 5; i++) {
    digitalWrite(colori[i], LOW);
  }
  for (int i = 0; i < 5; i++) {
    analogWrite(colori[i], brightness);
    delay(50);
  }
  delay(500);
  for (int i = 0; i < 5; i++) {
    digitalWrite(colori[i], LOW);
    delay(50);
  }
  delay(500);

  if (level < MAX_LEVEL) {
    level++;
    tone(buzzer, 1000);
    delay(500);
    noTone(buzzer);
    delay(500);
  }
  else {
    gooal();
    /*
      Sikeres megoldás
    */
  }
  velocity -= 40; //increase difficulty
}

void start() {
  for (int i = 0; i < 4; i++) {
    analogWrite(colori[i], brightness);
    delay(50);
  }
  for (int i = 0; i < 4; i++) {
    digitalWrite(colori[i], 0);
    delay(50);
  }
}
void gooal () {
 /* while (1) {
    for (int z = 0; z < 100; z = z + 10) {
      for (int i = 0; i < 4; i++) {
        analogWrite(colori[i], z);
        delay(40);
      }
    }
    for (int i = 0; i < 4; i++) {
      digitalWrite(colori[i], 0);
      delay(150);
      */
      /*
        Sikeres megoldás
      */
      digitalWrite(JoMegoldas, HIGH);
      while(true){}
    }
  //}
//}