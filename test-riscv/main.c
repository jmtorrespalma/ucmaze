#include <stdint.h>

#define GPIO_BASE ((volatile uint32_t *)0x10012000)
#define GPIO_INE  ((volatile uint32_t *)0x10012004)
#define GPIO_OUTE ((volatile uint32_t *)0x10012008)
#define GPIO_OUTV ((volatile uint32_t *)0x1001200c)
#define GPIO_OUTX ((volatile uint32_t *)0x10012040) /* Used to toggle the pin */
#define PIN_LED   (22) /* 21, 19, 22 */
#define DELAY     (0x100000)

int main(void)
{
	int i, pin_value = 0;

	*GPIO_INE = 0;
	*GPIO_OUTE |= (1 << PIN_LED);

	while (1) {
		*GPIO_OUTV = pin_value << PIN_LED;
		i = 0;
		while (i < DELAY)
			++i;
		pin_value ^= 1;
		//*GPIO_OUTX = (1 << PIN_LED);
	}
	return 0;
}
