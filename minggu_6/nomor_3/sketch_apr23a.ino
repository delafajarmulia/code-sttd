#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int potPin = A0;
const int buttonPin = 2;
String message = " Anda tidak punya Akses";

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(buttonPin, INPUT);
  
  lcd.setCursor(0, 0);
  lcd.print("Sistem Siap");
  delay(1000);
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    // Jika tombol ditekan, tampilkan nilai ADC
    int adcValue = analogRead(potPin); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Akses Diterima");
    lcd.setCursor(0, 1);
    lcd.print("Nilai ADC: ");
    lcd.print(adcValue);
    delay(2000); 
  } 
  else {
    // Jika tombol tidak ditekan, pesan bergeser (Scrolling)
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sistem Terkunci");
    
    for (int i = 0; i < message.length(); i++) {
      if(digitalRead(buttonPin) == HIGH) break; 
      
      lcd.setCursor(0, 1);
      String displayMsg = message.substring(i) + message.substring(0, i);
      lcd.print(displayMsg.substring(0, 16));
      delay(3000);
    }
  }
}