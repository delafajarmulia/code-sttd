#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', '+'}, 
  {'4', '5', '6', '-'}, 
  {'7', '8', '9', '*'}, 
  {'C', '0', '=', '/'}  
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {9, 8, 7, 6};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

long num1 = 0, num2 = 0;
char oper;
bool isSecondNum = false;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Kalkulator Ready");
  delay(1000);
  lcd.clear();
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    if (key >= '0' && key <= '9') {
      if (!isSecondNum) {
        num1 = (num1 * 10) + (key - '0');
        lcd.setCursor(0, 0); lcd.print(num1);
      } else {
        num2 = (num2 * 10) + (key - '0');
        lcd.setCursor(8, 0); lcd.print(num2);
      }
    } else if (key == '+' || key == '-' || key == '*' || key == '/') {
      oper = key;
      isSecondNum = true;
      lcd.setCursor(6, 0); lcd.print(oper);
    } else if (key == '=') {
      long result = 0;
      if (oper == '+') result = num1 + num2;
      else if (oper == '-') result = num1 - num2;
      else if (oper == '*') result = num1 * num2;
      else if (oper == '/') result = (num2 != 0) ? num1 / num2 : 0;

      lcd.setCursor(0, 1);
      lcd.print("Hasil: ");
      lcd.print(result); 
    } else if (key == 'C') {
      num1 = 0; num2 = 0; isSecondNum = false;
      lcd.clear();
    }
  }
}