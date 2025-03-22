#include "driver/rtc_io.h"

#include "defines.h"
#include "buttons.h"
#include "battery.h"
#include "potentiometer.h"
#include "networking.h"
#include "led.h"

RTC_DATA_ATTR bool buttonFlagMap[3][2] = {false}; // Button flag map
RTC_DATA_ATTR float lastPotReading = 0;           // Last potentiometer reading
RTC_DATA_ATTR float lastVoltage = 0;              // Last voltage reading
RTC_DATA_ATTR float lastBatteryPercentage = 0;    // Last battery percentage reading
int buttonIndex = -1;                             // Button index

bool button_update_available = false;        // Button update available
bool potentiometer_update_available = false; // Potentiometer update available
bool battery_update_available = false;       // Battery update available

void handle_wake_up()
{
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT1:
    DEBUG_PRINT(DEBUG_WAKEUP, "Wake-up was caused by external signal using RTC_IO");

    button_update_available = check_buttons(buttonFlagMap, &buttonIndex); // Check if any buttons were pressed

    potentiometer_update_available = check_potentiometer(&lastPotReading); // Check if the potentiometer has changed

    battery_update_available = check_battery(&lastVoltage, &lastBatteryPercentage); // Cause a battery status update

    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    DEBUG_PRINT(DEBUG_WAKEUP, "Wake-up was caused by battery measurement timer");

    battery_update_available = check_battery(&lastVoltage, &lastBatteryPercentage); // Cause a battery status update

    break;
  case ESP_SLEEP_WAKEUP_EXT0:
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
  case ESP_SLEEP_WAKEUP_ULP:
  default:
    DEBUG_PRINT(DEBUG_WAKEUP, "Wake-up was not caused by deep sleep: %d\n", wakeup_reason);

    battery_update_available = check_battery(&lastVoltage, &lastBatteryPercentage); // Cause a battery status update
    break;
  }
}

// Checks if there are any updates to publish and publishes them
void handle_data_updates()
{
  if (button_update_available)
  {
    publish_buttons_status(buttonIndex); // Publish the button status
    indicate_button_press();             // Indicate the button press via the LED
    button_update_available = false;     // Reset the flag
  }
  if (potentiometer_update_available)
  {
    publish_potentiometer_status(get_potentiometer_percentage(lastPotReading)); // Publish the potentiometer status
    indicate_dimmer_change();                                                   // Indicate the dimmer change via the LED
    potentiometer_update_available = false;                                     // Reset the flag
  }
  if (battery_update_available)
  {
    publish_battery_status(lastVoltage, lastBatteryPercentage); // Publish the battery status
    battery_update_available = false;                           // Reset the flag
  }
}

void handle_idle()
{
  unsigned long lastActivityTime = millis();
  while (millis() - lastActivityTime < SLEEP_AFTER_IDLE_MINUTES * 60 * 1000) // Spin a loop for the idle timeout
  {
    // Check if any buttons were pressed
    if (check_buttons(buttonFlagMap, &buttonIndex))
    {
      lastActivityTime = millis();
      publish_buttons_status(buttonIndex);
      indicate_button_press();
    }

    // Check if the potentiometer has changed
    if (check_potentiometer(&lastPotReading))
    {
      lastActivityTime = millis();
      publish_potentiometer_status(get_potentiometer_percentage(lastPotReading)); // Publish the potentiometer status
      indicate_dimmer_change();                                                   // Indicate the dimmer change via the LED
    }

    DEBUG_PRINT(DEBUG_WAKEUP, "Spinning...");
    delay(15); // Small delay to avoid unnecessary calls.
  }
}

void setup()
{
#if DEBUG_LEVEL != DEBUG_NONE
  Serial.begin(115200);
#endif

  DEBUG_PRINT(DEBUG_WAKEUP, "Setting up...");

  // The setup for functions references in the wakeup handler need to be initialized before it.
  setup_buttons();
  setup_potentiometer();
  setup_battery();

  handle_wake_up();

  // Initialize the networking and LED. They take a long time to initialize, so we do it after we handle the wakeup.
  setup_networking();
  setup_led();

  // Handle any updates to publish and handle idle time
  handle_data_updates();
  handle_idle();

  // Turn off the LED and set the wakeup sources
  turn_off_led();

  esp_sleep_enable_ext1_wakeup(WAKEUP_BITMASK, ESP_EXT1_WAKEUP_ANY_LOW);
  esp_sleep_enable_timer_wakeup(BATTERY_CHECK_INTERVAL_S * uS_TO_S_FACTOR);

  DEBUG_PRINT(DEBUG_WAKEUP, "Going to sleep");

  esp_deep_sleep_start();
}

void loop()
{
  // Loop does nothing, we're using the wakeup handler to handle the logic
}
