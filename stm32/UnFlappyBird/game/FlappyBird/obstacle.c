#include "ssd1306.h"
#include <obstacle.h>
#include <gravity.h>
#include <stdint.h>

AllObstacle UnFlappyObstacle;
void ssd1306_DrawObstacle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
    ssd1306_FillRectangle(x1, 0, x2, y1, White);
    ssd1306_FillRectangle(x1, y2, x2, 63, White);
}

void init_obstacle(TIM_HandleTypeDef *htim) {
  UnFlappyObstacle.size_queue = 1;
  for (uint8_t i = 0; i < NUM_OBSTACLE; i++) {
    UnFlappyObstacle.all_obstacle[i].x1 = 127;
    UnFlappyObstacle.all_obstacle[i].x2 = 127;
    UnFlappyObstacle.all_obstacle[i].y1 = __HAL_TIM_GET_COUNTER(htim);
    UnFlappyObstacle.all_obstacle[i].y2 = UnFlappyObstacle.all_obstacle[i].y1 + 30;
  }
}

void update_obstacle(TIM_HandleTypeDef *htim){
    for (uint8_t i=0;i<NUM_OBSTACLE;i++){
        ssd1306_DrawObstacle(
            (uint8_t)UnFlappyObstacle.all_obstacle[i].x1, 
            UnFlappyObstacle.all_obstacle[i].y1, 
            (uint8_t)UnFlappyObstacle.all_obstacle[i].x2, 
            UnFlappyObstacle.all_obstacle[i].y2
        );
    }

    for (uint8_t i=0;i<UnFlappyObstacle.size_queue;i++){
      Obstacle *curr_obstacle = UnFlappyObstacle.all_obstacle + i;
      if (curr_obstacle->x2 == 127) {
        curr_obstacle->x1 += vx * dt;
        if (curr_obstacle->x2 - curr_obstacle->x1 >= 8) {
          curr_obstacle->x2 += curr_obstacle->x1 + 8;
        }
      } else if (curr_obstacle->x1 <= 0){
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

    Obstacle *last_obstacle = UnFlappyObstacle.all_obstacle + (UnFlappyObstacle.size_queue - 1);
    if (last_obstacle->x2 < 127 - DISTANCE_OBSTACLE && UnFlappyObstacle.size_queue < NUM_OBSTACLE){
      UnFlappyObstacle.size_queue += 1;
    }
}