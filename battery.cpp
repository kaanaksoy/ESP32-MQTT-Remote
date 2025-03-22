
#include "battery.h"

// Configure battery monitoring.
bool setup_battery()
{
    pinMode(BATT_ADC_PIN, INPUT);
    return true;
}

// Read the battery voltage.
void read_battery_voltage(float *voltage)
{
    *voltage = analogReadMilliVolts(BATT_ADC_PIN) * 2.0 * VOLTAGE_CALIBRATION_FACTOR / 1000.0;
    DEBUG_PRINT(DEBUG_BATTERY, "Battery voltage: %.3f V", *voltage);
}

// Get the battery percentage based on the voltage.
float get_battery_percentage(float voltage)
{
    float percentage = (voltage - 3.0) * 100.0 / (4.2 - 3.0);
    DEBUG_PRINT(DEBUG_BATTERY, "Battery percentage: %.3f %%", percentage);
    return constrain(percentage, 0, 100);
}

// Check the battery.
bool check_battery(float *lastVoltage, float *lastBatteryPercentage)
{
    float voltage;
    read_battery_voltage(&voltage);
    *lastVoltage = voltage;
    *lastBatteryPercentage = get_battery_percentage(voltage);
    return true;
}
