#include <Stepper.h>

const int stepsPerRev = 200; 
Stepper myStepper(stepsPerRev, 8, 9, 10, 11);

const int btnPin = 2;
bool isRunning = false;    // by default, motor tidak berjalan
bool lastBtnState = HIGH; 

void setup() {
  pinMode(btnPin, INPUT_PULLUP); 
  myStepper.setSpeed(10);        
}

void loop() {
  bool currentBtnState = digitalRead(btnPin);
  
  if (currentBtnState == LOW && lastBtnState == HIGH) {
    isRunning = !isRunning; 
    delay(200);             
  }
  lastBtnState = currentBtnState;

  if (isRunning) {
    myStepper.step(1); 
  }
}