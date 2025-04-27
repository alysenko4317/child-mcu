// inactivity_service.c

#include "inactivity_service.h"
#include <stdbool.h>

// Internal configuration values
static uint32_t _initialTimeout;   // ms before first inactivity callback
static uint32_t _updateInterval;   // ms between subsequent callbacks
static uint32_t _counter;          // accumulated time since last callback
static bool     _fired;            // indicates whether first callback has fired
static InactivityCallback _cb;     // user-provided callback function

// Initialize the service with the given timeout and callback
void InactivityService_Init(uint32_t timeout_ms, InactivityCallback cb)
{
    _initialTimeout = timeout_ms;
    _updateInterval = 1000;  // default update interval: 1 second
    _cb             = cb;
    _counter        = 0;
    _fired          = false;
}

// Reset the inactivity timer and allow the first event to fire again
void InactivityService_Reset(void)
{
    _counter = 0;
    _fired   = false;
}

// Poll the service, adding elapsed_ms to the counter
// Triggers callback after initial timeout, then every update interval
void InactivityService_Poll(uint32_t elapsed_ms)
{
    _counter += elapsed_ms;

    if (!_fired) {
        if (_counter >= _initialTimeout) {
            _fired = true;
            _counter -= _initialTimeout;
            if (_cb) {
                _cb();
            }
        }
    }
    else {
        if (_counter >= _updateInterval) {
            _counter -= _updateInterval;
            if (_cb) {
                _cb();
            }
        }
    }
}

// Immediately enter the inactive state: reset counter, mark fired, and invoke callback
void InactivityService_EnterInactive(void)
{
    _fired   = true;
  //  if (_cb) {
  //      _cb();
  //  }
}
