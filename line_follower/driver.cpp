#include "driver.h"

const uint8_t AIN1 = 18;
const uint8_t AIN2 = 16;
const uint8_t PWMA = 15;

const uint8_t BIN1 = 35;
const uint8_t BIN2 = 37;
const uint8_t PWMB = 39;

const uint8_t STBY = 33;

void driverSetup(){
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
}

static void motorA(int speed){
  if (speed>=0){
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }
  else{
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }
  
  analogWrite(PWMA, abs(speed));
}

static void motorB(int speed){
  if (speed>=0){
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }
  else{
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  }
  
  analogWrite(PWMB, abs(speed));
}

static void brake(){
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA,0);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB,0);
}

void drive2(int angular, bool fwd, bool enable, float power, vel_t *vel){
  if (!enable){
    brake();
    vel->fwd = 0;
    vel->ang = 0;
  }
  else {
    angular = constrain(angular, -255, 255);
    int extra = (255-abs(angular))*fwd;
    motorA((-angular + extra)*power);
    motorB((angular + extra)*power);

    vel->ang = float(angular*MAX_VEL)*power/WHEEL_DISTANCE*2/255;
    vel->fwd = float(extra*MAX_VEL)*power/255;
  }
}

// void drive(vel_t *target_vel, vel_t *vel){

//   if(target_vel->fwd == 0 && target_vel->ang ==0){
//     brake();
//     vel->fwd = 0;
//     vel->ang = 0;
//   } else{
//     int diff_speed = target_vel->ang*WHEEL_DISTANCE/2;
// //    if (diff_speed >MAX_VEL/2){
// //      diff_speed = MAX_VEL/2;
// //      ms.vel.fwd = 0;
// //      ms.vel.ang = diff_speed*2/WHEEL_DISTANCE;
// //    }
//     int max_speed = MAX_VEL;
//     if (abs(vel->fwd) + abs(diff_speed)>MAX_VEL){
//       motorA( map(vel->fwd + diff_speed, 0, abs(vel->fwd) + abs(diff_speed), 0, 255));
//       motorB( map(vel->fwd - diff_speed, 0, abs(vel->fwd) + abs(diff_speed), 0, 255));
//       vel->fwd = target_vel->fwd*MAX_VEL/abs(vel->fwd) + abs(diff_speed);
//       vel->ang = target_vel->ang*MAX_VEL/abs(vel->fwd) + abs(diff_speed);
//     }else{
//       motorA( map(vel->fwd + diff_speed, 0, MAX_VEL, 0, 255));
//       motorB( map(vel->fwd - diff_speed, 0, MAX_VEL, 0, 255));
//       vel->fwd = target_vel->fwd;
//       vel->ang = target_vel->ang;
//     }
//   }
// }