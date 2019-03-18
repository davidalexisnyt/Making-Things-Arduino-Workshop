// TMP36 Temperature Sensor
// https://learn.adafruit.com/tmp36-temperature-sensor/overview

// Connect the analog voltage out pin on the sensor to an analog pin on the Arduino.
int sensorPin = 0;          

// The voltage reading must be converted to a usable value based on whether
// the Arduino voltage is 5 volts or 3.3 volts.
// Here we define the voltage of our board (5V or 3.3V)
int boardVoltage = 5.0;

void setup()
{
    // Start the serial connection to display results in the Arduino IDE's serial monitor
    Serial.begin(9600);
}
 
void loop()                 // run over and over again
{
    // Get the voltage reading from the temperature sensor.
    // The resulution of the sensor is 10mV per degree celsius, with an offset of 500mV
    int reading = analogRead(sensorPin);  
    

    float voltage = reading * boardVoltage;
    voltage /= 1024.0; 
    
    // Convert the voltage reading to temperature (celsius)
    float temperatureC = (voltage - 0.5) * 100; 

    Serial.print(temperatureC); 
    Serial.println(" degrees C");
    
    // Convert to fahrenheit
    float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

    Serial.print(temperatureF); 
    Serial.println(" degrees F");

    // Delay 1 second between readings
    delay(1000);
}
