


void setup() {
	 Serial.begin(9600);
	 Serial.print("Saving data to EEPROM");
	 pinMode(40, OUTPUT);
	 pinMode(41, INPUT);
	digitalWrite(40,HIGH);
	
	
}

bool t=false;

uint32_t time=0;
void loop() {

	if(millis()>time)
	{
		time=millis()+100;
		digitalWrite(40,t=!t);
	}
	Serial.println(digitalRead(41));

	delay(10);

}
