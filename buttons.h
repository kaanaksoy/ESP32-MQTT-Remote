
/**************************************************************************/
/*!
    @file     buttons.h
    @brief    Header file for buttons functionality

    @details  Contains functions for reading the buttons.

*/
/**************************************************************************/

#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "driver/rtc_io.h"
#include "defines.h"
#include "debug.h"

// Setup the buttons
bool setup_buttons();

// Check the buttons
bool check_buttons(bool (*buttonFlagMap)[2], int *buttonIndex);

// Scan the matrix and set the button flags
bool scan_matrix(bool (*buttonFlagMap)[2]);

// Get the button index from the button flag map
int get_button_index(bool (*buttonFlagMap)[2]);

// Clear the button flags
void clear_button_flags(bool (*buttonFlagMap)[2]);

#endif
