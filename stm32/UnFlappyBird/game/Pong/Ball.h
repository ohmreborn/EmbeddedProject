#pragma once
#include <stm32l4xx.h>
#include <ssd1306.h>
#include <stdint.h>

uint8_t get_score1(void);
uint8_t get_score2(void);

void ResetBall(TIM_HandleTypeDef *htim);

uint8_t RenderBall(TIM_HandleTypeDef *htim);
void RenderScore();

