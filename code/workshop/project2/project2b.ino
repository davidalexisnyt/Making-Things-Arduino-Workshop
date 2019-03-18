#include <NewPing.h>

const int triggerPin = 13;
const int echoPin = 12;
const int maxDistance = 500;

NewPing sonar(triggerPin, echoPin, maxDistance); 

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Get the distance reading in centimeters
    int distance = sonar.ping_cm();

    // ..Or we if we want to get the distance reading in inches use...
    // int distance = sonar.ping_in();
   
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Wait for a short while before reading the sensor again
    delay(100);
}