#include "machine.h"


machine_state_t ms;

void updateState(){
  unsigned long time = micros();
  unsigned long dt = time - ms.time;
  ms.pos.x -= ms.vel.fwd*sin(ms.pos.ang)*dt/1000000;
  ms.pos.y += ms.vel.fwd*cos(ms.pos.ang)*dt/1000000;
  ms.pos.fwd += ms.vel.fwd*dt/1000000;
  ms.pos.ang += ms.vel.ang*dt/1000000;
  // #ifdef DEBUG
  //   if (DEBUG_CLK){
  //     Serial.print("speed"); Serial.println(ms.vel.fwd);
  //     Serial.print("pos"); Serial.println(ms.pos.fwd);
  //   }
  // #endif
  ms.time = time;
}


#ifdef DEBUG
  static String name;
  static void modeToString(enum machine_mode mode){
    switch (mode){
      case RUN:
        name = "RUN"; 
        break;
      case CALIBRATION:
        name = "CALIBRATION";
        break;
      case TEST:
        name = "TEST";
        break;
      default:
        name = "IDLE";
        break;
    }
  }
#endif

void changeMode(enum machine_mode newMode){
  if (newMode == RUN || newMode == TEST){
    if (!ms.calibrated) {
      #ifdef DEBUG
        Serial.println("Warning: Calibrate before switching to RUN/TEST mode!");
      #endif
      return;
    }
  }
  switch (ms.mode){
    case RUN:
      resetRunMode();
      #ifdef DEBUG
        modeToString(newMode);
        Serial.println("RUN => " + name);
      #endif
      break; 
    case CALIBRATION:
      //reset calibration
      #ifdef DEBUG
        modeToString(newMode);
        Serial.println("CALIBRATION => " + name);
      #endif
      break;
    case TEST:
      #ifdef DEBUG
        modeToString(newMode);
        Serial.println("CALIBRATION => " + name);
      #endif
      break;
    default:
      #ifdef DEBUG
        modeToString(newMode);
        Serial.println("IDLE => " + name);
      #endif
      break;
  }
  ms.mode = newMode;
}