#pragma once
#include <stdint.h>
#include <stm32l4xx.h>

void GamePlay_Play(void);
void GamePlay_Play2(void);
uint8_t GamePlay_Idle(TIM_HandleTypeDef *htim, ADC_HandleTypeDef *hadc, uint32_t ldr);
void GamePlay_Reset(TIM_HandleTypeDef *htim);
