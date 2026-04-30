#include <Wire.h>

const int pinLM35 = A0;
int segPins[] = {2, 9, 4, 5, 6, 10, 8}; // a, b, c, d, e, f, g

// Pola angka 0-9 untuk Common Cathode (1 = Nyala)
byte digitFormat[10][7] = {
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
  for(int i = 0; i < 7; i++) pinMode(segPins[i], OUTPUT);
}

void loop() {
  long total = 0;
  for(int i=0; i<20; i++) {
    total += analogRead(pinLM35);
    delay(5);
  }
  float rawAvg = (float)total / 20;
  float suhu = (rawAvg * 500.0) / 1024.0;

  int satuan = (int)suhu % 10;
  
  for(int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], digitFormat[satuan][i]);
  }
  
  delay(500);
}