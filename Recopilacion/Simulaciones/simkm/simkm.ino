/*
 * Arduino Keyboard Emulation
 * learnelectronics
 * 13 FEB 2017
 * 
 * www.youtube.com/c/learnelectronics
 */

#include <Keyboard.h>    // This is a "built-in" library no need to install

//---------------------------------------------------------
//                           Setup
//---------------------------------------------------------



void setup() {
pinMode(0,INPUT_PULLUP);  // sets pin 0 to input & pulls it high w/ internal resistor
pinMode(1,INPUT_PULLUP);  // sets pin 1 to input & pulls it high w/ internal resistor
pinMode(2,INPUT_PULLUP);  // sets pin 3 to input & pulls it high w/ internal resistor

Serial.begin(9600);       // begin serial comms for debugging

}

//---------------------------------------------------------
//                           Loop
//---------------------------------------------------------

void loop() {
  
  
 Keyboard.begin();         //begin keyboard 
 if (digitalRead(0) == 0)  // if buton 3 is pushed
  {
    Keyboard.write('A');  // send single character "A"
    delay(200);           // delay so you don't get 20 A's
  }
  else if (digitalRead(2) == 0){  // if button 4 is pressed
    Keyboard.print("The rain in Spain falls mainly on the plain"); // send string
    delay(200);
}
  else if (digitalRead(1) == 0){  //if button 5 is pressed
    Keyboard.write(0xB0);         // send Hex value for RETURN key
    delay(200);
  }
  Keyboard.end();                 //stops keybord
}
