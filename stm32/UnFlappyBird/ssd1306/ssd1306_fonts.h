#pragma once

#include "ssd1306.h"

/* Always include the commonly used fonts unless explicitly disabled */
#ifndef SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_6x8
#endif
#ifndef SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_7x10
#endif
#ifndef SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_11x18
#endif

#ifdef SSD1306_INCLUDE_FONT_6x8
extern const SSD1306_Font_t Font_6x8;
extern const SSD1306_Font_t Font_7x10;
extern const SSD1306_Font_t Font_11x18;
extern const SSD1306_Font_t Font_16x26;
extern const SSD1306_Font_t Font_16x24;
extern const SSD1306_Font_t Font_16x15;

#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_16x26
#define SSD1306_INCLUDE_FONT_16x24
#define SSD1306_INCLUDE_FONT_16x15
#endif