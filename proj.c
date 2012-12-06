#include <minix/drivers.h>

#include "kbd.h"
#include "mouse.h"
#include "rtc.h"
#include "timer.h"
#include "sprite.h"
#include "video_gr.h"
#include "vbe.h"
#include "proj.h"

int main(int argc, char **argv) {

	endpoint_t ep;
	char name[256];
	int priv_f;
	int ipc_status;
	message msg;

	Sprite* pilhas[5];
	Sprite* sprites[1];
	Sprite* asteroids[5];
	int posicaopilhax=1024-110, posicaopilhay=0;

	sef_startup();
	/* Fetch our endpoint */
	sys_whoami(&ep, name, 256, &priv_f);
	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(ep);
	mouse_subscribe_int();
	timer_subscribe_int();

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

	sprites[0]=create_sprite(spaceship,768-100,(1024/2)-20);
	draw_sprite(sprites[0]);

	vg_buffertomem();
	sleep(1);

	int i;
	srand(time(NULL));

	for(i=0; i<5; i++) {
		asteroids[i]=create_sprite(spaceship,rand()%1024,0);
		draw_sprite(asteroids[i]);
	}

	vg_buffertomem();

	unsigned int intcounter = 0, frequency=60;

	while(intcounter < 5 * 60) {
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed with: %d", 55555);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & 4) {

					int a;
					if(intcounter%frequency==0) {
						for(a=0; a<5; a++) {
							erase_sprite(asteroids[a],BACKGROUND);
							asteroids[a]->y+=100;
							draw_sprite(asteroids[a]);
						}
						vg_buffertomem();
					}

					intcounter++;
				}
				break;
			default:
				break;
			}
		} else {
		}
	}

	//destroy_sprite(sprites[0]);

	mouse_unsubscribe_int();
	timer_unsubscribe_int();
	vg_exit(); /* Return to text mode */

	return 0;
}
