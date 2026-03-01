#pragma once
#include <stdint.h>

void GameOver_Init(uint16_t final_score);
void GameOver_Update(void);
void GameOver_Render(void);
uint8_t GameOver_IsRestartPressed(void);
