#include "Gamepad.h"

Gamepad rex;

int circleInt = 0;
int pulseInt = -7;

void circle(){
  rex.setLed(circleInt, 0);
  rex.setLed(circleInt+1, 1);
  rex.setLed(circleInt+2, 3);
  circleInt++;
  if(circleInt==rex.numLeds-2){
    circleInt=0;
    rex.setLed(19, 0);
    rex.setLed(18, 0);
  }
}

void pulse(){
  rex.setAll(abs(pulseInt)/2);
  pulseInt++;
  if(pulseInt>3)
    pulseInt=-7;
}

void setup () {
  rex.start();
}

void loop() {
  if(rex.readBtn(0)==2){
    rex.clearAll();  //like it says, clears the display but stops trying after 250 ms of the button being pressed
  }else if(rex.readBtn(1)){
    circle();  //leds scroll in a circle as long as the button is pressed.  I didn't handle the edge case for going from the 19th led to the 0th, so it looks wierd there
  }else if(rex.readBtn(2)==1){
    int rand = random(20);  //leds are set once per click (though some are set to off, its not buggy)
    rex.setLed(rand, rand%4); //DOES NOT set to a random brightness, eg. every 4th LED will be off, etc..
  }else if(rex.readBtn(3)==3){
    pulse();  //pulses the leds if the button is held down (doesn't work for first 250ms because that is state 2, could just use readBtn()>1
  }
    
  rex.update();
}
