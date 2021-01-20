#include <Servo.h>
#include "DHT.h"

// the delay for each loop cyle
#define REFRESH_RATE 2000
// type of temperature sensor
#define DHT_TYPE DHT22 
// data pin for temperature sensor
#define DHT_PIN 2
// data pins for servos
#define SERVO_PIN_TEMP 3
#define SERVO_PIN_HUMID 4

// library instance definitions
DHT dht(DHT_PIN, DHT_TYPE);
Servo servoTemp;
Servo servoHumid;

// variable definitions
float temperature;
float humidity;
int servoValTemp;
int servoValHumid;

void setup() {
  Serial.begin(9600);

  // start monitoring temperature/humidity
  dht.begin();

  // attach to servos
  servoTemp.attach(SERVO_PIN_TEMP);
  servoHumid.attach(SERVO_PIN_HUMID);
}

void loop() {
  Serial.println();

  humidity = dht.readHumidity(); 
  temperature = dht.readTemperature(true);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(REFRESH_RATE);
    return;
  }

  // calculate servo positions based on temp/humid values
  // NOTE: servo ranges must be flipped because of thier rotation
  int servoPosHumid = map(humidity, 0, 100, 180, 0);
  int servoPosTemp = map(temperature, 0, 100, 180, 0);
  servoHumid.write(servoPosHumid);
  servoTemp.write(servoPosTemp);  

  Serial.print(servoPosTemp);
  Serial.print("----");
  Serial.print("Humidity (%): ");
  Serial.println((float)humidity, 2);

  Serial.print("Temperature (F): ");
  Serial.println(temperature);

  delay(REFRESH_RATE);
}
