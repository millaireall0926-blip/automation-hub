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
#define Sleep(a)   usleep(a*1000000)

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

enum PCI_SEL {
	PCI1 = 0,
	PCI2 = 1,
	PCI_ALL = 0xff,
};


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

	uint8_t MinorRev;
	uint8_t MajorRev;
	uint8_t RevDummy[2];

	uint8_t Date;
	uint8_t Month;
	uint8_t Year;
	uint8_t DateDummy;	// Total 24B
} IP_INFO, * P_IP_INFO;

#define MAX_PCI 0x2

#define BRD_BASE    0x00000000
#define CFG_BASE    0x00001000
#define C2P_BASE    0x00002000
#define C2T_BASE    0x00003000
#define ADC_BASE    0x00004000
//#define DAC_BASE    0x00005000
//#define DPS_BASE    0x00006000
#define DAC_BASE    0x0000A000
#define DPS_BASE    0x0000B000
//#define PE_BASE     0x00007000
//#define CVM_BASE    0x00009000
#define COMI2C_BASE  0x0000C000
#define PROM_BASE    0x0000E000
#define PJT_BASE    0x0000F000

#define CFG_BUF_BASE  0x00010000
#define C2P_BUF_BASE  0x00020000
#define C2T_BUF_BASE  0x00030000


/* timeout */
// CFG : 3ms(2ms for 64KB transfer @ 33.3MHz)
// LB : 1ms(700us for 64KB transfer @ 100MHz)
// ADC : tSETTLING + tCONV(4.2~315us) + tREAD(5.1us @ 12.5MHz)
// I2C : 1ms(20us * (3+N-Byte) transfer @ 400KHz)
// PE : 3us(1.8us for 40b shift @ 25MHz)
#define CFG_TIMEOUT			1000000 * 5   // org * 5
#define LB_TIMEOUT			100000
#define ADC_TIMEOUT			1000000
#define DAC_TIMEOUT			500000
#define DPS_TIMEOUT			1000000
#define PE_TIMEOUT			1000000
#define CVM_TIMEOUT			1000000
#define PROM_TIMEOUT		1000000


/* brd */



/* cfg */
#define MAX_PG_NUM    2
#define MAX_TG_NUM	  1

#define MAX_CFG_SIZE  65536
#define MAX_ERASE_SIZE  65536
#define MAX_CFG_PROM_SIZE  128 //256


#define CFG_CMD_RST		1 << 31
#define CFG_CMD_LAST	1 << 25
#define CFG_CMD_START 1 << 24

#define CFG_LOC_START	24
#define CFG_LOC_PSEL	20
#define CFG_LOC_TSEL	16

#define CFG_STS_EOS		1 << 3
#define CFG_STS_BUSY	1 << 0
#define CFG_STS_DONE	1 << 4
#define CFG_STS_ERR		0xC0

#define PROM_STS_EOS	1 << 7
#define PROM_STS_ERR	1 << 3
#define PROM_STS_BUSY	1 << 0


/* lb-c2p */
#define MAX_C2P_NUM	  1

#define MAX_C2P_SIZE  65536

#define LB_CMD_RST		1 << 31
#define LB_CMD_DDR		1 << 28
#define LB_CMD_WR		  1 << 25
#define LB_CMD_RD	  	1 << 24

#define LB_LOC_MSB		26
#define LB_LOC_SEL		16

#define LB_STS_EOS		1 << 3
#define LB_STS_ERR		0xF0


/* adc */
#define MAX_ADC_NUM	    2
#define MAX_ADC_MUX	    16
#define MAX_ADC_CH	    8
#define ADC_SPAN        10.0  // +/- 5V
#define ADC_RESOLUTION  65535.0

#define ADC_CMD_RST		1 << 8
#define ADC_CMD_CLR		1 << 5
#define ADC_CMD_SET		1 << 4

#define ADC_LOC_SEL		6

#define ADC_STS_EOS		0x100


/* dac */
#define MAX_DAC_NUM	    2
#define MAX_DAC_CH	    32
//#define DAC_SPAN        2.5  // 0 ~ 5V
#define DAC_SPAN        5.0  // 0 ~ 5V
#define DAC_RESOLUTION  4095.0

#define DAC_CMD_WR		0x0 << 20//0x80 << 24
#define DAC_CMD_RD		0x1 << 20//0x81 << 24
#define DAC_CMD_RST		0x1 << 26// Reset Logic IP //0x40 << 24 
#define DAC_CMD_IPRST	0x1 << 27// Reset Logic IP //0x40 << 24 
#define DAC_CMD_DAC0	0x1 << 24// Reset Logic IP //0x40 << 24 
#define DAC_CMD_CLR		0x1 << 14//0x02 << 24

#define DAC_LOC_SFR   14
#define DAC_LOC_CH    14
#define DAC_LOC_REG   12


#define MAX_VTT		8
#define MAX_IOVTT	8
#define MAX_VREF	8
#define MAX_IOVP	2
#define MAX_DRVP	2
#define MAX_VTH		2
//#define DAC_REG_SFR    0x0
//#define DAC_REG_GAIN   0x1
//#define DAC_REG_OFFSET 0x2
//#define DAC_REG_INPUT  0x3
//#define DAC_LOC_CMD		20
//#define DAC_LOC_ADR		16
//#define DAC_LOC_DB		4

enum DAC_SFR_CMD {
	NOP = 0,
	WRITE_CLR = 1,
	SOFT_CLR = 2,
	POW_DN = 8,
	POW_UP = 9,
	CTRL_REG_WR = 0xC,
	CTRL_REG_RD = 0x4C,
	MON_CH = 0xA,
	SOFT_RST = 0xF,
};

enum DAC_REG_SEL {
	SFR = 0,
	GAIN = 1,
	OFFSET = 2,
	INPUT = 3,
};

#define DAC_STS_EOS		0x08 << 24
#define DAC_STS_ERR		0x40 << 24   // bitcharts에 현재 없음 확인 필요

#define DAC_C_RW_INREG    0x0
#define DAC_C_RW_DACREG   0x1
#define DAC_C_WO_ALLREG   0x2
#define DAC_C_WO_SINREG   0x3


/* dps debug only*/
#define DPS_START_NU	0 //4
#define DPS_START_CH	0 //18
#define DBG_PCI_NU 		0 //0x1

/* dps */
#define MAX_DPS_DEVICE	    5
#define MAX_DPS_CH		20

#define DPS_CMD_WR		1 << 31
#define DPS_CMD_RD		1 << 30
#define DPS_CMD_SEND	1 << 29
#define DPS_CMD_GB		1 << 28 // GLOBAL WRITE
#define DPS_CMD_SCAN	1 << 24

#define DPS_LOC_NO	  16
#define DPS_LOC_BL		8

#define DPS_STS_EOS		1 << 7
#define DPS_STS_ERR		1 << 3
#define DPS_STS_ALERT	0x0F << 24
#define DPS_STS_FAULT	0x0F << 16

#define INA_CFG_REG		00
#define INA_SHUNT_REG	01
#define INA_BUS_REG		02
#define INA_PWR_REG		03
#define INA_CUR_REG		04
#define INA_CAL_REG		05
#define INA_MASK_REG	06
#define INA_LIM_REG		07
#define INA_ID_REG		0xFF


/* pe */
#define MAX_PE_NUM	    22
#define MAX_PE_CH	176

#define PE_CMD_SET		1 << 31
#define PE_CMD_WR	  	1 << 30
#define PE_CMD_RD 		0 << 30
#define PE_CMD_RST		1 << 29

#define PE_LOC_CH     21
#define PE_LOC_ADR    16

#define PE_STS_EOS		1 << 31


#define CVM_REG_SHUNT		1 << 16
#define CVM_REG_VOLTAGE		2 << 16
#define CVM_REG_POWER		3 << 16
#define CVM_REG_CURRENT		4 << 16
#define CVM_REG_CAL			5 << 16
#define CVM_REG_ALMASK		6 << 16
#define CVM_REG_ALLIMIT		7 << 16
#define CVM_REG_FAC_ID		0xFE << 16
#define CVM_REG_DIE_ID		0xFF << 16

#define INA_CMD_SET		1 << 31
#define INA_CMD_RST		1 << 16
#define INA_CMD_WR	  	DPS_CMD_WR
#define INA_CMD_RD 		DPS_CMD_RD
#define CVM_CAL_REG_VAL 0x1234
//#define CVM_CUR_LSB		0.000366211
#define INA_CUR_LSB		0.0 // 값 확인 필요

#define CVM_STS_EOS		1 << 31
#define CVM_STS_ERR		1 << 30
#define CVM_MAX_EXP_CURRENT	 12

#define COMI2C_STS_EOS 1 << 7
#define COMI2C_STS_ERR 1 << 3

/// <summary>
/// BIB 4 FPGA 
/// </summary>
#define PGI_PJT_INFO			0xF000

#define PGI_CFG_CTRL			0x1000
	#define PGI_CFG_CTRL_CMD		PGI_CFG_CTRL | 0x20
		#define BIT_CFG_RESET		0x80000000
	#define PGI_CFG_CTRL_STAT		PGI_CFG_CTRL | 0x30
	

#define PGI_CFG_BUF				0x10000


#define PGI_PG_CTRL			0x2000
	#define PGI_ADR			PGI_PG_CTRL | 0x20
	#define PGI_CMD			PGI_PG_CTRL | 0x24
			#define POS_SEL			0x10     
			#define PG_0			0x1
			#define PG_1			0x2
			#define PG_ALL			0x3 //0xf
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
	int pcie_fd[MAX_PCI];

	void* pcie_map_base[MAX_PCI];
	
	volatile  uint8_t* P_PJT_BASE[MAX_PCI];
	volatile  uint8_t* P_BRD_BASE[MAX_PCI];
	volatile  uint8_t* P_CFG_BASE[MAX_PCI];
	volatile  uint8_t* P_C2P_BASE[MAX_PCI];
	volatile  uint8_t* P_C2T_BASE[MAX_PCI];
	volatile  uint8_t* P_ADC_BASE[MAX_PCI];
	volatile  uint8_t* P_DAC_BASE[MAX_PCI];
	volatile  uint8_t* P_DPS_BASE[MAX_PCI];
	volatile  uint8_t* P_PROM_BASE[MAX_PCI];
	volatile  uint8_t* P_COMI2C_BASE[MAX_PCI];

	//volatile  uint8_t* P_PE_BASE[MAX_PCI];
	//volatile  uint8_t* P_CVM_BASE[MAX_PCI];


	volatile  uint8_t* P_BRD_ID[MAX_PCI];
	volatile  uint8_t* P_BRD_INUSE[MAX_PCI];
	volatile  uint8_t* P_BRD_BIBSEL[MAX_PCI];
	volatile  uint8_t* P_BRD_BIBID[MAX_PCI];
	//volatile  uint8_t* P_BRD_SITEID[MAX_PCI];
	volatile  uint16_t* P_BRD_RACTRL[MAX_PCI];
	volatile  uint16_t* P_BRD_TSENSOR0[MAX_PCI];
	volatile  uint16_t* P_BRD_TSENSOR1[MAX_PCI];
	volatile  uint16_t* P_BRD_TSENSOR2[MAX_PCI];
	volatile  uint16_t* P_BRD_TSENSOR3[MAX_PCI];

	volatile  uint8_t* P_CFG_BUF[MAX_PCI];
	volatile uint32_t* P_CFG_CMD[MAX_PCI];
	volatile uint32_t* P_CFG_STS[MAX_PCI];
	//volatile uint16_t* P_CFG_DONE[MAX_PCI];

	volatile  uint8_t* P_C2P_BUF[MAX_PCI];
	volatile uint32_t* P_C2P_ADDR[MAX_PCI];
	volatile uint32_t* P_C2P_CMD[MAX_PCI];
	volatile uint32_t* P_C2P_LB_CLK_SEL[MAX_PCI];
	volatile  uint8_t* P_C2P_RL[MAX_PCI];
	volatile uint16_t* P_C2P_STS[MAX_PCI];

	volatile  uint8_t* P_C2T_BUF[MAX_PCI];
	volatile uint32_t* P_C2T_ADDR[MAX_PCI];
	volatile uint32_t* P_C2T_CMD[MAX_PCI];
	volatile uint32_t* P_C2T_LB_CLK_SEL[MAX_PCI];
	volatile  uint8_t* P_C2T_RL[MAX_PCI];
	volatile uint16_t* P_C2T_STS[MAX_PCI];

	volatile uint16_t* P_ADC_CMD[MAX_PCI];
	volatile uint32_t* P_ADC_SETTL[MAX_PCI];
	volatile  uint8_t* P_ADC_RANGE[MAX_PCI];
	volatile  uint8_t* P_ADC_OS[MAX_PCI];
	volatile uint16_t* P_ADC_STS[MAX_PCI];
	volatile uint16_t* P_ADC_DATA[MAX_PCI];

	volatile uint32_t* P_DAC_CMD[MAX_PCI];
	volatile uint32_t* P_DAC_STS[MAX_PCI];
	//volatile uint32_t* P_DAC_CLP[MAX_PCI];

	volatile uint32_t* P_DPS_CMD[MAX_PCI];
	volatile uint32_t* P_DPS_EN[MAX_PCI];
	volatile uint32_t* P_DPS_RUN[MAX_PCI];
	volatile uint16_t* P_DPS_SYNC[MAX_PCI];
	volatile  uint8_t* P_DPS_STS[MAX_PCI];
	volatile uint32_t* P_DPS_PPS_FAULT[MAX_PCI];
	volatile uint32_t* P_DPS_VTT_FAULT[MAX_PCI];
	volatile  uint8_t* P_DPS_DATA[MAX_PCI];

	//volatile uint32_t* P_PE_SEL[MAX_PCI];
	//volatile uint32_t* P_PE_CMD[MAX_PCI];
	//volatile  uint8_t* P_PE_CAL[MAX_PCI];
	//volatile uint32_t* P_PE_STS[MAX_PCI];
	//volatile uint32_t* P_PE_ALARM[MAX_PCI];

	volatile uint32_t* P_PROM_ADDR[MAX_PCI];
	volatile uint16_t* P_PROM_CMD[MAX_PCI];
	volatile  uint8_t* P_PROM_BOOT[MAX_PCI];
	volatile  uint8_t* P_PROM_STS[MAX_PCI];
	volatile  uint8_t* P_PROM_DATA[MAX_PCI];

	volatile uint32_t* P_COMI2C_CMD;
	volatile uint32_t* P_COMI2C_STS;
	volatile  uint8_t* P_COMI2C_BUF;

	//volatile uint32_t* P_CVM_CMD[MAX_PCI];
	//volatile uint32_t* P_CVM_STS[MAX_PCI];

	volatile uint8_t*	P8_BUF_BASE[MAX_PCI];
	volatile uint16_t*	P16_BUF_BASE[MAX_PCI];
	volatile uint32_t*	P32_BUF_BASE[MAX_PCI];

	volatile uint8_t*	P8_LED[MAX_PCI];
	volatile uint8_t*	P8_SW[MAX_PCI];

	//int m_nPCI_SEL;

public:	
	ioc() { m_bSim = false; m_bDebug = false; m_bHideDPSErr = false; /*m_nPCI_SEL = -1; */ }
	~ioc() { close_pcie(); }

	bool m_bSim;
	bool m_bDebug;

	bool m_bHideDPSErr;
	void printe(const char* str, ...);

	uint m_nBFPGA;  // FPAG_MODE is Default FPGA_DEFAULT

	vector<errmsg> m_sIOErrVec; // Manage errors separately for Using IO Class in Other Programs

	/* pcie control */
	int open_pcie(void);
	void close_pcie(void);

	int aligne_pcie(void);


	/* print */
	void print_ip_info(void* base);
	void printf_all_ip(void);

	


	/* brd */
	void brd_info(PCI_SEL pci);

	uint8_t readInuse();
	uint readBID();
	uint readSlotID();
	//bool checkBFPGA(); // 4 FPGA BIB  Check

	//int readSiteID(PCI_SEL pci);

	/* cfg */
	void cfg_reset(PCI_SEL pci);
	uint8_t cfg_wait_eos(PCI_SEL pci);
	uint8_t cfg_wait_busy(PCI_SEL pci);
	uint8_t cfg_wait_done(PCI_SEL pci);
	int cfg_fpga(PCI_SEL pci, const char* bitstream, bool bPG = true);
	int cfg_ctrl(uint32_t ctrl_sel, const char* bitstream);

	uint8_t prom_wait_eos(PCI_SEL pci);
	uint8_t prom_wait_busy(PCI_SEL pci);
	int erase_ctrl(uint32_t ctrl_sel, int size);
	int reboot_ctrl(PCI_SEL pci);

	/* cfg Interface*/
	void cfgi_reset();
	FUNC_RTN cfgi_check_wait(PCI_SEL pci, bool bLast);
	FUNC_RTN cfgi_wait(PCI_SEL pci, bool bLast);	// eos , busy wait
	int cfgi_fpga(PCI_SEL pci, const char* bitstream, uint sel = PG_ALL);
	


	/* lb */
	void c2p_reset(PCI_SEL pci);
	void c2p_lb_clk_sel(uint32_t sel);
	uint8_t c2p_wait_eos(PCI_SEL pci, uint8_t pg_sel); // default 0
	uint8_t c2p_read(PCI_SEL pci, uint8_t pg_sel, uint8_t ddr, uint8_t ddr_msb, uint32_t addr, uint16_t bl, uint8_t* rbuf);
	uint8_t c2p_write(PCI_SEL pci, uint8_t pg_sel, uint8_t ddr, uint8_t ddr_msb, uint32_t addr, uint16_t bl, uint8_t* wbuf);

	uint8_t c2p_wb(PCI_SEL pci, uint8_t pg_sel, uint32_t addr, uint8_t wdata);
	uint8_t c2p_wh(PCI_SEL pci, uint8_t pg_sel, uint32_t addr, uint16_t wdata);
	uint8_t c2p_ww(PCI_SEL pci, uint8_t pg_sel, uint32_t addr, uint32_t wdata, int sz = 3);

	uint8_t c2p_rb(PCI_SEL pci, uint8_t pg_sel, uint32_t addr, uint8_t* rdata);
	uint8_t c2p_rh(PCI_SEL pci, uint8_t pg_sel, uint32_t addr, uint16_t* rdata);
	uint8_t c2p_rw(PCI_SEL pci, uint8_t pg_sel, uint32_t addr, uint32_t* rdata, int sz = 3);


	void c2t_reset(PCI_SEL pci);
	void c2t_lb_clk_sel(uint32_t sel);
	void c2t_read_latency(uint8_t latency);
	uint8_t c2t_wait_eos(PCI_SEL pci); // default 0
	uint8_t c2t_read(PCI_SEL pci, uint32_t addr, uint16_t bl, uint8_t* rbuf);
	uint8_t c2t_write(PCI_SEL pci, uint32_t addr, uint16_t bl, uint8_t* wbuf);

	uint8_t c2t_burst_write(PCI_SEL pci, uint32_t addr, uint8_t * wdata, int size);
	uint16_t c2t_burst_read(PCI_SEL pci, uint32_t addr, uint8_t * wdata, int size);
	uint8_t c2t_wb(PCI_SEL pci, uint32_t addr, uint8_t wdata);
	uint8_t c2t_wh(PCI_SEL pci, uint32_t addr, uint16_t wdata);
	uint8_t c2t_ww(PCI_SEL pci, uint32_t addr, uint32_t wdata, int sz = 3);

	uint8_t c2t_rb(PCI_SEL pci, uint32_t addr, uint8_t* rdata);
	uint8_t c2t_rh(PCI_SEL pci, uint32_t addr, uint16_t* rdata);
	uint8_t c2t_rw(PCI_SEL pci, uint32_t addr, uint32_t* rdata, int sz = 3);



	/* adc */
	void adc_reset(PCI_SEL pci);
	//void adc_range(uint8_t dev_no, uint8_t range);
	//void adc_os(uint8_t dev_no, uint8_t os);
	void adc_range(PCI_SEL pci, uint8_t range);
	void adc_os(PCI_SEL pci,  uint8_t os);
	void adc_settling(PCI_SEL pci, uint32_t nsec);
	uint8_t adc_wait_eos(PCI_SEL pci);
	uint8_t adc_clear(PCI_SEL pci);
	uint8_t adc_read(PCI_SEL pci, uint8_t mux_no, short* code);


	/* dac */
	void dac_reset(PCI_SEL pci);
	//void dac_clamp(bool bOn);
	uint8_t dac_wait_eos(PCI_SEL pci);
	uint8_t dac_clear(PCI_SEL pci);
	uint8_t dac_read(PCI_SEL pci, DAC_REG_SEL reg, DAC_SFR_CMD cmd, uint16_t* rdata);
	uint8_t dac_write(PCI_SEL pci, DAC_REG_SEL reg, DAC_SFR_CMD cmd, uint16_t wdata);
	uint8_t dac_write(PCI_SEL pci, DAC_REG_SEL reg, uint8_t ch, uint16_t wdata);
	uint8_t set_hv_dac(PCI_SEL pci, double volt);

	uint8_t dac_vref_en(PCI_SEL pci, bool en);
	uint8_t dac_vtt_bias_en(PCI_SEL pci, bool en);
	uint8_t dac_pps_12_en(PCI_SEL pci, bool en);

	/* dps */
	//uint8_t dps_run(uint8_t dev_no, uint8_t onoff);
	uint8_t dps_run(PCI_SEL pci, uint32_t dev_en_bit, uint32_t vpp_en_bit, uint8_t onoff);
	uint8_t dps_wait_eos(PCI_SEL pci);
	uint8_t dps_read(PCI_SEL pci, uint8_t dev_no, uint8_t cmd, uint8_t byte, uint8_t* rbuf);
	uint8_t dps_write(PCI_SEL pci, uint8_t dev_no, uint8_t cmd, uint8_t byte, uint8_t* wbuf);
	uint8_t dps_write_global(PCI_SEL pci, uint8_t dev_no, uint8_t cmd, uint8_t byte, uint8_t *wbuf);
	uint8_t dps_send_byte(PCI_SEL pci, uint8_t dev_no, uint8_t cmd);
	uint8_t dps_read_alert(PCI_SEL pci);

	uint8_t dps_en(PCI_SEL pci, int mode, int ch, bool en);

	uint8_t i2c_addr_scan(PCI_SEL pci, uint8_t dev_no, uint8_t* addr_cnt, uint8_t* desired_addr, uint8_t* scaned_addr);

	uint32_t read_dps_run(PCI_SEL pci);
	uint32_t read_dps_sync(PCI_SEL pci);
	uint32_t read_dps_sts(PCI_SEL pci);
	uint32_t read_dps_fault(PCI_SEL pci);


	/* pe */
	//void pe_cal_ctrl(char* sel, uint8_t onoff);
	//uint8_t pe_init();
	//uint8_t pe_wait_eos(void);
	//uint8_t pe_clear(uint32_t sel);
	//uint8_t pe_read(uint32_t sel, uint8_t ch, uint8_t addr, uint16_t* rdata);
	//uint8_t pe_write(uint32_t sel, uint8_t ch, uint8_t addr, uint16_t wdata);
	//uint8_t pe_set_dvh(uint32_t sel, uint8_t ch, double volt, bool bCalApply = false);
	//uint8_t pe_set_dvl(uint32_t sel, uint8_t ch, double volt, bool bCalApply = false);
	//uint8_t pe_set_imp(uint32_t sel, uint8_t ch, int imp);
	//uint8_t pe_set_vt(uint32_t sel, uint8_t ch, double volt, bool bCalApply = false);
	//uint8_t pe_channel_on(uint32_t sel = 0x3FFFFF, uint8_t ch = 0xFF);
	//uint8_t pe_channel_off(uint32_t sel = 0x3FFFFF, uint8_t ch = 0xFF);
	//uint8_t pe_chip_on(uint32_t sel = 0x3FFFFF);
	//uint8_t pe_chip_off(uint32_t sel = 0x3FFFFF);
	//uint32_t pe_alarm_read();
	//uint16_t pe_alarm_read(uint32_t sel);
	//void pe_set_cal_data(int type, int ch, float gain, float offset );

	/* CVM */
	// default config value
	// avg(2) : 16 count
	// vbusctrl(4) : 1.1ms
	// vshctrl(4) : 1.1ms
	// mode(7) : Shunt and Bus continous measure
	uint8_t ina_set_config(PCI_SEL pci,int ch,  bool bReset = 0, uint16_t avg = 2, uint16_t vbusctrl = 4, uint16_t vshctrl = 4, uint8_t mod = 7);
	uint8_t ina_reset(PCI_SEL pci, int ch);
	uint8_t ina_wait_eos(PCI_SEL pci);
	uint8_t ina_write_cal_reg(PCI_SEL pci, int ch, uint16_t reg = 0xFFFF);
	uint8_t ina_read_voltage(PCI_SEL pci, int ch, short& code);
	uint8_t ina_read_current(PCI_SEL pci, int ch, short& code);

	/* Common Access*/

	//volatile byte* m_pBaseAdr[MAX_PCI];
	volatile void* getAdr(PCI_SEL pci, uint adr);
	void  writeIO(PCI_SEL pci, uint adr, uint value);
	uint  readIO(PCI_SEL pci, uint adr);


	void writePG(PCI_SEL pci, byte PGSel, uint adr, uint val, int sz = 4);
	void writePGD(PCI_SEL pci, byte PGSel, uint adr, uint val, int sz = 4);					// Direct
	void writePGI(PCI_SEL pci, byte PGSel, uint adr, uint val, int sz = 4);		// Interface 

	void writePGData(PCI_SEL pci, byte PGSel, uint adr, byte* pData, int sz);
	void writePGDataD(PCI_SEL pci, byte PGSel, uint adr, byte* pData, int sz);				// Direct 
	void writePGDataI(PCI_SEL pci, byte PGSel, uint adr, byte* pData, int sz);	// Inaterface

	uint readPG(PCI_SEL pci, byte PGSel, uint adr, int sz = 4);	
	uint readPGI(PCI_SEL pci, byte PGSel, uint adr, int sz = 4);
	byte pgi_wait_eos(PCI_SEL pci, byte PGSel);
	void addErr(byte PGSel, const char* func,  uint adr, int sz);
	
	void readPGData(PCI_SEL pci, byte PGSel, uint adr, byte* pData, int sz);	
	void readPGDataI(PCI_SEL pci, byte PGSel, uint adr, byte* pData, int sz);

	void writeTG(PCI_SEL pci, uint adr, uint val, int sz = 4);
	void writeTGD(PCI_SEL pci, uint adr, uint val, int sz = 4);					// Direct
	uint readTG(PCI_SEL pci, uint adr, int sz = 4);	

	void printDBG(uint adr, uint val, int sz, uint8_t ret);
	bool checkPG(PCI_SEL pci, uint compID = 0xFFFFFFFF);

	bool checkTG(PCI_SEL pci, uint compID = 0xFFFFFFFF);

	uint8_t writeComI2c(uint8_t addr, uint8_t addr_slb, uint8_t size, uint8_t* wbuf);
	uint8_t readComI2c(uint8_t addr, uint8_t addr_slb, uint8_t size, uint8_t* wbuf);



	/* BIB 4 FPGA InterFace*/
	bool checkPGInterface(PCI_SEL pci );


};

#endif

