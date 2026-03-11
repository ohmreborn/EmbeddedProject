#include "gameselect.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include <stdint.h>

// list of games - add more entries as you add games
static const char *games[] = {"UNFLAPPY", "Pong"};
static const uint8_t is_multi_player[2] = {0, 1};
#define NUM_GAMES 2

// state
static uint8_t current_index = 0;

void GameSelect_Init(void){
  current_index = 0;
}

void GameSelect_Render(void){
  ssd1306_Fill(Black);
  
  // title
  ssd1306_SetCursor(0, 0);
  ssd1306_WriteString("SELECT GAME", Font_7x10, White);
  
  // instructions
  ssd1306_SetCursor(0, 12);
  ssd1306_WriteString("Press: Navigate", Font_7x10, White);
  
  ssd1306_SetCursor(0, 24);
  ssd1306_WriteString("67: Select", Font_7x10, White);
  
  // list entries
  for(uint8_t i = 0; i < NUM_GAMES; i++){
    uint8_t y = 40 + i * 12;
    if(i == current_index){
      // highlight current selection
      ssd1306_DrawRectangle(0, y - 1, 127, y + 10, White);
      ssd1306_SetCursor(2, y);
      ssd1306_WriteString((char*)games[i], Font_7x10, Black);
    } else {
      ssd1306_SetCursor(2, y);
      ssd1306_WriteString((char*)games[i], Font_7x10, White);
    }
  }
  
  ssd1306_UpdateScreen();
}

void GameSelect_NavigateUp(void){
  current_index = (current_index+1) % NUM_GAMES;
  GameSelect_Render();
}

uint8_t GameSelect_CurrentIndex(void){
  return current_index;
}


uint8_t GameSelect_IsMultiPlayer(void){
  return is_multi_player[current_index];
}

uint8_t GameSelect_NUMGame(void){
  return NUM_GAMES;
}

const char *GameSelect_GetCurrentGameName(void){
  return games[current_index];
}
