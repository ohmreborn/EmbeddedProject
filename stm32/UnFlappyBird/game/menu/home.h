// Home menu header
#ifndef HOME_H
#define HOME_H

#include <stdint.h>
#include <stddef.h>

extern const uint8_t ui_margin; // equal margin from all sides
extern const char title[];
extern const size_t title_len;
extern const uint8_t title_w; // Font_11x18 width approx
extern const uint8_t title_x;
extern const uint8_t title_y;// a bit spacing above title

extern const char scroll_text[];
extern const uint8_t btn_w;
extern const uint8_t btn_h;
extern const uint8_t x1;
extern const uint8_t y1; // keep bottom margin equal to top margin
extern const uint8_t x2;
extern const uint8_t y2;

extern const char start_txt[];
extern const size_t start_len;
extern const uint8_t txt_w; // Font_7x10 width
extern const uint8_t txt_h; // Font_7x10 height
extern const uint8_t txt_x;
extern const uint8_t txt_y;

void Home_Init(void);
void Home_Render(void);

#endif // HOME_H
