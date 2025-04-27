// lcd_display_service.h
#ifndef LCD_DISPLAY_SERVICE_H
#define LCD_DISPLAY_SERVICE_H

#include <stdbool.h>
/**
 * @brief  Initialize the 1602 LCD display.
 */
void LCDMain_Init(void);

/**
 * @brief  Show a string on the 1602 LCD.
 */
void LCDMain_ShowMessage(const char *msg);

void LCDMain_EnableAnimation(bool enable);

#endif // LCD_DISPLAY_SERVICE_H
