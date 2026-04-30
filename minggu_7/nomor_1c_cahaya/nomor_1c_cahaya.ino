#include <Wire.h>

const int pinLDR = A1;
int segPins[] = {2, 9, 4, 5, 6, 10, 8}; 

byte digitFormat[10][7] = {
  {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1},
  {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1},
  {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1},
  {1,1,1,1,0,1,1}
};

void setup() {
  Serial.begin(9600); 
  for(int i = 0; i < 7; i++) pinMode(segPins[i], OUTPUT);
}

void loop() {
  // ambil rata-rata
  long total = 0;
  for(int i=0; i<10; i++) {
    total += analogRead(pinLDR);
    delay(5);
  }
  int nilaiLDR = total / 10;

  Serial.print("Nilai LDR: ");
  Serial.println(nilaiLDR);

  // range cahaya
  int level = map(nilaiLDR, 50, 150, 0, 9); 
  level = constrain(level, 0, 9);

  for(int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], digitFormat[level][i]);
  }

  delay(200);
}