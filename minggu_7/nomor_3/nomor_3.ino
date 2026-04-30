#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int pinLM35 = A0;
const int btnCelcius = 9;    
const int btnFahrenheit = 10; 

bool isFahrenheit = false;

void setup() {
  lcd.init();
  lcd.backlight();
  
  // Menggunakan internal pull-up untuk kedua tombol
  pinMode(btnCelcius, INPUT_PULLUP);
  pinMode(btnFahrenheit, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("Pilih Satuan...");
}

void loop() {
  // pakainya rata-rata
  long totalRaw = 0;
  for (int i = 0; i < 20; i++) {
    totalRaw += analogRead(pinLM35);
    delay(5); 
  }
  float rawData = totalRaw / 20.0; 
  
  float celcius = (rawData * 500.0) / 1024.0;
  float fahrenheit = (celcius * 9.0 / 5.0) + 32.0;

  if (digitalRead(btnCelcius) == LOW) {
    isFahrenheit = false;
    lcd.clear();
  }
  
  if (digitalRead(btnFahrenheit) == LOW) {
    isFahrenheit = true;
    lcd.clear();
  }

  lcd.setCursor(0, 0);
  lcd.print("Monitoring Suhu");
  
  lcd.setCursor(0, 1);
  if (isFahrenheit) {
    lcd.print("Temp: "); 
    lcd.print(fahrenheit, 1); 
    lcd.print(" F  ");
  } else {
    lcd.print("Temp: "); 
    lcd.print(celcius, 1); 
    lcd.print(" C  ");
  }

  delay(200); 
}