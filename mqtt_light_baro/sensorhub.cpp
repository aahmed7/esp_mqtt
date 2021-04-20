#include <Adafruit_BMP085.h>
#include <Arduino.h>
#include <BH1750.h>
#include <PubSubClient.h>
#include <Wire.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

/* Brightness Levels. */
#define _TOOLOW       25
#define _LOW          50
#define _HIGH         500
#define _TOOHIGH      750
#define LEVEL_TOOLOW  "TOO LOW"
#define LEVEL_LOW     "LOW"
#define LEVEL_OPTIMAL "OPTIMAL"
#define LEVEL_HIGH    "HIGH"
#define LEVEL_TOOHIGH "TOO HIGH"

/* BH1750 object. */
BH1750 lightMeter(0x23);

/* BMP180 object. */
Adafruit_BMP085 bmp;

/* WiFi Credentials. */
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASS";

/* MQTT Credentials. */
const char* mqttServer = "192.168.1.100";
const int   mqttPort = 1883;
const char* mqttUser = "YourMQTTUsername";
const char* mqttPassword = "YourMQTTPassword";

/* MQTT Connection Parameters. */
const char* clientID = "ESP8266_Client";
char*       topic = "test/ESP";

/* Client Objects. */
WiFiClient   espClient;
PubSubClient client(espClient);

void setup() {
    Wire.begin(2, 0);

    setup_wifi();
    setup_mqtt();

    lightMeter.begin();
    bmp.begin();
}

void loop() {
    light_sensor();
    baro();
    delay(1000);
}

void publish(char* topic, char* message) { client.publish(topic, message); }

/* Collect data from light sensor and send to broker. */
void light_sensor() {
    uint16_t lux = 250;
    int      luxLevel = 3;
    char     luxMessage[20];

    lux = lightMeter.readLightLevel();

    if (lux <= _TOOLOW) {
        luxLevel = 1;
        sprintf(luxMessage, "%s", LEVEL_TOOLOW);
    } else if (lux > _TOOLOW && lux <= _LOW) {
        luxLevel = 2;
        sprintf(luxMessage, "%s", LEVEL_LOW);
    } else if (lux > _LOW && lux <= _HIGH) {
        luxLevel = 3;
        sprintf(luxMessage, "%s", LEVEL_OPTIMAL);
    } else if (lux > _HIGH && lux < _TOOHIGH) {
        luxLevel = 4;
        sprintf(luxMessage, "%s", LEVEL_HIGH);
    } else {
        luxLevel = 5;
        sprintf(luxMessage, "%s", LEVEL_TOOHIGH);
    }

    client.publish("ESP/Lux", String(lux).c_str());
    client.publish("ESP/LuxLevel", String(luxLevel).c_str());
    client.publish("ESP/LuxMessage", luxMessage);
}

/* Collect data from barometer and send to broker. */
void baro() {
    float   temp;
    int32_t pressure;
    float   alt;
    int32_t sealevel_pressure;
    float   real_alt;

    temp = bmp.readTemperature();
    client.publish("ESP/Temperature(*C)", String(temp).c_str());

    pressure = bmp.readPressure();
    client.publish("ESP/Pressure(Pa)", String(pressure).c_str());

    alt = bmp.readAltitude();
    client.publish("ESP/Altitude(m)", String(alt).c_str());

    sealevel_pressure = bmp.readSealevelPressure();
    client.publish("ESP/SealevelPressure(Pa)",
                   String(sealevel_pressure).c_str());

    real_alt = bmp.readAltitude(101500);
    client.publish("ESP/RealAltitude(m)", String(real_alt).c_str());

    delay(500);
}

void setup_wifi() {
    /* Connect to WiFi. */
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void setup_mqtt() {
    /* Register at the MQTT Broker. */
    client.setServer(mqttServer, mqttPort);
    while (!client.connected()) {
        if (client.connect(clientID)) {
        } else {
            delay(2000);
        }
    }
    publish(topic, "HELLO FROM ESP");
}
