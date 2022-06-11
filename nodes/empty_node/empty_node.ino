#define btn_1 3
#define btn_2 4


#define ready 5
#define bad 11
#define good 12

//#define MEDIUM 512

//int bad_val=1024;
bool is_set=false;
void setup()
{
	Serial.begin(9600);
	pinMode(btn_1, INPUT_PULLUP);
	pinMode(btn_2, INPUT_PULLUP);

	pinMode(ready, OUTPUT);
	digitalWrite(ready, HIGH);

	pinMode(good, OUTPUT);
	digitalWrite(good, LOW);

	pinMode(bad, OUTPUT);
	digitalWrite(bad, LOW);
}


void loop()
{
	if(!is_set)
	{
		if (!digitalRead(btn_1))
		{
			is_set=true;
			digitalWrite(bad,LOW);
			digitalWrite(good, HIGH);
			while(1);
		}
		else if(!digitalRead(btn_2))
		{
			is_set=true;
			digitalWrite(bad,HIGH);
			digitalWrite(good, LOW);
			while(1);
		}
	}
}