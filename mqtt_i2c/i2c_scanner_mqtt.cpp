#include <Arduino.h>
#include <PubSubClient.h>
#include <Wire.h>

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
    Wire.begin(2, 0);

    setup_wifi();
    setup_mqtt();
}

void loop() { i2c_scanner(); }

void publish(char* topic, char* message) { client.publish(topic, message); }

void i2c_scanner() {
    char device_address[4];
    byte error, address;
    int  nDevices;

    nDevices = 0;
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            sprintf(device_address, "I2C_device%d", address);
            publish(device_address, device_address);
            nDevices++;
        }
    }
    if (nDevices == 0)
        publish(topic, "No I2C devices found");
    else
        publish(topic, "Done");

    delay(5000);
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
        if (!client.connect(clientID)) {
            delay(2000);
        }
    }

    publish(topic, "HELLO FROM ESP");
}
