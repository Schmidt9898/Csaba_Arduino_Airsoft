

void setup()
{
	Serial.begin(9600);
	pinMode(22, INPUT);
	pinMode(23, INPUT);
}


void loop()
{

	if (digitalRead(22))
	{
		Serial.print("ready :");
		Serial.println(digitalRead(23));
	}
	delay(500);
}