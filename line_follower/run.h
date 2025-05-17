#ifndef RUN_H
#define RUN_H

#include "Arduino.h"
#include "constants.h"
#include "machine.h"

#include "line_follower.h"
#include "drive_moves.h"

enum run_state{
  START,
  FOLLOW_LINE,
  LOST_LINE,
  OBSTACLE,
  INTERSECTION,
  DEAD_END,
  FINISH
};

void run(bool intersectionAlg);
void resetRunMode();

#endif //RUN_H