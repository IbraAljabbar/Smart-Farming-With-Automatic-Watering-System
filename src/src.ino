#include "DHT.h"

#define SOIL_PIN      34
#define DHT_PIN       4
#define RELAY_PIN     26
#define BUZZER_PIN    25
#define DHTTYPE       DHT11

DHT dht(DHT_PIN, DHTTYPE);

bool RELAY_ACTIVE_LOW = false;

int DRY_VALUE  = 4095;
int WET_VALUE  = 1500;

int   DRY_SOIL_THRESHOLD   = 40;
float HOT_TEMP_THRESHOLD   = 35.0;
int   PUMP_RUN_DURATION    = 3000;

void pumpON() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
}

void pumpOFF() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(SOIL_PIN,   INPUT);
  pinMode(RELAY_PIN,  OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pumpOFF();
  digitalWrite(BUZZER_PIN, LOW);

  dht.begin();

  Serial.println("=================================");
  Serial.println("    ESP32 SMART FARMING SYSTEM   ");
  Serial.println("=================================");
  Serial.print("Relay Mode : ");
  Serial.println(RELAY_ACTIVE_LOW ? "Active LOW (NO)" : "Active HIGH (NC)");
  Serial.println("System ready.");
}

void loop() {
  int soilRaw     = analogRead(SOIL_PIN);
  int soilPercent = map(soilRaw, DRY_VALUE, WET_VALUE, 0, 100);
  soilPercent     = constrain(soilPercent, 0, 100);

  float temperature = dht.readTemperature();
  float airHumidity = dht.readHumidity();

  if (isnan(temperature) || isnan(airHumidity)) {
    Serial.println("[ERROR] Failed to read from DHT11 sensor! Check wiring.");
    pumpOFF();
    digitalWrite(BUZZER_PIN, LOW);
    delay(2000);
    return;
  }

  bool drySoil = (soilPercent < DRY_SOIL_THRESHOLD);
  bool hotTemp = (temperature >= HOT_TEMP_THRESHOLD);

  Serial.println("---------------------------------");
  Serial.print("Soil Raw           : "); Serial.println(soilRaw);
  Serial.print("Soil Moisture      : "); Serial.print(soilPercent); Serial.println("%");
  Serial.print("Air Temperature    : "); Serial.print(temperature);  Serial.println(" C");
  Serial.print("Air Humidity       : "); Serial.print(airHumidity); Serial.println("%");

  if (drySoil) {
    Serial.println("Soil Status        : DRY -> Pump ON");
    Serial.println("Buzzer             : ON");

    digitalWrite(BUZZER_PIN, HIGH);
    pumpON();
    delay(PUMP_RUN_DURATION);
    pumpOFF();
    digitalWrite(BUZZER_PIN, LOW);

    Serial.println("Pump               : OFF (watering completed)");

  } else {
    Serial.println("Soil Status        : Moist -> Pump OFF");
    pumpOFF();

    if (hotTemp) {
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("Buzzer             : ON (high temperature)");
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("Buzzer             : OFF");
    }
  }

  delay(2000);
}
