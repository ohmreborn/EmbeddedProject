#include <stdint.h>
#include <string.h>

#include <ssd1306.h>
#include <ssd1306_fonts.h>

#include <gameselect.h>
#include <gamestart.h>

const uint8_t ui_margin = 6; // equal margin from all sides
char home_title[12]; // max length is 12 (include \0)
static size_t title_len;
static uint8_t title_w;
static uint8_t title_x;
static const uint8_t title_y = ui_margin + 2; // a bit spacing above title


const char scroll_text[22] = "Press button to start";
const uint8_t btn_w = 80;
const uint8_t btn_h = 14;
const uint8_t x1 = (128 - btn_w) / 2;
const uint8_t btn_y1 = 64 - ui_margin - btn_h; // keep bottom margin equal to top margin
const uint8_t x2 = x1 + btn_w;
const uint8_t btn_y2 = btn_y1 + btn_h;

const char start_txt[6] = "START";
const size_t start_len = strlen(start_txt);
const uint8_t txt_w = (uint8_t)start_len * 7; // Font_7x10 width
const uint8_t txt_h = 10;                     // Font_7x10 height
const uint8_t txt_x = x1 + (btn_w > txt_w ? (btn_w - txt_w) / 2 : 0);
const uint8_t txt_y = btn_y1 + (btn_h > txt_h ? (btn_h - txt_h) / 2 : 0);

static void GameMenu_Render(void){
  ssd1306_Fill(Black);

  // Layout: equal margin around UI, small spacing above title
  title_len = strlen(home_title);
  title_w = (uint8_t)title_len * 11; // Font_11x18 width approx
  title_x = (uint8_t)((128 - title_w) / 2);
  ssd1306_SetCursor(title_x, title_y);
  ssd1306_WriteString((char *)home_title, Font_11x18, White);

  // Scrolling marquee: draw two copies to make continuous marquee
  ssd1306_SetCursor(0, title_y + 18 + 4);
  ssd1306_WriteString((char*)scroll_text, Font_6x8, White);

  // Draw START button centered horizontally and positioned with equal margins
  ssd1306_DrawRectangle(x1, btn_y1, x2, btn_y2, White);
  ssd1306_SetCursor(txt_x, txt_y);
  ssd1306_WriteString((char *)start_txt, Font_7x10, White);

  ssd1306_UpdateScreen();
}

void GameStart_SetGameTitle(){
  strcpy(home_title,GameSelect_GetCurrentGameName());
  GameMenu_Render();
}

void GameStart_Start(void){
  ssd1306_FillRectangle(x1, btn_y1, x2, btn_y2, White);
  ssd1306_SetCursor(txt_x, txt_y);
  ssd1306_WriteString((char *)start_txt, Font_7x10, Black);
  ssd1306_UpdateScreen();
}