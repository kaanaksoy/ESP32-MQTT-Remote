#include "potentiometer.h"
#include <Wire.h>
#include "debug.h"

// Global instance
ADS1015_WE ADS(ADS1015_ADDRESS);

bool setup_potentiometer()
{
    rtc_gpio_init(ADS1015_ALERT_PIN);                                    // Initialize the alert pin
    rtc_gpio_pullup_en(ADS1015_ALERT_PIN);                               // Enable the pullup resistor (comment out if you wire a physical pullup resistor)
    rtc_gpio_set_direction(ADS1015_ALERT_PIN, RTC_GPIO_MODE_INPUT_ONLY); // Set the direction to input only

    Wire.begin();
    ADS.setVoltageRange_mV(ADS1015_RANGE_4096);  // using 0-4.096V range since Vref is 3.3v
    ADS.setCompareChannels(ADS1015_COMP_0_GND);  // compare ADC0 to GND
    ADS.setAlertPinMode(ADS1015_ASSERT_AFTER_1); // alert after 1 conversion outside of window
    ADS.setConvRate(ADS1015_128_SPS);            // 128 samples per second (can be increased)
    ADS.setMeasureMode(ADS1015_CONTINUOUS);      // continuous mode
    ADS.setAlertLatch(ADS1015_LATCH_DISABLED);   // disable latching of alert pin (i.e. self resets after 8uS)

    float result = ADS.getResult_V(); // read the ADC value to ensure the previous settings are applied.
    set_new_adc_bounds(&result);      // Set the new ADC bounds based on current pot position
    return true;
}

bool check_potentiometer(float *lastReading)
{
    if (read_potentiometer(lastReading))
    {
        set_new_adc_bounds(lastReading); // Set the new ADC bounds based on current pot position
        return true;
    }
    return false;
}

bool read_potentiometer(float *reading)
{
    float lastReading = *reading;
    float newReading = ADS.getResult_V();
    DEBUG_PRINT(DEBUG_POT, "ADC Reading: %.3f", newReading);

    // Return true if the reading is above or below the threshold. This is useful when we're spinning an idle loop to minimize mqtt calls.
    if (lastReading > (newReading + ADS1015_ALERT_THRESHOLD_V) || lastReading < (newReading - ADS1015_ALERT_THRESHOLD_V))
    {
        *reading = newReading;
        return true;
    }
    return false;
}

void set_new_adc_bounds(float *lastReading)
{
    float lowerBound = max(*lastReading - ADS1015_ALERT_THRESHOLD_V, 0.0);               // Set the lower bound to the last reading minus the threshold
    float upperBound = min(*lastReading + ADS1015_ALERT_THRESHOLD_V, OPERATING_VOLTAGE); // Set the upper bound to the last reading plus the threshold

    ADS.setAlertModeAndLimit_V(ADS1015_WINDOW, upperBound, lowerBound); // Set the alert mode and limits

    DEBUG_PRINT(DEBUG_POT, "New ADC bounds set to: %.3f - %.3f\n", lowerBound, upperBound); // Print the new ADC bounds
}

// Get the potentiometer value as a percentage.
float get_potentiometer_percentage(float reading)
{
    return (reading - 0.0) / (OPERATING_VOLTAGE - 0.0) * 100;
}
