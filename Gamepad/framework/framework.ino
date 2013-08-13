const byte btnPins[] = {8,6,7,10};
//the top left btn is btn[0] and it goes CW from there
byte btnState = 0;  //similar structure to the led storage blocks, the least significant btn is at left, with up to 4 different states available
//0: not pressed, 1: just pressed, 2: pressed within last (defined #)ms, 3: held down > 200ms 
long btnTime[] = {0,0,0,0};
byte debounce = 255;

byte readPin(byte pin){
  if (*portInputRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin)) return HIGH;
	return LOW;
}

void updateBtns(){
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


byte readBtn(byte btn){
  const byte shift = (btn%4)<<1;
  byte row = btnState<<shift;
  return row>>6;
}

void setBtn(byte btn, byte lvl){
  byte power = lvl<<6;
  byte shift = ((btn%4)<<1);
  btnState=(power>>(shift))|((btnState)&(~((B11000000)>>shift)));
}

//////////////////////////////////////////////////---------------Charlieplexing below this line---------------------////////////////////////////////////////
const byte numPins = 5;
const byte ledPins[numPins] = {0,1,2,3,9};
const byte* pinOffset = &ledPins[0];
const byte numLeds = (numPins-1)*numPins;
const byte numBlocks = (numLeds>>2);//works for 20
byte ledArray[numBlocks];
//least significant LED on the far left (lowest address)
//zero indexed
const byte delayTime = 1;

long lastTime;
byte circleInt = 0;
int pulseInt = -6;

void setLed(byte ledNum, byte powerLvl){
  byte power = powerLvl<<6;
  byte shift = ((ledNum%4)<<1);
  ledArray[(int)ledNum>>2]=(power>>(shift))|((ledArray[(int)ledNum>>2])&(~((B11000000)>>shift)));
}

byte readLed(byte ledNum){
  const byte shift = (ledNum%4)<<1;
  byte row = ledArray[(int)ledNum>>2]<<shift;
  return row>>6;
}

void clearAll(){
  for(int i = 0; i<numBlocks; i++){
    ledArray[i] = 0;
  }
}

void setAll(byte lvl){
  byte level = lvl<<6;
  byte setting = (level|((level>>2)|((level>>4)|(level>>6))));
  for(int i = 0; i<numBlocks; i++){
    ledArray[i] = setting;
  }
}

void setPinHigh(byte pin){
  *portModeRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin);
  *portOutputRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin);
}

void setPinLow(byte pin){
  *portModeRegister(digitalPinToPort(pin)) |= digitalPinToBitMask(pin);
  *portOutputRegister(digitalPinToPort(pin)) &= ~digitalPinToBitMask(pin);
}

void setPinOff(byte pin){
  *portModeRegister(digitalPinToPort(pin)) &= ~digitalPinToBitMask(pin);
  *portOutputRegister(digitalPinToPort(pin)) &= ~digitalPinToBitMask(pin);
}

void setBlock(byte block, byte level){
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

void blankBlock(byte block){
  for(byte i = 0; i<4; i++){
    setPinOff(*(i+pinOffset));
  }
}

boolean checkBlockZero(byte block){
  return ledArray[block];
}

void displayOut(){
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

void circle(){
  setLed(circleInt, 0);
  setLed(circleInt+1, 1);
  setLed(circleInt+2, 3);
  circleInt++;
  if(circleInt==numLeds-2){
    circleInt=0;
    setLed(19, 0);
    setLed(18, 0);
  }
}

void pulse(){
  setAll(abs(pulseInt)/2);
  pulseInt++;
  if(pulseInt>3)
    pulseInt=-7;
}

void update(){
  displayOut();
  updateBtns();
}

void start(){
  randomSeed(analogRead(4));
  clearAll();
  for(unsigned i=0;i<4;i++){
    *portModeRegister(digitalPinToPort(btnPins[i])) &= ~digitalPinToBitMask(btnPins[i]);
    *portOutputRegister(digitalPinToPort(btnPins[i])) |= digitalPinToBitMask(btnPins[i]);
  }
}

void setup () {
  start();
}

void loop() {
  if(readBtn(0)==2){
    clearAll();  //like it says, clears the display but stops trying after 250 ms of the button being pressed
  }else if(readBtn(1)){
    circle();  //leds scroll in a circle as long as the button is pressed.  I didn't handle the edge case for going from the 19th led to the 0th, so it looks wierd there
  }else if(readBtn(2)==1){
    int rand = random(20);  //leds are set once per click (though some are set to off, its not buggy)
    setLed(rand, rand%4); //DOES NOT set to a random brightness, eg. every 4th LED will be off, etc..
  }else if(readBtn(3)==3){
    pulse();  //pulses the leds if the button is held down (doesn't work for first 250ms because that is state 2, could just use readBtn()>1
  }
    
  update();
}
