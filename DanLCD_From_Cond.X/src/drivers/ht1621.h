#ifndef HT1621_H
#define HT1621_H

#include <stdint.h>

/**
 * @brief Initialize the HT1621 LCD driver.
 *        Configures control pins and sends initial setup commands.
 */
void HT1621_Init(void);

/**
 * @brief Send a raw command byte to the HT1621.
 * @param cmd 8-bit command code defined by the HT1621 datasheet.
 */
void HT1621_SendCommand(uint8_t cmd);

/**
 * @brief Write 4-bit data to a specific RAM address of the HT1621.
 * @param addr 6-bit address (0x00 - 0x3F).
 * @param data 4-bit data to set the segment pattern.
 */
void HT1621_SendData(uint8_t addr, uint8_t data);

#endif
