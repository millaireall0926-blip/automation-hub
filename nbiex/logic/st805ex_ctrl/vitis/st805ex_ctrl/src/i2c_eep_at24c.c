
#include "define.h"


struct _I2C_TARGET eep_i2c[] = {
	{ XPAR_AXI_IIC_EEP_BASEADDR, 0x50, "AT24CM01_U49", "CTRL_EEPROM" }
};

void eep_i2c_reset(void) {
	XIic_WriteReg(XPAR_AXI_IIC_EEP_BASEADDR, XIIC_RESETR_OFFSET, XIIC_RESET_MASK);
}

u8 i2c_eep_wr(u32 addr, u8 count, u8* data) {
	u8 trans_byte = 0;
	u8 byte_count = count + AT24C_DATA_IDX;
	u8 wbuf[AT24C_PAGE_SIZE+AT24C_DATA_IDX];
	u8 rbuf[4];
	u8 sa_lsb = (addr >> 16) & 0x1; // for M01 byte EEPROM

	if (count > AT24C_PAGE_SIZE) {
		return 1;
	}

	if (eep_i2c_sts) {
		return 1;
	}

	memset(wbuf, 0, sizeof(wbuf));
	memset(rbuf, 0, sizeof(rbuf));
	
#ifdef _SIM_
	return 0;
#endif

	wbuf[0] = (addr >> 8) & 0x7F;
	wbuf[1] = (addr >> 0) & 0xFF;

	for (u8 i = 0; i < byte_count; i++) {
		wbuf[AT24C_DATA_IDX + i] = *(data + i);
	}

	trans_byte = i2c_send_data(eep_i2c[0].base, eep_i2c[0].slave_addr + sa_lsb, wbuf, byte_count, XIIC_STOP);
	
	if (trans_byte != byte_count) {
		DBG_CRIT("eep write error...\r\n");
		
		i2c_recv(eep_i2c[0].base, rbuf, 4, XIIC_STOP);

		eep_i2c_reset();
		usleep(1000);

		trans_byte = i2c_send_data(eep_i2c[0].base, eep_i2c[0].slave_addr + sa_lsb, wbuf, byte_count, XIIC_STOP);

		if (trans_byte != byte_count) {
			DBG_CRIT("eep write error...\r\n");

			return 1;
		}
	}

	usleep(AT24C_WR_CYCLE_MS * 1000);

	return 0;
}

u8 i2c_eep_rd(u32 addr, u8 count, u8* data) {
	u8 trans_byte = 0;
	u8 byte_count = count;
	u8 wbuf[AT24C_DATA_IDX];
	u8 rbuf[AT24C_PAGE_SIZE];
	u8 sa_lsb = (addr >> 16) & 0x1; // for M01 byte EEPROM

	memset(wbuf, 0, AT24C_DATA_IDX);
	memset(rbuf, 0, AT24C_PAGE_SIZE);
	memset(data, 0, byte_count);

	if (count > AT24C_PAGE_SIZE) {
		return 1;
	}

	if (eep_i2c_sts) {
		return 1;
	}
	
#ifdef _SIM_
	for (u8 i = 0; i < byte_count; i++) {
		*(data + i) = addr + i;
	}
	return 0;
#endif

	wbuf[0] = (addr >> 8) & 0x7F;
	wbuf[1] = (addr >> 0) & 0xFF;

	trans_byte = i2c_recv_data(eep_i2c[0].base, eep_i2c[0].slave_addr + sa_lsb, wbuf, AT24C_DATA_IDX, rbuf, byte_count, XIIC_STOP);
	
	if (trans_byte != byte_count) {
		DBG_CRIT("eep read error...\r\n");
		
		i2c_recv(eep_i2c[0].base, rbuf, 4, XIIC_STOP);

		eep_i2c_reset();
		usleep(1000);

		memset(rbuf, 0, AT24C_PAGE_SIZE);
		trans_byte = i2c_recv_data(eep_i2c[0].base, eep_i2c[0].slave_addr + sa_lsb, wbuf, AT24C_DATA_IDX, rbuf, byte_count, XIIC_STOP);

		if (trans_byte != byte_count) {
			DBG_CRIT("eep read error...\r\n");

			return 1;
		}
	}

	for (u8 i = 0; i < byte_count; i++) {
		*(data + i) = rbuf[i];
	}
	
	return 0;
}

u8 eep_init(void) {
	u8 sts = 0;
	u8 wbuf[AT24C_DUMMY_BYTE];
	u8 rbuf[AT24C_DUMMY_BYTE];

	eep_i2c_sts = 0;
	memset(wbuf, 0, AT24C_DUMMY_BYTE);
	memset(rbuf, 0, AT24C_DUMMY_BYTE);

	for (u8 i = 0; i < AT24C_DUMMY_BYTE; i++) {
		wbuf[i] = i + 1;
	}
	sts = i2c_eep_wr(AT24C_DUMMY_ADDR, AT24C_DUMMY_BYTE, wbuf);
	sts = i2c_eep_rd(AT24C_DUMMY_ADDR, AT24C_DUMMY_BYTE, rbuf);

	for (u8 i = 0; i < AT24C_DUMMY_BYTE; i++) {
		if (wbuf[i] != rbuf[i]) {
			sts++;
		}
	}

	if (sts != 0) {
		eep_i2c_sts = 1;
	}

	memset(wbuf, 0, AT24C_DUMMY_BYTE);
	sts = i2c_eep_wr(AT24C_DUMMY_ADDR, AT24C_DUMMY_BYTE, wbuf);

	return eep_i2c_sts;
}

u8 os_eep_ctrl(void) {
	u8 sts = 0;
	u8 cmd = (*P_EEP_CMD) & EEP_CMD_SET;

	if (cmd != 0) {
		u8 data[64] = { 0 };
		u32 addr = ((*P_EEP_CMD) >> 8) & 0x1FFFF;
		u8  byte = ((*P_EEP_CMD)  + 1) & 0xFF;

		if (byte > AT24C_PAGE_SIZE) {
			sts = 1;
			*P_EEP_STS = EEP_STS_ERR;
			return sts;
		}
		
		if ((*P_EEP_CMD) & EEP_CMD_WR) {
			memcpy(data, (u8 *)P_EEP_BUF, byte);
			sts = i2c_eep_wr(addr, byte, data);
		} else if ((*P_EEP_CMD) & EEP_CMD_RD) {
			sts = i2c_eep_rd(addr, byte, data);
			memcpy((u8 *)P_EEP_BUF, data, byte);
		} else {
			sts = 1;
		}

		if (sts == 0) {
			*P_EEP_STS = EEP_STS_EOS;
		} else {
			*P_EEP_STS = EEP_STS_ERR;
		}
	}

	return sts;
}