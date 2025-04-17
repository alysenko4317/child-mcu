// include/buttons.h
#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

/**
 * @brief Initialize button input pins.
 *        Sets up group control lines and configures SPI button input pins as inputs.
 */
void Buttons_Init(void);

/**
 * @brief Scan extended buttons (up to 6) and detect which button is pressed.
 *
 * Button layout:
 * - Buttons are split into two groups: Group1 and Group2
 * - Each group is multiplexed via shared SPI input lines
 *
 * @return Button ID (0-5) if a button is detected, or 0xFF if none.
 */
uint8_t Buttons_Scan(void);

#endif // BUTTONS_H
