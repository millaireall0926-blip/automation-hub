
#ifndef _MAIN_H_
#define _MAIN_H_


#include "define.h"


/* defines */


/* struct */
#pragma pack (1)
typedef struct _ALL_STS {
	u8	ver[32];
	u8	tpcbRev;
	u8	pgRev;
	u8	slotId;
	u8	nBrdChk;
	u32	bibId;
	u16	adcCode[16][8];
	u16	dpsRun;
	u8	hvDpsEn;
	u8	vrefEn;
	u8	dpsAlert;
	u8	dpsFault;
	u16	dpsWsts[16];
	u16	dpsVout[16];
	u16	dpsIout[16];
	u16	inaVout[20];
	u16	inaIout[20];

	u8	adcSpiSts;
	u8	dacI2cSts;
	u8	dpsI2cSts;
	u32	inaI2cSts;
	u8	eepI2cSts;
} ALL_STS, *P_ALL_STS;
#pragma pack ()

typedef enum _LED_ONOFF {
	LED_OFF		= 0x0,
	LED_ON		= 0x1,
	LED_BLINK	= 0x2
} LED_ONOFF;


/* extern vars */
extern  u8 dbgFlag;
extern  u8 updateFlag;

extern  u8 adc_spi_sts;
extern  u8 dac_i2c_sts;
extern  u8 dps_i2c_sts;
extern u32 ina_i2c_sts;
extern  u8 eep_i2c_sts;

extern P_ALL_STS all_sts;


/* functions */
void led_act_on(void) ;
void led_on_on(void)  ;
void led_err_on(void) ;

void led_act_off(void);
void led_on_off(void) ;
void led_err_off(void);

void led_act_blink(void);

void intr_enable(void);
void intr_disable(void);


#endif
