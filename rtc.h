#ifndef TEST6_H_
#define TEST6_H_

#define STAT_REG			0x64
#define KBC_CMD_REG 		0x64
#define IN_BUF 				0x60 //or 0x64
#define OUT_BUF				0x60
#define ACK					0xFA
#define RESEND				0xFE
#define ERROR				0xFC
#define OKK					0xAA
#define READ_ID				0x00

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REF 0x71

#define RTC_IRQ				8

#define SECONDS				0x00
#define SECONDS_ALARM		0x01
#define MINUTES				0x02
#define MINUTES_ALARM		0x03
#define HOURS				0x04
#define HOURS_ALARM			0x05
#define DAY_OF_THE_WEEK		0x06
#define DAY_OF_THE_MONTH	0x07
#define MONTH				0x08
#define YEAR				0x09
#define REG_A				0x0A
#define REG_B				0x0B
#define REG_C				0x0C
#define REG_D				0x0D

#define BIT(n) (0x01<<(n))

int test_conf(void);
int test_date(void);
int test_period(/* to be defined in class */);
void interpret_A(unsigned long stat);
void interpret_B(unsigned long stat);
void interpret_C(unsigned long stat);
void interpret_D(unsigned long stat);
void choosePort(char port);
void readPort(unsigned long *stat);
void readTime(unsigned short *time, char port, unsigned long *stat);
void readDate(unsigned short *date, char port, unsigned long *stat);

#endif /* TEST6_H_ */
