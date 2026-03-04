#pragma once

#include <stdint.h>

void MainMenu_Init(void);
void MainMenu_Render(void);

// Called when button press occurs in main menu. Returns index of selected game
// (0..NUM_GAMES-1) on double-click, or 255 if no selection yet.
uint8_t MainMenu_ButtonPress(void);

const char *MainMenu_GetGameName(uint8_t idx);