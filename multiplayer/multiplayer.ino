#include "Gamepad.h"

Gamepad rex;

unsigned long gameTime = 10000;

int ledInt;
unsigned long startTime;
int vel;

int pulseInt = -7;
int halftime;

int winner = 4;

int player0 = 0;
int player1 = 0;
int player2 = 0;
int player3 = 0;

void nextLight(){
  rex.setAll(0);
  
  if (vel == 0){
    halftime += 1;
    if (halftime > 1){
      ledInt += 1;
      halftime = 0;
    }
  } else{
    ledInt += vel;
  }
  ledInt %= 20;
  rex.setLed(ledInt, 2);
  
  int changeVel = random(10);
  if (changeVel==0){
    vel = random(2);
  }
}

int pressed(int button){
  return max((4 - min(abs(ledInt - button - 1), abs(ledInt - (button + 19)))), -3);
}

void startGame(){
  rex.setAll(abs(pulseInt)/2);
  pulseInt++;
  if(pulseInt>3)
    pulseInt=-7;
}

void endGame(){
  if(millis() < startTime + gameTime + 1000){
    rex.setAll(abs(pulseInt)/2);
    pulseInt++;
    if(pulseInt>3)
      pulseInt=-7;
  }
  else if(millis() < startTime + gameTime + 3000) {
    if (winner > 3){
      if (player0 > player1){
        if (player2 > player3){
          if (player0 > player2){
            winner = 0;
          } else {
            winner = 2;
          }
        } else{
          if (player0 > player3){
            winner = 0;
          } else {
            winner = 3;
          }
        }
      } else {
        if (player2 > player3){
          if (player1 > player2){
            winner = 1;
          } else {
            winner = 2;
          }
        } else{
          if (player1 > player3){
            winner = 1;
          } else {
            winner = 3;
          }
        }
      }
    } else{
        rex.setAll(0);
        ledInt = (winner * 5) - 1;
        if (ledInt < 0){
          ledInt += 20;
        }
        rex.setLed(ledInt, abs(pulseInt)/2);
        pulseInt++;
        if(pulseInt>3)
          pulseInt=-7;
    }
  } else{
      startTime = millis() + 3000;
      winner = 4;
      player0 = 0;
      player1 = 0;
      player2 = 0;
      player3 = 0;
  }
}

void setup () {
  rex.start();
  ledInt = random(20);
  vel = random(2);
  halftime = 0;
  startTime = millis() + 3000;
}

void loop() {
  
  if(millis() < startTime){
    startGame();
  } else if(millis() < startTime + gameTime){
    if(rex.readBtn(0)==2){
      player0 += pressed(0);
    }
    if(rex.readBtn(1)==1){
      player1 += pressed(5);
    }
    if(rex.readBtn(2)==1){
      player2 += pressed(10);
    }
    if(rex.readBtn(3)==1){
      player3 += pressed(15);
    }
      
    nextLight();
  } else {
    endGame();
  }
  rex.update();
}
