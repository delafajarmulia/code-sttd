/*
 * Kotak Surat Otomatis + RFID
 * Kelompok 4 — Servo + LED + Ultrasonic + RFID
 *
 * Alur:
 *  1. Tap kartu RFID → jika terdaftar, servo BUKA, LED hijau ON
 *  2. Ultrasonik mendeteksi surat masuk (jarak < threshold)
 *  3. Setelah surat masuk, servo TUTUP otomatis
 *  4. Kartu tidak terdaftar → LED merah kedip, servo tetap tutup
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// ── Pin mapping ──────────────────────────────────────────
#define SS_PIN  10
#define RST_PIN  9

const int PIN_SERVO   =  6;
const int PIN_LED_HIJ =  7;
const int PIN_LED_MER =  8;
const int PIN_TRIG    =  2;
const int PIN_ECHO    =  3;

// ── Parameter ────────────────────────────────────────────
const int  THRESHOLD_CM  = 15;
const int  ANGLE_OPEN    = 90;
const int  ANGLE_CLOSE   =  0;
const long OPEN_DURATION = 5000;  // Timeout tutup otomatis (ms)

// ── Kartu terdaftar ──────────────────────────────────────
byte cards[][4] = {
  {17, 34, 51, 68}   // Kartu kamu
};

String cardNames[] = {
  "Pemilik Kotak"
};

// ── State ────────────────────────────────────────────────
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo   servo;

bool isOpen       = false;
long openedAtMs   = 0;

// ── Helpers ──────────────────────────────────────────────
long readDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  long duration = pulseIn(PIN_ECHO, HIGH, 30000L);
  if (duration == 0) return 999;
  return duration / 58L;
}

bool isCardRegistered(int &matchedIndex) {
  for (int x = 0; x < (int)(sizeof(cards) / sizeof(cards[0])); x++) {
    bool match = true;
    for (int i = 0; i < 4; i++) {
      if (mfrc522.uid.uidByte[i] != cards[x][i]) { match = false; break; }
    }
    if (match) { matchedIndex = x; return true; }
  }
  return false;
}

void openBox(String name) {
  servo.write(ANGLE_OPEN);
  digitalWrite(PIN_LED_HIJ, HIGH);
  digitalWrite(PIN_LED_MER, LOW);
  isOpen     = true;
  openedAtMs = millis();
  Serial.println("[BUKA] Akses diterima: " + name);
  Serial.println("       Silakan masukkan surat...");
}

void closeBox() {
  servo.write(ANGLE_CLOSE);
  digitalWrite(PIN_LED_HIJ, LOW);
  digitalWrite(PIN_LED_MER, HIGH);
  isOpen = false;
  Serial.println("[TUTUP] Kotak ditutup.");
  Serial.println("        Tap kartu untuk membuka kembali.");
  Serial.println("--------------------------------------");
}

void rejectCard() {
  Serial.println("[TOLAK] Kartu tidak terdaftar!");
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_LED_MER, LOW);
    delay(200);
    digitalWrite(PIN_LED_MER, HIGH);
    delay(200);
  }
}

// ── Setup ────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  delay(500);

  SPI.begin();
  mfrc522.PCD_Init();

  servo.attach(PIN_SERVO);
  servo.write(ANGLE_CLOSE);

  pinMode(PIN_LED_HIJ, OUTPUT);
  pinMode(PIN_LED_MER, OUTPUT);
  pinMode(PIN_TRIG,    OUTPUT);
  pinMode(PIN_ECHO,    INPUT);

  digitalWrite(PIN_LED_HIJ, LOW);
  digitalWrite(PIN_LED_MER, HIGH);

  Serial.println("=== Kotak Surat Otomatis ===");
  Serial.println("Tap kartu RFID untuk membuka kotak.");
}

// ── Loop ─────────────────────────────────────────────────
void loop() {
  long now = millis();

  // Jika kotak sedang terbuka
  if (isOpen) {
    long distanceCm = readDistanceCm();
    Serial.print("Jarak: ");
    Serial.print(distanceCm);
    Serial.println(" cm");

    // Tutup jika surat terdeteksi masuk
    if (distanceCm < THRESHOLD_CM) {
      Serial.println("       Surat masuk!");
      delay(1000);
      closeBox();
      return;
    }

    // Tutup otomatis jika timeout
    if (now - openedAtMs >= OPEN_DURATION) {
      Serial.println("[TIMEOUT] Tidak ada surat. Menutup kotak...");
      closeBox();
      return;
    }

    delay(200);
    return;
  }

  // Kotak tertutup — tunggu tap kartu RFID
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial())   return;

  Serial.print("UID: {");
  for (int i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i]);
    if (i < mfrc522.uid.size - 1) Serial.print(", ");
  }
  Serial.println("}");

  int matchedIndex = -1;
  if (isCardRegistered(matchedIndex)) {
    openBox(cardNames[matchedIndex]);
  } else {
    rejectCard();
  }

  mfrc522.PICC_HaltA();
  delay(500);
}