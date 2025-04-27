// ht1621_display_service.c

#include "lcd1602_service.h"
#include "../drivers/ht1621.h"

// Segment tables (tens and units) — adjust per your wiring:
static const uint8_t SEG_HI[10] = {
    0b00001111, // 0
    0b00000000, // 1
    0b00001101, // 2
    0b00001001, // 3
    0b00000010, // 4
    0b00001011, // 5
    0b00001111, // 6
    0b00000001, // 7
    0b00001111, // 8
    0b00001011  // 9
};

static const uint8_t SEG_LO[10] = {
    0b00000101, // 0
    0b00000101, // 1
    0b00000011, // 2
    0b00000111, // 3
    0b00000111, // 4
    0b00000110, // 5
    0b00000110, // 6
    0b00000101, // 7
    0b00001111, // 8
    0b00000111  // 9
};

void LCD1621_Init(void)
{
    HT1621_Init();
}

void LCD1621_ShowNumber(uint8_t number)
{
    uint8_t tens  = (number / 10) % 10;
    uint8_t units =  number % 10;

    // write tens at addr 7 & 6
    HT1621_SendData(7, SEG_HI[tens]);
    HT1621_SendData(6, SEG_LO[tens]);

    // write units at addr 5 & 4
    HT1621_SendData(5, SEG_HI[units]);
    HT1621_SendData(4, SEG_LO[units]);
}
