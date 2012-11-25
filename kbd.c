#include <minix/syslib.h>
#include <minix/drivers.h>
#include "kbd.h"

static int hook=2, timerhook=1;
static unsigned int intcounter = 0;

int test_scan(void) {
    driver_receiver();
}

int test_leds(unsigned short n, unsigned short *leds) {

	unsigned short i=0;
	unsigned char cmd;
	unsigned long stat;

	while( i<n ) {

		issue_command(KBD_LED);

		sys_inb(IN_BUF,&stat);

		do { issue_command(KBD_LED); sys_inb(IN_BUF,&stat); } while (stat==ERROR||stat==RESEND);


		cmd=BIT(leds[i]);

		issue_command(cmd);

		sys_inb(IN_BUF,&stat);

		do { issue_command(cmd); sys_inb(IN_BUF,&stat); } while (stat==ERROR||stat==RESEND);

		sleep(1);

		/*timer_test_int(1);*/

		i++;
	}

	return 0;
}

int issue_command(unsigned char cmd) {

	unsigned long data,stat, c=0;

	while(c<10) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */

		/* loop while 8042 input buffer is not empty */
		if( (stat & IBF) == 0 ) {
			sys_outb(WRITE_COMMAND_BYTE, cmd); /* no args command */

			return 0;
		}

		tickdelay(micros_to_ticks(DELAY_US));

		c++;
	}

	printf("Issue_command() timeout.\n");

	return -1;

}


int kbd_subscribe_int(void ) {

	sys_irqsetpolicy(KBD_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&hook);
	sys_irqenable(&hook);

	return 1;
}

int kbd_unsubscribe_int() {

	sys_irqrmpolicy(&hook);
	sys_irqdisable(&hook);

	return 1;
}

unsigned char kbd_int_handler() {

	 unsigned long data,stat;

	 while( 1 ) {
		 sys_inb(STAT_REG, &stat); /* assuming it returns OK */

		 /* loop while 8042 output buffer is empty */
		 if( stat & OBF ) {
			 sys_inb(OUT_BUF, &data); /* assuming it returns OK */

			 if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
				 return (unsigned char)data;
			 else return -1;
		 }

		 tickdelay(micros_to_ticks(DELAY_US));

	 }


}

int driver_receiver() {

	 int ipc_status;
	 message msg;
	 unsigned char scancode;

	 kbd_subscribe_int();

	 do {

		 /* Get a request message. */
		 if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			 printf("driver_receive failed with: %d", 55555);
			 continue;

		 }

		 if (is_ipc_notify(ipc_status)) { /* received notification */
			 switch (_ENDPOINT_P(msg.m_source)) {
			 case HARDWARE: /* hardware interrupt notification */
				 if (msg.NOTIFY_ARG & 4) { /* subscribed interrupt */

					 scancode = kbd_int_handler();

					 if(scancode==-1)
					 {
						 printf("Reading Error from Status Register");
						 return -1;
					 }
					 if(0x80 & scancode)
						 printf("Makecode: %x\n", scancode);
					 else printf("Breakcode: %x\n", scancode);
				 }
				 break;

			 default:
				 break; /* no other notifications expected: do nothing */
			}
		}

	} while(scancode!=ESC_BREAK);

	kbd_unsubscribe_int();

	return 0;
}


/*
	 ACK/RESEND/ERROR - como?
*/
