#include <bird.h>
#include <gravity.h>
#include <obstacle.h>
#include <stdint.h>


const float base_g = 500;
const float base_vy = 100;
const float base_vx = -100;
const float base_fly_vy = -100;

float g = base_g;
float vy = base_vy;
float vx = base_vx;
float fly_vy = base_fly_vy;

float x = 50;
float y = 20;
uint8_t w = 22;
uint8_t h = 17;

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
