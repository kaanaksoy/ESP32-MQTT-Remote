#include "led.h"

Adafruit_NeoPixel statusLed(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool setup_led()
{
    statusLed.begin();
    statusLed.clear();
    statusLed.show();
    DEBUG_PRINT(DEBUG_LED, "LED initialized");
    return true;
}

// Indicate a button press
void indicate_button_press()
{
    indicate_with_fade(BUTTON_COLOR);
}

// Indicate a dimmer change
void indicate_dimmer_change()
{
    indicate_with_fade(DIMMER_COLOR);
}

// Indicate a battery low
void indicate_battery_low()
{
    indicate_with_fade(BATTERY_LOW_COLOR);
}

// Indicate a battery charge
void indicate_battery_charge()
{
    indicate_with_fade(BATTERY_CHARGE_COLOR);
}

// Indicates via led using a fade effect
void indicate_with_fade(Color color)
{
    unsigned long startTime = millis();
    while (millis() - startTime < FADE_DURATION)
    {
        float progress = (float)(millis() - startTime) / FADE_DURATION; // Calculate the progress of the fade
        float brightness = sin(progress * PI) * LED_BRIGHTNESS;         // Calculate the brightness of the fade
        brightness = constrain(brightness, 0, LED_BRIGHTNESS);          // Constrain the brightness to the LED_BRIGHTNESS

        write_led_color(color, brightness); // Write the color to the led
        delay(1);                           // Delay for 1ms
    }
    statusLed.clear(); // Set all pixels to 'off'
    statusLed.show();  // Update the strip
}

// Writes the color to the led
void write_led_color(Color color, int brightness)
{
    float factor = brightness / 255.0;
    statusLed.setPixelColor(0, color.r * factor, color.g * factor, color.b * factor);
    statusLed.show();
}

// We need to actively turn off the LED when going to sleep to prevent leakage current on the data line from draining the battery
void turn_off_led()
{
    statusLed.clear(); // Set all pixels to 'off'
    statusLed.show();  // Update the strip

    // Now explicitly set the pin low
    pinMode(NEOPIXEL_PIN, OUTPUT);
    digitalWrite(NEOPIXEL_PIN, LOW);
}
