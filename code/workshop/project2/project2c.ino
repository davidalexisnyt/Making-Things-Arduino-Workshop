#include <NewPing.h>

const int triggerPin = 13;
const int echoPin = 12;
const int maxDistance = 500;
const int redLedPin = 7;
const int greenLedPin = 6;

NewPing sonar(triggerPin, echoPin, maxDistance); 

void setup() 
{
    Serial.begin (9600);
    
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
}

void loop() 
{
    // Get the distance reading in centimeters
    int distance = sonar.ping_cm();
   
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > maxDistance) {
        digitalWrite(redLedPin, LOW);
        digitalWrite(greenLedPin, LOW);
    }
    else if (distance > 10) {
        digitalWrite(redLedPin, LOW);
        digitalWrite(greenLedPin, HIGH);
    }
    else {
        digitalWrite(redLedPin, HIGH);
        digitalWrite(greenLedPin, LOW);
    }
    
    // Wait for a short while before reading the sensor again
    delay(100);
}