#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "sprite.h"

#define HRES             	1024
#define VRES				768

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", and
 * draws it in memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite *create_sprite(char *pic[], int x, int y) {

	//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
	if( sp == NULL )
		return NULL;

	// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if( sp->map == NULL ) {
		free(sp);
		return NULL;
	}

	sp->x=x;
	sp->y=y;

	return sp;
}

char *read_xpm(char *map[], int *wd, int *ht)
{
	int width, height, colors;
	char sym[256];
	int  col;
	int i, j;
	char *pix, *pixtmp, *tmp, *line;
	char symbol;

	/* read width, height, colors */
	if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
		printf("read_xpm: incorrect width, height, colors\n");
		return NULL;
	}

	if (width > HRES || height > VRES || colors > 256) {
		printf("read_xpm: incorrect width, height, colors\n");
		return NULL;
	}

	*wd = width;
	*ht = height;

	for (i=0; i<256; i++)
		sym[i] = 0;

	/* read symbols <-> colors */
	for (i=0; i<colors; i++) {
		if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
			printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
			return NULL;
		}

		if (col > 256) {
			printf("read_xpm: incorrect color at line %d\n", i+1);
			return NULL;
		}
		sym[col] = symbol;
	}

	/* allocate pixmap memory */
	pix = pixtmp = malloc(width*height);

	/* parse each pixmap symbol line */
	for (i=0; i<height; i++) {
		line = map[colors+1+i];

		for (j=0; j<width; j++) {
			tmp = memchr(sym, line[j], 256);  /* find color of each symbol */
			if (tmp == NULL) {
				printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
				return NULL;
			}
			*pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
		}
	}
	return pix;
}

void erase_sprite(Sprite *sprt, unsigned long background) {

	int i, j;
	Sprite tmp=*sprt;

	for (i = 0; i < tmp.height; i++)
		for (j = 0; j < tmp.width; j++, tmp.map++)
			if(vg_get_pixel_buffer(j,i)!=25 && vg_get_pixel_buffer(j,i)!=17)
				vg_set_pixel_buffer(j+tmp.x, i+tmp.y, background);

}

void erase_sprite2(Sprite *sprt, unsigned long background) {

	int i, j;
	Sprite tmp=*sprt;

	for (i = 0; i < tmp.height; i++)
		for(j = 0; j < tmp.width; j++, tmp.map++)
			vg_set_pixel_buffer(j+tmp.x, i+tmp.y, background);

}

int draw_sprite(Sprite *sprt) {
	int i, j;
	Sprite tmp=*sprt;

	for (i = 0; i < tmp.height; i++)
		for (j = 0; j < tmp.width; j++, tmp.map++)
			if(*tmp.map!=0 && vg_get_pixel_buffer(j,i)==0)
				vg_set_pixel_buffer(j+tmp.x, i+tmp.y, *tmp.map);


	return 0;
}

int draw_asteroid(Sprite *sprt) {
	int i, j;
	Sprite tmp=*sprt;

	for (i = 0; i < tmp.height; i++)
		for (j = 0; j < tmp.width; j++, tmp.map++)
			if(vg_get_pixel_buffer(j,i)==43)
				printf("Colidiu.\n");
			else if(*tmp.map!=0 && vg_get_pixel_buffer(j,i)==0)
				vg_set_pixel_buffer(j+tmp.x, i+tmp.y, *tmp.map);


	return 0;
}

int draw_sprite2(Sprite *sprt) {

	int i, j;
	Sprite tmp=*sprt;

	for (i = 0; i < tmp.height; i++)
		for (j = 0; j < tmp.width; j++, tmp.map++)
			if(*tmp.map!=0)
				vg_set_pixel_buffer(j+tmp.x, i+tmp.y, *tmp.map);


	return 0;
}
