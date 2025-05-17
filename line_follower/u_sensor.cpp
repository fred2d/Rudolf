#include "u_sensor.h"

//pin definition
const uint8_t trigPin = 34;
const uint8_t echoPin = 36;

// static bool calibrated = false;

static unsigned long startTime;
static bool pulseSent;
static int distance = 2000;

static unsigned long TIME_OFFSET = 2000;

static void uSensorISR(){
  if (pulseSent){
    distance = (micros()-startTime-TIME_OFFSET)*0.343/2;
  }
 pulseSent = false;
}

void uSensorSetup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  
  attachInterrupt(digitalPinToInterrupt(echoPin), uSensorISR, FALLING);
  #ifdef DEBUG
    Serial.println("Ultrasonic sensor setup completed.");
  #endif
}

void uSensorCalibration(){
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  startTime = micros();
  while (digitalRead(echoPin) != HIGH){
    
  }
  TIME_OFFSET = micros()-startTime;
  // calibrated = true;
  delay(40);

  #ifdef DEBUG
    Serial.println("Ultrasonic sensor calibration");
    Serial.println("time offset: "+String(TIME_OFFSET));
  #endif
}

int readUSensor(){
  // if (!calibrated) return distance;

  if(!pulseSent || micros()-startTime >50000){
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    startTime = micros();
    pulseSent = true;
  }

  return distance;
  
}