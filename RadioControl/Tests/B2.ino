//nRF24L01 communication 2 ways Arduino B

#include <SPI.h>
#include  <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte  addresses [][6] = {"00001", "00002"};    //Setting the two addresses. One for  transmitting and one for receiving
int button_pinB = 6;
int led_pinB = 5;
boolean  button_stateA = 0;
boolean button_stateB = 0;

void setup() {
  pinMode(button_pinB,  INPUT_PULLUP);
  pinMode(led_pinB, OUTPUT);
  radio.begin();                            //Starting  the radio communication
  radio.openWritingPipe(addresses[0]);      //Setting  the address at which we will send the data
  radio.openReadingPipe(1, addresses[1]);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);            //You can set it as minimum or maximum depending on the distance between  the transmitter and receiver.
}

void loop() 
{
  delay(5);
  radio.startListening();                    //This sets the module as receiver
  if (radio.available())                     //Looking for incoming data
  {
    radio.read(&button_stateA,  sizeof(button_stateA));
    if(button_stateA == LOW)
  {
     digitalWrite(led_pinB,  HIGH);
  }
  else
  {
     digitalWrite(led_pinB, LOW);
  }
  delay(5);
  
  radio.stopListening();                           //This sets the module  as transmitter
  button_stateB = digitalRead(button_pinB);
  radio.write(&button_stateB,  sizeof(button_stateB));   //Sending the data
  }
}