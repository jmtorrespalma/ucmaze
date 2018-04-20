/*
 * This file should be generated during build time, but so far we just stick to
 * writing it manually.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define PRIO_MAX 0u
#define PRIO_MIN 255u

#define TID_MIN 0u
#define TID_MAX 255u

#define TASK_N_MAX 4

#define STACK_SZ (256u)

#define CPU_FREQ (8000000u) /* 8 MHz */
#define TICKER_FREQ (100u)

#endif /* CONFIG_H_ */
