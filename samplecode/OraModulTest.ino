#include  <virtuabotixRTC.h>  //Library used

//Wiring SCLK -> 6, I/O -> 7, CE -> 8
//Or     CLK -> 6 , DAT -> 7, Reset -> 8

virtuabotixRTC myRTC(10, 11, 12); //If you change the wiring change the pins here also

void setup() {
  Serial.begin(9600);

  // Set the current date, and time in the following format:
  // seconds, minutes, hours, day of the week, day of the month, month, year
// myRTC.setDS1302Time(30, 36, 19, 4, 02, 06, 2022);  //Here you write your actual time/date as shown above 
                                                   //but remember to "comment/remove" this function once you're done
                                                   //The setup is done only one time and the module will continue counting it automatically
}

void loop() {
  // This allows for the update of variables for time or accessing the individual elements.
  myRTC.updateTime();

  // Start printing elements as individuals
  Serial.print("Jelenlegi dátum / Idő: ");
  Serial.print(myRTC.year);             //You can switch between day and month if you're using American system
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.dayofmonth);
  Serial.print(" ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);

char buf [4];
  sprintf(buf, "%d%02d", myRTC.hours,myRTC.minutes);
  int ido = myRTC.hours*100+myRTC.minutes;
  if  (ido > 2010)
  {
    Serial.println(buf);
    Serial.println("Kinyitva");    
  }
  
  

  // Delay so the program doesn't print non-stop
  delay(1000);
}
