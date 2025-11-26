@@ -0,0 +1,17 @@
#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

// Размеры дисплея
#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64

// Прототипы функций
void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_Update(void);
void SSD1306_DrawPixel(int16_t x, int16_t y, uint8_t color);
void SSD1306_TestPattern(void);

#endif
