/*
  =========================================================
  BRANKAS MULTI-FAKTOR (ESP32 DevKit)
  Tahapan : RFID -> PIN Keypad -> Kombinasi Potensiometer -> Servo Terbuka
  Override: DHT11 (suhu >=50C mengunci paksa & blokir semua tahap)

  ASUMSI DESAIN (silakan ubah kalau beda):
  - Kombinasi potensio dihitung KUMULATIF dari 0%:
      +30 -> target 30% | -20 -> target 10% | +45 -> target 55%
    Toleransi pembacaan +-3%. Salah di step manapun -> balik ke step 1.
  - Max salah PIN = 3x, max salah kombinasi potensio = 3x.
    Kalau kena max -> LOCKOUT 15 detik, lalu otomatis reset ke tahap RFID.
  - Suhu >=50C -> override TOTAL, servo dipaksa terkunci, semua tahap
    diabaikan, di state manapun posisi brankas saat itu.
  - Saat brankas terbuka, tekan '*' untuk kunci ulang & reset semua tahap.
  - OLED diasumsikan 1.3" I2C dengan driver SH1106 (128x64).
    Kalau modulmu SSD1306, ganti:
      #include <Adafruit_SH110X.h>      -> #include <Adafruit_SSD1306.h>
      Adafruit_SH1106G display(...)     -> Adafruit_SSD1306 display(...)
      display.begin(OLED_ADDR, true);   -> display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
      SH110X_WHITE                      -> SSD1306_WHITE
  =========================================================
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESP32Servo.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// ---------- PIN RFID (RC522 - SPI) ----------
#define RFID_SS_PIN   5
#define RFID_RST_PIN  4
// SCK=18, MISO=19, MOSI=23 -> pin default VSPI ESP32

// ---------- PIN OLED (I2C, 1.3" SH1106 128x64) ----------
#define OLED_SDA   21
#define OLED_SCL   22
#define OLED_ADDR  0x3C
#define SCREEN_W   128
#define SCREEN_H   64

// ---------- PIN SERVO ----------
#define SERVO_PIN     13
#define SERVO_LOCK    0     // sudut posisi terkunci
#define SERVO_UNLOCK  45    // sudut posisi terbuka

// ---------- PIN DHT11 ----------
#define DHT_PIN        27
#define DHT_TYPE       DHT11
#define HAZARD_TEMP    50.0   // ambang suhu bahaya (derajat C)
#define TEMP_INTERVAL  2000   // jeda baca DHT11 (ms), jangan terlalu sering

// ---------- PIN POTENSIOMETER ----------
#define POT_PIN        34     // ADC1, input only, cocok buat analogRead
#define POT_TOLERANSI  3      // toleransi % saat cocokkan target

// ---------- PIN KEYPAD 4x4 ----------
#define ROW1 14
#define ROW2 12
#define ROW3 26
#define ROW4 25
#define COL1 33
#define COL2 32
#define COL3 17
#define COL4 16

// ---------- OBJEK ----------
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
Adafruit_SH1106G display(SCREEN_W, SCREEN_H, &Wire, -1);
Servo servoBrankas;
DHT dht(DHT_PIN, DHT_TYPE);

byte rowPins[4] = {ROW1, ROW2, ROW3, ROW4};
byte colPins[4] = {COL1, COL2, COL3, COL4};
char keysMap[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
Keypad keypad = Keypad(makeKeymap(keysMap), rowPins, colPins, 4, 4);

// ---------- UID RFID YANG VALID ----------
byte uidValid[4] = {97, 67, 28, 23};

// ---------- PIN BRANKAS ----------
const String PIN_BENAR = "1379";
String pinInput = "";
int pinAttempt = 0;
const int PIN_MAX_ATTEMPT = 3;

// ---------- TAHAP KOMBINASI POTENSIOMETER ----------
// target kumulatif dari posisi 0%: +30 -> 30, -20 -> 10, +45 -> 55
const int potTarget[3] = {30, 10, 55};
const char* potLabel[3] = {"+30", "-20", "+45"};
int potStep = 0;
int potAttempt = 0;
const int POT_MAX_ATTEMPT = 3;

// ---------- STATE MACHINE ----------
enum SystemState { ST_RFID, ST_PIN, ST_POT, ST_UNLOCKED, ST_LOCKOUT };
SystemState state = ST_RFID;

bool hazardActive = false;
float suhuTerakhir = 0;
unsigned long lastTempCheck = 0;

unsigned long lockoutStart = 0;
const unsigned long LOCKOUT_DURATION = 15000; // 15 detik

// =========================================================
void setup() {
  Serial.begin(115200);

  // RFID
  SPI.begin();
  rfid.PCD_Init();

  // OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(OLED_ADDR, true);
  display.clearDisplay();
  display.display();

  // Servo
  servoBrankas.setPeriodHertz(50);
  servoBrankas.attach(SERVO_PIN, 500, 2400);
  servoBrankas.write(SERVO_LOCK);

  // DHT
  dht.begin();

  tampilkanPesan("BRANKAS SIAP", "Tempelkan RFID", "", "");
}

// =========================================================
void loop() {
  unsigned long now = millis();

  // 1) Cek suhu berkala (jangan baca DHT11 tiap loop, terlalu cepat)
  if (now - lastTempCheck >= TEMP_INTERVAL) {
    lastTempCheck = now;
    float t = dht.readTemperature();
    if (isnan(t)) {
      Serial.println("[DHT] Gagal baca (NaN) - cek wiring/pull-up");
    } else {
      suhuTerakhir = t;
      hazardActive = (t >= HAZARD_TEMP);
      Serial.print("[DHT] Suhu: ");
      Serial.print(t);
      Serial.print(" C | Hazard: ");
      Serial.println(hazardActive ? "AKTIF (servo dipaksa kunci)" : "tidak");
    }
  }

  // 2) Suhu bahaya -> override semua tahap, paksa terkunci
  if (hazardActive) {
    servoBrankas.write(SERVO_LOCK);
    tampilkanHazard();
    delay(150);
    return;
  }

  // 3) Jalankan tahap sesuai state aktif
  switch (state) {
    case ST_RFID:     prosesRFID();     break;
    case ST_PIN:      prosesPIN();      break;
    case ST_POT:      prosesPOT();      break;
    case ST_UNLOCKED: prosesUnlocked(); break;
    case ST_LOCKOUT:  prosesLockout();  break;
  }
}

// =========================================================
// TAHAP 1: RFID
// =========================================================
void prosesRFID() {
  tampilkanPesan("TAHAP 1/3", "Tempelkan RFID", "yang terdaftar", "");

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  bool cocok = (rfid.uid.size == 4);
  if (cocok) {
    for (int i = 0; i < 4; i++) {
      if (rfid.uid.uidByte[i] != uidValid[i]) { cocok = false; break; }
    }
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  if (cocok) {
    tampilkanPesan("RFID OK", "Lanjut ke PIN", "", "");
    delay(1000);
    state = ST_PIN;
  } else {
    tampilkanPesan("RFID SALAH", "Coba kartu lain", "", "");
    delay(1000);
  }
}

// =========================================================
// TAHAP 2: PIN KEYPAD
// =========================================================
void prosesPIN() {
  String mask = "";
  for (unsigned int i = 0; i < pinInput.length(); i++) mask += "*";

  tampilkanPesan("TAHAP 2/3", "Masukkan PIN:", mask,
                 "Sisa coba: " + String(PIN_MAX_ATTEMPT - pinAttempt));

  char k = keypad.getKey();
  if (!k) return;

  if (k >= '0' && k <= '9') {
    if (pinInput.length() < 8) pinInput += k;
  } else if (k == '*') {
    pinInput = ""; // tombol * = hapus input PIN
  } else if (k == '#') {
    if (pinInput == PIN_BENAR) {
      pinInput = "";
      pinAttempt = 0;
      tampilkanPesan("PIN BENAR", "Lanjut ke", "kombinasi", "potensiometer");
      delay(1200);
      state = ST_POT;
    } else {
      pinInput = "";
      pinAttempt++;
      if (pinAttempt >= PIN_MAX_ATTEMPT) {
        masukLockout("PIN salah 3x");
      } else {
        tampilkanPesan("PIN SALAH", "Sisa coba:",
                       String(PIN_MAX_ATTEMPT - pinAttempt), "");
        delay(1200);
      }
    }
  }
}

// =========================================================
// TAHAP 3: KOMBINASI POTENSIOMETER
// =========================================================
void prosesPOT() {
  int persen = bacaPotensioPersen();

  String judul = "TAHAP 3/3 (" + String(potStep + 1) + "/3)";
  String l1 = "Target: " + String(potLabel[potStep]) + " -> " + String(potTarget[potStep]) + "%";
  String l2 = "Posisi: " + String(persen) + "%";
  String l3 = "Tekan # | Sisa:" + String(POT_MAX_ATTEMPT - potAttempt);
  tampilkanPesan(judul, l1, l2, l3);

  char k = keypad.getKey();
  if (k != '#') return;

  int target = potTarget[potStep];
  if (abs(persen - target) <= POT_TOLERANSI) {
    potStep++;
    if (potStep >= 3) {
      potStep = 0;
      potAttempt = 0;
      state = ST_UNLOCKED;
      servoBrankas.write(SERVO_UNLOCK);
      tampilkanPesan("KOMBINASI OK", "BRANKAS", "TERBUKA", "");
      delay(1000);
    } else {
      tampilkanPesan("LANGKAH OK", "Lanjut langkah", String(potStep + 1) + "/3", "");
      delay(800);
    }
  } else {
    potStep = 0;
    potAttempt++;
    if (potAttempt >= POT_MAX_ATTEMPT) {
      masukLockout("Kombinasi salah 3x");
    } else {
      tampilkanPesan("SALAH!", "Ulangi dari", "langkah 1/3",
                     "Sisa:" + String(POT_MAX_ATTEMPT - potAttempt));
      delay(1200);
    }
  }
}

int bacaPotensioPersen() {
  int raw = analogRead(POT_PIN);   // 0 - 4095 (ADC 12 bit ESP32)
  return map(raw, 0, 4095, 0, 100);
}

// =========================================================
// TAHAP TERBUKA
// =========================================================
void prosesUnlocked() {
  tampilkanPesan("BRANKAS TERBUKA", "Tekan *", "utk kunci ulang", "");
  servoBrankas.write(SERVO_UNLOCK);

  char k = keypad.getKey();
  if (k == '*') {
    servoBrankas.write(SERVO_LOCK);
    pinInput = ""; pinAttempt = 0;
    potStep = 0; potAttempt = 0;
    state = ST_RFID;
    tampilkanPesan("BRANKAS", "DIKUNCI ULANG", "", "");
    delay(1000);
  }
}

// =========================================================
// LOCKOUT (terlalu banyak salah)
// =========================================================
void masukLockout(String alasan) {
  state = ST_LOCKOUT;
  lockoutStart = millis();
  tampilkanPesan("TERKUNCI!", alasan, "Tunggu 15 detik", "");
  delay(1000);
}

void prosesLockout() {
  servoBrankas.write(SERVO_LOCK);

  unsigned long elapsed = millis() - lockoutStart;
  if (elapsed >= LOCKOUT_DURATION) {
    pinInput = ""; pinAttempt = 0;
    potStep = 0; potAttempt = 0;
    state = ST_RFID;
    tampilkanPesan("RESET", "Silakan ulangi", "dari RFID", "");
    delay(1000);
  } else {
    unsigned long sisaDetik = (LOCKOUT_DURATION - elapsed) / 1000 + 1;
    tampilkanPesan("TERKUNCI!", "Tunggu...", String(sisaDetik) + " detik", "");
  }
}

// =========================================================
// TAMPILAN OLED
// =========================================================
void tampilkanPesan(String judul, String l1, String l2, String l3) {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(judul);
  display.drawLine(0, 10, SCREEN_W, 10, SH110X_WHITE);

  display.setCursor(0, 16);
  display.println(l1);
  display.setCursor(0, 30);
  display.println(l2);
  display.setCursor(0, 44);
  display.println(l3);

  display.display();
}

void tampilkanHazard() {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("!! BAHAYA SUHU !!");
  display.drawLine(0, 10, SCREEN_W, 10, SH110X_WHITE);

  display.setCursor(0, 20);
  display.print("Suhu: ");
  display.print(suhuTerakhir, 1);
  display.println(" C");

  display.setCursor(0, 35);
  display.println("Brankas dikunci");
  display.setCursor(0, 47);
  display.println("paksa, semua tahap");
  display.setCursor(0, 56);
  display.println("diblokir");

  display.display();
}
