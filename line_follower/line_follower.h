#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H

#include "Arduino.h"
#include "constants.h"
#include "machine.h"

#define QUEUE_LENGTH 32
typedef struct{
  float derivatives[QUEUE_LENGTH];
  float errors[QUEUE_LENGTH];
  unsigned long times[QUEUE_LENGTH];
  uint8_t index=0;
}pidq_t;

void resetLineFollower();

void followLine();
// void findLostLine();
// void resetLostLine();
// void tunePID();

#endif //LINE_FOLLOWER_H