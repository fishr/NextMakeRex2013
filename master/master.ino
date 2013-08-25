#include "Gamepad.h"

Gamepad rex;

byte gameSelected = 4;

//Felix
byte pattern[32];
unsigned int Fscore;
byte state;
byte current_input; //The index of `pattern` that we expect the user to input next.
byte pace; //Number of lok cycles per flash.
unsigned int lok;

//Halla
//Basic snake properties
int snakeLength = 0;
int snakeStart = 0;
int currentDir = 1;

//Keeps track of whether the game is playing or stopped
boolean play = true;

//Used to keep track of and display the score
int score = -1;
boolean pulseOn = true;

//Helps tell the snake when to get longer
int prevSnakeStart = 19;
int circleStart = 0;
int circleEnd = 19;

//Used to make the snake wait before moving
unsigned long presentTime;
unsigned long resumeTime = 0;
int waitTime=500;

//Hans

//Antonio



void setup() {
  rex.start();
  //felix
  Fscore = 2;
  lok = 0;
  state = 2;  //Start by flashing correct, also makes the first pattern.
  pace = 40;
  
  //halla
  
  //hans
  
  //antonio
  
  
  //startup
  byte _slowerDowner = 0;
  while(gameSelected == 4) {
      rex.clearAll();
      if((_slowerDowner/16)%4==0){
        side_on(0);
        side_on(2);
        side_on(3);
      }else if((_slowerDowner/16)%4==1){
        side_on(0);
        side_on(1);
        side_on(3);
      }else if((_slowerDowner/16)%4==2){
        side_on(0);
        side_on(1);
        side_on(2);
      }else if((_slowerDowner/16)%4==3){
        side_on(1);
        side_on(2);
        side_on(3);
      }
    
      for(byte i = 0; i<4; i++){
        if(rex.readBtn(i)>0) {
          gameSelected = i;
        }
      }

    _slowerDowner++;
    rex.update();
  }
  rex.clearAll();
}

//felix
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
  if (lok < Fscore*pace) {
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
    Fscore = 3;
  }else if(lok%pace <= (pace/2)) {
    for(byte i=0; i<Fscore-3; i++) {
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
  } else if (current_input >= Fscore) {
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
    Fscore += 1;
    current_input = 0;
    for (byte i = 0; i < Fscore; i++) {
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

void felix() {
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
}

//halla

/*
  Resets all the necessary variables for the game to start again.
*/
void reset(){
  snakeLength = 0;
  snakeStart = 0;
  currentDir = 1;
  play = true;
  score = -1;
  pulseOn = true;
  prevSnakeStart = 19;
  circleStart = 0;
  circleEnd = 19;
  waitTime=500;
}

/*
  Essentially returns ledPin modulus 20. Can't use % operator 
  because it doesn't think (-1)%20==19.
*/
int wrap(int ledPin){
  if(ledPin < 0){
    return wrap(ledPin+20);
  }
  if(ledPin >= 20){
    return wrap(ledPin-20);
  }
  return ledPin;
}
  
/*
  Updates resumeTime to be waitTime milliseconds 
  past the presentTime.
*/
void wait(){
  resumeTime = presentTime+waitTime;
}
  
/*
  Moves the snake one LED in the specified direction. 
  dir = 1 for counter-clockwise, dir = -1 for clockwise
*/
void moveSnake(int dir){
  //update LEDs
  rex.setLed(wrap(snakeStart+dir), 3);
  rex.setLed(wrap(snakeStart-dir*(snakeLength-1)), 0);
  //update variables
  prevSnakeStart = snakeStart;
  snakeStart = wrap(snakeStart+dir);
  //update the time the snake can move again
  wait();
}

/*
  Performs the necessary actions for when the snake changes
  direction.
*/
void changeDir(){
  //updates the point where the snake will get longer
  circleStart = snakeStart;
  circleEnd = wrap(snakeStart+currentDir);
  //updates the head of the snake
  prevSnakeStart = snakeStart;
  snakeStart -= currentDir*(snakeLength-1);
  //switches the direction from 1 to -1 or vice versa
  currentDir -= currentDir*2;
}

/*
  Reduces the waitTime. It decreases by 100 until it hits 500,
  50 until it hits 200, and 25 until it hits 0. This gives a 
  total of 20 different speeds.
*/
void updateWaitTime(){
  if(waitTime>250){
    waitTime-=50;
  }
  else if(waitTime>100){
    waitTime-=25;
  }
  else if(waitTime>0){
    waitTime-=12;
  }
  else{ //Can't decrease past 0, end game.
    setScore();
  }
}

/*
  Determine the score based on how fast the waitTime is.
*/
void setScore(){
  if(waitTime>=250){
    score = (500-waitTime)/500;
  }
  else if(waitTime>=100){
    score = 5+(250-waitTime)/25;
  }
  else if(waitTime>=0){
    score = 11 + (100-waitTime)/12;
  }
  //clear the LEDs and set the wait time for the score pulse
  waitTime = 500;
  rex.clearAll();
}

void halla() {
  //Resets the game if the game has been stopped.
  if(rex.readBtn(1)>0 && rex.readBtn(3)>0){
    reset();
  }
  while(play){
    if(score!=-1){
      //display the score by pulsing the appropriate LEDs
      presentTime = millis();
      if(presentTime > resumeTime){
        for(int i =0; i < score; i++){
          if (pulseOn){
              rex.setLed(i, 3);
          }
          else{
            rex.setLed(i, 0);
          }
        }
        pulseOn = !pulseOn;
        wait();
      }
    }
    else{
      //If the button corresponding to the position of the snake 
      //head is pressed, change the direciton and speed it up.
      if((rex.readBtn(0)==1 && snakeStart==19)||
         (rex.readBtn(1)==1 && snakeStart==4)||
         (rex.readBtn(2)==1 && snakeStart==9)||
         (rex.readBtn(3)==1 && snakeStart==14)){
            changeDir();
            updateWaitTime();
      }
      //wait until the resumeTime to move the snake
      presentTime = millis();
      if(presentTime > resumeTime){
        //This makes the snake actually stay at LED 0 for a 
        //wait cycle when the game first starts.
        if(snakeLength > 0){
          moveSnake(currentDir);
        }
        //If the snake passes the last place it was turned around,
        //it gets one LED longer.
        if(snakeStart==circleStart && prevSnakeStart==circleEnd){
          rex.setLed(wrap(snakeStart-snakeLength), 3);
          snakeLength++;
          wait();
        }
      }
      //If the snake catches up to its tail, end game
      if(snakeLength>=20){
        setScore();
      }
    }
      
    //Stops the game
    if(rex.readBtn(0)>1 && rex.readBtn(2)>1){
      play = false;
      rex.clearAll();
    }
    rex.update(); //update button and LED states
  }
}

//hans

void hans(){
  
}

//antonio

void antonio(){
  
}

void loop() {
  switch (gameSelected){
    case 0:
      felix();
      break;
    
    case 1:
      halla();
      break;
    
    case 2:
      hans();
      break;
      
    case 3:
      antonio();
      break;
  }
  
  rex.update();
}
