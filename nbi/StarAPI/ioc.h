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

	uint8_t MinorRev;
	uint8_t MajorRev;
	uint8_t RevDummy[2];

	uint8_t Date;
	uint8_t Month;
	uint8_t Year;
	uint8_t DateDummy;	// Total 24B
} IP_INFO, * P_IP_INFO;




#define BRD_BASE    0x00000000
#define CFG_BASE    0x00001000
#define C2P_BASE    0x00002000
#define C2T_BASE    0x00003000
#define ADC_BASE    0x00004000
#define DAC_BASE    0x00005000
#define DPS_BASE    0x00006000
#define PE_BASE     0x00007000
#define CVM_BASE    0x00009000
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


/* brd */



/* cfg */
#define MAX_PG_NUM	  1
#define MAX_TG_NUM	  0

#define MAX_CFG_SIZE  65536

#define CFG_CMD_RST		1 << 31
#define CFG_CMD_LAST	1 << 25
#define CFG_CMD_START 1 << 24

#define CFG_LOC_START	24
#define CFG_LOC_PSEL	20
#define CFG_LOC_TSEL	16

#define CFG_STS_EOS		1 << 3
#define CFG_STS_BUSY	1 << 0
#define CFG_STS_ERR		0xC0


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
#define MAX_ADC_MUX	    8
#define ADC_SPAN        10.0  // +/- 5V
#define ADC_RESOLUTION  65535.0

#define ADC_CMD_RST		1 << 8
#define ADC_CMD_CLR		1 << 5
#define ADC_CMD_SET		1 << 4

#define ADC_LOC_SEL		6

#define ADC_STS_EOS		0x100


/* dac */
#define MAX_DAC_NUM	    1
#define MAX_DAC_CH	    8
#define DAC_SPAN        2.5  // 0 ~ 5V
#define DAC_RESOLUTION  4095.0

#define DAC_CMD_WR		0x80 << 24
#define DAC_CMD_RD		0x81 << 24
#define DAC_CMD_RST		0x40 << 24
#define DAC_CMD_CLR		0x02 << 24

#define DAC_LOC_CMD		20
#define DAC_LOC_ADR		16
#define DAC_LOC_DB		4

#define DAC_STS_EOS		0x80 << 24
#define DAC_STS_ERR		0x40 << 24

#define DAC_C_RW_INREG    0x0
#define DAC_C_RW_DACREG   0x1
#define DAC_C_WO_ALLREG   0x2
#define DAC_C_WO_SINREG   0x3


/* dps */
#define MAX_DPS_NUM	    5
#define MAX_DPS_USER_NUM	4

#define DPS_CMD_WR		1 << 31
#define DPS_CMD_RD		1 << 30
#define DPS_CMD_GB		1 << 28 // GLOBAL WRITE
#define DPS_CMD_SCAN	1 << 24

#define DPS_LOC_NO	  16
#define DPS_LOC_BL		8

#define DPS_STS_EOS		1 << 7
#define DPS_STS_ERR		1 << 3
#define DPS_STS_ALERT	0x0F << 24
#define DPS_STS_FAULT	0x0F << 16


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

#define CVM_CMD_SET		1 << 31
#define CVM_CMD_RST		1 << 30
#define CVM_CMD_WR	  	0 << 24
#define CVM_CMD_RD 		1 << 24
#define CVM_CAL_REG_VAL 0x1234
#define CVM_CUR_LSB		0.000366211

#define CVM_STS_EOS		1 << 31
#define CVM_STS_ERR		1 << 30
#define CVM_MAX_EXP_CURRENT	 12


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
	
	volatile  uint8_t* P_PJT_BASE;
	volatile  uint8_t* P_BRD_BASE;
	volatile  uint8_t* P_CFG_BASE;
	volatile  uint8_t* P_C2P_BASE;
	volatile  uint8_t* P_C2T_BASE;
	volatile  uint8_t* P_ADC_BASE;
	volatile  uint8_t* P_DAC_BASE;
	volatile  uint8_t* P_DPS_BASE;
	volatile  uint8_t* P_PE_BASE;
	volatile  uint8_t* P_CVM_BASE;


	volatile  uint8_t* P_BRD_INUSE;
	volatile  uint8_t* P_BRD_BIBSEL;	
	volatile  uint8_t* P_BRD_BIBID;
	volatile  uint8_t* P_BRD_SITEID;

	volatile  uint8_t* P_CFG_BUF;
	volatile uint32_t* P_CFG_CMD;
	volatile  uint8_t* P_CFG_STS;
	volatile uint16_t* P_CFG_DONE;

	volatile  uint8_t* P_C2P_BUF;
	volatile uint32_t* P_C2P_ADDR;
	volatile uint32_t* P_C2P_CMD;
	volatile  uint8_t* P_C2P_RL;
	volatile  uint8_t* P_C2P_STS;

	volatile uint16_t* P_ADC_CMD;
	volatile uint32_t* P_ADC_SETTL;
	volatile  uint8_t* P_ADC_RANGE;
	volatile  uint8_t* P_ADC_OS;
	volatile uint16_t* P_ADC_STS;
	volatile uint16_t* P_ADC_DATA;

	volatile uint32_t* P_DAC_CMD;
	volatile uint32_t* P_DAC_STS;
	volatile uint32_t* P_DAC_CLP;

	volatile uint32_t* P_DPS_CMD;
	volatile  uint8_t* P_DPS_RUN;
	volatile  uint8_t* P_DPS_VS3F;
	volatile uint32_t* P_DPS_STS;
	volatile  uint8_t* P_DPS_DATA;

	volatile uint32_t* P_PE_SEL;
	volatile uint32_t* P_PE_CMD;
	volatile  uint8_t* P_PE_CAL;
	volatile uint32_t* P_PE_STS;
	volatile uint32_t* P_PE_ALARM;

	volatile uint32_t* P_CVM_CMD;
	volatile uint32_t* P_CVM_STS;

	volatile uint8_t*	P8_BUF_BASE;
	volatile uint16_t*	P16_BUF_BASE;
	volatile uint32_t*	P32_BUF_BASE;

	volatile uint8_t*	P8_LED;
	volatile uint8_t*	P8_SW;




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


	/* brd */
	void brd_info(void);

	bool odd_parity(uint8_t byte);
	uint readBID(void);
	bool checkBFPGA(); // 4 FPGA BIB  Check

	int readSiteID(void);

	/* cfg */
	void cfg_reset(void);
	uint8_t cfg_wait_eos(void);
	uint8_t cfg_wait_busy(void);
	int cfg_fpga(const char* bitstream);

	/* cfg Interface*/
	void cfgi_reset();
	FUNC_RTN cfgi_check_wait(bool bLast);
	FUNC_RTN cfgi_wait(bool bLast);	// eos , busy wait
	int cfgi_fpga(const char* bitstream, uint sel = PG_ALL);
	


	/* lb */
	void c2p_reset(void);
	uint8_t c2p_wait_eos(uint8_t dev_no = 0); // default 0
	uint8_t c2p_read(uint8_t dev_no, uint8_t ddr, uint8_t ddr_msb, uint32_t addr, uint16_t bl, uint8_t* rbuf);
	uint8_t c2p_write(uint8_t dev_no, uint8_t ddr, uint8_t ddr_msb, uint32_t addr, uint16_t bl, uint8_t* wbuf);

	uint8_t c2p_wb(uint8_t dev_no, uint32_t addr, uint8_t wdata);
	uint8_t c2p_wh(uint8_t dev_no, uint32_t addr, uint16_t wdata);
	uint8_t c2p_ww(uint8_t dev_no, uint32_t addr, uint32_t wdata, int sz = 3);

	uint8_t c2p_rb(uint8_t dev_no, uint32_t addr, uint8_t* rdata);
	uint8_t c2p_rh(uint8_t dev_no, uint32_t addr, uint16_t* rdata);
	uint8_t c2p_rw(uint8_t dev_no, uint32_t addr, uint32_t* rdata, int sz = 3);


	/* adc */
	void adc_reset(void);
	void adc_range(uint8_t dev_no, uint8_t range);
	void adc_os(uint8_t dev_no, uint8_t os);
	void adc_settling(uint32_t nsec);
	uint8_t adc_wait_eos(void);
	uint8_t adc_clear(uint8_t dev_no);
	uint8_t adc_read(uint8_t dev_no, uint8_t mux_no, short* code);


	/* dac */
	void dac_reset(void);
	void dac_clamp(bool bOn);
	uint8_t dac_wait_eos(void);
	uint8_t dac_clear(void);
	uint8_t dac_read(uint8_t cmd, uint8_t addr, uint16_t* rdata);
	uint8_t dac_write(uint8_t cmd, uint8_t addr, uint16_t wdata);
	uint8_t set_hv_dac(int ch, double volt);


	/* dps */
	void dps_vs3f(uint8_t on);
	//uint8_t dps_run(uint8_t dev_no, uint8_t onoff);
	uint8_t dps_run(uint16_t dev_en_bit, uint8_t onoff);
	uint8_t dps_wait_eos(void);
	uint8_t dps_read(uint8_t dev_no, uint8_t cmd, uint8_t byte, uint8_t* rbuf);
	uint8_t dps_write(uint8_t dev_no, uint8_t cmd, uint8_t byte, uint8_t* wbuf);
	uint8_t dps_write_global(uint8_t dev_no, uint8_t cmd, uint8_t byte, uint8_t *wbuf);
	uint8_t dps_send_byte(uint8_t dev_no, uint8_t cmd);
	uint8_t dps_read_alert();

	uint8_t i2c_addr_scan(uint8_t dev_no, uint8_t* addr_cnt, uint8_t* desired_addr, uint8_t* scaned_addr);


	/* pe */
	void pe_cal_ctrl(char* sel, uint8_t onoff);
	uint8_t pe_init();
	uint8_t pe_wait_eos(void);
	uint8_t pe_clear(uint32_t sel);
	uint8_t pe_read(uint32_t sel, uint8_t ch, uint8_t addr, uint16_t* rdata);
	uint8_t pe_write(uint32_t sel, uint8_t ch, uint8_t addr, uint16_t wdata);
	uint8_t pe_set_dvh(uint32_t sel, uint8_t ch, double volt, bool bCalApply = false);
	uint8_t pe_set_dvl(uint32_t sel, uint8_t ch, double volt, bool bCalApply = false);
	uint8_t pe_set_imp(uint32_t sel, uint8_t ch, int imp);
	uint8_t pe_set_vbb(uint32_t sel, uint8_t ch, uint16_t data);
	uint8_t pe_set_slr(uint32_t sel, uint8_t ch, uint16_t data);
	uint8_t pe_set_vt(uint32_t sel, uint8_t ch, double volt, bool bCalApply = false);
	uint8_t pe_channel_on(uint32_t sel = 0x3FFFFF, uint8_t ch = 0xFF);
	uint8_t pe_channel_off(uint32_t sel = 0x3FFFFF, uint8_t ch = 0xFF);
	uint8_t pe_chip_on(uint32_t sel = 0x3FFFFF);
	uint8_t pe_chip_off(uint32_t sel = 0x3FFFFF);
	uint32_t pe_alarm_read();
	uint16_t pe_alarm_read(uint32_t sel);
	void pe_set_cal_data(int type, int ch, float gain, float offset );

	/* CVM */
	// default config value
	// avg(2) : 16 count
	// vbusctrl(4) : 1.1ms
	// vshctrl(4) : 1.1ms
	// mode(7) : Shunt and Bus continous measure
	uint8_t cvm_set_config(bool bReset = 0, uint16_t avg = 2, uint16_t vbusctrl = 4, uint16_t vshctrl = 4, uint8_t mod = 7);
	uint8_t cvm_reset();
	uint8_t cvm_wait_eos();
	uint8_t cvm_write_cal_reg(uint16_t reg = 0xFFFF);
	uint8_t cvm_read_voltage(short& code);
	uint8_t cvm_read_current(short& code);

	/* zc706 */
	uint8_t zc_wr8(uint32_t addr, uint16_t bl, uint8_t* wbuf);
	uint8_t zc_wr16(uint32_t addr, uint16_t bl, uint8_t* wbuf);
	uint8_t zc_wr32(uint32_t addr, uint16_t bl, uint8_t* wbuf);

	uint8_t zc_rd8(uint32_t addr, uint16_t bl, uint8_t* rbuf);
	uint8_t zc_rd16(uint32_t addr, uint16_t bl, uint8_t* rbuf);
	uint8_t zc_rd32(uint32_t addr, uint16_t bl, uint8_t* rbuf);

	uint8_t zc_led(uint8_t led, uint8_t onoff);
	uint8_t zc_switch(uint8_t* rdata);


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

	void printDBG(uint adr, uint val, int sz, uint8_t ret);
	bool checkPG(uint compID);




	/* BIB 4 FPGA InterFace*/
	bool checkPGInterface();


};

#endif

