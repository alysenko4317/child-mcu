// File: ht1621.c
// Description: HT1621 LCD controller driver

#include "../bsp/board.h"
//#define _XTAL_FREQ MCU_CLOCK_FREQ

#include <xc.h>
#include <stdint.h>
#include "ht1621.h"

// Local helper prototypes
static void HT1621_Select(void);
static void HT1621_Deselect(void);
static void HT1621_WriteClock(void);
static void HT1621_WriteBit(uint8_t bit);
static void HT1621_WriteByte(uint8_t byte);

/**
 * @brief  Initialize HT1621 pins and controller.
 */
void HT1621_Init(void)
{
    // Configure HT1621 pins as outputs
    HT1621_DATA_DIR = 0;
    HT1621_CS_DIR   = 0;
    HT1621_WR_DIR   = 0;

    // Set idle pin levels
    HT1621_DATA = 0;
    HT1621_CS   = 1;  // CS inactive (high)
    HT1621_WR   = 1;  // WR inactive (high)

    // Send initialization commands (see HT1621 datasheet)
    HT1621_SendCommand(0x28);  // Bias and RC oscillator settings
    HT1621_SendCommand(0x01);  // System enable
    HT1621_SendCommand(0x03);  // LCD display ON
}

/**
 * @brief  Send a command to HT1621.
 * @param  cmd: 8‑bit command to send.
 */
void HT1621_SendCommand(uint8_t cmd)
{
    HT1621_Select();

    // "Command mode" prefix: 100
    HT1621_WriteBit(1);
    HT1621_WriteBit(0);
    HT1621_WriteBit(0);

    // Send the command byte (MSB first)
    HT1621_WriteByte(cmd);

    // Final 'end' bit
    HT1621_WriteBit(0);

    HT1621_Deselect();
}

/**
 * @brief  Send data to a given address.
 * @param  addr: 6‑bit RAM address (0x00–0x1F).
 * @param  data: 4‑bit segment data (bits D3–D0).
 */
void HT1621_SendData(uint8_t addr, uint8_t data)
{
    HT1621_Select();

    // "Write mode" prefix: 101
    HT1621_WriteBit(1);
    HT1621_WriteBit(0);
    HT1621_WriteBit(1);

    // Send 6‑bit address (MSB first)
    for (int i = 5; i >= 0; --i) {
        HT1621_WriteBit((addr >> i) & 0x01);
    }

    // Send 4‑bit data (MSB first)
    for (int i = 3; i >= 0; --i) {
        HT1621_WriteBit((data >> i) & 0x01);
    }

    HT1621_Deselect();
}

/**
 * @brief  Light all segments across all addresses (for test/demo).
 */
void HT1621_LightAllSegments(void)
{
    for (uint8_t addr = 0x00; addr < 0x20; ++addr) {
        HT1621_SendData(addr, 0x0F);
    }
}

/*==============================================================================
  Local helper implementations
==============================================================================*/

/**
 * @brief  Pull CS low to begin transaction.
 */
static void HT1621_Select(void)
{
    HT1621_CS = 0;  // Active low
    __delay_us(1);
}

/**
 * @brief  Pull CS high to end transaction.
 */
static void HT1621_Deselect(void)
{
    HT1621_CS = 1;
    __delay_us(1);
}

/**
 * @brief  Toggle WR to clock in a bit.
 */
static void HT1621_WriteClock(void)
{
    HT1621_WR = 0;
    __delay_us(5);
    HT1621_WR = 1;
    __delay_us(5);
}

/**
 * @brief  Write a single bit on DATA line, then clock it.
 * @param  bit: Either 0 or 1.
 */
static void HT1621_WriteBit(uint8_t bit)
{
    HT1621_DATA = bit;
    HT1621_WriteClock();
}

/**
 * @brief  Write 8 bits (one byte), MSB first.
 * @param  byte: Byte to send.
 */
static void HT1621_WriteByte(uint8_t byte)
{
    for (int i = 7; i >= 0; --i) {
        HT1621_WriteBit((byte >> i) & 0x01);
    }
}
