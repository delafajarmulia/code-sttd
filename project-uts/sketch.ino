#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// config keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// pin Komponen
const int LDR_PIN = A0;
const int LED_PIN = 13;
const int BUZZER_PIN = 10;

String passwordBenar = "1234";
String inputPassword = "";
bool alarmTerpicu = false;    
bool sudahMatikanAlarm = false; 
int ambangCahaya = 500; 

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.print("Sistem Aktif");
  delay(1500);
  lcd.clear();
}

void loop() {
  int nilaiLDR = analogRead(LDR_PIN);
  char key = keypad.getKey();

  if (nilaiLDR < ambangCahaya) { 
    // kondisi gelap
    digitalWrite(LED_PIN, HIGH);   
    noTone(BUZZER_PIN);            
    alarmTerpicu = false;          
    sudahMatikanAlarm = false;     // reset flag agar nanti bisa bunyi lagi saat terang
    inputPassword = "";            
  } 
  else { 
    // kondisi terag
    if (!sudahMatikanAlarm) {      
      alarmTerpicu = true;         
    }
  }

  // logic alarm terkunci
  if (alarmTerpicu) {
    digitalWrite(LED_PIN, HIGH);   
    tone(BUZZER_PIN, 1000);        
    
    updateLCD(nilaiLDR, "ON ", "ALARM");

    if (key) {
      if (key == '#') { 
        if (inputPassword == passwordBenar) {
          alarmTerpicu = false;
          sudahMatikanAlarm = true; 
          digitalWrite(LED_PIN, LOW); 
          noTone(BUZZER_PIN);         
          inputPassword = "";
          lcd.clear();
          lcd.print("Password Benar");
          delay(1000);
        } else {
          inputPassword = "";
          lcd.setCursor(0, 1);
          lcd.print("Salah!         ");
          delay(800);
        }
      } else if (key == '*') {
        inputPassword = "";
      } else {
        inputPassword += key;
      }
    }
  } 
  else {
    // kondisi normal (gelap/terang sudah dimatikan)
    if (nilaiLDR < ambangCahaya) {
       updateLCD(nilaiLDR, "ON ", "OFF  ");
    } else {
       digitalWrite(LED_PIN, LOW);
       noTone(BUZZER_PIN);
       updateLCD(nilaiLDR, "OFF", "OFF  ");
    }
  }
}

// biar lcd ga kedip
void updateLCD(int ldr, String statLED, String statBZ) {
  static int lastLdr = -1;
  static String lastLED = "";
  static String lastBZ = "";

  if (ldr != lastLdr || statLED != lastLED || statBZ != lastBZ) {
    lcd.setCursor(0, 0);
    lcd.print("LDR:");
    lcd.print(ldr);
    lcd.print(" LED:");
    lcd.print(statLED);
    lcd.print("    ");

    lcd.setCursor(0, 1);
    lcd.print("Buzzer:");
    lcd.print(statBZ);
    lcd.print("      ");
    
    lastLdr = ldr;
    lastLED = statLED;
    lastBZ = statBZ;
  }
}