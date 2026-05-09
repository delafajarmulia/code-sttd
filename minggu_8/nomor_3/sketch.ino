#include <Stepper.h>

const int stepsPerRev = 200;
Stepper myStepper(stepsPerRev, 8, 9, 10, 11);

const int btnKiri = 3;  // Pushbutton A
const int btnKanan = 4; // Pushbutton B

void setup() {
  pinMode(btnKiri, INPUT_PULLUP);
  pinMode(btnKanan, INPUT_PULLUP);
  myStepper.setSpeed(10);
}

void loop() {
  // Jika tombol A ditekan, putar ke kiri (nilai step negatif)
  if (digitalRead(btnKiri) == LOW) {
    myStepper.step(-1); 
  }
  // Jika tombol B ditekan, putar ke kanan (nilai step positif)
  else if (digitalRead(btnKanan) == LOW) {
    myStepper.step(1);  
  }
  // Jika tidak ada tombol ditekan, motor otomatis diam
}