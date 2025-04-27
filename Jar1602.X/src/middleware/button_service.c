// button_service.c
#include "../bsp/board.h"
#include "button_service.h"

#define DEBOUNCE_MS       50
#define POLL_INTERVAL_MS  10

static uint8_t        _stableState;
static uint8_t        _debounceCount[4];
static ButtonCallback _cb[4];

void ButtonService_Init(void)
{
    // sample the real raw-pins so we don't generate a fake ?change? on startup
    // bit = 1 ? released (pull-up), 0 ? pressed
    uint8_t now = 0;
    now |= (BTN1 != 0) ? 0x01 : 0;
    now |= (BTN2 != 0) ? 0x02 : 0;
    now |= (BTN3 != 0) ? 0x04 : 0;
    now |= (BTN4 != 0) ? 0x08 : 0;
    _stableState = now;

    for (int i = 0; i < 4; i++) {
        _debounceCount[i] = 0;
        _cb[i] = 0;
    }
}

void ButtonService_RegisterCallback(uint8_t btnId, ButtonCallback cb)
{
    if (btnId >= 1 && btnId <= 4) {
        _cb[btnId - 1] = cb;
    }
}

void ButtonService_Poll(void)
{
    // read raw pins: bit = 1 ? released, 0 ? pressed
    uint8_t now = 0;
    now |= (BTN1 != 0) ? 0x01 : 0;
    now |= (BTN2 != 0) ? 0x02 : 0;
    now |= (BTN3 != 0) ? 0x04 : 0;
    now |= (BTN4 != 0) ? 0x08 : 0;

    for (int i = 0; i < 4; i++) {
        bool bitNow  = (now >> i) & 1;
        bool bitLast = (_stableState >> i) & 1;

        if (bitNow != bitLast) {
            _debounceCount[i] += POLL_INTERVAL_MS;
            if (_debounceCount[i] >= DEBOUNCE_MS) {
                // confirmed change: update stable state
                if (bitNow)
                    _stableState |=  (1 << i);
                else
                    _stableState &= ~(1 << i);

                _debounceCount[i] = 0;

                // if newly pressed (bitNow == 0)
                if (!bitNow && _cb[i]) {
                    _cb[i]();
                }
            }
        }
        else {
            _debounceCount[i] = 0;
        }
    }
}
