#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Definisi Pin Tombol
const int btnBenar = 8;
const int btnSalah = 7;

void setup() {
  lcd.init();
  lcd.backlight();
  
  pinMode(btnBenar, INPUT_PULLUP);
  pinMode(btnSalah, INPUT_PULLUP);

  tampilkanPesanUtama();
}

void loop() {
  if (digitalRead(btnBenar) == LOW) {
    delay(50); 
    if (digitalRead(btnBenar) == LOW) { 
      aksiPassword("SELAMAT DATANG");
      while(digitalRead(btnBenar) == LOW); 
    }
  }

  // Cek Button 2 (Password Salah)
  if (digitalRead(btnSalah) == LOW) {
    delay(50); // Debounce
    if (digitalRead(btnSalah) == LOW) {
      aksiPassword("PASSWORD SALAH");
      while(digitalRead(btnSalah) == LOW); 
    }
  }
}

void tampilkanPesanUtama() {
  lcd.clear();
  String baris1 = "Masukkan Password";
  int pos1 = (20 - baris1.length()) / 2;
  lcd.setCursor(pos1, 1); 
  lcd.print(baris1);
}

void aksiPassword(String pesan) {
  lcd.clear();
  int pos = (20 - pesan.length()) / 2;
  lcd.setCursor(pos, 1); 
  lcd.print(pesan);
  
  delay(2000); 
  tampilkanPesanUtama(); 
}