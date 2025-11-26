@@ -0,0 +1,164 @@
#include "ssd1306.h"
#include "stm32f10x.h"

// Буфер дисплея в оперативной памяти
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// Приватные функции
static void display_cmd(uint8_t cmd);
static void display_data(uint8_t data);

// Простая функция задержки
static void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 8000; i++) {
        __asm__("nop");
    }
}

// Отправка команды дисплею
static void display_cmd(uint8_t cmd) {
    // CS = 0, DC = 0
    GPIOA->BSRR = GPIO_BSRR_BR4;  // CS = 0
    GPIOA->BSRR = GPIO_BSRR_BR1;  // DC = 0 (команда)

    // Ждём готовности SPI и отправляем данные
    while(!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = cmd;
    while(SPI1->SR & SPI_SR_BSY);

    GPIOA->BSRR = GPIO_BSRR_BS4;  // CS = 1
}

// Отправка данных дисплею
static void display_data(uint8_t data) {
    // CS = 0, DC = 1
    GPIOA->BSRR = GPIO_BSRR_BR4;  // CS = 0
    GPIOA->BSRR = GPIO_BSRR_BS1;  // DC = 1 (данные)

    // Ждём готовности SPI и отправляем данные
    while(!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;
    while(SPI1->SR & SPI_SR_BSY);

    GPIOA->BSRR = GPIO_BSRR_BS4;  // CS = 1
}

// Инициализация дисплея
void SSD1306_Init(void) {
    // Аппаратный сброс
    GPIOA->BSRR = GPIO_BSRR_BR2;  // RES = 0
    delay_ms(10);
    GPIOA->BSRR = GPIO_BSRR_BS2;  // RES = 1
    delay_ms(10);

    // Последовательность команд инициализации
    display_cmd(0xAE); // Display OFF

    display_cmd(0x20); // Set Memory Addressing Mode
    display_cmd(0x00); // Horizontal addressing mode

    display_cmd(0x21); // Set column address
    display_cmd(0x00); // Start column = 0
    display_cmd(0x7F); // End column = 127

    display_cmd(0x22); // Set page address
    display_cmd(0x00); // Start page = 0
    display_cmd(0x07); // End page = 7

    display_cmd(0x40); // Set start line

    display_cmd(0xA1); // Set segment re-map
    display_cmd(0xC8); // Set COM Output Scan Direction

    display_cmd(0xA8); // Set multiplex ratio
    display_cmd(0x3F); // 1/64 duty

    display_cmd(0xA4); // Entire display ON

    display_cmd(0xD3); // Set display offset
    display_cmd(0x00); // No offset

    display_cmd(0xD5); // Set display clock divide ratio
    display_cmd(0xF0); // Increase frequency

    display_cmd(0xD9); // Set pre-charge period
    display_cmd(0x22); // Default

    display_cmd(0xDA); // Set com pins hardware configuration
    display_cmd(0x12); // Alternative COM pin configuration

    display_cmd(0xDB); // Set vcomh
    display_cmd(0x20); // 0.77xVcc

    display_cmd(0x8D); // Set DC-DC enable
    display_cmd(0x14); // Enable charge pump

    display_cmd(0xAF); // Display ON

    SSD1306_Clear();
    SSD1306_Update();
}

// Очистка дисплея
void SSD1306_Clear(void) {
    for(uint16_t i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Buffer[i] = 0x00;
    }
}

// Обновление дисплея (отправка всего буфера)
void SSD1306_Update(void) {
    for(uint8_t page = 0; page < 8; page++) {
        display_cmd(0xB0 + page); // Set page address
        display_cmd(0x00);        // Set lower column address
        display_cmd(0x10);        // Set higher column address

        // Отправляем всю строку (128 байт)
        for(uint8_t col = 0; col < 128; col++) {
            display_data(SSD1306_Buffer[col + page * 128]);
        }
    }
}

// Установка пикселя
void SSD1306_DrawPixel(int16_t x, int16_t y, uint8_t color) {
    if(x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) return;

    uint16_t index = x + (y / 8) * SSD1306_WIDTH;

    if(color) {
        SSD1306_Buffer[index] |= (1 << (y % 8));
    } else {
        SSD1306_Buffer[index] &= ~(1 << (y % 8));
    }
}

// Генерация тестового изображения
void SSD1306_TestPattern(void) {
    SSD1306_Clear();

    // 1. Заполнение шахматной доской
    for(uint8_t y = 0; y < SSD1306_HEIGHT; y++) {
        for(uint8_t x = 0; x < SSD1306_WIDTH; x++) {
            if((x / 8 + y / 8) % 2 == 0) {
                SSD1306_DrawPixel(x, y, 1);
            }
        }
    }

    // 2. Горизонтальные линии
    for(uint8_t x = 0; x < SSD1306_WIDTH; x++) {
        SSD1306_DrawPixel(x, 0, 1);
        SSD1306_DrawPixel(x, SSD1306_HEIGHT-1, 1);
        SSD1306_DrawPixel(x, SSD1306_HEIGHT/2, 1);
    }

    // 3. Вертикальные линии
    for(uint8_t y = 0; y < SSD1306_HEIGHT; y++) {
        SSD1306_DrawPixel(0, y, 1);
        SSD1306_DrawPixel(SSD1306_WIDTH-1, y, 1);
        SSD1306_DrawPixel(SSD1306_WIDTH/2, y, 1);
    }

    SSD1306_Update();
}
