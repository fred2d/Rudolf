#include "inputs.h"

const uint8_t btnPin = 17;

static const uint TIME_OUT_TIME = 600;
static const uint LONG_PRESS_TIME = 500;
static const uint DEAD_TIME = 50; 

static bool pressed = false;
static bool state;

static uint8_t pressCount = 0;
static unsigned long pressTime;


void inputsSetup(){
  pinMode(btnPin, INPUT_PULLUP);
}

static void handleInput(uint8_t pressType){
  switch (pressType){
    case 0: //longpress
      changeMode(IDLE);
      break;
    case 1: //single press
      changeMode(RUN);
      break;
    case 2: //double press
      changeMode(CALIBRATION);
      break;
    case 3: //triple press
      changeMode(TEST);
      break;
    default: //multipress
      changeMode(IDLE);
      break;
  }
}

void checkInputs(){
  state = digitalRead(btnPin); // 0: pressed, 1: 
  if (pressCount>0){
    if (millis() - pressTime > TIME_OUT_TIME) {
      handleInput(pressCount);
      pressCount = 0;
    }
  }

  if (state == pressed){
    if (!pressed){
      pressTime = millis();
      pressed = true;
    }
    else{
      if (pressCount > 0 || millis()-pressTime < LONG_PRESS_TIME){
        if (millis()-pressTime >DEAD_TIME) pressCount++;
      }
      else{
        handleInput(0);
      }
      pressed = false;
    }
  }
}