#ifndef PROJ_H_
#define PROJ_H_
#include <minix/drivers.h>

#include "kbd.h"
#include "mouse.h"
#include "rtc.h"
#include "timer.h"
#include "sprite.h"
#include "video_gr.h"
#include "vbe.h"
#include "xpm.h"

#define BIT(n) 				(0x01<<(n))
#define BACKGROUND 			0
#define BATTERYXPOS			H_RES-110
#define BATTERYYPOS			0
#define NUM_ASTEROIDS		35
#define ASTEROID_PERIOD		1			// controls asteroid's movement rate
#define SHOT_PERIOD			150			//Time between shot's recharge
#define DELTA				100			//Time between velocity increment

static char** digits[]={digits0,digits1,digits2,digits3,digits4,digits5,digits6,digits7,digits8, digits9};

//Represents each element in the game
Sprite* batteries[5];
Sprite* spaceships[1];
Sprite* asteroids[NUM_ASTEROIDS];
Sprite* shotsprt[4];
Sprite* timesprt[3];
Sprite* rato[1];
int posx_inicial=(H_RES/2)-40;
int posy_inicial=V_RES-200;
unsigned short times[]={0,0,0}, timesalarm[]={0,0,0}; // stores time since the game started
													  // and the time read from the RTC

unsigned char packet[3];
unsigned short count=0;
unsigned long stat;

//Relevant to know when to stop reading time, playing, and when to update framebuffer
unsigned short readtime=0, finished=0, changed=0;

//Sprite's velocities and remaining shots
unsigned int asteroidvel=1, shotvel=5, shipvel=10, shots=4, intcounter=1;

int kbd_int_handler();
void timer_int_handler();
void mouse_int_handler();

#endif
