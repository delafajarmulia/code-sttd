#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

String teks = "43325207 - DELA FAJAR MULIA - D4TI POLINES";

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear(); 
}

void loop() {
  String padding = "                    "; 
  String fullTeks = padding + teks + padding;

  for (int i = 0; i < (fullTeks.length() - 20); i++) {
    lcd.setCursor(0, 1); 
    
    String displayTeks = fullTeks.substring(i, i + 20);
    
    lcd.print(displayTeks); 
    
    delay(350); 
  }
}