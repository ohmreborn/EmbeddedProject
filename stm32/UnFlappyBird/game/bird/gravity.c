#include <bird.h>
#include <gravity.h>
#include <obstacle.h>
#include <stdint.h>

float g = 2500;
float x = 50;
float y = 20;
uint8_t w = 22;
uint8_t h = 17;

float vy = 100;
float fly_vy = -200;
float dt = 1e-2;

void reset_game(TIM_HandleTypeDef *htim){
  y = 20;
  vy = 100;
  init_obstacle(htim);
}

void update_bird(uint8_t i, TIM_HandleTypeDef *htim ){
    vy += g*dt;
    y += vy*dt;
    DrawBitmapTransparentWhite(x, y, epd_bitmap_allArray[i], w, h, htim);
    if (y == 0 || y >= 64 - h){
        reset_game(htim);
    }
}
