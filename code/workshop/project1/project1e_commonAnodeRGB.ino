/*
 * This example shows how to work with a common anode type RGB LED.
 * These LEDs have the common pin connected to power, not ground, and
 * the "ON" value is the reverse of a normal LED.  i.e. ON = 0 voltage
 * and OFF = full voltage!
 * When using PWM (pulse width modulation) to control brightness, we
 * need to use the Arduino's PWM pins - 3, 5, 6, 9, 10, and 11, which
 * accept a range of values between 0 and 255 instead of just LOW and HIGH.
 * 
 * Common Anode RGB LED Example Wiring
 * ---------------------------
 * LED pin 1 (red)   -> 100 Ohm resistor -> Arduino pin 10
 * LED pin 2 (anode) -> Arduino 5V
 * LED pin 3 (green) -> 100 Ohm resistor -> Arduino pin 11
 * LED pin 4 (blue)  -> 100 Ohm resistor -> Arduino pin 12
 */

// Use a PWM pin in order to control LED brightness
const int RED_LED_PIN = 10;
const int GREEN_LED_PIN = 11;
const int BLUE_LED_PIN = 12;

void setup()
{
}

void loop()
    flashLed(RED_LED_PIN, 2000);
}

/*
    To flash an LED on using PWM, we need to use analogWrite() and
    set the value to 255 (full voltage) to turn it off and 0 (no voltage)
    to turn it on.
*/
void flashLed(int ledPin, int duration) 
{
    analogWrite(ledPin, 0);
    delay(duration); 
    analogWrite(ledPin, 255);
    delay(duration);
}

/*
    To pulse the LED, we're going to gradually increase the brightness from
    off to full brightness, then back down to off.
    Since the common anode type of RGB LED behaves the opposite of a regular LED
    (or a common cathode RGB LED), we need to set the brightness to the inverse of
    the brightness we want. (Weird).
*/
void pulseLed(int ledPin, int duration)
{
    int interval = duration / 255;
    
    for (int brightness = 0; brightness <= 255; brightness++)
    {
        analogWrite(ledPin, 255 - brightness);
        delay(interval);
    }
    
    for (int brightness = 255; brightness >= 0; brightness--)
    {
        analogWrite(ledPin, 255 - brightness);
        delay(interval);
    }
}


void setColor(int red, int green, int blue) {
    analogWrite(RED_LED_PIN, 255 - red);
    analogWrite(GREEN_LED_PIN, 255 - green);
    analogWrite(BLUE_LED_PIN, 255 - blue);
}