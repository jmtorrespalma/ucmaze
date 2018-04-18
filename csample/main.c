#include <stdint.h>

/* Clock defines */
#define RCC_BASE   ((volatile uint32_t *)0x40021000)
#define RCC_AHBENR ((volatile uint32_t *)0x40021014)
#define IOPCEN     ((uint32_t)(1 << 19))

/* GPIO defines */
#define GPIO_BASE  ((volatile uint32_t *)0x48000800)
#define GPIO_ODR   ((volatile uint32_t *)0x48000814)
#define PIN        8
#define DELAY      0x100000

int main(void)
{
	uint32_t led_status = 0;

	/* Enable clock */
	*RCC_AHBENR = IOPCEN;

	/* Set pin as output */
	*GPIO_BASE |= (1 << (PIN * 2));

	while (1) {
		*GPIO_ODR = led_status << PIN;
		led_status = !led_status;
		for (int i = 0; i < DELAY; ++i);
	}
	return 0;
}
