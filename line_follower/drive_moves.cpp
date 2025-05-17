#include "drive_moves.h"


bool checkLineFlag = false;
static float startDistance = 0;

//--------start-----------//
static unsigned long startTime = 0;

void resetStart(){
  checkLineFlag = false;
  startTime = 0;
  ms.drive = false;
  startDistance = 0;
}

void callStart(){
  if (startTime == 0){
    startTime = millis();
    startDistance = ms.pos.fwd;
    ms.drive = false;
  }
  if (millis()-startTime > 1000) {
    // #ifdef DEBUG
    //   if(DEBUG_CLK) Serial.print("pos" + String(ms.pos.fwd) + "ref: " + String(startDistance));
    // #endif
    if (ms.pos.fwd-startDistance >= 100) checkLineFlag = true;
    ms.drive = true;
    ms.angvel = 0;
  }
}

//--------dead end---------//

void deadEnd(){
  ms.drive = true;
  ms.angvel =  255;
}


//--------obstacle---------//
float OBSTACLE_INIT_ANG = -1;
uint8_t OBSTACLE_RETURN_ANGVEL = 34;

static float zero_pos;

void initObstacle(){
  zero_pos = ms.pos.ang;
  ms.drive = true;
}


void callObstacle(){
  if (checkLineFlag){
    ms.angvel = OBSTACLE_RETURN_ANGVEL;
  } else{
    ms.angvel = -100;
  }
  if (ms.pos.ang - zero_pos <= OBSTACLE_INIT_ANG) checkLineFlag = true;
}

void resetObstacle(){
  checkLineFlag = false;
  ms.drive = false;
}

//-------intersection---------
//static bool clearedIntersection = false;
// static uint8_t intersectionMap[8] = {0};
static bool checkingIntersection = true;
static uint8_t seen = 0;
static bool left = false;
static bool right = false;
static bool checkFalsePositive = false;
static bool checkFinish = false;

static bool falsePositive = true;
static bool allBlack = true;

static float angStartTurn = 0;

void initIntersection(){
  startDistance = ms.pos.fwd; 
  ms.drive = true;
  ms.angvel = 0;
}

bool callIntersection(uint8_t birsv, uint8_t pirsv, bool line){
  if (checkingIntersection){
    if ((birsv & pirsv & 0b11100000) == 0b11100000) right = true;
    if ((birsv & pirsv & 0b00000111) == 0b00000111) left = true;
    seen |= (birsv & pirsv);

    if (ms.pos.fwd-startDistance >= 20){
      checkFalsePositive = (line && !(pirsv & birsv & 0b10000001)); // line present
      if (!checkFalsePositive){
        falsePositive = false;
      }
      if ((birsv|pirsv) == 0b11111111) checkFinish = true;
      else if (checkFinish) allBlack = false;
      #ifdef DEBUG
        if (DEBUG_CLK) {
          Serial.println("check fp: " + String(checkFalsePositive) + " fp: " + String(falsePositive) + "  check finish: " + String(checkFinish) + " allblack: "+String(allBlack));
        }
      #endif
    }

    if ((birsv & 0b00011000) == 0){
      if (right || (!left && (seen & 0b11110000) == 0b11110000)){
        ms.angvel = -255;
        angStartTurn = ms.pos.ang;
      }
      else { 
        ms.angvel = 255;
        checkLineFlag = true;
      }
      
      checkingIntersection = false;
      #ifdef DEBUG
        Serial.println("dead crosspoint : " + String(ms.angvel));
      #endif
    } 
    //remove this to test if the above function works properly at all time!
    //filter out false positives
    //check finish continuously
    //don't remove sharp intersections

    else if (ms.pos.fwd - startDistance >= 80  && (checkFalsePositive || checkFinish)) {
      #ifdef DEBUG
          Serial.println("max intersection distance");
      #endif
      if (falsePositive) {
        checkLineFlag = true;
        checkingIntersection = false;
        #ifdef DEBUG
          Serial.println("false positive! ");
        #endif
      }
      else if (allBlack && checkFinish){
        return true;
        //finish!!
      }
      else{
        #ifdef DEBUG
          Serial.println("end intersection");
        #endif
        if (right && !left){
          ms.angvel = -255; // wait one radian
          #ifdef DEBUG
            Serial.println("right");
          #endif
        }
        else checkLineFlag = true;
        checkingIntersection = false;
      }
      // else {
      //   #ifdef DEBUG
      //     Serial.println("end intersection wihtout succes");
      //   #endif
      //   checkLineFlag = true;
      // }
    }
  }
  else if (ms.pos.ang - angStartTurn <-1 && (birsv & 0b10000001) == 0b10000000) {
    #ifdef DEBUG
      if (!checkLineFlag) Serial.println("turned cleared line and checking for line");
    #endif
    checkLineFlag = true;
  }
  return false;
}

void resetIntersection(){
  ms.drive = false;
  checkingIntersection = true;
  checkLineFlag = false;
  //clearedIntersection = false;
  startDistance = 0;
  seen = 0;
  left = false;
  right = false;

  checkFalsePositive = false;
  checkFinish = false;
  falsePositive = true;
  allBlack = true;
  angStartTurn = 0;
}


void followRight(){
  ms.drive = true;
  ms.angvel = -255;
}