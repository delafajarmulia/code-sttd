#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define TRIGGER_PIN  12  // Hubungkan ke Pin 12 Arduino Uno
#define ECHO_PIN     13  // Hubungkan ke Pin 13 Arduino Uno
#define MAX_DISTANCE 200 // Batas maksimal sensor membaca (dalam cm)

const int TINGGI_TOTAL_TIANG = 200; 

// Sesuaikan alamat 0x3F dan ukuran LCD 20x4 sesuai hardware-mu
LiquidCrystal_I2C lcd(0x27, 20, 4);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// --- VARIABEL UNTUK TIMER 10 DETIK ---
unsigned long waktuSebelumnya = 0; 
// 10 detik = 10 x 1000 milidetik = 10000 ms
const unsigned long JEDA_BACA = 1000; 

int jarakBACA = 0;
int tinggiBadan = 0;

void setup() {
  Serial.begin(115200); // Jangan lupa Serial Monitor disetel ke 115200 Baud
  
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Pengukur Tinggi");
  lcd.setCursor(0, 1);
  lcd.print("  Badan  v1.0  ");
  delay(2000);
  lcd.clear();

  // Tampilan standby awal sebelum pemindaian pertama
  lcd.setCursor(0, 0);
  lcd.print("Jarak : Menunggu");
  lcd.setCursor(0, 1);
  lcd.print("Tinggi: 10 Detik");
}

void loop() {
  // Ambil waktu berjalan Arduino dalam milidetik
  unsigned long waktuSekarang = millis();

  // Cek apakah sudah lewat 10 detik (10000 ms)
  if (waktuSekarang - waktuSebelumnya >= JEDA_BACA) {
    waktuSebelumnya = waktuSekarang; // Reset penanda waktu

    // 1. Ambil data dari sensor ultrasonik
    jarakBACA = sonar.ping_cm(); 
    
    // 2. Hitung tinggi badan jika ada objek terdeteksi
    if (jarakBACA > 0 && jarakBACA < TINGGI_TOTAL_TIANG) {
      tinggiBadan = TINGGI_TOTAL_TIANG - jarakBACA;
    } else {
      tinggiBadan = 0; 
    }
    
    // 3. Cetak ke Serial Monitor setiap 10 detik
    Serial.println("=== PEMBACAAN 10 DETIK ===");
    Serial.print("Jarak Sensor: ");
    Serial.print(jarakBACA);
    Serial.print("cm | Tinggi Badan: ");
    Serial.print(tinggiBadan);
    Serial.println("cm\n");
    
    // 4. Perbarui Tampilan LCD I2C
    lcd.clear(); 
    
    lcd.setCursor(0, 0);
    lcd.print("Jarak : ");
    lcd.print(jarakBACA);
    lcd.print(" cm");
    
    lcd.setCursor(0, 1);
    if (jarakBACA > 0) {
      lcd.print("Tinggi: ");
      lcd.print(tinggiBadan);
      lcd.print(" cm");
    } else {
      lcd.print("Silahkan Berdiri");
    }
  }

  // Loop tetap berputar lancar tanpa delay konvensional yang membuat Arduino lag
}