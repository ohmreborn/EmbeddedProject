#include "ssd1306_fonts.h"
#include <bird.h>
#include <FlappyBirdMain.h>

#include <ssd1306.h>

#include <stdio.h>
#include <stdint.h>

static const float base_g = 500;
static const float base_vy = 100;
static const float base_vx = -100;
static const float base_fly_vy = -100;

static float g = base_g;
static float vy = base_vy;
static float vx = base_vx;
static float fly_vy = base_fly_vy;

static const uint8_t x = 50;
static float y = 20;
static const uint8_t w = 22;
static const uint8_t h = 17;
static uint8_t i = 0;

static const float dt = 1e-2;
static uint16_t score = 0;
static char score_str[16];
static AllObstacle UnFlappyObstacle;


static void init_obstacle(TIM_HandleTypeDef *htim) {
  UnFlappyObstacle.size_queue = 1;
  score = 0;
  for (uint8_t i = 0; i < NUM_OBSTACLE; i++) {
    UnFlappyObstacle.all_obstacle[i].x1 = 127;
    UnFlappyObstacle.all_obstacle[i].x2 = 127;
    UnFlappyObstacle.all_obstacle[i].y1 = __HAL_TIM_GET_COUNTER(htim);
    UnFlappyObstacle.all_obstacle[i].y2 = UnFlappyObstacle.all_obstacle[i].y1 + 30;
  }
}

static uint8_t update_bird(uint8_t i, TIM_HandleTypeDef *htim, ADC_HandleTypeDef* hadc){
  vy += g * dt;
  y += vy * dt;
  // get_obstacles_passed
  ssd1306_SetCursor(0, 0);
  sprintf(score_str, "%d", score);
  ssd1306_WriteString(score_str, Font_6x8, White);
  ssd1306_SetCursor(110, 0);
  DrawBitmapTransparentWhite(x, (uint8_t)y, epd_bitmap_allArray[i], w, h, htim);

  // Smaller hitbox for collision detection (inset by 4 pixels)
  float hitbox_x = (float)x + 4;
  float hitbox_y = y + 4;
  float hitbox_w = (float)w - 8;
  float hitbox_h = h - 8;

  // Check collision with ground or ceiling
  if (hitbox_y <= 0 || hitbox_y + hitbox_h >= 64) {
    return 1; // Bird died
  }

  // Check collision with obstacles
  for (uint8_t j = 0; j < UnFlappyObstacle.size_queue; j++) {
    Obstacle *obs = UnFlappyObstacle.all_obstacle + j;
    // Check if bird hitbox overlaps with obstacle
    if (hitbox_x + hitbox_w > obs->x1 && hitbox_x < obs->x2) {
      // Check if bird hitbox overlaps with obstacle gap or walls
      if (hitbox_y < obs->y1 || hitbox_y + hitbox_h > obs->y2) {
        return 1; // Bird hit obstacle
      }
    }
  }

  return 0; // Bird alive
}

static void update_obstacle(TIM_HandleTypeDef *htim){
  for (uint8_t i = 0; i < NUM_OBSTACLE; i++) {
    ssd1306_DrawObstacle((uint8_t)UnFlappyObstacle.all_obstacle[i].x1,
                         UnFlappyObstacle.all_obstacle[i].y1,
                         (uint8_t)UnFlappyObstacle.all_obstacle[i].x2,
                         UnFlappyObstacle.all_obstacle[i].y2);
  }

  for (uint8_t i = 0; i < UnFlappyObstacle.size_queue; i++) {
    Obstacle *curr_obstacle = UnFlappyObstacle.all_obstacle + i;
    if ((uint8_t)curr_obstacle->x1 == x + w){
      score++;
    }
    if (curr_obstacle->x2 == 127) {
      curr_obstacle->x1 += vx * dt;
      if (curr_obstacle->x2 - curr_obstacle->x1 >= 8) {
        curr_obstacle->x2 += curr_obstacle->x1 + 8;
      }
    } else if (curr_obstacle->x1 <= 0) {
      curr_obstacle->x2 += vx * dt;
      if (curr_obstacle->x2 <= 1) {
        UnFlappyObstacle.all_obstacle[i].x1 = 127;
        UnFlappyObstacle.all_obstacle[i].x2 = 127;
        UnFlappyObstacle.all_obstacle[i].y1 = __HAL_TIM_GET_COUNTER(htim);
        UnFlappyObstacle.all_obstacle[i].y2 =
            UnFlappyObstacle.all_obstacle[i].y1 + 30;
      }
    } else {
      curr_obstacle->x1 += vx * dt;
      curr_obstacle->x2 = curr_obstacle->x1 + 8;
    }
  }

  Obstacle *last_obstacle =
      UnFlappyObstacle.all_obstacle + (UnFlappyObstacle.size_queue - 1);
  if (last_obstacle->x2 < 127 - DISTANCE_OBSTACLE &&
      UnFlappyObstacle.size_queue < NUM_OBSTACLE) {
    UnFlappyObstacle.size_queue += 1;
  }
}

void FlappyBirdReset(TIM_HandleTypeDef *htim){
  y = 20;
  vy = 0;
  i = 0;
  init_obstacle(htim);
}

uint8_t FlappyBirdIdle(TIM_HandleTypeDef *htim, ADC_HandleTypeDef* hadc, uint32_t ldr){
  ssd1306_Fill(Black);

  float ratio = ((float)ldr + 1) / 4096.0f;
  vx = base_vx * ratio;
  vy *= ratio;
  g = base_g * ratio;
  fly_vy = base_fly_vy * ratio;

  update_obstacle(htim);
  if (update_bird(i, htim, hadc)) {
    return 1;
  }
  i = (i + 1) % 8;
  ssd1306_UpdateScreen();
  return 0;
}

void FlappyBirdPlay(void){
  vy = fly_vy;
}

uint16_t FlappyBirdGetScore(void) {
  return score;
}
