
#include "lcd_display.h"

// digit-to-segment mapping:
// Each pair is {highSegments, lowSegments}
const uint8_t digitSegments[10][2] = {
    {0b00001111, 0b00000101}, // 0
    {0b00000000, 0b00000101}, // 1
    {0b00001101, 0b00000011}, // 2
    {0b00001001, 0b00000111}, // 3
    {0b00000010, 0b00000111}, // 4
    {0b00001011, 0b00000110}, // 5
    {0b00001111, 0b00000110}, // 6
    {0b00000001, 0b00000101}, // 7
    {0b00001111, 0b00001111}, // 8
    {0b00001011, 0b00000111}  // 9
};

void LCD_DisplayNumber(uint8_t number) {
    uint8_t tens = number / 10;
    uint8_t units = number % 10;

    // Send tens digit segments to addresses 7 and 6
    HT1621_SendData(7, digitSegments[tens][0]);
    HT1621_SendData(6, digitSegments[tens][1]);

    // Send units digit segments to addresses 5 and 4
    HT1621_SendData(5, digitSegments[units][0]);
    HT1621_SendData(4, digitSegments[units][1]);
}

void LCD_LightAllSegments(void) {
    // Light all segments at every address (0x00 - 0x1F)
    for (uint8_t addr = 0x00; addr < 0x20; ++addr) {
        HT1621_SendData(addr, 0x0F); // 0x0F turns on all 4 segments
    }
}

void LCD_ClearAllSegments(void) {
    // Clear all segments at every address (0x00 - 0x1F)
    for (uint8_t addr = 0x00; addr < 0x20; ++addr) {
        HT1621_SendData(addr, 0x00); // 0x00 turns off all segments
    }
}
