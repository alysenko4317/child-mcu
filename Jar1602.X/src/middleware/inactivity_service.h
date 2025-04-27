// inactivity_service.h
#ifndef INACTIVITY_SERVICE_H
#define INACTIVITY_SERVICE_H

#include <stdint.h>

// Callback type for inactivity events
typedef void (*InactivityCallback)(void);

// Initialize the inactivity service with a timeout (in ms) and a callback
void InactivityService_Init(uint32_t timeout_ms, InactivityCallback cb);

// Reset the inactivity timer and allow callback to fire again
void InactivityService_Reset(void);

// Poll the inactivity service; pass elapsed time (in ms) since last call
void InactivityService_Poll(uint32_t elapsed_ms);

// Immediately enter inactive state: fire callback and start periodic updates
void InactivityService_EnterInactive(void);

#endif // INACTIVITY_SERVICE_H
