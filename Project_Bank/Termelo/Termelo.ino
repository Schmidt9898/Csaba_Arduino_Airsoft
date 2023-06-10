
#include "display.h"
#include "CardService.h"



CardService cardservice;  //  9,10 pin

Profile temp_profile;

#define button_pin A1

#define zold_pin A7
#define sarga_pin A2
#define piros_pin A3

#define termeles_time 3

#define termeles_jutalom 100

bool isbuttonDown = false;

LED led_zold(zold_pin);
LED led_sarga(sarga_pin);
LED led_piros(piros_pin);

void setup()
{
	Serial.begin(9600); // Initialize serial communications with the PC

	cardservice.init();
	lcd_init();

	
	pinMode(button_pin, INPUT_PULLUP);
	//int buttonState = digitalRead(button_pin);
	//HIGH, LOW
	

	//sd card parts

	//name,uid,transaction_cunt,previous,added,new_balance\n header



	logn("Connected!");

	led_zold.turn(on);

}



void loop()
{
  clear_card:
	led_sarga.turn(off);
	led_piros.turn(off);
  	logn("Waiting for Card.");
    while(true)
    {
      if(cardservice.Get_new_card())
        break;
      if(cardservice.Check_card())
        break;
    //log(".");
    delay(1000);
    }
  cardservice.Read_profile_from_card(&temp_profile);
  temp_profile.print();

	//megvárjuk a gombnyomást
logn("Waiting for Button press.");
	while (true)
	{
		if(cardservice.Check_card())
		{
			if(!digitalRead(button_pin))
			{
        if(!isbuttonDown)
        { 
				isbuttonDown = true; // ha leragasztják a gombot akkor ez meggátolja a további elfogadást 
				logn("megnyomva.");
				delay(100);
				goto termeles;
        }
			}else
			{
				isbuttonDown = false;
			}
		}else{
			return; // same as goto clear_card
			//goto clear_card;
		}
	}
	termeles:
 logn("Working...");
	led_sarga.turn(on);
	uint32_t end_time = millis() + 1000 * termeles_time;
	while (millis()<end_time)
	{
		if(cardservice.Check_card())
		{
			//ez jó de nem kell csinálni semmit
		}else{
			return; // same as goto clear_card
			//goto clear_card;
		}
		delay(1000);
		//tune(100)
	}
	led_sarga.turn(off);


	// termelés sikeres pénz kell a kártyára írni
	led_piros.turn(on);
  logn("Transaction.");

  int32_t previous_balance = temp_profile.balance;  
	temp_profile.balance +=  termeles_jutalom;
  temp_profile.transaction_count +=  1;
	cardservice.Write_profile_to_card(&temp_profile);

 if(!cardservice.Check_card())
   {
    return; 
   }

  	temp_profile.print();
	//static char str[100];
	// log to card
	// name,uid,transaction_cunt,previous,added,new_balance

	//sprintf(str,"name :%s,id: %lu,trans: %lu,pre: %ld,added: %d,new: %ld\n",temp_profile.name,temp_profile.uid,temp_profile.transaction_count,previous_balance,termeles_jutalom,temp_profile.balance); // csv format
  //sprintf(str,"%s,%d,%d,%d,%d,%d,%d\n",temp_profile.name,16,1,2,3,4,5,6); // csv format
	//logn(String(str));

}
