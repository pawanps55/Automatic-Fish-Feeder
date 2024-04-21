#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Servo.h>

Servo myservo;
int pos = 0; 

#define WLAN_SSID       "POCO X2"
#define WLAN_PASS       "7890123456"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                
#define AIO_USERNAME    "pawanps55"
#define AIO_KEY         "aio_bIzv72a9UUitiWPKOUOOrcB163ta"
0
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe toggle = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/servotoggle");

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  myservo.attach(15); // NodeMCU D8 pin

  Serial.println(F("Adafruit MQTT demo"));
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mqtt.subscribe(&toggle);
}

void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &toggle) {
      Serial.print(F("Toggle State: "));
      Serial.println((char *)toggle.lastread);
      if (strcmp((char *)toggle.lastread, "ON") == 0) {           // toggle button
      for(int q=0; q<10; q++){
      myservo.write(180);
      delay(100);
      myservo.write(0);
      delay(100);
      }
      } 
      else {
      myservo.write(0); // Move servo to 0 degrees when toggle is OFF
      }
    }
  }
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
