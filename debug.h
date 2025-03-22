// Debug macros
#define DEBUG_NONE 0
#define DEBUG_WAKEUP (1 << 0)
#define DEBUG_BUTTONS (1 << 1)
#define DEBUG_POT (1 << 2)
#define DEBUG_BATTERY (1 << 3)
#define DEBUG_NETWORKING (1 << 4)
#define DEBUG_LED (1 << 5)
#define DEBUG_ALL 0xFF

// Set your debug level here - can be combination of above flags
#define DEBUG_LEVEL (DEBUG_ALL)

// Debug print macro
#define DEBUG_PRINT(level, ...)     \
    if (DEBUG_LEVEL & (level))      \
    {                               \
        Serial.printf(__VA_ARGS__); \
        Serial.println();           \
    }
