// Use a PWM pin in order to control LED brightness
const int LED_PIN = 6;
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
        flashLed(LED_PIN, 2000);
    }
}

void flashLed(int ledPin, int duration) 
{
    digitalWrite(ledPin, HIGH);
    delay(duration); 
    digitalWrite(ledPin, LOW);
    delay(duration);
}

void pulseLed(int ledPin, int duration)
{
    int interval = duration / 255;
    
    for (int brightness = 0; brightness <= 255; brightness++)
    {
        analogWrite(ledPin, brightness);
        delay(interval);
    }
    
    for (int brightness = 255; brightness >= 0; brightness--)
    {
        analogWrite(ledPin, brightness);
        delay(interval);
    }
}