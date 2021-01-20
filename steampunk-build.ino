#include <Servo.h>
#include "DHT.h"
#include <Adafruit_NeoPixel.h>

// the delay for each loop cyle
#define REFRESH_RATE 2000
// type of temperature sensor
#define DHT_TYPE DHT22 
// data pin for temperature sensor
#define DHT_PIN 2
// data pins for servos
#define SERVO_PIN_TEMP 3
#define SERVO_PIN_HUMID 4
// data pins for LEDS
#define DPIN_THERMO_LED 7

// analog pins
#define PIN_PHOTOCELL A0
#define PIN_POT A1

// library instance definitions
DHT dht(DHT_PIN, DHT_TYPE);
Servo servoTemp;
Servo servoHumid;
Adafruit_NeoPixel strip(1, DPIN_THERMO_LED, NEO_GRB + NEO_KHZ800);

// variable definitions
float temperature;
float humidity;
float luxVal;
float potVal;
int servoValTemp;
int servoValHumid;
uint32_t clrThermLed = strip.Color(255, 255, 0);

void setup() {
  Serial.begin(9600);

  // set up pins
  pinMode(PIN_PHOTOCELL, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // start monitoring temperature/humidity
  dht.begin();

  // attach to servos
  servoTemp.attach(SERVO_PIN_TEMP);
  servoHumid.attach(SERVO_PIN_HUMID);

  // shut of internal LED
  digitalWrite(LED_BUILTIN, LOW);

  // initialize pixels
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  Serial.println();

  strip.setPixelColor(0, clrThermLed);
  strip.show();

  potVal = analogRead(PIN_POT);
  luxVal = analogRead(PIN_PHOTOCELL);
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

  printDebugData();

  delay(REFRESH_RATE);
}

void printDebugData() {
  Serial.print("Lux:");
  Serial.println(luxVal);
  
  Serial.print("Humidity (%): ");
  Serial.println((float)humidity, 2);

  Serial.print("Temperature (F): ");
  Serial.println(temperature);

  Serial.print("Pot: ");
  Serial.println(potVal);
}
