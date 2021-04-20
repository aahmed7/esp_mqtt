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
const char* clientID = "ESP8266_Client";
char*       topic = "test/ESP";

/* Client Objects. */
WiFiClient   espClient;
PubSubClient client(espClient);

void setup() {
    Serial.begin(115200);

    setup_wifi();
    setup_mqtt();

    subscribe();
}

void loop() {
    publish(topic, "Hello from ESP");
    client.loop();
    delay(1000);
}

void publish(char* topic, char* message) { client.publish(topic, message); }

void subscribe() { client.subscribe(topic); }

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void setup_wifi() {
    /* Connect to WiFi. */
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
}

void setup_mqtt() {
    /* Register at the MQTT Broker. */
    client.setServer(mqttServer, mqttPort);
    while (!client.connected()) {
        Serial.println("Connecting to MQTT...");
        if (client.connect(clientID)) {
            Serial.println("connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            Serial.println();
            delay(2000);
        }
    }

    /* Set the callback function. */
    client.setCallback(callback);
}