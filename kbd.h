#ifndef _TEST4_H_
#define _TEST4_H_

#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define IN_BUF 				0x60 //or 0x64
#define OUT_BUF				0x60
#define KBD_IRQ				1
#define DELAY_US 20000

//C@KBD responses
#define ESC_SCAN			0x01
#define ESC_BREAK			0x81
#define ACK					0xFA
#define RESEND				0xFE
#define ERROR				0xFC

//Keyboard Commands
#define KBD_RESET	 		0xFF
#define KBD_DEFAULT 		0xF6 //set default values and enable KBD
#define KBD_DISABLE			0xF5 //set default values and stop scanning
#define CLEAR_BUFFER		0xF4 //clear buffer and enable kbd
#define KBD_CHANGE_RATE		0xF3 // 0-4: rate; 5-6:delay
#define KBD_LED				0xED // 0: scroll lock; 1: num lock; 2: caps lock
#define BIT(n) 				(0x01<<(n))

//Status Register
#define IBF					BIT(1) // =1: full; Inbuffer
#define OBF					BIT(0) // =1: full; Outbuffer
#define PAR_ERR				BIT(7)
#define TO_ERR				BIT(6)

//KBC Commands
#define READ_COMMAND_BYTE	0x20 // returns Command Byte
#define WRITE_COMMAND_BYTE	0x60 // write Command Byte
#define CHECK_KBC			0xAA
#define KBC_OK				0x55
#define KBC_ERROR 			0xFC
#define KBD_INT_CHECK 		0xAB // returns 0 if OK
#define DISABLE_KBD_INT		0xAD //Disable KBD Interface
#define ENABLE_KBD_INT		0xAE



int test_scan(void);
int test_leds(unsigned short n, unsigned short *leds);
int kbd_unsubscribe_int();
int kbd_subscribe_int();
unsigned char kbd_int_handler();
int driver_receiver();
int issue_command(unsigned char cmd);/*
int timer_unsubscribe_int();
int timer_subscribe_int();
void timer_int_handler();
int timer_test_int(unsigned long atime);*/

#endif
