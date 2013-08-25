#include "Gamepad.h"

Gamepad rex;

int circleInt = 0;
int pulseInt = -7;

int wrap(int ledPin){
   if(ledPin < 0){
     return wrap(ledPin+20);
   }
   if(ledPin >= 20){
     return wrap(ledPin-20);
   }
   return ledPin;
}
void circle(){
   rex.clearAll();
   rex.setLed(wrap(circleInt), 0);
   rex.setLed(wrap(circleInt+1), 1);
   rex.setLed(wrap(circleInt+2), 3);
   circleInt = wrap(++circleInt);
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

long time = 1;
long last_time = 0;
long max_time = 3251; // max time alloted for selection long rxn_time = 1000; //max time alloted to user to pres sbutton long random_time = random(max_time); byte rand_button; long press_time = 0; byte m_state = 0; //state 0 random flashing //state one one flashing, waiting on user input //state two user presses void loop() {
   time = millis();
   long update_time = time - last_time;
   
   if(m_state == 0){
     if(update_time < random_time ){
       circle();
     } else{
       rand_button = random(3);
       rex.clearAll();
       side_on(rand_button, 3);
       m_state = 1;
       last_time = time;
       random_time = random(max_time);
     }
   } else if (m_state == 1){
     //then you have shined the light at the  m
     
     if(update_time <= rxn_time){
       //still waiting on user
       if(rex.readBtn(rand_button) == 2){
         //almost there
         //correct
         press_time = update_time;
         m_state = 2;
         last_time = time;
       }
     } else {
       //you're too slow
       rex.clearAll();
       if((time/200)%2==0)
         side_on(rand_button, 3);
       if(update_time >= rxn_time + 1000){
         m_state = 0;
         last_time = time;
       }
     }
   } else if (m_state == 2){
     //pressed on time
     if(update_time <= 500){
       pulse();
     } else if(update_time <= 1750){
       //display results for one second
       longToPercent(press_time);
     } else {
       m_state = 0;
       last_time = time;
     }
   } else {
     m_state = 0;
     last_time = time;
   }
   rex.update();
}
//0 not pressed
//1 on for one cycle
//2 pressed in last 250 milliseconds (avoid bouncung)
//3 longer than 250 milliseconds

//variables less than 255, try to use byte to save size //read set LED, 4 settings, 0ff to 1-3 brightness //counterclockwise starting from top left (nw) void side_on(int side, int power) {
   if (side == 0) {
     rex.setLed(17, power);
     rex.setLed(18, power);
     rex.setLed(19, power);
     rex.setLed(0, power);
     rex.setLed(1, power);
   } else if (side == 1) {
     rex.setLed(2, power);
     rex.setLed(3, power);
     rex.setLed(4, power);
     rex.setLed(5, power);
     rex.setLed(6, power);
   } else if (side == 2) {
     rex.setLed(7, power);
     rex.setLed(8, power);
     rex.setLed(9, power);
     rex.setLed(10, power);
     rex.setLed(11, power);
   } else {
     rex.setLed(12, power);
     rex.setLed(13, power);
     rex.setLed(14, power);
     rex.setLed(15, power);
     rex.setLed(16, power);
   }
}

void longToPercent(long time_took){
   rex.clearAll();
   int percentage = ((int)time_took) / ((int)(((int)rxn_time)/((int)20)));
   for (int i = 0; i < percentage; i++){
     rex.setLed(i, 3);
   }
}
   

   


