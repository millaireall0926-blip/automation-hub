
#ifndef _ADC_AD4855_H_
#define _ADC_AD4855_H_

#include "define.h"


/* defines */
#define ADC_CMD_SET   (1 << 31)
#define ADC_CMD_AUTO  (1 << 30)
#define ADC_CMD_REG   (1 << 28)
#define ADC_CMD_READ  (1 << 23)

#define ADC_OPT_OS    (1 << 7)

#define ADC_STS_EOS   (1 << 31)
#define ADC_STS_ERR   (1 << 30)

#define ADC_LOC_MUX   (24)
#define ADC_LOC_ADDR  (8)


// AD4855 REG
#define AD4855_REG_CH_INCR      (0x12)

#define AD4855_REG_PRODUCT_ID_L (0x04)
#define AD4855_REG_PRODUCT_ID_H (0x05)

#define AD4855_REG_DEVICE_CTRL  (0x25)
#define AD4855_REG_OVERSAMPLE   (0x27)
#define AD4855_REG_SOFTSPAN     (0x2A)

#define AD4855_PID_L_VAL        (0x63)
#define AD4855_DEV_CTRL_INIT    (0x01)
#define AD4855_SOFTSPAN_INIT    (0x0B)


/* struct */


/* extern vars */


/* functions */
u8 spi_ad4855_eos(void);
u8 spi_ad4855_wr(u16 addr, u8 data);
u8 spi_ad4855_rd(u16 addr);

u8 ad4855_init(void);
u8 ad4855_get_volt(u8 mux);


#endif
