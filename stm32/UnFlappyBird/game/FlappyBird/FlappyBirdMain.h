#pragma once
#include <stdint.h>
#include <stm32l4xx.h>

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


void FlappyBirdReset(TIM_HandleTypeDef *htim);
uint8_t FlappyBirdIdle(TIM_HandleTypeDef *htim, ADC_HandleTypeDef* hadc, uint32_t ldr);
void FlappyBirdPlay(void);

uint16_t FlappyBirdGetScore(void);