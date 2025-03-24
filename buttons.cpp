#include "buttons.h"

// Setup the buttons
bool setup_buttons()
{
    // Initialize rows
    rtc_gpio_init(ROW_1);
    rtc_gpio_init(ROW_2);
    rtc_gpio_init(ROW_3);

    // Set rows as input only
    rtc_gpio_set_direction(ROW_1, RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_set_direction(ROW_2, RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_set_direction(ROW_3, RTC_GPIO_MODE_INPUT_ONLY);

    // If no pull-up resistors are used, uncomment the following lines
    // rtc_gpio_set_pull_mode(ROW_1, RTC_GPIO_PULLUP_ONLY);
    // rtc_gpio_set_pull_mode(ROW_2, RTC_GPIO_PULLUP_ONLY);
    // rtc_gpio_set_pull_mode(ROW_3, RTC_GPIO_PULLUP_ONLY);

    pinMode(COL_1, INPUT);
    pinMode(COL_2, INPUT);

    DEBUG_PRINT(DEBUG_BUTTONS, "Buttons initialized");
    return true;
}

// Scan the matrix and set the button flags (doesn't support multiple presses at once)
bool scan_matrix(bool (*buttonFlagMap)[2])
{
    DEBUG_PRINT(DEBUG_BUTTONS, "Scanning matrix...");

    // Set the columns as output
    pinMode(COL_1, OUTPUT);
    pinMode(COL_2, OUTPUT);

    // Set the columns to HIGH
    digitalWrite(COL_1, HIGH);
    digitalWrite(COL_2, HIGH);

    for (int col = COL_1; col <= COL_2; col++)
    {
        digitalWrite(col, LOW); // Activate column
        delayMicroseconds(50);  // Stabilize

        if (!rtc_gpio_get_level(ROW_1)) // Check if the first row is pressed
        {
            buttonFlagMap[0][col - COL_1] = true;                                            // Set the button flag
            DEBUG_PRINT(DEBUG_BUTTONS, "Button in ROW1, COL%d pressed!\n", col - COL_1 + 1); // Print the button press
            return true;
        }
        if (!rtc_gpio_get_level(ROW_2)) // Check if the second row is pressed
        {
            buttonFlagMap[1][col - COL_1] = true;                                            // Set the button flag
            DEBUG_PRINT(DEBUG_BUTTONS, "Button in ROW2, COL%d pressed!\n", col - COL_1 + 1); // Print the button press
            return true;
        }
        if (!rtc_gpio_get_level(ROW_3)) // Check if the third row is pressed
        {
            buttonFlagMap[2][col - COL_1] = true;                                            // Set the button flag
            DEBUG_PRINT(DEBUG_BUTTONS, "Button in ROW3, COL%d pressed!\n", col - COL_1 + 1); // Print the button press
            return true;
        }
        digitalWrite(col, HIGH); // Deactivate column
    }

    return false;
}

// Clear the button flags 2D array
void clear_button_flags(bool (*buttonFlagMap)[2])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            buttonFlagMap[i][j] = false;
        }
    }
}

// Get the button index from the button flag map
int get_button_index(bool (*buttonFlagMap)[2])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (buttonFlagMap[i][j])
            {
                return i * 2 + j;
            }
        }
    }
    return -1;
}

// Check the buttons, it will set the index of the pressed button if there is one
bool check_buttons(bool (*buttonFlagMap)[2], int *buttonIndex)
{
    if (scan_matrix(buttonFlagMap))
    {
        *buttonIndex = get_button_index(buttonFlagMap);
        clear_button_flags(buttonFlagMap);
        return true;
    }
    return false;
}
