#ifndef __IOC__
#define __IOC__
#pragma once


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <byteswap.h>
#include <termios.h>
#include <vector>
#include "define.h"


using namespace std;

typedef unsigned int uint;
typedef unsigned long long int uint64;
typedef unsigned short int	word;
typedef unsigned char byte;


#define MAP_ADDR 0x00000000
#define MAP_SIZE (1*1024*1024UL)
#define MAP_MASK (MAP_SIZE - 1)


#define str2buf	sprintf
#define Sleep   sleep

#define cBlack          "\033[30m"
#define cBlue           "\033[34m"
#define cGreen          "\033[32m"
#define cAqua           "\033[36m"
#define cRed            "\033[31m"
#define cPurple         "\033[35m"
#define cYellow         "\033[33m"
#define cWhite          "\033[37m"
#define cLightBlue      "\033[94m"
#define cLightGreen     "\033[92m"
#define cLightAqua      "\033[96m"
#define cLightRed       "\033[91m"
#define cLightPurple    "\033[95m"
#define cLightYellow    "\033[93m"
#define cBrightWhite    "\033[97m"

#define setColor( color ) fprintf(stderr, "%s", color)

#define DBG_CRIT( fmt, ... )	\
{ \
	setColor( cLightRed ); \
	printf( "[CRIT %s:%s:%d] " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__ ); \
	setColor( cBrightWhite ); \
}

#define _DEBUG_INFO_
#ifdef _DEBUG_INFO_
#define DBG_INFO( fmt, ... )	\
{ \
	if ( m_bDebug) { \
    setColor( cLightYellow ); \
	printf( "" fmt, ##__VA_ARGS__ ); \
	setColor( cBrightWhite ); \
    } \
}
#else
#define DBG_INFO( fmt, ... )
#endif

enum FUNC_RTN {
	RTN_OK = 0,
	RTN_ERR,
	RTN_ARGC_ERR,
	RTN_TIMEOUT,
	RTN_WAIT
};

enum FPGA_MODE {
	FPGA_DEFAULT = 0,
	FPGA_BIB_DRAM,
	FPGA_BIB_MCP

};

/* for CTRL */
typedef struct _IP_INFO {
	char IpName[16];

	byte MinorRev;
	byte MajorRev;
	byte RevDummy[2];

	byte Date;
	byte Month;
	byte Year;
	byte DateDummy;	// Total 24B
} IP_INFO, * P_IP_INFO;




#define BRD_BASE		0x00000000
#define CFG_BASE		0x00001000
#define C2P_BASE		0x00002000
#define ADC_BASE		0x00004000
#define DAC_BASE		0x00005000
#define DPS_BASE		0x00006000
#define INA_BASE		0x00007000

#define EPR_BASE		0x0000D000	// EEPROM
#define PROM_BASE		0x0000E000	// CFG-ROM 
#define PJT_BASE		0x0000F000

#define CFG_BUF_BASE	0x00010000
#define C2P_BUF_BASE	0x00020000
#define ALL_STS_BASE	0x00030000

///////////////////////////
/* timeout	Value		*/
///////////////////////////
// CFG : 3ms(2ms for 64KB transfer @ 33.3MHz)
// LB : 1ms(700us for 64KB transfer @ 100MHz)
// ADC : tSETTLING + tCONV(4.2~315us) + tREAD(5.1us @ 12.5MHz)
// I2C : 1ms(20us * (3+N-Byte) transfer @ 400KHz)
#define CFG_TIMEOUT			1000000 * 5   // org * 5
#define LB_TIMEOUT			100000
#define ADC_TIMEOUT			1000000
#define DAC_TIMEOUT			500000
#define DPS_TIMEOUT			1000000
#define PE_TIMEOUT			1000000
#define CVM_TIMEOUT			1000000

///////////////////////////
/* CFG Value			*/
///////////////////////////
#define MAX_PG_NUM	  1


#define MAX_CFG_SIZE  65536

#define CFG_CMD_RST		1 << 31
#define CFG_CMD_LAST	1 << 25
#define CFG_CMD_START	1 << 24

#define CFG_LOC_START	24
#define CFG_LOC_PSEL	20
#define CFG_LOC_TSEL	16

#define CFG_STS_EOS		1 << 3
#define CFG_STS_BUSY	1 << 0
#define CFG_STS_ERR		0xC0

///////////////////////////
/* C2P Value			*/
///////////////////////////
#define LB_CMD_RST		1 << 31
#define LB_CMD_DDR		1 << 28
#define LB_CMD_WR		1 << 25
#define LB_CMD_RD	  	1 << 24

#define LB_STS_EOS		1 << 3
#define LB_STS_ERR		0xF0

///////////////////////////
/* DAC Value			*/
///////////////////////////
#define DAC_SPAN        2.5  // 0 ~ 5V
#define DAC_RESOLUTION  4095.0


#define DAC_CMD_SET		0x80 << 24
#define DAC_CMD_RST		0x40 << 24



#define DAC_STS_EOS		0x80 << 24
#define DAC_STS_ERR		0x40 << 24

#define DAC_C_RW_INREG    0x0
#define DAC_C_RW_DACREG   0x1
#define DAC_C_WO_ALLREG   0x2
#define DAC_C_WO_SINREG   0x3

///////////////////////////
/* DPS Value			*/
///////////////////////////
#define MAX_DPS_DEVICE	    8
#define MAX_DPS_CH	    (MAX_DPS_DEVICE * 2)


#define DPS_CMD_WR		1 << 28
#define DPS_CMD_RD		1 << 29

#define DPS_LOC_NO	  16
#define DPS_LOC_BL		8

#define DPS_STS_EOS		1 << 31
#define DPS_STS_ERR		1 << 30


//////////////////////////////////
//  P_CFG_BASE   0x0000_0000	//
/////////////////////////////////
#define PGI_PJT_INFO			0xF000

#define PGI_CFG_CTRL			0x1000
#define PGI_CFG_CTRL_CMD		PGI_CFG_CTRL | 0x20
#define BIT_CFG_RESET			0x80000000
#define PGI_CFG_CTRL_STAT		PGI_CFG_CTRL | 0x30
	

#define PGI_CFG_BUF				0x10000


#define PGI_PG_CTRL			0x2000
#define PGI_ADR			PGI_PG_CTRL | 0x20
#define PGI_CMD			PGI_PG_CTRL | 0x24
#define POS_SEL			0x10     
#define PG_ALL			0xf
#define BIT_R			0x1000000
#define BIT_W			0x2000000
#define BIT_RST			0x80000000
#define PGI_STAT		PGI_PG_CTRL | 0x30
#define BIT_ERR			0x1
#define BIT_EOS			0x8
#define PGI_LBSEL		PGI_PG_CTRL | 0x28 // Clock Phase Contorl
#define PGI_LATANCY		PGI_PG_CTRL | 0x2C

#define PGI_PG_BUF				0x20000

struct errmsg {
	char s[256];
};


class ioc
{
protected:

	int pcie_in_progress = 0;
	int pcie_fd;

	void* pcie_map_base;
	
	volatile  byte* P_PJT_BASE;
	volatile  byte* P_BRD_BASE;
	volatile  byte* P_CFG_BASE;
	volatile  byte* P_C2P_BASE;	
	volatile  byte* P_ADC_BASE;
	volatile  byte* P_DAC_BASE;
	volatile  byte* P_DPS_BASE;	
	volatile  byte* P_EPR_BASE;
	

	//////////////////////////////////
	//  P_BRD_BASE   0x0000_0000	//
	/////////////////////////////////
	volatile  byte* P_BRD_TPCB_REV;
	volatile  byte* P_BRD_SLOT_ID;
	volatile  byte* P_BRD_BIB_CHECK;	
	volatile  uint* P_BRD_BIBID_VAL;
	

	volatile  byte* P_CFG_BUF;
	volatile  uint* P_CFG_CMD;
	volatile  byte* P_CFG_STS;
	

	volatile  byte* P_C2P_BUF;
	volatile uint* P_C2P_ADDR;
	volatile uint* P_C2P_CMD;
	volatile  byte* P_C2P_RL;
	volatile  byte* P_C2P_STS;
	
	volatile uint32_t* P_ADC_SETTL;
	volatile short* P_ADC_DATA;

	volatile uint*	P_DAC_CMD;
	volatile uint*	P_DAC_STS;
	volatile byte*	P_DAC_LDAC;

	volatile uint*	P_DPS_CMD;
	volatile  word* P_DPS_RUN;
	volatile  byte* P_DPS_HV;
	volatile  byte* P_DPS_VREF;	
	volatile uint*	P_DPS_STS;
	volatile uint*	P_DPS_FAULT;
	volatile  byte* P_DPS_DATA;

	volatile  byte* P_ALLST_BUF;

	volatile  uint* P_EPR_CMD;
	volatile  uint* P_EPR_STS;
	volatile  byte* P_EPR_BUF;




public:	
	ioc() { m_bSim = false; m_bDebug = false; }
	~ioc() { close_pcie(); }

	bool m_bSim;
	bool m_bDebug;
	uint m_nBFPGA;  // FPAG_MODE is Default FPGA_DEFAULT

	vector<errmsg> m_sIOErrVec; // Manage errors separately for Using IO Class in Other Programs

	/* pcie control */
	int open_pcie(void);
	void close_pcie(void);


	/* print */
	void print_ip_info(void* base);
	void printf_all_ip(void);

	//////////////////////////
	/* BOARD Function		*/
	//////////////////////////
	void brd_info(void);
	uint check_bib(void);		
	uint readBID(void);	
	int readSiteID(void);

	//////////////////////////
	/* cfg					*/
	//////////////////////////
	void cfg_reset(void);
	byte cfg_wait_eos(void);
	byte cfg_wait_busy(void);
	int cfg_fpga(const char* bitstream);

	/* cfg Interface*/
	FUNC_RTN cfgi_check_wait(bool bLast);
	FUNC_RTN cfgi_wait(bool bLast);	// eos , busy wait
	int cfgi_fpga(const char* bitstream, uint sel = PG_ALL);
	

	//////////////////////////
	/* C2P Funtion			*/
	//////////////////////////
	void c2p_reset(void);
	byte c2p_wait_eos(byte dev_no = 0); // default 0
	byte c2p_read(byte dev_no, byte ddr, byte ddr_msb, uint addr, short bl, byte* rbuf);
	byte c2p_write(byte dev_no, byte ddr, byte ddr_msb, uint addr, short bl, byte* wbuf);

	byte c2p_wb(byte dev_no, uint addr, byte wdata);
	byte c2p_wh(byte dev_no, uint addr, short wdata);
	byte c2p_ww(byte dev_no, uint addr, uint wdata, int sz = 3);

	byte c2p_rb(byte dev_no, uint addr, byte* rdata);
	byte c2p_rh(byte dev_no, uint addr, short* rdata);
	byte c2p_rw(byte dev_no, uint addr, uint* rdata, int sz = 3);

	//////////////////////////
	/* ADC Function			*/
	//////////////////////////
	void adc_settling(uint nsec);
	
	
	//////////////////////////
	/* DAC Function			*/
	//////////////////////////
	void dac_reset(void);	
	byte dac_wait_eos(void);		
	byte dac_set(uint ch, word val);	
	byte dac_enable(int type, bool isOn); // type 0 = vh, 1 = vth
	byte dac_ldac(byte no, bool isOn);
	/* 
		dac_ldac(uint no) usage
		1. Set dac_set_register (not enable output yet)
		2. dac_ldac  (enable output group,
						no : 1 = ch0~15, 2 = ch16~31 )
	
	*/

	//////////////////////////
	/* DPS Function			*/	
	//////////////////////////
	byte dps_run(word onoff, bool isOn = true);
	byte dps_hv_enable(byte onoff, bool isOn = true);
	byte dps_vref_enable(byte onoff, bool isOn = true);
	byte dps_wait_eos(void);
	byte dps_read(byte dev_no, byte cmd, byte len, byte* rbuf);
	byte dps_write(byte dev_no, byte cmd, byte len, byte* wbuf);	
	byte dps_send_byte(byte dev_no, byte cmd);
	word dps_read_alert();	


	//////////////////////////
	/* ALL State Function	*/
	//////////////////////////
	void read_all_status(byte* pBuf, int sz);
	void read_one_status(byte* pBuf, int ofs, int sz);

	//////////////////////////
	/* EEPROM Function	*/
	//////////////////////////
	byte epr_wait_eos();
	void eeprom_write(int adr, byte* pBuf, int sz);
	void eeprom_read(int adr, byte* pBuf, int sz);


	/* Common Access*/

	volatile byte* m_pBaseAdr;
	volatile byte* getAdr(uint adr);
	void  writeIO(uint adr, uint value);
	uint  readIO(uint adr);


	void writePG(uint adr, uint val, int sz = 4);
	void writePGD(uint adr, uint val, int sz = 4);					// Direct
	void writePGI(byte PGSel, uint adr, uint val, int sz = 4);		// Interface 

	void writePGData(uint adr, byte* pData, int sz);			
	void writePGDataD(uint adr, byte* pData, int sz);				// Direct 
	void writePGDataI(byte PGSel, uint adr, byte* pData, int sz);	// Inaterface

	uint readPG(uint adr, int sz = 4);	
	uint readPGI(byte PGSel, uint adr, int sz = 4);
	byte pgi_wait_eos(byte PGSel);
	void addErr(byte PGSel, const char* func,  uint adr, int sz);

	
	void readPGData(uint adr, byte* pData, int sz);	
	void readPGDataI(byte PGSel, uint adr, byte* pData, int sz);

	void printDBG(uint adr, uint val, int sz, byte ret);
	bool checkPG(uint compID);


	/* BIB 4 FPGA InterFace*/
	bool checkPGInterface();


};

#endif

