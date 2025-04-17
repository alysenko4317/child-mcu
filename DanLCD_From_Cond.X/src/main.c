// main.c
#define F_CPU 16000000UL // 16 MHz clock frequency

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "drivers/ht1621.h"
#include "middleware/lcd_display.h"
#include "middleware/button_service.h"

// Global counter value (0-99)
volatile uint8_t number = 0;

// Timer1 interrupt handler: increments number every 1 second
ISR(TIMER1_COMPA_vect) {
    number = (number + 1) % 100;
    LCD_DisplayNumber(number);
}

void Timer1_Init(void) {
    TCCR1B |= (1 << WGM12);              // CTC mode
    OCR1A = 15624;                       // 1 sec @ 16 MHz / 1024
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
    TIMSK1 |= (1 << OCIE1A);             // Enable interrupt
}

// Internal state for per-segment toggling (4 segments per COM line)
static uint8_t segmentState[4] = {0};

void HT1621_ToggleSegment(uint8_t com, uint8_t seg) {
    segmentState[com] ^= (1 << seg);
    HT1621_SendData(com, segmentState[com]);
}

// Handles button press actions (called from button service)
void button_callback(uint8_t btnID) {
    static uint8_t seg1 = 0, seg2 = 0, seg3 = 0, seg4 = 0;

    switch(btnID) {
        case 0:
            HT1621_ToggleSegment(0, seg1);
            seg1 = (seg1 + 1) % 4;
            break;
        case 1:
            HT1621_ToggleSegment(1, seg2);
            seg2 = (seg2 + 1) % 4;
            break;
        case 2:
            HT1621_ToggleSegment(2, seg3);
            seg3 = (seg3 + 1) % 4;
            break;
        case 3:
            HT1621_ToggleSegment(3, seg4);
            seg4 = (seg4 + 1) % 4;
            break;
        case 4:
            for (uint8_t com = 0; com < 4; com++) {
                segmentState[com] = 0x0F;
                HT1621_SendData(com, segmentState[com]);
            }
            break;
        case 5:
            for (uint8_t com = 0; com < 4; com++) {
                segmentState[com] = 0x00;
                HT1621_SendData(com, segmentState[com]);
            }
            break;
        default:
            break;
    }
}

int main(void) {
    HT1621_Init();
    LCD_ClearAllSegments();
    Timer1_Init();
    sei();

    ButtonService_Init(button_callback);

    // Flash LCD segments on startup
    LCD_LightAllSegments(); _delay_ms(200);
    LCD_ClearAllSegments(); _delay_ms(200);
    LCD_LightAllSegments(); _delay_ms(200);
    LCD_ClearAllSegments();

    while (1) {
        ButtonService_Update();
        // Place for future tasks
    }
    return 0;
}
