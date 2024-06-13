#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <GravityTDS.h>

// Pin configuration
const int turbidityPin = A0;    // Analog pin for turbidity sensor
const int temperaturePin = A1;  // Analog pin for temperature sensor
const int tdsPin = A2;          // Analog pin for TDS sensor
const int oneWireBus = 2;       // Digital pin for OneWire bus
const int pHpin = A3;
float temperatureValue;
unsigned long int avgValue;
float b;
int buf[10];

// Create instances of the sensors
OneWire oneWire(oneWireBus);          // OneWire instance
DallasTemperature sensors(&oneWire);  // Temperature sensor
GravityTDS gravityTds;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  // Initialize sensors
  gravityTds.begin();
  sensors.begin();
}

void loop() {
  for (int i = 0; i < 10; i++) {
    buf[1] = analogRead(pHpin);
    delay(10);
  }

  for (int i = 0; i < 9; i++) {

    for (int j = i + 1; j < 10; j++) {

      if (buf[i] > buf[j]) {
        temperatureValue = buf[i];
        buf[i] = buf[j];
        buf[j] = temperatureValue;
      }
    }
  }
  avgValue = 0;

  for (int i = 2; i < 8; i++)
    avgValue += buf[i];
  float pHvalue = (float)avgValue * 5.0 / 1024 / 6;
  pHvalue = 3.5 * pHvalue;

  // Read sensor values
  float turbidityValue = analogRead(turbidityPin);
  float temperatureValue = getTemperature();
  float tdsValue = gravityTds.getTdsValue();

  // Print values to Serial Monitor
  Serial.print("Turbidity: ");
  Serial.println(turbidityValue);

  Serial.print("Temperature: ");
  Serial.print(temperatureValue);
  Serial.println(" °C");

  Serial.print("TDS: ");
  Serial.print(tdsValue);
  Serial.println(" ppm");

  Serial.print("pH: ");
  Serial.print(pHvalue);

  Serial.println();  // Add a blank line for readability

  delay(2000);  // Adjust delay as needed
}

float getTemperature() {
  sensors.requestTemperatures();      // Request temperature readings
  return sensors.getTempCByIndex(0);  // Get the temperature in Celsius
}
