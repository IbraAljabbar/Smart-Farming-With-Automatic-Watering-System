#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID_HERE"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME_HERE"
#define BLYNK_AUTH_TOKEN    "YOUR_AUTH_TOKEN_HERE"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

// WiFi Credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

#define SOIL_PIN      34
#define DHT_PIN       4
#define RELAY_PIN     26
#define BUZZER_PIN    25
#define DHTTYPE       DHT11

DHT dht(DHT_PIN, DHTTYPE);
BlynkTimer timer;

bool RELAY_ACTIVE_LOW = false;
bool manualPumpControl = false; // Flag to track manual override from app

int DRY_VALUE  = 4095;
int WET_VALUE  = 1500;

int   DRY_SOIL_THRESHOLD   = 40;
float HOT_TEMP_THRESHOLD   = 35.0;
int   PUMP_RUN_DURATION    = 3000;

void pumpON() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
  Blynk.virtualWrite(V4, 1); // Update Blynk button switch to ON
}

void pumpOFF() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
  Blynk.virtualWrite(V4, 0); // Update Blynk button switch to OFF
}

// Blynk app button widget callback to toggle pump manually
BLYNK_WRITE(V4) {
  int pinValue = param.asInt(); 
  if (pinValue == 1) {
    Serial.println("[Blynk] Pump turned ON MANUALLY");
    pumpON();
    manualPumpControl = true;
  } else {
    Serial.println("[Blynk] Pump turned OFF MANUALLY");
    pumpOFF();
    manualPumpControl = false;
  }
}

// Periodic function to read sensors and update Blynk (prevents server flooding)
void sendSensorData() {
  int soilRaw     = analogRead(SOIL_PIN);
  int soilPercent = map(soilRaw, DRY_VALUE, WET_VALUE, 0, 100);
  soilPercent     = constrain(soilPercent, 0, 100);

  float temperature = dht.readTemperature();
  float airHumidity = dht.readHumidity();

  if (isnan(temperature) || isnan(airHumidity)) {
    Serial.println("[ERROR] Failed to read from DHT11 sensor!");
    return;
  }

  // Push data to Blynk Virtual Pins
  Blynk.virtualWrite(V1, soilPercent);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, airHumidity);

  Serial.println("---------------------------------");
  Serial.print("Soil Moisture      : "); Serial.print(soilPercent); Serial.println("%");
  Serial.print("Air Temperature    : "); Serial.print(temperature);  Serial.println(" C");
  Serial.print("Air Humidity       : "); Serial.print(airHumidity); Serial.println("%");

  bool drySoil = (soilPercent < DRY_SOIL_THRESHOLD);
  bool hotTemp = (temperature >= HOT_TEMP_THRESHOLD);

  // Automation Logic (Runs only if not in manual control mode)
  if (!manualPumpControl) {
    if (drySoil) {
      Serial.println("Soil Status        : DRY -> Automated Pump ON");
      digitalWrite(BUZZER_PIN, HIGH);
      pumpON();
      delay(PUMP_RUN_DURATION);
      pumpOFF();
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("Pump               : OFF (Watering Completed)");
    } else {
      pumpOFF();
      if (hotTemp) {
        digitalWrite(BUZZER_PIN, HIGH);
        Serial.println("Alert              : HIGH TEMPERATURE!");
      } else {
        digitalWrite(BUZZER_PIN, LOW
