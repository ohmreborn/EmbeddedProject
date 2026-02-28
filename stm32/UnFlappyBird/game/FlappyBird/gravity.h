#pragma once
#include <stdint.h>
#include "stm32l4xx_hal.h"
extern float g;
extern float x;
extern float y;
extern uint8_t w;
extern uint8_t h;
extern float vx;
extern float vy;
extern float fly_vy;
extern float dt;

void update_bird(uint8_t i, TIM_HandleTypeDef *htim);
void reset_game(TIM_HandleTypeDef *htim);