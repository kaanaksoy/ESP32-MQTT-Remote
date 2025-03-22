/**************************************************************************/
/*!
    @file     battery.h
    @brief    Header file for battery monitoring functionality

    @details  Contains functions for measuring battery voltage, calculating
              battery percentage, and managing battery status updates.

    @note     Voltage range: 3.0V (empty) to 4.2V (full) for LiPo/Li-Ion batteries

    @author   Kaan Aksoy
    @date     2025-03-22

    @see      https://wiki.dfrobot.com/SKU_DFR1075_FireBeetle_2_Board_ESP32_C6_Basic_Tutorial
              for the reference implementation this code is based on.
*/
/**************************************************************************/

#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include "debug.h"
#include "defines.h"

// This file contains the functions for the battery monitoring.

// Configure battery monitoring.
bool setup_battery();

// Read the battery voltage.
void read_battery_voltage(float *voltage);

// Get the battery percentage based on the voltage.
float get_battery_percentage(float voltage);

// Check the battery.
bool check_battery(float *lastVoltage, float *lastBatteryPercentage);

#endif
