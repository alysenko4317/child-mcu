#define F_CPU 16000000UL // 16 MHz clock frequency

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define HT1621 control lines (mapped to Pro Micro pins)
#define HT1621_DATA_PORT PORTB
#define HT1621_DATA_DDR  DDRB
#define HT1621_DATA_PIN  PORTB4  // Pro Micro Pin 8 (A8)

#define HT1621_CS_PORT PORTB
#define HT1621_CS_DDR  DDRB
#define HT1621_CS_PIN  PORTB5  // Pro Micro Pin 9 (A9)

#define HT1621_WR_PORT PORTB
#define HT1621_WR_DDR  DDRB
#define HT1621_WR_PIN  PORTB6  // Pro Micro Pin 10 (A10)

// Button input
#define BUTTON_DDR  DDRD
#define BUTTON_PORT PORTD
#define BUTTON_PIN  PIND
#define BUTTON_BIT  PIND2  // PD2 (INT0)

// Initialize GPIO pins
void HT1621_InitPins() {
    // Set HT1621 pins as output
    HT1621_DATA_DDR |= (1 << HT1621_DATA_PIN);
    HT1621_CS_DDR   |= (1 << HT1621_CS_PIN);
    HT1621_WR_DDR   |= (1 << HT1621_WR_PIN);

    // Initialize pin states
    HT1621_DATA_PORT &= ~(1 << HT1621_DATA_PIN); // DATA low
    HT1621_CS_PORT |= (1 << HT1621_CS_PIN);  // CS inactive (HIGH)
    HT1621_WR_PORT |= (1 << HT1621_WR_PIN);  // WR inactive (HIGH)

    // Button input (enable pull-up)
    BUTTON_DDR &= ~(1 << BUTTON_BIT);  // Set as input
    BUTTON_PORT |= (1 << BUTTON_BIT);  // Enable pull-up resistor
}

void HT1621_Select() {
    HT1621_CS_PORT &= ~(1 << HT1621_CS_PIN); // CS LOW (active)
}

void HT1621_Deselect() {
    HT1621_CS_PORT |= (1 << HT1621_CS_PIN); // CS HIGH (inactive)
}

void HT1621_WriteClock() {
    HT1621_WR_PORT &= ~(1 << HT1621_WR_PIN); // WR LOW
    _delay_us(10);
    HT1621_WR_PORT |= (1 << HT1621_WR_PIN); // WR HIGH
}

void HT1621_WriteBit(uint8_t bit) {
    if (bit)
        HT1621_DATA_PORT |= (1 << HT1621_DATA_PIN); // DATA HIGH
    else
        HT1621_DATA_PORT &= ~(1 << HT1621_DATA_PIN); // DATA LOW

    HT1621_WriteClock();
}

void HT1621_WriteByte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        HT1621_WriteBit((byte >> (7 - i)) & 0x01);
    }
}

void HT1621_SendCommand(uint8_t cmd) {
    HT1621_Select();
    
    // Send "Command Mode" (0b100)
    HT1621_WriteBit(1);
    HT1621_WriteBit(0);
    HT1621_WriteBit(0);

    // Send command
    HT1621_WriteByte(cmd);
    HT1621_WriteBit(0);

    HT1621_Deselect();
}

void HT1621_SendData(uint8_t addr, uint8_t data) {
    HT1621_Select();
    
    // Send "Write Mode" (0b101)
    HT1621_WriteBit(1);
    HT1621_WriteBit(0);
    HT1621_WriteBit(1);

    // Send address (6 bits)
    for (int i = 0; i < 6; i++) {
        HT1621_WriteBit((addr >> (5 - i)) & 0x01);
    }

    // Send data (4 bits)
    for (int i = 0; i < 4; i++) {
        HT1621_WriteBit((data >> (3 - i)) & 0x01);
    }

    HT1621_Deselect();
}

void HT1621_Init() {
    HT1621_InitPins();

    HT1621_SendCommand(0x28); // Bias and RC settings
    HT1621_SendCommand(0x01); // System Enable
    HT1621_SendCommand(0x03); // LCD ON
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

void HT1621_DisplayNumber(uint8_t number) {
    uint8_t tens = number / 10;
    uint8_t units = number % 10;

    HT1621_SendData(7, digitSegments[tens][0]);
    HT1621_SendData(6, digitSegments[tens][1]);

    HT1621_SendData(5, digitSegments[units][0]);
    HT1621_SendData(4, digitSegments[units][1]);
}

volatile uint8_t number = 0;

// Timer interrupt for 1-second increments
ISR(TIMER1_COMPA_vect) {
    number = (number + 1) % 100;
    HT1621_DisplayNumber(number);
}

void Timer1_Init() {
    TCCR1B |= (1 << WGM12); // CTC mode
    OCR1A = 15624;  // 1 sec at 16 MHz / 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
    TIMSK1 |= (1 << OCIE1A); // Enable compare interrupt
}

void Button_Check() {
    if (!(BUTTON_PIN & (1 << BUTTON_BIT))) { // If button pressed
        _delay_ms(50);  // Debounce
        if (!(BUTTON_PIN & (1 << BUTTON_BIT))) { 
            number = (number + 1) % 100;
            HT1621_DisplayNumber(number);
            while (!(BUTTON_PIN & (1 << BUTTON_BIT))); // Wait for release
        }
    }
}

void Blink_CS_WR_DATA() {
    // Toggle CS (active LOW)
  //  HT1621_CS_PORT ^= (1 << HT1621_CS_PIN);  // Toggle CS
  //  _delay_ms(500);                          // Wait 500ms

    // Toggle WR (active LOW)
    HT1621_WR_PORT ^= (1 << HT1621_WR_PIN);  // Toggle WR
   // _delay_ms(500);                          // Wait 500ms

    // Toggle DATA (normal logic)
   // HT1621_DATA_PORT ^= (1 << HT1621_DATA_PIN);  // Toggle DATA
    _delay_ms(1000);                              // Wait 500ms
}

int main_(void) {
    HT1621_InitPins(); // Initialize pins

    while (1) {
        Blink_CS_WR_DATA(); // Blink LEDs connected to CS, WR, DATA
    }

    return 0;
}

void HT1621_LightAllSegments() {
    for (unsigned char addr = 0x00; addr < 0x20; addr++) { // Assuming 32 addresses
        HT1621_SendData(addr, 0xF); // Write 0xF (all segments ON for each address)
    }
}

void HT1621_ClearAllSegments() {
    for (unsigned char addr = 0x00; addr < 0x20; addr++) { // Assuming 32 addresses
        HT1621_SendData(addr, 0x00); // Write 0x00 (all segments OFF for each address)
    }
}

// Define button pins (Pro Micro pin mappings)
#define BUTTON1_PIN PIND1  // Pro Micro Pin D2
#define BUTTON2_PIN PIND0  // Pro Micro Pin D3
#define BUTTON3_PIN PIND4  // Pro Micro Pin D4

// Function to initialize button pins as inputs
void Buttons_Init() {
    DDRD &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN) | (1 << BUTTON3_PIN)); // Set as input
    // No need to enable pull-ups since external pull-up resistors are used
}

// Function to read button states
uint8_t Read_Button(uint8_t pin) {
    return !(PIND & (1 << pin)); // Return 1 if button is pressed (active low)
}

int main(void) {
    Buttons_Init(); // Initialize button pins

    // Configure Pro Micro's built-in TX LED as an indicator
    DDRD |= (1 << PIND5); // Pro Micro TX LED (D5) as output
    PORTD &= ~(1 << PIND5); // Turn off the TX LED initially

    while (1) {
        if (Read_Button(BUTTON1_PIN)) {
            PORTD |= (1 << PIND5); // Turn on TX LED if Button 1 is pressed
        } else {
            PORTD &= ~(1 << PIND5); // Turn off TX LED otherwise
        }

        _delay_ms(100); // Simple debounce delay
    }

    return 0;
}

int main2(void) {
    HT1621_Init();
    Timer1_Init();
    sei(); // Enable global interrupts

    HT1621_LightAllSegments();
    _delay_ms(200);        
    HT1621_ClearAllSegments();
    _delay_ms(200);
    HT1621_LightAllSegments();
    _delay_ms(200);        
    HT1621_ClearAllSegments();
    
    while (1) {
        Button_Check();
    }
}
