#include "gameselect.h"
#include <stdint.h>
#include <stm32l4xx.h>

#include <gameplay.h>
#include <FlappyBirdMain.h>

void GamePlay_Play(void){
    FlappyBirdPlay();
}

static uint8_t (*GameFunc[2])(TIM_HandleTypeDef *htim, ADC_HandleTypeDef *hadc,
                    uint32_t ldr) = {&FlappyBirdIdle, &FlappyBirdIdle};
uint8_t GamePlay_Idle(TIM_HandleTypeDef *htim, ADC_HandleTypeDef *hadc, uint32_t ldr){
    return GameFunc[GameSelect_CurrentIndex()](htim, hadc, ldr);
}

uint16_t GamePlayGetScore(void){
    return FlappyBirdGetScore();
}

void GamePlay_Reset(TIM_HandleTypeDef *htim){
    FlappyBirdReset(htim);
}