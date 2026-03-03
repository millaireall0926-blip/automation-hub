
#include "define.h"


struct _I2C_TARGET dac_i2c[] = {
	{ XPAR_AXI_IIC_DAC_0_BASEADDR, 0x0C, "AD5677-U56", "DAC_PE_VH   " },
	{ XPAR_AXI_IIC_DAC_1_BASEADDR, 0x0C, "AD5677-U55", "DAC_VTH_VREF" },
	{ XPAR_AXI_IIC_DAC_2_BASEADDR, 0x0C, "AD5677-U57", "DAC_DPS     " }
};


void dac_i2c_reset(u8 idx) {
	XIic_WriteReg(dac_i2c[idx].base, XIIC_RESETR_OFFSET, XIIC_RESET_MASK);
}

void dac_reset(void) {
	*P_DAC_CMD = 0x40000000;
	usleep(1000);
}

void dac_load(u8 idx) {
	if (idx >= AD5677_MAX_IDX) {
		*P_DAC_LDAC = 0x7;
	} else {
		*P_DAC_LDAC = 1 << idx;
	}
}

u8 i2c_dac_wr(u8 idx, u8 cmd, u8 addr, u16 data) {
	u8 trans_byte = 0;
	u8 byte_count = 3;
	u8 wbuf[3];
	u8 rbuf[4];

	if (idx >= AD5677_MAX_IDX) return 1;
	if (dac_i2c_sts & (1 << idx)) return 1;

	memset(wbuf, 0, byte_count);
	memset(rbuf, 0, 4);
	
#ifdef _SIM_
	return 0;
#endif

	u8 ca = ((cmd << 4) & 0xF0) | ((addr << 0) & 0x0F);
	wbuf[0] = (ca   >> 0) & 0xFF;
	wbuf[1] = (data >> 8) & 0xFF;
	wbuf[2] = (data >> 0) & 0xFF;

	trans_byte = i2c_send_data(dac_i2c[idx].base, dac_i2c[idx].slave_addr, wbuf, byte_count, XIIC_STOP);
	
	if (trans_byte != byte_count) {
		DBG_CRIT("dac write error...\r\n");
		
		i2c_recv(dac_i2c[idx].base, rbuf, 4, XIIC_STOP);

		dac_i2c_reset(idx);
		usleep(1000);

		trans_byte = i2c_send_data(dac_i2c[idx].base, dac_i2c[idx].slave_addr, wbuf, byte_count, XIIC_STOP);

		if (trans_byte != byte_count) {
			DBG_CRIT("dac write error...\r\n");

			return 1;
		}
	}
	
	return 0;
}

u8 i2c_dac_rd(u8 idx, u8 cmd, u8 addr, u16* data) {
	u8 trans_byte = 0;
	u8 byte_count = 2;
	u8 wbuf[3];
	u8 rbuf[4];

	memset(wbuf, 0, byte_count);
	memset(rbuf, 0, 4);
	memset(data, 0, 2);

	if (idx >= AD5677_MAX_IDX) return 1;
	if (dac_i2c_sts & (1 << idx)) return 1;
	if (cmd != AD5677_CMD_RDBACK) return 1;

#ifdef _SIM_
	*data = 0x1234;
	return 0;
#endif

	u8 ca = ((cmd << 4) & 0xF0) | ((addr << 0) & 0x0F);
	wbuf[0] = (ca   >> 0) & 0xFF;
	wbuf[1] = 0x00;
	wbuf[2] = 0x00;


	trans_byte = XIic_Send(dac_i2c[idx].base, dac_i2c[idx].slave_addr, wbuf, 3, XIIC_STOP);
	if (trans_byte != 3) {
			DBG_CRIT("dac read.send error...\r\n");

			i2c_recv(dac_i2c[idx].base, rbuf, 4, XIIC_STOP);

			dac_i2c_reset(idx);
			usleep(1000);

			memset(rbuf, 0, 4);
			trans_byte = XIic_Send(dac_i2c[idx].base, dac_i2c[idx].slave_addr, wbuf, 3, XIIC_STOP);

			if (trans_byte != byte_count) {
				DBG_CRIT("dac read.send error...\r\n");

				return 1;
			}
		}

	trans_byte = XIic_Recv(dac_i2c[idx].base, dac_i2c[idx].slave_addr, rbuf, byte_count, XIIC_STOP);
	if (trans_byte != byte_count) {
		DBG_CRIT("dac read.recv error...\r\n");

		i2c_recv(dac_i2c[idx].base, rbuf, 4, XIIC_STOP);

		dac_i2c_reset(idx);
		usleep(1000);

		memset(rbuf, 0, 4);
		trans_byte = XIic_Recv(dac_i2c[idx].base, dac_i2c[idx].slave_addr, rbuf, byte_count, XIIC_STOP);

		if (trans_byte != byte_count) {
			DBG_CRIT("dac read.recv error...\r\n");

			return 1;
		}
	}
	
	*data = (rbuf[0] << 8) | rbuf[1];

	return 0;
}

u8 dac_init(void) {
	u8 sts = 0;
	u16 code = 0x0;

	dac_i2c_sts = 0;

	for (u8 idx = 0; idx < AD5677_MAX_IDX; idx++) {
		sts = 0;
		for (u8 ch = 0; ch < AD5677_MAX_CH; ch++) {
			sts += i2c_dac_wr(idx, AD5677_CMD_WR_IN, ch, ch + 1);
			if (sts != 0) {
				dac_i2c_sts |= (1 << idx);
				break;
			}
		}

		if (sts == 0) {
			for (u8 ch = 0; ch < AD5677_MAX_CH; ch++) {
				sts += i2c_dac_rd(idx, AD5677_CMD_RDBACK, ch, &code);
				if ((sts != 0) || (code != (ch + 1))) {
					dac_i2c_sts |= (1 << idx);
					break;
				}
			}

			sts = 0;
			for (u8 ch = 0; ch < AD5677_MAX_CH; ch++) {
				sts += i2c_dac_wr(idx, AD5677_CMD_WR_IN, ch, 0);
				if (sts != 0) {
					dac_i2c_sts |= (1 << idx);
					break;
				}
			}
		}
	}

	return dac_i2c_sts;
}

u8 os_dac_ctrl(void) {
	u8 sts = 0;
	u8 cmd = (*P_DAC_CMD) & DAC_CMD_SET;

	if (cmd != 0) {
		updateFlag = 1;
		
		u16 data = 0;
		u8  idx  = ((*P_DPS_CMD) >> 24) & 0x03;
		u8  cmd  = ((*P_DPS_CMD) >> 20) & 0x0F;
		u8  addr = ((*P_DPS_CMD) >> 16) & 0x0F;
		
		sts = i2c_dac_wr(idx, cmd, addr, data);

		if (sts == 0) {
			*P_DPS_STS = DPS_STS_EOS;
		} else {
			*P_DPS_STS = DPS_STS_ERR;
		}
	}

	return sts;
}

// 
u8 dac_set_volt(u8 idx, u8 ch, float mV) {
	u8 sts = 0;
	u16 code = 0;

	if ((idx >= AD5677_MAX_IDX) || (ch >= AD5677_MAX_CH)) return 1;

	if (mV >= AD5677_MAX_VOLT) {
		mV = AD5677_MAX_VOLT;
	}

	code = (u16)(mV / AD5677_LSB);
	sts = i2c_dac_wr(idx, AD5677_CMD_WR_DAC, ch, code);
	// sts = i2c_dac_wr(idx, AD5677_CMD_WR_IN, ch, code);

	return sts;
}

u8 dac_set_update(u8 idx, u16 sel) {
	u8 sts = 0;

	if (idx >= AD5677_MAX_IDX) return 1;

	sts = i2c_dac_wr(idx, AD5677_CMD_UP_DAC, 0, sel);

	return sts;
}

u8 dac_get_volt(u8 idx, u8 ch, float* mV) {
	u8 sts = 0;
	u16 code = 0;

	if ((idx >= AD5677_MAX_IDX) || (ch >= AD5677_MAX_CH)) return 1;

	*mV = 0;
	sts = i2c_dac_rd(idx, AD5677_CMD_RDBACK, ch, &code);
	if (sts == 0) {
		*mV = AD5677_LSB * code;
	}

	return sts;
}
