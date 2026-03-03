
#include "define.h"


XSpi spiCfgProm;

u8 prom_init(void) {
	u8 sts = 0;

	XSpi_Config *ConfigPromPtr;

#ifndef SDT
	ConfigPromPtr = XSpi_LookupConfig(SPI_PROM_DEVICE_ID);
#else
	ConfigPromPtr = XSpi_LookupConfig(XPAR_XSPI_0_BASEADDR);
#endif
	if (ConfigPromPtr == NULL) {
		return XST_DEVICE_NOT_FOUND;
	}

	sts = XSpi_CfgInitialize(&spiCfgProm, ConfigPromPtr, ConfigPromPtr->BaseAddress);
	if (sts != XST_SUCCESS) {
		return XST_FAILURE;
	}

	sts = XSpi_SetOptions(&spiCfgProm, XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION);
	if (sts != XST_SUCCESS) {
		return XST_FAILURE;
	}

	sts = XSpi_SetSlaveSelect(&spiCfgProm, SPI_SELECT);
	if(sts != XST_SUCCESS) {
		return XST_FAILURE;
	}

	sts = XSpi_Start(&spiCfgProm);
	if(sts != XST_SUCCESS) {
		return XST_FAILURE;
	}
	
	XSpi_IntrGlobalDisable(&spiCfgProm);
	
	return XST_SUCCESS;
}

u8 os_prom_ctrl(void) {
	u8 sts = 0;

	u8 cmd = (*P_PROM_CMD) & 0xC1;

	if (cmd != 0) {
		*P_PROM_STS = PROM_STS_BUSY;

		u32 addr = (*P_PROM_ADDR) & 0xFFFFFF;
		u16 byte = (*P_PROM_BYTE) + 1;

		if (cmd == PROM_CMD_RD) {
			*P_PROM_CMD = 0;

			memset(spiRdBuf, 0, sizeof(spiRdBuf));

			sts = SpiFlashRead(&spiCfgProm, addr, byte, COMMAND_QUAD_READ);
			if (sts != XST_SUCCESS) {
				*P_PROM_STS = PROM_STS_ERR;
			} else {
				for(u8 idx = 0; idx < byte; idx++) {
					*(P_PROM_BUF + idx) = spiRdBuf[idx + READ_WRITE_EXTRA_BYTES + QUAD_READ_DUMMY_BYTES];
				}

				*P_PROM_STS = PROM_STS_EOS;
			}
		} else if (cmd == PROM_CMD_WR) {
			*P_PROM_CMD = 0;

			for(u8 idx = READ_WRITE_EXTRA_BYTES; idx < (byte + READ_WRITE_EXTRA_BYTES); idx++) {
				spiWrBuf[idx] = *(P_PROM_BUF + idx - READ_WRITE_EXTRA_BYTES);
			}

			sts = SpiFlashWriteEnable(&spiCfgProm);
			if (sts != XST_SUCCESS) {
				*P_PROM_STS = PROM_STS_ERR;
			}

			sts = SpiFlashWrite(&spiCfgProm, addr, byte, COMMAND_QUAD_WRITE);
			if (sts != XST_SUCCESS) {
				*P_PROM_STS = PROM_STS_ERR;
			} else {
				*P_PROM_STS = PROM_STS_EOS;
			}
		} else if (cmd == PROM_CMD_ERASE) {
			*P_PROM_CMD = 0;

			sts = SpiFlashWriteEnable(&spiCfgProm);
			if (sts != XST_SUCCESS) {
				*P_PROM_STS = PROM_STS_ERR;
			}

			sts = SpiFlashSectorErase(&spiCfgProm, addr);
			if (sts != XST_SUCCESS) {
				*P_PROM_STS = PROM_STS_ERR;
			} else {
				*P_PROM_STS = PROM_STS_EOS;
			}
		} else {
			*P_PROM_CMD = 0;
			*P_PROM_STS = PROM_STS_ERR;
		}
	}
	
	return sts;
}
