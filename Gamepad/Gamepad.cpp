/*
  Gamepad.hpp - NextMake REX 2013 library
*/

#include "Arduino.h"

#include "Gamepad.h"

Gamepad::Gamepad(){
}
  
const byte Gamepad::btnPins[] = {8,6,7,10};
long Gamepad::btnTime[] = {0,0,0,0};
const byte Gamepad::ledPins[] = {0,1,2,3,9};
const byte* Gamepad::pinOffset = &ledPins[0];
byte Gamepad::ledArray[numBlocks];


byte Gamepad::readPin(byte pin){
  if (*portInputRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin)) return HIGH;
	return LOW;
}

void Gamepad::updateBtns(){
  for(byte i = 0; i<4; i++){
    switch (readBtn(i)) {
    case 0:
      if(!readPin(btnPins[i])){
        setBtn(i, 1);
        btnTime[i] = millis();
      }
      break;
    case 1:
      if(readPin(btnPins[i])){
        setBtn(i, 0);
        break;
      }
      setBtn(i, 2);
      break;
    case 2:
      if(readPin(btnPins[i])){
        setBtn(i, 0);
        break;
      }
      if((millis()-btnTime[i])>debounce){
        setBtn(i, 3);
      }
      break;
    case 3:
      if(readPin(btnPins[i])){
        setBtn(i, 0);
      }
      break; 
    }
  }
}


byte Gamepad::readBtn(byte btn){
  const byte shift = (btn%4)<<1;
  byte row = btnState<<shift;
  return row>>6;
}

void Gamepad::setBtn(byte btn, byte lvl){
  byte power = lvl<<6;
  byte shift = ((btn%4)<<1);
  btnState=(power>>(shift))|((btnState)&(~((B11000000)>>shift)));
}

//////////////////////////////////////////////////---------------Charlieplexing below this line---------------------////////////////////////////////////////

void Gamepad::setLed(byte ledNum, byte powerLvl){
  byte power = powerLvl<<6;
  byte shift = ((ledNum%4)<<1);
  ledArray[(int)ledNum>>2]=(power>>(shift))|((ledArray[(int)ledNum>>2])&(~((B11000000)>>shift)));
}

byte Gamepad::readLed(byte ledNum){
  const byte shift = (ledNum%4)<<1;
  byte row = ledArray[(int)ledNum>>2]<<shift;
  return row>>6;
}

void Gamepad::clearAll(){
  for(int i = 0; i<numBlocks; i++){
    ledArray[i] = 0;
  }
}

void Gamepad::setAll(byte lvl){
  byte level = lvl<<6;
  byte setting = (level|((level>>2)|((level>>4)|(level>>6))));
  for(int i = 0; i<numBlocks; i++){
    ledArray[i] = setting;
  }
}

void Gamepad::setPinHigh(byte pin){
  *portModeRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin);
  *portOutputRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin);
}

void Gamepad::setPinLow(byte pin){
  *portModeRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin);
  *portOutputRegister(digitalPinToPort(pin)) &= ~digitalPinToBitMask(pin);
}

void Gamepad::setPinOff(byte pin){
  *portModeRegister(digitalPinToPort(pin)) &= ~digitalPinToBitMask(pin);
  *portOutputRegister(digitalPinToPort(pin)) &= ~digitalPinToBitMask(pin);
}

void Gamepad::setBlock(byte block, byte level){
  setPinLow(*(block+pinOffset));
  byte pinSet = 0;
  for(byte i = 0; i<numBlocks; i++){
    if(i==block){  
    }else if(readLed((block*4)+pinSet)<level){
      setPinOff(*(i+pinOffset));
      pinSet++;
    }else{ 
      //Serial.println((block*4)+pinSet, DEC);
      setPinHigh(*(i+pinOffset));
      pinSet++;
    }  
  }
}

void Gamepad::blankBlock(byte block){
  for(byte i = 0; i<4; i++){
    setPinOff(*(i+pinOffset));
  }
}

boolean Gamepad::checkBlockZero(byte block){
  return ledArray[block];
}

void Gamepad::displayOut(){
  for(byte i = 1; i<4; i++){
    for(byte j = 0; j<numBlocks; j++){
      if(checkBlockZero(j)){
        setBlock(j, i);
        delay(delayTime);
      }else{
        blankBlock(j);
        delay(delayTime);
      }
    }
  }
  blankBlock(numBlocks-1);
}

void Gamepad::update(){
  displayOut();
  updateBtns();
}

void Gamepad::start(){
  randomSeed(analogRead(4));
  clearAll();
  for(unsigned i=0;i<4;i++){
    *portModeRegister(digitalPinToPort(btnPins[i])) &= ~digitalPinToBitMask(btnPins[i]);
    *portOutputRegister(digitalPinToPort(btnPins[i])) |= digitalPinToBitMask(btnPins[i]);
  }
}