const int ne555OutputPin = 2;  // Connect NE555 output to this pin
const int analogReadPin = A0;  // Analog pin for current measurement
unsigned int elapsedTime, startTime;
volatile bool interrupt = false;
unsigned long pulseOnTime,pulseOFFTime;
void setup() {
  pinMode(ne555OutputPin, INPUT);
  pinMode(analogReadPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(ne555OutputPin), handleInterrupt, RISING);
  pulseOnTime = pulseIn(analogReadPin, HIGH);
  pulseOFFTime = pulseIn(analogReadPin, LOW);
  Serial.begin(115200);
}
void loop() {
  if (!interrupt) {
    delay(1000);
  } else {
    noInterrupts();    
    startTime = micros();
    int adcValue = analogRead(analogReadPin);
    elapsedTime = micros() - startTime;
    float voltage = (adcValue * (5.0 / 1023));  // Convert ADC to voltage assuming your reference voltage
    float current = voltage / 330;
    if (digitalRead(ne555OutputPin) == HIGH) {
      Serial.println("-----------------------High Time For Pulse-----------------------------------");
      Serial.println("High time: " + String(pulseOnTime) + " microSeconds");
    } else {
      Serial.println("-----------------------Low Time For Pulse------------------------------------");
      Serial.println("Low time: " + String(pulseOFFTime) + " microSeconds");
    }
    Serial.println("Output Current: " + String(current,5) + " A");
    Serial.println("Time taken for reading ADC : " + String(elapsedTime) + " microseconds");
    interrupt = false;
    interrupts();
  }
}
void handleInterrupt() {
  interrupt = true;
}