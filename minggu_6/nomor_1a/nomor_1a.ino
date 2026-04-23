const int potPin = A0; 

void setup() {
  Serial.begin(9600);
  Serial.println("--- Monitoring ADC & Potensio (0-9) ---");
}

void loop() {
  int adcValue = analogRead(potPin);
  
  int potensioValue = map(adcValue, 0, 1023, 0, 9);

  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print("  |  Angka (0-9): ");
  Serial.println(potensioValue);

  delay(200); 
}