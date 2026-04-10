#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// 🔐 WiFi Credentials
#define WIFI_SSID "D_tgfesdgdsgbfd 2.4GHz"
#define WIFI_PASS "dufrgfdffdgfdd"

// 🔐 Adafruit IO Credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "jadgfdghcgfhdsghfdsghdghdghdkis"
#define AIO_KEY         "aio_hdgsfdhjfdsghjfdsghfdsfghjfdsfghjfdsghjfdsafgujcucQggCENYd"

// LED Pin
#define LED_PIN 2

WiFiClient client;

// MQTT Client
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feed (must match exactly)
Adafruit_MQTT_Subscribe ledFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led");

// Function declaration
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // LED OFF initially

  // 🔌 Connect WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Subscribe to feed
  mqtt.subscribe(&ledFeed);
}

void loop() {
  MQTT_connect();

  Serial.println("📡 Waiting for data...");

  Adafruit_MQTT_Subscribe *subscription;

  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &ledFeed) {
      String value = (char *)ledFeed.lastread;

      Serial.print("📥 Received: ");
      Serial.println(value);

      if (value == "1") {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("💡 LED ON");
      } 
      else if (value == "0") {
        digitalWrite(LED_PIN, LOW);
        Serial.println("💡 LED OFF");
      } 
      else {
        Serial.println("⚠️ Unknown value");
      }
    }
  }
}

// 🔁 MQTT reconnect function
void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) return;

  Serial.print("🔌 Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) {
    Serial.print("❌ Error: ");
    Serial.println(mqtt.connectErrorString(ret));

    mqtt.disconnect();
    delay(3000);
  }

  Serial.println("✅ MQTT Connected!");
}
