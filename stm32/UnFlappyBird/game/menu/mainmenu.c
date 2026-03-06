#include "mainmenu.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stm32l4xx_hal.h>

// list of games - add more entries as you add games
static const char *games[] = {"UNFLAPPY", "GAME2"};
#define NUM_GAMES (sizeof(games)/sizeof(games[0]))

// state
static uint8_t current_index = 0;
static uint32_t last_click_ms = 0;
static uint8_t click_count = 0;

void MainMenu_Render(void){
  ssd1306_Fill(Black);
  // title
  ssd1306_SetCursor(0,0);
  ssd1306_WriteString("SELECT GAME", Font_7x10, White);

  // list entries
  for(uint8_t i=0;i<NUM_GAMES;i++){
    uint8_t y = 16 + i * 12;
    if(i == current_index){
      // highlight
      ssd1306_DrawRectangle(0, y-1, 127, y+10, White);
      ssd1306_SetCursor(2, y);
      ssd1306_WriteString((char*)games[i], Font_7x10, Black);
    } else {
      ssd1306_SetCursor(2, y);
      ssd1306_WriteString((char*)games[i], Font_7x10, White);
    }
  }
  ssd1306_UpdateScreen();
}

uint8_t MainMenu_ButtonPress(void){
  uint32_t now = HAL_GetTick();
  if(now - last_click_ms < 500){
    click_count++;
  } else {
    click_count = 1;
  }
  last_click_ms = now;

  if(click_count == 2){
    click_count = 0;
    return current_index;
  } else {
    // move selection on first click
    current_index = (current_index + 1) % NUM_GAMES;
    return 255;
  }
}

const char *MainMenu_GetGameName(uint8_t idx){
  if(idx < NUM_GAMES) return games[idx];
  return "";
}