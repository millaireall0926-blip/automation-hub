#ifndef __STAR_TEST__
#define __STAR_TEST__

#define VER_DATE "2025/08/11"
#define API_VER "1.0.5"

#include "macro.h"
#include <stdio.h>
#include <map>
#include <vector>
#include "string.h"

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
enum : unsigned long long
{			
	VS0  = 0x00000100,
	VS1  = 0x00000200,
	VS2  = 0x00000400,
	VS3  = 0x00000800,

	VS4  = 0x00001000,
	VS5  = 0x00002000,
	VS6  = 0x00004000, 
	VS7  = 0x00008000, 

	VS8  = 0x00010000,
	VS9  = 0x00020000,
	VS10 = 0x00040000,
	VS11 = 0x00080000,

	VS12 = 0x00100000,
	VS13 = 0x00200000,
	VS14 = 0x00400000,
	VS15 = 0x00800000,

	VS16 = 0x01000000,
	VS17 = 0x02000000,
	VS18 = 0x04000000,
	VS19 = 0x08000000,

	VPP0 = 0x10000000,
	VPP1 = 0x20000000,
	PE   = 0x40000000,
	DVREF1 = 0x080000000,
	DVREF2 = 0x100000000,

	VSALL= 0x0FFFFF00,
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
	//SCAN8, SCAN8_B, SCAN9, SCAN9_B, SCAN10, SCAN10_B, SCAN11, SCAN11_B, SCAN12, SCAN12_B, SCAN13, SCAN13_B, SCAN14, SCAN14_B, SCAN15, SCAN15_B,
	//SCAN16, SCAN16_B, SCAN17, SCAN17_B, SCAN18, SCAN18_B, SCAN19, SCAN19_B, SCAN20, SCAN20_B, SCAN21, SCAN21_B, SCAN22, SCAN22_B, SCAN23, SCAN23_B,
	//SCAN24, SCAN24_B, SCAN25, SCAN25_B, SCAN26, SCAN26_B, SCAN27, SCAN27_B, SCAN28, SCAN28_B, SCAN29, SCAN29_B, SCAN30, SCAN30_B, SCAN31, SCAN31_B,

	STRB = 208, STRB_B,
	DRE, DRE_B
};

//////////////////////////////////////////
//		 ECLK FUNCTION DEST				//
//////////////////////////////////////////
//enum {   // ECLK Setting  
//	BCLK1 = 5, CCLK1,  BCLK2, CCLK2, BCLK3, CCLK3, BCLK4, CCLK4, BCLK5, CCLK5, BCLK6, CCLK6, 
//	BCLK7, CCLK7, BCLK8, CCLK8, BCLK9, CCLK9, BCLK10, CCLK10, BCLK11, CCLK11, BCLK12, CCLK12,
//	BCLK13, CCLK13, BCLK14, CCLK14, BCLK15, CCLK15, BCLK16, CCLK16
//};

enum {   // ECLK Setting  
	BCLK1 = 0, CCLK1,  BCLK2, CCLK2, BCLK3, CCLK3, BCLK4, CCLK4
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


enum {
	DR = 0, IO, ALL
};

typedef struct {
    byte ioComp[16];    // 16byte
    byte ioDrv[2];      // 2byte
    byte adrDrv[5];     // 5byte
    byte scan;          // 1byte
    byte reserved[8];
}DIAG_FM,FM;


//<<IDE DEFINE START >>
/************************************
	BIB					
************************************/

void BIB_SIZE(int col, int row, int ioCount, int multiChip = 1);//
/*
Brieping
	int col : Columnn Size
	int row : Row Size
	int ioCount : IO Count
	int multichip : Multi chip Count
Burn In Board Size Setting
*/

void macro_BIB(int row, const char* format); //
/*
void BIB(int row, const char* format);
Brieping	
	int row : Row Line
	const char* format : Duts Scan, IO Setting in Same Line	
Column of Burn In Board Duts Setting 
*/

void SCAN_TABLE(int table, uint value);//
/*
Brieping	
	int table : Scan Group
	uint value : Scan Pin Enable 
Scan Pin Setting in Scan Group
*/
void ASCAN_TABLE(int table, uint value);//
/*
Brieping
	int table : AScan Group
	uint value : AScan Pin Enable
AScan Pin Setting in AScan Group
*/
void IO_MASK(uint mask0_31, uint mask32_63, uint mask64_95, uint mask96_127);//
/*
Brieping
	uint mask0_31 : IO 0~31 Pin 
	uint mask32_63 : IO 32~63 Pin
	uint mask32_63 : IO 32~63 Pin
	uint mask64_95 : IO 96~127 Pin
IO Pin Mask Setting 0~64
*/

int GET_DUT_COUNT();//
/*
Brieping	
	Return : int Dut Count
Provice Dut Count In Setting BIB_SIZE
*/

int GET_SCAN_COUNT();//
/*
Brieping
	Return : int Scan Count
Provice Scan Count In Setting BIB_SIZE
*/
int GET_DUT_OF_SCAN_COUNT(int ScanNo);//
/*
Brieping
	int ScanNo : Scan Select
	Return : int Dut Count
Provide Dut Count of Select Scan 
*/
int GET_DUT_OF_SCAN(int ScanNo, int DutIndex);//
/*
Brieping
	int ScanNo : Scan Select
	int DutIndex : Dut Select
	Return : int Dut Number
Provide Dut Number by Dut Index of Select Scan
*/

uint GET_BID();//
/*
Brieping
	Return : unsigned int BID
Provide Burn In Board ID in HEX
*/

uint GET_SLOT_ID();
/*
Brieping
	Return : unsigned int Slot ID
Provide Slot ID in HEX
*/

/************************************
	CATEGORY
************************************/

void SET_PCAT(int PCat); //
/*
Brieping
	int PCat : Pass Category
Setting Pass Category In Step About Pass Dut
*/

bool GET_PCAT(int Dut, int sPCat, int ePCat = -1);  //
/*
Brieping
	int Dut : Dut Number
	int sPCat : Start of Pass Category Range
	int ePCat : End of Pass Category Range
	Retern : bool Result of Pass Category
Provide Whether Pass Category of Dut
*/

int	 PCAT_COUNT(int Dut);//
/*
Brieping
	int Dut : Dut Number
	Retern : int Pass Category Count
Provide Pass Category Count of Dut
*/

int  GET_PCAT_BY_COUNT(int Dut, int Index);//
/*
Brieping
	int Dut : Dut Number
	int Index : Pass Catetory Index
	Retern : int Pass Category Count
Provice Set Pass Category Number
*/

void SET_FCAT(int PCat);//
/*
Brieping
	int PCat : Pass Category
Set Pass Category Number in Step
*/

bool GET_FCAT(int Dut, int sPCat, int ePCat = -1);//
/*
Brieping
	int Dut : Dut Number
	int sPCat : Start of Fail Category Range
	int ePCat : End of Fail Category Range
	Retern : bool Result of Fail Category
Provide Whether Fail Category of Dut
*/

int	 FCAT_COUNT(int Dut);//
/*
Brieping
	int Dut : Dut Number
	Retern : int Fail Category Count
Provide Fail Category Count of Dut
*/

int  GET_FCAT_BY_COUNT(int Dut, int Index);//
/*
Brieping
	int Dut : Dut Number
	int Index : Fail Catetory Index
	Retern : int Fail Category Count
Provice Set Fail Category Number
*/

void SET_BIN(int Dut, int Bin);//
/*
Brieping
	int Dut : Dut Number
	int Bin : Bin Number
Set Bin Number of Dut
*/

int  GET_BIN(int Dut);//
/*
Brieping
	int Dut : Dut Number
	Return : int Bin Number
Provide Set Bin Number of Dut
*/

void SET_DUT_PCAT(int Dut, int PCat);//
/*
Brieping
	int Dut : Dut Number
	int PCat : Pass Category
Force Dut to Pass Category 
*/

void SET_DUT_FCAT(int Dut, int FCat);//
/*
Brieping
	int Dut : Dut Number
	int PCat : Pass Category
Force Dut to Fail Category
*/

void SET_PDUT(int Dut);//
/*
Brieping
	int Dut : Dut Number	
Force Dut to Pass in Current Step
*/

void SET_FDUT(int Dut);//
/*
Brieping
	int Dut : Dut Number	
Force Dut to Fail in Current Step
*/

bool PDUT(int dno, int Mode = CUR);	//
/*
Brieping
	int Dut : Dut Number
	int Mode : Type Select. 
		CON - Contact Step 
		CUR - Current Step
		TOTAL - Total Result
Provide Dut Result Whether Pass
*/

bool FDUT(int dno, int Mode = CUR);	//
/*
Brieping
	int Dut : Dut Number
	int Mode : Type Select.
		CON - Contact Step
		CUR - Current Step
		TOTAL - Total Result
Provide Dut Result Whether Fail
*/

void SET_CHIPID(int dno, char* chipid);//
/*
Brieping
	int Dut : Dut Number
	char* chipid: Chip ID Text
Set Chip ID of Dut
*/

const char* GET_CHIPID(int dno);//
/*
Brieping
	int Dut : Dut Number
	Return : Chip ID Text
Provide Set Chip ID of Dut
*/

/************************************
	TS
************************************/
void RATE(double r0, double r1, double r2, double r3, double r4, double r5, double r6, double r7, double r8, double r9, double r10, double r11, double r12, double r13, double r14, double r15);//
/*
Brieping
	double r0~r5 : Timing Rate TS0~15 NS Uint	
Timing Rate Setting of Each Timing Set
*/

//void ECLK(int type, double r0, double r1, double r2, double r3, double r4, double r5, double r6, double r7, double r8, double r9, double r10, double r11, double r12, double r13, double r14, double r15);
///*
//Brieping
//	int type : Clock Type BCLK1~16, CCLK 1~16
//	double r0~r5 : Clock Edge TS0~15. NS Uint
//CLOCK Edge Setting of Clock and Ecah Timing Set
//*/
void IO_EDGE(int type, double e0, double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8, double e9, double e10, double e11, double e12, double e13, double e14, double e15);//
/*
Brieping
	int type : BCLK1 or CCLK 1
	double e0~e5 : IO Edge TS0~15. NS Uint
IO Edge Setting of Ecah Timing Set
*/

void DRE_EDGE(int type, double e0, double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8, double e9, double e10, double e11, double e12, double e13, double e14, double e15);//
/*
Brieping
	int type : BCLK1 or CCLK 1
	double e0~e5 : DRE Edge TS0~15. NS Uint
DRE Edge Setting of Ecah Timing Set
*/

void ADR_EDGE(int type, double e0, double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8, double e9, double e10, double e11, double e12, double e13, double e14, double e15);//
/*
Brieping
	int type : BCLK1 or CCLK 1
	double e0~e5 : Address Edge TS0~15. NS Uint
Address Edge Setting of Ecah Timing Set
*/

void CLK_EDGE(int type, double e0, double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8, double e9, double e10, double e11, double e12, double e13, double e14, double e15);//
/*
Brieping
	int type : BCLK1~4 or CCLK1~4
	double e0~e5 : Address Edge TS0~15. NS Uint
Clock Edge Setting of Ecah Timing Set
*/

void SCN_EDGE(int type, double e0, double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8, double e9, double e10, double e11, double e12, double e13, double e14, double e15);//
/*
Brieping
	int type : BCLK1~4 or CCLK1~4
	double e0~e5 : Address Edge TS0~15. NS Uint
SCAN Edge Setting of Ecah Timing Set
*/

void STRB_EDGE(int tsNo, double strb0, double strb1);//
/*
Brieping
	int tsNo : Timing Set Number 0~15
	double strb0 : strob BCLK Edge. NS Unit
	double strb1 : strob CCLK Edge. NS Unit
Strob Edge Setting of Standard Timing Set
*/

void AUTO_EDGE(int type, double e0, double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8, double e9, double e10, double e11, double e12, double e13, double e14, double e15);
/*
Brieping
	int type : BCLK1 or CCLK 1
	double e0~e5 : IO Edge TS0~15. NS Uint
IO Edge Setting of Ecah Timing Set
*/

void AUTO_EDGE2(int type, double e0, double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8, double e9, double e10, double e11, double e12, double e13, double e14, double e15);
/*
Brieping
	int type : BCLK1 or CCLK 1
	double e0~e5 : IO Edge TS0~15. NS Uint
IO Edge Setting of Ecah Timing Set
*/

void EDGE_SET(int ch, int type, double e0, double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8, double e9, double e10, double e11, double e12, double e13, double e14, double e15);
/*
Brieping
	int ch : clock No.
	int type : BCLK1 or CCLK 1
	double e0~e5 : IO Edge TS0~15. NS Uint
	Edge Setting of Each Clock & Each Timing Set
*/


void CPE_CLK_DELAY(int first, int second);
/*
Brieping
	int first, second : CPE CLK Delay value
	Set the front and back separately based on the median value of the rate.
*/

void CPE_TUNE(int first, int second);
/*
Brieping
	int first, second : CPE fine-tuned value
Set the front and back separately based on the median value of the rate.
*/

void CPE_TUNE2(int first, int second);
/*
Brieping
	int first, second : CPE fine-tuned value
Set the front and back separately based on the median value of the rate.
*/

void TSSET(uint pin, uint format);//
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
void ENABLE_SCRAM(bool enable, int type = SCR_ALL); // SCR_ADR , SCR_DATA, SCR_ALL  //
/*
Brieping
	bool enbable : Enable Scramble
	int type : Scramble Type 
		SCR_ADR : Enable Adr Scramble
		SCR_DATA : Enable Data Scramble
		SCR_ALL :Enable Adr, Data
Scaramble Enalbe Setting
*/

void macro_PDS(uint pin, const char* source);//
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

void macro_SCRAM(const char* source);//
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
void VIH(int type, double value, bool bCalApply = true);//
/*
Brieping
	int type : Chanel Select
		DR IO 
	double value : Level Setting
	bool bCalApply : Whether to Apply Cal Data 
VIH Level Setting
*/

void VTH(double value);//
/*
Brieping	
	double value : Level Setting
VTH Level Setting
*/

void VTT(int type, double value);//
/*
Brieping	
	int type : Chanel Select
		DR IO 
	double value : Level Setting
VTT Level Setting
*/

void DUTVTT(int type, double value);//
/*
Brieping	
	int type : Chanel Select
		DR IO 
	double value : Level Setting
Dut VTT Level Setting
*/

//void DUTVREF(double value);//
///*
//Brieping	
//	double value : Level Setting
//Dut VREF Level Setting
//*/

void DUTVREF1(double value);
/*
Brieping	
	double value : Level Setting
Dut VREF Level Setting
*/

void DUTVREF2(double value);
/*
Brieping	
	double value : Level Setting
Dut VREF Level Setting
*/

void TIME(int seq, unsigned long long Select, float value);//
/*
Brieping
	int seq : Sequnce Select
	int Select : VS Group, PE
		VS0~VS19
	int value : Delay Time Sec Unit
		Availabel US MS S MIN HOUR 
Setting VS Group Delay Time
*/

void HDPS(int type, double Level, bool bCalApply = true);//
/*
Brieping
	int type : Channel Select
		HVS0~HVS1
	double level : Level Setting		
	bool bCalApply : Whether to Apply Cal Data
Setting High Voltage DPS Channel
*/

void DPS(unsigned long long type, double level, double under, double over, double limit, bool bCalApply = true );//
/*
Brieping
	int type : Channel Select
		VS0~VS19 ex) VS0 | VS1 | VS3
	double level : Level Setting		
	double under : UnderVoltage Setting
	double over : OverVoltage Setting
	double limit : Current Limit Setting
	bool bCalApply : Whether to Apply Cal Data
Setting DPS Channel
*/

float READ_VOUT(int ch, int cnt = 1, bool bCalApply = true);//
/*
Brieping
	int type : Channel Select
		VS0~VS19		
Provide Real-Time DPS Min Max Level by Reference Value
*/

float READ_VOUT_ADC(int ch);//
/*
Brieping
	int type : Channel Select
		0 ~ 19
Provide Real-Time DPS Level
*/

float READ_IOUT(int ch, int cnt = 1, bool bCalApply = true);//
/*
Brieping
	int type : Channel Select
		VS0~VS19
	int cnt : sampling count
Provide Real-Time DPS Current
*/

float READ_DPS_TEMP1(int ch);//
/*
Brieping
	int type : Channel Select
		VS1~VS5
Provide Real-Time DPS Temp1
*/

float READ_DPS_TEMP2(int ch);//
/*
Brieping
	int type : Channel Select
		VS1~VS5
Provide Real-Time DPS Temp2
*/

void SRON();//
/*
Brieping
Power On by TIME Function Sequnce
*/

void SROF();//
/*
Brieping
Power Off by TIME Function Sequnce
*/

/************************************
	PATTERN
************************************/
void SEND_MPAT(const char* file); //
/*
Brieping
	const char* file : Pattern File
Loading Pattern Binary. if F12 Press, Follow Pattern Source
*/
void MEAS_MPAT(const char* pc, bool Judge = true, bool bShow = false); //
/*
Brieping
	const char* pc : Start PC 
	bool judge : Pass / Fail Evaluation
	bool bShow : Show IO Print. Debug Purpose
Pattern Running Accoding to Argument
*/

void START_MPAT(const char* pc); // Start Pattern Without WAIT_MPAT //
/*
Brieping
	const char* pc : Start PC
Pattern Running And Retern Immediately
*/

void STOP_MPAT();//
/*
Brieping	
Pattern Stop
*/

void WAIT_MPAT(uint wait = 0);//
/*
Brieping
	unsigned int wait : Wait Time Setting. 0 is Endless	
Wait for Pattern Stop Until Wait Time
*/

void SET_JNCFLAG(uint val);//
/*
Brieping
	unsigned int val : JNC Register Setting Value
Setting JNC Register
*/

void SET_REG(uint reg, uint val); //
/*
Brieping
	unsigned int reg : Register Setting Value		
	uint value 
Setting Pattern Register 
Register Value refer to Pattern Assist View
*/

void SET_TIMER(uint reg, double val);   // ex) SET_TIMER(JET_TIMER, 10 NS);//
/*
Brieping
	unsigned int reg : Type Select
		JET_TIMER, REF_TIMER
	uint value : Time Setting
		Availabel US MS S 
Setting JET/REF Timer
*/

/************************************
	OPERRATION
************************************/
void DELAY(double time);//
// ---> IDE Define Format Example
/*   
Brieping
	double time : Sleep Time 
Sleep Function. Sec Unit  Eable Double Type 0.5, 0.1 ...
*/
/// ---> IDE Define Format End

void MESSAGE(const char* msg, int type = INFO);//
/*
Brieping
	const char* msg : Messege
	int type : Messege Type
		INFO : Normal 
		WARN : Warring 
		ERR : Error
Send Messege To StarOS. Messege Output LogView In StarOS, And Save To Log File
*/


void RUN_TEMP(double temp, int Mode = NO_WAIT, int timeLimit = 0); // MODE = NO_WAIT/WAIT,  timeLimit = Wait Limit Time (Sec)
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

/************************************
	USER IMPLEMENT
************************************/
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
	FAIL MEMORY
************************************/
void JUDGE(bool bShow = false);		//	
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
void DDR_MODE(bool bDDR = true, bool bFirst = true);	
/*
Brieping
	bool bDDR : Whether to Use DDR Mode
Setting DDR Mode When Data Capture or Compare
*/
void FM_DRV_MODE(int mode = 0);				// 0 = Pattern Source, 1=PDS Source
/*
Brieping
	bool mode : Mode Setting
		0 = Pattern Source, 1=PDS Source
Setting FM Capture.  Debug Purpose
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

int READ_FMDATA(int FcRead = 0);			
/*
Brieping
	int FcRead : Read Count. Derfault 0
		0 = FM Read By Pattern
		if You Set Number, Force Read FM
Excute FM Read
*/
FM* GET_FMDATA_H(int nPG, int pos);
/*
Brieping
	int nPG : PG Select 0~3
	int pos : FM Position Offset
get Captured FM Data by FM Struct. H is First Data
*/
FM* GET_FMDATA_L(int nPG, int pos);
/*
Brieping
	int nPG : PG Select 0~3
	int pos : FM Position Offset
get Captured FM Data by FM Struct. H is Second Data
*/
void SHOW_FM_DRV(int nPG, int ofs, int size);
/*
Brieping
	int nPG : PG Select 0~3
	int pos : FM Position Offset
	int pos : Display Size
Display Captured FM DRV Data. X,Y,Z ...
*/
void SHOW_FM_DATA(int nPG, int ofs, int size);
/*
Brieping
	int nPG : PG Select 0~3
	int pos : FM Position Offset
	int pos : Display Size
Display Captured FM IO Data. 
*/


													
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

void writePG(uint adr, uint val, int sz);   // sz =  1~4 

void writeFPGA(const char* file, bool bPG);
void showIO(bool bShow);
void writeIO(uint adr, uint value);
uint readIO(int pci, uint adr);
void writePG(uint8_t PGSel, uint adr, uint value, uint size);
void writeTG(uint adr, uint value, uint size);
uint readPG(uint8_t PGSel, uint adr, uint size);
uint readTG(int pci, uint adr, uint size);

void writeDPS(int pci, uint dev_no, uint cmd, ushort data, uint size = 1);
ushort readDPS(int pci, uint dev_no, uint cmd, uint size = 1);
uint32_t readCtrlVer(int pci);
uint32_t readCtrlDate(int pci);

#endif

