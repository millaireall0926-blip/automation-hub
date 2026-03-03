
#include "define.h"


u8 spi_ad4855_eos(void) {
  // tCYCLE = 4us
	// OS = 4
  // max : tSETTLING + tCYCLE * OS * 2 < 10ms
  u32 timeout = ADC_TIMEOUT;

  while ((((*P_ADC_STS) & ADC_STS_EOS) == 0x0) && (--timeout > 0)) {
    if (((*P_ADC_STS) & ADC_STS_ERR) != 0x0) {
      DBG_CRIT("\t - spi_ad4855_eos.err : expect[ %08X ] - sts[ %08X ] !!!\r\n", ADC_STS_EOS, *P_ADC_STS);
      return 1;
    }
  }

  if (timeout == 0) {
    DBG_CRIT("\t - spi_ad4855_eos.timeout : expect[ %08X ] - sts[ %08X ] !!!\r\n", ADC_STS_EOS, *P_ADC_STS);
    return 1;
  }

  return 1;
}

u8 spi_ad4855_wr(u16 addr, u8 data) {
	u8 sts = 0;

	*P_ADC_CMD = ADC_CMD_SET | ADC_CMD_REG | ((addr & 0x7FFF) << ADC_LOC_ADDR) | data;
	sts = spi_ad4855_eos();

	return sts;
}

u8 spi_ad4855_rd(u16 addr) {
	u8 sts = 0;
	u8 data = 0;

	*P_ADC_CMD = ADC_CMD_SET | ADC_CMD_REG | ADC_CMD_READ | ((addr & 0x7FFF) << ADC_LOC_ADDR);
	sts = spi_ad4855_eos();

	if (sts == 0) {
		data = (*P_ADC_STS) & 0xFF;
	}

	return data;
}

u8 ad4855_init(void) {
	u8 sts = 0;
	u8 os_ratio = 1; // os = 1 << (os_ratio + 1);

	adc_spi_sts = 0;

	// ip setting
	*P_ADC_PD = 0;
	*P_ADC_OS = ADC_OPT_OS | os_ratio;
	*P_ADC_DLY = 500000; // 5ms settling time


	// chip setting
	// dev id check : AD4855_PID_L_VAL
	if (spi_ad4855_rd(AD4855_REG_PRODUCT_ID_L) != AD4855_PID_L_VAL) {
		adc_spi_sts = 1;
		return 1;
	}

	sts += spi_ad4855_wr(AD4855_REG_DEVICE_CTRL, AD4855_DEV_CTRL_INIT);

	sts += spi_ad4855_wr(AD4855_REG_OVERSAMPLE, os_ratio);
	sts += spi_ad4855_wr(AD4855_REG_OVERSAMPLE, 0x80 | os_ratio);

	for (u8 ch = 0; ch < 8; ch++) {
		sts += spi_ad4855_wr(AD4855_REG_SOFTSPAN + AD4855_REG_CH_INCR * ch, AD4855_SOFTSPAN_INIT); // +/-20V range
	}

	return sts;
}

u8 ad4855_get_volt(u8 mux) {
	u8 sts = 0;

	*P_ADC_CMD = ADC_CMD_SET | ((mux & 0x0F) << ADC_LOC_MUX);
	sts = spi_ad4855_eos();

	return sts;
}
