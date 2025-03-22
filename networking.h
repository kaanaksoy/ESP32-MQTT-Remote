
/**************************************************************************/
/*!
    @file     networking.h
    @brief    Header file for networking functionality

    @details  Contains functions for connecting to wifi and publishing data to the MQTT broker.

*/
/**************************************************************************/

#ifndef NETWORKING_H
#define NETWORKING_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "debug.h"
#include "defines.h"

// Try to include credentials.h, if it doesn't exist, include the example
#if __has_include("credentials.h")
#include "credentials.h"
#else
#include "credentials_example.h"
#endif

extern PubSubClient mqttClient;
extern WiFiClient espClient;

// Setup the networking.
bool setup_networking();

// Publish the battery status.
void publish_battery_status(float voltage, float percentage);

// Publish the potentiometer status.
void publish_potentiometer_status(float value);

// Publish the buttons status.
void publish_buttons_status(int button_index);

// -----------------------------------------------------------------------------
// Private functions
// -----------------------------------------------------------------------------

// Setup the wifi.
void setup_wifi();

// Setup the MQTT client.
void setup_mqtt();

// Reconnect to the MQTT broker.
void reconnect_mqtt();

// Send a message to the MQTT broker.
void send_mqtt_message(const char *topic, const char *message);

#endif
