/*
 ESP8266 Blink
 Blink the blue LED on the ESP-01 module
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because 
    // it is acive low on the ESP-01)
    digitalWrite(LED_BUILTIN, LOW);   

    delay(500);

    // Turn the LED off by making the voltage HIGH
    digitalWrite(LED_BUILTIN, HIGH);  

    delay(1000);
}