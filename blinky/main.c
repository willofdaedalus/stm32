#include "stm32f4xx.h"

int main(void)
{
	volatile int i;

	// enable clock for GPIOA and GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

	// clear the bits pre-existing
	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	// enable general output mode
	GPIOA->MODER |= GPIO_MODER_MODER5_0;

	while (1) {
		// works but needs button debouncing
		if (!(GPIOC->IDR & GPIO_IDR_ID13)) {
			// crude debounce delay
			for (i = 0; i < 100000; i++)
				;

			// confirm still pressed
			if (!(GPIOC->IDR & GPIO_IDR_ID13))
				GPIOA->ODR ^= GPIO_ODR_OD5;

			while (!(GPIOC->IDR & GPIO_IDR_ID13))
				;
		}
	}
}
