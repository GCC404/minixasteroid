#ifndef __TIMER_H
#define __TIMER_H

static int timerhook=2;

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int(void );

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();

/**
 * @brief Tests programming timer in square wave mode
 *
 * Programs Timer 0 to generate square mode with input frequency
 *
 * @param freq Frequency of square wave to generate
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_test_square(unsigned long freq);

#endif /* __TIMER_H */
