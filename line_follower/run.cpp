#include "run.h"

// enum line_type{
//   LINE,
//   WHITE,
//   BLACK,
//   MIXED
// };

static bool line = false;
static uint8_t pirsv = 0;
static uint8_t birsv = 0;
// static bool temp_irsv[8] = {0};

static enum run_state state = START;

static int distanceQue[4] = {2000, 2000, 2000, 2000};
static uint8_t dqIndex = 0;

static bool obstacle = false;
// static enum line_type lineType = WHITE;
// static uint8_t intersection;


void resetRunMode(){
  state = START;

  dqIndex = 0;
  
  for(uint8_t i; i<4; i++){
    distanceQue[i] = 2000;
    //temp_irsv[i] = 0;
  }

  obstacle = false;
  // lineType = WHITE;
  line = false;
  pirsv = 0;
  birsv = 0;

  resetLineFollower();
  resetObstacle();
  resetIntersection();
  //reset follow path

  // ms.targetVel = {};
  ms.drive = false;
}

#ifdef DEBUG
  static String name;
  static void stateToString(enum run_state localState){
    switch (localState){
      case START:
        name = "START";
        break;
      case FOLLOW_LINE:
        name = "FOLLOW_LINE";
        break;
      case OBSTACLE:
        name = "OBSTACLE";
        break;
      case LOST_LINE:
        name = "LOST_LINE";
        break;
      case INTERSECTION:
        name = "INTERSECTION";
        break;
      case DEAD_END:
        name = "DEAD_END";
        break;
      case FINISH:
        name = "FINISH";
        break;
    }
  }
  unsigned long stateTime = millis();
#endif

static void changeState(enum run_state newState){
  #ifdef DEBUG
    stateToString(state);
    Serial.print(name);
    Serial.print(" -> ");
    stateToString(newState);
    Serial.println(name);
    Serial.println("state time: "+String(millis()-stateTime));
    stateTime = millis();
  #endif
  switch (newState){
    case FOLLOW_LINE:
      //init_follow_line
      break;
    case OBSTACLE:
      initObstacle();
      break;
    case LOST_LINE:
      //init lost line
      break;
    case INTERSECTION:
      initIntersection();
      break;
    case DEAD_END:
      //init dead_end
      break;
    case FINISH:
      //init finish
      break;
    }
  state = newState;
}


static void checkObstacle(){
  if (distanceQue[dqIndex] != ms.distance){
    dqIndex = (dqIndex+1)%4;
    distanceQue[dqIndex] = ms.distance;
  }
  int distance = 0;
  for(uint8_t i=0; i<4; i++) distance += distanceQue[i];
  distance/=4;
  obstacle = (distance < MIN_OBSTACLE_DISTANCE);
}


//!!!this only works when IR_SENSOR_COUNT == 8!!!
static bool checkLine(){
  bool white = true, temp = false, stop = false;
  birsv = 0;
  for (uint8_t i=0; i<IR_SENSOR_COUNT; i++){
    bool b = ms.irsv[i] > 50; //(1<<(IR_SENSOR_RESOLUTION-1));
    //if (b != temp_irsv[i]) {b = !b;}
    //temp_irsv[i] = b;
    birsv += b<<i;
    if (b) white = false;
    if (!white && !b) temp = true;
    if (temp && b) stop = true;
  }
  if (stop || birsv ==0 || white) return false; // laatste statement kan normaal ook vervangen worden door white origineel: birsv == (1<<IR_SENSOR_COUNT)-1
  uint8_t diff = birsv ^ pirsv;
  if ((diff & (diff<<1)) != 0) return false;
  uint8_t count = 0;
  for (uint8_t i=0; i<8; i++){
    count += 1 & (diff>>i);
  }
  if (count>2) return false;
  return true;
}

// int sign;
//temporarly variable
static bool temp_prev = false;
static bool check_prev = false;

void run(bool intersectionAlg){
  checkObstacle();
  line = checkLine();
  
  // tunePID();
  #ifdef DEBUG
    if(line != temp_prev) Serial.println("line: " + String(line));
    temp_prev = line;
    if (checkLineFlag != check_prev) Serial.println("checkline: "+ String(checkLineFlag));
    check_prev = checkLineFlag;
    if (birsv != pirsv) Serial.println(birsv);
      // Serial.print("line: "); Serial.print(line);
      // Serial.print("  checklineflag: "); Serial.println(checkLineFlag);

  #endif

  switch (state){
    case START:
      //start: delay + beep + drive straight while line not detected.
      if (line && checkLineFlag) {
        resetStart();
        changeState(FOLLOW_LINE);
      }else{
        callStart();
      }
      break;
    case FOLLOW_LINE:
      if (obstacle){
        resetLineFollower();
        changeState(OBSTACLE);
        break;
      }
      if (!line){
        if (birsv == 0){
          if ((pirsv & 0b10000001) == 0) changeState(DEAD_END);
          else changeState(LOST_LINE);
        }else changeState(INTERSECTION);
      }
      //if intersection ? state = intersection; reset follow line
      followLine();
      break;
    case LOST_LINE:
      // lost line
      if(line) changeState(FOLLOW_LINE);
      followLine();
      break;
    case OBSTACLE:
      if (checkLineFlag && line){
       resetObstacle();
       changeState(FOLLOW_LINE);  
      }
      callObstacle();

      break;
    case INTERSECTION:
      if (intersectionAlg){
        if(line && checkLineFlag) {
          resetIntersection();
          changeState(FOLLOW_LINE);
        }else if (callIntersection(birsv, pirsv, line)){
          resetIntersection();
          changeState(FINISH);
        }
      } else {
        if (birsv & 0b10000000) followRight();
        else if(line) changeState(FOLLOW_LINE);
        else followLine();
      }
      
      break;
    case DEAD_END:
      if (line){
        changeState(FOLLOW_LINE);
      }
      deadEnd();
      break;
    case FINISH:
      //victory routine
      changeMode(IDLE);
      break;
  }

  pirsv = birsv; // for line detection algorithm
}