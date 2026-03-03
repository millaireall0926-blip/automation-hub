
#ifndef _SPI_CFG_PROM_H_
#define _SPI_CFG_PROM_H_

#include "define.h"


/* defines */
#ifndef SDT
#define SPI_PROM_DEVICE_ID  XPAR_SPI_0_DEVICE_ID
#endif

#define PROM_CMD_WR		1 << 7
#define PROM_CMD_RD		1 << 6
#define PROM_CMD_ERASE	1 << 0

#define PROM_STS_BUSY	0x01
#define PROM_STS_ERR	0x08
#define PROM_STS_EOS	0x80


/* struct */


/* extern vars */
extern XSpi spiCfgProm;


/* functions */
u8 prom_init(void);
u8 os_prom_ctrl(void);

#endif
