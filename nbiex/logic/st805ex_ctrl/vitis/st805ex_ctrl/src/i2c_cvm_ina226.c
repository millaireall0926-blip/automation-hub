
#include "define.h"


struct _I2C_TARGET ina_i2c[] = {
	{  XPAR_AXI_IIC_INA_0_BASEADDR,    0x40, "INA226", "DPS_F00_MEAS"  },
	{  XPAR_AXI_IIC_INA_1_BASEADDR,    0x40, "INA226", "DPS_F01_MEAS"  },
	{  XPAR_AXI_IIC_INA_2_BASEADDR,    0x40, "INA226", "DPS_F02_MEAS"  },
	{  XPAR_AXI_IIC_INA_3_BASEADDR,    0x40, "INA226", "DPS_F03_MEAS"  },
	{  XPAR_AXI_IIC_INA_4_BASEADDR,    0x40, "INA226", "DPS_F04_MEAS"  },
	{  XPAR_AXI_IIC_INA_5_BASEADDR,    0x40, "INA226", "DPS_F05_MEAS"  },
	{  XPAR_AXI_IIC_INA_6_BASEADDR,    0x40, "INA226", "DPS_F06_MEAS"  },
	{  XPAR_AXI_IIC_INA_7_BASEADDR,    0x40, "INA226", "DPS_F07_MEAS"  },
	{  XPAR_AXI_IIC_INA_8_BASEADDR,    0x40, "INA226", "DPS_F08_MEAS"  },
	{  XPAR_AXI_IIC_INA_9_BASEADDR,    0x40, "INA226", "DPS_F09_MEAS"  },
	{  XPAR_AXI_IIC_INA_10_BASEADDR,   0x40, "INA226", "DPS_F10_MEAS"  },
	{  XPAR_AXI_IIC_INA_11_BASEADDR,   0x40, "INA226", "DPS_F11_MEAS"  },
	{  XPAR_AXI_IIC_INA_12_BASEADDR,   0x40, "INA226", "DPS_F12_MEAS"  },
	{  XPAR_AXI_IIC_INA_13_BASEADDR,   0x40, "INA226", "DPS_F13_MEAS"  },
	{  XPAR_AXI_IIC_INA_14_BASEADDR,   0x40, "INA226", "DPS_F14_MEAS"  },
	{  XPAR_AXI_IIC_INA_15_BASEADDR,   0x40, "INA226", "DPS_F15_MEAS"  },
	{  XPAR_AXI_IIC_HV_INA_0_BASEADDR, 0x40, "INA226", "HVDPS_F0_MEAS" },
	{  XPAR_AXI_IIC_HV_INA_1_BASEADDR, 0x40, "INA226", "HVDPS_F1_MEAS" },
	{  XPAR_AXI_IIC_HV_INA_2_BASEADDR, 0x40, "INA226", "HVDPS_F2_MEAS" },
	{  XPAR_AXI_IIC_HV_INA_3_BASEADDR, 0x40, "INA226", "HVDPS_F3_MEAS" }
};


void ina_i2c_reset(u8 idx) {
	XIic_WriteReg(ina_i2c[idx].base, XIIC_RESETR_OFFSET, XIIC_RESET_MASK);
}

u8 i2c_ina_wr(u8 idx, u8 addr, u16 data) {
	u8 trans_byte = 0;
	u8 byte_count = 3;
	u8 wbuf[3];
	u8 rbuf[4];

	if (idx >= MAX_INA_I2C) return 1;
	if (ina_i2c_sts & (1 << idx)) return 1;

	memset(wbuf, 0, sizeof(wbuf));
	memset(rbuf, 0, sizeof(rbuf));
	
#ifdef _SIM_
	return 0;
#endif

	wbuf[0] = addr;
	wbuf[1] = (data >> 8) & 0xFF;
	wbuf[2] = (data >> 0) & 0xFF;

	trans_byte = i2c_send_data(ina_i2c[idx].base, ina_i2c[idx].slave_addr, wbuf, byte_count, XIIC_STOP);
	
	if (trans_byte != byte_count) {
		DBG_CRIT("ina write error...\r\n");
		
		i2c_recv(ina_i2c[idx].base, rbuf, 4, XIIC_STOP);

		ina_i2c_reset(idx);
		usleep(1000);

		trans_byte = i2c_send_data(ina_i2c[idx].base, ina_i2c[idx].slave_addr, wbuf, byte_count, XIIC_STOP);

		if (trans_byte != byte_count) {
			DBG_CRIT("ina write error...\r\n");

			return 1;
		}
	}

	return 0;
}

u8 i2c_ina_rd(u8 idx, u8 addr, u16* data) {
	u8 trans_byte = 0;
	u8 byte_count = 2;
	u8 wbuf[3];
	u8 rbuf[4];

	memset(data, 0, byte_count);

	if (idx >= MAX_INA_I2C) return 1;
	if (ina_i2c_sts & (1 << idx)) return 1;

	memset(wbuf, 0, sizeof(wbuf));
	memset(rbuf, 0, sizeof(rbuf));
	
#ifdef _SIM_
	for (u8 i = 0; i < byte_count; i++) {
		*(data + i) = addr + i;
	}
	return 0;
#endif

	wbuf[0] = addr;

	trans_byte = i2c_recv_data(ina_i2c[idx].base, ina_i2c[idx].slave_addr, wbuf, INA_DATA_IDX, rbuf, byte_count, XIIC_STOP);
	
	if (trans_byte != byte_count) {
		DBG_CRIT("eep read error...\r\n");
		
		i2c_recv(ina_i2c[idx].base, rbuf, 4, XIIC_STOP);

		ina_i2c_reset(idx);
		usleep(1000);

		memset(rbuf, 0, sizeof(rbuf));
		trans_byte = i2c_recv_data(ina_i2c[idx].base, ina_i2c[idx].slave_addr, wbuf, INA_DATA_IDX, rbuf, byte_count, XIIC_STOP);

		if (trans_byte != byte_count) {
			DBG_CRIT("eep read error...\r\n");

			return 1;
		}
	}

	*data = (rbuf[0] << 8) | rbuf[1];

	return 0;
}

u8 ina_init(void) {
	u8 sts = 0;
	u16 rdata = 0;
	u16 wdata = 0;

	ina_i2c_sts = 0;
	for (u8 idx = 0; idx < MAX_INA_I2C; idx++) {
		sts = i2c_ina_rd(idx, INA226_REG_MANU_ID, &rdata);

		if ((sts != 0) || (rdata != INA226_MID_VALUE)) {
			ina_i2c_sts |= (0x1 << idx);
		} else {
			wdata = 0x4127;
			sts += i2c_ina_wr(idx, INA226_REG_CONFIG, wdata);
			// sts += i2c_ina_rd(idx, INA226_REG_CONFIG, rdata);

			// 0x1400 : 1.0mA resolution w/ Max.32.768A ( Rshunt =  1mOhm ) for DPS
			// 0x0200 : 1.0mA resolution w/ Max.32.768A ( Rshunt = 10mOhm ) for HV-DPS
			if (idx < 16) {
				wdata = 0x1400; 
			} else {
				wdata = 0x0200; 
			}
			sts += i2c_ina_wr(idx, INA226_REG_CALIBRATION, wdata);

			wdata = 0x0000;
			sts += i2c_ina_wr(idx, INA226_REG_MASK_EN, wdata);

			if (sts != 0) {
				ina_i2c_sts |= (0x1 << idx);
			}
		}
	}

	return sts;
}

u8 os_ina_ctrl(void) {
	u8 sts = 0;
	u8 cmd = (*P_INA_CMD) & INA_CMD_SET;

	if (cmd != 0) {
		updateFlag = 1;
		
		u8  idx  = ((*P_INA_CMD) >> 20) & 0x1F;
		u8  addr = ((*P_INA_CMD) >> 16) & 0x0F;
		u16 data = (*P_INA_CMD) & 0xFFFF;
		
		if ((*P_INA_CMD) & INA_CMD_WR) {
			sts = i2c_ina_wr(idx, addr, data);
		} else if ((*P_INA_CMD) & INA_CMD_RD) {
			sts = i2c_ina_rd(idx, addr, &data);
		} else {
			sts = 1;
		}

		if (sts == 0) {
			*P_INA_STS = INA_STS_EOS;
		} else {
			*P_INA_STS = INA_STS_ERR;
		}
	}

	return 0;
}

u8 ina_get_val(u8 idx, u16 *iout, u16* vout) {
	u8 sts = 0;

	if (idx >= MAX_INA_I2C) return 1;
	if (ina_i2c_sts & (1 << idx)) return 1;

	sts += i2c_ina_rd(idx, INA226_REG_CURRET,   iout); // 1.00mA/LSB
	sts += i2c_ina_rd(idx, INA226_REG_BUS_VOLT, vout); // 1.25mV/LSB
	
	return sts;
}
