#include <dummy.h>
#include <TM1637.h>
#include "ESP8266WiFi.h"

TM1637 tm(4, 5); //kijelzdő CLK=D2 port DIO=D1 port

int buzzer = 0;  //D3 port
#define gomb1 12 //gomb1 D6 port
#define gomb2 13 //gomb2 D7 port
#define gomb3 3 //gomb3 RX port
#define led1 16 //led1 D0 port piros
#define led2 14 //led2 D5 port kék
#define led3 2 //led3 D4 port sárga

int jelerosseg;
int keresett = 0;

String gameing_ssid = "";
int gaming_rssi     = 0;

extern "C" {
#include "user_interface.h"
#include "Esp.h"
}

ADC_MODE(ADC_VCC)

bool kezdes = false;

void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(gomb1, INPUT_PULLUP);
  pinMode(gomb2, INPUT_PULLUP);
  pinMode(gomb3, INPUT_PULLUP);
  tm.init();

  // KIJELZŐ FÉNYERŐ 0-7
  tm.set(3);
  Serial.println("LED_BUILTIN értéke:");
  Serial.println(LED_BUILTIN);
}


void loop() {
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("A program kezdodik");
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  noTone(buzzer);


  if (kezdes == false)
  { 
    Serial.println("Elso futtatas");
    tone(buzzer, 500);
    if (digitalRead(gomb1) == LOW)
    {
      digitalWrite(led1, HIGH);
      Serial.println("piros");
    }
    if (digitalRead(gomb2) == LOW)
    {
      digitalWrite(led2, HIGH);
      Serial.println("kék");
    }
    if (digitalRead(gomb3) == LOW)
    {
      digitalWrite(led3, HIGH);
      Serial.println("sárga");
    }
    delay(500);
    tone(buzzer, 700);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    tone(buzzer, 500);
    if (digitalRead(gomb1) == LOW)
    {
      digitalWrite(led1, HIGH);
    }
    if (digitalRead(gomb2) == LOW)
    {
      digitalWrite(led2, HIGH);
    }
    if (digitalRead(gomb3) == LOW)
    {
      digitalWrite(led3, HIGH);
    }
    delay(500);
    noTone(buzzer);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    kezdes = true;
  }

  if (kezdes == true)
  {

    Serial.print("\nKeresés...\n ");
    int rec_count = 0;
    int ledlvl = 0;

    wifi_set_phy_mode(PHY_MODE_11B);
    int n = WiFi.scanNetworks();



    for (int i = 0; i < n; ++i) {
      if ((WiFi.SSID(i) == "enemy" and digitalRead(gomb1) == LOW) or  (WiFi.SSID(i) == "arty" and digitalRead(gomb2) == LOW) or (WiFi.SSID(i) == "rad" and digitalRead(gomb3) == LOW))
      {
        keresett++;
      }
    }
    if (keresett == 0)
    {
      Serial.print("\nNincs találat!\n ");
      tm.display(3, 0);
      tm.display(2, 0);
      tm.display(1, 0);
      tm.display(0, 0);
      noTone(buzzer);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      // keresett = 0;
    }
    if (keresett > 0)
    {
      keresett = 0;
      for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        if (WiFi.SSID(i) == "enemy" and digitalRead(gomb1) == LOW)
        {
          Serial.print("Látom a SZÖRNYET!\n ");
          Serial.printf("RSSI: %d dBm\n", WiFi.RSSI(i));
          jelerosseg = abs(WiFi.RSSI(i));
          // jelerosseg kiiratása a kijelzőre
          tm.display(3, jelerosseg % 10);
          tm.display(2, jelerosseg / 10 % 10);
          tm.display(1, jelerosseg / 100 % 10);
          tm.display(0, jelerosseg / 1000 % 10);

          if (0 < jelerosseg   and  jelerosseg <= 10 )
          {
            tone(buzzer, 100);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(5);
            tone(buzzer, 100);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(5);
            tone(buzzer, 100);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
          }
          else if (10 < jelerosseg   and  jelerosseg <= 20 )
          {
            tone(buzzer, 200);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(50);
            tone(buzzer, 200);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(50);
            tone(buzzer, 200);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
          }
          else if (20 < jelerosseg   and  jelerosseg <= 30 )
          {
            tone(buzzer, 300);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(100);
            tone(buzzer, 300);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(100);
            tone(buzzer, 300);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
          }
          else if (30 < jelerosseg   and  jelerosseg <= 40 )
          {
            tone(buzzer, 400);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(200);
            tone(buzzer, 400);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(200);
            tone(buzzer, 400);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
          }
          else if (40 < jelerosseg   and  jelerosseg <= 50 )
          {
            tone(buzzer, 500);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(400);
            tone(buzzer, 500);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(400);
            tone(buzzer, 500);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
          }
          else if (50 < jelerosseg   and  jelerosseg <= 60 )
          {
            tone(buzzer, 600);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(600);
            tone(buzzer, 600);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(600);
            tone(buzzer, 600);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
          }
          else if (60 < jelerosseg   and  jelerosseg <= 70 )
          {
            tone(buzzer, 700);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(800);
            tone(buzzer, 700);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(800);
            tone(buzzer, 700);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
          }
          else if (70 < jelerosseg   and  jelerosseg <= 80 )
          {
            tone(buzzer, 800);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(1000);
            tone(buzzer, 800);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(1000);
            tone(buzzer, 800);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
          }
          else if (80 < jelerosseg   and  jelerosseg <= 90 )
          {
            tone(buzzer, 900);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(1200);
            tone(buzzer, 900);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(1200);
            tone(buzzer, 900);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
          }
          else if (90 < jelerosseg )

          {
            tone(buzzer, 1000);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(1500);
            tone(buzzer, 1000);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
            delay(1500);
            tone(buzzer, 1000);
            digitalWrite(led1, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led1, LOW);
          }
        }

        if (WiFi.SSID(i) == "arty" and digitalRead(gomb2) == LOW)
        {
          Serial.print("Látom aZ ARTIFACTOT!\n ");
          Serial.printf("RSSI: %d dBm\n", WiFi.RSSI(i));
          jelerosseg = abs(WiFi.RSSI(i));
          // jelerosseg kiiratása a kijelzőre
          tm.display(3, jelerosseg % 10);
          tm.display(2, jelerosseg / 10 % 10);
          tm.display(1, jelerosseg / 100 % 10);
          tm.display(0, jelerosseg / 1000 % 10);

          if (0 < jelerosseg   and  jelerosseg <= 10 )
          {
            tone(buzzer, 100);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(5);
            tone(buzzer, 100);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(5);
            tone(buzzer, 100);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
          }
          else if (10 < jelerosseg   and  jelerosseg <= 20 )
          {
            tone(buzzer, 200);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(50);
            tone(buzzer, 200);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(50);
            tone(buzzer, 200);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
          }
          else if (20 < jelerosseg   and  jelerosseg <= 30 )
          {
            tone(buzzer, 300);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(100);
            tone(buzzer, 300);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(100);
            tone(buzzer, 300);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
          }
          else if (30 < jelerosseg   and  jelerosseg <= 40 )
          {
            tone(buzzer, 400);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(200);
            tone(buzzer, 400);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(200);
            tone(buzzer, 400);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
          }
          else if (40 < jelerosseg   and  jelerosseg <= 50 )
          {
            tone(buzzer, 500);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(400);
            tone(buzzer, 500);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(400);
            tone(buzzer, 500);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
          }
          else if (50 < jelerosseg   and  jelerosseg <= 60 )
          {
            tone(buzzer, 600);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(600);
            tone(buzzer, 600);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(600);
            tone(buzzer, 600);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
          }
          else if (60 < jelerosseg   and  jelerosseg <= 70 )
          {
            tone(buzzer, 700);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(800);
            tone(buzzer, 700);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(800);
            tone(buzzer, 700);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
          }
          else if (70 < jelerosseg   and  jelerosseg <= 80 )
          {
            tone(buzzer, 800);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(1000);
            tone(buzzer, 800);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(1000);
            tone(buzzer, 800);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
          }
          else if (80 < jelerosseg   and  jelerosseg <= 90 )
          {
            tone(buzzer, 900);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(1200);
            tone(buzzer, 900);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(1200);
            tone(buzzer, 900);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
          }
          else if (90 < jelerosseg )

          {
            tone(buzzer, 1000);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(1500);
            tone(buzzer, 1000);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
            delay(1500);
            tone(buzzer, 1000);
            digitalWrite(led2, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led2, LOW);
          }
        }

        if (WiFi.SSID(i) == "rad" and digitalRead(gomb3) == LOW)
        {
          Serial.print("Látom a RADIOAKTIVITÁST!\n ");
          Serial.printf("RSSI: %d dBm\n", WiFi.RSSI(i));
          jelerosseg = abs(WiFi.RSSI(i));
          // jelerosseg kiiratása a kijelzőre
          tm.display(3, jelerosseg % 10);
          tm.display(2, jelerosseg / 10 % 10);
          tm.display(1, jelerosseg / 100 % 10);
          tm.display(0, jelerosseg / 1000 % 10);

          if (0 < jelerosseg   and  jelerosseg <= 10 )
          {
            tone(buzzer, 100);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(5);
            tone(buzzer, 100);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(5);
            tone(buzzer, 100);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
          }
          else if (10 < jelerosseg   and  jelerosseg <= 20 )
          {
            tone(buzzer, 200);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(50);
            tone(buzzer, 200);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(50);
            tone(buzzer, 200);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
          }
          else if (20 < jelerosseg   and  jelerosseg <= 30 )
          {
            tone(buzzer, 300);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(100);
            tone(buzzer, 300);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(100);
            tone(buzzer, 300);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
          }
          else if (30 < jelerosseg   and  jelerosseg <= 40 )
          {
            tone(buzzer, 400);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(200);
            tone(buzzer, 400);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(200);
            tone(buzzer, 400);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
          }
          else if (40 < jelerosseg   and  jelerosseg <= 50 )
          {
            tone(buzzer, 500);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(400);
            tone(buzzer, 500);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(400);
            tone(buzzer, 500);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
          }
          else if (50 < jelerosseg   and  jelerosseg <= 60 )
          {
            tone(buzzer, 600);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(600);
            tone(buzzer, 600);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(600);
            tone(buzzer, 600);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
          }
          else if (60 < jelerosseg   and  jelerosseg <= 70 )
          {
            tone(buzzer, 700);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(800);
            tone(buzzer, 700);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(800);
            tone(buzzer, 700);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
          }
          else if (70 < jelerosseg   and  jelerosseg <= 80 )
          {
            tone(buzzer, 800);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(1000);
            tone(buzzer, 800);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(1000);
            tone(buzzer, 800);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
          }
          else if (80 < jelerosseg   and  jelerosseg <= 90 )
          {
            tone(buzzer, 900);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(1200);
            tone(buzzer, 900);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(1200);
            tone(buzzer, 900);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
          }
          else if (90 < jelerosseg )

          {
            tone(buzzer, 1000);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(1500);
            tone(buzzer, 1000);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
            delay(1500);
            tone(buzzer, 1000);
            digitalWrite(led3, HIGH);
            delay(200);
            noTone(buzzer);
            digitalWrite(led3, LOW);
          }
        }

      }
    }
  }
}
