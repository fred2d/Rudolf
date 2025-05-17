#include "IR_sensor.h"

//pin definition
const uint8_t enablePin = 1;
const uint8_t sensorPins[IR_SENSOR_COUNT] = {10,8,6,4,9,7,5,3};

static bool calibrated = false;

static int IR_SENSOR_WHITE[IR_SENSOR_COUNT] = {0};
static int IR_SENSOR_BLACK[IR_SENSOR_COUNT] = {0};

static const int blackThreshold = (1<<(ADC_RESOLUTION)) * 3/4;
static const int whiteThreshold = 1<<(ADC_RESOLUTION-2);

void irSensorSetup(){
  //for(uint8_t i=0; i<IR_SENSOR_COUNT;i++){IR_SENSOR_WHITE[i] = (1<<ADC_RESOLUTION) - 1;}
  for(uint8_t i=0; i<IR_SENSOR_COUNT;i++){ pinMode(sensorPins[i], INPUT);}
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);

  #ifdef DEBUG
    Serial.println("IR sensor setup completed.");
  #endif
}

void irSensorCalibration(){
  #ifdef DEBUG
    Serial.println("start IR sensor calibration");
  #endif
  for(uint8_t i=0; i<IR_SENSOR_COUNT;i++){
    IR_SENSOR_WHITE[i] = (1<<ADC_RESOLUTION) - 1;
    IR_SENSOR_BLACK[i] = 0;
  }
  delay(1000);
  unsigned long start = millis();
  while (millis() - start < 5000){
    for(uint8_t i=0; i<IR_SENSOR_COUNT;i++){
      int rawval = analogRead(sensorPins[i]);
      if(rawval < IR_SENSOR_WHITE[i]){IR_SENSOR_WHITE[i] = rawval;} 
      if(rawval > IR_SENSOR_BLACK[i]){IR_SENSOR_BLACK[i] = rawval;}
    }
    delay(10);
  }
  calibrated = true;


  #ifdef DEBUG
    for (uint8_t i=0; i<IR_SENSOR_COUNT; i++){
      Serial.print(IR_SENSOR_WHITE[i]);
      Serial.print("/");
      Serial.print(IR_SENSOR_BLACK[i]);
      Serial.print(" ");
    }
    Serial.println();
  #endif

}


void readIRSensor(uint8_t irValues[]){
  if (!calibrated) return;


  int white = 0; //(1<<ADC_RESOLUTION-2);
  int black = 0; //(1<<(ADC_RESOLUTION))- (1<<(ADC_RESOLUTION-2));
  uint8_t countW = 0; uint8_t countB = 0;
  
  int rawIrValues[IR_SENSOR_COUNT];

  for(uint8_t i=0; i<IR_SENSOR_COUNT;i++){
    rawIrValues[i] = map(analogRead(sensorPins[i]), IR_SENSOR_WHITE[i], IR_SENSOR_BLACK[i], 0, (1<<ADC_RESOLUTION)-1);
    // irValues[i] = map(analogRead(sensorPins[i]), IR_SENSOR_WHITE[i], IR_SENSOR_BLACK[i], 0, (1<<IR_SENSOR_RESOLUTION) -1);
    if (rawIrValues[i] <= whiteThreshold){white += rawIrValues[i]; countW++;}
    if (rawIrValues[i] >= blackThreshold){black += rawIrValues[i]; countB++;}
    // if (rawIrValues[i] < white){white = rawIrValues[i];}
    // if (rawIrValues[i] > black){black = rawIrValues[i];}
    // ms.rawirsv[i] = rawIrValues[i];
  }


  white = (countW >0) ? white/countW : 0;
  black = (countB >0) ? black/countB : (1<<ADC_RESOLUTION)-1;

  for(uint8_t i=0; i<IR_SENSOR_COUNT;i++){ 
    irValues[i] = constrain(map(rawIrValues[i], white, black, 0, (1<<IR_SENSOR_RESOLUTION) -1), 0, (1<<IR_SENSOR_RESOLUTION) -1);
  }

  // static unsigned long time = millis();
  // if (millis() - time > 500){
  //   Serial.print("ir: ");
  //   for (uint8_t i=0; i<IR_SENSOR_COUNT; i++){
  //     Serial.print(irValues[i]);
  //     Serial.print("/");
  //     Serial.print(rawIrValues[i]);
  //     Serial.print("  ");
  //   }
  //   Serial.println("white: "+ String(white) + " black: " + String(black));

  //   time = millis();
  // }
}