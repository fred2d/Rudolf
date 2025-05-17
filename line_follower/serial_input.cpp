#include "serial_input.h"

static char rxByte;

static float readFloat(){
  String number = "";
  bool reading = true;
  bool seenDigit = false;
  while (reading){
    rxByte = Serial.read();
    if (isDigit(rxByte) || rxByte == '.' || rxByte == '-'){
      number += rxByte;
      seenDigit = true;
    } else if (isAlpha(rxByte) || seenDigit) {
      reading = false;
    }
  }
  return number.toFloat();
}

static int readInt(){
  return 0;
}

void readSerialInput(){
  while (Serial.available()){
    rxByte = Serial.read();
    switch(rxByte){
      case 'P':
        Kp = readFloat();
        Serial.println("Kp=" + String(Kp));
        break;
      case 'I':
        Ki = readFloat();
        Serial.println("Ki=" + String(Ki));
        break;
      case 'D':
        Kd = readFloat();
        Serial.println("Kd=" + String(Kd));
        break;
      case 'W':
        ms.power = readFloat();
        Serial.println("power=" + String(ms.power));
        break;
      case 'T':
        OBSTACLE_INIT_ANG = readFloat();
        Serial.println("obstacle init ang=" + String(OBSTACLE_INIT_ANG));
        break;
      case 'R':
        OBSTACLE_RETURN_ANGVEL = int(readFloat());
        Serial.println("obstacle return angvel=" + String(OBSTACLE_RETURN_ANGVEL));
        break;
    }
  }
}