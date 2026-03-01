/* Home menu implementation */
#include "home.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define DEBOUNCE_COUNT 3

static uint8_t start_pressed = 0;
static uint8_t debounce = 0;
static uint8_t pressed_registered = 0;
static char scroll_text[] = "Press button to start";
static const uint8_t ui_margin = 6; // equal margin from all sides

void Home_Init(void){
  start_pressed = 0;
  debounce = 0;
  pressed_registered = 0;
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();
}

void Home_Update(void){
  if (HAL_GPIO_ReadPin(START_BTN_GPIO_Port, START_BTN_Pin) == GPIO_PIN_SET){
    if (debounce < DEBOUNCE_COUNT) debounce++;
    
    // Register press only once when debounce reaches threshold
    if (debounce >= DEBOUNCE_COUNT && !pressed_registered){
      start_pressed = 1;
      pressed_registered = 1;
    }
  } else {
    // Button released, reset debounce
    if (debounce > 0) debounce = 0;
    pressed_registered = 0;
  }
}

void Home_Render(void){
  ssd1306_Fill(Black);

    // Layout: equal margin around UI, small spacing above title
    const char title[] = "UNFLAPPY";
    size_t title_len = strlen(title);
    uint8_t title_w = (uint8_t)title_len * 11; // Font_11x18 width approx
    uint8_t title_x = (uint8_t)((128 - title_w) / 2);
    uint8_t title_y = ui_margin + 2; // a bit spacing above title
    ssd1306_SetCursor(title_x, title_y);
    ssd1306_WriteString((char*)title, Font_11x18, White);

    // Scrolling marquee: draw two copies to make continuous marquee
    ssd1306_SetCursor(0, title_y+18+4);
    ssd1306_WriteString(scroll_text, Font_6x8, White);

    // Draw START button centered horizontally and positioned with equal margins
    uint8_t btn_w = 80;
    uint8_t btn_h = 14;
    uint8_t x1 = (128 - btn_w) / 2;
    uint8_t y1 = 64 - ui_margin - btn_h; // keep bottom margin equal to top margin
    uint8_t x2 = x1 + btn_w;
    uint8_t y2 = y1 + btn_h;
    ssd1306_DrawRectangle(x1, y1, x2, y2, White);
    const char start_txt[] = "START";
    size_t start_len = strlen(start_txt);
    uint8_t txt_w = (uint8_t)start_len * 7; // Font_7x10 width
    uint8_t txt_h = 10; // Font_7x10 height
    uint8_t txt_x = x1 + (btn_w > txt_w ? (btn_w - txt_w) / 2 : 0);
    uint8_t txt_y = y1 + (btn_h > txt_h ? (btn_h - txt_h) / 2 : 0);
    ssd1306_SetCursor(txt_x, txt_y);
    ssd1306_WriteString((char*)start_txt, Font_7x10, White);

  if (debounce > 0) {
    ssd1306_InvertRectangle(x1+1, y1+1, x2-1, y2-1);
  }

  ssd1306_UpdateScreen();
}

uint8_t Home_IsStartPressed(void){
  if (start_pressed){
    start_pressed = 0;
    return 1;
  }
  return 0;
}
