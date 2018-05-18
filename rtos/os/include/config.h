/*
 * System configurable parameters.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define PRIO_MAX 0u
#define PRIO_MIN 255u

#define TID_MIN 0u
#define TID_MAX 255u

#define TASK_N_MAX 4

#define STACK_SZ (256u)

#define TICKER_FREQ (500u) /* 2ms */

#define TTY_BR (9600u)

#endif /* CONFIG_H_ */
