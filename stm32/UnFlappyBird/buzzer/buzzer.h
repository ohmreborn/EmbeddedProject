#pragma once

#include <stdint.h>
#include "stm32l4xx_hal.h"  // bring in GPIO types and HAL routines

// Play a simple tone at the given frequency (Hz) for duration (ms).
// If frequency is zero the buzzer will remain silent for the duration.
void Buzzer_PlayNote(uint32_t frequency, uint32_t duration_ms);

// Play a sequence of notes. Both arrays must have "count" elements;
// durations are given in ms.  Notes with frequency 0 insert rests.
static inline void Buzzer_PlaySequence(const uint32_t *freq, const uint32_t *dur_ms, size_t count) {
    for (size_t i = 0; i < count; i++) {
        Buzzer_PlayNote(freq[i], dur_ms[i]);
    }
}

// A little "loader" sound used at startup.
void Buzzer_Loader(void);

// Play the main menu theme song
void Buzzer_PlayTheme(void);

// Abort any currently-playing sequence (theme, loader, etc.)
// safe to call from an interrupt.
void Buzzer_Abort(void);

// convenience wrappers for an active buzzer
static inline void Buzzer_On(void)  { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); }
static inline void Buzzer_Off(void) { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); }
