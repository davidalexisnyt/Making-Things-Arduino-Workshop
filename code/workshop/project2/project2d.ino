#include <NewPing.h>

const int triggerPin = 13;
const int echoPin = 12;
const int maxDistance = 500;
const int redLedPin = 7;
const int greenLedPin = 6;

int inches = 0;
int cm = 0;

NewPing sonar(triggerPin, echoPin, maxDistance); 

void setup() {
    Serial.begin (9600);
    
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
}

void loop() {
    // Get the distance reading in centimeters
    cm = sonar.ping_cm();
   
    Serial.print("Distance: ");
    Serial.print(cm);
    Serial.println(" cm");

    if (cm > maxDistance) {
        setColor(0, 0, 0);
    }
    else if (cm > 50) {
        setColor(0, 255, 0);
    }
    else {
        setColor(255, 0, 0);
    }
    
    // Wait for a short while before reading the sensor again
    delay(100);
}

void setColor(int red, int green) {
  	analogWrite(redLedPin, red);
  	analogWrite(greenLedPin, green);
}