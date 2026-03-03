
#include "ioc.h"

#include <sys/mman.h>
#include "define.h"


#define SIM_RET if(m_bSim)return RTN_OK;
#define SIM if(m_bSim)return;

/* pcie */

int ioc::open_pcie(void) {
	SIM_RET

	if ((pcie_fd = open("/dev/xdma0_bypass", O_RDWR | O_SYNC)) == -1) {
		DBG_CRIT("\t Can't open PCIe[ %s ] !!!\n", "/dev/xdma0_bypass");
		return RTN_ERR;
	}
	pcie_in_progress = 1;
	printf("PCIe %s opened.\n", "/dev/xdma0_bypass");

	pcie_map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, pcie_fd, MAP_ADDR);
	if (pcie_map_base == (byte*)-1) {
		DBG_CRIT("\t Can't mmap PCIe !!!\n");
		return RTN_ERR;
	}

	printf("Memory mapped at address %p.\n", pcie_map_base);

	m_pBaseAdr = (byte*)((byte*)pcie_map_base);

	P_PJT_BASE = (byte*)((byte*)pcie_map_base + PJT_BASE);
	P_BRD_BASE = (byte*)((byte*)pcie_map_base + BRD_BASE);
	P_CFG_BASE = (byte*)((byte*)pcie_map_base + CFG_BASE);
	P_C2P_BASE = (byte*)((byte*)pcie_map_base + C2P_BASE);	
	P_ADC_BASE = (byte*)((byte*)pcie_map_base + ADC_BASE);
	P_DAC_BASE = (byte*)((byte*)pcie_map_base + DAC_BASE);
	P_DPS_BASE = (byte*)((byte*)pcie_map_base + DPS_BASE);
	

	P_BRD_TPCB_REV = (byte*)((byte*)pcie_map_base + BRD_BASE + 0x20);
	P_BRD_SLOT_ID = (byte*)((byte*)pcie_map_base + BRD_BASE + 0x24);
	P_BRD_BIB_CHECK = (byte*)((byte*)pcie_map_base + BRD_BASE + 0x30);
	P_BRD_BIBID_VAL = (uint*)((byte*)pcie_map_base + BRD_BASE + 0x3C);

	P_CFG_BUF = (byte*)((byte*)pcie_map_base + CFG_BUF_BASE);
	P_CFG_CMD = (uint*)((byte*)pcie_map_base + CFG_BASE + 0x20);
	P_CFG_STS = (byte*)((byte*)pcie_map_base + CFG_BASE + 0x30);
	

	P_C2P_BUF = (byte*)((byte*)pcie_map_base + C2P_BUF_BASE);
	P_C2P_ADDR = (uint*)((byte*)pcie_map_base + C2P_BASE + 0x20);
	P_C2P_CMD = (uint*)((byte*)pcie_map_base + C2P_BASE + 0x24);
	P_C2P_RL = (byte*)((byte*)pcie_map_base + C2P_BASE + 0x2C);
	P_C2P_STS = (byte*)((byte*)pcie_map_base + C2P_BASE + 0x30);

	P_ADC_SETTL = (uint32_t*)((uint8_t*)pcie_map_base + ADC_BASE + 0x28);
	P_ADC_DATA = (short*)((byte*)pcie_map_base + ADC_BASE + 0x100);
	
	P_DAC_CMD = (uint*)((byte*)pcie_map_base + DAC_BASE + 0x20);	
	P_DAC_STS = (uint*)((byte*)pcie_map_base + DAC_BASE + 0x30);
	P_DAC_LDAC = (byte*)((byte*)pcie_map_base + DAC_BASE + 0x2C);
	
	P_DPS_CMD = (uint*)((byte*)pcie_map_base + DPS_BASE + 0x20);
	P_DPS_RUN = (word*)((byte*)pcie_map_base + DPS_BASE + 0x28);
	P_DPS_HV = (byte*)((byte*)pcie_map_base + DPS_BASE + 0x2A);
	P_DPS_VREF = (byte*)((byte*)pcie_map_base + DPS_BASE + 0x2F);	
	P_DPS_STS = (uint*)((byte*)pcie_map_base + DPS_BASE + 0x30);
	P_DPS_FAULT = (uint*)((byte*)pcie_map_base + DPS_BASE + 0x38);
	P_DPS_DATA = (byte*)((byte*)pcie_map_base + DPS_BASE + 0x40);

	P_ALLST_BUF = (byte*)((byte*)pcie_map_base + ALL_STS_BASE);

	P_EPR_BASE = (byte*)((byte*)pcie_map_base + EPR_BASE);
	P_EPR_CMD = (uint*)((byte*)pcie_map_base + EPR_BASE + 0x20);
	P_EPR_STS = (uint*)((byte*)pcie_map_base + EPR_BASE + 0x30);
	P_EPR_BUF = (byte*)((byte*)pcie_map_base + EPR_BASE + 0x40);


	DBG_INFO("\n");
	DBG_INFO("P_PJT_BASE    : %p\n", P_PJT_BASE);
	DBG_INFO("P_BRD_BASE    : %p\n", P_BRD_BASE);
	DBG_INFO("P_CFG_BASE    : %p\n", P_CFG_BASE);
	DBG_INFO("P_C2P_BASE    : %p\n", P_C2P_BASE);	
	DBG_INFO("P_ADC_BASE    : %p\n", P_ADC_BASE);
	DBG_INFO("P_DAC_BASE    : %p\n", P_DAC_BASE);
	DBG_INFO("P_DPS_BASE    : %p\n", P_DPS_BASE);	
	DBG_INFO("P_EPR_BASE    : %p\n", P_EPR_BASE);

	DBG_INFO("\n");
	DBG_INFO("P_BRD_TPCB_REV   : %p\n",		P_BRD_TPCB_REV);
	DBG_INFO("P_BRD_SLOT_ID  : %p\n",		P_BRD_SLOT_ID);
	DBG_INFO("P_BRD_BIB_CHECK   : %p\n",	P_BRD_BIB_CHECK);
	DBG_INFO("P_BRD_BIBID_VAL  : %p\n",		P_BRD_BIBID_VAL);

	DBG_INFO("\n");
	DBG_INFO("P_CFG_BUF     : %p\n", P_CFG_BUF);
	DBG_INFO("P_CFG_CMD     : %p\n", P_CFG_CMD);
	DBG_INFO("P_CFG_STS     : %p\n", P_CFG_STS);	

	DBG_INFO("\n");
	DBG_INFO("P_C2P_BUF     : %p\n", P_C2P_BUF);
	DBG_INFO("P_C2P_ADDR    : %p\n", P_C2P_ADDR);
	DBG_INFO("P_C2P_CMD     : %p\n", P_C2P_CMD);
	DBG_INFO("P_C2P_RL      : %p\n", P_C2P_RL);
	DBG_INFO("P_C2P_STS     : %p\n", P_C2P_STS);

	DBG_INFO("\n");	
	DBG_INFO("P_ADC_DATA    : %p\n", P_ADC_DATA);

	DBG_INFO("\n");
	DBG_INFO("P_DAC_CMD     : %p\n", P_DAC_CMD);	
	DBG_INFO("P_DAC_STS     : %p\n", P_DAC_STS);
	
	DBG_INFO("\n");
	DBG_INFO("P_DPS_CMD     : %p\n", P_DPS_CMD);
	DBG_INFO("P_DPS_RUN     : %p\n", P_DPS_RUN);
	DBG_INFO("P_DPS_HV     : %p\n", P_DPS_HV);
	DBG_INFO("P_DPS_VREF     : %p\n", P_DPS_VREF);
	DBG_INFO("P_DPS_STS     : %p\n", P_DPS_STS);
	DBG_INFO("P_DPS_FAULT     : %p\n", P_DPS_FAULT);
	DBG_INFO("P_DPS_DATA    : %p\n", P_DPS_DATA);
	
	DBG_INFO("\n");
	DBG_INFO("P_EPR_CMD     : %p\n",	P_EPR_CMD);
	DBG_INFO("P_EPR_STS     : %p\n",	P_EPR_STS);
	DBG_INFO("P_EPR_BUF	    : %p\n",	P_EPR_BUF);

	
	m_nBFPGA = FPGA_DEFAULT;
	m_sIOErrVec.clear();

	return pcie_fd;
}

void ioc::close_pcie(void) {
	if (pcie_in_progress == 0)
		return;

	if (munmap(pcie_map_base, MAP_SIZE) == -1) {
		DBG_CRIT("\t Can't munmap PCIe !!!\n");
		return;
	}

	pcie_in_progress = 0;
	close(pcie_fd);
}


void ioc::print_ip_info(void* base) {
	SIM;
	P_IP_INFO in_info = (P_IP_INFO)base;

	printf("**** %16s Rev.%d.%02d @ 20%02d.%02d.%02d ****\r\n", in_info->IpName, in_info->MajorRev, in_info->MinorRev, in_info->Year, in_info->Month, in_info->Date);
}

void ioc::printf_all_ip(void) {
	print_ip_info((void*)P_PJT_BASE);
	print_ip_info((void*)P_BRD_BASE);
	print_ip_info((void*)P_CFG_BASE);
	print_ip_info((void*)P_C2P_BASE);	
	print_ip_info((void*)P_ADC_BASE);
	print_ip_info((void*)P_DAC_BASE);
	print_ip_info((void*)P_DPS_BASE);	
}

uint ioc::check_bib() {
	SIM_RET		
	return (uint)(*P_BRD_BIB_CHECK);
}


void ioc::brd_info(void) {
	SIM
	printf("\t BIB-CHECK : %s [ %d ]\n", ((*P_BRD_BIB_CHECK) != 0) ? "INSERTED" : "not INSERTED", (*P_BRD_BIB_CHECK) & 0xf);
	printf("\t SITE-ID : %02X\n",  ((*P_BRD_SLOT_ID) & 0xff));
	printf("\t BIB-ID : %02X\n", ((*P_BRD_BIBID_VAL)));

}





uint ioc::readBID(void)
{
	brd_info();	
	return (*P_BRD_BIBID_VAL);
}



int ioc::readSiteID(void)
{
	return (int)*P_BRD_SLOT_ID;
}


/* cfg */
void ioc::cfg_reset(void) {
	SIM;
	*P_CFG_CMD = CFG_CMD_RST;

	Sleep(0.1);
}

byte ioc::cfg_wait_eos(void) {
	SIM_RET
	// tCK : 30ns
	// max : 2ms for 64KB transfer
	uint timeout = CFG_TIMEOUT;

#ifdef _SIM_
	* P_CFG_STS = CFG_STS_EOS;
#endif
	while ((((*P_CFG_STS) & CFG_STS_EOS) == 0x0) && (--timeout > 0)) {
		if (((*P_CFG_STS) & CFG_STS_ERR) != 0x0) {
			printf( "cfg.err : expect[ %02X ] - sts[ %02X ] !!!\r\n", CFG_STS_EOS, *P_CFG_STS);
			return RTN_ERR;
		}
//		usleep(1000);  // 1MS Sleep
	}

	if (timeout == 0) {
		printf( "eos.timeout : expect[ %02X ] - sts[ %02X ] !!!\r\n", CFG_STS_EOS, *P_CFG_STS);
		cfg_reset();
		return RTN_TIMEOUT;
	}

	return RTN_OK;
}

byte ioc::cfg_wait_busy(void) {
	SIM_RET
	// tCK : 30ns
	// max : 2ms for 64KB transfer
	uint timeout = CFG_TIMEOUT;

	while ((((*P_CFG_STS) & CFG_STS_BUSY) == CFG_STS_BUSY) && (--timeout > 0)) {
		if (((*P_CFG_STS) & CFG_STS_ERR) != 0x0) {
			printf( "cfg.err : expect[ %02X ] - sts[ %02X ] !!!\r\n", CFG_STS_EOS, *P_CFG_STS);
			return RTN_ERR;
		}
	}

	if (timeout == 0) {
		printf( "eos.timeout : expect[ %02X ] - sts[ %02X ] !!!\r\n", CFG_STS_EOS, *P_CFG_STS);


		cfg_reset();
		Sleep(0.1);

		return RTN_TIMEOUT;
	}

	return RTN_OK;
}

int ioc::cfg_fpga(const char* bitstream) {
	SIM_RET
	int retryCnt = 5;
	byte rv = 0;

	for (int i = 0; i < retryCnt; i++) {
		FILE* f = NULL;
		if ((f = fopen(bitstream, "rb")) == NULL) {
			DBG_CRIT("bitstream \"%s\" open error!\n", bitstream);
			return RTN_ARGC_ERR;
		}

		fseek(f, 0, SEEK_END);
		uint fileSize = ftell(f);
		fseek(f, 0, SEEK_SET);

		DBG_INFO("\t bitstream[ %s ] size[ %7.3f MByte ]\n", bitstream, fileSize / 1024.0 / 1024.0);

		//DBG_INFO("\t Cfg : prev.sts[ %02X ]\n", *P_CFG_STS);

		byte* rbuf = new byte[MAX_CFG_SIZE + 10];
		memset(rbuf, 0, MAX_CFG_SIZE + 10);
		while (fileSize > 0) {
			byte last = 0;
			uint transSize = (fileSize > MAX_CFG_SIZE) ? MAX_CFG_SIZE : fileSize;

			if (transSize < MAX_CFG_SIZE)
				last = 1;

			if (fread(rbuf, transSize, 1, f) != 1) {
				rv = RTN_ERR;
				break;
			}

			//memcpy((byte *)P_CFG_BUF, rbuf, transSize);
			for (uint i = 0; i < transSize; i++) {
				*(P_CFG_BUF + i) = rbuf[i];
			}
//			printf("\t Cfg : last[ %d ] fileSize[ %d ] transSize[ %d ] 1st.data[ %02X ] last.data[ %02X ]\n", last, fileSize, transSize, *(P_CFG_BUF), *(P_CFG_BUF + transSize - 1));

			//*(P_CFG_CMD + 2) = (sel >> 4);
			*P_CFG_CMD = (1 << (CFG_LOC_START + last)) | 1<< CFG_LOC_PSEL | (transSize - 1);
			//DBG_INFO("\t Cfg : cmd[ %08X ] sel[ %08X ]\n", *P_CFG_CMD, *(P_CFG_CMD + 2));

			if (last == 1)
				rv = cfg_wait_eos();
			else
				rv = cfg_wait_busy();

			if (rv != RTN_OK)
				break;

			fileSize -= transSize;
		}

		delete[] rbuf;
		fclose(f);

		if (rv != RTN_OK) {
			Sleep(0.5);
			DBG_CRIT("Config fail times : [ %d / %d ]\n", i + 1, retryCnt);
		}
		else {
			printf("FPGA Upload OK %s\n", bitstream);
			break;
		}
	}

	return rv;
}

FUNC_RTN ioc::cfgi_check_wait(bool bLast)
{
	uint stat = readPG(PGI_CFG_CTRL_STAT) & 0xff;
	if (stat & CFG_STS_ERR)return RTN_ERR;

	if (bLast)
		return ((stat & CFG_STS_EOS) == 0) ? RTN_WAIT : RTN_OK;
	return ((stat & CFG_STS_BUSY) != 0) ? RTN_WAIT : RTN_OK;
}

FUNC_RTN ioc::cfgi_wait(bool bLast)
{
	uint timeout = CFG_TIMEOUT / 5; // CFGI More Slow then CFG Direct
	FUNC_RTN ret = RTN_WAIT;
	for (uint t = 0 ; t < timeout; t++)
	{
		ret = cfgi_check_wait(bLast);
		if (ret == RTN_ERR) {
			printf("%s Error State[ %02X ] !!!\r\n", __FUNCTION__, readPG(PGI_CFG_CTRL_STAT) & 0xff);
			return RTN_ERR;
		}
		else if (ret == RTN_OK) break;
	}

	if (ret == RTN_WAIT) {
		printf("%s TimeOut State[ %02X ] !!!\r\n", __FUNCTION__, readPG(PGI_CFG_CTRL_STAT) & 0xff);
		writePGD(PGI_CFG_CTRL_CMD, BIT_CFG_RESET);
		return RTN_TIMEOUT;
	}

	return RTN_OK;
}

int ioc::cfgi_fpga(const char* bitstream, uint sel)
{
	int retryCnt = 5;
	byte rv = 0;

	for (int i = 0; i < retryCnt; i++) {
		FILE* f = NULL;
		if ((f = fopen(bitstream, "rb")) == NULL) {
			DBG_CRIT("bitstream \"%s\" open error!\n", bitstream);
			return RTN_ARGC_ERR;
		}

		fseek(f, 0, SEEK_END);
		uint fileSize = ftell(f);
		fseek(f, 0, SEEK_SET);

		DBG_INFO("\t bitstream[ %s ] size[ %7.3f MByte ]\n", bitstream, fileSize / 1024.0 / 1024.0);

		//DBG_INFO("\t Cfg : prev.sts[ %02X ]\n", *P_CFG_STS);

		byte* rbuf = new byte[MAX_CFG_SIZE + 10];
		memset(rbuf, 0, MAX_CFG_SIZE + 10);
		while (fileSize > 0) {
			bool bLast = false;
			uint transSize = (fileSize > MAX_CFG_SIZE) ? MAX_CFG_SIZE : fileSize;

			if (transSize < MAX_CFG_SIZE)
				bLast = true;

			if (fread(rbuf, transSize, 1, f) != 1) {
				rv = RTN_ERR;
				break;
			}

			writePGDataD(PGI_CFG_BUF, rbuf, transSize);

			uint cmd = (1 << (CFG_LOC_START + (bLast?1:0))) | (sel << 20) | (transSize - 1);
			writePGD(PGI_CFG_CTRL_CMD, cmd);

			rv = cfgi_wait(bLast);
			if (rv != RTN_OK)
				break;

			fileSize -= transSize;
		}

		delete[] rbuf;
		fclose(f);

		if (rv != RTN_OK) {
			Sleep(0.1);
			DBG_CRIT("CFGI Config fail times : [ %d / %d ]\n", i + 1, retryCnt);
		}
		else {
			printf("CFGI FPGA Upload OK %s\n", bitstream);
			break;
		}
	}

	return rv;
}




/* lb */
void ioc::c2p_reset(void) {
	SIM;

	*P_C2P_CMD = LB_CMD_RST;
	
}

byte ioc::c2p_wait_eos(byte dev_no) {
	SIM_RET
	// wr : 360ns + BL * 10ns
	// rd : 460ns + BL * 10ns
	// max : 656us for 64KB transfer
	uint timeout = LB_TIMEOUT;

#ifdef _SIM_
	* P_C2P_STS = LB_STS_EOS;
#endif

	while ((((*(P_C2P_STS + dev_no)) & LB_STS_EOS) == 0x0) && (--timeout > 0)) {
		if (((*(P_C2P_STS + dev_no)) & LB_STS_ERR) != 0x0) {
			printf( "com.err : expect[ %02X ] - sts[ %02X ] !!!\r\n", LB_STS_EOS, *(P_C2P_STS + dev_no));
			return RTN_ERR;
		}
	}

	if (timeout == 0) {
		printf( "eos.timeout : expect[ %02X ] - sts[ %02X ] !!!\r\n", LB_STS_EOS, *(P_C2P_STS + dev_no));
		printf( "eos.timeout : expect[ %02X] - sts[ %02X] !!!\r\n", LB_STS_EOS, *(P_C2P_STS + dev_no));
		c2p_reset();
		Sleep(0.1);

		return RTN_TIMEOUT;
	}


	return RTN_OK;
}

byte ioc::c2p_write(byte dev_no, byte ddr, byte ddr_msb, uint addr, short bl, byte* wbuf) {
	SIM_RET
	for (short i = 0; i < (bl); i++) {
//		printf("idx%d: adr %08x val %02x\n",i, P_C2P_BUF + i, wbuf[i]);
		*(P_C2P_BUF + i) = wbuf[i];
	}
 
	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_WR | (0x1 << (16 + dev_no)) | (bl-1);
//	printf("write adr %08x cmd %02x\n",addr, *P_C2P_CMD);
	return c2p_wait_eos(dev_no);
}

byte ioc::c2p_read(byte dev_no, byte ddr, byte ddr_msb, uint addr, short bl, byte* rbuf) {
	SIM_RET
 
	* P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_RD | (0x1 << (16 + dev_no)) | (bl-1);
//	printf("read adr %08x cmd %02x\n",addr, *P_C2P_CMD);
	byte rv = 0;
	rv = c2p_wait_eos(dev_no);
	if (rv != RTN_OK)
		return rv;

	//memcpy(rbuf, (byte *)P_C2P_BUF, bl + 1);
	for (short i = 0; i < (bl); i++) {
//		printf("idx%d: adr %08x val %02x\n",i, P_C2P_BUF + i, *(P_C2P_BUF + i));
		rbuf[i] = *(P_C2P_BUF + i);
	}

	return RTN_OK;
}

byte ioc::c2p_wb(byte dev_no, uint addr, byte wdata) {
	SIM_RET
	short bl = 0;

	*P_C2P_BUF = wdata;
	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_WR | (0x1 << (16 + dev_no)) | bl;

	return c2p_wait_eos(dev_no);
}

byte ioc::c2p_wh(byte dev_no, uint addr, short wdata) {
	SIM_RET
	short bl = 1;

	*((short*)P_C2P_BUF) = wdata;
	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_WR | (0x1 << (16 + dev_no)) | bl;

	return c2p_wait_eos(dev_no);
}

byte ioc::c2p_ww(byte dev_no, uint addr, uint wdata, int sz) {
	SIM_RET
	short bl = (sz);

	*((uint*)P_C2P_BUF) = wdata;
	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_WR | (0x1 << (16 + dev_no)) | bl;

	return c2p_wait_eos(dev_no);
}

byte ioc::c2p_rb(byte dev_no, uint addr, byte* rdata) {
	SIM_RET
	short bl = 0;

	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_RD | (0x1 << (16 + dev_no)) | bl;

	byte rv = 0;
	rv = c2p_wait_eos(dev_no);
	if (rv != RTN_OK)
		return rv;

	*rdata = *P_C2P_BUF;

	return RTN_OK;
}

byte ioc::c2p_rh(byte dev_no, uint addr, short* rdata) {
	SIM_RET
	short bl = 1;

	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_RD | (0x1 << (16 + dev_no)) | bl;

	byte rv = 0;
	rv = c2p_wait_eos(dev_no);
	if (rv != RTN_OK)
		return rv;

	*rdata = *((short*)P_C2P_BUF);

	return RTN_OK;
}

byte ioc::c2p_rw(byte dev_no, uint addr, uint* rdata, int sz) {
	SIM_RET
	short bl = (sz);

	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_RD | (0x1 << (16 + dev_no)) | bl;

	byte rv = 0;
	
	rv = c2p_wait_eos(dev_no);
	if (rv != RTN_OK)
		return rv;

	*rdata = *((uint*)P_C2P_BUF);

	return RTN_OK;
}



void ioc::adc_settling(uint nsec) {
	SIM;
	*P_ADC_SETTL = (int)(nsec / 10);
}


///////////////////////////
/* DAC Function			*/
///////////////////////////
void ioc::dac_reset(void) {
	SIM;
	*P_DAC_CMD = DAC_CMD_RST;

	Sleep(0.1);
}


byte ioc::dac_wait_eos(void) {
	// dac : 400KHz I2C
	// tSHIFT : 20us * ( N + 3 )Byte = 120us
	uint timeout = DAC_TIMEOUT;

#ifdef _SIM_
	* P_DAC_STS = DAC_STS_EOS;
#endif

	while ((((*P_DAC_STS) & DAC_STS_EOS) == 0x0) && (--timeout > 0)) {
		if (((*P_DAC_STS) & DAC_STS_ERR) != 0x0) {
			printf( "i2c.err : expect[ %08X ] - sts[ %08X ] !!!\r\n", DAC_STS_EOS, *P_DAC_STS);
			return RTN_ERR;
		}
	}

	if (timeout == 0) {
		printf( "eos.timeout : expect[ %08X ] - sts[ %08X ] !!!\r\n", DAC_STS_EOS, *P_DAC_STS);
		DBG_INFO(" - after timeout[ %08X ]", *P_DAC_CMD);		
		dac_reset();
		Sleep(0.1);

		return RTN_TIMEOUT;
	}

	return RTN_OK;
}

byte ioc::dac_set(uint ch, word val) {
	SIM_RET

	uint index = (ch / 16) << 24;	
	uint cmd = 0x3 << 20;
	uint adr = (ch % 16) << 16;

	// ADC Resolution : 16bit
	//word wdata = (word)((val * 40.0)/65536.0); 

	*P_DAC_CMD =  (DAC_CMD_SET | index | cmd | adr | (val & 0xFFFF) );
	DBG_INFO(" - post[ %08X ]", *P_DAC_CMD);

	return dac_wait_eos();
}

byte ioc::dac_enable(int type, bool isOn) {
	SIM_RET
	
	uint cmd = 0x4 << 20;
	uint adr = 0 << 19;
	if (type == 33) { // 33 = VH_ALL		
		uint index = 0x0 << 24;		
		uint val = 0x5555;
		if (isOn) val = 0x0;
		*P_DAC_CMD = (DAC_CMD_SET | index | cmd | adr | val); // dac 0~7 Set
		adr = 0x1 << 19;
		*P_DAC_CMD = (DAC_CMD_SET | index | cmd | adr | val); // dac 8~15 Set
	}
	else if (type == 34) { // 34 = VTH_ALL
		uint index = 0x1 << 24;		
		uint val = 0x55;
		if (isOn) val = 0x0;
		*P_DAC_CMD = (DAC_CMD_SET | index | cmd | adr | val); // dac 0~7 Set
	}
	else {
		return RTN_ARGC_ERR;
	}

	return RTN_OK;
}



byte ioc::dac_ldac(byte no, bool isOn) {
	SIM_RET
	byte prev = *P_DAC_LDAC;
	if (isOn)
		*P_DAC_LDAC = prev | (no & 0x7);
	else
		*P_DAC_LDAC = prev & (~(no & 0x7));
	
	return RTN_OK;
}



///////////////////////////
/* DPS Function			 */
///////////////////////////
byte ioc::dps_run(word onoff, bool isOn) {
	SIM_RET
	word prev = *P_DPS_RUN;
	if (isOn)
		*P_DPS_RUN = prev | onoff;
	else
		*P_DPS_RUN = prev & (~onoff);
		
	return RTN_OK;
}

byte ioc::dps_hv_enable(byte onoff, bool isOn) {
	SIM_RET	
	byte prev = *P_DPS_HV;	
	*P_DPS_HV = onoff;
	if (isOn)
		*P_DPS_HV = prev | onoff;
	else
		*P_DPS_HV = prev & (~onoff);

	return RTN_OK;
}

byte ioc::dps_vref_enable(byte onoff, bool isOn) {
	SIM_RET
	byte prev = *P_DPS_VREF;
	*P_DPS_VREF = onoff;
	if (isOn)
		*P_DPS_VREF = prev | onoff;
	else
		*P_DPS_VREF = prev & (~onoff);
	return RTN_OK;
}

word ioc::dps_read_alert()
{
	SIM_RET
	uint read;
	word data;	

	read = *P_DPS_FAULT;  
	data = read & 0xFF;
	data |= ((read >> 16) & 0xFF) << 8;
	
	DBG_INFO("\t DPS Alert data : 0x%X\n", data & 0xFF);
	DBG_INFO("\t DPS Fault data : 0x%X\n", data & 0xFF00);

	return data;
}


byte ioc::dps_wait_eos(void) {

	SIM_RET
	uint timeout = DPS_TIMEOUT;

	int st = (*P_DPS_STS);
	if(st & (DPS_STS_EOS)) return RTN_OK;

	usleep(1);
	st = (*P_DPS_STS);
	while (((st & (DPS_STS_EOS)) == 0x0) && (timeout > 0)) {
		if ((st & (DPS_STS_ERR)) != 0x0) {
			printf( "i2c.err : expect[ %08X ] - sts[ %08X ] !!!\r\n", DPS_STS_EOS, *P_DPS_STS);
			return RTN_ERR;
		}
		st = (*P_DPS_STS);
		timeout--;
		usleep(1);
	}	

	if (timeout == 0) {
		printf( "eos.timeout : expect[ %08X ] - sts[ %08X ] !!!\r\n", DPS_STS_EOS, *P_DPS_STS);

		Sleep(0.1);

		return RTN_TIMEOUT;
	}

	return RTN_OK;

}

byte ioc::dps_read(byte dev_no, byte cmd, byte len, byte* rbuf) {
	SIM_RET
	*P_DPS_CMD = DPS_CMD_RD | (dev_no << DPS_LOC_NO) | ((len - 1) << DPS_LOC_BL) | cmd;

	byte rv = 0;
	rv = dps_wait_eos();

	if (rv != RTN_OK)
		return rv;
	
	for (uint i = 0; i < len; i++) {
		rbuf[i] = *(P_DPS_DATA + i);
	}

	return RTN_OK;
}

byte ioc::dps_write(byte dev_no, byte cmd, byte len, byte* wbuf) {
	SIM_RET	
	for (uint i = 0; i < len; i++) {
		*(P_DPS_DATA + i) = wbuf[i];
	}

	*P_DPS_CMD = DPS_CMD_WR | (dev_no << DPS_LOC_NO) | ((len - 1) << DPS_LOC_BL) | cmd;

	byte rv = 0;
	rv = dps_wait_eos();

	return rv;
}

byte ioc::dps_send_byte(byte dev_no, byte cmd)
{

	*P_DPS_CMD = DPS_CMD_RD | (dev_no << DPS_LOC_NO) | (0 << DPS_LOC_BL) | cmd;
	
	usleep(1000*20);
	return 0;
	byte rv = 0;
	rv = dps_wait_eos();

	return rv;
}


///////////////////////////
/* ALL Stat Function	 */
///////////////////////////
void ioc::read_all_status(byte* pBuf, int sz)
{
	for (int i = 0; i < sz; i++) {
		pBuf[i] = *(P_ALLST_BUF + i);
	}	
}

void ioc::read_one_status(byte* pBuf, int sz, int offset)
{
	for (int i = 0; i < sz; i++) {
		pBuf[i] = *(P_ALLST_BUF + offset + i);
	}
}

//////////////////////////
	/* EEPROM Function	*/
//////////////////////////



byte ioc::epr_wait_eos(void) {

	SIM_RET
		uint timeout = DPS_TIMEOUT;
	
	uint st = (*P_EPR_STS);
	while (((st & (1<<31)) == 0x0) && (timeout > 0)) {  // Check End Of Stop
		if ((st & (1<<30)) != 0x0) {					// Check Error
			printf("i2c.err : expect[ %08X ] - sts[ %08X ] !!!\r\n", st, *P_EPR_STS);
			return RTN_ERR;
		}
		st = (*P_EPR_STS);
		timeout--;
		usleep(1);
	}

	if (timeout == 0) {
		printf("eos.timeout : expect[ %08X ] - sts[ %08X ] !!!\r\n", st, *P_EPR_STS);

		Sleep(0.1);

		return RTN_TIMEOUT;
	}

	return RTN_OK;

}

void ioc::eeprom_write(int adr, byte* pBuf, int sz)
{
	for (int i = 0; i < sz; i++)
		*(P_EPR_BUF + i) = *(pBuf + i);
	*(P_EPR_CMD) = (1 << 28) | ((adr & 0xffff) << 8) | (sz & 0xff);
	epr_wait_eos();
}

void ioc::eeprom_read(int adr, byte* pBuf, int sz)
{	
	*(P_EPR_CMD) = (1 << 29) | ((adr & 0xffff) << 8) | (sz & 0xff);
	if (epr_wait_eos() != RTN_OK)return;
	for (int i = 0; i < sz; i++)
		*(pBuf + i) =  *(P_EPR_BUF + i);
}

///////////////////////////////////////////////////////////////////////////////////





volatile byte* ioc::getAdr(uint adr)
{
	return (m_pBaseAdr + adr);
}

void ioc::writeIO(uint adr, uint value)
{
	SIM;
	uint* base = (uint*)getAdr(adr);
	*base = value;
}

uint ioc::readIO(uint adr)
{
	SIM_RET
	uint* base = (uint*)getAdr(adr);
	return *base;
}

void ioc::writePG(uint adr, uint val, int sz)
{
	if (m_nBFPGA != FPGA_DEFAULT)
		writePGI(PG_ALL, adr, val, sz);
	else
		writePGD(adr, val, sz);	
	printDBG(adr, val, sz, RTN_OK);
}

void ioc::writePGD(uint adr, uint val, int sz)
{

	byte ret = RTN_OK;
	if (sz == 1)
		ret = c2p_wb(0, adr, val & 0xff);
	else if (sz == 2)
		ret = c2p_wh(0, adr, val & 0xffff);
	else if (sz == 3)
		ret = c2p_ww(0, adr, val & 0xffffff, 2);
	else if (sz == 4)
		ret = c2p_ww(0, adr, val);
	
	if (ret != RTN_OK)
	addErr(0xf, __FUNCTION__, adr, sz);;

}

void ioc::writePGI(byte PGSel, uint adr, uint val, int sz)
{
	writePGDataD(PGI_ADR, (byte*)&adr, 4);
	writePGDataD(PGI_PG_BUF, (byte*)&val, sz);

	uint sel = (PGSel == PG_ALL ? PG_ALL : (1 << PGSel)) << POS_SEL;	
	uint cmd = sel | BIT_W | sz;

	writePGDataD(PGI_CMD, (byte*)&cmd, 4);
	if (pgi_wait_eos(PGSel) != RTN_OK)
		addErr(PGSel, __FUNCTION__, adr, sz);
	
}


uint ioc::readPG(uint adr, int sz)
{
	uint uv = 0;
	byte ret = RTN_OK;
	if (sz == 1) {
		byte v;
		ret = c2p_rb(0, adr, &v);
		uv = v;
	}
	else if (sz == 2)
	{
		short v;
		ret = c2p_rh(0, adr, &v);
		uv = v;
	}
	else if (sz == 3) {
		ret = c2p_rw(0, adr, &uv, 2);
	}
	else if (sz == 4) {
		ret = c2p_rw(0, adr, &uv);
	}
	
	printDBG(adr, uv, sz, ret);

	return uv;
}

uint ioc::readPGI(byte PGSel, uint adr, int sz)
{
	writePGDataD(PGI_STAT, (byte*)&adr, 4);

	uint sel = (PGSel == PG_ALL ? PG_ALL : (1 << PGSel)) << POS_SEL;
	uint cmd = sel | BIT_R | sz;

	writePGDataD(PGI_CMD, (byte*)&cmd, 4);

	if (pgi_wait_eos(PGSel) != RTN_OK)
		addErr(PGSel, __FUNCTION__, adr, sz);;

	uint rd = 0;
	readPGData(PGI_PG_BUF, (byte*)&rd, sz );
	return rd;

}

void ioc::writePGData(uint adr, byte* pData, int sz)
{
	if (m_nBFPGA != FPGA_DEFAULT)
		writePGDataI(PG_ALL, adr, pData, sz);
	else
		writePGDataD(adr, pData, sz);

	if (m_bDebug)
		for (int i = 0; i < sz; i++)
			printf("(%s)\t0x%08x : 0x%02x\n", __FUNCTION__, adr + i, *(pData + i));
}

void ioc::writePGDataD(uint adr, byte* pData, int sz)
{	
	if (c2p_write(0, 0, 0, adr, sz, pData) != RTN_OK)			
		addErr(0xf, __FUNCTION__, adr, sz);
}

void ioc::writePGDataI(byte PGSel, uint adr, byte* pData, int sz)
{	
	if (c2p_write(0, 0, 0, PGI_PG_BUF, sz, pData) != RTN_OK)
		addErr(0xf, __FUNCTION__, adr, sz);

	writePGDataD(PGI_ADR, (byte*)&adr, 4);
	uint sel = (PGSel == PG_ALL ? PG_ALL : (1 << PGSel)) << POS_SEL;
	uint cmd = sel | BIT_W | sz;

	writePGDataD(PGI_CMD, (byte*)&cmd, 4);

	if (pgi_wait_eos(PGSel) != RTN_OK)
		addErr(PGSel, __FUNCTION__, adr, sz);
}

void ioc::readPGData(uint adr, byte* pData, int sz)
{
	//byte ret = c2p_read(0, 0, 0, adr, sz, pData);
	if (c2p_read(0, 0, 0, adr, sz, pData) != RTN_OK)
		addErr(0xf, __FUNCTION__, adr, sz);

	if (m_bDebug)
		for (int i = 0; i < sz; i++)
			printf("(%s)\t0x%08x : 0x%02x\n", __FUNCTION__, adr + i, *(pData + i));
}


void ioc::readPGDataI(byte PGSel, uint adr,byte* pData, int sz)
{
	writePGDataD(PGI_STAT, (byte*)&adr, 4);

	uint sel = (PGSel == PG_ALL ? PG_ALL : (1 << PGSel)) << POS_SEL;
	uint cmd = sel | BIT_R | sz;

	writePGDataD(PGI_CMD, (byte*)&cmd, 4);

	if (pgi_wait_eos(PGSel) != RTN_OK)
		addErr(PGSel, __FUNCTION__, adr, sz);
	
	readPGData(PGI_PG_BUF, pData, sz);
		
}


void ioc::addErr(byte PGSel, const char* func, uint adr, int sz)
{
	errmsg em;	
	sprintf(em.s, "%s PG%d adr:0x%x sz:%d", func, PGSel, adr, sz);
	m_sIOErrVec.push_back(em);
}


byte ioc::pgi_wait_eos(byte PGSel)
{
	uint statAll = readPG(PGI_STAT);

	if (PGSel == 0xf)
	{
		for (int i = 0; i < 4; i++)
		{
			uint stat = (statAll >> (i * 8)) & 0xff;
			uint timeout = LB_TIMEOUT;

			while (((stat & LB_STS_EOS) == 0x0) && (--timeout > 0)) {
				if ((stat & LB_STS_ERR) != 0x0) {
					printf("com.err : expect[ %02X ] - sts[ %02X ] !!!\r\n", LB_STS_EOS, stat);
					return RTN_ERR;
				}
			}

			if (timeout == 0) {
				printf("eos.timeout : expect[ %02X ] - sts[ %02X ] !!!\r\n", LB_STS_EOS, stat);
				printf("eos.timeout : expect[ %02X] - sts[ %02X] !!!\r\n", LB_STS_EOS, stat);
				writePGI(PGSel, PGI_CMD, BIT_RST);
				return RTN_TIMEOUT;
			}
		}
	}
	else {
		uint stat = (statAll >> (PGSel * 8)) & 0xff;

		uint timeout = LB_TIMEOUT;
		while (((stat & LB_STS_EOS) == 0x0) && (--timeout > 0)) {
			if ((stat & LB_STS_ERR) != 0x0) {
				printf("com.err : expect[ %02X ] - sts[ %02X ] !!!\r\n", LB_STS_EOS, stat);
				return RTN_ERR;
			}
		}

		if (timeout == 0) {
			printf("eos.timeout : expect[ %02X ] - sts[ %02X ] !!!\r\n", LB_STS_EOS, stat);
			printf("eos.timeout : expect[ %02X] - sts[ %02X] !!!\r\n", LB_STS_EOS, stat);
			writePGI(PGSel, PGI_CMD, BIT_RST);
			return RTN_TIMEOUT;
		}
	}

	return RTN_OK;
}

void ioc::printDBG(uint adr, uint val, int sz, byte ret)
{
	if (m_bDebug)
		printf("(%s)\t0x%08x : 0x%08x(%d) [%s]\n", __FUNCTION__, adr, val, sz,
			(ret == RTN_OK) ? "OK" : "Fail");
	else
		if (ret != RTN_OK)
			printf("(%s)\t0x%08x : 0x%08x(%d) [%s]\n", __FUNCTION__, adr, val, sz,
				(ret == RTN_OK) ? "OK" : "Fail");
}

bool ioc::checkPG(uint compID)
{
	bool bRet = true;

	if (m_nBFPGA != FPGA_DEFAULT)
	{
		for (int i = 0; i < 4; i++) {
			uint val = readPGI(i, PG_SYSTEM | 0x100);
			if (val != compID) {
				bRet = false;
				break;
			}
		}
	}
	else {
		uint val = readPG(PG_SYSTEM | 0x100);

		if (val != compID) {
			bRet = false;
			printf("%sID Fail Val=0x%08x Exp=0x%08x\n", __FUNCTION__, val, compID);
		}
	}

	return bRet;
}

bool ioc::checkPGInterface()
{
	writePG(PGI_LATANCY, 2, 1);
	char id[32];
	readPGData(PGI_PJT_INFO, (byte*)id, 9);
	id[9] = '\0';
//	memcpy(id, &val, 12);
	printf("id s : %s\n",  id);

	if (strcmp(id, "PG-BRIDGE") == 0) {
		return true;
	}
	
	return true;
}



