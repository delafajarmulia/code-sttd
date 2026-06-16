#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Sistem siap! Tap kartu RFID Anda...");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    Serial.print("Angka ID Kartu Anda: {");

    // read 4 byte UID kartu dan menampilkannya dalam format desimal
    for (int i = 0; i < 4; i++) {
      Serial.print(mfrc522.uid.uidByte[i]);
      if (i < 3) Serial.print(", ");
    }
    Serial.println("}");

    Serial.println("Copy angka di atas ke dalam variabel 'cards' atau 'registeredCards'");
    Serial.println("--------------------------------------------------------------");

    mfrc522.PICC_HaltA(); // stop pembacaan agar tidak spam
  }
}