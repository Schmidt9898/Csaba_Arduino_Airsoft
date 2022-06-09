#define btn_1 3
#define btn_2 4


#define ready 5
#define output 11
#define activate 12

//#define MEDIUM 512

//int output_val=1024;
bool is_set=false;
void setup()
{
	Serial.begin(9600);
	pinMode(btn_1, INPUT_PULLUP);
	pinMode(btn_2, INPUT_PULLUP);

	pinMode(ready, OUTPUT);
	digitalWrite(ready, HIGH);

	pinMode(activate, OUTPUT);
	digitalWrite(activate, LOW);

	pinMode(output, OUTPUT);
	digitalWrite(output, LOW);
}


void loop()
{
	if(!is_set)
	{
		if (!digitalRead(btn_1))
		{
			is_set=true;
			digitalWrite(activate, LOW);
			digitalWrite(output, HIGH);
			digitalWrite(activate, HIGH);
			Serial.println(HIGH);
		}
		else if(!digitalRead(btn_2))
		{
			is_set=true;
			digitalWrite(activate, LOW);
			digitalWrite(output, LOW);
			digitalWrite(activate, HIGH);
			Serial.println(LOW);
		}
	}
}