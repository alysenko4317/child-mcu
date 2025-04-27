#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief  Initialize the HD44780 in 4‑bit mode.
 */
void LCD_Init(void);

/**
 * @brief  Send an instruction to the LCD controller.
 */
void LCD_Command(uint8_t cmd);

/**
 * @brief  Write a single character.
 */
void LCD_Char(uint8_t data);

/**
 * @brief  Write a NUL‑terminated string.
 */
void LCD_String(const char *str);

/**
 * @brief  Clear the display.
 */
void LCD_Clear(void);

void LCD_EnableAnimation(bool enable);

#endif // LCD_H
