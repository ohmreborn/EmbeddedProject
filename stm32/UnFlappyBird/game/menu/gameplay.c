#include "PongMain.h"
#include "gameselect.h"
#include <stdint.h>
#include <stm32l4xx.h>

#include <gameplay.h>
#include <FlappyBirdMain.h>


static uint8_t (*GameIdle[2])(TIM_HandleTypeDef *htim, ADC_HandleTypeDef *hadc,
                    uint32_t ldr) = {&FlappyBirdIdle, &PongIdle};
uint8_t GamePlay_Idle(TIM_HandleTypeDef *htim, ADC_HandleTypeDef *hadc, uint32_t ldr){
    return GameIdle[GameSelect_CurrentIndex()](htim, hadc, ldr);
}

static void (*GamePlay[2])(void) = {&FlappyBirdPlay, &PongPlay};
void GamePlay_Play(void){
    GamePlay[GameSelect_CurrentIndex()]();
}

static void (*GamePlay2[2])(void) = {&FlappyBirdPlay, &PongPlay};
void GamePlay_Play2(void){
  if (GameSelect_IsMultiPlayer()) {
    GamePlay2[GameSelect_CurrentIndex()]();
  }
}

static uint16_t (*GameGetScore[2])(void) = {&FlappyBirdGetScore, &PongGetScore};
uint16_t GamePlayGetScore(void){
    return GameGetScore[GameSelect_CurrentIndex()]();
}

static void (*GameReset[2])(TIM_HandleTypeDef *htim) = {&FlappyBirdReset, &PongReset};
void GamePlay_Reset(TIM_HandleTypeDef *htim){
    GameReset[GameSelect_CurrentIndex()](htim);
}