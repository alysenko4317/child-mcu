// File: lcd.c
// Description: HD44780-compatible 4-bit LCD driver

#include "../bsp/board.h"

#include <xc.h>
#include <stdint.h>
#include "lcd1602.h"

// Local helper prototypes
static void LCD_WriteNibble(uint8_t nibble);
static void LCD_Enable(void);

// Animation flag: true = delays on, false = delays off
static bool _animate = false;

void LCD_EnableAnimation(bool enable) {
    _animate = enable;
}

// Delay wrappers that check _animate flag
static inline void lcd_delay_ms(uint16_t ms)
{
    if (_animate) {
        while (ms--)
            __delay_ms(1);
    } else
        __delay_us(5);
}

/**
 * @brief  Initialize the LCD in 4-bit mode.
 *         Follows the HD44780 initialization sequence.
 */
void LCD_Init(void)
{
    // Initial delay after VCC rises to >4.5V
    __delay_ms(100);

    // Sequence: send 0x3 (0x30 nibble) three times
    LCD_WriteNibble(0x30);
    lcd_delay_ms(100);

    LCD_WriteNibble(0x30);
    lcd_delay_ms(200);

    LCD_WriteNibble(0x30);
    lcd_delay_ms(200);

    // Switch to 4-bit mode
    LCD_WriteNibble(0x20);
    lcd_delay_ms(200);

    // Function set: 4-bit, 2 lines, 5×8 dots
    LCD_Command(0b00101000);

    // Display ON, cursor ON, blink ON
    LCD_Command(0b00001111);

    // Clear display
    LCD_Clear();

    // Entry mode: increment cursor, no display shift
    LCD_Command(0b00000110);
}

/**
 * @brief  Send a command byte to the LCD.
 * @param  cmd: Instruction code.
 */
void LCD_Command(uint8_t cmd)
{
    LCD_RS = 0;  // Command mode

    // Send upper nibble
    LCD_WriteNibble(cmd);

    // Send lower nibble
    LCD_WriteNibble((uint8_t)(cmd << 4));

    // Wait for command to process
    lcd_delay_ms(2);
}

/**
 * @brief  Write a single character to the LCD.
 * @param  data: ASCII character code.
 */
void LCD_Char(uint8_t data)
{
    LCD_RS = 1;  // Data mode

    // Send upper nibble
    LCD_WriteNibble(data);

    // Send lower nibble
    LCD_WriteNibble((uint8_t)(data << 4));

    // Wait for write to complete
    lcd_delay_ms(2);
}

/**
 * @brief  Write a null‑terminated string to the LCD.
 * @param  str: Pointer to the string.
 */
void LCD_String(const char *str)
{
    while (*str)
    {
        LCD_Char((uint8_t)*str++);
        lcd_delay_ms(30);  // Character display delay
    }

    // Return control lines to idle
    LCD_RS = 0;
    LCD_EN = 0;
    LCD_DB7 = 0;
    LCD_DB6 = 0;
    LCD_DB5 = 0;
    LCD_DB4 = 0;
}

/**
 * @brief  Clear the display and return cursor to home.
 */
void LCD_Clear(void)
{
    LCD_Command(0x01);  // Clear display instruction
    __delay_ms(2);      // Delay for clear
}

/**
 * @brief  Write a 4‑bit nibble to the data lines and strobe Enable.
 * @param  nibble: Upper 4 bits are sent to DB7–DB4.
 */
static void LCD_WriteNibble(uint8_t nibble)
{
    // Place nibble on data lines
    LCD_DB7 = (nibble >> 7) & 0x01;
    LCD_DB6 = (nibble >> 6) & 0x01;
    LCD_DB5 = (nibble >> 5) & 0x01;
    LCD_DB4 = (nibble >> 4) & 0x01;

    // Pulse Enable to latch data
    LCD_Enable();
}

/**
 * @brief  Generate Enable pulse (high then low) to latch data.
 */
static void LCD_Enable(void)
{
    LCD_EN = 1;
    __delay_us(1);   // Enable pulse width > 450 ns
    LCD_EN = 0;
    __delay_us(100); // Post enable hold time
}
