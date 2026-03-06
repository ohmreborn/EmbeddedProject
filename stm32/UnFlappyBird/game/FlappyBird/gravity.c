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

static uint16_t score = 0;

void reset_game(TIM_HandleTypeDef *htim){
  y = 20;
  vy = 100;
  score = 0;
  init_obstacle(htim);
}

uint16_t get_score(void){
  extern uint16_t get_obstacles_passed(void);
  return get_obstacles_passed() + 1;
}

uint8_t update_bird(uint8_t i, TIM_HandleTypeDef *htim, ADC_HandleTypeDef* hadc){
    vy += g*dt;
    y += vy*dt;
    DrawBitmapTransparentWhite(x, y, epd_bitmap_allArray[i], w, h, htim);
    
    // Smaller hitbox for collision detection (inset by 4 pixels)
    float hitbox_x = x + 4;
    float hitbox_y = y + 4;
    float hitbox_w = w - 8;
    float hitbox_h = h - 8;
    
    // Check collision with ground or ceiling
    if (hitbox_y <= 0 || hitbox_y + hitbox_h >= 64){
        return 1;  // Bird died
    }
    
    // Check collision with obstacles
    for (uint8_t j = 0; j < UnFlappyObstacle.size_queue; j++){
        Obstacle *obs = UnFlappyObstacle.all_obstacle + j;
        // Check if bird hitbox overlaps with obstacle
        if (hitbox_x + hitbox_w > obs->x1 && hitbox_x < obs->x2){
            // Check if bird hitbox overlaps with obstacle gap or walls
            if (hitbox_y < obs->y1 || hitbox_y + hitbox_h > obs->y2){
                return 1;  // Bird hit obstacle
            }
        }
    }

    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    uint32_t raw = HAL_ADC_GetValue(hadc);
    float ratio = ((float)raw + 1) / 4096.0f;
    vx = base_vx * ratio;
    vy *= ratio;
    g = base_g * ratio;
    fly_vy = base_fly_vy * ratio;

    return 0;  // Bird alive
}
