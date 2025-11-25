@@ -1,25 +1,56 @@
#include <stdint.h>
#include <stm32f10x.h>



void delay(uint32_t ticks) {
	for (int i=0; i<ticks; i++) {
		__NOP();
	}
    for (int i=0; i<ticks; i++) {
        __NOP();
    }
}

// Главная функция (точка входа программы)
int __attribute((noreturn)) main(void) {
	// Enable clock for AFIO
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	// Enable clock for GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	// Enable PC13 push-pull mode
	GPIOC->CRH &= ~GPIO_CRH_CNF13; //clear cnf bits
	GPIOC->CRH |= GPIO_CRH_MODE13_0; //Max speed = 10Mhz
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
	    GPIOC->ODR |= (1U<<13U); //U -- unsigned suffix (to avoid syntax warnings in IDE)
		delay(1000000);
	    GPIOC->ODR &= ~(1U<<13U);
	    delay(1000000);
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
    }
}
}
