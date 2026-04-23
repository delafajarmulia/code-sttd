const int potPin = A0;
const int segPins[] = {2, 3, 4, 5, 6, 7, 8};

// Pola angka 0-9 untuk Common Cathode
byte numbers[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

void setup() {
  for(int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }
}

void loop() {
  int sensorValue = analogRead(potPin); 
  int displayVal = map(sensorValue, 0, 1023, 0, 9); 

  displayDigit(displayVal);
  delay(100);
}

void displayDigit(int num) {
  for(int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], numbers[num][i]);
  }
}