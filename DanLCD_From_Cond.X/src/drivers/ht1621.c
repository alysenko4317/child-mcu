
#include "board.h"
#include "ht1621.h"

#include <avr/io.h>
#include <util/delay.h>

/**
 * @brief Select the HT1621 by pulling CS low.
 */
static inline void HT1621_Select(void) {
    HT1621_CS_PORT &= ~(1 << HT1621_CS_PIN); // CS LOW (active)
}

/**
 * @brief Deselect the HT1621 by pulling CS high.
 */
static inline void HT1621_Deselect(void) {
    HT1621_CS_PORT |= (1 << HT1621_CS_PIN); // CS HIGH (inactive)
}

/**
 * @brief Generate a single clock pulse on the WR pin.
 */
static inline void HT1621_WriteClock(void) {
    HT1621_WR_PORT &= ~(1 << HT1621_WR_PIN); // WR LOW
    _delay_us(4);
    HT1621_WR_PORT |= (1 << HT1621_WR_PIN); // WR HIGH
    _delay_us(4);
}

/**
 * @brief Write one bit to DATA pin, then pulse WR.
 * @param bitValue 0 or 1 to send.
 */
static inline void HT1621_WriteBit(uint8_t bitValue) {
    if (bitValue)
        HT1621_DATA_PORT |= (1 << HT1621_DATA_PIN); // DATA HIGH
    else
        HT1621_DATA_PORT &= ~(1 << HT1621_DATA_PIN); // DATA LOW

    HT1621_WriteClock();
}

/**
 * @brief Send a full byte (8 bits) MSB-first.
 * @param byte 8-bit value to transmit.
 */
static void HT1621_WriteByte(uint8_t byte) {
    for (uint8_t i = 0; i < 8; ++i) {
        HT1621_WriteBit((byte >> (7 - i)) & 0x01);
    }
}

void HT1621_Init(void) {
    // Configure DATA, CS, WR pins as outputs
    HT1621_DATA_DDR |= (1 << HT1621_DATA_PIN);
    HT1621_CS_DDR   |= (1 << HT1621_CS_PIN);
    HT1621_WR_DDR   |= (1 << HT1621_WR_PIN);

    // Set default states: DATA low, CS high (inactive), WR high
    HT1621_DATA_PORT &= ~(1 << HT1621_DATA_PIN);
    HT1621_CS_PORT   |=  (1 << HT1621_CS_PIN);
    HT1621_WR_PORT   |=  (1 << HT1621_WR_PIN);

    // Initial commands as recommended by datasheet
    HT1621_SendCommand(0x28); // Bias and oscillator settings
    HT1621_SendCommand(0x01); // System enable
    HT1621_SendCommand(0x03); // LCD display on
}

void HT1621_SendCommand(uint8_t cmd) {
    HT1621_Select();
    // Command mode prefix: 1,0,0
    HT1621_WriteBit(1);
    HT1621_WriteBit(0);
    HT1621_WriteBit(0);

    // Send the command byte
    HT1621_WriteByte(cmd);

    // One terminating bit
    HT1621_WriteBit(0);
    HT1621_Deselect();
}

void HT1621_SendData(uint8_t addr, uint8_t data) {
    HT1621_Select();
    // Write mode prefix: 1,0,1
    HT1621_WriteBit(1);
    HT1621_WriteBit(0);
    HT1621_WriteBit(1);

    // Send 6-bit address MSB-first
    for (uint8_t i = 0; i < 6; ++i) {
        HT1621_WriteBit((addr >> (5 - i)) & 0x01);
    }

    // Send 4-bit data MSB-first
    for (uint8_t i = 0; i < 4; ++i) {
        HT1621_WriteBit((data >> (3 - i)) & 0x01);
    }

    HT1621_Deselect();
}
