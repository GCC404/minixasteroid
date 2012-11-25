#include <minix/syslib.h>
#include <minix/drivers.h>
#include "mouse.h"

static int mousehook=3, hook=2;
static unsigned int intcounter = 0, errorc=0;

int test_packet() {
    
	unsigned char packet[3];
	unsigned short count=0;
	unsigned long stat;
	int ipc_status, buttonterminate=0;
	message msg;

	
	

	//Enable Mouse
	sys_outb(KBC_CMD_REG,ENABLE_MOUSE);

	//Enable Sending Data Packets
	sys_outb(KBC_CMD_REG,WRITE_BYTE);
	sys_inb(IN_BUF,&stat);
	sys_outb(OUT_BUF,ENABLE_DATA);
	sys_inb(IN_BUF, &stat);


	mouse_subscribe_int();

	if(stat!=ACK)
	{
		mouse_unsubscribe_int();
		printf("Enabling DataPackets failed, retrying..\n");

		if(errorc>5)
			return -1;

		errorc++;
		test_packet();
	}
	//tickdelay(micros_to_ticks(WAIT_TIME));


	while(1) {


		/* Get a request message. */
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			 printf("driver_receive failed with: %d", 55555);
			 continue;
		}

		//printf("AAA\n");

		if (is_ipc_notify(ipc_status)) { /* received notification */
			 switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & 8) { /* subscribed interrupt */
						
						sys_inb(IN_BUF,&stat);
						//printf("BBB\n");

						if( !(stat&BIT(3)) && (count==0))
						{
							//printf("CCC\n");
							break;
						}
						
						packet[count]=stat;
						count++;
						

						if(count>2)
						{
							count = 0;
							printf("B1=0x%x B2=0x%x B3=0x%x LB=%x MB=%x RB=%x XOV=%x YOV=%x X=%x Y=%x\n",
								packet[0], packet[1], packet[2],
								LB&packet[0], (MB&packet[0])>>2, (RB&packet[0])>>1,
								(X_OVF&packet[0])>>6, (Y_OVF&packet[0])>>7, (X&packet[0])>>4, (Y&packet[0])>>5);



							if(handle_mouse(packet[0])==-1)
								return 1;

							break;
						}
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	
	mouse_unsubscribe_int();
	

	return 1;
}

typedef enum{A,B,C} state_qwe;
int handle_mouse(unsigned char mouse_evt) {
	static state_qwe st = A;
	// initial state; keep state
	switch (st) {
	case A:
		if( mouse_evt&LB )
			st = B;
		break;
	case B:
		if( mouse_evt&RB ) {
			st = C;
			return -1;
		}
			else
				st = A;
			break;
	default:
		break;
	}

	return 1;
}

int mouse_subscribe_int(void ) {

	sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&mousehook);
	sys_irqenable(&mousehook);

	return 1;
}

int mouse_unsubscribe_int() {

	sys_irqrmpolicy(&mousehook);
	sys_irqdisable(&mousehook);

	return 1;
}

int test_asynch(unsigned short duration) {

	unsigned char packet[3];
	unsigned short count=0,counter=0, time=0;
	unsigned long stat;
	int ipc_status, buttonterminate=0;
	message msg;


	mouse_subscribe_int();

	while(counter<30||time<duration*60) {

		/* Get a request message. */
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", 55555);
			continue;
		}


		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & 8) { /* subscribed interrupt */

					sys_inb(IN_BUF,&stat);
											//printf("BBB\n");

											if( !(stat&BIT(3)) && (count==0))
											{
												//printf("CCC\n");
												break;
											}

											packet[count]=stat;
											count++;


											if(count>2)
											{
												count = 0;
												printf("B1=0x%x B2=0x%x B3=0x%x LB=%x MB=%x RB=%x XOV=%x YOV=%x X=%x Y=%x\n",
													packet[0], packet[1], packet[2],
													LB&packet[0], (MB&packet[0])>>2, (RB&packet[0])>>1,
													(X_OVF&packet[0])>>6, (Y_OVF&packet[0])>>7, (X&packet[0])>>4, (Y&packet[0])>>5);



												if(handle_mouse(packet[0])==-1)
													return 1;

												break;
											}

				}

				if (msg.NOTIFY_ARG & 4) {

					intcounter++;

					time=intcounter%60;

				}

				break;

			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	mouse_unsubscribe_int();

	return 1;
	
}
/*
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




int test_config(void) {
    /* To be completed ... */
//}
