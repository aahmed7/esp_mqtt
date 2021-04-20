#include <Arduino.h>
#include <PubSubClient.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

/* WiFi Credentials. */
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASS";

/* MQTT Credentials. */
const char* mqttServer = "192.168.1.100";
const int   mqttPort = 1883;
const char* mqttUser = "YourMQTTUsername";
const char* mqttPassword = "YourMQTTPassword";

/* MQTT Connection Parameters. */
const char* clientID = "ESP8266_RELAY";
const char* topic = "ESP2/RELAY";

/* Client Objects. */
WiFiClient   espClient;
PubSubClient client(espClient);

// relay connected to  GPIO0
#define RELAY 0

void setup() {
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, LOW);

    setup_wifi();
    setup_mqtt();
}

void loop() {
    client.loop();
    delay(1000);
}

void subscribe() { client.subscribe(topic); }

void callback(char* topic, byte* payload, unsigned int length) {
    if (payload[0] == '0') {
        digitalWrite(RELAY, HIGH);
    }
    if (payload[0] == '1') {
        digitalWrite(RELAY, LOW);
    }
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
    /* Set the callback function. */
    client.setCallback(callback);

    /* Subscribe to the broker. */
    subscribe();
}
