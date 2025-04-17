
#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <stdint.h>
#include "../drivers/ht1621.h"

/**
 * @brief Segment patterns for digits 0-9.
 *        Each entry holds two 4-bit values for the high and low segment banks.
 */
extern const uint8_t digitSegments[10][2];

/**
 * @brief Display a two-digit number (00-99) on the LCD.
 * @param number Value between 0 and 99.
 */
void LCD_DisplayNumber(uint8_t number);

/**
 * @brief Light up all segments on the LCD (test pattern).
 */
void LCD_LightAllSegments(void);

/**
 * @brief Clear all segments on the LCD.
 */
void LCD_ClearAllSegments(void);

#endif // LCD_DISPLAY_H
