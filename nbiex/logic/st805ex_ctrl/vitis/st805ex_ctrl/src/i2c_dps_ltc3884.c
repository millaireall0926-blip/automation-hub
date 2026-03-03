
#include "define.h"


struct _I2C_TARGET dps_i2c[] = {
	{  XPAR_AXI_IIC_DPS_0_BASEADDR, 0x33, "LTC3884", "DPS_M00" },
	{  XPAR_AXI_IIC_DPS_1_BASEADDR, 0x33, "LTC3884", "DPS_M01" },
	{  XPAR_AXI_IIC_DPS_2_BASEADDR, 0x33, "LTC3884", "DPS_M02" },
	{  XPAR_AXI_IIC_DPS_3_BASEADDR, 0x33, "LTC3884", "DPS_M03" },
	{  XPAR_AXI_IIC_DPS_4_BASEADDR, 0x33, "LTC3884", "DPS_M04" },
	{  XPAR_AXI_IIC_DPS_5_BASEADDR, 0x33, "LTC3884", "DPS_M05" },
	{  XPAR_AXI_IIC_DPS_6_BASEADDR, 0x33, "LTC3884", "DPS_M06" },
	{  XPAR_AXI_IIC_DPS_7_BASEADDR, 0x33, "LTC3884", "DPS_M07" }
};

void dps_i2c_reset(u8 idx) {
	XIic_WriteReg(dps_i2c[idx].base, XIIC_RESETR_OFFSET, XIIC_RESET_MASK);
}

u8 i2c_dps_wr(u8 idx, u8 cmd, u8 count, u8* data) {
	u8 trans_byte = 0;
	u8 byte_count = count + DPS_DATA_IDX;
	u8 wbuf[DPS_REG_BYTE + DPS_DATA_IDX];
	u8 rbuf[4];

	if (idx >= MAX_DPS_I2C) return 1;
	if (dps_i2c_sts & (1 << idx)) return 1;

	memset(wbuf, 0, sizeof(wbuf));
	memset(rbuf, 0, sizeof(rbuf));
	
#ifdef _SIM_
	return 0;
#endif

	wbuf[0] = cmd;

	for (u8 i = 0; i < byte_count; i++) {
		wbuf[DPS_DATA_IDX + i] = *(data + i);
	}

	trans_byte = i2c_send_data(dps_i2c[idx].base, dps_i2c[idx].slave_addr, wbuf, byte_count, XIIC_STOP);
	
	if (trans_byte != byte_count) {
		DBG_CRIT("dps write error...\r\n");
		
		i2c_recv(dps_i2c[idx].base, rbuf, 4, XIIC_STOP);

		dps_i2c_reset(idx);
		usleep(1000);

		trans_byte = i2c_send_data(dps_i2c[idx].base, dps_i2c[idx].slave_addr, wbuf, byte_count, XIIC_STOP);

		if (trans_byte != byte_count) {
			DBG_CRIT("dps write error...\r\n");

			return 1;
		}
	}

	return 0;
}

u8 i2c_dps_rd(u8 idx, u8 cmd, u8 count, u8* data) {
	u8 trans_byte = 0;
	u8 byte_count = count;
	u8 wbuf[DPS_DATA_IDX];
	u8 rbuf[4];

	memset(data, 0, count);

	if (idx >= MAX_DPS_I2C) return 1;
	if (dps_i2c_sts & (1 << idx)) return 1;

	memset(wbuf, 0, sizeof(wbuf));
	memset(rbuf, 0, sizeof(rbuf));
	
#ifdef _SIM_
	for (u8 i = 0; i < byte_count; i++) {
		*(data + i) = cmd + i;
	}
	return 0;
#endif

	wbuf[0] = cmd;

	trans_byte = i2c_recv_data(dps_i2c[idx].base, dps_i2c[idx].slave_addr, wbuf, DPS_DATA_IDX, rbuf, byte_count, XIIC_STOP);
	
	if (trans_byte != byte_count) {
		DBG_CRIT("dps read error...\r\n");
		
		i2c_recv(dps_i2c[idx].base, rbuf, 4, XIIC_STOP);

		dps_i2c_reset(idx);
		usleep(1000);

		memset(rbuf, 0, sizeof(rbuf));
		trans_byte = i2c_recv_data(dps_i2c[idx].base, dps_i2c[idx].slave_addr, wbuf, DPS_DATA_IDX, rbuf, byte_count, XIIC_STOP);

		if (trans_byte != byte_count) {
			DBG_CRIT("dps read error...\r\n");

			return 1;
		}
	}

	for (u8 i = 0; i < byte_count; i++) {
		*(data + i) = rbuf[i];
	}

	return 0;
}

u8 dps_init(void) {
  u8 sts = 0;
	u8 mfr_model[8] = { 0 };

	dps_i2c_sts = 0;
	for (u8 idx = 0; idx < MAX_DPS_I2C; idx++) {
		sts = i2c_dps_rd(idx, LTC3884_REG_MFR_MDL, DPS_MFR_BYTE, mfr_model);
		if ((sts != 0) || (strncmp((char *)mfr_model, LTC3884_MFR_MODEL, strlen(LTC3884_MFR_MODEL)) == 0)) {
			dps_i2c_sts |= (0x1 << idx);
		}
	}

  return sts;
}

u8 os_dps_ctrl(void) {
  u8 sts = 0;

	u8 cmd = (*P_DPS_CMD) & DPS_CMD_SET;

	if (cmd != 0) {
		updateFlag = 1;
		
		u8  data[256] = { 0 };
		u8  idx  = ((*P_DPS_CMD)  >> 16) & 0xFF;
		u8  byte = (((*P_DPS_CMD) >>  8) + 1) & 0xFF;
		u32 pcmd = ((*P_DPS_CMD)  >>  0) & 0xFF;
		
		if ((*P_DPS_CMD) & DPS_CMD_WR) {
			memcpy(data, (u8 *)P_DPS_BUF, byte);
			sts = i2c_dps_wr(idx, pcmd, byte, data);
		} else if ((*P_DPS_CMD) & DPS_CMD_RD) {
			sts = i2c_dps_rd(idx, pcmd, byte, data);
			memcpy((u8 *)P_DPS_BUF, data, byte);
		} else {
			sts = 1;
		}

		if (sts == 0) {
			*P_DPS_STS = DPS_STS_EOS;

			if (pcmd == LTC3884_REG_VOUT_CMD) {
				// dps_get_val(idx, )
			}
		} else {
			*P_DPS_STS = DPS_STS_ERR;
		}
	}

  return sts;
}

u8 dps_get_page(u8 idx, u8 *page) {
	u8 sts = 0;
	*page = 0xFF;
	
	sts = i2c_dps_rd(idx, LTC3884_REG_PAGE, DPS_PAGE_BYTE, page);

	return sts;
}

u8 dps_set_page(u8 idx, u8 page) {
	u8 sts = 0;
	
	sts = i2c_dps_wr(idx, LTC3884_REG_PAGE, DPS_PAGE_BYTE, &page);

	return sts;
}

u8 dps_get_val(u8 idx, u8 page, u16 *wsts, u16 *iout, u16* vout) {
	u8 sts = 0;

	*wsts = 0;
	*iout = 0;
	*vout = 0;

	if (idx >= MAX_DPS_I2C) return 1;
	if (page >= MAX_DPS_PAGE) return 1;
	if (dps_i2c_sts & (1 << idx)) return 1;
	
	sts = i2c_dps_wr(idx, LTC3884_REG_PAGE, DPS_PAGE_BYTE, &page);
	if (sts == 0) {
		sts += i2c_dps_rd(idx, LTC3884_REG_STS_WORD,  DPS_REG_BYTE, (u8 *)wsts);
		sts += i2c_dps_rd(idx, LTC3884_REG_READ_IOUT, DPS_REG_BYTE, (u8 *)iout);
		sts += i2c_dps_rd(idx, LTC3884_REG_READ_VOUT, DPS_REG_BYTE, (u8 *)vout);
	}
	
	return sts;
}
