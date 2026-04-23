const int potPin = A0;      
const int ledPins[] = {2, 3, 4, 5, 6}; // pin untuk 5 LED

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  int sensorValue = analogRead(potPin); 
  
  int numLedsLit = map(sensorValue, 0, 1023, 0, 5);

  for (int i = 0; i < 5; i++) {
    if (i < numLedsLit) {
      digitalWrite(ledPins[i], HIGH); 
    } else {
      digitalWrite(ledPins[i], LOW);  
    }
  }

  Serial.print("ADC: ");
  Serial.print(sensorValue);
  Serial.print(" | LED Nyala: ");
  Serial.println(numLedsLit);

  delay(50); 
}