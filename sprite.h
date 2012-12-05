#ifndef __SPRITE_H
#define __SPRITE_H

typedef struct {
	int x, y; // current position
	int width, height; // dimensions
	//int xspeed, yspeed; // current speed
	char *map; // the pixmap
} Sprite;


int draw_sprite(Sprite *sp);
int draw_sprite2(Sprite *sprt);
char *read_xpm(char *map[], int *wd, int *ht);

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite * create_sprite(char *pic[], int x, int y);

/** Animate the sprite "fig" according to its attributes in memory,
 * whose address is "base".  The animation detects the screen borders
 * and change the speed according; it also detects collision with
 * other objects, including the player pad. Collisions with the screen
 * border generates a perfect reflection, while collision with other
 * objects generates a random perturbation in the object speed. Other
 * strategies can be devised: use quasi-elastic collision based on the
 * objects "mass" and speed, generate spin effect based on amount of
 * tangential speed direction relative to the object center of
 * "mass"...  Returns the kind of collision detected, RIGHT_HIT or
 * LEFT_HIT, if a collision with the players pad (WHITE colored!) is
 * detected.
 */
int animate_sprite(Sprite *fig, char *base);

/** The "fig" sprite is erased from memory whose address is "base"
 * and used resources released.
 */
void destroy_sprite(Sprite *fig);

/** Move in memory whose address is 'base', the 'fig' cursor, a
 * standard sprite, from its current position to a new position
 * 'xstep' and 'ystep' away.
 * The cursor is erased from its present position in xor-mode and draw
 * in the new position also oin xor-mode.
 * The first time this function is called it only draws the cursor at the
 * current position.
 */

void move_cursor(Sprite *fig, int xstep, int ystep, char *base);


#endif
