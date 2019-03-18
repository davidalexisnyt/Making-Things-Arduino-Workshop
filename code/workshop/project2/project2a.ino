const int triggerPin = 13;
const int echoPin = 12;

int inches = 0;
int cm = 0;

void setup() {
    Serial.begin(9600);

    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop() {
    // Clear the trigger
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);

    // Send an ultarasonic ping for 10 microseconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // Reads the echo pin, and returns the sound wave travel time in microseconds
    int pingTime = pulseIn(echoPin, HIGH);

    // Convert the ping time to distance in centimeters
    cm = 0.01723 * pingTime;

    // Convert centimeters to inches
    inches = (cm / 2.54);

    Serial.print("Distance: ");
    Serial.print(cm);
    Serial.print(" cm or ");
    Serial.print(inches);
    Serial.println(" inches");

    // Wait for a short while before reading the sensor again
    delay(100);
}