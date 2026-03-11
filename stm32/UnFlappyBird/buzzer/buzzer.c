#include "buzzer.h"
#include "main.h" // for HAL and possibly TIM handle
#include "stm32l4xx_hal_gpio.h"
#include <stdbool.h>

// simple microsecond delay using DWT cycle counter
static void delay_us(uint32_t us)
{
    // make sure DWT is enabled
    if ((DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) == 0) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }

    uint32_t start = DWT->CYCCNT;
    uint32_t clk = HAL_RCC_GetHCLKFreq() / 1000000U;
    uint32_t ticks = us * clk;
    while ((DWT->CYCCNT - start) < ticks) {
        // spin
    }
}

void Buzzer_PlayNote(uint32_t frequency, uint32_t duration_ms)
{
    if (frequency == 0) {
        HAL_Delay(duration_ms);
        return;
    }

    // calculate half period in microseconds
    uint32_t half_period_us = 500000U / frequency; // = (1/freq)/2 * 1e6
    uint32_t total_cycles = (duration_ms * 1000U) / (2U * half_period_us);
    if (total_cycles == 0) {
        // if duration too short, just toggle once and exit
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
        delay_us(half_period_us);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
        return;
    }

    for (uint32_t i = 0; i < total_cycles; i++) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
        delay_us(half_period_us);
    }
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
}

// helper used by theme and loader
// flag used to cancel a running sequence
static volatile bool abort_seq = false;

void Buzzer_Abort(void)
{
    abort_seq = true;
}

static void play_sequence(const uint32_t *freq, const uint32_t *dur_ms, size_t count)
{
    abort_seq = false;
    for (size_t i = 0; i < count; i++) {
        if (abort_seq) break;
        Buzzer_PlayNote(freq[i], dur_ms[i]);
    }
}

void Buzzer_Loader(void)
{
    // simple ascending arpeggio as loader sound
    const uint32_t notes[] = {200, 400, 600, 800};
    const uint32_t dur[]   = {100, 100, 100, 100};
    play_sequence(notes, dur, sizeof(notes)/sizeof(notes[0]));
}

void Buzzer_PlayTheme(void)
{
    // expanded Littleroot Town theme with bridge and variation
    const uint32_t NOTE_C4  = 262;
    const uint32_t NOTE_D4  = 294;
    const uint32_t NOTE_E4  = 330;
    const uint32_t NOTE_F4  = 349;
    const uint32_t NOTE_G4  = 392;
    const uint32_t NOTE_A4  = 440;
    const uint32_t NOTE_AS4 = 466;
    const uint32_t NOTE_B4  = 494;
    const uint32_t NOTE_C5  = 523;
    const uint32_t NOTE_CS5 = 554;
    const uint32_t NOTE_D5  = 587;
    const uint32_t NOTE_DS5 = 622;
    const uint32_t NOTE_E5  = 659;
    const uint32_t NOTE_F5  = 698;
    const uint32_t NOTE_G5  = 784;
    const uint32_t REST     = 0;

    const uint32_t melody[] = {
        // Intro
        NOTE_C4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_AS4, NOTE_C5,
        // Main Theme - Part 1
        NOTE_D5, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_C4,
        NOTE_D5, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4,
        // Main Theme - Part 2 (Variation)
        NOTE_D5, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_C5,
        NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A4, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5,
        // Bridge
        NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_AS4, NOTE_C5,
        NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_C4
    };

    const uint32_t noteValues[] = {
        4, 4, 4, 2, 8, 8, 8, 8, 4, 2,
        4, 4, 4, 4, 8, 8, 4, 4, 2,
        4, 4, 4, 4, 8, 8, 4, 4, 2,
        4, 4, 4, 4, 8, 8, 4, 4, 2,
        4, 4, 4, 4, 4, 4, 8, 8, 2,
        4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 2
    };

    const size_t count = sizeof(melody)/sizeof(melody[0]);
    const uint32_t base_ms = 2400; // slower tempo (larger value = slower notes)
    uint32_t durations[count];
    for (size_t i = 0; i < count; i++) {
        durations[i] = base_ms / noteValues[i];
    }

    play_sequence(melody, durations, count);
}
