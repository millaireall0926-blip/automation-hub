
#ifndef _I2C_DAC_AD5677_H_
#define _I2C_DAC_AD5677_H_

#include "define.h"


/* defines */
#define AD5677_MAX_IDX	    3
#define AD5677_MAX_CH		    16
#define AD5677_MAX_VOLT		  3350.0	// mV

#define AD5677_IVREF			  2500.0	// mV
#define AD5677_GAIN				  2.0
#define AD5677_SPAN				  (AD5677_IVREF * AD5677_GAIN)
#define AD5677_LSB				  (AD5677_SPAN / 65535.0)

#define AD5677_CMD_NOP		  0x0
#define AD5677_CMD_WR_IN	  0x1
#define AD5677_CMD_UP_DAC	  0x2
#define AD5677_CMD_WR_DAC	  0x3
#define AD5677_CMD_PCTRL	  0x4
#define AD5677_CMD_RDBACK	  0x9

#define AD5677_DB_PWR_UP		0
#define AD5677_DB_PWR_DOWN	1


#define DAC_CMD_SET   0x80000000

#define DAC_STS_EOS   0x80000000
#define DAC_STS_ERR   0x40000000


/* struct */


/* extern vars */


/* functions */
void dac_i2c_reset(u8 idx);
void dac_reset(void);
void dac_load(u8 idx);

u8 i2c_dac_wr(u8 idx, u8 cmd, u8 addr, u16 data);
u8 i2c_dac_rd(u8 idx, u8 cmd, u8 addr, u16* data);

u8 dac_init(void);
u8 os_dac_ctrl(void);

u8 dac_set_volt(u8 idx, u8 ch, float mV);
u8 dac_set_update(u8 idx, u16 sel);
u8 dac_get_volt(u8 idx, u8 ch, float* mV);


#endif
