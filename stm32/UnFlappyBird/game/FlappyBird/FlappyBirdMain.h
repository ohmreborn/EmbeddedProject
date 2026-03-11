#pragma once
#include <stdint.h>
#include <stm32l4xx.h>

void FlappyBirdReset(TIM_HandleTypeDef *htim);
uint8_t FlappyBirdIdle(TIM_HandleTypeDef *htim, ADC_HandleTypeDef* hadc, uint32_t ldr);
void FlappyBirdPlay(void);

uint16_t FlappyBirdGetScore(void);