#include <Stepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int stepsPerRev = 200;
Stepper myStepper(stepsPerRev, 8, 9, 10, 11);

LiquidCrystal_I2C lcd(0x27, 16, 2); 

long totalSteps = 0;
unsigned long waktuTerakhir = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  myStepper.setSpeed(1);
}

void loop() {
  myStepper.step(1);  
  totalSteps++;       

  if (millis() - waktuTerakhir > 300) {
    float derajat = (totalSteps % stepsPerRev) * (360.0 / stepsPerRev);

    lcd.setCursor(0, 0);
    lcd.print("Step: "); 
    lcd.print(totalSteps);

    lcd.setCursor(0, 1);
    lcd.print("Derajat: "); 
    lcd.print(derajat);
    
    waktuTerakhir = millis();
  }
}