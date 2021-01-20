#include "DHT.h"

#define DHT_TYPE DHT22
#define DHT_PIN 2

// library instance definitions
DHT dht(DHT_PIN, DHT_TYPE);


void setup() {
  Serial.begin(9600);
  
  dht.begin();
}

void loop() {
  Serial.println();

  float humidity = dht.readHumidity(); 
  float temperature = dht.readTemperature(true);
  
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(2000);
    return;
  }

  Serial.print("Humidity (%): ");
  Serial.println((float)humidity, 2);

  Serial.print("Temperature (F): ");
  Serial.println(temperature);

  delay(2000);
}
