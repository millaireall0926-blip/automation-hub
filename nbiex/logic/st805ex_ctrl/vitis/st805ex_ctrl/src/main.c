
#include "define.h"

 u8 dbgFlag = 0;
 u8 updateFlag = 0;

// vars for monitoring
 u8 adc_spi_sts = 0;
 u8 dac_i2c_sts = 0;
 u8 dps_i2c_sts = 0;
u32 ina_i2c_sts = 0;
 u8 eep_i2c_sts = 0;

P_ALL_STS all_sts = (P_ALL_STS)STS_BASE;

int main(void) {
  intr_disable();
  
	u8 sts = 0;
	memset(all_sts, 0, sizeof(struct _ALL_STS));

	memset(all_sts->ver, 0, sizeof(all_sts->ver));
	sprintf((char *)all_sts->ver, "%s Rev%d.%02d @ 20%02d%02d%02d", PJT_NAME, MAJOR, MINOR, YEAR, MONTH, DAY);

	printf("\r\n***********************************************************");
	printf("\r\n*************%32s *************", all_sts->ver);
	printf("\r\n***********************************************************\r\n");
	
	// init
	led_act_off();
	led_on_off();
	led_err_off();

	sts = ad4855_init();
	if (sts != 0) {
		DBG_CRIT("ad4855_init() error...\r\n");
		led_err_blink();
	}

	sts = dac_init();
	if (sts != 0) {
		DBG_CRIT("dac_init() error...\r\n");
		led_err_blink();
	}
	
	sts = dps_init();
	if (sts != 0) {
		DBG_CRIT("dps_init() error...\r\n");
		led_err_blink();
	}
	
	sts = ina_init();
	if (sts != 0) {
		DBG_CRIT("ina_init() error...\r\n");
		led_err_blink();
	}
	
	sts = eep_init();
	if (sts != 0) {
		DBG_CRIT("eep_init() error...\r\n");
		led_err_blink();
	}

	sts = prom_init();
	if (sts != 0) {
		DBG_CRIT("eep_init() error...\r\n");
		led_err_blink();
	}

	// TODO : bid read

	// all_sts copy
	all_sts->tpcbRev	= *P_BOARD_TPCB_REV;
	all_sts->pgRev		= *P_BOARD_PG_REV;
	all_sts->slotId		= *P_BOARD_SLOT_ID;
	all_sts->nBrdChk	= *P_BOARD_BIB_nCHK;
	// all_sts->bibId
	memcpy(all_sts->adcCode, (u8 *)P_ADC_AUTO, 8 * 16 * 2);
	all_sts->dpsRun		= *P_DPS_RUN;
	all_sts->hvDpsEn	= *P_DPS_HV_EN;
	all_sts->vrefEn		= *P_DPS_VREF_EN;
	all_sts->dpsAlert	= *P_DPS_ALERT;
	all_sts->dpsFault	= *P_DPS_FAULT;
	all_sts->adcSpiSts= adc_spi_sts;
	all_sts->dacI2cSts= dac_i2c_sts;
	all_sts->dpsI2cSts= dps_i2c_sts;
	all_sts->inaI2cSts= ina_i2c_sts;
	all_sts->eepI2cSts= eep_i2c_sts;
	
	usleep(100000);

	dbgFlag = false;
	updateFlag = true;
	
	intr_enable();

	while(1) {
		led_on_on();

		// os job -> updateFlag = 1
		sts = os_dac_ctrl();
		sts = os_dps_ctrl();
		sts = os_ina_ctrl();
		sts = os_eep_ctrl();
		sts = os_prom_ctrl();
		
		// updating
		if (updateFlag == true) {
			led_act_blink();

			// 0.5sec update rate
			timer_count_start(500 * TIMER_1MS);
			updateFlag = false;

			// TODO : 
			// bid check
			
			// i2c : 2.5us * (10bit * 6) = 150us / once
			// dps : 150us * 10 * 8ea dps = 12ms
			for (u8 dps = 0; dps < MAX_DPS_I2C; dps++) {
				u16 wsts = 0; u16 iout = 0; u16 vout = 0;
				
				u8 prevPage = 0;
				dps_get_page(dps, &prevPage);

				for (u8 page = 0; page < MAX_DPS_PAGE; page++) {
					sts = dps_get_val(dps, page, &wsts, &iout, &vout);
					if (sts == 0) {
						u8 idx = MAX_DPS_PAGE * dps + page;
						all_sts->dpsWsts[idx]	= wsts;
						all_sts->dpsIout[idx] = iout;
						all_sts->dpsVout[idx] = vout;
					}
				}

				dps_set_page(dps, prevPage);
			}
			
			// ina : 150us * 2 * 20ea ina = 6ms
			for (u8 ina = 0; ina < MAX_INA_I2C; ina++) {
				u16 iout = 0; u16 vout = 0;
				
				sts = ina_get_val(ina, &iout, &vout);
				if (sts == 0) {
					all_sts->inaIout[ina] = iout;
					all_sts->inaVout[ina] = vout;
				}
			}
		}

		// all_sts copy
		all_sts->tpcbRev	= *P_BOARD_TPCB_REV;
		all_sts->pgRev		= *P_BOARD_PG_REV;
		all_sts->slotId		= *P_BOARD_SLOT_ID;
		all_sts->nBrdChk	= *P_BOARD_BIB_nCHK;
		// all_sts->bibId
		memcpy(all_sts->adcCode, (u8 *)P_ADC_AUTO, 8 * 12 * 2);
		all_sts->dpsRun		= *P_DPS_RUN;
		all_sts->hvDpsEn	= *P_DPS_HV_EN;
		all_sts->vrefEn		= *P_DPS_VREF_EN;
		all_sts->dpsAlert	= *P_DPS_ALERT;
		all_sts->dpsFault	= *P_DPS_FAULT;

		if (timer_count_value() == 0xFFFFFFFF) updateFlag = true;
	}

  return 0;
}


void led_act_on(void)		{ *P_BOARD_LED_ACT = LED_ON; }
void led_on_on(void)		{ *P_BOARD_LED_ON  = LED_ON; }
void led_err_on(void)		{ *P_BOARD_LED_ERR = LED_ON; }

void led_act_off(void)  { *P_BOARD_LED_ACT = LED_OFF; }
void led_on_off(void)   { *P_BOARD_LED_ON  = LED_OFF; }
void led_err_off(void)  { *P_BOARD_LED_ERR = LED_OFF; }

void led_act_blink(void){ *P_BOARD_LED_ACT = LED_BLINK; }
void led_on_blink(void)	{ *P_BOARD_LED_ON  = LED_BLINK; }
void led_err_blink(void){ *P_BOARD_LED_ERR = LED_BLINK; }

void intr_enable(void)	{ Xil_ExceptionEnable();	}
void intr_disable(void)	{ Xil_ExceptionDisable();	}
