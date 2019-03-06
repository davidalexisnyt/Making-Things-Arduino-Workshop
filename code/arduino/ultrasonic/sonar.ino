#include <NewPing.h>

#define TrigPin 8
#define EchoPin 9

NewPing sonar(TrigPin, EchoPin);

long duration, distance;

void setup() {
    Serial.begin(115200);  
}

void loop() {
    distance = sonar.ping_in();
    Serial.println(distance);

    delay(1000);
}
