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

void destroy_sprite(Sprite *sp) {

	if( sp == NULL )
		return;
	free(sp->map);
	free(sp);
	sp = NULL; // hopeless: pointer is passed by value

}

void erase_sprite(Sprite *sprt, unsigned long background) {

	int i, j;

	for (i = 0; i < sprt->height; i++)
		for (j = 0; j < sprt->width; j++, sprt->map++)
			vg_set_pixel_buffer(j+sprt->x, i+sprt->y, background);

}

static int check_collision(Sprite *sp, char *base) {
	//...
}

int draw_sprite(Sprite *sprt) {

	int i, j;

	for (i = 0; i < sprt->height; i++)
		for (j = 0; j < sprt->width; j++, sprt->map++)
			if(*sprt->map!=0)
				vg_set_pixel_buffer(j+sprt->x, i+sprt->y, *sprt->map);


	return 0;
}
