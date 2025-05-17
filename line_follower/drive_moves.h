#ifndef DRIVE_MOVES_H
#define DRIVE_MOVES_H

#include "Arduino.h"
#include "constants.h"
#include "machine.h"

extern bool checkLineFlag; 

void resetStart();
void callStart();

void deadEnd();

void initObstacle();
void callObstacle();
void resetObstacle();

void initIntersection();
bool callIntersection(uint8_t birsv, uint8_t pirsv, bool line);
void resetIntersection();

void followRight();

#endif //DRIVE_MOVES_H