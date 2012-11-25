#include <minix/drivers.h>

#include "kbd.h"
#include "mouse.h"
#include "rtc.h"
#include "timer.h"
#include "sprite.h"
#include "video_gr.h"
#include "vbe.h"
#include "video_gr.h"

/*
 * Estou a: remover codigo duplicado e testar lab a lab, para ficar tudo coerente e pronto a integrar.
 *
 *
 * Drawline nao esta a funcionar (nao e importante, nao vai ser usado)
 *
 * No meu ecra, muda bem os pixel, mas ficam muito afastados (a tratar depois) -> considerar aumentar resolucao -> mudar o modo grafico 0x105 -> precisa get mode info
 *
 * Teclado: breakcode trocado com scancode, acho..
 *
 * Falta testar resto dos perifericos
 */

int main(int argc, char **argv) {

	endpoint_t ep;
	char name[256];
	int priv_f;

	sef_startup();
	/* Fetch our endpoint */
	sys_whoami(&ep, name, 256, &priv_f);
	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(ep);

	//vg_init(0x105); /* Change to video mode */

	test_scan();

	sleep(3);

	vg_exit(); /* Return to text mode */

	return 0;
}
