const int button1 = 3; //first button is on pin
const int button2 = 4; //second is on pin
const int button3 = 5; //third is pin 
const int button4 = 6; //fourth is pin 
const int button5 = 7; //third is pin 
const int button6 = 8; //fourth is pin 

const int buzzer = 2;

const int greenLed = 11; //green LED is pin 11
const int Red = 12; //red LED is on pin 12

void checkEntered1(int button);

int code[] = {1,2,3,4,5,6}; //the desired code is entered in this array,
                        //separated by commas

int entered[7]; //create a new empty array for the code entered by
                //the user (has 4 elements)

void setup(){ //run once at sketch startup
  Serial.begin(9600); //begin Serial

  pinMode(button1, INPUT_PULLUP); //button 1 is an input
  pinMode(button2, INPUT_PULLUP); //button 2 is an input
  pinMode(button3, INPUT_PULLUP); //button 3 is an input
  pinMode(button4, INPUT_PULLUP); //button 4 is an input
  pinMode(button5, INPUT_PULLUP); //button 3 is an input
  pinMode(button6, INPUT_PULLUP); //button 4 is an input

  pinMode(Red, OUTPUT); //the red LED is an output
  pinMode(greenLed, OUTPUT); // the green LED is an output
//  setupLights(); //run the setupLights routine
//  setupLights(); //run it again
 // delay(650); //delay (only for effect, :P not needed)
  digitalWrite(Red, LOW); //turn the red LED on
  for (int i = 0; i < 6;i++){ //work through numbers 0-3
    Serial.println(code[i]); //print each digit of the code
    Serial.println(entered[i]); //print each element of the entered[]
                                //array (this was for me to check that it 
                                //started at 0
    
  }
}

void loop(){ //run repeatedly
  if (digitalRead(button1) == LOW){ //if button1 is pressed
    checkEntered1(1); //call checkEntered and pass it a 1
    tone(buzzer, 200);  
    delay(250);
    noTone(buzzer);
  }
  else if (digitalRead(button2) == LOW){ //if button2 is pressed
    checkEntered1(2); //call checkEntered1 and pass it a 2
    
    tone(buzzer, 200);  
    delay(250);
    noTone(buzzer);
    
  }
  else if (digitalRead(button3) == LOW){ //if button3 is pressed
    checkEntered1(3); //call checkEntered1 and pass it a 3
    
    tone(buzzer, 200);  
    delay(250);
    noTone(buzzer);
    
  }
  else if (digitalRead(button4) == LOW){ //if button4 is pressed
    checkEntered1(4); //call checkEntered1 and pass it a 4
    
    tone(buzzer, 200);  
    delay(250);
    noTone(buzzer);
    
  }
    else if (digitalRead(button5) == LOW){ //if button4 is pressed
    checkEntered1(5); //call checkEntered1 and pass it a 4
    
    tone(buzzer, 200);  
    delay(250);
    noTone(buzzer);
    
  }
    else if (digitalRead(button6) == LOW){ //if button4 is pressed
    checkEntered1(6); //call checkEntered1 and pass it a 4
    
    tone(buzzer, 200);  
    delay(250);
    noTone(buzzer);
    
  }
  

}

void checkEntered1(int button){ //check the first element of the entered[] array
  
  if (entered[0] != 0){ //if it is not a zero, i.e. it has already been inputted
    checkEntered2(button); //move on to checkEntered2, passing it "button"
  }
  
  else if(entered[0] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[0] = button; //set the first element as the button that has been pressed
    Serial.print("1: ");Serial.println(entered[0]); //for debugging
  }
  
}

void checkEntered2(int button){ //check the second element of the entered[] array
  
  if (entered[1] != 0){ //if it is not a zero, i.e. it has already been inputted
    checkEntered3(button); //move on to checkEntered3, passing it "button"
  }
  
  else if(entered[1] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[1] = button; //set the second element as the button that has been pressed
    Serial.print("2: ");Serial.println(entered[1]); //for debugging
  }
  
}

void checkEntered3(int button){  //check the third element of the entered[] array
  
  if (entered[2] != 0){ //if it is not a zero, i.e. it has already been inputted
    checkEntered4(button); //move on to checkEntered4, passing it "button"
  }
  
  else if (entered[2] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[2] = button; //set the third element as the button that has been pressed
    Serial.print("3: ");Serial.println(entered[2]); //for debugging
  }
  
}

void checkEntered4(int button){  //check the third element of the entered[] array
  
  if (entered[3] != 0){ //if it is not a zero, i.e. it has already been inputted
    checkEntered5(button); //move on to checkEntered4, passing it "button"
  }
  
  else if (entered[3] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[3] = button; //set the third element as the button that has been pressed
    Serial.print("4: ");Serial.println(entered[3]); //for debugging
  }
  
}


void checkEntered5(int button){  //check the third element of the entered[] array
  
  if (entered[4] != 0){ //if it is not a zero, i.e. it has already been inputted
    checkEntered6(button); //move on to checkEntered4, passing it "button"
  }
  
  else if (entered[4] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[4] = button; //set the third element as the button that has been pressed
    Serial.print("5: ");Serial.println(entered[4]); //for debugging
  }
  
}

void checkEntered6(int button){ //check the fourth element of the entered[] array
  
  if (entered[5] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[5] = button; //set the final element as the button that has been pressed
    Serial.print("6: ");Serial.println(entered[5]); //for debugging
    delay(100); //allow time for processing
    compareCode(); //call the compareCode function
  }
}

void compareCode(){ //checks if the code entered is correct by comparing the code[] array with the entered[] array
  for (int i = 0; i<6;i++){ //these three lines are for debugging
    Serial.println(entered[i]);
  }
  if ((entered[0]==code[0]) && (entered[1]==code[1]) && (entered[2]==code[2]) && (entered[3]==code[3]) && (entered[4]==code[4])&& (entered[5]==code[5])){ //if all the elements of each array are equal
    //Jó megoldás
    digitalWrite(Red, LOW); // turn the red LED off
    digitalWrite(greenLed, HIGH); //turn the green LED on
    while(true)
    {
      
    }


    
    for (int i = 0; i < 7; i++){ //this next loop is for debugging
      entered[i] = 0;
      
    }
   
    loop(); //return to loop() (not really necessary)
  }
  
  else { //if you (or the intruder) get the code wrong
    //Rossz megoldás
    
    digitalWrite(Red,HIGH);
    delay(5000);
    digitalWrite(Red,LOW);
    Serial.println("Red OFF");
    for (int i = 0; i < 7; i++){ //this next loop is for debugging
      entered[i] = 0;
     
    }
    
  }
  close_all();
}



void close_all(){
  
}