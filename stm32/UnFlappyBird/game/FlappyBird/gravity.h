#pragma once
#include <stdint.h>
#include "stm32l4xx_hal.h"

extern const float base_g;
extern const float base_vx;
extern const float base_vy;
extern const float base_fly_vy;

extern float g;
extern float vx;
extern float vy;
extern float fly_vy;

extern float x;
extern float y;
extern uint8_t w;
extern uint8_t h;
extern float dt;

void reset_game(TIM_HandleTypeDef *htim);
uint16_t get_score(void);
uint8_t update_bird(uint8_t i, TIM_HandleTypeDef *htim);

uint8_t update_bird(uint8_t i, TIM_HandleTypeDef *htim);
void reset_game(TIM_HandleTypeDef *htim);
uint16_t get_score(void);