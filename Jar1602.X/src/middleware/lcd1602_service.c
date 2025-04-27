// lcd_display_service.c

#include "lcd1602_service.h"
#include "../drivers/lcd1602.h"

void LCDMain_Init(void) {
    LCD_Init();
}

void LCDMain_EnableAnimation(bool enable) {
    LCD_EnableAnimation(enable);
}

void LCDMain_ShowMessage(const char *msg) {
    LCD_Clear();
    LCD_String(msg);
}
