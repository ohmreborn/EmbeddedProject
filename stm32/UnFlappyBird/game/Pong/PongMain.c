#include <PongMain.h>
#include <stdint.h>

#include <ssd1306.h>

static uint8_t PaddleLength = 20;

static float x1 = 0;
static float y1 = 127;
static float x2 = 127;
static float y2 = 127;

static const float base_g = 100;
static const float base_vy = 100;
static const float push_vy = -100;
static float g = base_g;
static float vy1 = base_vy;
static float vy2 = base_vy;
static float dt = 1e-1;

void PongReset(TIM_HandleTypeDef *htim){
  x1 = 0;
  y1 = 63 - PaddleLength;
  x2 = 127;
  y2 = 63 - PaddleLength;
  vy1 = base_vy;
  vy2 = base_vy;
}

void draw_paddles(){
  ssd1306_Fill(Black);
  ssd1306_Line((uint8_t)x1, (uint8_t)y1, (uint8_t)x1, (uint8_t)y1 + PaddleLength,
               White);
  ssd1306_Line((uint8_t)x2, (uint8_t)y2, (uint8_t)x2, (uint8_t)y2 + PaddleLength,
               White);
  ssd1306_UpdateScreen();
}

uint8_t PongIdle(TIM_HandleTypeDef *htim, ADC_HandleTypeDef* hadc, uint32_t ldr){
  if (y1+PaddleLength < 64) {
    vy1 += g * dt;
    y1 += vy1 * dt;
  } else {
    vy1 = 0;
    y1 = 63 - PaddleLength;
  }
  if (y2+PaddleLength < 64) {
    vy2 += g * dt;
    y2 += vy2 * dt;
  } else {
    vy2 = 0;
    y2 = 63 - PaddleLength;
  }
  draw_paddles();
  return 0;
}

void PongPlay(void){
    vy1 = push_vy;
    y1 = 63 - PaddleLength - 1;
}
void PongPlay2(void){
    vy2 = push_vy;
    y2 = 63 - PaddleLength - 1;
}
uint16_t PongGetScore(void){
    return 0;
}