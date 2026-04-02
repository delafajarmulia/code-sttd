int segA = 2;
int segB = 12;
int segC = 4;
int segD = 5;
int segE = 6;
int segF = 7;
int segG = 8;

void setup() {
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
}

void loop() {
  displayDigit(0); delay(1000);
  displayDigit(1); delay(1000);
  displayDigit(2); delay(1000);
  displayDigit(3); delay(1000);
  displayDigit(4); delay(1000);
  displayDigit(5); delay(1000);
  displayDigit(6); delay(1000);
  displayDigit(7); delay(1000);
  displayDigit(8); delay(1000);
  displayDigit(9); delay(1000);
}

void displayDigit(int num) {
  digitalWrite(segA, HIGH);
  digitalWrite(segB, HIGH);
  digitalWrite(segC, HIGH);
  digitalWrite(segD, HIGH);
  digitalWrite(segE, HIGH);
  digitalWrite(segF, HIGH);
  digitalWrite(segG, HIGH);

  if (num == 0) {
    number0();
  }
  else if (num == 1) {
    number1();
  }
  else if (num == 2) {
    number2();
  }
  else if (num == 3) {
    number3();
  }
  else if (num == 4) {
    number4();
  }
  else if (num == 5) {
    number5();
  }
  else if (num == 6) {
    number6();
  }
  else if (num == 7) {
    number7();
  }
  else if (num == 8) {
    number8();
  }
  else if (num == 9) {
    number9();
  }
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