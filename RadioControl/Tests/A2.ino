//nRF24L01 communication 2 ways Arduino A

#include <SPI.h>
#include   <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte   addresses [][6] = {"00001", "00002"};  //Setting the two addresses. One for   transmitting and one for receiving
int button_pinA = 4;
int led_pinA = 3;
boolean   button_stateA = 0;
boolean button_stateB = 0;

void setup() {
  pinMode(button_pinA,   INPUT_PULLUP);
  pinMode(led_pinA, OUTPUT);
  radio.begin();                           //Starting   the radio communication
  radio.openWritingPipe(addresses[1]);     //Setting   the address at which we will send the data
  radio.openReadingPipe(1, addresses[0]);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);   //You can set it as minimum or maximum depending on the distance between the transmitter   and receiver. 
}

void loop() 
{  
  delay(5);
  radio.stopListening();                              //This sets the module as transmitter
  button_stateA   = digitalRead(button_pinA);
  radio.write(&button_stateA, sizeof(button_stateA));   //Sending the data
  delay(5);
  
  radio.startListening();                            //This   sets the module as receiver
  while(!radio.available());                         //Looking   for incoming data
  radio.read(&button_stateB, sizeof(button_stateB)); //Reading   the data
  if (button_stateB == LOW)
  {
    digitalWrite(led_pinA, HIGH);
   }
  else
  {
    digitalWrite(led_pinA, LOW);
  }
}