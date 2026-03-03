
#ifndef _I2C_CVM_INA226_H_
#define _I2C_CVM_INA226_H_

#include "define.h"


/* defines */
#define INA226_REG_CONFIG				0x00
#define INA226_REG_SHUNT_VOLT		0x01
#define INA226_REG_BUS_VOLT			0x02
#define INA226_REG_POWER				0x03
#define INA226_REG_CURRET				0x04
#define INA226_REG_CALIBRATION	0x05
#define INA226_REG_MASK_EN			0x06
#define INA226_REG_MANU_ID 			0xFE

#define INA226_MID_VALUE        0x5449


#define MAX_INA_I2C   20
#define MAX_INA_IDX   20

#define INA_DATA_IDX  1
#define INA_REG_BYTE  2
#define INA_CUR_BYTE  2


#define INA_CMD_SET   (1 << 31)
#define INA_CMD_RD    (1 << 29)
#define INA_CMD_WR    (1 << 28)

#define INA_STS_EOS   (1 << 31)
#define INA_STS_ERR   (1 << 30)


/* struct */


/* extern vars */


/* functions */
void ina_i2c_reset(u8 idx);
u8 i2c_ina_wr(u8 idx, u8 addr, u16 data);
u8 i2c_ina_rd(u8 idx, u8 addr, u16* data);

u8 ina_init(void);
u8 os_ina_ctrl(void);

u8 ina_get_val(u8 idx, u16 *iout, u16* vout);


#endif
