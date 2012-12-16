#ifndef __SPRITE_H
#define __SPRITE_H

typedef struct {
	int x, y; // current position
	int width, height; // dimensions
	char *map; // the pixmap
} Sprite;


int draw_sprite(Sprite *sp);
char *read_xpm(char *map[], int *wd, int *ht);
void erase_sprite(Sprite *sprt, unsigned long background);
int draw_asteroid(Sprite *sprt);
int draw_spaceship(Sprite *sprt);

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite * create_sprite(char *pic[], int x, int y);

#endif
