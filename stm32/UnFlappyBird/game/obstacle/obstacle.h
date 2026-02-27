#pragma once
#include "stm32l4xx_hal.h"
#include <ssd1306.h>
#include <stdint.h>

#define OBSTACLE_LENGTH  8
#define DISTANCE_OBSTACLE 32
#define NUM_OBSTACLE 128/(OBSTACLE_LENGTH + DISTANCE_OBSTACLE)

typedef struct {
    float x1;
    float x2;
    uint8_t y1;
    uint8_t y2;
} Obstacle;

typedef struct {
    Obstacle all_obstacle[NUM_OBSTACLE];
    uint8_t size_queue;
} AllObstacle;

extern float vx;

extern AllObstacle UnFlappyObstacle;

void ssd1306_DrawObstacle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void init_obstacle(TIM_HandleTypeDef *htim);

void update_obstacle(TIM_HandleTypeDef *htim);