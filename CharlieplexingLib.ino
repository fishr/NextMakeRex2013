#define numPins 5

const byte pinOffset = 2;
const byte numLeds = (numPins-1)*numPins;
const byte numBlocks = (numLeds>>2);//works for 20
byte ledArray[numBlocks];
//least significant LED on the far left (lowest address)
//zero indexed
const byte delayTime = 1;

long lastTime;
byte testInt = 0;


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

void setPinHigh(byte pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

void setPinLow(byte pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void setPinOff(byte pin){
  pinMode(pin, INPUT);
}

void setBlock(byte block, byte level){
  setPinHigh(block+pinOffset);
  byte pinSet = 0;
  for(byte i = 0; i<numBlocks; i++){
    if(i==block){  
    }else if(readLed((block*4)+pinSet)<level){
      setPinOff(i+pinOffset);
      pinSet++;
    }else{ 
      //Serial.println((block*4)+pinSet, DEC);
      setPinLow(i+pinOffset);
      pinSet++;
    }  
  }
}

void blankBlock(byte block){
  for(byte i = 0; i<4; i++){
    setPinOff(i+pinOffset);
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

void test(){
  setLed(testInt, 1);
  setLed(testInt+1, 2);
  setLed(testInt+2, 3);
  testInt++;
  if(testInt==numLeds-2){
    testInt=0;
  }
}

void setup () {
  clearAll();
  lastTime = millis();
  ledArray[0]=0;
}

void loop() {
  if(millis()-lastTime>50){
    test();
    lastTime = millis();
  }
  displayOut();
}
