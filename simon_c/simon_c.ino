#include "Gamepad.h"

Gamepad rex;

byte pattern[32];
unsigned int score;
byte state;
byte current_input; //The index of `pattern` that we expect the user to input next.
byte pace; //Number of lok cycles per flash.
unsigned int lok; //Used for timing within each state.

/*
States:
0 - Showing new pattern
1 - Taking user input
2 - Flashing correct
3 - Game over
4 - Flashing Incorrect
Anything else - Error -> 3
*/

void setup () {
  rex.start();
  score = 2;
  lok = 0;
  state = 2;  //Start by flashing correct, also makes the first pattern.
  pace = 40;
}

void side_on(int side) {
  if (side == 0) {
    rex.setLed(17, 3);
    rex.setLed(18, 3);
    rex.setLed(19, 3);
    rex.setLed(0, 3);
    rex.setLed(1, 3);
  } else if (side == 1) {
    rex.setLed(2, 3);
    rex.setLed(3, 3);
    rex.setLed(4, 3);
    rex.setLed(5, 3);
    rex.setLed(6, 3);
  } else if (side == 2) {
    rex.setLed(7, 3);
    rex.setLed(8, 3);
    rex.setLed(9, 3);
    rex.setLed(10, 3);
    rex.setLed(11, 3);
  } else {
    rex.setLed(12, 3);
    rex.setLed(13, 3);
    rex.setLed(14, 3);
    rex.setLed(15, 3);
    rex.setLed(16, 3);
  }
}

void all_on() {
  rex.setAll(3);
}

void show_pattern() {
  if (lok < score*pace) {
    if (lok - lok/pace*pace < 3*pace/4) {
      side_on(pattern[lok/pace]);
    }
  } else {
    state = 1;
    lok = 0;
  }
}

void show_score() {
  if (lok == pace*3) {
    state = 0;
    lok = 0;
    score = 3;
  }else if(lok%pace <= (pace/2)) {
    for(byte i=0; i<score-3; i++) {
      rex.setLed(i, 3);
    }    
  }else if((lok%(pace))>(pace/2)) {
    rex.clearAll();
  }
}

void take_input() {
  if (lok > 3*pace) {
    state = 4;
    lok = 0;
  } else if (current_input >= score) {
    //The entire sequence was entered.
    state = 2;
    lok = 0;
  }else {
    //Display the last thing the user pushed.
    if (current_input > 0) {
      side_on(pattern[current_input - 1]); 
    }
    //Still taking input.
    for (int i = 0; i < 4; i++){
      if (rex.readBtn(i) == 1) {
        if (i == pattern[current_input]) {
          //Correct.  Go on.
          lok = 0;
          current_input++;
        } else {
          //Wrong.
          state = 4;
          lok = 0;
          break;
        }
      }
    }
  }
}

void next_pattern() {
  if (lok <= 1) {
    score += 1;
    current_input = 0;
    for (byte i = 0; i < score; i++) {
      pattern[i] = random(4);
    }
    lok = 1;
  } else if (lok < 80) {
    if ((lok / 5) % 2 == 0) {
      all_on();
    }
  } else if (lok > 80) {
    lok = 0;
    state = 0;
  }
}

void game_over() {
  all_on();
}

void loop() {
  rex.clearAll();
  if (state == 0){
    show_pattern();
  } else if (state == 1) {
    take_input();
  } else if (state == 2) {
    next_pattern();
  }else if (state == 4) {
    show_score();
  } else {
    game_over();
  }
  lok += 1;
  
//  if(rex.readBtn(0)==2){
//    rex.clearAll();  //like it says, clears the display but stops trying after 250 ms of the button being pressed
//  }else if(rex.readBtn(1)){
//    circle();  //leds scroll in a circle as long as the button is pressed.  I didn't handle the edge case for going from the 19th led to the 0th, so it looks wierd there
//  }else if(rex.readBtn(2)==1){
//    int rand = random(20);  //leds are set once per click (though some are set to off, its not buggy)
//    rex.setLed(rand, rand%4); //DOES NOT set to a random brightness, eg. every 4th LED will be off, etc..
//  }else if(rex.readBtn(3)==3){
//    pulse();  //pulses the leds if the button is held down (doesn't work for first 250ms because that is state 2, could just use readBtn()>1
//  }
    
  rex.update();
}
