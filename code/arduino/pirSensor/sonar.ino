#include <NewPing.h>

#define TrigPin 8
#define EchoPin 9

NewPing sonar(TrigPin, EchoPin);

long duration, distance;

void setup() {
//    pinMode(LED_BUILTIN, OUTPUT);
//    pinMode(TrigPin, OUTPUT);
//    pinMode(EchoPin, INPUT);
//    
//    digitalWrite(TrigPin, LOW);

    Serial.begin(115200);  
}

void loop() {
//    digitalWrite(TrigPin, HIGH);        // Turn on ultrasonic pulse
//    delayMicroseconds(10);
//    digitalWrite(TrigPin, LOW);         // Turn off ultrasonic pulse
//
//    duration = pulseIn(EchoPin, HIGH);  // Get amount of time that the echo was received
//    distance = (duration / 2) / 29.1;

    distance = sonar.ping_in();
    Serial.println(distance);

    delay(1000);
}
