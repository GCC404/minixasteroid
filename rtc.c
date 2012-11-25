#include "rtc.h"
#include <minix/drivers.h>

int rtc_hook=4;

/* BEFORE USING
 *
 * 	endpoint_t ep;
	char name[256];
	int priv_f;

	sef_startup();
	/* Fetch our endpoint */
	//sys_whoami(&ep, name, 256, &priv_f);
	/* Enable IO-sensitive operations for ourselves */
	//sys_enable_iop(ep);

int test_conf(void) {
	unsigned long stat;
	char port=REG_A;
/*
 * port=SECONDS;
	while(port<=0x0D) {
		sys_outb(RTC_ADDR_REG,port);
		sys_inb(RTC_DATA_REF,&stat);

		printf("0x%x\n",stat);

		port++;
	}*/

	while(port<=REG_D) {
		choosePort(port);
		readPort(&stat);

		switch(port) {
		case REG_A:
			interpret_A(stat);
			break;
		case REG_B:
			interpret_B(stat);
			break;
		case REG_C:
			interpret_C(stat);
			break;
		case REG_D:
			interpret_D(stat);
			break;
		default:
			printf("Unknown port.\n");
			break;
		}
		port++;
	}
}

void choosePort(char port) {
	sys_outb(RTC_ADDR_REG,port);
}

void interpret_A(unsigned long stat) {
	//unsigned long stat7 = stat&BIT(7);

	printf("UIP: %x \n",(stat&BIT(7))!=0);
	printf("DV2-DV0: %x %x %x \n",(stat&BIT(6))!=0,(stat&BIT(5))!=0,(stat&BIT(4))!=0);
	printf("RS3-RS0: %x %x %x %x\n \n",(stat&BIT(3))!=0,(stat&BIT(2))!=0,(stat&BIT(1))!=0,(stat&BIT(0))!=0);

}

void interpret_B(unsigned long stat) {
	printf("SET: %x \n",(stat&BIT(7))!=0);
	printf("PIE: %x \n",(stat&BIT(6))!=0);
	printf("AIE: %x \n",(stat&BIT(5))!=0);
	printf("UIE: %x \n",(stat&BIT(4))!=0);
	printf("SQWE: %x \n",(stat&BIT(3))!=0);
	printf("DM: %x \n",(stat&BIT(2))!=0);
	printf("24/12: %x \n",(stat&BIT(1))!=0);
	printf("DSE: %x \n \n",(stat&BIT(0))!=0);
}

void interpret_C(unsigned long stat) {
	printf("IRQF: %x \n",(stat&BIT(7))!=0);
	printf("PF: %x \n",(stat&BIT(6))!=0);
	printf("AF: %x \n",(stat&BIT(5))!=0);
	printf("UE: %x \n \n",(stat&BIT(4))!=0);
}

void interpret_D(unsigned long stat) {
	printf("VRT: %x \n \n",(stat&BIT(7))!=0);
}

void readPort(unsigned long *stat) {
	sys_inb(RTC_DATA_REF,stat);
}

void writePort(unsigned long stat) {
	sys_outb(RTC_DATA_REF,stat);
}

int test_date(void) {

	int ipc_status;
	unsigned long stat;
	message msg;
	char port=YEAR;
	unsigned short date[3];
	unsigned short time[3];
	unsigned short i=0;
	unsigned short cont=1;



	choosePort(REG_B);
	readPort(&stat);
	choosePort(REG_B);
	writePort((stat&0x7F)|BIT(4));

	rtc_subscribe_int();

	while(cont) {
		/* Get a request message. */
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", 55555);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & 16) { /* subscribed interrupt */

					choosePort(REG_C);
					readPort(&stat);

					if((stat&BIT(4))!=0)
						printf("Received update interrupt!\n");

					readDate(&date[0],port,&stat);
					port--;
					readDate(&date[1],port,&stat);
					port--;
					readDate(&date[2],port,&stat);
					port-=3;
					readTime(&time[2],port,&stat);
					port-=2;
					readTime(&time[1],port,&stat);
					port-=2;
					readTime(&time[0],port,&stat);

					cont=0;

				}
				break;

			default:
				break; /* no other notifications expected: do nothing */
			}
		}
	}

	rtc_unsubscribe_int();

	printf("Date: %x-%x-%x\n",date[2],date[1],date[0]);
	printf("Time: %x:%x:%x",time[2],time[1],time[0]);

}

void readDate(unsigned short *date, char port, unsigned long *stat) {
	choosePort(port);
	readPort(stat);
	*date=*stat;
}

void readTime(unsigned short *time, char port, unsigned long *stat) {
	choosePort(port);
	readPort(stat);
	*time=*stat;
}

int test_int(unsigned long delta) {

	int ipc_status;
	unsigned long stat;
	message msg;
	char port=YEAR;
	unsigned short time[3];
	unsigned short i=0;
	unsigned short cont=1,readtime=0;

	printf("test_int: Setting alarm within %u s\n",delta);

	choosePort(REG_B);
	readPort(&stat);
	choosePort(REG_B);
	writePort((stat&0x7F)|BIT(4)|BIT(5));

	rtc_subscribe_int();

/*
	if(date[2]+delta>60)
		date[2]=60-delta;
	else */



	while(cont) {
			/* Get a request message. */
			if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
				printf("driver_receive failed with: %d", 55555);
				continue;
			}

			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & 16) { /* subscribed interrupt */

						choosePort(REG_C);
						readPort(&stat);

						if( ((stat&BIT(4))!=0) && (readtime==0)) {

							//Reads time
							readTime(&time[2],HOURS,&stat);
							readTime(&time[1],MINUTES,&stat);
							readTime(&time[0],SECONDS,&stat);


							//Sets the alarm
							choosePort(SECONDS_ALARM);

							if(time[0]+delta>=60) {
								time[0]=time[0]+delta-60;
								if(time[1]+1>=60)
									time[1]=time[1]+delta-60;
								else time[1]=time[1]+1;

								if(time[2]+1>=24)
									time[2]=time[2]+delta-24;
								else time[2]=time[2]+1;

							}else time[0]=time[0]+delta;

							writePort(time[0]);
							choosePort(MINUTES_ALARM);
							writePort(time[1]);
							choosePort(HOURS_ALARM);
							writePort(time[2]);
/*
							printf("Time: %x:%x:%x\n",time[2],time[1],time[0]);
							printf("Alarm seconds: %x\n",time[0]+delta);*/

							readtime=1;
						}
						else {
							printf("test_int: Alarm raised after %u s\n ",delta);
							cont=0;
						}



					}
					break;

				default:
					break; /* no other notifications expected: do nothing */
				}
			}
		}

		rtc_unsubscribe_int();


}

int rtc_subscribe_int(void ) {

	sys_irqsetpolicy(RTC_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&rtc_hook);
	sys_irqenable(&rtc_hook);

	return 1;
}

int rtc_unsubscribe_int() {

	sys_irqrmpolicy(&rtc_hook);
	sys_irqdisable(&rtc_hook);

	return 1;
}
