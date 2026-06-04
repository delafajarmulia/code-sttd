#include <NewPing.h>

#define TRIGGER_PIN  12  // Pin Trig sensor ke Pin 12 Arduino Uno
#define ECHO_PIN     13  // Pin Echo sensor ke Pin 13 Arduino Uno
#define MAX_DISTANCE 200 // Batas maksimal sensor membaca (200 cm) [cite: 159]

#define BUZZER_PIN   8   // Kaki positif (+) Buzzer ke Pin 8

// Inisialisasi NewPing ke objek bernama sonar [cite: 160]
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// --- PENGATURAN SENSITIVITAS JARAK ---
const int JARAK_DETEKSI = 20; // Jarak tamu berdiri agar bel berbunyi (dalam cm)

// --- VARIABEL TIMER NON-BLOCKING (MILLIS) ---
unsigned long waktuSebelumnyaSensor = 0;
const unsigned long JEDA_SENSOR = 60; // Sensor membaca ulang setiap 60ms [cite: 166]

unsigned long waktuSebelumnyaBuzzer = 0;
const unsigned long JEDA_NADA = 150;  // Jeda ketukan bunyi putus-putus bel (150ms)

int statusBuzzer = LOW;
int jarakBACA = 0;

void setup() {
  Serial.begin(115200); // Kecepatan komunikasi Serial Monitor 
  
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Pastikan buzzer mati saat awal menyala
  
  Serial.println("--- Sistem Bel Rumah Otomatis Tanpa LCD Siap ---");
}

void loop() {
  unsigned long waktuSekarang = millis();

  // 1. MEMBACA SENSOR ULTRASONIK SETIAP 60ms
  if (waktuSekarang - waktuSebelumnyaSensor >= JEDA_SENSOR) {
    waktuSebelumnyaSensor = waktuSekarang;
    
    jarakBACA = sonar.ping_cm(); // Mengambil hasil pemantauan objek (cm) [cite: 169]
    
    // Tampilkan data ke Serial Monitor untuk pengecekan
    if (jarakBACA > 0) {
      Serial.print("Terdeteksi Objek pada Jarak: ");
      Serial.print(jarakBACA);
      Serial.println(" cm");
    }
  }

  // 2. LOGIKA MEMBUNYIKAN BUZZER SECARA MANUAL (ANTI-CONFLICT TIMER 1)
  if (jarakBACA > 0 && jarakBACA <= JARAK_DETEKSI) {
    
    // Mengatur ketukan irama putus-putus bel (Bip... Bip... Bip...)
    if (waktuSekarang - waktuSebelumnyaBuzzer >= JEDA_NADA) {
      waktuSebelumnyaBuzzer = waktuSekarang;
      statusBuzzer = !statusBuzzer; // Membalik kondisi hidup-mati buzzer
    }

    // Jika ketukan berstatus aktif (HIGH), buat frekuensi suara bel secara manual
    if (statusBuzzer == HIGH) {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(500); // Frekuensi suara bel kasar sekitar ~1kHz
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(500);
    } else {
      digitalWrite(BUZZER_PIN, LOW); // Matikan buzzer di jeda ketukan
    }
    
  } else {
    // Jika tidak ada orang di depan sensor atau objek menjauh, bel mati total
    digitalWrite(BUZZER_PIN, LOW);
    statusBuzzer = LOW;
  }
}