#pragma once
#include <stdint.h>
#include <stm32l4xx.h>


uint8_t PongIdle(TIM_HandleTypeDef *htim, ADC_HandleTypeDef* hadc, uint32_t ldr);
void PongPlay(void);
void PongPlay2(void);
void PongReset(TIM_HandleTypeDef *htim);