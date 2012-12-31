#ifndef RTC_H_
#define RTC_H_

#define RTC_IRQ				8
#define RTC_ADDR_REG 		0x70
#define RTC_DATA_REF		0x71

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
#define REG_B				0x0B
#define REG_C				0x0C

#define BIT(n) (0x01<<(n))

static int rtc_hook=4;

static void choosePort(char port);
static void readPort(unsigned long *stat);
static void writePort(unsigned long stat);
static void readTime(unsigned short *time, char port, unsigned long *stat);
int rtc_subscribe_int();
int rtc_unsubscribe_int();
static int bcd_to_decimal(char bcd);
static char decimal_to_bcd(int decimal);
int rtc_int_handler(unsigned short readtime, unsigned short* timesalarm, int DELTA);

#endif /* RTC_H_ */
