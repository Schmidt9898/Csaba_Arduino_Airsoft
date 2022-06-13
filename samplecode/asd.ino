#include "LedControl.h"
/*
 Vcc to Arduino 5V Pin
GND to Arduino GND
DIN to Arduino pin  -14
CS to Arduino pin  -15
CLK to Arduino pin  16
 */
LedControl lc=LedControl(14,16,15,1);

unsigned long delaytime=1000;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}
void loop() { 
    lc.setDigit(0,7,7,false);//balról
    lc.setDigit(0,6,6,false);
    lc.setDigit(0,5,5,false);
    lc.setDigit(0,4,4,false);
    lc.setDigit(0,3,3,false);
    lc.setDigit(0,2,2,false);
    lc.setDigit(0,1,1,false);
    lc.setDigit(0,0,0,false);
    delay(delaytime);
  lc.clearDisplay(0);
  delay(delaytime);
}
