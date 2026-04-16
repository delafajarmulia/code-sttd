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

// pin led
const int ledHijau = 10;
const int ledMerah = 11;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String inputPassword = "";
String correctPassword = "1234"; 

void setup() {
  pinMode(ledHijau, OUTPUT);
  pinMode(ledMerah, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  displayInputPrompt();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == 'd') { // enter
      checkPassword();
    } 
    else if (key == '*') { // reset
      resetInput();
    } 
    else { 
      if (inputPassword.length() < 16) {
        inputPassword += key;
        lcd.setCursor(inputPassword.length() - 1, 1);
        lcd.print("*"); // hide
      }
    }
  }
}

void checkPassword() {
  lcd.clear();
  if (inputPassword == correctPassword) {
    digitalWrite(ledHijau, HIGH);
    lcd.print("Selamat Datang");
    delay(3000); 
    digitalWrite(ledHijau, LOW);
  } else {
    digitalWrite(ledMerah, HIGH);
    lcd.print("Password Salah");
    delay(3000); 
    digitalWrite(ledMerah, LOW);
  }
  resetInput();
}

void resetInput() {
  inputPassword = "";
  lcd.clear();
  displayInputPrompt();
}

void displayInputPrompt() {
  lcd.setCursor(0, 0);
  lcd.print("Masukan Password:");
}