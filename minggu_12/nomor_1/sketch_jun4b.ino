#include <NewPing.h> 
#include <Servo.h> 

#define trigPin 12 
#define echoPin 13 
#define MAX_DISTANCE 500 // Set jarak maksimal 500cm

NewPing sonar(trigPin, echoPin, MAX_DISTANCE);
Servo myservo; 

const int LEDmerah = 2;
const int LEDhijau = 3;

void setup() {
  Serial.begin(115200); 
  
  pinMode(LEDmerah, OUTPUT);
  pinMode(LEDhijau, OUTPUT);
  
  myservo.attach(9); // Set servo pada pin PWM 9
  myservo.write(90); // Posisi awal servo standby di 90 derajat
}

void loop() {
  // OPTIMASI: Menggunakan fungsi bawaan NewPing yang jauh lebih cepat dan akurat
  int jarak = sonar.ping_cm(); 
  
  // Jika hasil pantulan 0, berarti objek di luar jangkauan (MAX_DISTANCE)
  if (jarak == 0) {
    jarak = MAX_DISTANCE;
  }

  Serial.print(jarak);
  Serial.println(" cm");

  if (jarak <= 15 && jarak > 0) {
    // Kondisi jika objek dekat (< 15cm)
    digitalWrite(LEDhijau, LOW);
    
    // Gerakan servo: 90 -> 180 -> 0 -> kembali ke 90
    digitalWrite(LEDmerah, HIGH);
    myservo.write(180);
    delay(450); 
    
    digitalWrite(LEDmerah, LOW);
    myservo.write(90);
    delay(450); 
    
    digitalWrite(LEDmerah, HIGH);
    myservo.write(0);
    delay(450); 
    
    digitalWrite(LEDmerah, LOW);
    myservo.write(90);
    delay(450);
  } 
  else {
    // Kondisi jika objek jauh / aman
    digitalWrite(LEDmerah, LOW);
    digitalWrite(LEDhijau, HIGH);
    myservo.write(90); 
    delay(50); 
  }
}