/* Game Over menu implementation */
#include "Ball.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <Ball.h>

#include <ssd1306.h>
#include <ssd1306_fonts.h>


static char score_str[16];
static const uint8_t btn_w = 80;
static const uint8_t btn_h = 14;
static const uint8_t x1 = (128 - btn_w) / 2;
static const uint8_t y1 = 64 - 6 - btn_h; // keep top/bottom margins equal (6)
static const uint8_t x2 = x1 + btn_w;
static const uint8_t y2 = y1 + btn_h;

void PongGameOver_Render(void){
  ssd1306_Fill(Black);

  uint8_t score1 = get_score1();
  uint8_t score2 = get_score2();
  // Format and display score on one centered line
  sprintf(score_str, "Score: %u-%u", score1, score2);
  size_t slen = strlen(score_str);
  uint8_t s_width = (uint8_t)slen * 11; // Font_11x18 width per char
  uint8_t s_x = (128 - s_width) / 2;
  if (s_x < 0) s_x = 0;
  ssd1306_SetCursor((uint8_t)s_x, 0);
  ssd1306_WriteString(score_str, Font_11x18, White);


  ssd1306_SetCursor(15, 22);
  if (score1 > score2) {
    ssd1306_WriteString("Player 1 Wins", Font_7x10, White);
  }else{
    ssd1306_WriteString("Player 1 Wins", Font_7x10, White);
  }

  // Draw RESTART button and center text inside (both axes)
  ssd1306_DrawRectangle(x1, y1, x2, y2, White);
  const char restart_txt[] = "RESTART";
  size_t rtlen = strlen(restart_txt);
  uint8_t rt_w = (uint8_t)rtlen * 6; // Font_6x8 width
  uint8_t rt_h = 8; // Font_6x8 height
  uint8_t rt_x = x1 + (btn_w > rt_w ? (btn_w - rt_w) / 2 : 0);
  uint8_t rt_y = y1 + (btn_h > rt_h ? (btn_h - rt_h) / 2 : 0);
  ssd1306_SetCursor(rt_x, rt_y);
  ssd1306_WriteString((char*)restart_txt, Font_6x8, White);

  ssd1306_UpdateScreen();
}