const int LED_PIN = 7;
const int BUTTON_PIN = 9;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
}

void loop()
{
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
        flashLed(LED_PIN, 500);
    }
}

void flashLed(int ledPin, int duration) 
{
    digitalWrite(ledPin, HIGH);
    delay(duration); 
    digitalWrite(ledPin, LOW);
    delay(duration);
}