#ifndef BOARD_H
#define BOARD_H

//------------------------------------------------------------------
// Define HT1621 control lines (mapped to Pro Micro pins)
//------------------------------------------------------------------

#define HT1621_DATA_PORT PORTB
#define HT1621_DATA_DDR  DDRB
#define HT1621_DATA_PIN  PORTB4  // Pro Micro Pin 8 (A8)

#define HT1621_CS_PORT PORTB
#define HT1621_CS_DDR  DDRB
#define HT1621_CS_PIN  PORTB5  // Pro Micro Pin 9 (A9)

#define HT1621_WR_PORT PORTB
#define HT1621_WR_DDR  DDRB
#define HT1621_WR_PIN  PORTB6  // Pro Micro Pin 10 (A10)

//------------------------------------------------------------------
// Group control pins for button groups (and LCD backlight control)
//------------------------------------------------------------------

// Group 1 control pin (D5 on PC6_MCU)
#define GROUP1_CTRL_PIN  PC6  
#define GROUP1_CTRL_PORT PORTC
#define GROUP1_CTRL_DDR  DDRC

// Group 2 control pin (D4 on PD4_MCU)
#define GROUP2_CTRL_PIN  PD4  
#define GROUP2_CTRL_PORT PORTD
#define GROUP2_CTRL_DDR  DDRD

// SPI input pins for button second leads (each SPI line serves two buttons)
#define BUTTON_SPI_SCK_BIT   1   //  SCK on PB1_MCU
#define BUTTON_SPI_MISO_BIT  2   // MISO on PB2_MCU
#define BUTTON_SPI_MOSI_BIT  3   // MOSI on PB3_MCU
#define BUTTON_SPI_PORT      PORTB
#define BUTTON_SPI_DDR       DDRB
#define BUTTON_SPI_PIN_REG   PINB

#endif // BOARD_H
