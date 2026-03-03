
#ifndef _I2C_DPS_LTC3884_H_
#define _I2C_DPS_LTC3884_H_

#include "define.h"


/* defines */
#define LTC3884_REG_PAGE      0x00
#define LTC3884_REG_VOUT_CMD  0x21
#define LTC3884_REG_STS_WORD  0x79
#define LTC3884_REG_READ_VOUT 0x8B
#define LTC3884_REG_READ_IOUT 0x8C

#define LTC3884_REG_MFR_ID    0x99
#define LTC3884_REG_MFR_MDL   0x9A

#define LTC3884_MFR_MODEL     "LTC3884"


#define MAX_DPS_I2C   8
#define MAX_DPS_PAGE  2
#define MAX_DPS_CH    (MAX_DPS_I2C * MAX_DPS_PAGE)

#define DPS_DATA_IDX  1
#define DPS_PAGE_BYTE 1
#define DPS_REG_BYTE  2
#define DPS_MFR_BYTE  8

#define DPS_CMD_SET   (1 << 31)
#define DPS_CMD_RD    (1 << 29)
#define DPS_CMD_WR    (1 << 28)

#define DPS_STS_EOS   (1 << 31)
#define DPS_STS_ERR   (1 << 30)


/* struct */


/* extern vars */


/* functions */
void dps_i2c_reset(u8 idx);
u8 i2c_dps_wr(u8 idx, u8 cmd, u8 count, u8* data);
u8 i2c_dps_rd(u8 idx, u8 cmd, u8 count, u8* data);

u8 dps_init(void);
u8 os_dps_ctrl(void);

u8 dps_get_page(u8 idx, u8 *page);
u8 dps_set_page(u8 idx, u8 page);
u8 dps_get_val(u8 idx, u8 page, u16 *wsts, u16 *iout, u16* vout);


#endif
