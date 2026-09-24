#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define TRIG_PIN 9
#define ECHO_PIN 10

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int measureNum = 0;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("=== Эксперимент: температурная компенсация ===");
  Serial.println("N | T (°C) | v (м/с) | D_raw (см) | D_comp (см) | Δ (см)");
  Serial.println("----------------------------------------------------------");
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC == -127.0) {
    delay(200);
    return;
  }

  // Измерение
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  // Скорость звука с учётом температуры
  float speedOfSound = 331.3 + 0.606 * tempC;

  // Расстояния
  float distanceComp = duration * speedOfSound / 2.0 / 10000.0;
  float distanceRaw  = duration * 343.0 / 2.0 / 10000.0;
  float delta = distanceRaw - distanceComp;

  measureNum++;

  Serial.print(measureNum);
  Serial.print(" | ");
  Serial.print(tempC, 2);
  Serial.print(" | ");
  Serial.print(speedOfSound, 2);
  Serial.print(" | ");
  Serial.print(distanceRaw, 2);
  Serial.print(" | ");
  Serial.print(distanceComp, 2);
  Serial.print(" | ");
  Serial.println(delta, 3);

  delay(500);
}