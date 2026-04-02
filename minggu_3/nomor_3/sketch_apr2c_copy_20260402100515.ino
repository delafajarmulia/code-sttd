int segA = 2;
int segB = 12;
int segC = 4;
int segD = 5;
int segE = 6;
int segF = 7;
int segG = 8;

int buttonPin = 3;
int currentNumber = 0;
unsigned long lastTime = 0; 
int interval = 1000;
bool wasPressed = false; // Untuk deteksi momen pertama tombol ditekan

void setup() {
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  
  pinMode(buttonPin, INPUT_PULLUP);
  
  displayDigit(currentNumber); // Tampil 0 statis saat pertama nyala
}

void loop() {
  bool isPressed = (digitalRead(buttonPin) == LOW);

  if (!isPressed) {
    if (!wasPressed) {
      lastTime = millis();
      wasPressed = true;
    }

    // Increment setelah 1 detik
    if (millis() - lastTime >= interval) {
      lastTime = millis();
      currentNumber++;
      if (currentNumber > 9) currentNumber = 0;
    }

    displayDigit(currentNumber);
  } 
  else {
    displayDigit(currentNumber);
    wasPressed = false;
  }
}

void displayDigit(int num) {
  digitalWrite(segA, HIGH); digitalWrite(segB, HIGH); digitalWrite(segC, HIGH);
  digitalWrite(segD, HIGH); digitalWrite(segE, HIGH); digitalWrite(segF, HIGH);
  digitalWrite(segG, HIGH);

  if (num == 0) number0();
  else if (num == 1) number1();
  else if (num == 2) number2();
  else if (num == 3) number3();
  else if (num == 4) number4();
  else if (num == 5) number5();
  else if (num == 6) number6();
  else if (num == 7) number7();
  else if (num == 8) number8();
  else if (num == 9) number9();
}

void number0(){ 
  digitalWrite(segA, LOW); digitalWrite(segB, LOW); digitalWrite(segC, LOW); 
  digitalWrite(segD, LOW); digitalWrite(segE, LOW); digitalWrite(segF, LOW); 
}

void number1(){ 
  digitalWrite(segB, LOW); digitalWrite(segC, LOW); 
}

void number2(){ 
  digitalWrite(segA, LOW); digitalWrite(segB, LOW); digitalWrite(segG, LOW); 
  digitalWrite(segE, LOW); digitalWrite(segD, LOW); 
}

void number3(){ 
  digitalWrite(segA, LOW); digitalWrite(segB, LOW); digitalWrite(segG, LOW); 
  digitalWrite(segC, LOW); digitalWrite(segD, LOW); 
}

void number4(){ 
  digitalWrite(segF, LOW); digitalWrite(segG, LOW); digitalWrite(segB, LOW);
  digitalWrite(segC, LOW); 
}

void number5(){ 
  digitalWrite(segA, LOW); digitalWrite(segF, LOW); digitalWrite(segG, LOW); 
  digitalWrite(segC, LOW); digitalWrite(segD, LOW);
}

void number6(){ 
  digitalWrite(segA, LOW); digitalWrite(segF, LOW); digitalWrite(segG, LOW); 
  digitalWrite(segE, LOW); digitalWrite(segC, LOW); digitalWrite(segD, LOW); 
}

void number7(){ 
  digitalWrite(segA, LOW); digitalWrite(segB, LOW); digitalWrite(segC, LOW); 
}

void number8(){ 
  digitalWrite(segA, LOW); digitalWrite(segB, LOW); digitalWrite(segC, LOW); 
  digitalWrite(segD, LOW); digitalWrite(segE, LOW); digitalWrite(segF, LOW); 
  digitalWrite(segG, LOW); 
}

void number9(){ 
  digitalWrite(segA, LOW); digitalWrite(segB, LOW); digitalWrite(segC, LOW);
  digitalWrite(segD, LOW); digitalWrite(segF, LOW); digitalWrite(segG, LOW); 
}