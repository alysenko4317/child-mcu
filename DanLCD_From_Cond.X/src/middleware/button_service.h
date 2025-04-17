
#ifndef BUTTON_SERVICE_H
#define BUTTON_SERVICE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize the button service.
 *
 * Sets up hardware button scanning and registers the callback function
 * to be triggered when a valid button press is detected.
 *
 * @param callback Function pointer to user-defined handler: void callback(uint8_t btnID)
 */
void ButtonService_Init(void (*callback)(uint8_t));

/**
 * @brief Call periodically from main loop.
 *
 * Debounces input and triggers callback on button press.
 */
void ButtonService_Update(void);

/**
 * @brief Returns true if a button press was registered and handled recently.
 */
bool ButtonService_HasEvent(void);

#endif // BUTTON_SERVICE_H