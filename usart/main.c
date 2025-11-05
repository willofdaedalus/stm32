#include "stm32f4xx.h"

#define TX_PORT (2)
#define RX_PORT (3)

void gpio_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// configure pa2 and pa3 as alternate function ports
	GPIOA->MODER &= ~(3 << (TX_PORT * 2)) | ~(3 << (RX_PORT * 2));
	GPIOA->MODER |= (2 << (TX_PORT * 2)) | (2 << (RX_PORT * 2));

	// enable the alternate function number (AF7)
	GPIOA->AFR[0] &= ~((0xf << (4 * 2)) | (0xf << (4 * 3)));
	GPIOA->AFR[0] |= (7 << (TX_PORT * 4)) | (7 << (RX_PORT * 4));
}

void usart_init(void)
{
	// enable usart2 clock interface
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	USART2->BRR = 0x0683; // 9600 baud @ 16 MHz
	// enable usart UE
	USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void usart_display(const char *str)
{
	while (*str) {
		while (!(USART2->SR & USART_SR_TXE))
			;

		USART2->DR = *str++;
	}
}

int main(void)
{
	const char *str = "hello world from usart running on the stm32!\r\n";

	gpio_init();
	usart_init();

	while (1) {
		usart_display(str);
		for (volatile int i = 0; i < 1000000; i++)
			; // small delay
	}
}
