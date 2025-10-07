#include <libopencm3/stm32/rcc.h>

static void rcc_setup(void)
{
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

int main(void)
{
	while (1) {
	}
}
