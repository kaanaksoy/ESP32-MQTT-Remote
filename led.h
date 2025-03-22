/**************************************************************************/
/*!
    @file     led.h
    @brief    Header file for LED functionality

    @details  Contains functions for controlling the LED.

*/
/**************************************************************************/

#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "defines.h"
#include "debug.h"

struct Color
{
    uint8_t r, g, b;
};

extern Color currentColor;

extern Adafruit_NeoPixel statusLed;

// Setup the LED
bool setup_led();

// Indicate a button press
void indicate_button_press();

// Indicate a dimmer change
void indicate_dimmer_change();

// Indicate a battery low
void indicate_battery_low();

// Indicate a battery charge
void indicate_battery_charge();

// Private functions
void write_led_color(Color color, int brightness);

// Indicate with a fade
void indicate_with_fade(Color color);

// Turn off the LED
void turn_off_led();
#endif
