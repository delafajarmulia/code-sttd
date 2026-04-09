#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

String nim = "43325215";
String nama = "MUHAMMAD FADHIL";

void setup() {
  lcd.init();
  lcd.backlight();

  if (nim.length() > 20) nim = nim.substring(0, 20);
  int pos1 = (20 - nim.length()) / 2;
  lcd.setCursor(pos1, 1);
  lcd.print(nim);

  if (nama.length() > 20) nama = nama.substring(0, 20);
  int pos2 = (20 - nama.length()) / 2;
  lcd.setCursor(pos2, 2);
  lcd.print(nama);
}

void loop() {}