#include "stm32f4xx.h"

#define TX_PIN (2)
#define RX_PIN (3)
#define APB1_CLOCK (16000000)
#define BAUDRATE (9600)

void gpio_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// clear mode bits
	GPIOA->MODER &= ~((3 << (TX_PIN * 2)) | (3 << (RX_PIN * 2)));
	GPIOA->MODER |= (2 << (TX_PIN * 2)) | (2 << (RX_PIN * 2));

	// configure af7 for usart2
	GPIOA->AFR[0] &= ~((0xF << (TX_PIN * 4)) | (0xF << (RX_PIN * 4)));
	GPIOA->AFR[0] |= (7 << (TX_PIN * 4)) | (7 << (RX_PIN * 4));
}

void usart_init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	USART2->CR1 = USART_CR1_TE | USART_CR1_RE;
	USART2->BRR = APB1_CLOCK / BAUDRATE;
	USART2->CR1 |= USART_CR1_UE;
}

void usart_display(const char *str)
{
	while (*str) {
		while (!(USART2->SR & USART_SR_TXE)) {
		}

		USART2->DR = (uint8_t) *str++;
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
			;
	}
}
