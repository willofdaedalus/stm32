#include "stm32f4xx.h"

void usart_init(void)
{
	// enable usart2 clock interface
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// enable usart UE
	USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
	USART2->BRR = 9600;
}

void usart_display(const char *str)
{
	while (str) {
		if (USART2->CR1 & USART_SR_TC) {
			USART2->DR = *str++;
		}
	}
}

int main(void)
{
	const char *str = "hello world from usart running on the stm32!\n\n";

	usart_init();

	while (1) {
		usart_display(str);
	}
}
