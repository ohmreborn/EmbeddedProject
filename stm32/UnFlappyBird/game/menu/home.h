// Home menu header
#ifndef HOME_H
#define HOME_H

#include <stdint.h>

void Home_Init(void);
void Home_Render(void);

// update title displayed on home screen
void Home_SetTitle(const char *title);

#endif // HOME_H
