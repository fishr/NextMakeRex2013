/*
  Gamepad.hpp - NextMake REX 2013 library
*/

#ifndef Gamepad_hpp
#define Gamepad_hpp

#include "Arduino.h"

class Gamepad
{
  public:
  Gamepad();
  
  //button methods
	byte readBtn(byte btn);
   
  //charlieplexing constants
	static const byte numLeds = 20;

  //charlieplexing methods
	void setLed(byte ledNum, byte powerLvl);
	byte readLed(byte ledNum);
	void clearAll();
	void setAll(byte lvl);
	
  //place at end of loop()
	void update();
  //place at beginning of setup()
	void start();


  private:
  
  //button constants
	static const byte btnPins[4];
	//the top left btn is btn[0] and it goes CW from there
	byte btnState;  //similar structure to the led storage blocks, the least significant btn is at left, with up to 4 different states available
	//0: not pressed, 1: just pressed, 2: pressed within last (defined #)ms, 3: held down > 200ms 
	static long btnTime[4];
	static const byte debounce = 255;
	
  //button methods
  byte readPin(byte pin);
  void updateBtns();
  void setBtn(byte btn, byte lvl);
	
  //charlieplexing constants
	static const byte numPins = 5;
	static const byte ledPins[numPins];
	static const byte* pinOffset;
	static const byte numBlocks = (numLeds>>2);//works for 20
	static byte ledArray[numBlocks];
	//least significant LED on the far left (lowest address)
	//zero indexed
	static const byte delayTime = 1;
	
  //charlieplexing methods
	void setPinHigh(byte pin);
	void setPinLow(byte pin);
	void setPinOff(byte pin);
	void setBlock(byte block, byte level);
	void blankBlock(byte block);
	boolean checkBlockZero(byte block);
	void displayOut();
	
};

#endif

