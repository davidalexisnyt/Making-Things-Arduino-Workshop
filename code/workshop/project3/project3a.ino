/*
 * This example uses the DHT11 digital humidity and temperature sensor.
 * It requires the Adafruit DHT Sensor and Adafruit_Sensor libraries.
 * Make sure to install them using the Library Manager before compiling this Sketch.
 */

#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    Serial.println(F("Starting temperature and humidity readings"));

    dht.begin();
}

void loop() {
    delay(2000);

    float humidity = dht.readHumidity();    
    float temperature = dht.readTemperature(true);

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    float heatIndex = dht.computeHeatIndex(temperature, humidity);

    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(temperature);
    Serial.print(F("°F  Heat index: "));
    Serial.print(heatIndex);
    Serial.println(F("°F"));
}