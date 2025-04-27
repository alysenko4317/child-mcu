#include "board.h"

/**
 * @brief  Initialize MCU I/O: set all pins to inputs then enable needed outputs.
 */
void MCU_Init(void)
{
    /* Set all ports to input (safe state) */
    TRISA = 0xFF; TRISB = 0xFF;
    TRISC = 0xFF; TRISD = 0xFF;
    TRISE = 0xFF;
    
    /* Clear all latches */
    PORTA = 0; PORTB = 0;
    PORTC = 0; PORTD = 0;
    PORTE = 0;

    /* Disable Parallel Slave Port (PSP) mode */
    TRISEbits.PSPMODE = 0;   
    
    /* Enable PORTB pull ups */
    OPTION_REGbits.nRBPU = 0;

    /* Configure LCD pins as outputs */
    LCD_DB7_DIR = 0; LCD_DB6_DIR = 0;
    LCD_DB5_DIR = 0; LCD_DB4_DIR = 0;
    LCD_EN_DIR  = 0; LCD_RS_DIR  = 0;

    /* Configure HT1621 pins as outputs */
    HT1621_DATA_DIR = 0;
    HT1621_CS_DIR   = 0;
    HT1621_WR_DIR   = 0;

    /* Configure button pins as inputs */
    BTN1_DIR = 1; BTN2_DIR = 1;
    BTN3_DIR = 1; BTN4_DIR = 1;
}
