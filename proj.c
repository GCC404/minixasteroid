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

static char *pic1[] = {
"32 13 4", /* número de pixels horizontais, verticais, e cores */
". 0", /* símbolo ’.’ representa pixel de cor 0 */
"x 2", /* símbolo ’x’ representa pixel de cor 2 */
"o 14",
"+ 4",
"................................", /* pixels ... */
"..............xxx...............",
"............xxxxxxx.............",
".........xxxxxx+xxxxxx..........",
"......xxxxxxx+++++xxxxxxx.......",
"....xxxxxxx+++++++++xxxxxxx.....",
"....xxxxxxx+++++++++xxxxxxx.....",
"......xxxxxxx+++++xxxxxxx.......",
".........xxxxxx+xxxxxx..........",
"..........ooxxxxxxxoo...........",
".......ooo...........ooo........",
".....ooo...............ooo......",
"...ooo...................ooo...."
};

#define VRAM_PHYS_ADDR    	0xD0000000 //ja em video_gr

int main(int argc, char **argv) {

	endpoint_t ep;
	char name[256];
	int priv_f;
	char* video_mem;

	sef_startup();
	/* Fetch our endpoint */
	sys_whoami(&ep, name, 256, &priv_f);
	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(ep);

	vg_init(0x105); /* Change to video mode */

	//test_scan(); /* Rato */

	Sprite* s1=create_sprite(pic1,video_mem);
	draw_sprite(sp,video_mem);


	sleep(3);

	vg_exit(); /* Return to text mode */

	return 0;
}
