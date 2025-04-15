#include <xc.h>

// Configuration bits
#pragma config FOSC = HS        // High-speed oscillator
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = OFF      // Power-up Timer disabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low Voltage Programming disabled
#pragma config CPD = OFF        // Data EEPROM memory code protection disabled
#pragma config WRT = OFF        // Flash program memory write protection off
#pragma config CP = OFF         // Code protection off

#define _XTAL_FREQ 4000000    // 4MHz oscillator frequency

// Pin definitions for the LCD
#define LCD_DB7 PORTCbits.RC2
#define LCD_DB6 PORTDbits.RD0
#define LCD_DB5 PORTDbits.RD1
#define LCD_DB4 PORTDbits.RD2
#define LCD_EN  PORTDbits.RD3
#define LCD_RS  PORTCbits.RC4
//#define LCD_BACKLIGHT PORTDbits.RD4

// HT1621 Pins
#define HT1621_DATA PORTDbits.RD4
#define HT1621_CS   PORTCbits.RC6
#define HT1621_WR   PORTCbits.RC7

// Pin Directions
#define HT1621_DATA_DIR TRISDbits.TRISD4
#define HT1621_CS_DIR   TRISCbits.TRISC6
#define HT1621_WR_DIR   TRISCbits.TRISC7


// Function prototypes
void LCD_Init();
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_String(const char *str);
void LCD_Clear();
void LCD_Enable();
void LCD_WriteNibble(unsigned char nibble);

void MCU_Init() {
    // Set all pins as inputs initially (safe state)
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;
    TRISD = 0xFF;
    TRISE = 0xFF;

    // Clear output latches
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;

    TRISEbits.PSPMODE = 0; // Disable Parallel Slave Port (PSP) mode
    
    // Configure LCD pins as outputs
    TRISCbits.TRISC2 = 0; // LCD_DB7
    TRISDbits.TRISD0 = 0; // LCD_DB6
    TRISDbits.TRISD1 = 0; // LCD_DB5
    TRISDbits.TRISD2 = 0; // LCD_DB4
    TRISDbits.TRISD3 = 0; // LCD_EN
    TRISCbits.TRISC4 = 0; // LCD_RS
   // TRISDbits.TRISD4 = 0; // LCD backlight
    
        // Configure RB1?RB4 as inputs (buttons)
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB4 = 1;

    // Enable pull-ups globally for PORTB
    OPTION_REGbits.nRBPU = 0; // Enable pull-ups globally

    // Ensure RB1?RB4 have pull-ups (other PORTB pins are inputs but do not use pull-ups)
    PORTBbits.RB1 = 1; // Pull-up enabled for RB1
    PORTBbits.RB2 = 1; // Pull-up enabled for RB2
    PORTBbits.RB3 = 1; // Pull-up enabled for RB3
    PORTBbits.RB4 = 1; // Pull-up enabled for RB4
}

void HT1621_InitPins() {
    // Configure HT1621 pins as outputs
    HT1621_DATA_DIR = 0; // DATA as output
    HT1621_CS_DIR = 0;   // CS as output
    HT1621_WR_DIR = 0;   // WR as output

    // Initialize HT1621 pins
    HT1621_DATA = 0;
    HT1621_CS = 1; // CS inactive (inversed)
    HT1621_WR = 1; // WR inactive (inversed)
}

void LCD_Init() {
    // Turn on the backlight
  //  LCD_BACKLIGHT = 1;
    LCD_EN = 0;

    // Step 1: Wait for more than 15 ms after VCC rises to 4.5V
    __delay_ms(100);

    LCD_WriteNibble(0x30);
    __delay_ms(100);
    
    LCD_WriteNibble(0x30);
    __delay_ms(200);
    
    LCD_WriteNibble(0x30);
    __delay_ms(200);
    
    LCD_WriteNibble(0x20);
    __delay_ms(200);

    // Step 4: Configure LCD for 4-bit mode
    LCD_Command(0b00101000); // Function set: 4-bit, 2-line, 5x8 dots
    LCD_Command(0b00001111); // Display ON, cursor ON, blink ON
    LCD_Clear();             // Clear display
    LCD_Command(0b00000110); // Entry mode: Increment cursor, no shift
}

void LCD_WriteNibble(unsigned char nibble) {
    LCD_RS = 0; // Command mode

    // Send the upper nibble (bits 4-7)
    LCD_DB7 = (nibble >> 7) & 1;
    LCD_DB6 = (nibble >> 6) & 1;
    LCD_DB5 = (nibble >> 5) & 1;
    LCD_DB4 = (nibble >> 4) & 1;

    // Pulse the Enable pin
    LCD_EN = 1;
    __delay_us(10); // Enable pulse width > 450 ns
    __delay_ms(5); // Enable pulse width > 450 ns
    LCD_EN = 0;
 //   __delay_us(1); // Command execution delay
}

void LCD_Command(unsigned char cmd) {
    LCD_RS = 0; // Command mode

    // Send upper nibble
    LCD_DB7 = (cmd >> 7) & 1;
    LCD_DB6 = (cmd >> 6) & 1;
    LCD_DB5 = (cmd >> 5) & 1;
    LCD_DB4 = (cmd >> 4) & 1;
    LCD_Enable();

    // Send lower nibble
    LCD_DB7 = (cmd >> 3) & 1;
    LCD_DB6 = (cmd >> 2) & 1;
    LCD_DB5 = (cmd >> 1) & 1;
    LCD_DB4 = cmd & 1;
    LCD_Enable();

    __delay_ms(2); // Command execution time
}

void LCD_Char(unsigned char data) {
    LCD_RS = 1; // Data mode

    // Send upper nibble
    LCD_DB7 = (data >> 7) & 1;
    LCD_DB6 = (data >> 6) & 1;
    LCD_DB5 = (data >> 5) & 1;
    LCD_DB4 = (data >> 4) & 1;
    LCD_Enable();

    // Send lower nibble
    LCD_DB7 = (data >> 3) & 1;
    LCD_DB6 = (data >> 2) & 1;
    LCD_DB5 = (data >> 1) & 1;
    LCD_DB4 = data & 1;
    LCD_Enable();

    __delay_ms(2); // Data write time
}

void LCD_String(const char *str) {
    while (*str) {
        LCD_Char(*str++);
        __delay_ms(50);
    }
    
    LCD_RS = 0;
    LCD_EN = 0;
    LCD_DB7 = 0;
    LCD_DB6 = 0;
    LCD_DB5 = 0;
    LCD_DB4 = 0;
}

void LCD_Clear() {
    LCD_Command(0x01); // Clear display command
    __delay_ms(2);
}

void LCD_Enable() {
    LCD_EN = 1; // Pulse the Enable pin
    __delay_us(1);
    __delay_ms(5);
    LCD_EN = 0;
    __delay_us(1);
//    __delay_us(100);
}

void HT1621_Select() {
    HT1621_CS = 0; // Activate CS (active LOW)
}

void HT1621_Deselect() {
    HT1621_CS = 1; // Deactivate CS
}

void HT1621_WriteClock() {
    HT1621_WR = 0; // Pull WR LOW
    __delay_us(10); // Small delay for timing
    HT1621_WR = 1; // Pull WR HIGH
}

void HT1621_WriteBit(unsigned char bit) {
    HT1621_DATA = bit;  // Set DATA line
    HT1621_WriteClock(); // Toggle WR
}

void HT1621_WriteByte(unsigned char byte) {
    for (int i = 0; i < 8; i++) {
        HT1621_WriteBit((byte >> (7 - i)) & 0x01); // Send MSB first
    }
}

void HT1621_SendCommand(unsigned char cmd) {
    HT1621_Select(); // Activate CS
    
    // Send "Command Mode" prefix (0b100)
    HT1621_WriteBit(1);
    HT1621_WriteBit(0);
    HT1621_WriteBit(0);

    // Send the actual command (8 bits)
    HT1621_WriteByte(cmd);
    HT1621_WriteBit(0);

    HT1621_Deselect(); // Deactivate CS
}

void HT1621_SendData(unsigned char addr, unsigned char data) {
    HT1621_Select(); // Activate CS
    
    // Send "Write Mode" prefix (0b101)
    HT1621_WriteBit(1);
    HT1621_WriteBit(0);
    HT1621_WriteBit(1);

    // Send the address (6 bits)
    for (int i = 0; i < 6; i++) {
        HT1621_WriteBit((addr >> (5 - i)) & 0x01);
    }

    // Send the data (4 bits or more depending on your LCD)
    for (int i = 0; i < 4; i++) {
        HT1621_WriteBit((data >> (3 - i)) & 0x01);
    }

    HT1621_Deselect(); // Deactivate CS
}

void HT1621_Init() {
    HT1621_InitPins(); // Configure pins

    // Example initialization commands (refer to HT1621 datasheet)
    HT1621_SendCommand(0x28); // Bias and RC settings
    HT1621_SendCommand(0x01); // System Enable
    HT1621_SendCommand(0x03); // LCD ON
}

void HT1621_LightAllSegments() {
    for (unsigned char addr = 0x00; addr < 0x20; addr++) { // Assuming 32 addresses
        HT1621_SendData(addr, 0xF); // Write 0xF (all segments ON for each address)
    }
}


void main_() {
    MCU_Init();
    LCD_Init();
    HT1621_Init();     // Initialize HT1621 controller

    HT1621_LightAllSegments(); // Light all HT1621 LCD segments

    // Example interaction with HT1621
   // HT1621_SendData(0x00, 0x0F); // Write 0xF to address 0x00
  //  HT1621_SendData(0x01, 0x05); // Write 0x5 to address 0x01

    // Clear LCD and display a welcome message
    LCD_Clear();
    LCD_String("Press a Button!");

    while (1) {
        // Button RB1
        if (PORTBbits.RB1 == 0) { // Button pressed (active low)
            __delay_ms(50);       // Simple debounce delay
            if (PORTBbits.RB1 == 0) {
                LCD_Clear();
                LCD_String("Privet JARIK");
                while (PORTBbits.RB1 == 0); // Wait for button release
            }
        }

        // Button RB2
        if (PORTBbits.RB2 == 0) {
            __delay_ms(50);
            if (PORTBbits.RB2 == 0) {
                LCD_Clear();
                LCD_String("ROCKETA Gotova...");
                while (PORTBbits.RB2 == 0);
            }
        }

        // Button RB3
        if (PORTBbits.RB3 == 0) {
            __delay_ms(50);
            if (PORTBbits.RB3 == 0) {
                LCD_Clear();
                LCD_String("Privet DANJA");
                while (PORTBbits.RB3 == 0);
            }
        }

        // Button RB4
        if (PORTBbits.RB4 == 0) {
            __delay_ms(50);
            if (PORTBbits.RB4 == 0) {
                LCD_Clear();
                LCD_String("ROCKETA - PUSK !!!");
                while (PORTBbits.RB4 == 0);
            }
        }
    }
}

void main0() {
	MCU_Init();
    LCD_Init();
    LCD_String("Hello, World!");
    HT1621_Init();
    
  //  TRISCbits.TRISC6 = 0; // Set RC6 as output
    while (1) {
        HT1621_CS = 0; // Turn LED ON
        HT1621_DATA = 1;
        __delay_ms(500);
        HT1621_CS = 1; // Turn LED OFF
        HT1621_DATA = 0;
        __delay_ms(500);
    }
}

void HT1621_ToggleSegment(unsigned char com, unsigned char seg) {
    static unsigned char segmentState[4] = {0}; // Store states for COM1-COM4
    unsigned char addr = com;                  // COM corresponds to address

    // Toggle the bit for the specific segment
    segmentState[com] ^= (1 << seg); 

    // Send updated state to HT1621
    HT1621_SendData(addr, segmentState[com]);
}

void HT1621_ClearAllSegments() {
    for (unsigned char addr = 0x00; addr < 0x20; addr++) { // Assuming 32 addresses
        HT1621_SendData(addr, 0x00); // Write 0x00 (all segments OFF for each address)
    }
}

void HT1621_TestDigitSegmentsWithButton() {
    unsigned char addresses[] = {4, 5, 6, 7}; // ?????? ???????? ?????????
    static unsigned char addrIndex = 0;       // ??????? ?????? ??????
    static unsigned char bitIndex = 0;        // ??????? ??? ????????

    // ?????? RB1 ??? ???????????? ?????????
    if (PORTBbits.RB1 == 0) {
        __delay_ms(50); // ??????? ???????????
        if (PORTBbits.RB1 == 0) {
            // ????????? ??? ???????? ?? ??????? ??????
            HT1621_SendData(addresses[addrIndex], 0x00);

            // ????????????? ?? ????????? ???????
            bitIndex++;
            if (bitIndex >= 8) {
                bitIndex = 0;
                addrIndex = (addrIndex + 1) % 4; // ???????????? ??????? (4-7)
            }

            // ???????? ??????? ???????
            unsigned char data = (1 << bitIndex);
            HT1621_SendData(addresses[addrIndex], data);

            // ???????? ??????? 1602 ? ??????? ??????? ? ?????
            LCD_Clear();
            LCD_String("Addr: ");
            LCD_Char('0' + addresses[addrIndex]);
            LCD_String(" Bit: ");
            LCD_Char('0' + bitIndex);

            // ???? ?????????? ??????
            while (PORTBbits.RB1 == 0);
        }
    }
}

const unsigned char digitSegments[10][2] = {
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

void HT1621_DisplayNumber(unsigned char number) {
    unsigned char tens = number / 10;  // ??????? ?????? (???????)
    unsigned char units = number % 10; // ??????? ?????? (???????)

    // ?????????? ??????? ?????? ?? ??????? 6 ? 7
    HT1621_SendData(7, digitSegments[tens][0]); // ?????? ? ????? 7
    HT1621_SendData(6, digitSegments[tens][1]); // ?????? ? ????? 6

    // ?????????? ??????? ?????? ?? ??????? 4 ? 5
    HT1621_SendData(5, digitSegments[units][0]); // ?????? ? ????? 5
    HT1621_SendData(4, digitSegments[units][1]); // ?????? ? ????? 4
}

void HT1621_IncrementNumberWithButton() {
    static unsigned char number = 0; // ??????? ?????

    // ???????? ??????? ?????? RB1
    if (PORTBbits.RB1 == 0) {
        __delay_ms(50); // ???????????
        if (PORTBbits.RB1 == 0) {
            // ????????? ?????
            number = (number + 1) % 100; // ????? ? ????????? 00-99

            // ?????????? ?????
            HT1621_DisplayNumber(number);

            // ???? ?????????? ??????
            while (PORTBbits.RB1 == 0);
        }
    }
}

void Timer0_Init() {
    OPTION_REG = 0b00000111; // Prescaler 1:256 (for slower counting)
    TMR0 = 0;                // Reset timer count
    INTCONbits.TMR0IE = 1;   // Enable TMR0 interrupt
    INTCONbits.GIE = 1;      // Enable global interrupts
}

volatile unsigned int timerCounter = 0; // Counter for timer interrupts
volatile unsigned char number = 0;      // Number to display

void __interrupt() ISR() {
    if (INTCONbits.TMR0IF) { // Check if TMR0 overflowed
        TMR0 = 0;            // Reset timer count
        INTCONbits.TMR0IF = 0; // Clear interrupt flag

        timerCounter++;
        if (timerCounter >= 61) { // ~1 second at 4 MHz with prescaler 1:256
            timerCounter = 0; // Reset the counter
            number = (number + 1) % 100; // Increment number and wrap around at 99
            HT1621_DisplayNumber(number); // Display updated number
        }
    }
}

void main() {
    MCU_Init();
    LCD_Init();
    HT1621_Init(); // Initialize HT1621 controller
    Timer0_Init();  // Initialize Timer0
        
    HT1621_LightAllSegments();
    __delay_ms(200);        
    HT1621_ClearAllSegments();
    __delay_ms(200);
    HT1621_LightAllSegments();
    __delay_ms(200);        
    HT1621_ClearAllSegments();
    
  //  while (1) {
  //      HT1621_IncrementNumberWithButton(); // ???????????? ????????? ? ???????
  //  }
            
    // Clear both displays
    LCD_Clear();
    LCD_String("Press a Button!");

    while (1) {
        // Button RB1 corresponds to COM1
        if (PORTBbits.RB1 == 0) {
            __delay_ms(50); // Simple debounce delay
            if (PORTBbits.RB1 == 0) {
                static unsigned char seg1 = 0; // Keep track of the current segment
                HT1621_ToggleSegment(0, seg1); // Toggle SEGx on COM1
                LCD_Clear();
                LCD_String("COM1-SEG");
                LCD_Char('0' + seg1); // Display segment number
                seg1 = (seg1 + 1) % 4; // Cycle through SEG0-SEG3
                while (PORTBbits.RB1 == 0); // Wait for button release
            }
        }

        // Button RB2 corresponds to COM2
        if (PORTBbits.RB2 == 0) {
            __delay_ms(50); // Simple debounce delay
            if (PORTBbits.RB2 == 0) {
                static unsigned char seg2 = 0;
                HT1621_ToggleSegment(1, seg2); // Toggle SEGx on COM2
                LCD_Clear();
                LCD_String("COM2-SEG");
                LCD_Char('0' + seg2);
                seg2 = (seg2 + 1) % 4; // Cycle through SEG0-SEG3
                while (PORTBbits.RB2 == 0); // Wait for button release
            }
        }

        // Button RB3 corresponds to COM3
        if (PORTBbits.RB3 == 0) {
            __delay_ms(50); // Simple debounce delay
            if (PORTBbits.RB3 == 0) {
                static unsigned char seg3 = 0;
                HT1621_ToggleSegment(2, seg3); // Toggle SEGx on COM3
                LCD_Clear();
                LCD_String("COM3-SEG");
                LCD_Char('0' + seg3);
                seg3 = (seg3 + 1) % 4; // Cycle through SEG0-SEG3
                while (PORTBbits.RB3 == 0); // Wait for button release
            }
        }

        // Button RB4 corresponds to COM4
        if (PORTBbits.RB4 == 0) {
            __delay_ms(50); // Simple debounce delay
            if (PORTBbits.RB4 == 0) {
                static unsigned char seg4 = 0;
                HT1621_ToggleSegment(3, seg4); // Toggle SEGx on COM4
                LCD_Clear();
                LCD_String("COM4-SEG");
                LCD_Char('0' + seg4);
                seg4 = (seg4 + 1) % 4; // Cycle through SEG0-SEG3
                while (PORTBbits.RB4 == 0); // Wait for button release
            }
        }
    }
}
