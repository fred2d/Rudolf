#include "constants.h"
#include "IR_sensor.h"
#include "u_sensor.h"
#include "inputs.h"
#include "driver.h"
#include "machine.h"
#include "run.h"

#ifdef DEBUG
  #include "serial_input.h"
  bool DEBUG_CLK = false;
#endif

void setup() {
  
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.println("started");
  #endif

  analogReadResolution(ADC_RESOLUTION); 

  irSensorSetup();
  uSensorSetup();
  inputsSetup();
  driverSetup();
}

void loop() {
  checkInputs();
  #ifdef DEBUG 
    readSerialInput();
    DEBUG_CLK = getDebugClock();
  #endif 
  readIRSensor(ms.irsv);
  ms.distance = readUSensor();
  updateState();

  switch (ms.mode){
    case RUN:
      run(1);
      // runDebug();
      break;
    case CALIBRATION:
      calibrate();
      break;
    case TEST:
      run(0);
      break;
    default:
      //bluetooth communication?
      #ifdef DEBUG
        idleDebug();
      #endif
      break;
  }
  // drive(&ms.targetVel, &ms.vel); 
  drive2(ms.angvel, ms.fwd, ms.drive, ms.power, &ms.vel);
}

#ifdef DEBUG
void test(){
  // futuere
}


bool getDebugClock(){
  static unsigned long time = millis();
  if (millis() - time > DEBUG_PERIOD){
    time = millis();
    return true;
  }
  return false;
}

void idleDebug(){
  static unsigned long time = millis();
  if (millis() - time > 1000){
    Serial.print("ir: ");
    for (uint8_t i=0; i<IR_SENSOR_COUNT; i++){
      Serial.print(ms.irsv[i]);
      // Serial.print("/");
      // Serial.print(ms.rawirsv[i]);
      Serial.print(" ");
    }
    // Serial.println();
    Serial.println("distance: "+ String(ms.distance));

    time = millis();
  }
}
// void runDebug(){
//   static unsigned long time = millis();
//   if (millis() - time > 20){
//     // Serial.print("ir: ");
//     // for (uint8_t i=0; i<IR_SENSOR_COUNT; i++){
//     //   Serial.print(ms.irsv[i]);
//     //   Serial.print(" ");
//     // // }
//     // Serial.print(" vel fwd: ");
//     // Serial.print(ms.vel.fwd);
//     // Serial.print(" ang: ");
//     // Serial.print(ms.vel.ang);
//     // Serial.print("      pos x: ");
//     // Serial.print(ms.pos.x);
//     // Serial.print(" y: ");
//     // Serial.print(ms.pos.y);
//     // Serial.print(" ang: ");
//     // Serial.println(ms.pos.ang);
//     time = millis();
//   }
// }
#endif

void calibrate(){
  irSensorCalibration();
  uSensorCalibration();
  ms.calibrated = true;
  changeMode(IDLE);
}