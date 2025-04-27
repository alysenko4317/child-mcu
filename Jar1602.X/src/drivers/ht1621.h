#ifndef HT1621_H
#define HT1621_H

#include <stdint.h>

/**
 * @brief  Initialize HT1621 pins and controller.
 */
void HT1621_Init(void);

/**
 * @brief  Send a command to HT1621.
 */
void HT1621_SendCommand(uint8_t cmd);

/**
 * @brief  Send data to a given address.
 */
void HT1621_SendData(uint8_t addr, uint8_t data);

/**
 * @brief  Light all segments for testing.
 */
void HT1621_LightAllSegments(void);

#endif // HT1621_H
