#include <bird.h>
#include <gravity.h>
#include <obstacle.h>
#include <ssd1306.h>


void play_UnFlappyBird(){
  for (uint8_t i = 0; i < FRAME_COUNT; i++) {
    ssd1306_Fill(Black);
    update_obstacle(&htim1);
    if (update_bird(i, &htim1, &hadc1)) {
      /* Bird died, transition to game over */
      app_state = APP_GAME_OVER;
      GameOver_Init(get_score());
      break;
    }
    ssd1306_UpdateScreen();
  }
}