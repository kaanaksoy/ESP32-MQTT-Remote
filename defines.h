/**************************************************************************/
/*!
    @file     defines.h
    @brief    Header file for definitions that you might want to change.ADS1015_ADDRESS

    @details  These definitions are used to configure the project for different
              wiring configurations and preferences.

    @author   Kaan Aksoy
    @date     2025-03-22

*/
/**************************************************************************/

#ifndef DEFINES_H
#define DEFINES_H

// Configuration
#define SLEEP_AFTER_IDLE_MINUTES 1        // Sleep after idle in minutes
#define BATTERY_CHECK_INTERVAL_MINUTES 10 // Time in minutes between battery checks

#define OPERATING_VOLTAGE 3.3 // If you are using a different supply voltage, change this. (it is used to calculate the potentiometer percentage)

// Button Matrix
#define ROW_1 GPIO_NUM_5 // Must be RTC GPIO
#define ROW_2 GPIO_NUM_7 // Must be RTC GPIO
#define ROW_3 GPIO_NUM_6 // Must be RTC GPIO

#define COL_1 22 // Regular GPIO
#define COL_2 23 // Regular GPIO

// ADS1015
#define ADS1015_ADDRESS 0x48           // I2C address of the ADS1015 @see https://cdn-shop.adafruit.com/datasheets/ads1015.pdf
#define ADS1015_ALERT_PIN GPIO_NUM_4   // Pin connected to the alert pin of the ADS1015 (Must be RTC GPIO)
#define ADS1015_ALERT_THRESHOLD_V 0.01 // Threshold for alert pin (in volts) (Increase this value to make the alert less sensitive and vice versa)

#define WAKEUP_BITMASK ((1ULL << ROW_1) | (1ULL << ROW_2) | (1ULL << ROW_3) | (1ULL << ADS1015_ALERT_PIN)) // Wakeup bitmask for the wakeup source

// WS2812 LED strip
#define NEOPIXEL_PIN GPIO_NUM_21 // Regular GPIO
#define NUM_LEDS 1               // Number of LEDs in the strip
#define FADE_DURATION 200        // Time in ms for the fade effect
#define LED_BRIGHTNESS 255       // Max brightness of the LED strip (0-255)

#define BUTTON_COLOR {0, 5, 255}      // Blue
#define DIMMER_COLOR {222, 5, 255}    // Purple
#define BATTERY_COLOR {33, 222, 112}  // Green
#define BATTERY_LOW_COLOR {255, 0, 0} // Red

// Battery Monitoring
#define BATT_ADC_PIN 0                   // ADC pin for the battery monitoring (For the Beetle ESP32-C6 V1.0 it's GPIO 0)
#define VOLTAGE_CALIBRATION_FACTOR 0.984 // Calibration factor for the battery voltage (differentiates between microcontrollers)
#define uS_TO_S_FACTOR 1000000ULL        /* Conversion factor for micro seconds to seconds */

// MQTT topics
#define BUTTONS_TOPIC "livingRoomRemote/buttons/"
#define POTENTIOMETER_TOPIC "livingRoomRemote/potentiometer"
#define BATTERY_TOPIC "livingRoomRemote/battery/percentage"

#endif
