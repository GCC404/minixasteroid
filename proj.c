#include <minix/drivers.h>

#include "kbd.h"
#include "mouse.h"
#include "rtc.h"
#include "timer.h"
#include "sprite.h"
#include "video_gr.h"
#include "vbe.h"
#include "proj.h"

//int kbdhook=5, timerhook=2, mousehook=3, rtc_hook=4;
//27 pilhadentro
//17 interior
//25 exterior
//43 nave

/* RTC atualizar velocidade (asteroidvel) com alarme
 * Modular codigo
 * (do)comentar codigo
 * remover extras dos .c
 * page flipping
 * Colisoes
 * Acabar rato
 * Ataques especiais (?)
 */

int main(int argc, char **argv) {

	endpoint_t ep;
	char name[256];
	int priv_f;
	int ipc_status;
	message msg;

	Sprite* pilhas[5];
	Sprite* sprites[1];
	Sprite* asteroids[35];
	Sprite* shotsprt[4];
	Sprite* timesprt[3];
	unsigned int times[]={0,0,0};
	int posicaopilhax=1024-110, posicaopilhay=0;

	sef_startup();
	/* Fetch our endpoint */
	sys_whoami(&ep, name, 256, &priv_f);
	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(ep);
	rtc_subscribe_int();
	mouse_subscribe_int();
	timer_subscribe_int();
	kbd_subscribe_int();

	vg_init(0x105);
	vg_fill(BACKGROUND);
	vg_memtobuffer();

	pilhas[0]=create_sprite(pilha,posicaopilhax,posicaopilhay);
	draw_sprite2(pilhas[0]);
	pilhas[1]=create_sprite(pilhadentro,posicaopilhax+83,posicaopilhay+12);
	draw_sprite2(pilhas[1]);
	pilhas[2]=create_sprite(pilhadentro,posicaopilhax+63,posicaopilhay+12);
	draw_sprite2(pilhas[2]);
	pilhas[3]=create_sprite(pilhadentro,posicaopilhax+43,posicaopilhay+12);
	draw_sprite2(pilhas[3]);
	pilhas[4]=create_sprite(pilhadentro,posicaopilhax+23,posicaopilhay+12);
	draw_sprite2(pilhas[4]);

	sprites[0]=create_sprite(spaceship,(1024/2)-20,768-100);
	draw_sprite(sprites[0]);

	timesprt[0]=create_sprite(digits[0],140,0);
	draw_sprite(timesprt[0]);
	timesprt[1]=create_sprite(digits[0],70,0);
	draw_sprite(timesprt[1]);
	timesprt[2]=create_sprite(digits[0],0,0);
	draw_sprite(timesprt[2]);

	int i;
	srand(time(NULL));

	for(i=0; i<5; i++) {
		shotsprt[i]=create_sprite(shot,0,0);
	}

	for(i=0; i<35; i++) {
		asteroids[i]=create_sprite(asteroid,rand()%1024,-90);
		asteroids[i]=create_sprite(asteroid,rand()%1024, -90*((i%9)+1) );
	}

	vg_buffertomem();
	sleep(1);

	unsigned int intcounter=1, asteroidperiod=1, asteroidvel=1, shotvel=5, shipvel=10, shotsperiod=150, shots=4;
	unsigned char scancode;
	unsigned int changed=0;

	do {
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", 55555);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:
				if (msg.NOTIFY_ARG & 4) {

					int a;
					if(intcounter%asteroidperiod==0) {
						for(a=0; a<35; a++) {
							asteroids[a]->y+=asteroidvel;

							if(asteroids[a]->y>768) {
								asteroids[a]->y=-90;
								asteroids[a]->x=rand()%1024;
							}else draw_sprite(asteroids[a]);

						}

						for(a=4-shots; a>0; a--) {
							shotsprt[a-1]->y-=shotvel;
						}

						changed=1;
					}

					if(intcounter%shotsperiod==0 && shots<5) {
						if(shots<4)
							shots++;

						changed=1;
					}

					if(intcounter%120==0) {

						if(times[0]==9) {
							times[0]=0;

							if(times[1]==9) {
								times[1]=0;

								if(times[2]==9)
									times[2]=0;
								else times[2]++;

							}else times[1]++;

						}else times[0]++;

						for(a=0; a<3; a++) {
							timesprt[a]=create_sprite(digits[times[a]],timesprt[a]->x,timesprt[a]->y);
						}

						changed=1;
					}

					if(changed==1) {
						draw_sprite(sprites[0]);
						for(a=4-shots; a>0; a--) {
							draw_sprite(shotsprt[a-1]);
						}
						for(a=0; a<3; a++) {
							draw_sprite2(timesprt[a]);
						}
						for(a=0; a<=shots; a++) {
							draw_sprite2(pilhas[a]);
						}
						vg_buffertomem();
						vg_fill_buffer(BACKGROUND);
						changed=0;
					}

					intcounter++;
				}

				if (msg.NOTIFY_ARG & 32) {

					scancode = kbd_int_handler();

					if(scancode==-1)
					{
						printf("Reading Error from Status Register");
						return -1;
					}


					if(scancode==A_BREAK) {
						sprites[0]->x-=shipvel;
						changed=1;
					}

					if(scancode==D_BREAK) {
						sprites[0]->x+=shipvel;
						changed=1;
					}

					if(scancode==SPACE_BREAK && shots>0) {

						shotsprt[4-shots]->x=sprites[0]->x;
						shotsprt[4-shots]->y=sprites[0]->y;

						shots--;
						changed=1;
					}

				}
				break;

			default:
				break;
			}
		} else {
		}
	} while(scancode!=ESC_BREAK);

	/*
	Sprite* rato[2];
	int posx_inicial=20;
	int posy_inicial=23;
	//rato[0]=create_sprite(spaceship,posx_inicial,posy_inicial);
	//draw_sprite(rato[0]);
	rato[1]=create_sprite(asteroid,posx_inicial,posy_inicial);
		draw_sprite(rato[1]);*/
/*
	unsigned char packet[3];
	unsigned char desl_x=0, desl_y=0, novo_x=0, novo_y=0;
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

	if(stat!=ACK) {
		mouse_unsubscribe_int();
		printf("Enabling DataPackets failed, retrying..\n");

		if(errorc>5)
			return -1;

		errorc++;
		test_packet();
	}

	while(1) {

		/* Get a request message. */
		/*if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", 55555);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			/*switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				/*if (msg.NOTIFY_ARG & 8) { /* subscribed interrupt */

					/*sys_inb(IN_BUF,&stat);

					if( !(stat&BIT(3)) && (count==0)) {
						continue;
					}

					packet[count]=stat;
					count++;

					if(count>2) {
						desl_x=packet[1];
						desl_y=packet[2];

					}

					if(count==2 && (desl_x!=0 || desl_y!=0)) {
						count=0;
						//erase_sprite(rato[0], BACKGROUND);

						if(desl_x!=0) {
							if(desl_x>127) {
								posx_inicial+=desl_x-256;
							}
							else posx_inicial=posx_inicial+desl_x;
						}
						if(desl_y!=0) {
							if(desl_y>127) {
								posy_inicial-=desl_y-256;
							}
							else posy_inicial-=desl_y;
						}

						//rato[0]->x=posx_inicial;
						//rato[0]->y=posy_inicial;
						rato[1]=create_sprite(spaceship,posx_inicial,posy_inicial);
						draw_sprite(rato[1]);
						//draw_sprite(rato[0]);
						vg_buffertomem();
					}
				}

				break;
			default:
				break; /* no other notifications expected: do nothing */
//			}
	/*	} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		/*}
	}
	*/
	rtc_unsubscribe_int();
	mouse_unsubscribe_int();
	timer_unsubscribe_int();
	kbd_unsubscribe_int();
	vg_exit(); /* Return to text mode */


	return 0;
}
