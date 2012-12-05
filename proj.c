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

	Sprite* pilhas[5];
	Sprite* sprites[5];
	int posicaopilhax=1024-110, posicaopilhay=0;

	sef_startup();
	/* Fetch our endpoint */
	sys_whoami(&ep, name, 256, &priv_f);
	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(ep);

	vg_init(0x105); /* Change to video mode */
	vg_fill(BACKGROUND);
	vg_memtobuffer();

	pilhas[0]=create_sprite(pilha,posicaopilhax,posicaopilhay);
	draw_sprite(pilhas[0]);
	pilhas[1]=create_sprite(pilhadentro,posicaopilhax+83,posicaopilhay+12);
	draw_sprite2(pilhas[1]);
	pilhas[2]=create_sprite(pilhadentro,posicaopilhax+63,posicaopilhay+12);
	draw_sprite2(pilhas[2]);
	pilhas[3]=create_sprite(pilhadentro,posicaopilhax+43,posicaopilhay+12);
	draw_sprite2(pilhas[3]);
	pilhas[4]=create_sprite(pilhadentro,posicaopilhax+23,posicaopilhay+12);
	draw_sprite2(pilhas[4]);

	sprites[4]=create_sprite(spaceship,768-100,1024/2);
	draw_sprite(sprites[4]);

	vg_buffertomem();
	sleep(4);
	//destroy_sprite(sprites[0]);

	vg_exit(); /* Return to text mode */

	return 0;
}
