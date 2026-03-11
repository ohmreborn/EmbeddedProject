#include <stm32l4xx.h>

#include <ssd1306.h>
#include <ssd1306_fonts.h>

#include <Ball.h>

#include <stdint.h>
#include <stdio.h>

static float x = 64;
static float y = 32;
static float vx = 100;
static float vy = 100;
static float dt = 1e-2;
static uint8_t r = 4;

static uint8_t score1 = 0;
static char score_str1[10];
static uint8_t score2 = 0;
static char score_str2[10];

uint8_t get_score1(void){
  return score1;
}

uint8_t get_score2(void){
  return score2;
}

void ResetBall(TIM_HandleTypeDef *htim){
  x = 64;
  y = 32;
  vx = 80 + __HAL_TIM_GET_COUNTER(htim);
  if (__HAL_TIM_GET_COUNTER(htim) > 15){
    vx = -vx;
  }
  vy = 80 + __HAL_TIM_GET_COUNTER(htim);
  if (__HAL_TIM_GET_COUNTER(htim) > 15){
    vy = -vy;
  }
  if (score1 == 7 || score2 == 7){
    uint8_t btn_w = 80;
    uint8_t btn_h = 14;
    uint8_t screen_x1 = (128 - btn_w) / 2;
    uint8_t screen_y1 = 64 - 6 - btn_h; // keep top/bottom margins equal (6)
    uint8_t screen_x2 = screen_x1 + btn_w;
    uint8_t screen_y2 = screen_y1 + btn_h;
    ssd1306_InvertRectangle(screen_x1 + 1, screen_y1 + 1, screen_x2 - 1,
                            screen_y2 - 1);
    ssd1306_UpdateScreen();
    score1 = 0;
    score2 = 0;
  }
}

uint8_t RenderBall(TIM_HandleTypeDef *htim){
    x -= vx*dt;
    y -= vy*dt;
    if (y-r <= 0 || y+r >= 63){
        vy = -vy;
    }
    if (y - r <= 0) {
      y = r;
    }
    if (y + r >= 63) {
      y = 63-r;
    }

    if (x-r <= 0){
        x = r;
      if (ssd1306_IsHit(x - r, y)) {
        vx = -vx;
      }else{
        score2 += 1;
        if (score2 >= 7){
          return 2;
        }else{
          ResetBall(htim);
          return 1;
        }
      }
    }
    if (x+r >= 127){
        x = 127-r;
      if (ssd1306_IsHit(x + r, y)) {
        vx = -vx;
      }else{
        score1 += 1;
        if (score1 == 7){
          return 2;
        }else{
          ResetBall(htim);
          return 1;
        }
      }
    }
    ssd1306_DrawCircle((uint8_t)x,(uint8_t)y, r, White);
    return 0;
}

void RenderScore(){
    sprintf(score_str1, "%d", score1);
    sprintf(score_str2, "%d", score2);
    ssd1306_SetCursor(64-6, 0);
    ssd1306_WriteString(score_str1, Font_6x8, White);
    ssd1306_SetCursor(64+6, 0);
    ssd1306_WriteString(score_str2, Font_6x8, White);
}
