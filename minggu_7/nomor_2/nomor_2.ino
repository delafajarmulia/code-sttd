#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int pinLDR = A1;
const int pinBuzzer = 10;
const int pinLED = 13;
const int pinButton = 9;

bool alarmAktif = false;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLED, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP); 
}

void loop() {
  int dataLDR = analogRead(pinLDR);

  if (dataLDR > 500) { 
    alarmAktif = true;
    lcd.setCursor(0, 0);
    lcd.print("ADA MALING!!");
  }

  if (alarmAktif) {
    digitalWrite(pinLED, HIGH);
    tone(pinBuzzer, 1000); 
    delay(200);
    digitalWrite(pinLED, LOW);
    noTone(pinBuzzer);
    delay(200);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Kondisi Aman    ");
    digitalWrite(pinLED, LOW);
    noTone(pinBuzzer);
  }

  if (digitalRead(pinButton) == LOW) {
    alarmAktif = false;
    lcd.clear();
    delay(500); 
  }
}