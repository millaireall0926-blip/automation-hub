
#include "ioc.h"

#include <sys/mman.h>
#include "bitcharts.h"


#define SIMCK if(m_bSim)return RTN_OK
#define SIMCK2 if(m_bSim)return

/* pcie */

int ioc::open_pcie(void) {
	SIMCK;

	if ((pcie_fd = open("/dev/xdma0_bypass", O_RDWR | O_SYNC)) == -1) {
		DBG_CRIT("\t Can't open PCIe[ %s ] !!!\n", "/dev/xdma0_bypass");
		return RTN_ERR;
	}
	pcie_in_progress = 1;
	printf("PCIe %s opened.\n", "/dev/xdma0_bypass");

	pcie_map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, pcie_fd, MAP_ADDR);
	if (pcie_map_base == (uint8_t*)-1) {
		DBG_CRIT("\t Can't mmap PCIe !!!\n");
		return RTN_ERR;
	}

	printf("Memory mapped at address %p.\n", pcie_map_base);

	m_pBaseAdr = (uint8_t*)((uint8_t*)pcie_map_base);

	P_PJT_BASE = (uint8_t*)((uint8_t*)pcie_map_base + PJT_BASE);
	P_BRD_BASE = (uint8_t*)((uint8_t*)pcie_map_base + BRD_BASE);
	P_CFG_BASE = (uint8_t*)((uint8_t*)pcie_map_base + CFG_BASE);
	P_C2P_BASE = (uint8_t*)((uint8_t*)pcie_map_base + C2P_BASE);
	P_C2T_BASE = (uint8_t*)((uint8_t*)pcie_map_base + C2T_BASE);
	P_ADC_BASE = (uint8_t*)((uint8_t*)pcie_map_base + ADC_BASE);
	P_DAC_BASE = (uint8_t*)((uint8_t*)pcie_map_base + DAC_BASE);
	P_DPS_BASE = (uint8_t*)((uint8_t*)pcie_map_base + DPS_BASE);
	P_PE_BASE  = (uint8_t*)((uint8_t*)pcie_map_base + PE_BASE);
	P_CVM_BASE = (uint8_t*)((uint8_t*)pcie_map_base + CVM_BASE);

	P_BRD_INUSE = (uint8_t*)((uint8_t*)pcie_map_base + BRD_BASE + 0x20);
	P_BRD_BIBSEL = (uint8_t*)((uint8_t*)pcie_map_base + BRD_BASE + 0x24);	
	P_BRD_BIBID = (uint8_t*)((uint8_t*)pcie_map_base + BRD_BASE + 0x28);
	P_BRD_SITEID = (uint8_t*)((uint8_t*)pcie_map_base + BRD_BASE + 0x2C);

	P_CFG_BUF = (uint8_t*)((uint8_t*)pcie_map_base + CFG_BUF_BASE);
	P_CFG_CMD = (uint32_t*)((uint8_t*)pcie_map_base + CFG_BASE + 0x20);
	P_CFG_STS = (uint8_t*)((uint8_t*)pcie_map_base + CFG_BASE + 0x30);
	P_CFG_DONE = (uint16_t*)((uint8_t*)pcie_map_base + CFG_BASE + 0x32);

	P_C2P_BUF = (uint8_t*)((uint8_t*)pcie_map_base + C2P_BUF_BASE);
	P_C2P_ADDR = (uint32_t*)((uint8_t*)pcie_map_base + C2P_BASE + 0x20);
	P_C2P_CMD = (uint32_t*)((uint8_t*)pcie_map_base + C2P_BASE + 0x24);
	P_C2P_RL = (uint8_t*)((uint8_t*)pcie_map_base + C2P_BASE + 0x2C);
	P_C2P_STS = (uint8_t*)((uint8_t*)pcie_map_base + C2P_BASE + 0x30);

	P_ADC_CMD = (uint16_t*)((uint8_t*)pcie_map_base + ADC_BASE + 0x20);
	P_ADC_SETTL = (uint32_t*)((uint8_t*)pcie_map_base + ADC_BASE + 0x24);
	P_ADC_RANGE = (uint8_t*)((uint8_t*)pcie_map_base + ADC_BASE + 0x28);
	P_ADC_OS = (uint8_t*)((uint8_t*)pcie_map_base + ADC_BASE + 0x2C);
	P_ADC_STS = (uint16_t*)((uint8_t*)pcie_map_base + ADC_BASE + 0x30);
	P_ADC_DATA = (uint16_t*)((uint8_t*)pcie_map_base + ADC_BASE + 0x40);

	P_DAC_CMD = (uint32_t*)((uint8_t*)pcie_map_base + DAC_BASE + 0x20);
	P_DAC_CLP = (uint32_t*)((uint8_t*)pcie_map_base + DAC_BASE + 0x2C);
	P_DAC_STS = (uint32_t*)((uint8_t*)pcie_map_base + DAC_BASE + 0x30);
	
	P_DPS_CMD = (uint32_t*)((uint8_t*)pcie_map_base + DPS_BASE + 0x20);
	P_DPS_RUN = (uint8_t*)((uint8_t*)pcie_map_base + DPS_BASE + 0x28);
	P_DPS_VS3F = (uint8_t*)((uint8_t*)pcie_map_base + DPS_BASE + 0x2C);
	P_DPS_STS = (uint32_t*)((uint8_t*)pcie_map_base + DPS_BASE + 0x30);
	P_DPS_DATA = (uint8_t*)((uint8_t*)pcie_map_base + DPS_BASE + 0x40);

	P_PE_SEL = (uint32_t*)((uint8_t*)pcie_map_base + PE_BASE + 0x20);
	P_PE_CMD = (uint32_t*)((uint8_t*)pcie_map_base + PE_BASE + 0x24);
	P_PE_CAL = (uint8_t*)((uint8_t*)pcie_map_base + PE_BASE + 0x2C);
	P_PE_STS = (uint32_t*)((uint8_t*)pcie_map_base + PE_BASE + 0x30);
	P_PE_ALARM = (uint32_t*)((uint8_t*)pcie_map_base + PE_BASE + 0x34);

	P_CVM_CMD = (uint32_t*)((uint8_t*)pcie_map_base + CVM_BASE + 0x20);
	P_CVM_STS = (uint32_t*)((uint8_t*)pcie_map_base + CVM_BASE + 0x30);


	DBG_INFO("\n");
	DBG_INFO("P_PJT_BASE    : %p\n", P_PJT_BASE);
	DBG_INFO("P_BRD_BASE    : %p\n", P_BRD_BASE);
	DBG_INFO("P_CFG_BASE    : %p\n", P_CFG_BASE);
	DBG_INFO("P_C2P_BASE    : %p\n", P_C2P_BASE);
	DBG_INFO("P_C2T_BASE    : %p\n", P_C2T_BASE);
	DBG_INFO("P_ADC_BASE    : %p\n", P_ADC_BASE);
	DBG_INFO("P_DAC_BASE    : %p\n", P_DAC_BASE);
	DBG_INFO("P_DPS_BASE    : %p\n", P_DPS_BASE);
	DBG_INFO("P_PE_BASE     : %p\n", P_PE_BASE);
	DBG_INFO("P_CVM_BASE    : %p\n", P_CVM_BASE);


	DBG_INFO("\n");
	DBG_INFO("P_BRD_INUSE   : %p\n", P_BRD_INUSE);
	DBG_INFO("P_BRD_BIBSEL  : %p\n", P_BRD_BIBSEL);
	DBG_INFO("P_BRD_BIBID   : %p\n", P_BRD_BIBID);	
	DBG_INFO("P_BRD_SITEID  : %p\n", P_BRD_SITEID);

	DBG_INFO("\n");
	DBG_INFO("P_CFG_BUF     : %p\n", P_CFG_BUF);
	DBG_INFO("P_CFG_CMD     : %p\n", P_CFG_CMD);
	DBG_INFO("P_CFG_STS     : %p\n", P_CFG_STS);
	DBG_INFO("P_CFG_DONE    : %p\n", P_CFG_DONE);

	DBG_INFO("\n");
	DBG_INFO("P_C2P_BUF     : %p\n", P_C2P_BUF);
	DBG_INFO("P_C2P_ADDR    : %p\n", P_C2P_ADDR);
	DBG_INFO("P_C2P_CMD     : %p\n", P_C2P_CMD);
	DBG_INFO("P_C2P_RL      : %p\n", P_C2P_RL);
	DBG_INFO("P_C2P_STS     : %p\n", P_C2P_STS);

	DBG_INFO("\n");
	DBG_INFO("P_ADC_CMD     : %p\n", P_ADC_CMD);
	DBG_INFO("P_ADC_SETTL   : %p\n", P_ADC_SETTL);
	DBG_INFO("P_ADC_RANGE   : %p\n", P_ADC_RANGE);
	DBG_INFO("P_ADC_OS      : %p\n", P_ADC_OS);
	DBG_INFO("P_ADC_STS     : %p\n", P_ADC_STS);
	DBG_INFO("P_ADC_DATA    : %p\n", P_ADC_DATA);

	DBG_INFO("\n");
	DBG_INFO("P_DAC_CMD     : %p\n", P_DAC_CMD);
	DBG_INFO("P_DAC_CLP     : %p\n", P_DAC_CLP);
	DBG_INFO("P_DAC_STS     : %p\n", P_DAC_STS);
	
	DBG_INFO("\n");
	DBG_INFO("P_DPS_CMD     : %p\n", P_DPS_CMD);
	DBG_INFO("P_DPS_RUN     : %p\n", P_DPS_RUN);
	DBG_INFO("P_DPS_VS3F    : %p\n", P_DPS_VS3F);
	DBG_INFO("P_DPS_STS     : %p\n", P_DPS_STS);
	DBG_INFO("P_DPS_DATA    : %p\n", P_DPS_DATA);

	DBG_INFO("\n");
	DBG_INFO("P_PE_SEL      : %p\n", P_PE_SEL);
	DBG_INFO("P_PE_CMD      : %p\n", P_PE_CMD);
	DBG_INFO("P_PE_CAL      : %p\n", P_PE_CAL);
	DBG_INFO("P_PE_STS      : %p\n", P_PE_STS);
	DBG_INFO("P_PE_ALARM    : %p\n", P_PE_ALARM);

	DBG_INFO("\n");
	DBG_INFO("P_CVM_CMD      : %p\n", P_CVM_CMD);
	DBG_INFO("P_CVM_STS      : %p\n", P_CVM_STS);

	DBG_INFO("\n");

	P8_BUF_BASE = (uint8_t*)((uint8_t*)pcie_map_base + 0x0000);
	P16_BUF_BASE = (uint16_t*)((uint8_t*)pcie_map_base + 0x0000);
	P32_BUF_BASE = (uint32_t*)((uint8_t*)pcie_map_base + 0x0000);

	P8_LED = (uint8_t*)((uint8_t*)pcie_map_base + 0x1000);
	P8_SW = (uint8_t*)((uint8_t*)pcie_map_base + 0x1008);

	DBG_INFO("P8_BUF_BASE  : %p\n", P8_BUF_BASE);
	DBG_INFO("P16_BUF_BASE : %p\n", P16_BUF_BASE);
	DBG_INFO("P32_BUF_BASE : %p\n", P32_BUF_BASE);
	DBG_INFO("P8_LED       : %p\n", P8_LED);
	DBG_INFO("P8_SW        : %p\n", P8_SW);

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

uint8_t ioc::zc_wr8(uint32_t addr, uint16_t bl, uint8_t* wbuf) {
	SIMCK;
	if ((addr + bl + 1) > 0xFFF)
		return RTN_ARGC_ERR;

	for (uint16_t i = 0; i < (bl + 1); i++) {
		*((uint8_t*)P8_BUF_BASE + addr + i) = wbuf[i];
	}

	return RTN_OK;
}
uint8_t ioc::zc_wr16(uint32_t addr, uint16_t bl, uint8_t* wbuf) {
	SIMCK;
	if ((addr + bl + 1) > 0xFFF)
		return RTN_ARGC_ERR;

	for (uint16_t i = 0; i < (bl + 1); i++) {
		*((uint8_t*)P16_BUF_BASE + addr + i) = wbuf[i];
	}

	return RTN_OK;
}
uint8_t ioc::zc_wr32(uint32_t addr, uint16_t bl, uint8_t* wbuf) {
	SIMCK;
	if ((addr + bl + 1) > 0xFFF)
		return RTN_ARGC_ERR;

	for (uint16_t i = 0; i < (bl + 1); i++) {
		*((uint8_t*)P32_BUF_BASE + addr + i) = wbuf[i];
	}

	return RTN_OK;
}

uint8_t ioc::zc_rd8(uint32_t addr, uint16_t bl, uint8_t* rbuf) {
	if ((addr + bl + 1) > 0xFFF)
		return RTN_ARGC_ERR;

	for (uint16_t i = 0; i < (bl + 1); i++) {
		rbuf[i] = *((uint8_t*)P8_BUF_BASE + addr + i);
	}

	return RTN_OK;
}
uint8_t ioc::zc_rd16(uint32_t addr, uint16_t bl, uint8_t* rbuf) {
	SIMCK;
	if ((addr + bl + 1) > 0xFFF)
		return RTN_ARGC_ERR;

	for (uint16_t i = 0; i < (bl + 1); i++) {
		rbuf[i] = *((uint8_t*)P16_BUF_BASE + addr + i);
	}

	return RTN_OK;
}
uint8_t ioc::zc_rd32(uint32_t addr, uint16_t bl, uint8_t* rbuf) {
	SIMCK;
	if ((addr + bl + 1) > 0xFFF)
		return RTN_ARGC_ERR;

	for (uint16_t i = 0; i < (bl + 1); i++) {
		rbuf[i] = *((uint8_t*)P32_BUF_BASE + addr + i);
	}

	return RTN_OK;
}

uint8_t ioc::zc_led(uint8_t led, uint8_t onoff) {
	SIMCK;
	uint8_t prev_code = 0;

	prev_code = *P8_LED;

	if (led == 0)
		*P8_LED = (prev_code & 0x0E) | (onoff << led);
	else if (led == 1)
		*P8_LED = (prev_code & 0x0D) | (onoff << led);
	else if (led == 2)
		*P8_LED = (prev_code & 0x0B) | (onoff << led);
	else if (led == 3)
		*P8_LED = (prev_code & 0x07) | (onoff << led);
	else
		*P8_LED = 0x0F;

	return RTN_OK;
}

uint8_t ioc::zc_switch(uint8_t* rdata) {
	SIMCK;
	*rdata = (*P8_SW) & 0x0F;

	return RTN_OK;
}
/* test w/ zc706 eval b'd */



void ioc::print_ip_info(void* base) {
	SIMCK2;
	P_IP_INFO in_info = (P_IP_INFO)base;

	printf("**** %16s Rev.%d.%02d @ 20%02d.%02d.%02d ****\r\n", in_info->IpName, in_info->MajorRev, in_info->MinorRev, in_info->Year, in_info->Month, in_info->Date);
}

void ioc::printf_all_ip(void) {
	print_ip_info((void*)P_PJT_BASE);
	print_ip_info((void*)P_BRD_BASE);
	print_ip_info((void*)P_CFG_BASE);
	print_ip_info((void*)P_C2P_BASE);
	print_ip_info((void*)P_C2T_BASE);
	print_ip_info((void*)P_ADC_BASE);
	print_ip_info((void*)P_DAC_BASE);
	print_ip_info((void*)P_DPS_BASE);
	print_ip_info((void*)P_PE_BASE);
}

/* brd */

bool ioc::odd_parity(uint8_t byte)
{
	unsigned char parity = byte;

	parity ^= (parity >> 1);
	parity ^= (parity >> 2);
	parity ^= (parity >> 4);

	return ((parity & 1) ^ 1);
}

void ioc::brd_info(void) {
	SIMCK2;
	printf("\t BIB : %s [ %d ]\n", ((*P_BRD_INUSE) == 1) ? "INSERTED" : "not INSERTED", *P_BRD_INUSE);
	printf("\t SITE-ID : %02X\n",  ((*P_BRD_SITEID) & 0xff));
}

uint ioc::readBID(void)
{
//	brd_info();

	if ((*P_BRD_INUSE) == 0) return 0;
	int nBid = 0;


	int selCnt = 4;
	uint8_t idArr[4];
	for (int i = 0; i < selCnt; i++)
	{
		*P_BRD_BIBSEL = (~(1 << i) & 0xf);
		idArr[i] = *P_BRD_BIBID;
		idArr[i] = ~idArr[i];
//		printf("ID SEL : 0x%X = 0x%08X\n", (~(1 << i) & 0xf), idArr[i]);
	}

	//////////////////////////
	// 8-Bit Gathering
	/////////////////////////
	for (int j = 0; j < 4; j++)
		nBid |= idArr[j] << (j * 8);

	//////////////////////////
	// 4-Bit Gathering
	//////////////////////////
	//int read;
	//for (int i = 0; i < 8; i++)
	//{
	//	read = 0;
	//	for (int j = 0; j < 4; j++)
	//		read |= (((idArr[j] >> i) & 1) << j);
	//	printf("ID%d : %x\n", i, read);
	//	nBid |= read << (i * 4);
	//}

	return nBid;
}

bool ioc::checkBFPGA()
{
	uint nbid = readBID();
	int nBall = (nbid >> 16) & 0xf;
	int nDevFam = (nbid >> 20) & 0x1f;
	int nFunc = (nbid >> 25) & 0x7;

	if (nFunc == 8) m_nBFPGA = FPGA_BIB_DRAM;
	else if (nFunc == 9) m_nBFPGA = FPGA_BIB_MCP;

	return m_nBFPGA;
}


int ioc::readSiteID(void)
{
	return (int)*P_BRD_SITEID;
}


/* cfg */
void ioc::cfg_reset(void) {
	SIMCK2;
	*P_CFG_CMD = CFG_CMD_RST;

	Sleep(0.1);
}

uint8_t ioc::cfg_wait_eos(void) {
	SIMCK;
	// tCK : 30ns
	// max : 2ms for 64KB transfer
	uint32_t timeout = CFG_TIMEOUT;

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

uint8_t ioc::cfg_wait_busy(void) {
	SIMCK;
	// tCK : 30ns
	// max : 2ms for 64KB transfer
	uint32_t timeout = CFG_TIMEOUT;

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
	SIMCK;
	int retryCnt = 5;
	uint8_t rv = 0;

	for (int i = 0; i < retryCnt; i++) {
		FILE* f = NULL;
		if ((f = fopen(bitstream, "rb")) == NULL) {
			DBG_CRIT("bitstream \"%s\" open error!\n", bitstream);
			return RTN_ARGC_ERR;
		}

		fseek(f, 0, SEEK_END);
		uint32_t fileSize = ftell(f);
		fseek(f, 0, SEEK_SET);

		DBG_INFO("\t bitstream[ %s ] size[ %7.3f MByte ]\n", bitstream, fileSize / 1024.0 / 1024.0);

		//DBG_INFO("\t Cfg : prev.sts[ %02X ]\n", *P_CFG_STS);

		uint8_t* rbuf = new uint8_t[MAX_CFG_SIZE + 10];
		memset(rbuf, 0, MAX_CFG_SIZE + 10);
		while (fileSize > 0) {
			uint8_t last = 0;
			uint32_t transSize = (fileSize > MAX_CFG_SIZE) ? MAX_CFG_SIZE : fileSize;

			if (transSize < MAX_CFG_SIZE)
				last = 1;

			if (fread(rbuf, transSize, 1, f) != 1) {
				rv = RTN_ERR;
				break;
			}

			//memcpy((uint8_t *)P_CFG_BUF, rbuf, transSize);
			for (uint32_t i = 0; i < transSize; i++) {
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
		uint32_t fileSize = ftell(f);
		fseek(f, 0, SEEK_SET);

		DBG_INFO("\t bitstream[ %s ] size[ %7.3f MByte ]\n", bitstream, fileSize / 1024.0 / 1024.0);

		//DBG_INFO("\t Cfg : prev.sts[ %02X ]\n", *P_CFG_STS);

		uint8_t* rbuf = new uint8_t[MAX_CFG_SIZE + 10];
		memset(rbuf, 0, MAX_CFG_SIZE + 10);
		while (fileSize > 0) {
			bool bLast = false;
			uint32_t transSize = (fileSize > MAX_CFG_SIZE) ? MAX_CFG_SIZE : fileSize;

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
	SIMCK2;

	*P_C2P_CMD = LB_CMD_RST;
	
}

uint8_t ioc::c2p_wait_eos(uint8_t dev_no) {
	SIMCK;
	// wr : 360ns + BL * 10ns
	// rd : 460ns + BL * 10ns
	// max : 656us for 64KB transfer
	uint32_t timeout = LB_TIMEOUT;

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

uint8_t ioc::c2p_write(uint8_t dev_no, uint8_t ddr, uint8_t ddr_msb, uint32_t addr, uint16_t bl, uint8_t* wbuf) {
	SIMCK;
	for (uint16_t i = 0; i < (bl); i++) {
//		printf("idx%d: adr %08x val %02x\n",i, P_C2P_BUF + i, wbuf[i]);
		*(P_C2P_BUF + i) = wbuf[i];
	}
 
	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_WR | (0x1 << (16 + dev_no)) | (bl-1);
//	printf("write adr %08x cmd %02x\n",addr, *P_C2P_CMD);
	return c2p_wait_eos(dev_no);
}

uint8_t ioc::c2p_read(uint8_t dev_no, uint8_t ddr, uint8_t ddr_msb, uint32_t addr, uint16_t bl, uint8_t* rbuf) {
//#ifndef _SIM_
//	memset((uint8_t*)P_C2P_BUF, 0x0, bl + 1);
//#endif
	SIMCK;
 
	* P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_RD | (0x1 << (16 + dev_no)) | (bl-1);
//	printf("read adr %08x cmd %02x\n",addr, *P_C2P_CMD);
	uint8_t rv = 0;
	rv = c2p_wait_eos(dev_no);
	if (rv != RTN_OK)
		return rv;

	//memcpy(rbuf, (uint8_t *)P_C2P_BUF, bl + 1);
	for (uint16_t i = 0; i < (bl); i++) {
//		printf("idx%d: adr %08x val %02x\n",i, P_C2P_BUF + i, *(P_C2P_BUF + i));
		rbuf[i] = *(P_C2P_BUF + i);
	}

	return RTN_OK;
}

uint8_t ioc::c2p_wb(uint8_t dev_no, uint32_t addr, uint8_t wdata) {
	SIMCK;
	uint16_t bl = 0;

	*P_C2P_BUF = wdata;
	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_WR | (0x1 << (16 + dev_no)) | bl;

	return c2p_wait_eos(dev_no);
}

uint8_t ioc::c2p_wh(uint8_t dev_no, uint32_t addr, uint16_t wdata) {
	SIMCK;
	uint16_t bl = 1;

	*((uint16_t*)P_C2P_BUF) = wdata;
	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_WR | (0x1 << (16 + dev_no)) | bl;

	return c2p_wait_eos(dev_no);
}

uint8_t ioc::c2p_ww(uint8_t dev_no, uint32_t addr, uint32_t wdata, int sz) {
	SIMCK;
	uint16_t bl = (sz);

	*((uint32_t*)P_C2P_BUF) = wdata;
	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_WR | (0x1 << (16 + dev_no)) | bl;

	return c2p_wait_eos(dev_no);
}

uint8_t ioc::c2p_rb(uint8_t dev_no, uint32_t addr, uint8_t* rdata) {
	SIMCK;
	uint16_t bl = 0;

	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_RD | (0x1 << (16 + dev_no)) | bl;

	uint8_t rv = 0;
	rv = c2p_wait_eos(dev_no);
	if (rv != RTN_OK)
		return rv;

	*rdata = *P_C2P_BUF;

	return RTN_OK;
}

uint8_t ioc::c2p_rh(uint8_t dev_no, uint32_t addr, uint16_t* rdata) {
	SIMCK;
	uint16_t bl = 1;

	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_RD | (0x1 << (16 + dev_no)) | bl;

	uint8_t rv = 0;
	rv = c2p_wait_eos(dev_no);
	if (rv != RTN_OK)
		return rv;

	*rdata = *((uint16_t*)P_C2P_BUF);

	return RTN_OK;
}

uint8_t ioc::c2p_rw(uint8_t dev_no, uint32_t addr, uint32_t* rdata, int sz) {
	SIMCK;
	uint16_t bl = (sz);

	*P_C2P_RL = 0;
	*P_C2P_ADDR = addr;
	*P_C2P_CMD = LB_CMD_RD | (0x1 << (16 + dev_no)) | bl;

	uint8_t rv = 0;
	
	rv = c2p_wait_eos(dev_no);
	if (rv != RTN_OK)
		return rv;

	*rdata = *((uint32_t*)P_C2P_BUF);

	return RTN_OK;
}


/* adc */
void ioc::adc_reset(void) {
	SIMCK2;
	*P_ADC_CMD = ADC_CMD_RST;

	Sleep(0.1);
}

double adcRange[2] = { 1.0, 1.0 };
void ioc::adc_range(uint8_t dev_no, uint8_t range) {
	SIMCK2;
	uint8_t prev;

	prev = *P_ADC_RANGE;
	if (dev_no == 0)
		*P_ADC_RANGE = (prev & 0x2) | ((range & 0x1) << 0);
	else
		*P_ADC_RANGE = (prev & 0x1) | ((range & 0x1) << 1);

	adcRange[0] = (((*P_ADC_RANGE) & 0x1) == 0) ? 1.0 : 2.0;
	adcRange[1] = (((*P_ADC_RANGE) & 0x2) == 0) ? 1.0 : 2.0;
	DBG_INFO("\t ADC_0.Range : %s\n", (adcRange[0] == 1.0) ? "+/- 5V" : "+/-10V");
	DBG_INFO("\t ADC_1.Range : %s\n", (adcRange[1] == 1.0) ? "+/- 5V" : "+/-10V");
}

void ioc::adc_os(uint8_t dev_no, uint8_t os) {
	SIMCK2;
	uint8_t prev;

	prev = *P_ADC_OS;
	if (dev_no == 0)
		*P_ADC_OS = (prev & 0x70) | ((os & 0x7) << 0);
	else
		*P_ADC_OS = (prev & 0x07) | ((os & 0x7) << 4);

	DBG_INFO("\t ADC_0.Os : %d\n", ((*P_ADC_OS) >> 0) & 0xF);
	DBG_INFO("\t ADC_1.Os : %d\n", ((*P_ADC_OS) >> 4) & 0xF);
}

void ioc::adc_settling(uint32_t nsec) {
	SIMCK2;
	*P_ADC_SETTL = (int)(nsec / 10);
}

uint8_t ioc::adc_wait_eos(void) {
	SIMCK;
	// adc : tSETTLING + tCONV + tREAD
	// tCONV : 4.2 ~ 315us
	// tREAD : 16bit * 4 * 80ns = 5.12us
	uint32_t timeout = ADC_TIMEOUT;

#ifdef _SIM_
	* P_ADC_STS = ADC_STS_EOS;
#endif

	while ((((*P_ADC_STS) & ADC_STS_EOS) == 0x0) && (--timeout > 0));

	if (timeout == 0) {
		printf( "eos.timeout : expect[ %03X ] - sts[ %03X ] !!!\r\n", ADC_STS_EOS, *P_ADC_STS);

		adc_reset();
		Sleep(0.1);

		return RTN_TIMEOUT;
	}

	return RTN_OK;
}

uint8_t ioc::adc_clear(uint8_t dev_no) {
	SIMCK;
	*P_ADC_CMD = ADC_CMD_CLR | (dev_no << ADC_LOC_SEL);

	Sleep(0.1);

	return adc_wait_eos();
}

uint8_t ioc::adc_read(uint8_t dev_no, uint8_t mux_no, short* code) {
	*P_ADC_CMD = ADC_CMD_SET | (dev_no << ADC_LOC_SEL) | mux_no;

	uint8_t rv = 0;
	rv = adc_wait_eos();
	if (rv != RTN_OK)
		return rv;

#ifdef _SIM_
	for (uint32_t i = 0; i < 8; i++) {
		*(P_ADC_DATA + i) = 0x1000 + 0x1500 * i;
	}
#endif

	//memcpy(code, (uint8_t *)P_ADC_DATA, 16);
	for (uint32_t i = 0; i < 8; i++) {
		*(code + i) = *(P_ADC_DATA + i);
	}

	return RTN_OK;
}


/* dac */
void ioc::dac_reset(void) {
	SIMCK2;
	*P_DAC_CMD = DAC_CMD_RST;

	Sleep(0.1);
}

void ioc::dac_clamp(bool bOn) {
	SIMCK2;
	*P_DAC_CLP = bOn ? 0x1 : 0x0;
	DBG_INFO(" - post[ %08X ]", *P_DAC_CLP);

	Sleep(0.1);
}



uint8_t ioc::dac_wait_eos(void) {
	// dac : 400KHz I2C
	// tSHIFT : 20us * ( N + 3 )Byte = 120us
	uint32_t timeout = DAC_TIMEOUT;

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
		DBG_INFO(" - after timeout[ %08X ]", *P_DAC_CLP);

		dac_reset();
		Sleep(0.1);

		return RTN_TIMEOUT;
	}

	return RTN_OK;
}

uint8_t ioc::dac_clear(void) {
	SIMCK;
	*P_DAC_CMD = DAC_CMD_CLR;

	Sleep(0.1);

	return dac_wait_eos();
}

uint8_t ioc::dac_read(uint8_t cmd, uint8_t addr, uint16_t* rdata) {
	SIMCK;
	*P_DAC_CMD = DAC_CMD_RD | ((cmd & 0x0F) << DAC_LOC_CMD) | ((addr & 0x0F) << DAC_LOC_ADR);

	uint8_t rv = 0;
	rv = dac_wait_eos();
	if (rv != RTN_OK)
		return rv;

#ifdef _SIM_
	* rdata = 0x123;
#endif

	* rdata = ((*P_DAC_STS) >> 4) & 0x0FFF;

	return RTN_OK;
}

uint8_t ioc::dac_write(uint8_t cmd, uint8_t addr, uint16_t wdata) {
	SIMCK;
	DBG_INFO("\t dac.write : prev[ %08X ]", DAC_CMD_WR | ((cmd & 0x0F) << DAC_LOC_CMD) | ((addr & 0x0F) << DAC_LOC_ADR) | ((wdata & 0x0FFF) << DAC_LOC_DB));
	*P_DAC_CMD = DAC_CMD_WR | ((cmd & 0x0F) << DAC_LOC_CMD) | ((addr & 0x0F) << DAC_LOC_ADR) | ((wdata & 0x0FFF) << DAC_LOC_DB);
	DBG_INFO(" - post[ %08X ]", *P_DAC_CMD);

	return dac_wait_eos();
}

uint8_t ioc::set_hv_dac(int ch, double volt)
{
	double setVolt = 0;

	//volt = (volt - gOPER.m_peDpsVccCal.offset) / gOPER.m_peDpsVccCal.gain;

	if(ch == 2)// VS3
		setVolt = (volt == 0.0 ? 2.366 : (2.366 - ((2.18*volt)/20.0)));
	else if(ch == 5)
		setVolt = (volt/2.0);
	else
		setVolt = (volt/4.0);

	uint16_t dacCode = 0;
    dacCode = (uint16_t)(setVolt / (DAC_SPAN / DAC_RESOLUTION)) & 0xFFF;
	//printf("\t DAC.Set : %s[ %01X ] %s[ %6.3f(%6.3f) ] %s[ %03X ]\n", "ch", ch, "volt", volt, setVolt, "dacCode", dacCode);
	return dac_write(DAC_C_WO_SINREG, ch, dacCode);
}

/* dps */
void ioc::dps_vs3f(uint8_t on) {
	SIMCK2;
	*P_DPS_VS3F = on & 0x1;

	DBG_INFO("\t DPS_VS3F_EN : %s\n", ((*P_DPS_VS3F) & 0x1) == 0 ? "OFF" : " ON");
}

//uint8_t ioc::dps_run(uint8_t dev_no, uint8_t onoff) {
uint8_t ioc::dps_run(uint16_t dev_en_bit, uint8_t onoff) {
	SIMCK;
	uint8_t prev;

	//
	//if (dev_no == 0)
	//	*P_DPS_RUN = (prev & 0xE) | ((onoff & 0x1) << 0);
	//else if (dev_no == 1)
	//	*P_DPS_RUN = (prev & 0xD) | ((onoff & 0x1) << 1);
	//else if (dev_no == 2)
	//	*P_DPS_RUN = (prev & 0xB) | ((onoff & 0x1) << 2);
	//else if (dev_no == 3)
	//	*P_DPS_RUN = (prev & 0x7) | ((onoff & 0x1) << 3);
	//else
	//	return RTN_ARGC_ERR;

	if (dev_en_bit > 0xF) {
		return RTN_ARGC_ERR;
	}

	prev = *P_DPS_RUN;
	if(onoff == 1)
		*P_DPS_RUN = (prev | dev_en_bit) ;
	else
		*P_DPS_RUN = (prev & ~(dev_en_bit)) ;

	DBG_INFO("\t DPS_1 : %s\n", (((*P_DPS_RUN) & 0x1) == 0x0) ? "OFF" : " ON");
	DBG_INFO("\t DPS_2 : %s\n", (((*P_DPS_RUN) & 0x2) == 0x0) ? "OFF" : " ON");
	DBG_INFO("\t DPS_4 : %s\n", (((*P_DPS_RUN) & 0x4) == 0x0) ? "OFF" : " ON");
	DBG_INFO("\t DPS_5 : %s\n", (((*P_DPS_RUN) & 0x8) == 0x0) ? "OFF" : " ON");

	return RTN_OK;
}

uint8_t ioc::dps_read_alert()
{
	SIMCK;
	uint32_t data;
	uint8_t rtn;

	data = *P_DPS_STS;
	rtn = (data >> 24) & 0xF;
	DBG_INFO("\t DPS Alert data : 0x%X\n", rtn);

	return rtn;
}

uint8_t ioc::i2c_addr_scan(uint8_t dev_no, uint8_t* addr_cnt, uint8_t* desired_addr, uint8_t* scaned_addr) {
	SIMCK;

	*P_DPS_CMD = DPS_CMD_SCAN | (dev_no << DPS_LOC_NO);

	uint8_t rv = 0;
	rv = dps_wait_eos();

	if (rv != RTN_OK)
		return rv;

#ifdef _SIM_
	* (P_DPS_DATA + 0) = 0x4;
	*(P_DPS_DATA + 1) = 0xAA;
	*(P_DPS_DATA + 2) = 0x11;
	*(P_DPS_DATA + 3) = 0x22;
	*(P_DPS_DATA + 4) = 0x33;
	*(P_DPS_DATA + 5) = 0x44;
	*(P_DPS_DATA + 6) = 0x55;
#endif

	* addr_cnt = *(P_DPS_DATA + 0);

	if (addr_cnt == 0)
		return RTN_OK;

	*desired_addr = *(P_DPS_DATA + 1);
	//memcpy(scaned_addr, (uint8_t *)(P_DPS_DATA + 2), addr_cnt);
	for (uint32_t i = 0; i < *addr_cnt; i++) {
		*(scaned_addr + i) = *(P_DPS_DATA + 2 + i);
	}

	/*
	DBG_INFO("\t DPS_%d [ %d EA ] : expect[ %02X ] scaned[ ", dev_no, *addr_cnt, *desired_addr);
	for (int i = 0; i < *addr_cnt; i++) {
	    DBG_INFO("%02X ", scaned_addr[i]);
	}
	DBG_INFO("]\n");
	*/

	return RTN_OK;
}

uint8_t ioc::dps_wait_eos(void) {

#if 0
	// dps : 400KHz I2C
	// tSHIFT : 20us * ( N + 3 )Byte < 240us
	uint32_t timeout = DPS_TIMEOUT;

//  *P_DPS_STS = DPS_STS_EOS;
    int i = 0;
    for (; i< 1000 * 10; i++)  // 1000 * 10  10MS
    {
    	int st = *P_DPS_STS;
        if (st & DPS_STS_EOS)break;

        if (i > 1)
        {
            if ((st & DPS_STS_ERR) != 0x0) 
            {
				printf("\t - i2c.err : expect[ %08X ] - sts[ %08X ] cnt[%d]\n", DPS_STS_EOS, st, i);
                DBG_CRIT("\t - i2c.err : expect[ %08X ] - sts[ %08X ] cnt[%d]\n", DPS_STS_EOS, st, i);
            }
        }
        usleep(1);
    }

    if (i >= 1000 * 10)
    {
		printf("\t - i2c timeout\r\n");
        DBG_CRIT("\t - i2c timeout\r\n");
    }

    return RTN_OK;
#else 
	// dps : 400KHz I2C
	// tSHIFT : 20us * ( N + 3 )Byte < 240us
	SIMCK;
	uint32_t timeout = DPS_TIMEOUT;

#ifdef _SIM_
	* P_DPS_STS = DPS_STS_EOS;
#endif
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
	//while ((((*P_DPS_STS) & DPS_STS_EOS) == 0x0) && (--timeout > 0)) {
	//	if (((*P_DAC_STS) & DPS_STS_ERR) != 0x0) {
	//		printf( "i2c.err : expect[ %08X ] - sts[ %08X ] !!!\r\n", DPS_STS_EOS, *P_DPS_STS);
	//		return RTN_ERR;
	//	}
	//}

	if (timeout == 0) {
		printf( "eos.timeout : expect[ %08X ] - sts[ %08X ] !!!\r\n", DPS_STS_EOS, *P_DPS_STS);

		Sleep(0.1);

		return RTN_TIMEOUT;
	}

	return RTN_OK;
#endif

}

uint8_t ioc::dps_read(uint8_t dev_no, uint8_t cmd, uint8_t byte, uint8_t* rbuf) {
	SIMCK;
	*P_DPS_CMD = DPS_CMD_RD | (dev_no << DPS_LOC_NO) | ((byte - 1) << DPS_LOC_BL) | cmd;

	uint8_t rv = 0;
	rv = dps_wait_eos();

	if (rv != RTN_OK)
		return rv;

	//memcpy(rbuf, (uint8_t *)P_DPS_DATA, byte);
	for (uint32_t i = 0; i < byte; i++) {
#ifdef _SIM_
		rbuf[i] = 0x4F + i;
#else
		rbuf[i] = *(P_DPS_DATA + i);
#endif
	}
/*
	if (((*P_DAC_STS) & DPS_STS_ALERT) != 0x0) {
		printf( "DPS : %s[ %08X ] !!!\r\n", "DPS_STS_ALERT", (*P_DPS_STS) & DPS_STS_ALERT);
	}

	if (((*P_DAC_STS) & DPS_STS_FAULT) != 0x0) {
		printf( "DPS : %s[ %08X ] !!!\r\n", "DPS_STS_FAULT", (*P_DPS_STS) );
	}
*/
	return RTN_OK;
}

uint8_t ioc::dps_write(uint8_t dev_no, uint8_t cmd, uint8_t byte, uint8_t* wbuf) {
	SIMCK;
	//memcpy((uint8_t *)P_DPS_DATA, wbuf, byte);
	for (uint32_t i = 0; i < byte; i++) {
		*(P_DPS_DATA + i) = wbuf[i];
	}

	*P_DPS_CMD = DPS_CMD_WR | (dev_no << DPS_LOC_NO) | ((byte - 1) << DPS_LOC_BL) | cmd;

	uint8_t rv = 0;
	rv = dps_wait_eos();
/*
	if (((*P_DPS_STS) & DPS_STS_ALERT) != 0x0) {
		printf( "DPS : %s[ %08X ] !!!\r\n", "DPS_STS_ALERT", (*P_DPS_STS) & DPS_STS_ALERT);
	}

	if (((*P_DPS_STS) & DPS_STS_FAULT) != 0x0) {
		printf( "DPS : %s[ %08X ] !!!\r\n", "DPS_STS_FAULT", (*P_DPS_STS) & DPS_STS_FAULT);
	}
*/
	return rv;
}

uint8_t ioc::dps_write_global(uint8_t dev_no, uint8_t cmd, uint8_t byte, uint8_t *wbuf)
{
	SIMCK;
	// memcpy((uint8_t *)P_DPS_DATA, wbuf, byte);
	for (uint32_t i = 0; i < byte; i++)
	{
		*(P_DPS_DATA + i) = wbuf[i];
	}

	*P_DPS_CMD = DPS_CMD_WR | DPS_CMD_GB | ((1 << dev_no) << DPS_LOC_NO) | ((byte - 1) << DPS_LOC_BL) | cmd;

	uint8_t rv = 0;
	rv = dps_wait_eos();

/*
	if (((*P_DPS_STS) & DPS_STS_ALERT) != 0x0) {
		printf("DPS : %s[ %08X ] !!!\r\n", "DPS_STS_ALERT", (*P_DPS_STS) & DPS_STS_ALERT);
	}

	if (((*P_DPS _STS) & DPS_STS_FAULT) != 0x0) {
		printf("DPS : %s[ %08X ] !!!\r\n", "DPS_STS_FAULT", (*P_DPS_STS) & DPS_STS_FAULT);
	}
*/

	return rv;
}

uint8_t ioc::dps_send_byte(uint8_t dev_no, uint8_t cmd)
{
#if 0
SIMCK;
	*P_DPS_CMD = DPS_CMD_WR | (dev_no << DPS_LOC_NO) | (0 << DPS_LOC_BL) | cmd;

	uint8_t rv = 0;
	rv = dps_wait_eos();

	if (rv != RTN_OK)
		return rv;
#else
	//*P_DPS_CMD = DPS_CMD_RD | (dev_no << DPS_LOC_NO) | ((byte - 1) << DPS_LOC_BL) | cmd;
	*P_DPS_CMD = DPS_CMD_RD | (dev_no << DPS_LOC_NO) | (0 << DPS_LOC_BL) | cmd;
	
	usleep(1000*20);
	return 0;
	uint8_t rv = 0;
	rv = dps_wait_eos();

	if (rv != RTN_TIMEOUT)
		return rv;

	//memcpy(rbuf, (uint8_t *)P_DPS_DATA, byte);
#endif
/*
	if (((*P_DAC_STS) & DPS_STS_ALERT) != 0x0) {
		printf( "DPS : %s[ %08X ] !!!\r\n", "DPS_STS_ALERT", (*P_DPS_STS) & DPS_STS_ALERT);
	}

	if (((*P_DAC_STS) & DPS_STS_FAULT) != 0x0) {
		printf( "DPS : %s[ %08X ] !!!\r\n", "DPS_STS_FAULT", (*P_DPS_STS) );
	}
*/
	return RTN_OK;
}

/* pe */
uint8_t ioc::pe_init()
{
	//uint16_t rdata = 0;
	//uint16_t code = 0;
	uint32_t dev_sel = 0x3FFFFF;
	uint8_t ch_sel = 0xFF; // select all channels
	uint8_t rv = 0;

	//PE Central Setting
	rv = pe_write(dev_sel, 0, PE_C_PDO_RESET, 0x2200); 
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, 0, PE_C_DIAG,      0x0000); // DIAG DISABLE
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, 0, PE_C_IREF_CAL,  0x0000); // REF  DISABLE
	if(rv != RTN_OK) return rv;

	//PE All Channel Setting
	rv = pe_write(dev_sel, ch_sel, PE_CH_POWER_ON_CTRL  , 0x0B1E); // OFF
	if(rv != RTN_OK) return rv;	
	rv = pe_write(dev_sel, ch_sel, PE_CH_DATA_EN_CONF   , 0x1C55); //0x0D55 -> 1C55 by Park
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_COMP_CONF      , 0x003A);
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_ROSC_CTRL      , 0x0190);
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_DRV_SLEW       , 0x1F1F); // TR, TF // 0x909F
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_DRV_CTRL       , 0x0E80); // IMPEDANCE 50
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_DATA_EN_PULSE  , 0x0000);
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_COMP_PULSE     , 0x0000);
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_DATA_FINE_DLY  , 0x8000);
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_EN_FINE_DLY    , 0x0000);
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_COMP_FINE_DLY  , 0x0000);
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_CON_DIAG       , 0x0000);
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_OT_CTRL        , 0x4002); // Ext Alarm, OT-en //0x0000);
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_LOW_FORCE_LVL  , 0x4926); // VIL 0.0V
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_HIGH_FORCE_LVL , 0x5B70); // VIH 1.0V
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_COMP_TH	    , 0x524B); // VTH 0.5V
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_SEL_DIAG_0	    , 0x0000); // DIAG DISABLE
	if(rv != RTN_OK) return rv;
	rv = pe_write(dev_sel, ch_sel, PE_CH_SEL_DIAG_1	    , 0x0000); // REF DISABLE
	if(rv != RTN_OK) return rv;

	// pe channel on
	//rv = pe_write(dev_sel, ch_sel, PE_CH_POWER_ON_CTRL, 0x0B1F);
	//if(rv != RTN_OK) return rv;
	//// pe chip off
	//rv = pe_write(dev_sel, ch_sel, PE_C_PDO_RESET, 0x2200);
	//if(rv != RTN_OK) return rv;

	//ResetCalFactor();
	//MakeCalFactor();

	////// PE Low/High CLAMP Level Setting /////////////////
	set_hv_dac(0, 2.5); // VIL CLAMP 0.0V
	set_hv_dac(1, 0.0); // VIH CLAMP 0.0V	
	dac_clamp(true);

	return RTN_OK;

}

void ioc::pe_cal_ctrl(char* sel, uint8_t onoff) {
	SIMCK2;
	uint8_t prev;

	prev = *P_PE_CAL;
	if (strcmp(sel, "low") == 0)
		*P_PE_CAL = (prev & 0x2) | ((onoff & 0x1) << 0);
	else
		*P_PE_CAL = (prev & 0x1) | ((onoff & 0x1) << 1);

	DBG_INFO("\t PE_CAL_HIGH : %s\n", ((*P_PE_CAL) & 0x2) == 0 ? "OFF" : " ON");
	DBG_INFO("\t PE_CAL_LOW  : %s\n", ((*P_PE_CAL) & 0x1) == 0 ? "OFF" : " ON");
}

uint8_t ioc::pe_wait_eos(void) {
	SIMCK;
	// tCK : 40ns
	// tSHIFT : 40-bit * 40ns = 1600ns
	uint32_t timeout = PE_TIMEOUT;

#ifdef _SIM_
	* P_PE_STS = PE_STS_EOS;
#endif
	while ((((*P_PE_STS) & PE_STS_EOS) == 0x0) && (--timeout > 0));

	if (timeout == 0) {
		printf( "eos.timeout : expect[ %08X ] - sts[ %08X ] !!!\r\n", PE_STS_EOS, *P_PE_STS);

		Sleep(0.1);

		return RTN_TIMEOUT;
	}

	return RTN_OK;
}

uint8_t ioc::pe_clear(uint32_t sel) {
	SIMCK;
	*P_PE_SEL = sel;
	*P_PE_CMD = PE_CMD_RST;

	Sleep(0.1);

	uint8_t rv = 0;
	rv = pe_wait_eos();
	*P_PE_SEL = 0x0;

	return rv;
}

uint8_t ioc::pe_read(uint32_t sel, uint8_t ch, uint8_t addr, uint16_t* rdata) {
	SIMCK;
	*P_PE_SEL = sel;
	*P_PE_CMD = PE_CMD_SET | PE_CMD_RD | (ch << PE_LOC_CH) | ((addr & 0x1F) << PE_LOC_ADR);

	uint8_t rv = 0;
	rv = pe_wait_eos();

#ifdef _SIM_
	* P_PE_STS = 0x1234;
#endif

	* rdata = (*P_PE_STS) & 0xFFFF;

	DBG_INFO("\t PE.ALARM : %08X\n", *P_PE_ALARM);

	return rv;
}

uint8_t ioc::pe_write(uint32_t sel, uint8_t ch, uint8_t addr, uint16_t wdata) {
	SIMCK;
	*P_PE_SEL = sel;
	*P_PE_CMD = PE_CMD_SET | PE_CMD_WR | (ch << PE_LOC_CH) | ((addr & 0x1F) << PE_LOC_ADR) | wdata;

	uint8_t rv = 0;

	rv = pe_wait_eos();

	DBG_INFO("\t PE.ALARM : %08X\n", *P_PE_ALARM);
	
	return rv; 
}

uint8_t ioc::pe_set_dvh(uint32_t sel, uint8_t ch, double volt, bool bCalApply)
{
	uint16_t code = (int)((volt + 4.0) * 1000000.0 / 213.6) & 0xFFFF;
	return pe_write(sel, ch, PE_CH_HIGH_FORCE_LVL, code);
	//pe_write(sel, ch, PE_CH_POWER_ON_CTRL, 0x0B1F);
}

uint8_t ioc::pe_set_dvl(uint32_t sel, uint8_t ch, double volt, bool bCalApply)
{
	uint16_t code = (int)((volt + 4.0) * 1000000.0 / 213.6) & 0xFFFF;
	return pe_write(sel, ch, PE_CH_LOW_FORCE_LVL, code);
	//pe_write(sel, ch, PE_CH_POWER_ON_CTRL, 0x0B1F);
}

uint8_t ioc::pe_set_imp(uint32_t sel, uint8_t ch, int imp) 
{
	// uint16_t code = 0x0E80;
	// if (imp >= 26.0)
	//	code = 0x0E80; // 50
	// else if (imp >= 16)
	//	code = 0x0E81; //
	// else if (imp < 16)
	//	code = 0x0E82;

	uint16_t code = 0x0E00;
	uint16_t fine = 0x1F;
	if (imp > 65) {
		fine = 0x0;
	} else if (imp >= 35) {
		fine -= (imp - 35);
	} else if (imp >= 10) {
		code += 1;
		fine -= (imp - 10);
	} else if (imp < 10) {
		code += 2;
		fine -= (imp - 1);
	}

	code |= (fine << 3);

	//printf("imp set = 0x%X\n", code);
	return pe_write(sel, ch, PE_CH_DRV_CTRL, code);
	// pe_write(sel, ch, PE_CH_POWER_ON_CTRL, 0x0B1F);
}

uint8_t ioc::pe_set_vbb(uint32_t sel, uint8_t ch, uint16_t data)
{
	uint16_t rdata = 0x0;
	pe_read(sel, 0x1, PE_CH_POWER_ON_CTRL, &rdata);
	rdata |= ((data & 0xF) << 8);
	//printf("vbb set = 0x%X\n", code);
	return pe_write(sel, ch, PE_CH_POWER_ON_CTRL, rdata);
}

uint8_t ioc::pe_set_slr(uint32_t sel, uint8_t ch, uint16_t data)
{
	//printf("imp set = 0x%X\n", code);
	return pe_write(sel, ch, PE_CH_DRV_SLEW, data);
}

uint8_t ioc::pe_set_vt(uint32_t sel, uint8_t ch, double volt, bool bCalApply)
{
	uint16_t code = (int)((volt + 4.0) * 1000000.0 / 213.6) & 0xFFFF;
	return pe_write(sel, ch, PE_CH_COMP_TH, code);
	//pe_write(sel, ch, PE_CH_POWER_ON_CTRL, 0x0B1F);
}

uint8_t ioc::pe_channel_on(uint32_t sel, uint8_t ch)
{
	return pe_write(sel, ch, PE_CH_POWER_ON_CTRL, 0x0B1F);
	//pe_write(sel,  0, PE_C_PDO_RESET, 0x2201);
}

uint8_t ioc::pe_channel_off(uint32_t sel, uint8_t ch)
{
	return pe_write(sel, ch, PE_CH_POWER_ON_CTRL, 0x0B1E);
	//pe_write(sel,  0, PE_C_PDO_RESET, 0x2200);
}

uint8_t ioc::pe_chip_on(uint32_t sel)
{
	return pe_write(sel,  0, PE_C_PDO_RESET, 0x2201);
}

uint8_t ioc::pe_chip_off(uint32_t sel)
{
	uint8_t rv =  pe_write(sel,  0, PE_C_PDO_RESET, 0x2200);
	return rv;
	
}

uint32_t ioc::pe_alarm_read()
{
	uint32_t status = *P_PE_ALARM;
	return status;
}
uint16_t ioc::pe_alarm_read(uint32_t sel)
{
	uint16_t rdata = 0;
	pe_read(sel,  0, PE_C_EXT_INT_ALARM, &rdata);
	return rdata;
}
///////////////////////////////////////////////////////////////////////////////////

uint8_t ioc::cvm_set_config(bool bReset, uint16_t avg, uint16_t vbusctrl, uint16_t vshctrl, uint8_t mod)
{
	uint32_t data = CVM_CMD_SET | CVM_CMD_WR;
	data |= 0x1 << 14;
	data |= (avg & 0x0007) << 9;
	data |= (vbusctrl & 0x0007) << 6;
	data |= (vshctrl & 0x0007) << 3;
	data |= (mod & 0x0007);

	*P_CVM_CMD = data;

	printf("CVM CFG Write DATA : 0x%08X\n", data);

	uint8_t rv = 0;
	rv = cvm_wait_eos();
	DBG_INFO("\t CVM.Status : %08X\n", *P_CVM_STS);

	return rv;
}

uint8_t ioc::cvm_reset()
{
	SIMCK;
	*P_CVM_CMD = CVM_CMD_RST;
	Sleep(0.1);

	uint8_t rv = 0;
	rv = cvm_wait_eos();
	DBG_INFO("\t CVM.Status : %08X\n", *P_CVM_STS);

	return rv;
}

uint8_t ioc::cvm_wait_eos(void) {
	SIMCK;
	// adc : tSETTLING + tCONV + tREAD
	// tCONV : 4.2 ~ 315us
	// tREAD : 16bit * 4 * 80ns = 5.12us
	uint32_t timeout = CVM_TIMEOUT;

#ifdef _SIM_
	* P_ADC_STS = ADC_STS_EOS;
#endif

	while ((((*P_CVM_STS) & CVM_STS_EOS) == 0x0) && (--timeout > 0)) {
		if (((*P_CVM_STS) & CVM_STS_ERR) != 0x0) {
			printf( "i2c.err : expect[ %08X ] - sts[ %08X ] !!!\r\n", CVM_STS_EOS, *P_CVM_STS);
			return RTN_ERR;
		}
	};

	if (timeout == 0) {
		printf( "eos.timeout : expect[ %03X ] - sts[ %03X ] !!!\r\n", CVM_STS_EOS, *P_CVM_STS);

		DBG_INFO(" - after timeout[ %08X ]", *P_CVM_CMD);

		cvm_reset();
		Sleep(0.1);

		return RTN_TIMEOUT;
	}

	return RTN_OK;
}

uint8_t ioc::cvm_write_cal_reg(uint16_t reg)
{
	uint32_t data = reg;
	//if(reg != 0xFFFF)
	//	data = reg;
	//else 
		data = CVM_CAL_REG_VAL;

	data |= CVM_CMD_SET | CVM_CMD_WR | CVM_REG_CAL;
	*P_CVM_CMD = data;

	printf("CVM Cal Write : 0x%08X\n", data);
	
	uint8_t rv = 0;
	rv = cvm_wait_eos();
	DBG_INFO("\t CVM.Status : %08X\n", *P_CVM_STS);
	return rv;
}

uint8_t ioc::cvm_read_voltage(short& code)
{
	uint32_t data = 0;
	data |= CVM_CMD_SET | CVM_CMD_RD | CVM_REG_VOLTAGE ;
	*P_CVM_CMD = data;

	Sleep(0.100);

	uint8_t rv = 0;
	rv = cvm_wait_eos();
	if (rv != RTN_OK)
		return rv;

	data = (*P_CVM_STS);
	code = (data & 0xFFFF);
	return RTN_OK;
}

uint8_t ioc::cvm_read_current(short& code)
{
	uint32_t data = 0;
	data |= CVM_CMD_SET | CVM_CMD_RD | CVM_REG_CURRENT ;
	*P_CVM_CMD = data;

	Sleep(0.100);

	uint8_t rv = 0;
	rv = cvm_wait_eos();
	if (rv != RTN_OK)
		return rv;

	data = (*P_CVM_STS);

	printf("cvm shnt reg code value : 0x%X\n", data);

	code = (data & 0xFFFF);

	return RTN_OK;
}

///////////////////////////////////////////////////////////////////////////////////

volatile byte* ioc::getAdr(uint adr)
{
	return (m_pBaseAdr + adr);
}

void ioc::writeIO(uint adr, uint value)
{
	SIMCK2;
	uint* base = (uint*)getAdr(adr);
	*base = value;
}

uint ioc::readIO(uint adr)
{
	SIMCK;
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

	uint8_t ret = RTN_OK;
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
	uint8_t ret = RTN_OK;
	if (sz == 1) {
		uint8_t v;
		ret = c2p_rb(0, adr, &v);
		uv = v;
	}
	else if (sz == 2)
	{
		uint16_t v;
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
	//uint8_t ret = c2p_read(0, 0, 0, adr, sz, pData);
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

void ioc::printDBG(uint adr, uint val, int sz, uint8_t ret)
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
