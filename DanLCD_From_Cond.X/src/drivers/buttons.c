
#include "buttons.h"

#include <avr/io.h>
#include <util/delay.h>
#include "board.h"

// SPI input lines used to multiplex buttons
const uint8_t spiButtonBits[3] = {
    BUTTON_SPI_MISO_BIT,  // Button lines mapped via MISO
    BUTTON_SPI_MOSI_BIT,  // ... MOSI
    BUTTON_SPI_SCK_BIT    // ... SCK
};

void Buttons_Init(void) {
    // Set Group1 control pin (PC6) as output and initialize it to LOW (LCD backlight active)
    GROUP1_CTRL_DDR |= (1 << GROUP1_CTRL_PIN);
    GROUP1_CTRL_PORT &= ~(1 << GROUP1_CTRL_PIN);
    
    // Set Group2 control pin (PD7) as output and initialize it to LOW (LCD backlight active)
    GROUP2_CTRL_DDR |= (1 << GROUP2_CTRL_PIN);
    GROUP2_CTRL_PORT &= ~(1 << GROUP2_CTRL_PIN);
    
    // Configure SPI button pins as inputs; external pull-ups are used
    BUTTON_SPI_DDR &= ~((1 << BUTTON_SPI_MISO_BIT) |
                        (1 << BUTTON_SPI_MOSI_BIT) |
                        (1 << BUTTON_SPI_SCK_BIT));
}

uint8_t Buttons_Scan(void) {
    for (uint8_t line = 0; line < 3; line++) {
        // If the SPI input is LOW (active low via external pull-ups and button press)
        if (!(BUTTON_SPI_PIN_REG & (1 << spiButtonBits[line]))) {
            _delay_ms(10); // Debounce delay
            if (!(BUTTON_SPI_PIN_REG & (1 << spiButtonBits[line]))) {
                // Test Group1: temporarily set Group1 control pin (PC6) HIGH
                GROUP1_CTRL_PORT |= (1 << GROUP1_CTRL_PIN);
                _delay_us(50); // Short delay for signal stabilization
                if (BUTTON_SPI_PIN_REG & (1 << spiButtonBits[line])) {
                    // If the SPI line goes HIGH, a button from Group1 is pressed
                    GROUP1_CTRL_PORT &= ~(1 << GROUP1_CTRL_PIN); // Restore Group1 to LOW
                    return line * 2 + 0;
                }
                // Restore Group1 control pin and test Group2: set Group2 control pin (PD7) HIGH
                GROUP1_CTRL_PORT &= ~(1 << GROUP1_CTRL_PIN);
                GROUP2_CTRL_PORT |= (1 << GROUP2_CTRL_PIN);
                _delay_us(50);
                if (BUTTON_SPI_PIN_REG & (1 << spiButtonBits[line])) {
                    // If the SPI line goes HIGH, a button from Group2 is pressed
                    GROUP2_CTRL_PORT &= ~(1 << GROUP2_CTRL_PIN); // Restore Group2 to LOW
                    return line * 2 + 1;
                }
                // Ensure both group control pins are set back to LOW
                GROUP2_CTRL_PORT &= ~(1 << GROUP2_CTRL_PIN);
            }
        }
    }
    return 0xFF;  // No button pressed
}
