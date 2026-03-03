
#ifndef _I2C_EEP_AT24C_H_
#define _I2C_EEP_AT24C_H_

#include "define.h"


/* defines */
#define AT24C_PAGE_SIZE   64

#define AT24C_DATA_IDX		2
#define AT24C_PAGE_SIZE		64

#define AT24C_WR_CYCLE_MS	5	// 5ms

#define AT24C_DUMMY_ADDR	0x1FFF0
#define AT24C_DUMMY_BYTE	16

#define EEP_CMD_SET   (1 << 31)
#define EEP_CMD_RD    (1 << 29)
#define EEP_CMD_WR    (1 << 28)

#define EEP_STS_EOS   (1 << 31)
#define EEP_STS_ERR   (1 << 30)


/* struct */


/* extern vars */


/* functions */
void eep_i2c_reset(void);
u8 i2c_eep_wr(u32 addr, u8 count, u8* data);
u8 i2c_eep_rd(u32 addr, u8 count, u8* data);

u8 eep_init(void);
u8 os_eep_ctrl(void);


#endif
