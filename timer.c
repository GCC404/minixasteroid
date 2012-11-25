#include <minix/syslib.h>
#include <minix/drivers.h>
#include <time.h>
#include "i8254.h"

static unsigned int intcounter = 0;
static int hook=2;

int timer_set_square(unsigned long timer, unsigned long freq) {

	switch(timer) {
		case 0:
			sys_outb(TIMER_CTRL, TIMER_BIN|TIMER_SQR_WAVE|TIMER_LSB_MSB|TIMER_SEL0);
			sys_outb(TIMER_0, (uint8_t) (TIMER_FREQ/freq));
			sys_outb(TIMER_0, (uint8_t) ((TIMER_FREQ/freq) >> 4));
			break;

		case 1:
			sys_outb(TIMER_CTRL, TIMER_BIN|TIMER_SQR_WAVE|TIMER_LSB_MSB|TIMER_SEL1);
			sys_outb(TIMER_1, (uint8_t) (TIMER_FREQ/freq));
			sys_outb(TIMER_1, (uint8_t) ((TIMER_FREQ/freq) >> 4));
			break;

		case 2:
			sys_outb(TIMER_CTRL, TIMER_BIN|TIMER_SQR_WAVE|TIMER_LSB_MSB|TIMER_SEL2);
			sys_outb(TIMER_2, (uint8_t) (TIMER_FREQ/freq));
			sys_outb(TIMER_2, (uint8_t) ((TIMER_FREQ/freq) >> 4));
			break;
		default:
			printf("No valid timer selected.\n");
			return -1;
	}

	return 0;
}

int timer_subscribe_int(void ) {

	sys_irqsetpolicy(0,IRQ_REENABLE,&hook);
	sys_irqenable(&hook);

	return 1;
}

int timer_unsubscribe_int() {

	sys_irqrmpolicy(&hook);
	sys_irqdisable(&hook);

	return 1;
}

void timer_int_handler() {

	intcounter++;

	if(intcounter%60==0) {
		printf("Count: %d\n",intcounter/60);
	}

}

int timer_test_square(unsigned long freq) {
	
	if(timer_set_square(0, freq)==-1) {
		printf("Failed timer_test_int()\n");
		return -1;
	}

	return 0;
}

int timer_test_int(unsigned long atime) {
	
	int ipc_status,itime=time(NULL);
	message msg;

	timer_subscribe_int();

	while(intcounter < atime * 60) {

		/* Get a request message. */
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			 printf("driver_receive failed with: %d", 55555);
			 continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			 switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & 4) { /* subscribed interrupt */
						timer_int_handler();
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	timer_unsubscribe_int();


	return 1;
}
