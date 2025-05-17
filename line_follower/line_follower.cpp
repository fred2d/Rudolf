#include "line_follower.h"

static float delta = 0;
static float lineWidth = 0;

static pidq_t PIDQue;

//see constants.h
float Kp = 6;
float Ki = 0;
float Kd = 0; 

float error = 0;
float integral = 0;
float derivative = 0;

void resetLineFollower(){
  delta = 0;
  lineWidth = 0;
  PIDQue = {};

  integral = 0;
}

static void getLinePos(){
  uint8_t min_i=IR_SENSOR_COUNT;
  uint8_t max_i=0;
  for(uint8_t i=0; i<IR_SENSOR_COUNT;i++){
    if (ms.irsv[i] >= (1<<IR_SENSOR_RESOLUTION)*3/4){
      if(i <min_i){min_i = i;}
      if(i> max_i){max_i = i;}
    }
  }
  min_i = max(0, min_i-1);
  max_i = min(IR_SENSOR_COUNT-1, max_i+1);
  
  if (min_i > max_i) return;
  
  delta = 0;
  int count = 0;
  for(uint8_t i=min_i; i<max_i+1;i++){delta +=i*ms.irsv[i]; count += ms.irsv[i];}
  
  delta = delta*IR_SENSOR_PITCH/count -IR_SENSOR_OFFSET;
  // ms.delta = delta; //remove!
  lineWidth = IR_SENSOR_PITCH*count/(1<<IR_SENSOR_RESOLUTION);
}

// void tunePID(){
//   if(Serial.available()){
//     String input = Serial.readStringUntil('\n'); //0.00 1.11 2.22 0.0
//     Kp = input.substring(0, 4).toFloat();
//     Ki = input.substring(5, 9).toFloat();
//     Kd = input.substring(10, 14).toFloat();
//     ms.power = input.substring(15, 18).toFloat();
    
//   }
// }

void followLine(){ 

  getLinePos();
  error = delta+pow(delta,3)/800;
  unsigned int dt = ms.time-PIDQue.times[(PIDQue.index-1)%QUEUE_LENGTH];
  if (dt <100000){
    integral += PIDQue.errors[(PIDQue.index-1)%QUEUE_LENGTH]*(dt)/1000000;
  }
  PIDQue.derivatives[PIDQue.index] = 1000000*(delta-PIDQue.errors[PIDQue.index])/(ms.time-PIDQue.times[PIDQue.index]); //!
  
  PIDQue.times[PIDQue.index] = ms.time;
  PIDQue.errors[PIDQue.index] = delta;
  PIDQue.index = (PIDQue.index + 1)%QUEUE_LENGTH;
  
  derivative = 0;
  for (uint8_t index=0; index<QUEUE_LENGTH; index++){derivative+=PIDQue.derivatives[index];}
  derivative/=QUEUE_LENGTH;
  ms.drive = true;
  ms.angvel = -(Kp*error + Ki*integral + Kd*derivative);
}


// static bool lostline = false;
// void resetLostLine(){  
//   lostline = false;
//   ms.drive = false;
// }

// void findLostLine(){
//   if (!lostline){
//     #ifdef DEBUG
//       Serial.println("errors: ");
//     #endif
//     ms.drive = true;
//     int avg = 0;
//     for (uint8_t i=0; i<QUEUE_LENGTH; i++){
//       avg+=PIDQue.errors[i];
//       #ifdef DEBUG
//         Serial.print(PIDQue.errors[i]); Serial.print(' ');}
//       #endif
//     ms.angvel = (avg>0) ? -255 : 255;
//     lostline = true;
//     #ifdef DEBUG
//       Serial.println(ms.angvel);
//     #endif
//   }
  
// }