#include "proj.h"

//int kbdhook=5, timerhook=2, mousehook=3, rtc_hook=4;
//27 pilhadentro
//17 interior
//25 exterior
//43 nave
//63 asteroid
//30 shot

/* RTC atualizar velocidade (asteroidvel) com alarme -> corrigir isto -> acho que o lab nao esta bem
 * Destruir Asteroides
 * Carregar palete
 *
 * Modular codigo -> tirar do principal e meter nos outros (ver ponto abaixo)
 * Partilhar variaveis globais com modulos
 * remover extras dos .c
 *
 * (do)comentar codigo
 *
 * Pormenores:
 * Page flipping
 * Ataques especiais (?)
 * Tiro sincronia - provavelmente nao e preciso
 */

int main(int argc, char **argv) {

	//Used for peripheric functioning (including interrupts)
	endpoint_t ep;
	char name[256];
	int priv_f;
	int ipc_status;
	message msg;

	sef_startup();
	sys_whoami(&ep, name, 256, &priv_f);
	sys_enable_iop(ep);

	//Subscribing RTC interrupts
	unsigned long stat;
	choosePort(REG_B);
	readPort(&stat);
	choosePort(REG_B);
	writePort((stat&0x7F)|BIT(4)|BIT(5));
	rtc_subscribe_int();

	//Enable Mouse
	sys_outb(KBC_CMD_REG,ENABLE_MOUSE);

	//Enable Sending Data Packets
	sys_outb(KBC_CMD_REG,WRITE_BYTE);
	sys_inb(IN_BUF,&stat);
	sys_outb(OUT_BUF,ENABLE_DATA);
	sys_inb(IN_BUF, &stat);
	mouse_subscribe_int();

	timer_subscribe_int();
	kbd_subscribe_int();

	vg_init(0x105);
	vg_fill(BACKGROUND);
	vg_memtobuffer();

	//Initializing sprites, using randomness on asteroids
	batteries[0]=create_sprite(battery,BATTERYXPOS,BATTERYYPOS);
	draw_sprite(batteries[0]);
	batteries[1]=create_sprite(batteryinside,BATTERYXPOS+83,BATTERYYPOS+12);
	draw_sprite(batteries[1]);
	batteries[2]=create_sprite(batteryinside,BATTERYXPOS+63,BATTERYYPOS+12);
	draw_sprite(batteries[2]);
	batteries[3]=create_sprite(batteryinside,BATTERYXPOS+43,BATTERYYPOS+12);
	draw_sprite(batteries[3]);
	batteries[4]=create_sprite(batteryinside,BATTERYXPOS+23,BATTERYYPOS+12);
	draw_sprite(batteries[4]);

	spaceships[0]=create_sprite(spaceship,(H_RES/2)-20,V_RES-100);
	draw_spaceship(spaceships[0]);

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
	for(i=0; i<NUM_ASTEROIDS; i++) {
		asteroids[i]=create_sprite(asteroid,rand()%1024,-90);
		asteroids[i]=create_sprite(asteroid,rand()%1024, -90*((i%9)+1) );
	}

	rato[0]=create_sprite(spaceship,posx_inicial,posy_inicial);
	draw_sprite(rato[0]);

	vg_buffertomem();



	do {
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", 55555);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:
				if (msg.NOTIFY_ARG & 4) {

					timer_int_handler();
					intcounter++;
				}

				if (msg.NOTIFY_ARG & 32) {

					while(1) {
						if(kbd_int_handler()!=-1)
							break;
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

						if(timesalarm[0]+DELTA>=60) {
							timesalarm[0]=timesalarm[0]+DELTA-60;
							if(timesalarm[1]+1>=60)
								timesalarm[1]=timesalarm[1]+DELTA-60;
							else timesalarm[1]=timesalarm[1]+1;

							if(timesalarm[2]+1>=24)
								timesalarm[2]=timesalarm[2]+DELTA-24;
							else timesalarm[2]=timesalarm[2]+1;

						}else timesalarm[0]=timesalarm[0]+DELTA;

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
				if (msg.NOTIFY_ARG & 8) {
					mouse_int_handler();
				}
				break;

			default:
				break;
			}
		}
	} while(finished==0);

	rtc_unsubscribe_int();
	mouse_unsubscribe_int();
	timer_unsubscribe_int();
	kbd_unsubscribe_int();
	vg_exit();

	return 0;
}

void timer_int_handler() {
	int a;

	if(changed==1) {
		finished=draw_spaceship(spaceships[0]);

		for(a=4-shots; a>0; a--) {
			draw_sprite(shotsprt[a-1]);
		}
		for(a=0; a<3; a++) {
			draw_sprite(timesprt[a]);
		}
		for(a=0; a<=shots; a++) {
			draw_sprite(batteries[a]);
		}
		vg_buffertomem();
		vg_fill_buffer(BACKGROUND);
		changed=0;
	}

	if(intcounter%ASTEROID_PERIOD==0) {
		for(a=0; a<NUM_ASTEROIDS; a++) {
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

	if(intcounter%SHOT_PERIOD==0 && shots<5) {
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
}

int kbd_int_handler() {
	//Will store C@KBD responses
	unsigned char scancode;

	scancode = kbd_readscancode();

	if(scancode==-1) {
		printf("Reading Error from Status Register");
		return -1;
	}
	if(scancode==A_BREAK) {
		spaceships[0]->x-=shipvel;
		changed=1;
	}
	if(scancode==D_BREAK) {
		spaceships[0]->x+=shipvel;
		changed=1;
	}
	if(scancode==SPACE_BREAK && shots>0) {

		shotsprt[4-shots]->x=spaceships[0]->x;
		shotsprt[4-shots]->y=spaceships[0]->y;

		shots--;
		changed=1;
	}
	if(scancode==ESC_BREAK)
		finished=1;
}

void mouse_int_handler() {

	sys_inb(IN_BUF,&stat);

	int confirma=0;

	if( !(stat&BIT(3)) && (count==0))
		confirma=1;

	packet[count]=stat;
	count++;

	if(count==3 && confirma==0) {
		count=0;

		if(packet[1]!=0 || packet[2]!=0) {
			changed=1;
			erase_sprite(rato[0], BACKGROUND);

			if(packet[1]!=0) {
				if(packet[1]<128)
					posx_inicial+=packet[1];
				else posx_inicial+=packet[1]-256;

				rato[0]->x=posx_inicial;
			}
			if(packet[2]!=0) {
				if(packet[2]<128)
					posy_inicial-=packet[2];
				else posy_inicial-=packet[2]-256;

				rato[0]->y=posy_inicial;
			}
			draw_sprite(rato[0]);
		}
	}
}
