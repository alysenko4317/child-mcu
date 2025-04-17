
#include "button_service.h"
#include "../drivers/buttons.h"

#include <util/delay.h>

// Pointer to callback function provided by user
static void (*_buttonCallback)(uint8_t) = 0;

// Track last known state
static uint8_t lastButton = 0xFF;

void ButtonService_Init(void (*callback)(uint8_t)) {
    _buttonCallback = callback;
    Buttons_Init();
}

void ButtonService_Update(void) {
    uint8_t btn = Buttons_Scan();
    if (btn != 0xFF && btn != lastButton) {
        lastButton = btn;

        if (_buttonCallback) {
            _buttonCallback(btn);
        }

        // Wait for button release to prevent retriggering
        while (Buttons_Scan() != 0xFF) {
            _delay_ms(10);
        }
        _delay_ms(50);  // Additional debounce delay
    } else if (btn == 0xFF) {
        lastButton = 0xFF;  // Reset when no button is pressed
    }
}

bool ButtonService_HasEvent(void) {
    return lastButton != 0xFF;
}
