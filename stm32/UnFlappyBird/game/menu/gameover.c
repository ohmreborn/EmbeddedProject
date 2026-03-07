/* Game Over menu implementation */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <gameover.h>

#include <ssd1306.h>
#include <ssd1306_fonts.h>


#define RESTART_BTN_PORT GPIOA
#define RESTART_BTN_PIN  GPIO_PIN_3
#define DEBOUNCE_COUNT 3

static uint8_t restart_pressed = 0;
static uint8_t debounce = 0;
static uint8_t pressed_registered = 0;
static uint16_t score = 0;
static char score_str[16];

void GameOver_Init(uint16_t final_score){
  score = final_score;
  restart_pressed = 0;
  debounce = 0;
  pressed_registered = 0;
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();
}

void GameOver_Update(void){
  /* Button handling is now done via interrupt in main.c HAL_GPIO_EXTI_Callback */
}

void GameOver_SetRestartPressed(void){
  restart_pressed = 1;
}

void GameOver_Render(void){
  ssd1306_Fill(Black);

  // Center GAME OVER title (9 chars * 11 pixels = 99 pixels, centered on 128)
  ssd1306_SetCursor(15, 0);
  ssd1306_WriteString("GAME OVER", Font_11x18, White);

  // Format and display score on one centered line
  sprintf(score_str, "Score: %u", score);
  size_t slen = strlen(score_str);
  uint8_t s_width = (uint8_t)slen * 11; // Font_11x18 width per char
  uint8_t s_x = (128 - s_width) / 2;
  if (s_x < 0) s_x = 0;
  ssd1306_SetCursor((uint8_t)s_x, 22);
  ssd1306_WriteString(score_str, Font_11x18, White);

  // Draw RESTART button and center text inside (both axes)
  uint8_t btn_w = 80;
  uint8_t btn_h = 14;
  uint8_t x1 = (128 - btn_w) / 2;
  uint8_t y1 = 64 - 6 - btn_h; // keep top/bottom margins equal (6)
  uint8_t x2 = x1 + btn_w;
  uint8_t y2 = y1 + btn_h;
  ssd1306_DrawRectangle(x1, y1, x2, y2, White);
  const char restart_txt[] = "RESTART";
  size_t rtlen = strlen(restart_txt);
  uint8_t rt_w = (uint8_t)rtlen * 6; // Font_6x8 width
  uint8_t rt_h = 8; // Font_6x8 height
  uint8_t rt_x = x1 + (btn_w > rt_w ? (btn_w - rt_w) / 2 : 0);
  uint8_t rt_y = y1 + (btn_h > rt_h ? (btn_h - rt_h) / 2 : 0);
  ssd1306_SetCursor(rt_x, rt_y);
  ssd1306_WriteString((char*)restart_txt, Font_6x8, White);

  if (debounce > 0) {
    ssd1306_InvertRectangle(x1+1, y1+1, x2-1, y2-1);
  }

  ssd1306_UpdateScreen();
}

uint8_t GameOver_IsRestartPressed(void){
  if (restart_pressed){
    restart_pressed = 0;
    return 1;
  }
  return 0;
}
