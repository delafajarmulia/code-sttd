#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN  10
#define RST_PIN  9

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);   // alamat I2C: 0x27 atau 0x3F

int led   = 7;
int buzzer = 8;

// -------------------------------------------------------
// Ganti UID di bawah dengan UID kartu kamu yang
// muncul di Serial Monitor (format: {byte, byte, byte, byte})
// -------------------------------------------------------
byte cards[][4] = {
  {97, 67, 28, 23}    // Kartu 2 — ubah ini
};

// -------------------------------------------------------
// Nama pemilik kartu (urutan sama dengan array cards[])
// -------------------------------------------------------
String cardNames[] = {
  "Evan"
};

bool access = false;
int  matchedIndex = -1;

// ── Buzz helper ─────────────────────────────────────────
void buzz(int times, int onMs = 200, int offMs = 150) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzer, HIGH);
    delay(onMs);
    digitalWrite(buzzer, LOW);
    if (i < times - 1) delay(offMs);
  }
}

// ── Setup ────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.init();
  lcd.backlight();

  pinMode(led,    OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(led,    LOW);
  digitalWrite(buzzer, LOW);

  lcd.setCursor(0, 0);
  lcd.print("  Sistem Akses  ");
  lcd.setCursor(0, 1);
  lcd.print("  RFID Ready!  ");
  Serial.println("=== Sistem Akses RFID ===");
  Serial.println("Tempelkan kartu...");
  delay(2000);
  lcd.clear();
}

// ── Loop ─────────────────────────────────────────────────
void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Tempelkan Kartu!");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial())   return;

  // Tampilkan UID di Serial Monitor
  Serial.print("UID Terdeteksi: {");
  for (int i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i]);
    if (i < mfrc522.uid.size - 1) Serial.print(", ");
  }
  Serial.println("}");

  // Cocokkan UID
  access       = false;
  matchedIndex = -1;
  for (int x = 0; x < (int)(sizeof(cards) / sizeof(cards[0])); x++) {
    bool match = true;
    for (int i = 0; i < 4; i++) {
      if (mfrc522.uid.uidByte[i] != cards[x][i]) { match = false; break; }
    }
    if (match) { access = true; matchedIndex = x; break; }
  }

  lcd.clear();

  if (access) {
    Serial.println("Status : AKSES DITERIMA");
    Serial.print  ("Selamat datang, ");
    Serial.println(cardNames[matchedIndex]);
    Serial.println();

    lcd.setCursor(0, 0);
    lcd.print("Welcome!");
    lcd.setCursor(0, 1);
    lcd.print(cardNames[matchedIndex].substring(0, 16));

    digitalWrite(led, HIGH);
    buzz(1, 300);
    delay(700);
    digitalWrite(led, LOW);

  } else {
    Serial.println("Status : AKSES DITOLAK");
    Serial.println("Kartu tidak terdaftar!");
    Serial.println();

    lcd.setCursor(0, 0);
    lcd.print("  Not Allowed!  ");
    lcd.setCursor(0, 1);
    lcd.print(" Kartu Ditolak  ");

    for (int i = 0; i < 2; i++) {
      digitalWrite(led, HIGH); delay(300);
      digitalWrite(led, LOW);  delay(200);
    }
    buzz(2, 200, 150);
  }

  delay(2000);
  lcd.clear();
  mfrc522.PICC_HaltA();
}
