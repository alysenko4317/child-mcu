#ifndef BOARD_H
#define BOARD_H

#include <xc.h>

/*==============================================================================
  Clock configuration
==============================================================================*/
#pragma config FOSC = HS        // High-speed oscillator
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = OFF      // Power-up Timer disabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low Voltage Programming disabled
#pragma config CPD = OFF        // Data EEPROM memory code protection disabled
#pragma config WRT = OFF        // Flash program memory write protection off
#pragma config CP = OFF         // Code protection off

#define _XTAL_FREQ 4000000    // 4MHz oscillator frequency

/*==============================================================================
  LCD pin assignments
==============================================================================*/
#define LCD_DB7         PORTCbits.RC2
#define LCD_DB6         PORTDbits.RD0
#define LCD_DB5         PORTDbits.RD1
#define LCD_DB4         PORTDbits.RD2
#define LCD_EN          PORTDbits.RD3
#define LCD_RS          PORTCbits.RC4

/* LCD pin directions */
#define LCD_DB7_DIR     TRISCbits.TRISC2
#define LCD_DB6_DIR     TRISDbits.TRISD0
#define LCD_DB5_DIR     TRISDbits.TRISD1
#define LCD_DB4_DIR     TRISDbits.TRISD2
#define LCD_EN_DIR      TRISDbits.TRISD3
#define LCD_RS_DIR      TRISCbits.TRISC4

/*==============================================================================
  HT1621 pin assignments
==============================================================================*/
#define HT1621_DATA     PORTDbits.RD4
#define HT1621_CS       PORTCbits.RC6
#define HT1621_WR       PORTCbits.RC7

/* HT1621 pin directions */
#define HT1621_DATA_DIR TRISDbits.TRISD4
#define HT1621_CS_DIR   TRISCbits.TRISC6
#define HT1621_WR_DIR   TRISCbits.TRISC7

/*==============================================================================
  Button inputs (active‑low with pull‑ups)
==============================================================================*/
#define BTN1            PORTBbits.RB1
#define BTN2            PORTBbits.RB2
#define BTN3            PORTBbits.RB3
#define BTN4            PORTBbits.RB4

#define BTN1_DIR        TRISBbits.TRISB1
#define BTN2_DIR        TRISBbits.TRISB2
#define BTN3_DIR        TRISBbits.TRISB3
#define BTN4_DIR        TRISBbits.TRISB4

#endif // BOARD_H
