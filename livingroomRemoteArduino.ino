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

int buttonIndex = -1; // Button index

// Flags to check if there are any updates to publish
bool button_update_available = false;
bool potentiometer_update_available = false;
bool battery_update_available = false;

/* -------------------------------------------------------------------------- */
/* ---------------------------- Forward Declarations ------------------------ */

void handle_wake_up();
void handle_idle();
bool handle_data_updates();

/* -------------------------------------------------------------------------- */
/* ---------------------------- Main Functions ------------------------------ */

void setup()
{
#if DEBUG_LEVEL != DEBUG_NONE
  Serial.begin(115200);
#endif

  handle_wake_up(); // Handle the wakeup (timing is important here)

  DEBUG_PRINT(DEBUG_WAKEUP, "Setting up...");

  /**
   * At first boot we need to initialize the buttons.
   * They are not initialized in the wake_up handler to save time, as they persist in deep sleep.
   * */
  esp_reset_reason_t reset_reason = esp_reset_reason();
  if (true) // E.g. first boot
  {
    setup_buttons();
    setup_potentiometer();
    setup_battery();
  }
#if DEBUG_LEVEL != DEBUG_NONE
  DEBUG_PRINT(DEBUG_WAKEUP, "Reset reason: %d", reset_reason);
#endif

  // Initialize the networking and LED. They take a long time to initialize, so we do it after we handle the wakeup.
  setup_networking();
  setup_led();

  // Since we are fully setup now, handle any updates to publish and handle idle time
  handle_data_updates();

  handle_idle();

  // We need to actively turn off the LED to eliminate current leakage.
  turn_off_led();
  // Set the wakeup sources
  esp_sleep_enable_ext1_wakeup(WAKEUP_BITMASK, ESP_EXT1_WAKEUP_ANY_LOW);               // Wake up on button press & potentiometer change
  esp_sleep_enable_timer_wakeup(BATTERY_CHECK_INTERVAL_MINUTES * 60 * uS_TO_S_FACTOR); // Wake up on battery check interval

  DEBUG_PRINT(DEBUG_WAKEUP, "Going to sleep!"); // Print a message before going to sleep

  esp_deep_sleep_start(); // Go to sleep
}

void loop()
{
  // The loop function doesn't run.
}

/* -------------------------------------------------------------------------- */
/* ---------------------------- Handler Functions --------------------------- */

void handle_wake_up()
{
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT1:

    // First we check the buttons (time critical)
    setup_buttons();
    button_update_available = check_buttons(buttonFlagMap, &buttonIndex);

    // Then we check the potentiometer (not time critical)
    setup_potentiometer();
    potentiometer_update_available = check_potentiometer(&lastPotReading);

    // Then we log the wakeup reason
    DEBUG_PRINT(DEBUG_WAKEUP, "Wake-up was caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    DEBUG_PRINT(DEBUG_WAKEUP, "Wake-up was caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    DEBUG_PRINT(DEBUG_WAKEUP, "Wake-up was caused by ULP");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    DEBUG_PRINT(DEBUG_WAKEUP, "Wake-up was caused by timer");
    setup_battery();
    battery_update_available = check_battery(&lastVoltage, &lastBatteryPercentage); // Cause a battery status update
    break;
  default:
    DEBUG_PRINT(DEBUG_WAKEUP, "Wake-up was not caused by deep sleep: %d\n", wakeup_reason);
    setup_battery();
    battery_update_available = check_battery(&lastVoltage, &lastBatteryPercentage); // Cause a battery status update
    break;
  }
}

void handle_idle()
{
  DEBUG_PRINT(DEBUG_WAKEUP, "Entered idle loop...");
  unsigned long lastActivityTime = millis();
  while (millis() - lastActivityTime < SLEEP_AFTER_IDLE_MINUTES * 60 * 1000) // Spin a loop for the idle timeout
  {
    handle_data_updates();

    if (check_buttons(buttonFlagMap, &buttonIndex))
    {
      button_update_available = true;
      lastActivityTime = millis();
    }

    if (check_potentiometer(&lastPotReading))
    {
      potentiometer_update_available = true;
      lastActivityTime = millis();
    }

    DEBUG_PRINT(DEBUG_WAKEUP, "...");
    delay(50); // Small delay to avoid unnecessary calls.
  }
}

// Checks if there are any updates to publish and publishes them
bool handle_data_updates()
{
  if (button_update_available)
  {
    indicate_button_press();             // Indicate the button press via the LED
    publish_buttons_status(buttonIndex); // Publish the button status
    button_update_available = false;     // Reset the flag
    return true;
  }
  if (potentiometer_update_available)
  {
    indicate_dimmer_change();                                                   // Indicate the dimmer change via the LED
    publish_potentiometer_status(get_potentiometer_percentage(lastPotReading)); // Publish the potentiometer status
    potentiometer_update_available = false;                                     // Reset the flag
    return true;
  }
  if (battery_update_available)
  {
    indicate_battery_low();                                     // Indicate the battery low via the LED
    publish_battery_status(lastVoltage, lastBatteryPercentage); // Publish the battery status
    battery_update_available = false;                           // Reset the flag
    return false;
  }
  return false;
}
