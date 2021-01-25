#include <Servo.h>
#include "DHT.h"
#include <Adafruit_NeoPixel.h>

#define DEBUG_ALIGN_SERVOS 90

// the delay for each loop cyle
#define REFRESH_RATE 4000
// type of temperature sensor
#define DHT_TYPE DHT22 
// data pin for temperature sensor
#define DHT_PIN 2
// data pins for servos
#define SERVO_PIN_TEMP 3
#define SERVO_PIN_HUMID 4
// data pins for LEDS
#define DPIN_THERMO_LED 7
// total number of LEDs to control
#define LED_COUNT 8

// analog pins
#define PIN_PHOTOCELL A0
#define PIN_POT A1
#define PIN_LED_SWITCH A2

// library instance definitions
DHT dht(DHT_PIN, DHT_TYPE);
Servo servoTemp;
Servo servoHumid;
Adafruit_NeoPixel strip(LED_COUNT, DPIN_THERMO_LED, NEO_GRBW + NEO_KHZ800);

// variable definitions
float temperature;
float humidity;
float luxVal;
float thermBrightness;
int potVal;
int ledSwitchVal;
int servoValTemp;
int servoValHumid;
int clrsTherm[] = {255, 255, 0, 255};
//uint32_t clrThermLed = strip.Color(255, 255, 0);

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
  int servoPosHumid;
  int servoPosTemp;
  
  Serial.println();

  potVal = analogRead(PIN_POT);
  luxVal = analogRead(PIN_PHOTOCELL);
  ledSwitchVal = analogRead(PIN_LED_SWITCH);
  humidity = dht.readHumidity(); 
  temperature = dht.readTemperature(true);

  // if ledSwitch is off, set brightness to zero
  thermBrightness = (ledSwitchVal > 10) ?  map(potVal, 0, 1024, 16, 255) / 255.0 : 0;

  for (int index = 0; index < LED_COUNT; index++) {
    strip.setPixelColor(index,
      clrsTherm[0] * thermBrightness,
      clrsTherm[1] * thermBrightness,
      clrsTherm[2] * thermBrightness,
      clrsTherm[3] * thermBrightness
    );
  }

  strip.show();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(REFRESH_RATE);
    return;
  }

  // calculate servo positions based on temp/humid values
  // NOTE: servo ranges must be flipped because of thier rotation
  servoPosHumid = DEBUG_ALIGN_SERVOS ? DEBUG_ALIGN_SERVOS : map(humidity, 0, 100, 180, 0);
  servoPosTemp = DEBUG_ALIGN_SERVOS ? DEBUG_ALIGN_SERVOS: map(temperature, 0, 100, 180, 0);
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

  Serial.print("LED Switch: ");
  Serial.println(ledSwitchVal);
}
