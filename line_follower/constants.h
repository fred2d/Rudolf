#ifndef CONSTANTS_H
#define CONSTANTS_H

//#define DEBUG
#ifdef DEBUG
  extern bool DEBUG_CLK;
  const int DEBUG_PERIOD = 50; //ms
#endif
#include "Arduino.h"
//--------machine constant definition-----------//
/* 
          (-)                     (+)
          +--------IR_SENSOR_COUNT-------+
          | [0] [1] [2] [3] [4] [5] |
          +---------+  i  +---|---|-+
                    |  r  | sensor pitch
                    |     |
                    |  s  |
                    |  e  |                     ++ -^-
                    |  n  |                     ||  | 
                    |  s  |                     ::  | m
                    |  o  |                     ||  | m
                    |  r  |                  1  ||  |                     
                    |     |                /    ||  | p
                    |  r  |               |  R  ||  | e
                    |  a  |                \ E  ||  | r
                    |  d  |                  V  ||  | 
                    |  i  |                     ||  | r
+-----+             |  u  |             +-----+ ||  | e
|     |             |  s  |             |     | ||  | v
|     +-------------+     +-------------+     | ||  |
|  1  |         wheel distance          |  2  | ++ _v_
|     +---------------------------------+     |
|     |                                 |     |
+-----+                                 +-----+
*/


//see constants.h file
const uint8_t IR_SENSOR_COUNT = 8;

const float NOMINAL_LINE_WIDTH = 20;
const float IR_SENSOR_PITCH = 8;
const float MM_PER_REV = 138;

const float WHEEL_DISTANCE = 103;
const float IR_SENSOR_RADIUS = 87;

const float IR_SENSOR_OFFSET = (IR_SENSOR_COUNT-1)*IR_SENSOR_PITCH/2;

const int MAX_VEL = 2000;

const uint8_t ADC_RESOLUTION = 9;
const uint8_t IR_SENSOR_RESOLUTION = 6;

const int MIN_OBSTACLE_DISTANCE = 250;

// structs
typedef struct {
  float x = 0;
  float y = 0;
  float fwd = 0;
  float ang = 0;
}pos_t;

typedef struct {
  float fwd = 0;
  float ang = 0;
}vel_t;

//pid parameters
extern float Kp;
extern float Ki;
extern float Kd;

extern float OBSTACLE_INIT_ANG;
extern uint8_t OBSTACLE_RETURN_ANGVEL;

#endif