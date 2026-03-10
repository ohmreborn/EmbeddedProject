#include <PongMain.h>
#include <Ball.h>
#include <stdint.h>

#include <ssd1306.h>

static uint8_t PaddleLength = 20;

static float x1 = 0;
static float y1 = 127;
static float x2 = 127;
static float y2 = 127;

static const float base_g = 1000;
static const float base_vy = 100;
static const float push_vy = -200;
static float g = base_g;
static float vy1 = base_vy;
static float vy2 = base_vy;
static float dt = 1e-2;


void PongReset(TIM_HandleTypeDef *htim){
  x1 = 127;
  y1 = 63 - PaddleLength;
  x2 = 0;
  y2 = 63 - PaddleLength;
  vy1 = base_vy;
  vy2 = base_vy;
  ResetBall(htim);
}

static void draw_paddles(){
  ssd1306_Fill(Black);
  ssd1306_Line((uint8_t)x1, (uint8_t)y1, (uint8_t)x1, (uint8_t)y1 + PaddleLength,
               White);
  ssd1306_Line((uint8_t)x2, (uint8_t)y2, (uint8_t)x2, (uint8_t)y2 + PaddleLength,
               White);
}

uint8_t PongIdle(TIM_HandleTypeDef *htim, ADC_HandleTypeDef* hadc, uint32_t ldr){
  if (y1+PaddleLength < 63 && 0 < y1) {
    vy1 += g * dt;
    y1 += vy1 * dt;
  } else if(y1+PaddleLength >= 63){
    vy1 = 0;
    y1 = 63 - PaddleLength;
  }else{
    vy1 = 0;
    y1 = 1;
  }
  if (y2+PaddleLength < 63 && 0 < y2) {
    vy2 += g * dt;
    y2 += vy2 * dt;
  } else if(y2+PaddleLength >= 63){
    vy2 = 0;
    y2 = 63 - PaddleLength;
  }else{
    vy2 = 0;
    y2 = 1;
  }
  draw_paddles();
  RenderScore();
  uint8_t ball_status = RenderBall(htim);
  if (ball_status == 1){ // increase 1 score
    PongReset(htim);
  }else if(ball_status == 2){ // go to menu
    // PongReset(htim);
    return 1;
  }
  ssd1306_UpdateScreen();
  return 0;
}

void PongPlay(void){
    vy1 = push_vy;
    y1--;
}
void PongPlay2(void){
    vy2 = push_vy;
    y2--;
}