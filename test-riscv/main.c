#include <stdint.h>

#define GPIO_BASE ((volatile uint32_t *)0x10012000)
#define GPIO_OUTE ((volatile uint32_t *)0x10012008)
#define GPIO_OUTX ((volatile uint32_t *)0x10012040) /* Used to toggle the pin */
#define PIN_LED   (21) /* 21, 19, 22 */
#define DELAY     (0x10000)

int main(void)
{
	*GPIO_OUTE |= (1 << PIN_LED);

	while (1) {
		int i = 0;
		while (i < DELAY)
			++i;
		*GPIO_OUTX = (1 << PIN_LED);
	}
	return 0;
}
