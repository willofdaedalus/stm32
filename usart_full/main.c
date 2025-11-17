#include "stm32f4xx.h"
#include <string.h>

#define TX_PIN 2
#define RX_PIN 3
#define BUFSZ 32
#define APB_CLOCK 16000000
#define SECS 2000
#define BAUDRATE 9600

static volatile uint8_t buf[BUFSZ];
static volatile uint8_t counter;
static volatile uint8_t send;

void usart_send(const char *str);

void USART2_IRQHandler(void)
{
	if (USART2->SR & USART_SR_RXNE) {
		uint8_t c = USART2->DR;

		if ((c == 0x08 || c == 0x7F) && counter > 0) {
			counter--;
		} else if (c == '\n' || c == '\r') {
			send = 1;
		} else if (counter < BUFSZ - 1) {
			buf[counter++] = c;
			USART2->DR = c;
		}
	}
}

void init_gpio(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// enable pins pa2 and pa3
	GPIOA->MODER &= ~((3 << (TX_PIN * 2)) | (3 << (RX_PIN * 2)));
	GPIOA->MODER |= ((2 << (TX_PIN * 2)) | (2 << (RX_PIN * 2)));

	GPIOA->AFR[0] &= ~((0xf << (TX_PIN * 4)) | (0xf << (RX_PIN * 4)));
	GPIOA->AFR[0] |= ((7 << (TX_PIN * 4)) | (7 << (RX_PIN * 4)));
}

void init_usart(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE;
	USART2->CR1 |= USART_CR1_RXNEIE;

	USART2->BRR = APB_CLOCK / BAUDRATE;

	NVIC_EnableIRQ(USART2_IRQn);

	// enable usart
	USART2->CR1 |= USART_CR1_UE;
}

void usart_send(const char *str)
{
	while (*str) {
		while (!(USART2->SR & USART_SR_TXE)) {
		}

		USART2->DR = (uint8_t) *str++;
	}
}

void init_tim2(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	TIM2->CNT = 0;
	TIM2->PSC = (APB_CLOCK / 1000) - 1;
	TIM2->ARR = SECS - 1;

	TIM2->DIER |= TIM_DIER_UIE;

	NVIC_EnableIRQ(TIM2_IRQn);

	TIM2->CR1 |= TIM_CR1_CEN;
}

int main(void)
{
	init_gpio();
	init_usart();

	usart_send("STM32 echo test. Type something:\r\n");

	while (1) {
		if (send) {
			send = 0;
			if (counter > 0) {
				buf[counter] = 0;
				usart_send((const char *) buf);
				usart_send("\r\n");
				counter = 0;
				memset((void *) buf, 0, sizeof(buf));
			}
		}
	}
}
