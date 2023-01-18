#define btn_1 3
#define btn_2 4
#define btn_3 5
#define btn_4 6
#define btn_5 7

#define JoMegoldas 11
#define RosszMegoldas 12
const int buzzer = 2;

bool utem1 = false;
bool utem2 = false;
bool utem3 = false;
bool utem4 = false;
bool utem5 = false;

void setup()
{
  Serial.begin(9600);
  pinMode(btn_1, INPUT_PULLUP);
  pinMode(btn_2, INPUT_PULLUP);
  pinMode(btn_3, INPUT_PULLUP);
  pinMode(btn_4, INPUT_PULLUP);
  pinMode(btn_5, INPUT_PULLUP);

  pinMode(JoMegoldas, OUTPUT);
  pinMode(RosszMegoldas, OUTPUT);

  pinMode(buzzer, OUTPUT);

  tone(buzzer, 200);
  delay(200);
  noTone(buzzer);
  delay(200);
  tone(buzzer, 200);
  delay(200);
  noTone(buzzer);

  bool utem1 = false;
  bool utem2 = false;
  bool utem3 = false;
  bool utem4 = false;
  bool utem5 = false;
}


void loop()
{
  Serial.println("Kezdés");
  if (digitalRead(btn_1) == LOW and digitalRead(btn_2) == HIGH and digitalRead(btn_3) == HIGH and digitalRead(btn_4) == HIGH and digitalRead(btn_5) == HIGH )
  {
    utem1 = true;
    Serial.println("utem1 = ok");
  }
  if (utem1 == true and digitalRead(btn_1) == LOW and digitalRead(btn_2) == LOW and digitalRead(btn_3) == HIGH and digitalRead(btn_4) == HIGH and digitalRead(btn_5) == HIGH )
  {
    utem2 = true;
    Serial.println("utem2 = ok");
  }
  if (utem2 == true and digitalRead(btn_1) == LOW and digitalRead(btn_2) == LOW and digitalRead(btn_3) == LOW and digitalRead(btn_4) == HIGH and digitalRead(btn_5) == HIGH )
  {
    utem3 = true;
    Serial.println("utem3 = ok");
  }
  if (utem3 == true and digitalRead(btn_1) == LOW and digitalRead(btn_2) == LOW and digitalRead(btn_3) == LOW and digitalRead(btn_4) == LOW and digitalRead(btn_5) == HIGH )
  {
    utem4 = true;
    Serial.println("utem4 = ok");
  }
  if (utem4 == true and digitalRead(btn_1) == LOW and digitalRead(btn_2) == LOW and digitalRead(btn_3) == LOW and digitalRead(btn_4) == LOW and digitalRead(btn_5) == LOW )
  {
    utem5 = true;
    Serial.println("utem5 = ok");
  }

  if (digitalRead(btn_1) == LOW and digitalRead(btn_2) == LOW and digitalRead(btn_3) == LOW and digitalRead(btn_4) == LOW and digitalRead(btn_5) == LOW )
  {
    if (utem5 == true)
    {
      //jó
      Serial.println("JÓ MEGOLDÁS");
      digitalWrite(JoMegoldas, HIGH);
      while (true)
      {
        delay(2000);
      }
    }
    else
    {
      //rossz
      Serial.println("ROSSZ MEGOLDÁS");
      digitalWrite(RosszMegoldas, HIGH);
      delay(5000);
      digitalWrite(RosszMegoldas, LOW);
    }
  }
delay(200);
digitalWrite(JoMegoldas, LOW);
digitalWrite(RosszMegoldas, LOW);
}