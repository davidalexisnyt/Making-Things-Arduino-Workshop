/*
 * This example keeps the LED on for as long as the button is pressed.
 * If the button is pressed, we turn the LED on.  If it is not pressed
 * we turn it off.
 */

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
        digitalWrite(ledPin, HIGH);
    }
    else
    {
        digitalWrite(ledPin, LOW);
    }
}