#include <WiFi.h>
#include <Wire.h>
#include <MPU6050.h>
#include <HTTPClient.h>

/* ---------- WIFI ---------- */
const char* ssid = "Qwerty";
const char* password = "whmc6517";

/* ---------- THINGSPEAK ---------- */
const char* server = "http://api.thingspeak.com/update";
String apiKey = "XO515X6V560Q6KYG";

/* ---------- PINS ---------- */
#define SDA_PIN 21
#define SCL_PIN 22

#define SOIL1 34
#define SOIL2 35
#define SW420 27
#define BUZZER 26   //  Buzzer Pin

/* ---------- OBJECTS ---------- */
MPU6050 mpu;

/* ---------- VARIABLES ---------- */
float tiltX = 0;
int soil1, soil2;
int vibration;

/* ---------- ALERT THRESHOLDS ---------- */
float tiltThreshold = 30.0;   // degrees
int soilThreshold = 3500;     // adjust based on calibration

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(115200);

  pinMode(SW420, INPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  Wire.begin(SDA_PIN, SCL_PIN);
  mpu.initialize();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
}

/* ---------- LOOP ---------- */
void loop() {

  /* ---- Read MPU6050 ---- */
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Improved tilt formula
  tiltX = atan2(ax, sqrt(ay * ay + az * az)) * 180 / PI;

  /* ---- Read Sensors ---- */
  soil1 = analogRead(SOIL1);
  soil2 = analogRead(SOIL2);
  vibration = digitalRead(SW420);

  /* ---- ALERT CONDITION ---- */
  bool alert = false;

  if (vibration == 1) {
    alert = true;
    Serial.println(" VIBRATION ALERT!");
  }

  if (abs(tiltX) > tiltThreshold) {
    alert = true;
    Serial.println(" TILT ALERT!");
  }

  if (soil1 > soilThreshold || soil2 > soilThreshold) {
    alert = true;
    Serial.println(" SOIL ALERT!");
  }

  /* ---- BUZZER CONTROL ---- */
  if (alert) {
    digitalWrite(BUZZER, HIGH);   // Beep ON
  } else {
    digitalWrite(BUZZER, LOW);    // Beep OFF
  }

  /* ---- SERIAL OUTPUT ---- */
  Serial.println("----------- SENSOR DATA -----------");
  Serial.print("Soil 1: "); Serial.println(soil1);
  Serial.print("Soil 2: "); Serial.println(soil2);
  Serial.print("Tilt: "); Serial.println(tiltX);
  Serial.print("Vibration: "); Serial.println(vibration);
  Serial.println("----------------------------------");

  /* ---- SEND DATA TO THINGSPEAK ---- */
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = server;
    url += "?api_key=" + apiKey;
    url += "&field1=" + String(soil1);
    url += "&field2=" + String(soil2);
    url += "&field4=" + String(tiltX);
    url += "&field5=" + String(vibration);

    http.begin(url);
    http.GET();
    http.end();
  }

  delay(16000);   // ThingSpeak minimum interval
}