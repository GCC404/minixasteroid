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
//63 asteroid
//30 shot

/* RTC atualizar velocidade (asteroidvel) com alarme -> corrigir isto
 * Colisoes
 * Carregar palete
 * Modular codigo
 * (do)comentar codigo
 * remover extras dos .c
 * page flipping
 * Acabar rato
 * Ataques especiais (?)
 * tiro sincronia
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
	unsigned short times[]={0,0,0}, timesalarm[]={0,0,0};
	int posicaopilhax=1024-110, posicaopilhay=0;

	sef_startup();
	sys_whoami(&ep, name, 256, &priv_f);
	sys_enable_iop(ep);

	unsigned long stat;
	choosePort(REG_B);
	readPort(&stat);
	choosePort(REG_B);
	writePort((stat&0x7F)|BIT(4)|BIT(5));

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

	unsigned int intcounter=1, asteroidperiod=1,
			asteroidvel=1, shotvel=5, shipvel=10,
			shotsperiod=150, shots=4, delta=100;
	unsigned short readtime=0, finished=0;
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

					if(changed==1) {
						finished=draw_spaceship(sprites[0]);

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

					if(intcounter%asteroidperiod==0) {
						for(a=0; a<35; a++) {
							asteroids[a]->y+=asteroidvel;

							if(asteroids[a]->y>768) {
								asteroids[a]->y=-90;
								asteroids[a]->x=rand()%1024;
							}else draw_asteroid(asteroids[a]);

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

				if (msg.NOTIFY_ARG & 16) {

					choosePort(REG_C);
					readPort(&stat);

					if( ((stat&BIT(4))!=0) && (readtime==0)) {

						//Reads timesalarm
						readTime(&timesalarm[2],HOURS,&stat);
						readTime(&timesalarm[1],MINUTES,&stat);
						readTime(&timesalarm[0],SECONDS,&stat);


						//Sets the alarm
						choosePort(SECONDS_ALARM);

						if(timesalarm[0]+delta>=60) {
							timesalarm[0]=timesalarm[0]+delta-60;
							if(timesalarm[1]+1>=60)
								timesalarm[1]=timesalarm[1]+delta-60;
							else timesalarm[1]=timesalarm[1]+1;

							if(timesalarm[2]+1>=24)
								timesalarm[2]=timesalarm[2]+delta-24;
							else timesalarm[2]=timesalarm[2]+1;

						}else timesalarm[0]=timesalarm[0]+delta;

						writePort(timesalarm[0]);
						choosePort(MINUTES_ALARM);
						writePort(timesalarm[1]);
						choosePort(HOURS_ALARM);
						writePort(timesalarm[2]);

						readtime=1;
					}
					else {
						//asteroidvel++;
						//shipvel++;
					}
				}
				break;

			default:
				break;
			}
		}
	} while(scancode!=ESC_BREAK && finished==0);

	//sleep(2);

	rtc_unsubscribe_int();
	mouse_unsubscribe_int();
	timer_unsubscribe_int();
	kbd_unsubscribe_int();
	vg_exit();


	return 0;
}
