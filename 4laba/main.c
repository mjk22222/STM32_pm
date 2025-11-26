@@ -1,56 +1,50 @@
#include <stdint.h>
#include <stm32f10x.h>
#include "ssd1306.h"
#include "stm32f10x.h"

void Peripherals_Init(void) {
    // Включение тактирования
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_SPI1EN;

    // Настройка SPI1 пинов: PA5=SCK, PA7=MOSI
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5 |
                    GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5 |  // PA5: Alternate Push-Pull, 50MHz
                   GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7);  // PA7: Alternate Push-Pull, 50MHz

    // Настройка управляющих пинов: PA1=DC, PA4=CS, PA2=RES (Output, Push-Pull, 2MHz)
    GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1 |
                    GPIO_CRL_CNF4 | GPIO_CRL_MODE4 |
                    GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
    GPIOA->CRL |= (GPIO_CRL_MODE1_0 |   // PA1: Output, 2MHz
                   GPIO_CRL_MODE4_0 |   // PA4: Output, 2MHz
                   GPIO_CRL_MODE2_0);   // PA2: Output, 2MHz

    // Установка начальных состояний
    GPIOA->BSRR = GPIO_BSRR_BS4;  // CS = 1
    GPIOA->BSRR = GPIO_BSRR_BS2;  // RES = 1

    // Настройка SPI1
    SPI1->CR1 = SPI_CR1_CPOL | SPI_CR1_CPHA |  // Полярность и фаза
                SPI_CR1_MSTR |                 // Режим мастера
                SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 | // Baudrate = fPCLK/256
                SPI_CR1_SSM | SPI_CR1_SSI;     // Программное управление CS

    SPI1->CR1 |= SPI_CR1_SPE; // Включение SPI
}

int main(void) {
    // SystemInit() вызывается автоматически до входа в main()

    // Инициализация нашей периферии
    Peripherals_Init();

void delay(uint32_t ticks) {
    for (int i=0; i<ticks; i++) {
        __NOP();
    }
}
    // Инициализация дисплея
    SSD1306_Init();

    // Отображение тестового изображения
    SSD1306_TestPattern();

// Главная функция (точка входа программы)
int __attribute((noreturn)) main(void) {
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;  // Тактирование альтернативных функций
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;  // Тактирование порта A
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;  // Тактирование порта C

    GPIOC->CRH &= ~GPIO_CRH_CNF13;       // Очищаем биты конфигурации (CNF) для пина 13
    GPIOC->CRH |= GPIO_CRH_MODE13_0;     // Устанавливаем MODE = 01 (выход, 10MHz)

    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0); // Очищаем настройки для PA0
    GPIOA->CRL |= GPIO_CRL_CNF0_1;       // CNF = 10 (вход с pull-up/pull-down)

    GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1); // Очищаем настройки для PA1
    GPIOA->CRL |= GPIO_CRL_CNF1_1;       // CNF = 10 (вход с pull-up/pull-down)

    GPIOA->ODR |= (1U << 0);             //` Pull-up для PA0
    GPIOA->ODR |= (1U << 1U);            // Pull-up для PA1

    uint32_t delays[] = {
        2000000000,  // Очень медленно
        200000000,   // Медленно
        40000000,    // Средне
        8000000      // Быстро
    };

    uint8_t mode = 0;  // Текущий режим (индекс в массиве delays)

    while (1) {
        if (!(GPIOA->IDR & (1U << 0))) {  // Если на PA0 низкий уровень (кнопка нажата)
            if (mode < sizeof(delays) / sizeof(delays[0]) - 1) {  // Если не достигнут максимум
                mode++;  // Увеличиваем скорость
            }
        }

        // ПРОВЕРКА КНОПКИ УМЕНЬШЕНИЯ СКОРОСТИ (PA1)
        if (!(GPIOA->IDR & (1U << 1))) {  // Если на PA1 низкий уровень (кнопка нажата)
            if (mode > 1) {  // Если не достигнут минимум (кроме самого медленного)
                mode--;  // Уменьшаем скорость (переходим к большей задержке)
            }
        }

        GPIOC->ODR ^= (1U << 13U);
        delay(delays[mode]);
    while(1) {
        // Основной цикл
    }
}
