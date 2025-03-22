#include "networking.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Sets up the networking
bool setup_networking()
{
    setup_wifi();
    setup_mqtt();

    DEBUG_PRINT(DEBUG_NETWORKING, "Networking setup complete");
    return true;
}

// Sets up the WiFi connection
void setup_wifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        DEBUG_PRINT(DEBUG_NETWORKING, ".");
    }

    DEBUG_PRINT(DEBUG_NETWORKING, "WiFi connected");
    DEBUG_PRINT(DEBUG_NETWORKING, "IP address: %s", WiFi.localIP().toString().c_str());
}

// Sets up the MQTT client
void setup_mqtt()
{
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

    DEBUG_PRINT(DEBUG_NETWORKING, "MQTT server: %s", MQTT_SERVER);
    DEBUG_PRINT(DEBUG_NETWORKING, "MQTT port: %d", MQTT_PORT);
}

// Publishes the battery status to the MQTT server
void publish_battery_status(float lastVoltage, float lastBatteryPercentage)
{
    send_mqtt_message(BATTERY_TOPIC, String(lastBatteryPercentage).c_str());
}

// Publishes the potentiometer status to the MQTT server
void publish_potentiometer_status(float value)
{
    send_mqtt_message(POTENTIOMETER_TOPIC, String(value).c_str());
}

// Publishes the buttons status to the MQTT server
void publish_buttons_status(int button_index)
{
    String topic = String(BUTTONS_TOPIC) + String(button_index + 1); // +1 because button index is 0-based
    send_mqtt_message(topic.c_str(), "PRESSED");                     // Using "PRESSED" as a message to simplify handling on the other side
}

// Reconnects to the MQTT server
void reconnect_mqtt()
{
    while (!mqttClient.connected())
    {
        DEBUG_PRINT(DEBUG_NETWORKING, "Attempting MQTT connection...");
        String clientId = "ESP32Client-" + String(random(0xffff), HEX);

        if (mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD))
        {
            DEBUG_PRINT(DEBUG_NETWORKING, "connected");
            return;
        }

        DEBUG_PRINT(DEBUG_NETWORKING, "failed, rc=%d try again in 5 seconds", mqttClient.state());
        delay(5000);
    }
}

// Sends a message to the MQTT server
void send_mqtt_message(const char *topic, const char *message)
{
    DEBUG_PRINT(DEBUG_NETWORKING, "Sending message to MQTT server, topic: %s, message: %s", topic, message);
    if (!mqttClient.connected())
    {
        reconnect_mqtt();
    }
    mqttClient.publish(topic, message);
}
