# ESP MQTT Examples

## I2C Scanner

This runs the classic arduino I2C scanner as an MQTT publisher.

Every discovered device appears on the broker as a new topic I2C_devicexx where xx is the device address.

## Template

This is a simple boilerplate code for getting started with MQTT projects.

## Light_Baro

This example uses a BH1750 and BMP180 to log temperature, pressure, altitude and brightness to the mqtt broker.

## Relay Control

This examples controls a single relay to switch an appliance.

## Node-red Flow

This example deploys a node-red dashboard to provide a frontend for the Light_Baro and relay examples.

## Requirements

- ESP8266/ESP32 Arduino Core
- Arduino pubsub library(<https://github.com/knolleary/pubsubclient>)
- BMP085, BH1750 library(for Light_Baro example)
- Node-red(for node-red flow)
- Nore-red-dashboard(<https://flows.nodered.org/node/node-red-dashboard>)(for node-red flow)
