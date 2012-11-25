#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"

/* Constants for VBE 0x105 mode */

#define VRAM_PHYS_ADDR    	0xD0000000
#define H_RES             	1024
#define V_RES				768
#define BITS_PER_PIXEL	 	8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


void * vg_init(unsigned long mode) {

	struct reg86u r;

	r.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode -- ah:0x4F invoking VBE function, al:0x02 function being called:set VBE mode
	r.u.w.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r.u.b.intno = 0x10;

	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
	}


/*
	vbe_mode_info_t info;

	if(vbe_get_mode_info(mode,&info)==1)
		printf("error in vbe_get_mode \n");

	h_res=info.XResolution;
	v_res=info.YResolution;
	bits_per_pixel=info.BitsPerPixel;
	*video_mem=info.PhysBasePtr;*/




	h_res=H_RES;
	v_res=V_RES;
	bits_per_pixel=BITS_PER_PIXEL;


	int y;
	struct mem_range mr;
	/* Allow memory mapping */

	mr.mr_base = VRAM_PHYS_ADDR;
	mr.mr_limit = mr.mr_base + h_res*v_res;
	if( OK != (y = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", y);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, h_res*v_res);

	if(video_mem == MAP_FAILED) panic("video_txt couldn't map video memory");


    return NULL;
}


int vg_fill(unsigned long color) {

	char *p = video_mem;
	int i;

	printf("video_mem, h_res, v_res: %x %d %d", &video_mem, h_res, v_res);

	if(color>=256||color<0)
		return -1;

	for(i=0; i<h_res*v_res;i++)
	{
		*p=(char)color;
		p++;
	}

	return 0;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {

	char *p = video_mem;
	int i;

	if(color>=256||color<0||x<0||y<0||x>=h_res||y>=v_res)
		return -1;

	p+=x*bits_per_pixel;
	p+=y*h_res*bits_per_pixel;

	*p=(char)color;

	return 0;
}

long vg_get_pixel(unsigned long x, unsigned long y) {

	char *p = video_mem;

	if(x<0||y<0||x>=h_res||y>=v_res)
			return -1;

	p+=x*bits_per_pixel;
	p+=y*h_res*bits_per_pixel;

	return *p;
}

unsigned long equation (unsigned long x, unsigned long yi, int m)
{
	return x*m+yi;
}

int vg_draw_line(unsigned long xi, unsigned long yi, 
		 unsigned long xf, unsigned long yf, unsigned long color) {

	char *p = video_mem;

	if(color>=256||color<0||xi<0||yi<0||xi>=h_res||yi>=v_res||xf<0||yf<0||xf>=h_res||yf>=v_res)
			return -1;

	p+=xi;
	p+=yi*h_res;

	unsigned long i, m=(yf-yi)/(xf-xi);

	*p=(char)color;

	if(yi==yf)
		for(i=0; i<xi-xf;i++)
		{
			p++;
			*p=(char)color;
		}
	else if(xi==xf)
		for(i=0; i<xi-xf;i++)
		{
			p+=h_res;
			*p=(char)color;
		}
	else for (i=xi; i<=xf; i++)
		vg_set_pixel(i,equation(i,yi,m),color);

	return 0;
}


int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}
