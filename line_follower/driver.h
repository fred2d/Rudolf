#ifndef DRIVER_H
#define DRIVER_H

#include "Arduino.h"
#include "constants.h"

void driverSetup();
// void drive(vel_t *target_vel, vel_t *vel);
void drive2(int angular, bool maxfwd, bool enable, float power, vel_t *vel);

#endif //DRIVER_H