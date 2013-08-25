/*
   Snake!
   How to play: 
       Press a button just as the snake's head is on the
       corresponding LED (the LED near the button that's 
       indented in) to make the snake turn around. If you 
       let the snake go in the same direction for a full 
       circle, it gets longer! Don't let the snake's head 
       reach its tail, or it's GAME OVER. The snake is clever
       though; everytime it turns around, it speeds up! There 
       are 20 different speeds. Can you get past them all?
   Scoring:
       When the game is over, a number of LEDs will pulse.
       The number of pulsing LEDs is equivalent to your score.
       The score is calculated by the speed the snake reached.
       If the snake was still on the slowest speed, your score
       will be 1 and a single LED will pulse. If you beat all 
       20 speeds, your score will be 20 and all the LEDs will
       pulse.
   Stopping and Resetting:
       To stop the game, press and hold buttons 0 and 2.
       To reset the game, the game must first be stopped, and
       then buttons 1 and 3 should be pressed.       
*/

#include "Gamepad.h"

Gamepad snake;

//Basic snake properties
int snakeLength = 0;
int snakeStart = 0;
int currentDir = 1;

//Keeps track of whether the game is playing or stopped
bool play = true;

//Used to keep track of and display the score
int score = -1;
bool pulseOn = true;

//Helps tell the snake when to get longer
int prevSnakeStart = 19;
int circleStart = 0;
int circleEnd = 19;

//Used to make the snake wait before moving
unsigned long presentTime;
unsigned long resumeTime = 0;
int waitTime=500;

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
  snake.setLed(wrap(snakeStart+dir), 3);
  snake.setLed(wrap(snakeStart-dir*(snakeLength-1)), 0);
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
  snake.clearAll();
}

void setup(){
  snake.start();
}

void loop(){
  //Resets the game if the game has been stopped.
  if(snake.readBtn(1)>0 && snake.readBtn(3)>0){
    reset();
  }
  while(play){
    if(score!=-1){
      //display the score by pulsing the appropriate LEDs
      presentTime = millis();
      if(presentTime > resumeTime){
        for(int i =0; i < score; i++){
          if (pulseOn){
              snake.setLed(i, 3);
          }
          else{
            snake.setLed(i, 0);
          }
        }
        pulseOn = !pulseOn;
        wait();
      }
    }
    else{
      //If the button corresponding to the position of the snake 
      //head is pressed, change the direciton and speed it up.
      if((snake.readBtn(0)==1 && snakeStart==19)||
         (snake.readBtn(1)==1 && snakeStart==4)||
         (snake.readBtn(2)==1 && snakeStart==9)||
         (snake.readBtn(3)==1 && snakeStart==14)){
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
          snake.setLed(wrap(snakeStart-snakeLength), 3);
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
    if(snake.readBtn(0)>1 && snake.readBtn(2)>1){
      play = false;
      snake.clearAll();
    }
    snake.update(); //update button and LED states
  }
  snake.update(); //update button and LED states
}
