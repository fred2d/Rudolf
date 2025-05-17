#ifndef MACHINE_H
#define MACHINE_H

#include "Arduino.h"
#include "constants.h"
#include "run.h"

enum machine_mode{
  IDLE,
  CALIBRATION,
  RUN,
  TEST
};

typedef struct {
 pos_t pos;
 vel_t vel;
//  vel_t targetVel;

 int angvel;
 bool fwd = true;
 bool drive;
// float delta;

 int distance;
 uint8_t irsv[IR_SENSOR_COUNT];
 int rawirsv[IR_SENSOR_COUNT];
 bool calibrated = false;

 float power=0.30;
 
 enum machine_mode mode = IDLE;

 unsigned long time = micros();
} machine_state_t;

extern machine_state_t ms;

void updateState();
void changeMode(enum machine_mode newMode);

#endif //MACHINE_H