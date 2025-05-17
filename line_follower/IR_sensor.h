#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include "arduino.h"
#include "constants.h"
// #include "machine.h"

void irSensorSetup();
void irSensorCalibration();
void readIRSensor(uint8_t irValues[]);

#endif //IR_SENSOR_H