// button_service.h
#ifndef BUTTON_SERVICE_H
#define BUTTON_SERVICE_H

#include <stdint.h>
#include <stdbool.h>

typedef void (*ButtonCallback)(void);

/**
 * @brief  Initialize button inputs (must call after MCU_Init).
 */
void ButtonService_Init(void);

/**
 * @brief  Poll buttons and debounce (call each ~10 ms).
 */
void ButtonService_Poll(void);

/**
 * @brief  Register a callback for when btnId (1–4) is pressed.
 */
void ButtonService_RegisterCallback(uint8_t btnId, ButtonCallback cb);

#endif // BUTTON_SERVICE_H
