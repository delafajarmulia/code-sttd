#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int pinLM35 = A0; 
const int pinLDR = A1;  

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  float suhu = (analogRead(pinLM35) * 500.0) / 1024.0;
  int nilaiLDR = analogRead(pinLDR);

  lcd.setCursor(0, 0);
  lcd.print("Suhu: "); lcd.print(suhu, 1); lcd.print(" C  ");
  lcd.setCursor(0, 1);
  lcd.print("LDR : "); lcd.print(nilaiLDR); lcd.print("      ");
\
  delay(500);
}