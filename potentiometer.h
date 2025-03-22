/**************************************************************************/
/*!
    @file     potentiometer.h
    @brief    Header file for potentiometer functionality

    @details  Contains functions for reading the potentiometer.

*/
/**************************************************************************/

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <Arduino.h>
#include <ADS1015_WE.h>
#include "driver/rtc_io.h"

#include "defines.h"

extern ADS1015_WE ADS;

// Setup the potentiometer
bool setup_potentiometer();

// Check if the potentiometer has changed
bool check_potentiometer(float *lastReading);

// Read the potentiometer
bool read_potentiometer(float *reading);

// Set the new ADC bounds
void set_new_adc_bounds(float *lastReading);

// Get the potentiometer percentage
float get_potentiometer_percentage(float reading);

#endif
