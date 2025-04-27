// File: main.c

#include "bsp/board.h"
#include "middleware/button_service.h"
#include "middleware/lcd1602_service.h"
//#include "middleware/lcd1621_service.h"
#include "middleware/inactivity_service.h"
#include <xc.h>      // for __delay_ms
#include <stdint.h>  // for uint32_t
#include <stdbool.h>  // for uint32_t
//#define _XTAL_FREQ MCU_CLOCK_FREQ

extern void MCU_Init(void);

//----------------------------------------------------------------------------
// Global uptime counter (ms since reset)
static uint32_t g_uptime_ms = 0;

//----------------------------------------------------------------------------
// Button callbacks — reset inactivity and show a message
static void OnBtn1(void) {
    InactivityService_Reset();
    LCDMain_EnableAnimation(true);
    LCDMain_ShowMessage("Hello, JARIK");
    LCDMain_EnableAnimation(false);
}

static void OnBtn2(void) {
    InactivityService_Reset();
    LCDMain_EnableAnimation(true);
    LCDMain_ShowMessage("Rocketa GOTOVA");
    LCDMain_EnableAnimation(false);
}

static void OnBtn3(void) {
    InactivityService_Reset();
    LCDMain_EnableAnimation(true);
    LCDMain_ShowMessage("Rocketa PUSK");
    LCDMain_EnableAnimation(false);
}

static void OnBtn4(void) {
    InactivityService_Reset();
    LCDMain_ShowMessage("*");
    InactivityService_EnterInactive();
}

//----------------------------------------------------------------------------
// Called after 10 000 ms of no button presses.
// Formats g_uptime_ms into HH:MM:SS and displays it.
static void OnInactivity(void) {
    uint32_t total_sec = g_uptime_ms / 1000;
    uint8_t hours = (uint8_t)((total_sec / 3600UL) % 24);
    uint8_t minutes = (total_sec % 3600) / 60;
    uint8_t seconds = total_sec % 60;

    // Build a "HH:MM:SS" string
    char buf[12];
    buf[0] = '0' + (hours   / 10);
    buf[1] = '0' + (hours   % 10);
    buf[2] = ':';
    buf[3] = '0' + (minutes / 10);
    buf[4] = '0' + (minutes % 10);
    buf[5] = ':';
    buf[6] = '0' + (seconds / 10);
    buf[7] = '0' + (seconds % 10);
    buf[8]= '\0';

    LCDMain_EnableAnimation(false);
    LCDMain_ShowMessage(buf);
}

//----------------------------------------------------------------------------

int main(void)
{
    MCU_Init();

    // Initialize services
    LCDMain_Init();
    ButtonService_Init();
    InactivityService_Init(10000, OnInactivity);

    // Register button handlers
    ButtonService_RegisterCallback(1, OnBtn1);
    ButtonService_RegisterCallback(2, OnBtn2);
    ButtonService_RegisterCallback(3, OnBtn3);
    ButtonService_RegisterCallback(4, OnBtn4);

    // Show initial prompt
    LCDMain_ShowMessage("Press any button");

    // Main loop: poll services and keep time
    while (1)
    {
        ButtonService_Poll();
        InactivityService_Poll(10);
        __delay_ms(10);

        // Increment uptime by our loop period
        g_uptime_ms += 10;
    }

    // never reached
    return 0;
}

