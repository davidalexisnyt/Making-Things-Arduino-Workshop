const int LED_PIN = 9;

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    flashLed(LED_PIN, 250
    flashLed(LED_PIN, 250);
    flashLed(LED_PIN, 1000);
}
          
void flashLed(int ledPin, int duration) {
    digitalWrite(ledPin, HIGH);
    delay(duration); 
    digitalWrite(ledPin, LOW);
    delay(duration);
}