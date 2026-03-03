#ifndef __STAR_TEST__
#define __STAR_TEST__

#define VER_DATE	"2025/09/03"
#define API_VER		"1.4.5"


#include "macro.h"
#include <stdio.h>
#include <map>

/************************************************************
	* argument
	-load	= SETUP excute
	-con	= SETUP, CONTACT excute
	-run	= SETUP, TEST  excute
	-net	= using network
	-sim	= simulation mode 

	ex) sample -con -run -net  ==> setup, Contact, Run Excute

* ***********************************************************/
//////////////////////////////////////////
//		DEFINE VALUE					//
//////////////////////////////////////////
typedef unsigned int uint;
typedef unsigned long long int uint64;
typedef unsigned short int	word;
typedef unsigned char byte;

//////////////////////////////////////////
//			Time						//
//////////////////////////////////////////
#define	PS		* 1.0e-12
#define	NS		* 1.0e-9
#define	US		* 1.0e-6
#define	MS		* 1.0e-3
#define	S		* 1.0
#define	MIN		* 60.0
#define	HOUR	* 3600.0
//////////////////////////////////////////
//			 Power						//
//////////////////////////////////////////
#define	UV		* 1.0e-6
#define	MV		* 1.0e-3
#define	V		* 1.0
#define	UA		* 1.0e-6
#define	MA		* 1.0e-3
#define	A		* 1.0
// DPS
enum
{			
	VS1 = 0x0100,		// LTC
	VS2 = 0x0200,		// LTC
	VS4 = 0x0400,		// LTC 
	VS5 = 0x0800,		// LTC 
	VS3 = 0x1000,		// OTHER 
	PE  = 0x2000,
};

enum {
	DR = 0, IO, SCAN, CLK, ALL
};

//////////////////////////////////////////
//  INSTRUCTION REGISTER SOURCE         //
//////////////////////////////////////////
enum {
	IDX0 = 0,IDX1,IDX2,IDX3,IDX4,IDX5,IDX6,IDX7,IDX8,IDX9,IDX10,IDX11,IDX12,IDX13,IDX14,IDX15,
	XMAX,YMAX, LMAX = 19,HMAX,
	XH,XT,YH,YT,ZRSV2,
	D1A = 26 ,D1B,D1C,D1D,D1E,D1F,D1G,D1H,D2A,D2B,D2C,D2D,D3B,D4B,
	TPH1,TPH1A,TPH1B,TPH1C,TPH1D,TPH2,TPH2A,TPH2B,TPH2C,TPH2D,
	D5,D6,D7,D8,XOS,YOS,
	DCMR,DCMR2,DBMAP,JET_TIMER,PWAIT,ISP,REF_TIMER,JNCFLAG,REF_MODE,SCAN_REG,VMPA,STATUS,
	XMASK,YMASK,ZRSV3,
	CCD = 71,RCD,DSD,DCMRH,DCMRL,MODE,BL,BT,WL,RL,
	XMIN,YMIN, S1A = 84,MLOOP,ZMAX,ZH,ZMASK,ZMIN
};


//////////////////////////////////////////
//     PDS, TSSET FUNCTION DEST         //
//////////////////////////////////////////
enum {
	ADDR0 = 0, ADDR0_B, ADDR1, ADDR1_B, ADDR2, ADDR2_B, ADDR3, ADDR3_B, ADDR4, ADDR4_B, ADDR5, ADDR5_B, ADDR6, ADDR6_B, ADDR7, ADDR7_B,
	ADDR8, ADDR8_B, ADDR9, ADDR9_B, ADDR10, ADDR10_B, ADDR11, ADDR11_B, ADDR12, ADDR12_B, ADDR13, ADDR13_B, ADDR14, ADDR14_B, ADDR15, ADDR15_B,
	ADDR16, ADDR16_B, ADDR17, ADDR17_B, ADDR18, ADDR18_B, ADDR19, ADDR19_B, ADDR20, ADDR20_B, ADDR21, ADDR21_B, ADDR22, ADDR22_B, ADDR23, ADDR23_B,
	ADDR24, ADDR24_B, ADDR25, ADDR25_B, ADDR26, ADDR26_B, ADDR27, ADDR27_B, ADDR28, ADDR28_B, ADDR29, ADDR29_B, ADDR30, ADDR30_B, ADDR31, ADDR31_B,

	CLK0, CLK0_B, CLK1, CLK1_B, CLK2, CLK2_B, CLK3, CLK3_B, CLK4, CLK4_B, CLK5, CLK5_B, CLK6, CLK6_B, CLK7, CLK7_B,

	IO0, IO0_B, IO1, IO1_B, IO2, IO2_B, IO3, IO3_B, IO4, IO4_B, IO5, IO5_B, IO6, IO6_B, IO7, IO7_B,
	IO8, IO8_B, IO9, IO9_B, IO10, IO10_B, IO11, IO11_B, IO12, IO12_B, IO13, IO13_B, IO14, IO14_B, IO15, IO15_B,
	IO16, IO16_B, IO17, IO17_B, IO18, IO18_B, IO19, IO19_B, IO20, IO20_B, IO21, IO21_B, IO22, IO22_B, IO23, IO23_B,
	IO24, IO24_B, IO25, IO25_B, IO26, IO26_B, IO27, IO27_B, IO28, IO28_B, IO29, IO29_B, IO30, IO30_B, IO31, IO31_B,

	SCAN0, SCAN0_B, SCAN1, SCAN1_B, SCAN2, SCAN2_B, SCAN3, SCAN3_B, SCAN4, SCAN4_B, SCAN5, SCAN5_B, SCAN6, SCAN6_B, SCAN7, SCAN7_B,
	SCAN8, SCAN8_B, SCAN9, SCAN9_B, SCAN10, SCAN10_B, SCAN11, SCAN11_B, SCAN12, SCAN12_B, SCAN13, SCAN13_B, SCAN14, SCAN14_B, SCAN15, SCAN15_B,
	SCAN16, SCAN16_B, SCAN17, SCAN17_B, SCAN18, SCAN18_B, SCAN19, SCAN19_B, SCAN20, SCAN20_B, SCAN21, SCAN21_B, SCAN22, SCAN22_B, SCAN23, SCAN23_B,
	SCAN24, SCAN24_B, SCAN25, SCAN25_B, SCAN26, SCAN26_B, SCAN27, SCAN27_B, SCAN28, SCAN28_B, SCAN29, SCAN29_B, SCAN30, SCAN30_B, SCAN31, SCAN31_B,

	STRB, STRB_B,
	DRE, DRE_B
};
//////////////////////////////////////////
//		 ECLK FUNCTION DEST				//
//////////////////////////////////////////
enum {   // ECLK Setting  
	BCLK1 = 5, CCLK1,  BCLK2, CCLK2, BCLK3, CCLK3, BCLK4, CCLK4, BCLK5, CCLK5, BCLK6, CCLK6, 
	BCLK7, CCLK7, BCLK8, CCLK8, BCLK9, CCLK9, BCLK10, CCLK10, BCLK11, CCLK11, BCLK12, CCLK12,
	BCLK13, CCLK13, BCLK14, CCLK14, BCLK15, CCLK15, BCLK16, CCLK16
};

enum {  // TSSET Setting
	ECLK1 = 0, ECLK2, ECLK3, ECLK4, ECLK5, ECLK6, ECLK7, ECLK8, ECLK9,
	ECLK10, ECLK11, ECLK12, ECLK13, ECLK14, ECLK15, ECLK16, 
	ECLK_SAME = 0xF0
};

//////////////////////////////////////////
//		 TSSET FUNCTION FORMAT			//
//////////////////////////////////////////
enum {
	NRZB = 0x0, _NRZB, 
	NRZC,		_NRZC,
	NRZBC,		_NRZBC,
	RZO,		_RZO,
	RZZ,		_RZZ,
	RZX,		_RZX,
	DNRZ,		_DNRZ,
	FIXL,		FIXH
};

//////////////////////////////////////////
//		 OS MESSAGE TYPE  			    //
//////////////////////////////////////////
enum { 
	INFO = 0,
	WARN = 1,
	ERR
};

//////////////////////////////////////////
//		 SCRAMBLE SELECT  			    //
//////////////////////////////////////////
enum {
	SCR_ADR = 0,
	SCR_DATA,
	SCR_ALL
};

//////////////////////////////////////////
//		 TEMP SELECT	  			    //
//////////////////////////////////////////
enum {
	PV = 0,
	SV = 1,
	NO_WAIT = 0,
	WAIT = 1
};

//////////////////////////////////////////
//		 DUT PASS/FAIL	 			    //
//////////////////////////////////////////
enum {
	CON = 0,	
	CUR = 1,
	TOTAL = 2,
	BOTH = 2,
	MASK = 4,   // USING BLOCK TYPE : MASK, CUR, TOTAL
	       
};

//////////////////////////////////////////
//		 VECTOR FILE MODE 			    //
//////////////////////////////////////////
enum {
	BINARY = 0,
	TEXT = 1
};

//////////////////////////////////////////
//		 TIME MEASURE 			    //
//////////////////////////////////////////
enum {
	START = 0,
	REMIND = 0,    // Using SET_VECTOR,  Ret Value Remind, End
	END = 1
};

//////////////////////////////////////////
//		 SHOW, HIDE 			    //
//////////////////////////////////////////
#define SHOW true
#define HIDE false

//////////////////////////////////////////
//		 TSET EDGE OPEN 			    //
//////////////////////////////////////////
#define OPEN        (-1.0)

//<<IDE DEFINE START >>
/************************************
	BIB					
************************************/

void BIB_SIZE(int col, int row, int ioCount, int multiChip = 1);	
/*
Brieping
	int col : Columnn Size
	int row : Row Size
	int ioCount : IO Count
	int multichip : Multi chip Count
Burn In Board Size Setting
*/

void macro_BIB(int row, const char* format); 
/*
void BIB(int row, const char* format);
Brieping	
	int row : Row Line
	const char* format : Duts Scan, IO Setting in Same Line	
Column of Burn In Board Duts Setting 
*/

void SCAN_TABLE(int table, uint value);
/*
Brieping	
	int table : Scan Group
	uint value : Scan Pin Enable 
Scan Pin Setting in Scan Group
*/
void ASCAN_TABLE(int table, uint value);
/*
Brieping
	int table : AScan Group
	uint value : AScan Pin Enable
AScan Pin Setting in AScan Group
*/
void IO_MASK(uint mask0_31, uint mask32_63);
/*
Brieping
	uint mask0_31 : IO 0~31 Pin 
	uint mask32_63 : IO 32~63 Pin
IO Pin Mask Setting 0~64
*/

int GET_DUT_COUNT();
/*
Brieping	
	Return : int Dut Count
Provice Dut Count In Setting BIB_SIZE
*/
int GET_SCAN_COUNT();
/*
Brieping
	Return : int Scan Count
Provice Scan Count In Setting BIB_SIZE
*/
int GET_DUT_OF_SCAN_COUNT(int ScanNo);
/*
Brieping
	int ScanNo : Scan Select
	Return : int Dut Count
Provide Dut Count of Select Scan 
*/
int GET_DUT_OF_SCAN(int ScanNo, int DutIndex);
/*
Brieping
	int ScanNo : Scan Select
	int DutIndex : Dut Select
	Return : int Dut Number
Provide Dut Number by Dut Index of Select Scan
*/

uint GET_BID();
/*
Brieping
	Return : unsigned int BID
Provide Burn In Board ID in HEX
*/

/************************************
	CATEGORY
************************************/

void SET_PCAT(int PCat);
/*
Brieping
	int PCat : Pass Category
Setting Pass Category In Step About Pass Dut
*/

bool GET_PCAT(int Dut, int sPCat, int ePCat = -1);  
/*
Brieping
	int Dut : Dut Number
	int sPCat : Start of Pass Category Range
	int ePCat : End of Pass Category Range
	Retern : bool Result of Pass Category
Provide Whether Pass Category of Dut
*/

int	 PCAT_COUNT(int Dut);
/*
Brieping
	int Dut : Dut Number
	Retern : int Pass Category Count
Provide Pass Category Count of Dut
*/

int  GET_PCAT_BY_COUNT(int Dut, int Index);
/*
Brieping
	int Dut : Dut Number
	int Index : Pass Catetory Index
	Retern : int Pass Category Count
Provice Set Pass Category Number
*/

void SET_FCAT(int PCat);
/*
Brieping
	int PCat : Pass Category
Set Pass Category Number in Step
*/

bool GET_FCAT(int Dut, int sPCat, int ePCat = -1);
/*
Brieping
	int Dut : Dut Number
	int sPCat : Start of Fail Category Range
	int ePCat : End of Fail Category Range
	Retern : bool Result of Fail Category
Provide Whether Fail Category of Dut
*/

int	 FCAT_COUNT(int Dut);
/*
Brieping
	int Dut : Dut Number
	Retern : int Fail Category Count
Provide Fail Category Count of Dut
*/

int  GET_FCAT_BY_COUNT(int Dut, int Index);
/*
Brieping
	int Dut : Dut Number
	int Index : Fail Catetory Index
	Retern : int Fail Category Count
Provice Set Fail Category Number
*/

void SET_BIN(int Dut, int Bin);
/*
Brieping
	int Dut : Dut Number
	int Bin : Bin Number
Set Bin Number of Dut
*/

int  GET_BIN(int Dut);
/*
Brieping
	int Dut : Dut Number
	Return : int Bin Number
Provide Set Bin Number of Dut
*/


/************************************
	DEVICE 
************************************/
void SET_DUT_PCAT(int Dut, int PCat);
/*
Brieping
	int Dut : Dut Number
	int PCat : Pass Category
Force Dut to Pass Category 
*/

void SET_DUT_FCAT(int Dut, int FCat);
/*
Brieping
	int Dut : Dut Number
	int PCat : Pass Category
Force Dut to Fail Category
*/

void SET_PDUT(int Dut);
/*
Brieping
	int Dut : Dut Number	
Force Dut to Pass in Current Step
*/

void SET_FDUT(int Dut);
/*
Brieping
	int Dut : Dut Number	
Force Dut to Fail in Current Step
*/

bool PDUT(int dno, int Mode = CUR);	
/*
Brieping
	int Dut : Dut Number
	int Mode : Type Select. 
		CON - Contact Step 
		CUR - Current Step
		TOTAL - Total Result
Provide Dut Result Whether Pass
*/

bool FDUT(int dno, int Mode = CUR);	
/*
Brieping
	int Dut : Dut Number
	int Mode : Type Select.
		CON - Contact Step
		CUR - Current Step
		TOTAL - Total Result
Provide Dut Result Whether Fail
*/

void SET_CHIPID(int dno, char* chipid);
/*
Brieping
	int Dut : Dut Number
	char* chipid: Chip ID Text
Set Chip ID of Dut
*/

const char* GET_CHIPID(int dno);
/*
Brieping
	int Dut : Dut Number
	Return : Chip ID Text
Provide Set Chip ID of Dut
*/

/************************************
	TS
************************************/
void RATE(double r0, double r1, double r2, double r3, double r4, double r5, double r6, double r7, double r8, double r9, double r10, double r11, double r12, double r13, double r14, double r15);
/*
Brieping
	double r0~r5 : Timing Rate TS0~15 NS Uint	
Timing Rate Setting of Each Timing Set
*/

void ECLK(int type, double r0, double r1, double r2, double r3, double r4, double r5, double r6, double r7, double r8, double r9, double r10, double r11, double r12, double r13, double r14, double r15);
/*
Brieping
	int type : Clock Type BCLK1~16, CCLK 1~16
	double r0~r5 : Clock Edge TS0~15. NS Uint
CLOCK Edge Setting of Clock and Ecah Timing Set
*/

void STRB_EDGE(int tsNo, double strb0, double strb1);
/*
Brieping
	int tsNo : Timing Set Number 0~15
	double strb0 : strob BCLK Edge. NS Unit
	double strb1 : strob CCLK Edge. NS Unit
Strob Edge Setting of Standard Timing Set
*/

void TSSET(uint pin, uint format, uint edgeB = ECLK_SAME, uint edgeC = ECLK_SAME);
/*
Brieping
	uint pin : pin Type SCAN0~15, ADDR0~23, IO0~31
	uint format : Format Type 
		NRZB, NRZC, NRZBC, RZO, RZZ, RZX, DNRZ, FIXL, FIXH 
		_NRZB, _NRZC, ...
	uint edgeB : BEDGE Select ECLK1~ECLK16		
	uint edgeC : CEDGE Select Default Same EDGEB Using ECLK_SAME
Timing Format, Edge Setting Each Pin
*/

/************************************
	PDS, SCRAMBLE
************************************/
void ENABLE_SCRAM(bool enable, int type = SCR_ALL); // SCR_ADR , SCR_DATA, SCR_ALL
/*
Brieping
	bool enbable : Enable Scramble
	int type : Scramble Type 
		SCR_ADR : Enable Adr Scramble
		SCR_DATA : Enable Data Scramble
		SCR_ALL :Enable Adr, Data
Scaramble Enalbe Setting
*/

void macro_PDS(uint pin, const char* source);
/*
void PDS(uint pin, const char* source);
Brieping
	uint pin : Pin Type
		SCAN0~SCAN31
		CLK0~CLK7
		ADDR0~31
		IO0~31
		STRB
		DRE
		B Side Append _B
	const char* source : Soure Type
		FIXH FIXL
		C0~31
		SC0~31
		X0~23
		Y0~23
		D0~17
		Z0~15
Match Pin To Source of Each PDS Group CYP0~15
*/

void macro_SCRAM(const char* source);
/*
void SCRAM(const char* source);
Brieping
	const char* source : Scramble Setting Text
		Dest :	X0~x23
				y0~y23
				d0~d17
		Source : ax0~ax23
				 ay0~ay23	
		Logic : ^ | &, ! 
Scramble Setting
*/

/************************************
	POWER
************************************/
void VIH(int type, double value, bool bCalApply = true);
/*
Brieping
	int type : Chanel Select
		CLK DR SCAN IO 
	double value : Level Setting
	bool bCalApply : Whether to Apply Cal Data 
VIH Level Setting Each Channel
*/

void VIH_CLK(int nCLK, double value, bool bCalApply = true);
/*
Brieping
	int nCLK : Chanel Select		
	double value : Level Setting
	bool bCalApply : Whether to Apply Cal Data
Clock VIH Level Setting Each CLK
*/

void VIL(int type, double value, bool bCalApply = true);
/*
Brieping
	int type : Chanel Select
			CLK DR SCAN IO 
	double value : Level Setting
	bool bCalApply : Whether to Apply Cal Data 
VIL Level Setting Each Channel
*/

void VIL_CLK(int nCLK, double value, bool bCalApply = true);
/*
Brieping
	int nCLK : Chanel Select
	double value : Level Setting
	bool bCalApply : Whether to Apply Cal Data
Clock VIL Level Setting Each CLK
*/

void IMP(int type, double value);
/*
Brieping
	int type : Chanel Select
		CLK DR SCAN IO 
	double value : Impdence Setting
Empdence Setting Each CLK
*/

void VBB(int type, uint16_t data);
/*
Brieping
	int type : Chanel Select
		CLK DR SCAN IO 
	uint16 data : Input Threshold Data
Empdence Setting Each CLK
*/

void SLR(int type, uint16_t data);
/*
Brieping
	int type : Chanel Select
		CLK DR SCAN IO 
	double value : Slew Rate Setting
Empdence Setting Each CLK
*/

void VTH(double value);
/*
Brieping	
	double value : Impdence Setting
VTH Level Setting
*/

void SET_PE_CAL(int type, int ch, float gain, float offset );
/*
Brieping
	PE Chip Calibration. Maint Function
*/

void TIME(int seq, int Select, float value);
/*
Brieping
	int seq : Sequnce Select
	int Select : VS Group, PE
		VS1~VS5
		PE
	int value : Delay Time Sec Unit
		Availabel US MS S MIN HOUR 
Setting VS Group Delay Time
*/
void DPS(int type, double level, double under, double over, double limit, bool bCalApply = true );
/*
Brieping
	int type : Channel Select
		VS1~VS5
		PE
	double level : Level Setting		
	double under : UnderVoltage Setting
	double over : OverVoltage Setting
	double limit : Current Limit Setting
	bool bCalApply : Whether to Apply Cal Data
Setting DPS Channel
*/

void READ_VOUT(int ch, float &VMin, float &VMax);
/*
Brieping
	int type : Channel Select
		VS1~VS5		
	float Min : Min Level Reference Argument
	float Max : Max Level Reference Argument
Provide Real-Time DPS Min Max Level by Reference Value
*/

float READ_VOUT_ADC(int ch);
/*
Brieping
	int type : Channel Select
		VS1~VS5
Provide Real-Time DPS Level
*/

float READ_IOUT(int ch, bool bCalApply = true);
/*
Brieping
	int type : Channel Select
		VS1~VS5
Provide Real-Time DPS Current
*/

float READ_DPS_TEMP1(int ch);
/*
Brieping
	int type : Channel Select
		VS1~VS5
Provide Real-Time DPS Temp1
*/

float READ_DPS_TEMP2(int ch);
/*
Brieping
	int type : Channel Select
		VS1~VS5
Provide Real-Time DPS Temp2
*/

uint8_t PE_WRITE(uint8_t addr, uint16_t wdata, bool bCentral = false);
uint8_t PE_READ(uint32_t nChip, int16_t ch, uint8_t addr, uint16_t * wdata);

void SRON();
/*
Brieping
Power On by TIME Function Sequnce
*/

void SROF();
/*
Brieping
Power Off by TIME Function Sequnce
*/

void DPSON();
/*
Brieping
Only DPS Power On by TIME Function Sequnce
*/

void DPSOF();
/*
Brieping
Only DPS Power Off by TIME Function Sequnce
*/
/************************************
	PATTERN
************************************/
void SEND_MPAT(const char* file); 
/*
Brieping
	const char* file : Pattern File
Loading Pattern Binary. if F12 Press, Follow Pattern Source
*/
void MEAS_MPAT(const char* pc, bool Judge = true, bool bShow = false); 
/*
Brieping
	const char* pc : Start PC 
	bool judge : Pass / Fail Evaluation
	bool bShow : Show IO Print. Debug Purpose
Pattern Running Accoding to Argument
*/

void START_MPAT(const char* pc); // Start Pattern Without WAIT_MPAT
/*
Brieping
	const char* pc : Start PC
Pattern Running And Retern Immediately
*/

void STOP_MPAT();
/*
Brieping	
Pattern Stop
*/

void WAIT_MPAT(uint wait = 0);
/*
Brieping
	unsigned int wait : Wait Time Setting. 0 is Endless	
Wait for Pattern Stop Until Wait Time
*/

void MEAS_MPAT_DATA_SC(const char* pc);
/*
Brieping
	const char* pc : Start PC
Base Compare Test, capture ADRs and failed io by socket in a csv file
*/

void SET_JNCFLAG(uint val);
/*
Brieping
	unsigned int val : JNC Register Setting Value
Setting JNC Register
*/

void SET_REG(uint reg, uint value);
/*
Brieping
	unsigned int reg : Register Setting Value		
	uint value 
Setting Pattern Register 
Register Value refer to Pattern Assist View
*/

void SET_TIMER(uint reg, double value);   // ex) SET_TIMER(JET_TIMER, 10 NS);
/*
Brieping
	unsigned int reg : Type Select
		JET_TIMER, REF_TIMER
	uint value : Time Setting
		Availabel US MS S 
Setting JET/REF Timer
*/

/************************************
	NAND TEST
************************************/

///////   SETUP  ///////////
void NAND_BIT_LOCATE(int* bits, int ReadCount = 2);
void NAND_JUDGE_LIMIT(int col, int page, int block);



///////   TEST   ///////////
void NAND_ID(const char* pc);											// ID Check

void NAND_FBLIST_READ_MASK_SC(const char* pc);		// Fail Info Block Read 
void NAND_FBLIST_SC(const char* pc, int Mode = MASK | TOTAL);		// Fail Info Block Write
void NAND_IMPRINT_SC(const char* pc, int Mode = MASK | TOTAL);			// Fail Block Imprint

void NAND_BLOCK_TEST(const char* pc, int Mode = CUR);					// Block Judge 
void NAND_BLOCK_MASK(const char* pc);
void NAND_FCOUNT_SC(const char* pc, int limit = 0);	// Fail Count Each Block

void NAND_FCOUNT_DETAIL_SC(const char* pc, int BlockADR = 0, int columnLimit = 0, int pageLimit = 0, bool bAllData = false);	// bAllData = true -> save included pass data, false = save only fail data
void NAND_DATA_READ_SC(const char* pc);									// Data -> File Proc

void NAND_CYCLE_TEST_SC(const char* pc, double rate);

///////   BLOCK   ///////////
int  GET_FBLOCK_SIZE(int dut, int type = CUR);
bool GET_FBLOCK(int dut, int block, int type = CUR);	// TOTAL Block Collect After Step Finish
bool SET_FBLOCK(int dut, int block, int type = CUR);	// CUR Block is Check Enable In Step

int GET_FPAGE_COUNT(int dut, int page);
/*
Brieping
	unsigned int dut : DUT Select
	unsigned int page : Page Select 0~4095
Provides Page Fail Count of DUT��s BLOCK
*/
int GET_FCOLUMN_COUNT(int dut, int page, int col);
/*
Brieping	
	unsigned int dut : DUT Select
	unsigned int page : Page Select 0~4095
	unsigned int col : Column Select 0~7
Provides Column Fail Count of DUT��s BLOCK��s PAGE
*/

void ENABLE_RB(bool enable);

/* 
Brieping
	bool enable : RB Feature Enable

*/

uint GET_RB_COUNT(int pin);
/*
Brieping
	unsigned int pin : Selct Pin 0~7
	
Return RB Count of Pin
*/

/************************************
	EMMC TEST
************************************/
enum {
	TOGGLE_NONE = 0,
	TOGGLE_HIGH, 
	TOGGLE_LOW,
	TOGGLE_PROG
};

void EMMC_TOGGLE_TEST(const char* pc);
/*
Brieping
TOGGLE_TEST

Internal : 
 	1. DATA_MODE Set DUT_DATA_ENABLE | DUT_DATA_BYSCAN
	2. START_MPAT

How to Use:
    1. EMMC_TOGGLE_TEST
	2. SYNC 
	3. READ_DUTDATA
	4. GET_GOGGLE_RESULT
*/


int GET_TOGGLE_RESULT(int dut);
/*
Brieping
	unsinged int dut : DUT Select
	Return Value : Toggle Value
		TOGGLE_NONE : NO READ DATA
		TOGGLE_HIGH : READ ALL HIGH
		TOGGLE_LOW :  READ ALL LOW
		TOGGLE_PROG : TOGGLE PROGRESS
TOGGLE_DATA Read

Internal :
	1. GET_DUTDATA_COUNT
	2. GET_DUTDATA
	3. Check Dut Data High Low Sequnce Assuming 1IO

*/


/************************************
	FAIL MEMORY
************************************/
void JUDGE(bool bShow = false);			
/*
Brieping
	bool bShow : Show Result Print. Debug Purpose
 Evaluation Pass / Fail 
*/

void INVERT();
/*
Brieping
Invert Pass / Fail Result by Dut
INVERT Function is Reset in Next Step
*/

enum {
	DUT_DATA_NONE = 0x0,
	DUT_DATA_ENABLE = 0x1,
	DUT_DATA_FILE = 0x3,

	DUT_DATA_BYSCAN = 0,
	DUT_DATA_ALLSCAN = 0x10,
};

void DATA_MODE(int process = DUT_DATA_NONE | DUT_DATA_BYSCAN);
/*
Brieping
	int process : Mode Option, Enable | keyword
		DUT_DATA_NONE : No Data Capture 
		DUT_DATA_ENABLE : Using Data Capture 
		DUT_DATA_FILE : Data Capture to File
		DUT_DATA_BYSCAN : Data Capture each Scan
		DUT_DATA_ALLSCAN : Data Capture regardless SCAN
Setting When Using DataMode
*/

void NAND_MODE();						
/*
Brieping
Setting NAND Test Mode
*/
void COMPARE_MODE(bool FM_Capture_Always = false);
/*
Brieping
	bool FM_Capture_Always : Capture Mode Select. Debug Purpose
Setting Compare Test Mode. Usually Use Dram Test 
*/

void DDR_MODE(bool bDDR = true);	
/*
Brieping
	bool bDDR : Whether to Use DDR Mode
Setting DDR Mode When Data Capture or Compare
*/

void FM_OVER_STOP(bool bEnable = false);
/*
Brieping
	bool bEnable : FM Over Stop Enable Flag
if set enabled, fail memory capture don't work when fail memory is full by 2k
*/

void FM_DRV_MODE(int mode = 0);				// 0 = Pattern Source, 1=PDS Source
/*
Brieping
	bool mode : Mode Setting
		0 = Pattern Source, 1=PDS Source
Setting FM Capture.  Debug Purpose
*/

void WRITE_SCAN(int scanIndex);
/*
Brieping
	int scanIndex : Scan Index		
Write Pattern Generator Scan Index
*/

int READ_SCAN();
/*
Brieping
	Return Value : Current Scan Index
Provide Current Scan Index
*/

void READ_DUTDATA();
/*
Brieping
Read FM Data As Dut Data By Scan

How To Use : 
   1. READ_DUDATA
   2. GET_CUTDATA_COUNT
   3. byte* pData = GET_DUTDATA  
   4. Use pData Until Count
*/

byte* GET_DUTDATA(int dut);
/*
Brieping
	bool mode : Mode Setting
		0 = Pattern Source, 1=PDS Source
Provide Dut Data as Byte Pointer

How To Use :
   1. READ_DUDATA
   2. GET_CUTDATA_COUNT
   3. byte* pData = GET_DUTDATA
   4. Use pData Until Count
*/

int GET_DUTDATA_COUNT();
/*
Brieping	
Provide Dut Data Size. Byte Unit
*/

typedef struct {
	byte ioComp[8];     // 8byte
	byte ioDrv[2];      // 2byte
	byte adrDrv[5];     // 5byte
	byte scan;          // 1byte
}FM;

int	READ_FMDATA(int FcRead = 0);			
FM* GET_FMDATA_H(int pos);				
FM* GET_FMDATA_L(int pos);				

void SHOW_FMDATA(int offset, int readSZ);
void SHOW_FMDRV(int offset, int readSZ);

/************************************
	VECTOR MEMORY
************************************/

		  // RET Finsh, Remind   Mode = BINARY, TEXT
bool SET_VECTOR(const char* name, int& idxCnt, int Mode = BINARY);  
/*
Brieping
	const char* name : Vector File Name
	int& idxCnt : Vector Size of IDX Loof Count. Provide Value as Reference 
	int Mode : Vector File Type 
		BINARY, TEXT
	Return : Success
Uploading Vector 
*/

int SET_VECTOR(int* pData, int& idxCnt, int ofs = 0);	
/*
Brieping
	const char* name : Vector File Name
	int& idxCnt : Vector Size of IDX Loof Count. Provide Value as Reference
	int ofs : Vector Memory position Offset
	Return : pData Proceeded Index 
Uploading Vector as byte Pointer  
pData Last Value Must -1  
ex. int pData[] = {1,2,3, ... , -1 };
*/

bool NEXT_VECTOR(int& idxCnt);
/*
Brieping
	int& idxCnt : Vector Size of IDX Loof Count. Provide Value as Reference	
	Return : whether finish
Uploading Vector continue If Vector File Size Big over VM Memory
*/											
/************************************
	USER IMPLEMENT
************************************/
void SETUP();
/*
Brieping
Set Items in This Function. BIB, Timing-Set, Power, SCan ETC
Excuted This Function First When PGM Run Befor CONTACT or TEST 
*/

void CONTACT();
/*
Brieping
Implement Functions in Here When CONTACT  
Excuted This Function When StarOS Excute CONTACT
*/

void TEST();
/*
Brieping
Implement This Function When TEST
Excuted This Function When StarOS Excute RUN
*/

void SYNC(int outCMD);					
/*
Brieping
SYNC Called When Pattern Opcode OUT
Last of This Function Need NEXT_SYNC For Next Pattern Progress
*/


/************************************
	OPERRATION
************************************/
void DELAY(double time);
/*
Brieping
	double time : Sleep Time 
Sleep Function. Sec Unit  Eable Double Type 0.5, 0.1 ...

*/
void MESSAGE(const char* msg, int type = INFO);
/*
Brieping
	const char* msg : Messege
	int type : Messege Type
		INFO : Normal 
		WARN : Warring 
		ERR : Error
Send Messege To StarOS. Messege Output LogView In StarOS, And Save To Log File

*/

void RUN_TEMP(double temp, int Mode = NO_WAIT, int timeLimit = 0); 
/*
Brieping
	double tmep : SV Temp
	int type : Mode
		NO_WAIT : Pgm Excute Immediately whihout Wait
		WAIT : After Temp Reache SetTemp, Wait Until Time Limit
	int timeLimit : if Mode Set WAIT, Wait Time Limit. 0 is Endless
When Operation Chamber Temp, Use This Function
if Call RUN_TEMP, When finish PGM Excute, Chamber Operation to Ambient and Stop
*/

double READ_TEMP(int Mode = PV);
/*
Brieping
	int : Mode
		PV : Read Temp Value
		SV : Read Set Temp Value
Recived Temp Value of Chamber
*/

void SOAK_TIME(int time /*SEC*/);
/*
Brieping
	int : time (Sec)
Wait Chamber Temp Operation for setting time
*/

void AGING(const char* pc, int time /*SEC*/);
/*
Brieping
	const char* pc
	int : time (Sec)
Pattern Running for Setting Time
*/

void NEXT_SYNC();
/*
Brieping
Using in Sync Function For Next Pattern Progress
Before Call This Function, Hold Pattern PC in OUT Command
*/


/************************************
	HV DAC function
************************************/
//uint8_t SET_HV_DAC(int ch, double volt);

void DAC_CLAMP(bool bOn);
void READ_DPS();

/************************************
	UTIL function
************************************/
long long MEASURE_TIME(int mode, bool bPrint = true);   // MODE = START, END   Return = NS Unit
														
//<<IDE DEFINE END >>
/************************************
	inner function  
************************************/
extern std::map<std::string, void(*)()>_mapStepFunc;
extern std::map<std::string, void (*)(int)> _mapStepIFunc;
extern std::map<std::string, int> _mapStepIArg;
extern bool _bAddStepMap;
extern char _sCurStep[128];

void start_step(const char* name);
void end_step();

void setDefaultMpatDelay(int delay);   // us unit 
void writeIO(uint adr, uint value); 
uint readIO(uint adr);
void writePG(uint adr, uint val, int sz);   // sz =  1~4 
uint readPG(uint adr, int sz);				// sz =  1~4
void writePGData(uint adr, byte* pData, int sz);
void readPGData(uint adr, byte* pData, int sz);
void writeFPGA(const char* file, bool bBibFPGA = false);

void showIO(bool bShow);
void showDrv(int ofs, int size);
void showData(int ofs, int size);

void writeCpeDelay(int stb0, int stb1);
void writeDskDelay(int delay);
void writeClkDelay(int select, int delay);  /*  clock selection
											0:rate,1:srtb0,2:srtb1,3:srtb2,4:srtb3,5:IO,6:IO_AD,
											7:DRE 8:AD,9:CK0----24:CK15,25:SC0-----28:SC3  */
void initDelay();

#endif
