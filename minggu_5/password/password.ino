#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'a'}, 
  {'4', '5', '6', 'b'},
  {'7', '8', '9', 'c'}, 
  {'*', '0', '#', 'd'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {9, 8, 7, 6};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String inputPassword = "";
String correctPassword = "1234"; 

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Masukkan Pin:");
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    if (key == 'd') { 
      lcd.clear();
      if (inputPassword == correctPassword) {
        lcd.print("Akses Diterima!");
      } else {
        lcd.print("Pin Salah!");
      }
      delay(2000);
      lcd.clear();
      lcd.print("Masukkan Pin:");
      inputPassword = "";
    } else if (key == '*') { // reset
      inputPassword = "";
      lcd.setCursor(0, 1);
      lcd.print("                ");
    } else {
      inputPassword += key;
      lcd.setCursor(0, 1);
      lcd.print(inputPassword);
    }
  }
}