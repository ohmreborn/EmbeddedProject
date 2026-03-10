/* Game Over menu implementation */

#include "gameselect.h"
#include <gameover.h>
#include <FlappyBirdGameOver.h>
#include <PongGameOver.h>


#include <ssd1306.h>
#include <ssd1306_fonts.h>


static void (*GameInit[2])(void) = {&FlappyBirdGameOver_Init, &PongGameOver_Init};
void GameOver_Init(void){
  GameInit[GameSelect_CurrentIndex()]();
}

static void (*GameRender[2])(void) = {&FlappyBirdGameOver_Render, &PongGameOver_Render};
void GameOver_Render(void){
  GameRender[GameSelect_CurrentIndex()]();
}

static void (*GameRestart[2])(void) = {&FlappyBirdGameOver_Restart, &PongGameOver_Restart};
void GameOver_Restart(void){
  GameRestart[GameSelect_CurrentIndex()]();
}