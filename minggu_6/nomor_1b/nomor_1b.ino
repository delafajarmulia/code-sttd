#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int potPin = A0;

void setup() {
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Monitoring Nilai");
  delay(1000);
  lcd.clear();
}

void loop() {
  int adcValue = analogRead(potPin);
  
  int displayVal = map(adcValue, 0, 1023, 0, 9);

  lcd.setCursor(0, 0);
  lcd.print("Nilai ADC: ");
  lcd.print(adcValue);
  lcd.print("   "); 

  lcd.setCursor(0, 1);
  lcd.print("Output (0-9): ");
  lcd.print(displayVal);
  lcd.print("   ");

  delay(100); 
}