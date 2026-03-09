#pragma once

#include <stdint.h>

// Initialize game selection menu
void GameSelect_Init(void);

// Render game selection menu to display
void GameSelect_Render(void);

// Handle A4 pin press (navigate)
void GameSelect_NavigateUp(void);
void GameSelect_NavigateDown(void);

// Handle A8 pin press (confirm selection)
// Returns index of selected game (0..NUM_GAMES-1) on confirm, or 255 if not confirmed yet
uint8_t GameSelect_CurrentIndex(void);
uint8_t GameSelect_NUMGame(void);

// Get currently selected game name
const char *GameSelect_GetCurrentGameName(void);
