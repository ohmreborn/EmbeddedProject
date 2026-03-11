/* Game Over menu implementation */

#include "gameselect.h"
#include <gameover.h>
#include <FlappyBirdGameOver.h>
#include <PongGameOver.h>


#include <ssd1306.h>
#include <ssd1306_fonts.h>


static void (*GameRender[2])(void) = {&FlappyBirdGameOver_Render, &PongGameOver_Render};
void GameOver_Render(void){
  GameRender[GameSelect_CurrentIndex()]();
}