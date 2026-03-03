#include "define.h"
#include "STAR_HSBI.h"
#include "DIAG_HSBI.h"
#include "../starlib/starlib_dif.h"
#include "errctrl.h"
#include "Scanner.h"
#include "ioc.h"
#include "bib.h"
#include "pgc.h"
#include "tgc.h"
#include "dps.h"
#include "dac.h"
#include "net.h"
#include "macro.h"
#include "failmem.h"
#include "nand.h"
#include "operation.h"
#include "power.h"
#include "common.h"
#include "result.h"
#include "cmd.h"
#include "eeprom.h"

ioc			gIO;   // gIO
starlib		gLIB;  // XML, CLIETN
net			gNET;  // Network
pgc			gPG;   // PG Control
tgc			gTG;   // TG Control
pds			gPDS;  // PDS
//power		gPwr;  // Power 
failmem		gFM;   // Fail Memory
nand		gNAND; // NAND
errctrl		gERR;  // Error Check
operation	gOPR;  // Operation
common		gCOM;  // Common Func, Data 
bib			gBIB;  // BIB, Result
result      gRES;  // Result 
dps			gDPS;  // dps
dac			gDAC;  // dac
scramble	gSCR;			//  gSCR
command		gCMD;
eeprom		gROM;			//  gROM

std::map<std::string, void (*)()> _mapStepFunc;
std::map<std::string, void (*)(int)> _mapStepIFunc;
std::map<std::string, void (*)(int , ...)> _mapStepINFunc;
std::map<std::string, int> _mapStepIArg;
bool _bAddStepMap;

/************************************
	BIB
************************************/
void BIB_SIZE(int col, int row, int ioCount, int MultiChip)
{
	gBIB.setBIBSize(col, row, ioCount, MultiChip);
}

void macro_BIB(int row, const char* format)
{	
	gBIB.setBIBRow(row, format);
}

void SCAN_TABLE(int table, uint value)
{
	gPG.writeScan(table, value);
}

void ASCAN_TABLE(int table, uint value)
{
	gPG.writeAScan(table, value);
}

void IO_MASK(uint mask0_31, uint mask32_63, uint mask64_95, uint mask96_127)
{
	gPG.writeIOMask(mask0_31, mask32_63, mask64_95, mask96_127);
}

int GET_DUT_COUNT()
{
	return gBIB.m_nDutCnt;
}

int GET_SCAN_COUNT()
{
	return gBIB.getScanSize();
}

int GET_DUT_OF_SCAN_COUNT(int ScanNo)
{
	return gBIB.getDutOfScanCnt(ScanNo);
}

int GET_DUT_OF_SCAN(int ScanNo, int DutIndex)
{
	return gBIB.getDutOfScan(ScanNo, DutIndex);
}

uint GET_BID()
{
	return gIO.readBID();
}

uint GET_SLOT_ID()
{
	return gIO.readSlotID();
}

/************************************
	CATEGORY
************************************/

void SET_PCAT(int PCat)
{
	for (int i = 0; i < (int)gBIB.m_DutArr.size(); i++)
	{
		dut* pDut = gBIB.getDut(i);
		if (pDut == NULL)return;
		if (pDut->m_nCurResult != STEP_PASS)continue;

		int idx = PCat / 32;
		int rem = PCat % 32;
		pDut->m_nPCat[idx] |= (1 << rem);
		pDut->m_PCatArr.push_back(PCat);
	}
}

bool GET_PCAT(int nDut, int sPCat, int ePCat)
{
	dut* pDut = gBIB.getDut(nDut);
	if (pDut == NULL)return false;
	if (ePCat == -1) ePCat = sPCat;
	
	bool bRet = false;
	
	for (int i = sPCat; i <= ePCat; i++)
	{
		int idx = i / 32;
		int rem = i % 32;

		if (pDut->m_nPCat[idx] & (1<< rem)) {
			bRet = true;
			break;
		}
	}

	return bRet;
}

int	PCAT_COUNT(int Dut)
{
	dut* pDut = gBIB.getDut(Dut);
	if (pDut == NULL)return 0;
	return pDut->m_PCatArr.size();
}

int GET_PCAT_BY_COUNT(int Dut, int index)
{
	dut* pDut = gBIB.getDut(Dut);
	if (pDut == NULL)return 0;
	return pDut->m_PCatArr[index];
}

void SET_FCAT(int FCat)
{
	for (int i = 0; i < (int)gBIB.m_DutArr.size(); i++)
	{
		dut* pDut = gBIB.getDut(i);
		if (pDut == NULL)return;
		if (pDut->m_nCurResult != STEP_FAIL)continue;
		int idx = FCat / 32;
		int rem = FCat % 32;
		pDut->m_nFCat[idx] |= (1 << rem);
		pDut->m_FCatArr.push_back(FCat);
	}
}

bool GET_FCAT(int nDut, int sFCat, int eFCat)
{
	dut* pDut = gBIB.getDut(nDut);
	if (pDut == NULL)return false;
	if (eFCat == -1) eFCat = sFCat;

	bool bRet = false;

	for (int i = sFCat; i <= eFCat; i++)
	{
		int idx = i / 32;
		int rem = i % 32;

		if (pDut->m_nFCat[idx] & (1 << rem)) {
			bRet = true;
			break;
		}
	}

	return bRet;
}

int	FCAT_COUNT(int Dut)
{
	dut* pDut = gBIB.getDut(Dut);
	if (pDut == NULL)return 0;
	return pDut->m_FCatArr.size();
}

int GET_FCAT_BY_COUNT(int Dut, int index)
{
	dut* pDut = gBIB.getDut(Dut);
	if (pDut == NULL)return 0;
	return pDut->m_FCatArr[index];
}

int GET_BIN(int Dut)
{
	dut* pDut = gBIB.getDut(Dut);
	if (pDut == NULL)return -1;
	return pDut->m_nResultBin;
}

void SET_BIN(int Dut, int Bin)
{
	dut* pDut = gBIB.getDut(Dut);
	if (pDut == NULL)return;
	pDut->m_nResultBin = Bin;
}

/************************************
	DEVICE
************************************/

void SET_DUT_PCAT(int Dut, int PCat)
{
	dut* pDut = gBIB.getDut(Dut);
	if (pDut == NULL)return;

	int idx = PCat / 32;
	int rem = PCat % 32;
	pDut->m_nPCat[idx] |= (1 << rem);
	pDut->m_PCatArr.push_back(PCat);
}

void SET_DUT_FCAT(int Dut, int FCat)
{
	dut* pDut = gBIB.getDut(Dut);
	if (pDut == NULL)return;
	int idx = FCat / 32;
	int rem = FCat % 32;
	pDut->m_nFCat[idx] |= (1 << rem);
	pDut->m_FCatArr.push_back(FCat);
}

void SET_PDUT(int dno)
{
	dut* pDut = gBIB.getDut(dno);
	if (pDut == NULL)return;
	pDut->m_nCurResult = STEP_PASS;
	if (gCOM.m_nTestMode == TEST_MODE_CONTACT) pDut->m_nConResult = pDut->m_nCurResult;
	pDut->m_nTotalResult = pDut->m_nPrevTotal;
	pDut->m_nTotalResult &= pDut->m_nCurResult;
}

void SET_FDUT(int dno)
{
	dut* pDut = gBIB.getDut(dno);
	if (pDut == NULL)return;
	printf("%s, dno %d", __FUNCTION__, dno);
	pDut->m_nCurResult = STEP_FAIL;	
	if (gCOM.m_nTestMode == TEST_MODE_CONTACT) pDut->m_nConResult = pDut->m_nCurResult;
	pDut->m_nTotalResult = pDut->m_nPrevTotal;
	pDut->m_nTotalResult &= pDut->m_nCurResult;
}

bool PDUT(int dno, int Mode)	//  MODE = CON, CUR, TOTAL   
{

	dut* pDut = gBIB.getDut(dno);
	if (pDut == NULL)return false;

	int nRes = pDut->m_nCurResult;
	if (Mode == CON)
		nRes = pDut->m_nConResult;
	else if (Mode == TOTAL)
		nRes = pDut->m_nTotalResult;

	return (nRes == STEP_PASS) ? true : false;
}

bool FDUT(int dno, int Mode)	//  MODE = CON, CUR, TOTAL   
{
	return !PDUT(dno, Mode);
}

void SET_CHIPID(int dno, char* chipid)
{
	gBIB.m_nResultMode |= RES_CHIP_ID;

	dut* pDut = gBIB.getDut(dno);
	if (pDut == NULL)return;
	pDut->m_sChipID.Format("%s", chipid);
}

const char* GET_CHIPID(int dno)
{
	dut* pDut = gBIB.getDut(dno);
	if (pDut == NULL)return NULL;

	return pDut->m_sChipID.c_str();
}

/************************************
	TG
************************************/
void RATE(double r0, double r1, double r2, double r3, double r4,
	double r5, double r6, double r7, double r8, double r9,
	double r10, double r11, double r12, double r13,
	double r14, double r15)
{

	//printf("Rate m_bDebug(%d)\n", gIO.m_bDebug ? 1 :0);
	gTG.writeRate(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15);
}

//void ECLK(int type, double r0, double r1, double r2, double r3, double r4,
//	double r5, double r6, double r7, double r8, double r9,
//	double r10, double r11, double r12, double r13,
//	double r14, double r15)
//{
//	gTG.writeEClk(type, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15);
//}

void IO_EDGE(int type, double e0, double e1, double e2, double e3, double e4,
	double e5, double e6, double e7, double e8, double e9, double e10,
	double e11, double e12, double e13, double e14, double e15)
{
	if(type != BCLK1 && type != CCLK1) {
		MESSAGE("IO_EDGE type check.", ERR);
		return;
	}
	bool bCedge = ((type % 2) != 0 ? true : false);
	//printf("== %s %d\n", __FUNCTION__, TG_MEM_IO + type);
	gTG.writeEdge(TG_MEM_IO + type, e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, bCedge);
	//gTG.writeTGMem(TG_MEM_IO + type,)
}

void DRE_EDGE(int type, double e0, double e1, double e2, double e3, double e4,
	double e5, double e6, double e7, double e8, double e9, double e10,
	double e11, double e12, double e13, double e14, double e15)
{
	if(type != BCLK1 && type != CCLK1) {
		MESSAGE("DRE_EDGE type check.", ERR);
		return;
	}
	bool bCedge = ((type % 2) != 0 ? true : false);
	//printf("== %s %d\n", __FUNCTION__, TG_MEM_DRE + type);
	gTG.writeEdge(TG_MEM_DRE + type, e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, bCedge);
}

void ADR_EDGE(int type, double e0, double e1, double e2, double e3, double e4,
	double e5, double e6, double e7, double e8, double e9, double e10,
	double e11, double e12, double e13, double e14, double e15)
{
	if(type != BCLK1 && type != CCLK1) {
		MESSAGE("ADR_EDGE type check.", ERR);
		return;
	}
	bool bCedge = ((type % 2) != 0 ? true : false);
	//printf("== %s %d\n", __FUNCTION__, TG_MEM_DR + type);
	gTG.writeEdge(TG_MEM_DR + type, e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, bCedge);
}

void CLK_EDGE(int type, double e0, double e1, double e2, double e3, double e4,
	double e5, double e6, double e7, double e8, double e9, double e10,
	double e11, double e12, double e13, double e14, double e15)
{
	if(type < BCLK1 || type > CCLK4) {
		MESSAGE("CLK_EDGE type check.", ERR);
		return;
	}
	bool bCedge = ((type % 2) != 0 ? true : false);
	//printf("== %s %d\n", __FUNCTION__, TG_MEM_CLK + type);
	gTG.writeEdge(TG_MEM_CLK + type, e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, bCedge);
}

void SCN_EDGE(int type, double e0, double e1, double e2, double e3, double e4,
	double e5, double e6, double e7, double e8, double e9, double e10,
	double e11, double e12, double e13, double e14, double e15)
{
	if(type != BCLK1 && type != CCLK1) {
		MESSAGE("SCN_EDGE type check.", ERR);
		return;
	}
	bool bCedge = ((type % 2) != 0 ? true : false);
	//printf("== %s %d\n", __FUNCTION__, TG_MEM_CLK + type);
	gTG.writeEdge(TG_MEM_SCAN + type, e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, bCedge);
}

void STRB_EDGE(int tsNo, double strb0, double strb1)
{
	gTG.writeSTRBEdge(tsNo, strb0, strb1);
}

void AUTO_EDGE(int type, double e0, double e1, double e2, double e3, double e4,
	double e5, double e6, double e7, double e8, double e9, double e10,
	double e11, double e12, double e13, double e14, double e15)
{
	if(type != BCLK1 && type != CCLK1) {
		MESSAGE("IO_EDGE type check.", ERR);
		return;
	}
	bool bCedge = ((type % 2) != 0 ? true : false);
	//printf("== %s %d\n", __FUNCTION__, TG_MEM_IO + type);
	gTG.writeEdge(23 + type, e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, bCedge);
	//gTG.writeTGMem(TG_MEM_IO + type,)
}

void AUTO_EDGE2(int type, double e0, double e1, double e2, double e3, double e4,
	double e5, double e6, double e7, double e8, double e9, double e10,
	double e11, double e12, double e13, double e14, double e15)
{
	if(type != BCLK1 && type != CCLK1) {
		MESSAGE("IO_EDGE type check.", ERR);
		return;
	}
	bool bCedge = ((type % 2) != 0 ? true : false);
	//printf("== %s %d\n", __FUNCTION__, TG_MEM_IO + type);
	gTG.writeEdge(21 + type, e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, bCedge);
	//gTG.writeTGMem(TG_MEM_IO + type,)
}

void EDGE_SET(int ch, int type, double e0, double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8, double e9, double e10, double e11, double e12, double e13, double e14, double e15)
{
	if(type != BCLK1 && type != CCLK1) {
		MESSAGE("IO_EDGE type check.", ERR);
		return;
	}
	bool bCedge = ((type % 2) != 0 ? true : false);
	//printf("== %s %d\n", __FUNCTION__, TG_MEM_IO + type);
	gTG.writeEdge(5 + (ch * 2) + type, e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, bCedge);
	//gTG.writeTGMem(TG_MEM_IO + type,)
}

void CPE_CLK_DELAY(int first, int second)
{
	gTG.writeCpeDelay(first, second);
}

void CPE_TUNE(int first, int second)
{
	gTG.m_iCpeFine[0][0] = first;
	gTG.m_iCpeFine[0][1] = second;
}

void CPE_TUNE2(int first, int second)
{
	gTG.m_iCpeFine[1][0] = first;
	gTG.m_iCpeFine[1][1] = second;
}

void TSSET(uint pin, uint format)
{
	gTG.writeFormat(pin, format);
}

/************************************
	PDS, SCRAMBLE
************************************/
void macro_PDS(uint pin, const char* source)
{
	gPDS.setPDS(pin, source);
}

void ENABLE_SCRAM(bool enable, int type)
{
	gSCR.enableScr(enable, type);
}

void macro_SCRAM(const char* source)
{
	gSCR.setScr(source);
}

/************************************
	POWER
************************************/

void VIH(int type, double value, bool bCalApply)
{
	gCOM.debug("	%s type:%d in -> ", __FUNCTION__, type);

	//std::vector<int> ch_vec;
	
	if(type == DR)  {
		//ch_vec.push_back(0x1);
		//ch_vec.push_back(0x4);
		gOPR.m_DRVIH = value;
		//gOPR.m_vecDacValue[4] = gOPR.m_vecDacValue[1] = 4.224/1.989 * value;
		//gOPR.m_vecDacValue[4] = gOPR.m_vecDacValue[1] = 2.25 * value;

		gOPR.m_vecDacValue[0][1] = 2.25 * (value * gOPR.m_DacCal[0][1].gain + gOPR.m_DacCal[0][1].offset);
		gOPR.m_vecDacValue[1][1] = 2.25 * (value * gOPR.m_DacCal[1][1].gain + gOPR.m_DacCal[1][1].offset);

		gOPR.m_vecDacValue[0][4] = 2.25 * (value * gOPR.m_DacCal[0][4].gain + gOPR.m_DacCal[0][4].offset);
		gOPR.m_vecDacValue[1][4] = 2.25 * (value * gOPR.m_DacCal[1][4].gain + gOPR.m_DacCal[1][4].offset);

		//gOPR.m_vecDacValue[4] = value;
	}		
	else if(type == IO) {
		//ch_vec.push_back(0x0);
		//ch_vec.push_back(0x3);
		gOPR.m_IOVIH = value;
		//gOPR.m_vecDacValue[3] = gOPR.m_vecDacValue[0] = 4.224/1.962 * value;
		//gOPR.m_vecDacValue[3] = gOPR.m_vecDacValue[0] = 2.25 * value;

		gOPR.m_vecDacValue[0][0] = 2.25 * (value * gOPR.m_DacCal[0][0].gain + gOPR.m_DacCal[0][0].offset);
		gOPR.m_vecDacValue[1][0] = 2.25 * (value * gOPR.m_DacCal[1][0].gain + gOPR.m_DacCal[1][0].offset);

		gOPR.m_vecDacValue[0][3] = 2.25 * (value * gOPR.m_DacCal[0][3].gain + gOPR.m_DacCal[0][3].offset);
		gOPR.m_vecDacValue[1][3] = 2.25 * (value * gOPR.m_DacCal[1][3].gain + gOPR.m_DacCal[1][3].offset);
		//gOPR.m_vecDacValue[3] = value;
	}
	else if(type == ALL) {
		//ch_vec.push_back(0x1);
		//ch_vec.push_back(0x4);
		//ch_vec.push_back(0x0);
		//ch_vec.push_back(0x3);
		gOPR.m_DRVIH = value;
		gOPR.m_IOVIH = value;
		//gOPR.m_vecDacValue[4] = gOPR.m_vecDacValue[1] = 4.224/1.989 * value;
		//gOPR.m_vecDacValue[3] = gOPR.m_vecDacValue[0] = 4.224/1.962 * value;
		//gOPR.m_vecDacValue[4] = gOPR.m_vecDacValue[1] = 2.25 * value;
		//gOPR.m_vecDacValue[3] = gOPR.m_vecDacValue[0] = 2.25 * value;

		gOPR.m_vecDacValue[0][1] = 2.25 * (value * gOPR.m_DacCal[0][1].gain + gOPR.m_DacCal[0][1].offset);
		gOPR.m_vecDacValue[1][1] = 2.25 * (value * gOPR.m_DacCal[1][1].gain + gOPR.m_DacCal[1][1].offset);

		gOPR.m_vecDacValue[0][4] = 2.25 * (value * gOPR.m_DacCal[0][4].gain + gOPR.m_DacCal[0][4].offset);
		gOPR.m_vecDacValue[1][4] = 2.25 * (value * gOPR.m_DacCal[1][4].gain + gOPR.m_DacCal[1][4].offset);

		gOPR.m_vecDacValue[0][0] = 2.25 * (value * gOPR.m_DacCal[0][0].gain + gOPR.m_DacCal[0][0].offset);
		gOPR.m_vecDacValue[1][0] = 2.25 * (value * gOPR.m_DacCal[1][0].gain + gOPR.m_DacCal[1][0].offset);

		gOPR.m_vecDacValue[0][3] = 2.25 * (value * gOPR.m_DacCal[0][3].gain + gOPR.m_DacCal[0][3].offset);
		gOPR.m_vecDacValue[1][3] = 2.25 * (value * gOPR.m_DacCal[1][3].gain + gOPR.m_DacCal[1][3].offset);
	}
	else
		return;

	//uint16_t code = (value/5.0)*4096;

	//for(int pci = 0; pci < MAX_PCI; pci++) {
	//	for( unsigned int n = 0 ; n < ch_vec.size(); n++)
	//		gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch_vec[n], code);
	//}
}

void VTH(double value)
{
	gCOM.debug("	%s value:%lf in -> ", __FUNCTION__, value);

	//std::vector<int> ch_vec; 

	//ch_vec.push_back(2);
	//ch_vec.push_back(5);
	gOPR.m_VTH = value;
	//gOPR.m_vecDacValue[5] = gOPR.m_vecDacValue[2] = 4 * value;
	//gOPR.m_vecDacValue[5] = gOPR.m_vecDacValue[2] = 2.25 * value;

	gOPR.m_vecDacValue[0][2] = 2.25 * (value * gOPR.m_DacCal[0][2].gain + gOPR.m_DacCal[0][2].offset);
	gOPR.m_vecDacValue[1][2] = 2.25 * (value * gOPR.m_DacCal[1][2].gain + gOPR.m_DacCal[1][2].offset);

	gOPR.m_vecDacValue[0][5] = 2.25 * (value * gOPR.m_DacCal[0][5].gain + gOPR.m_DacCal[0][5].offset);
	gOPR.m_vecDacValue[1][5] = 2.25 * (value * gOPR.m_DacCal[1][5].gain + gOPR.m_DacCal[1][5].offset);
	//gOPR.m_vecDacValue[5] = value;

	//uint16_t code = (value/5.0)*4096;

	//for(int pci = 0; pci < MAX_PCI; pci++) {
	//	for( unsigned int n = 0 ; n < ch_vec.size(); n++)
	//		gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch_vec[n], code);
	//}
}

void VTT(int type, double value)
{
	gCOM.debug("	%s type:%d in -> ", __FUNCTION__, type);

	//std::vector<int> ch_vec; 

	if(type == DR)  {
		//ch_vec.push_back(8);
		//ch_vec.push_back(9);
		//ch_vec.push_back(12);
		//ch_vec.push_back(13);
		gOPR.m_DRVTT = value;
		//value *= 2.0;
		//gOPR.m_vecDacValue[8] = value;
		//gOPR.m_vecDacValue[9] = value;
		//gOPR.m_vecDacValue[12] = value;
		//gOPR.m_vecDacValue[13] = value;

		gOPR.m_vecDacValue[0][8] = 2.0 * (value * gOPR.m_DacCal[0][8].gain + gOPR.m_DacCal[0][8].offset);
		gOPR.m_vecDacValue[1][8] = 2.0 * (value * gOPR.m_DacCal[1][8].gain + gOPR.m_DacCal[1][8].offset);

		gOPR.m_vecDacValue[0][9] = 2.0 * (value * gOPR.m_DacCal[0][9].gain + gOPR.m_DacCal[0][9].offset);
		gOPR.m_vecDacValue[1][9] = 2.0 * (value * gOPR.m_DacCal[1][9].gain + gOPR.m_DacCal[1][9].offset);

		gOPR.m_vecDacValue[0][12] = 2.0 * (value * gOPR.m_DacCal[0][12].gain + gOPR.m_DacCal[0][12].offset);
		gOPR.m_vecDacValue[1][12] = 2.0 * (value * gOPR.m_DacCal[1][12].gain + gOPR.m_DacCal[1][12].offset);

		gOPR.m_vecDacValue[0][13] = 2.0 * (value * gOPR.m_DacCal[0][13].gain + gOPR.m_DacCal[0][13].offset);
		gOPR.m_vecDacValue[1][13] = 2.0 * (value * gOPR.m_DacCal[1][13].gain + gOPR.m_DacCal[1][13].offset);
	}		
	else if(type == IO) {
		//ch_vec.push_back(10);
		//ch_vec.push_back(11);
		//ch_vec.push_back(14);
		//ch_vec.push_back(15);
		gOPR.m_IOVTT = value;
		//value *= 2.0;
		//gOPR.m_vecDacValue[10] = value;
		//gOPR.m_vecDacValue[11] = value;
		//gOPR.m_vecDacValue[14] = value;
		//gOPR.m_vecDacValue[15] = value;

		gOPR.m_vecDacValue[0][10] = 2.0 * (value * gOPR.m_DacCal[0][10].gain + gOPR.m_DacCal[0][10].offset);
		gOPR.m_vecDacValue[1][10] = 2.0 * (value * gOPR.m_DacCal[1][10].gain + gOPR.m_DacCal[1][10].offset);

		gOPR.m_vecDacValue[0][11] = 2.0 * (value * gOPR.m_DacCal[0][11].gain + gOPR.m_DacCal[0][11].offset);
		gOPR.m_vecDacValue[1][11] = 2.0 * (value * gOPR.m_DacCal[1][11].gain + gOPR.m_DacCal[1][11].offset);

		gOPR.m_vecDacValue[0][14] = 2.0 * (value * gOPR.m_DacCal[0][14].gain + gOPR.m_DacCal[0][14].offset);
		gOPR.m_vecDacValue[1][14] = 2.0 * (value * gOPR.m_DacCal[1][14].gain + gOPR.m_DacCal[1][14].offset);

		gOPR.m_vecDacValue[0][15] = 2.0 * (value * gOPR.m_DacCal[0][15].gain + gOPR.m_DacCal[0][15].offset);
		gOPR.m_vecDacValue[1][15] = 2.0 * (value * gOPR.m_DacCal[1][15].gain + gOPR.m_DacCal[1][15].offset);
	}
	else if(type == ALL) {
		//ch_vec.push_back(8);
		//ch_vec.push_back(9);
		//ch_vec.push_back(12);
		//ch_vec.push_back(13);
		//ch_vec.push_back(10);
		//ch_vec.push_back(11);
		//ch_vec.push_back(14);
		//ch_vec.push_back(15);
		gOPR.m_DRVTT = value;
		gOPR.m_IOVTT = value;
		//value *= 2.0;
		//gOPR.m_vecDacValue[8] = value;
		//gOPR.m_vecDacValue[9] = value;
		//gOPR.m_vecDacValue[12] = value;
		//gOPR.m_vecDacValue[13] = value;
//
		//gOPR.m_vecDacValue[10] = value;
		//gOPR.m_vecDacValue[11] = value;
		//gOPR.m_vecDacValue[14] = value;
		//gOPR.m_vecDacValue[15] = value;

		gOPR.m_vecDacValue[0][8] = 2.0 * (value * gOPR.m_DacCal[0][8].gain + gOPR.m_DacCal[0][8].offset);
		gOPR.m_vecDacValue[1][8] = 2.0 * (value * gOPR.m_DacCal[1][8].gain + gOPR.m_DacCal[1][8].offset);

		gOPR.m_vecDacValue[0][9] = 2.0 * (value * gOPR.m_DacCal[0][9].gain + gOPR.m_DacCal[0][9].offset);
		gOPR.m_vecDacValue[1][9] = 2.0 * (value * gOPR.m_DacCal[1][9].gain + gOPR.m_DacCal[1][9].offset);

		gOPR.m_vecDacValue[0][12] = 2.0 * (value * gOPR.m_DacCal[0][12].gain + gOPR.m_DacCal[0][12].offset);
		gOPR.m_vecDacValue[1][12] = 2.0 * (value * gOPR.m_DacCal[1][12].gain + gOPR.m_DacCal[1][12].offset);

		gOPR.m_vecDacValue[0][13] = 2.0 * (value * gOPR.m_DacCal[0][13].gain + gOPR.m_DacCal[0][13].offset);
		gOPR.m_vecDacValue[1][13] = 2.0 * (value * gOPR.m_DacCal[1][13].gain + gOPR.m_DacCal[1][13].offset);


		gOPR.m_vecDacValue[0][10] = 2.0 * (value * gOPR.m_DacCal[0][10].gain + gOPR.m_DacCal[0][10].offset);
		gOPR.m_vecDacValue[1][10] = 2.0 * (value * gOPR.m_DacCal[1][10].gain + gOPR.m_DacCal[1][10].offset);

		gOPR.m_vecDacValue[0][11] = 2.0 * (value * gOPR.m_DacCal[0][11].gain + gOPR.m_DacCal[0][11].offset);
		gOPR.m_vecDacValue[1][11] = 2.0 * (value * gOPR.m_DacCal[1][11].gain + gOPR.m_DacCal[1][11].offset);

		gOPR.m_vecDacValue[0][14] = 2.0 * (value * gOPR.m_DacCal[0][14].gain + gOPR.m_DacCal[0][14].offset);
		gOPR.m_vecDacValue[1][14] = 2.0 * (value * gOPR.m_DacCal[1][14].gain + gOPR.m_DacCal[1][14].offset);

		gOPR.m_vecDacValue[0][15] = 2.0 * (value * gOPR.m_DacCal[0][15].gain + gOPR.m_DacCal[0][15].offset);
		gOPR.m_vecDacValue[1][15] = 2.0 * (value * gOPR.m_DacCal[1][15].gain + gOPR.m_DacCal[1][15].offset);
	}
	else
		return;

	//uint16_t code = (value/5.0)*4096;

	//for(int pci = 0; pci < MAX_PCI; pci++) {
	//	for( unsigned int n = 0 ; n < ch_vec.size(); n++)
	//		gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch_vec[n], code);
	//}
}

void DUTVTT(int type, double value)
{
	gCOM.debug("	%s type:%d value:%f in -> ", __FUNCTION__, type, value);

	if(type == DR)  {
		gOPR.m_DutDRVTT = value;
		//value *= 2.0;
		//gOPR.m_vecDacValue[16] = value;
		//gOPR.m_vecDacValue[17] = value;
		//gOPR.m_vecDacValue[20] = value;
		//gOPR.m_vecDacValue[21] = value;

		gOPR.m_vecDacValue[0][16] = 2.0 * (value * gOPR.m_DacCal[0][16].gain + gOPR.m_DacCal[0][16].offset);
		gOPR.m_vecDacValue[1][16] = 2.0 * (value * gOPR.m_DacCal[1][16].gain + gOPR.m_DacCal[1][16].offset);

		gOPR.m_vecDacValue[0][17] = 2.0 * (value * gOPR.m_DacCal[0][17].gain + gOPR.m_DacCal[0][17].offset);
		gOPR.m_vecDacValue[1][17] = 2.0 * (value * gOPR.m_DacCal[1][17].gain + gOPR.m_DacCal[1][17].offset);

		gOPR.m_vecDacValue[0][20] = 2.0 * (value * gOPR.m_DacCal[0][20].gain + gOPR.m_DacCal[0][20].offset);
		gOPR.m_vecDacValue[1][20] = 2.0 * (value * gOPR.m_DacCal[1][20].gain + gOPR.m_DacCal[1][20].offset);

		gOPR.m_vecDacValue[0][21] = 2.0 * (value * gOPR.m_DacCal[0][21].gain + gOPR.m_DacCal[0][21].offset);
		gOPR.m_vecDacValue[1][21] = 2.0 * (value * gOPR.m_DacCal[1][21].gain + gOPR.m_DacCal[1][21].offset);
	}
	else if(type == IO)  {
		gOPR.m_DutIOVTT = value;
		//value *= 2.0;
		//gOPR.m_vecDacValue[18] = value;
		//gOPR.m_vecDacValue[19] = value;
		//gOPR.m_vecDacValue[22] = value;
		//gOPR.m_vecDacValue[23] = value;

		gOPR.m_vecDacValue[0][18] = 2.0 * (value * gOPR.m_DacCal[0][18].gain + gOPR.m_DacCal[0][18].offset);
		gOPR.m_vecDacValue[1][18] = 2.0 * (value * gOPR.m_DacCal[1][18].gain + gOPR.m_DacCal[1][18].offset);

		gOPR.m_vecDacValue[0][19] = 2.0 * (value * gOPR.m_DacCal[0][19].gain + gOPR.m_DacCal[0][19].offset);
		gOPR.m_vecDacValue[1][19] = 2.0 * (value * gOPR.m_DacCal[1][19].gain + gOPR.m_DacCal[1][19].offset);

		gOPR.m_vecDacValue[0][22] = 2.0 * (value * gOPR.m_DacCal[0][22].gain + gOPR.m_DacCal[0][22].offset);
		gOPR.m_vecDacValue[1][22] = 2.0 * (value * gOPR.m_DacCal[1][22].gain + gOPR.m_DacCal[1][22].offset);

		gOPR.m_vecDacValue[0][23] = 2.0 * (value * gOPR.m_DacCal[0][23].gain + gOPR.m_DacCal[0][23].offset);
		gOPR.m_vecDacValue[1][23] = 2.0 * (value * gOPR.m_DacCal[1][23].gain + gOPR.m_DacCal[1][23].offset);
	}
	else if(type == ALL) {
		gOPR.m_DutDRVTT = value;
		gOPR.m_DutIOVTT = value;
		//value *= 2.0;
		//gOPR.m_vecDacValue[16] = value;
		//gOPR.m_vecDacValue[17] = value;
		//gOPR.m_vecDacValue[20] = value;
		//gOPR.m_vecDacValue[21] = value;
//
		//gOPR.m_vecDacValue[18] = value;
		//gOPR.m_vecDacValue[19] = value;
		//gOPR.m_vecDacValue[22] = value;
		//gOPR.m_vecDacValue[23] = value;

		gOPR.m_vecDacValue[0][16] = 2.0 * (value * gOPR.m_DacCal[0][16].gain + gOPR.m_DacCal[0][16].offset);
		gOPR.m_vecDacValue[1][16] = 2.0 * (value * gOPR.m_DacCal[1][16].gain + gOPR.m_DacCal[1][16].offset);

		gOPR.m_vecDacValue[0][17] = 2.0 * (value * gOPR.m_DacCal[0][17].gain + gOPR.m_DacCal[0][17].offset);
		gOPR.m_vecDacValue[1][17] = 2.0 * (value * gOPR.m_DacCal[1][17].gain + gOPR.m_DacCal[1][17].offset);

		gOPR.m_vecDacValue[0][20] = 2.0 * (value * gOPR.m_DacCal[0][20].gain + gOPR.m_DacCal[0][20].offset);
		gOPR.m_vecDacValue[1][20] = 2.0 * (value * gOPR.m_DacCal[1][20].gain + gOPR.m_DacCal[1][20].offset);

		gOPR.m_vecDacValue[0][21] = 2.0 * (value * gOPR.m_DacCal[0][21].gain + gOPR.m_DacCal[0][21].offset);
		gOPR.m_vecDacValue[1][21] = 2.0 * (value * gOPR.m_DacCal[1][21].gain + gOPR.m_DacCal[1][21].offset);


		gOPR.m_vecDacValue[0][18] = 2.0 * (value * gOPR.m_DacCal[0][18].gain + gOPR.m_DacCal[0][18].offset);
		gOPR.m_vecDacValue[1][18] = 2.0 * (value * gOPR.m_DacCal[1][18].gain + gOPR.m_DacCal[1][18].offset);

		gOPR.m_vecDacValue[0][19] = 2.0 * (value * gOPR.m_DacCal[0][19].gain + gOPR.m_DacCal[0][19].offset);
		gOPR.m_vecDacValue[1][19] = 2.0 * (value * gOPR.m_DacCal[1][19].gain + gOPR.m_DacCal[1][19].offset);

		gOPR.m_vecDacValue[0][22] = 2.0 * (value * gOPR.m_DacCal[0][22].gain + gOPR.m_DacCal[0][22].offset);
		gOPR.m_vecDacValue[1][22] = 2.0 * (value * gOPR.m_DacCal[1][22].gain + gOPR.m_DacCal[1][22].offset);

		gOPR.m_vecDacValue[0][23] = 2.0 * (value * gOPR.m_DacCal[0][23].gain + gOPR.m_DacCal[0][23].offset);
		gOPR.m_vecDacValue[1][23] = 2.0 * (value * gOPR.m_DacCal[1][23].gain + gOPR.m_DacCal[1][23].offset);
	}
	else
		return;
}

//void DUTVREF(double value)
//{
//	gCOM.debug("	%s Level:%d in -> ", __FUNCTION__, value);
//
//	gOPR.m_DutVREF = value;
//	value /= 2.0;
//	gOPR.m_vecDacValue[24] = value;
//	gOPR.m_vecDacValue[25] = value;
//	gOPR.m_vecDacValue[26] = value;
//	gOPR.m_vecDacValue[27] = value;
//	gOPR.m_vecDacValue[28] = value;
//	gOPR.m_vecDacValue[29] = value;
//	gOPR.m_vecDacValue[30] = value;
//	gOPR.m_vecDacValue[31] = value;
//
//	//uint16_t code = (value/5.0)*4096;
//
//	//for(int pci = 0; pci < MAX_PCI; pci++) {
//	//	for( unsigned int n = 0 ; n < ch_vec.size(); n++)
//	//		gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch_vec[n], code);
//	//}
//}

void DUTVREF1(double value)
{
	gCOM.debug("	%s Level:%d in -> ", __FUNCTION__, value);

	gOPR.m_DutVREF = value;
	//value /= 2.0;
	//gOPR.m_vecDacValue[24] = value;
	//gOPR.m_vecDacValue[25] = value;
	//gOPR.m_vecDacValue[26] = value;
	//gOPR.m_vecDacValue[27] = value;

	gOPR.m_vecDacValue[0][24] = (value * gOPR.m_DacCal[0][24].gain + gOPR.m_DacCal[0][24].offset) / 2.0;
	gOPR.m_vecDacValue[1][24] = (value * gOPR.m_DacCal[1][24].gain + gOPR.m_DacCal[1][24].offset) / 2.0;

	gOPR.m_vecDacValue[0][25] = (value * gOPR.m_DacCal[0][25].gain + gOPR.m_DacCal[0][25].offset) / 2.0;
	gOPR.m_vecDacValue[1][25] = (value * gOPR.m_DacCal[1][25].gain + gOPR.m_DacCal[1][25].offset) / 2.0;

	gOPR.m_vecDacValue[0][26] = (value * gOPR.m_DacCal[0][26].gain + gOPR.m_DacCal[0][26].offset) / 2.0;
	gOPR.m_vecDacValue[1][26] = (value * gOPR.m_DacCal[1][26].gain + gOPR.m_DacCal[1][26].offset) / 2.0;

	gOPR.m_vecDacValue[0][27] = (value * gOPR.m_DacCal[0][27].gain + gOPR.m_DacCal[0][27].offset) / 2.0;
	gOPR.m_vecDacValue[1][27] = (value * gOPR.m_DacCal[1][27].gain + gOPR.m_DacCal[1][27].offset) / 2.0;

	//uint16_t code = (value/5.0)*4096;

	//for(int pci = 0; pci < MAX_PCI; pci++) {
	//	for( unsigned int n = 0 ; n < ch_vec.size(); n++)
	//		gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch_vec[n], code);
	//}
}

void DUTVREF2(double value)
{
	gCOM.debug("	%s Level:%d in -> ", __FUNCTION__, value);

	gOPR.m_DutVREF = value;
	//value /= 2.0;
	//gOPR.m_vecDacValue[28] = value;
	//gOPR.m_vecDacValue[29] = value;
	//gOPR.m_vecDacValue[30] = value;
	//gOPR.m_vecDacValue[31] = value;

	gOPR.m_vecDacValue[0][28] = (value * gOPR.m_DacCal[0][28].gain + gOPR.m_DacCal[0][28].offset) / 2.0;
	gOPR.m_vecDacValue[1][28] = (value * gOPR.m_DacCal[1][28].gain + gOPR.m_DacCal[1][28].offset) / 2.0;

	gOPR.m_vecDacValue[0][29] = (value * gOPR.m_DacCal[0][29].gain + gOPR.m_DacCal[0][29].offset) / 2.0;
	gOPR.m_vecDacValue[1][29] = (value * gOPR.m_DacCal[1][29].gain + gOPR.m_DacCal[1][29].offset) / 2.0;

	gOPR.m_vecDacValue[0][30] = (value * gOPR.m_DacCal[0][30].gain + gOPR.m_DacCal[0][30].offset) / 2.0;
	gOPR.m_vecDacValue[1][30] = (value * gOPR.m_DacCal[1][30].gain + gOPR.m_DacCal[1][30].offset) / 2.0;

	gOPR.m_vecDacValue[0][31] = (value * gOPR.m_DacCal[0][31].gain + gOPR.m_DacCal[0][31].offset) / 2.0;
	gOPR.m_vecDacValue[1][31] = (value * gOPR.m_DacCal[1][31].gain + gOPR.m_DacCal[1][31].offset) / 2.0;

	//uint16_t code = (value/5.0)*4096;

	//for(int pci = 0; pci < MAX_PCI; pci++) {
	//	for( unsigned int n = 0 ; n < ch_vec.size(); n++)
	//		gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch_vec[n], code);
	//}
}

void TIME(int seq, unsigned long long Select, float value)
{
	gOPR.dpsPowerSeq(seq, Select, value);
}

void HDPS(int type, double Level, bool bCalApply)
{
	gDPS.SetHVout(type, Level);

	gOPR.m_PMonArr[20 + type].sv = Level;
	gOPR.m_PMonArr[20 + type].uv = -1.0;
	gOPR.m_PMonArr[20 + type].ov = -1.0;
	gOPR.m_PMonArr[20 + type].oc = -1.0;
}

void DPS(unsigned long long type, double level, double under, double over, double limit, bool bCalApply)
{
	gCOM.debug("	%s type %X in -> ", __FUNCTION__, type);

	if(type > VSALL || type < 0) {
		MESSAGE("DPS channel check.", ERR);
		return;
	}

	unsigned int dpsCh = 0;
	if(type < 0x100) { /// 0, 1, 2, 3, 4, 5, ... , 19
		if(type < 0 || type > 19) {
			MESSAGE("DPS channel check.", ERR);
			return;
		}
		dpsCh = (0x1 << type);
	}else {/// VS0, VS1, VS2, VS3, VS4 ... VS19
		int count = 0;
		for(int idx = 0; idx < MAX_DPS_CH; idx++) {
			if((type & (VS0<<idx)) != 0) {
				count++;
				dpsCh |= (0x1 << idx);
			}
		}
		if(count != 1) {
			MESSAGE("DPS channel check.", ERR);
			return;
		}
	}

	double diff = 0.0;
	if(level < 0.6) {
		char strMsg[100];
		sprintf(strMsg, "DPS set level is low. Set(%f) Limit(0.6)\n", level);
		printf(strMsg);
		MESSAGE(strMsg, WARN);
		diff = 0.6 - level;
		level = 0.6;
		under += diff;
		over += diff;
	}

	if(level < under) MESSAGE("DPS set and under level check.", ERR);
	if(level > over) MESSAGE("DPS Set and over level check.", ERR);
	if(under >= over) MESSAGE("DPS Set, over and under level check.", ERR);
	// gIO.dps_write();

	for(int i = 0; i < MAX_DPS_CH; i++) {
		if(!(dpsCh & (1 << i )))
			continue;
		gOPR.m_PMonArr[i].sv = level;
		gOPR.m_PMonArr[i].uv = under;
		gOPR.m_PMonArr[i].ov = over;
		gOPR.m_PMonArr[i].oc = limit;

		//if(bCalApply) level = (level - gOPR.m_DPSVSCal[dpsCh].offset) / gOPR.m_DPSVSCal[dpsCh].gain + gOPR.m_DPSVSCal[dpsCh].offset2;
		gDPS.SetVout(i, level, under, over, limit, bCalApply);
		//printf("read dps %d run   %X\n", i, gIO.read_dps_run(PCI_SEL(i/10)));
		//printf("read dps %d sync  %X\n", i, gIO.read_dps_sync(PCI_SEL(i/10)));
		//printf("read dps %d sts   %X\n", i, gIO.read_dps_sts(PCI_SEL(i/10)));
		//printf("read dps %d fault %X\n", i, gIO.read_dps_fault(PCI_SEL(i/10)));
		//printf("== DPS ch %d SET %fV\n",i, level);
	}

	gCOM.debug("	%s out <- ", __FUNCTION__);
}


float READ_VOUT(int type, int cnt, bool bCalApply)
{
	gCOM.debug("	%s type %X in -> ", __FUNCTION__, type);

	if(type > 19 || type < 0) {
		MESSAGE("DPS channel check.", ERR);
		return -1.0;
	}

	float Volt;
	gDPS.ReadChVout(type, Volt, cnt, bCalApply);

	return Volt;
}

float READ_VOUT_ADC(int ch)
{
	if (ch < 0 || ch > 19)
		return -1.0;

	float volt;
	short data[8];
	gIO.adc_read(PCI_SEL(ch/10), ch+4, data);

	volt = data[5] * 10.0 / 65535.0;
	volt = volt * gOPR.m_AdcCal[ch/10][5].gain + gOPR.m_AdcCal[ch/10][5].offset;
	return volt;
}

float READ_IOUT(int type, int cnt, bool bCalApply)
{
	gCOM.debug("	%s type %X in -> ", __FUNCTION__, type);

	if(type > 19 || type < 0) {
		MESSAGE("DPS channel check.", ERR);
		return -1.0;
	}

	return gDPS.ReadChIout(type, cnt, bCalApply);
}

float READ_DPS_TEMP1(int ch)
{
	if (ch < 0 || ch > 19)
		printf("DPS Channel number is invalide. : %d\n", ch);

	return gDPS.ReadTemperature_1(DPS_SINGLE_SEL, ch);
}

float READ_DPS_TEMP2(int ch)
{
	if (ch < 0 || ch > 19)
		printf("DPS Channel number is invalide. : %d\n", ch);

	return gDPS.ReadTemperature_2(DPS_SINGLE_SEL, ch);
}

void SRON()
{
	gCOM.debug("	%s in -> ", __FUNCTION__);
		
	uint64_t dps_sel = 0xFFFFF;
	uint16_t code[2] = {0,0};

	//gIO.dac_vref_en(PCI1, 1);
	//gIO.dac_vref_en(PCI2, 1);
	gDAC.dac_vref_en(1);

	//gIO.dac_vtt_bias_en(PCI1, 1);
	//gIO.dac_vtt_bias_en(PCI2, 1);
	gDAC.dac_vtt_bias_en(1);

	bool bS3S5 = true;

	////// Power On Sequence /////////////////////////
	if(gOPR.m_PSeq[0].chBit == 0) {
		//gIO.dac_reset(PCI1);
		//gIO.dac_reset(PCI2);
		//gDAC.dac_reset();

		usleep(300000);

		// gIO.dac_vref_en(PCI1, 1);
		// gIO.dac_vref_en(PCI2, 1);
		// gDAC.dac_vref_en(1);

		//gIO.dac_vtt_bias_en(PCI1, 1);
		//gIO.dac_vtt_bias_en(PCI2, 1);
		//gDAC.dac_vref_en(1)
		
		//DG S3 S5 enable
		gIO.writePG(PCI1, 0x3, 0x238, 0xFF, 1);
		gIO.writePG(PCI2, 0x3, 0x238, 0xFF, 1);
    	//PG S3 S5 enable
		gIO.writePG(PCI1, 0x3, 0x239, 0xFF, 1);
		gIO.writePG(PCI2, 0x3, 0x239, 0xFF, 1);

		for(int ch = 0; ch < 32 ; ch++) {
			//code = (gOPR.m_vecDacValue[ch]/5.0)*4095;
			//code[0] = ((gOPR.m_vecDacValue[ch]* gOPR.m_DacCal[0][ch].gain + gOPR.m_DacCal[0][ch].offset)/5.0)*4095;
			//code[1] = ((gOPR.m_vecDacValue[ch]* gOPR.m_DacCal[1][ch].gain + gOPR.m_DacCal[1][ch].offset)/5.0)*4095;

			code[0] = (gOPR.m_vecDacValue[0][ch]/5.0)*4095;
			code[1] = (gOPR.m_vecDacValue[1][ch]/5.0)*4095;
			for(int pci = 0; pci < MAX_PCI; pci++) {
				gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code[pci]);
				gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code[pci]);
			}
		}

		gIO.dps_run(PCI1, dps_sel & 0x3FF, 0x1, 1);
		gIO.dps_run(PCI2, (dps_sel >> 10) & 0x3FF, 0x1, 1);

		//gOPR.m_flgDpsOn = dps_sel;
		//printf("SRON DPS SEL %X\n", dps_sel);
	} else {
		for(unsigned int idx = 0; idx < gOPR.m_PSeq.size(); idx++ ) // seq
		{
			dps_sel = 0;
			if(gOPR.m_PSeq[idx].chBit == 0) continue;
			printf("On Seq(%d) Sel(0x%lX) Delay(%f)\n", idx, gOPR.m_PSeq[idx].chBit, gOPR.m_PSeq[idx].delay);
			for(int i = 0 ; i < 32; i++) {
				if(gOPR.m_PSeq[idx].chBit & ((uint64_t)0x100 << i)) { // channel
					dps_sel |= (0x1 << i);
				}
			}

			gIO.dps_run(PCI1, (dps_sel & 0x3FF),       dps_sel & (0x1<<20), 1);
			gIO.dps_run(PCI2, ((dps_sel>>10) & 0x3FF), dps_sel & (0x1<<21), 1);

			if(dps_sel & (0x1<<23)) {// DUTVREF1
				//printf("DutVREF 1 Set\n");
				//if(bS3S5) {
				//	bS3S5 = false;
				//	//DG S3 S5 enable
				//	gIO.writePG(PCI1, 0x3, 0x238, 0xFF, 1);
				//	gIO.writePG(PCI2, 0x3, 0x238, 0xFF, 1);
				//	//PG S3 S5 enable
				//	gIO.writePG(PCI1, 0x3, 0x239, 0xFF, 1);
				//	gIO.writePG(PCI2, 0x3, 0x239, 0xFF, 1);
				//}

				for(int ch = 24; ch < 28 ; ch++) {
					//code = (gOPR.m_vecDacValue[ch]/5.0)*4095;
					//code[0] = ((gOPR.m_vecDacValue[ch]* gOPR.m_DacCal[0][ch].gain + gOPR.m_DacCal[0][ch].offset)/5.0)*4095;
					//code[1] = ((gOPR.m_vecDacValue[ch]* gOPR.m_DacCal[1][ch].gain + gOPR.m_DacCal[1][ch].offset)/5.0)*4095;

					code[0] = (gOPR.m_vecDacValue[0][ch]/5.0)*4095;
					code[1] = (gOPR.m_vecDacValue[1][ch]/5.0)*4095;
					//printf("1 DAC ch %d : %fV(%d)\n", ch, gOPR.m_vecDacValue[ch], code);
					for(int pci = 0; pci < MAX_PCI; pci++) {
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code[pci]);
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code[pci]);
					}
				}
				
			}

			if(dps_sel & (0x1<<24)) {// DUTVREF2
				printf("DutVREF 2 Set\n");
				//if(bS3S5) {
				//	bS3S5 = false;
				//	//DG S3 S5 enable
				//	gIO.writePG(PCI1, 0x3, 0x238, 0xFF, 1);
				//	gIO.writePG(PCI2, 0x3, 0x238, 0xFF, 1);
				//	//PG S3 S5 enable
				//	gIO.writePG(PCI1, 0x3, 0x239, 0xFF, 1);
				//	gIO.writePG(PCI2, 0x3, 0x239, 0xFF, 1);
				//}

				for(int ch = 28; ch < 32 ; ch++) {
					//code = (gOPR.m_vecDacValue[ch]/5.0)*4095;
					//code[0] = ((gOPR.m_vecDacValue[ch]* gOPR.m_DacCal[0][ch].gain + gOPR.m_DacCal[0][ch].offset)/5.0)*4095;
					//code[1] = ((gOPR.m_vecDacValue[ch]* gOPR.m_DacCal[1][ch].gain + gOPR.m_DacCal[1][ch].offset)/5.0)*4095;

					code[0] = (gOPR.m_vecDacValue[0][ch]/5.0)*4095;
					code[1] = (gOPR.m_vecDacValue[1][ch]/5.0)*4095;
					//printf("2 DAC ch %d : %fV(%d)\n", ch, gOPR.m_vecDacValue[ch], code);
					for(int pci = 0; pci < MAX_PCI; pci++) {
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code[pci]);
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code[pci]);
					}
				}
				
			}

			if(dps_sel & (0x1<<22)) {
				// PE Voltage Set 은 한번만
				dps_sel &= ~(0x1<<22);

				if(bS3S5) {
					bS3S5 = false;
					//DG S3 S5 enable
					gIO.writePG(PCI1, 0x3, 0x238, 0xFF, 1);
					gIO.writePG(PCI2, 0x3, 0x238, 0xFF, 1);
    				//PG S3 S5 enable
					gIO.writePG(PCI1, 0x3, 0x239, 0xFF, 1);
					gIO.writePG(PCI2, 0x3, 0x239, 0xFF, 1);
				}

				for(int ch = 0; ch < 24 ; ch++) {
					//code = (gOPR.m_vecDacValue[ch]/5.0)*4095;
					//code[0] = ((gOPR.m_vecDacValue[ch]* gOPR.m_DacCal[0][ch].gain + gOPR.m_DacCal[0][ch].offset)/5.0)*4095;
					//code[1] = ((gOPR.m_vecDacValue[ch]* gOPR.m_DacCal[1][ch].gain + gOPR.m_DacCal[1][ch].offset)/5.0)*4095;
					
					code[0] = (gOPR.m_vecDacValue[0][ch]/5.0)*4095;
					code[1] = (gOPR.m_vecDacValue[1][ch]/5.0)*4095;
					for(int pci = 0; pci < MAX_PCI; pci++) {
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code[pci]);
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code[pci]);
					}
				}
			}
			//printf("SRON %d DPS SEL %X\n", idx , dps_sel );
			usleep(gOPR.m_PSeq[idx].delay);
		}
//		gOPER.m_flgDpsOn |= dps_sel;
	}

	
	//gIO.pe_chip_on();
	// gIO.pe_write(dev_sel, ch_sel, PE_CH_POWER_ON_CTRL, 0x0B1F);
	// gIO.pe_write(dev_sel, ch_sel, PE_C_PDO_RESET, 0x2201);
	gNET.m_nSendPower = SEND_PWR_OFF;
	gCOM.debug("	%s out <- ", __FUNCTION__);
}

void SROF()
{
	gCOM.debug("	%s in -> ", __FUNCTION__);

	//gIO.dac_reset(PCI1);
	//gIO.dac_reset(PCI2);
	//gDAC.dac_reset();

	uint32_t dps_sel = 0xFFFFF;
	uint16_t code = 0;

	if(gOPR.m_PSeq[0].chBit == 0) {
		for(int ch = 0; ch < 32 ; ch++) {
			for(int pci = 0; pci < MAX_PCI; pci++) {
				gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, 0);
				gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, 0);
			}
		}

		//for(int idx = DPS_START_CH; idx < MAX_DPS_CH; idx++)
		//	gDPS.SetVout(idx, 0.1, 0, 0.1, 5);

		gIO.dps_run(PCI1, 0x3FF, 0x1, 0);
		gIO.dps_run(PCI2, 0x3FF, 0x1, 0);

		DELAY(0.1);

		//DG S3 S5 enable
		gIO.writePG(PCI1, 0x3, 0x238, 0, 1);
		gIO.writePG(PCI2, 0x3, 0x238, 0, 1);
   		//PG S3 S5 enable
		gIO.writePG(PCI1, 0x3, 0x239, 0, 1);
		gIO.writePG(PCI2, 0x3, 0x239, 0, 1);
	}else
	{
		for(int idx = gOPR.m_PSeq.size() - 1; idx > -1 ;idx-- ) // seq
		{
			if(gOPR.m_PSeq[idx].chBit == 0) continue;

			usleep(gOPR.m_PSeq[idx].delay);

			dps_sel = 0;
			//printf("On Seq(%d) Sel(0x%X) Delay(%f)\n", idx, gOPR.m_PSeq[idx].chBit, gOPR.m_PSeq[idx].delay);
			for(int i = 0 ; i < 32; i++) {
				if(gOPR.m_PSeq[idx].chBit & (0x100 << i)) { // channel
					dps_sel |= (0x1 << i);
				}
			}

			gIO.dps_run(PCI1, (dps_sel & 0x3FF),       dps_sel & (0x1<<20), 0);
			gIO.dps_run(PCI2, ((dps_sel>>10) & 0x3FF), dps_sel & (0x1<<21), 0);

			if(dps_sel & (0x1<<23)) { // DVREF1
				for(int ch = 24; ch < 28 ; ch++) {
					code = 0;// (0.0/5.0)*4096;
					for(int pci = 0; pci < MAX_PCI; pci++) {
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code);
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code);
					}
				}
			}

			if(dps_sel & (0x1<<24)) { // DVREF2
				for(int ch = 28; ch < 32 ; ch++) {
					code = 0;// (0.0/5.0)*4096;
					for(int pci = 0; pci < MAX_PCI; pci++) {
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code);
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code);
					}
				}
			}

			if(dps_sel & (0x1<<22)) {
				for(int ch = 0; ch < 24 ; ch++) {
					code = 0;// (0.0/5.0)*4096;
					//if(ch == 0 || ch == 3) // IO VIH
					//	code = 4.224/1.962 * 1.2;
					//if(ch == 1 || ch == 4) // DR VIH
					//	code = 4.224/1.989 * 1.2;

					//if(ch == 2 || ch == 5) // VTH
					//	code = 4.224/1.989 * 0.6;

					for(int pci = 0; pci < MAX_PCI; pci++) {
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code);
						gIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, code);
					}
				}

				////DG S3 S5 disable
				//gIO.writePG(PCI1, 0x3, 0x238, 0x0, 1);
				//gIO.writePG(PCI2, 0x3, 0x238, 0x0, 1);
   				////PG S3 S5 disable
				//gIO.writePG(PCI1, 0x3, 0x239, 0x0, 1);
				//gIO.writePG(PCI2, 0x3, 0x239, 0x0, 1);
			}
			printf("SROF %d DPS SEL %X\n", idx , dps_sel & 0xFFFFF);
			//if(idx > 0)
			//	usleep(gOPR.m_PSeq[idx-1].delay);

		}
		//DG S3 S5 disable
		gIO.writePG(PCI1, 0x3, 0x238, 0x0, 1);
		gIO.writePG(PCI2, 0x3, 0x238, 0x0, 1);
   		//PG S3 S5 disable
		gIO.writePG(PCI1, 0x3, 0x239, 0x0, 1);
		gIO.writePG(PCI2, 0x3, 0x239, 0x0, 1);
	}

	//gIO.dac_vtt_bias_en(PCI1, 0);
	//gIO.dac_vtt_bias_en(PCI2, 0);
	//gDAC.dac_vtt_bias_en(0);
//
	//gIO.dac_vref_en(PCI1, 0);
	//gIO.dac_vref_en(PCI2, 0);
	//gDAC.dac_vref_en(0);

}

/************************************
	PG
************************************/

/************************************
	OPERRATION
************************************/
void DELAY(double time)
{
	if(_bAddStepMap) return;

	if(time > 1.0)
		sleep(time);
	else 
		usleep(time*1000000);
	
}

void MESSAGE(const char* msg, int type)
{
	if(_bAddStepMap) return;

	gNET.send_msg(type, msg);
}

void RUN_TEMP(double temp, int Mode, int timeLimit)
{
	// Setting Default State of PLC On  if State of PLC Off is Checked reqTemp
	gCOM.debug("	%s in -> ", __FUNCTION__);
	gOPR.m_bSetTemp = true;
	gOPR.m_fSetTemp = temp;
	gNET.sendTemp(1);
	if (Mode == WAIT)
		gOPR.waitTemp(timeLimit);
	gCOM.debug("	%s out <- ", __FUNCTION__);
}

double READ_TEMP(int Mode)
{
	double ret = gOPR.m_fReadTemp;
	if (Mode == SV)
		ret = gOPR.m_fSetTemp;

	return ret;
}

void SOAK_TIME(int time)
{
	gCOM.debug("	%s in -> ", __FUNCTION__);
	gNET.sendAging(2, time);
	gNET.m_nSendPower = SEND_PWR_ZERO;
	DELAY(time);
	gNET.m_nSendPower = SEND_PWR_OFF;
	gCOM.debug("	%s out <- ", __FUNCTION__);
}


void AGING(const char* pc, int time)
{
	gCOM.debug("	%s in -> ", __FUNCTION__);
	gNET.sendAging(1, time);
	
	gOPR.startTest(pc);
	gOPR.waitTest(SLEEP_100US, time);
	gOPR.stopTest();
	gCOM.debug("	%s out <- ", __FUNCTION__);
}


/************************************
	PATTERN
************************************/
void SEND_MPAT(const char* file)
{
	gCOM.debug("	%s %s in -> ", __FUNCTION__, file);
	gPG.writePattern(file);
	gCOM.debug("	%s out <- ", __FUNCTION__);
}

void MEAS_MPAT(const char* pc, bool Judge, bool Show)
{
	//	COMPARE_MODE();
	gCOM.debug("	%s %s in -> ", __FUNCTION__, pc);
	gFM.writeScanIndex(0);
	gOPR.startTest(pc);	
	gOPR.waitTest(gOPR.m_nDefaultMpatDelay);
	gOPR.stopTest();

	if (Judge) 
		JUDGE(Show);

	gCOM.debug("	%s out <- ", __FUNCTION__);
}

void START_MPAT(const char* pc) // Start Pattern Without WAIT_MPAT
{
	gCOM.debug("	%s %s in -> ", __FUNCTION__, pc);
	gOPR.startTest(pc);
	gCOM.debug("	%s out <- ", __FUNCTION__);
}

void STOP_MPAT()
{
	gOPR.stopTest();
}

void WAIT_MPAT(uint wait)
{
	gCOM.debug("	%s wait:%d in -> ", __FUNCTION__, wait);
	gOPR.waitTest(gOPR.m_nDefaultMpatDelay, wait);
	gOPR.stopTest();
	gCOM.debug("	%s out <- ", __FUNCTION__);
}

void SET_JNCFLAG(uint val)//
{
	gPG.setJnc(val);
}


void SET_REG(uint reg, uint val)//
{
	gPG.setReg(reg, val);
}

void SET_TIMER(uint reg, double val)
{
	gPG.setTimer(reg, val);
}


/************************************
	FAIL MEMORY
************************************/
void JUDGE(bool bShow) 
{
	gBIB.judge(bShow);
}	

void INVERT()
{
	gBIB.m_bResultInvert = true;
}

void DATA_MODE(int process) 
{
	gFM.m_nDutDataProc = process;
	gFM.setFmMode(FM_DATA);
}

void NAND_MODE()
{
	gFM.setFmMode(FM_NAND);
}

void COMPARE_MODE(bool FM_Capture_Always)
{
	int conditionCapture = 1;
	if (FM_Capture_Always) conditionCapture = 0;
	gFM.setFmMode(FM_COMP, conditionCapture);
}

void DDR_MODE(bool bDDR, bool bFirst)
{
	gBIB.m_bDDR = bDDR;
	gBIB.m_bJudgeFirtst = bFirst;
}

void FM_DRV_MODE(int mode)
{
	gIO.writePG(PCI1, PG_ALL, (1 << 9) | 21, mode, 1);
	gIO.writePG(PCI2, PG_ALL, (1 << 9) | 21, mode, 1);
}


int READ_FMDATA(int read)
{
	return gFM.readFmData(read);
}

FM* GET_FMDATA_H(int nPG, int pos)
{
	return (FM*)gFM.getFmDataH(nPG, pos);
}

FM* GET_FMDATA_L(int nPG, int pos)
{
	return (FM*)gFM.getFmDataL(nPG, pos);
}

int READ_SCAN()
{
	return gFM.readScanIndex();
}

void READ_DUTDATA()
{
	gNAND.readData(gFM.readScanIndex());
	
}

void SHOW_FM_DRV(int nPG, int ofs, int size)
{
	gFM.showFmDrv(nPG, ofs, size);
}

void SHOW_FM_DATA(int nPG, int ofs, int size)
{
	gFM.showFmData(nPG, ofs, size);
}

void end_test();

int main(int argc, char* argv[])
{    
	printf("DATE : %s, API VER : %s\n", VER_DATE, API_VER);
	gCOM.loadStarLib();
	gCOM.doArgument(argc, argv);
	gIO.open_pcie();
	//gIO.c2p_lb_clk_sel(0x606);
	//gIO.c2t_read_latency(3);
	//gIO.c2t_lb_clk_sel(2);

	if (gCOM.m_nTestMode == TEST_MODE_CMD)
	{
		gCMD.cmd();
		return 1;
	}
//printf("==== %s ==== 0 =====\n", __FUNCTION__);
	gNET.shm_init();
	gCOM.loadConfig();

	if (!gIO.checkPG(PCI1, gCOM.m_nPGID[0]))
	{
		gNET.send_msg(MSG_TYPE_ERR, "Ctrl 0 PG_FPGA Is Not Loaded! Process PG Upload.");
	}

	if (!gIO.checkPG(PCI2, gCOM.m_nPGID[2]))
	{
		gNET.send_msg(MSG_TYPE_ERR, "Ctrl 1 PG_FPGA Is Not Loaded! Process PG Upload.");
	}
//printf("==== %s ==== 1 =====\n", __FUNCTION__);

	//gIO.cvm_set_config();
	//gIO.cvm_write_cal_reg();

	//gIO.dac_reset(PCI1);
	//gIO.dac_reset(PCI2);
	gDAC.dac_reset();
	gIO.adc_reset(PCI1);
	gIO.adc_reset(PCI2);
	gIO.adc_settling(PCI1, 1000000);
	gIO.adc_settling(PCI2, 1000000);
	gPG.stopPG();

//printf("==== %s ==== 2 =====\n", __FUNCTION__);
	//gFM.initFailMem();
	gPG.writeIOMask(0, 0, 0, 0);

//printf("==== %s ==== 3 =====\n", __FUNCTION__);

	printf("DPS 12V On\n");
	//gIO.dac_pps_12_en(PCI1, 1);
	//gIO.dac_pps_12_en(PCI2, 1);
	gDAC.dac_pps_12_en(1);

	//gIO.dac_init();
	gDAC.dac_init();

	Sleep(1);
//	printf("========= DPS Init ==========\n");
	gDPS.AllChannelInit();
//	printf("========= DPS Init END ==========\n");

//	printf("========= Cal Data Read ==========\n");
	gOPR.readCalFactor();
//	printf("========= Cal Data Read END==========\n");

//	printf("========= tdr Data Read ==========\n");
	gOPR.readTDRFactor();
//	printf("========= tdr Data Read END==========\n");


	//gIO.dac_vref_en(PCI1, 0);
	//gIO.dac_vref_en(PCI2, 0);
	//gDAC.dac_vref_en(0);
//
	//gIO.dac_vtt_bias_en(PCI1, 0);
	//gIO.dac_vtt_bias_en(PCI2, 0);
	//gDAC.dac_vtt_bias_en(0);
//
	////DG S3 S5 enable
	//gIO.writePG(PCI1, 0x3, 0x238, 0x0, 1);
	//gIO.writePG(PCI2, 0x3, 0x238, 0x0, 1);
  	////PG S3 S5 enable
	//gIO.writePG(PCI1, 0x3, 0x239, 0x0, 1);
	//gIO.writePG(PCI2, 0x3, 0x239, 0x0, 1);

	if (gCOM.m_nTestMode == TEST_MODE_DIAG) { // DIAG MODE
		diag_log_open();
		TEST();
		SROF();
		diag_log_close();
		return 1;
	}

	_bAddStepMap = true;  //  Make Function Map
	CONTACT();
	TEST();
	_bAddStepMap = false;

	gNET.sendStartTest();
	SETUP();

	//	printf("========= tdr Data Read ==========\n");
	gOPR.applyTDRFactor();
	//	printf("========= tdr Data Read END==========\n");


	gCOM.debug("%s in -> ", __FUNCTION__);

	if (gCOM.m_nTestMode == TEST_MODE_CONTACT) {	// CONTACT
		gRES.initResult();
		gNET.sendStepList();
		CONTACT();
		end_test();
		gRES.initCatFile();

	}
	else if (gCOM.m_nTestMode == TEST_MODE_RUN) {	// TEST
		gRES.loadResult(); 				
		TEST(); 
		end_test();
		gRES.binResult();
		gNET.sendBin();
	}
	else if (gCOM.m_nTestMode == TEST_MODE_SINGLE) {	// TEST
		gRES.loadResult();		
		gRES.loadCatFile();
		for (int i = 0; i<(int)gCOM.m_singleStepVec.size(); i++ )
			gCOM.singleStep(gCOM.m_singleStepVec[i].c_str());
		end_test();
		gRES.binResult();
		gNET.sendBin();

	}

	////DG S3 S5 enable
	//gIO.writePG(PCI1, 0x3, 0x238, 0x0, 1);
	//gIO.writePG(PCI2, 0x3, 0x238, 0x0, 1);
  	////PG S3 S5 enable
	//gIO.writePG(PCI1, 0x3, 0x239, 0x0, 1);
	//gIO.writePG(PCI2, 0x3, 0x239, 0x0, 1);
//
	//gIO.dac_vtt_bias_en(PCI1, 0);
	//gIO.dac_vtt_bias_en(PCI2, 0);
	//gDAC.dac_vtt_bias_en(0);
//
	//gIO.dac_vref_en(PCI1, 0);
	//gIO.dac_vref_en(PCI2, 0);
	//gDAC.dac_vref_en(0);

	if (gOPR.m_bSetTemp)
		if (gCOM.m_nTestMode != TEST_MODE_CONTACT)
			gNET.sendTemp(2);

	gNET.sendEndTest();
	gRES.freeResult();
	gRES.freeSubResult();
	usleep(1000 * 1000); // Wait Message Send

	gCOM.debug("%s out <- ", __FUNCTION__);
}

/************************************
	inner function
************************************/

void start_step(const char* name) 
{ 
	if (_bAddStepMap) {
		bool bAdd = true;
		for (int i = 0; i < gCOM.m_nStepIdx; i++)
		{
			if (strcmp(gCOM.m_sStepName[i], name) == 0)
				bAdd = false;
		}
		if (bAdd)
			sprintf(gCOM.m_sStepName[gCOM.m_nStepIdx++], "%s", name);
		return;
	}

	gCOM.debug("%s %s in -> ", __FUNCTION__, name);
	printf("---- %s %s - Start ----\n", __FUNCTION__, name);	
	if (!gIO.checkPG(PCI1, gCOM.m_nPGID[0]) ) {
		gERR.addErr(ERR_IO, __FUNCTION__, "Ctrl 0 Can't Communicate FPGA. Check Firmware");
	}

	if (!gIO.checkPG(PCI2, gCOM.m_nPGID[2])) {
		gERR.addErr(ERR_IO, __FUNCTION__, "Ctrl 1 Can't Communicate FPGA. Check Firmware");
	}
	
	
	gFM.setReadDut();

	sprintf(gCOM.m_sCurStep, "%s", name);
//	sprintf(gNET.m_pShmDef->sCurStep, "%s", name);

	gPG.m_nWritePC = 0;    // PT PC Count
	gBIB.initStepItem();	
	gRES.startStepRes();

	
	sprintf(gNET.m_pShmDef->sCurStep, "%s", gRES.m_sResCurStep);

	gNET.sendStartStep();
	gNET.sendPowerSet();
	gCOM.debug("%s %s out <- ", __FUNCTION__, name);
	printf("---- %s %s - End ----\n", __FUNCTION__, name);

}

void end_step() 
{ 
	if (_bAddStepMap) return;
	gCOM.debug("%s in -> ", __FUNCTION__);

	gRES.endStepRes();
	gNET.sendEndStep();

	gRES.saveResOption();
	gNET.sendResOption();

	gCOM.debug("%s out <- ", __FUNCTION__); 
}

void end_test()
{
	for(int n = 0 ; n < MAX_DPS_CH; n++)
		gDPS.SetVout(n, 0.0, -0.1, 0.3, 3.0 );

	gOPR.m_DutDRVTT = 0.0;
	gOPR.m_IOVIH = 0.0;
	gOPR.m_DRVIH = 0.0;
	gOPR.m_VTH = 0.0;
	gOPR.m_DRVTT = 0.0;
	gOPR.m_IOVTT = 0.0;
	gOPR.m_DutDRVTT = 0.0;
	gOPR.m_DutIOVTT = 0.0;
	gOPR.m_DutVREF = 0.0;

	for(int i = 0; i < 32 ; i++) {
		gOPR.m_vecDacValue[0][i] = 0.0;
		gOPR.m_vecDacValue[1][i] = 0.0;
	}

	TIME(0, 0, 0);
	SRON();
	usleep(2000000);
	SROF();
}

void writePG(uint adr, uint val, int sz)
{
	gIO.writePG(PCI1, PG_ALL, adr, val, sz);
	gIO.writePG(PCI2, PG_ALL, adr, val, sz);
}

void writeFPGA(const char* file, bool bPG)
{
	int ret = RTN_OK;

	ret = gIO.cfg_fpga(PCI1, file, bPG);
	
	if (ret != RTN_OK ) {
		gERR.addErr(ERR_IO, __FUNCTION__, "PCI0 FPGA Upload Fail!");
		gNET.send_msg(MSG_TYPE_ERR, "FPGA Upload Fail!");
	}

	ret = gIO.cfg_fpga(PCI2, file, bPG);

	if (ret != RTN_OK ) {
		gERR.addErr(ERR_IO, __FUNCTION__, "PCI1 FPGA Upload Fail!");
		gNET.send_msg(MSG_TYPE_ERR, "FPGA Upload Fail!");
	}
}

void showIO(bool bShow)
{
	gIO.m_bDebug = bShow;
}

void writeIO(uint adr, uint value)
{
	gIO.writeIO(PCI1, adr, value); 
	gIO.writeIO(PCI2, adr, value); 
}
uint readIO(int pci, uint adr)
{
	return gIO.readIO(PCI_SEL(pci), adr); 
}

void writePG(uint8_t PGSel, uint adr, uint value, uint size)
{
	int pci = (PGSel >> 1);
	int npg = PGSel & 1;
	gIO.writePG(PCI_SEL(pci), (1<<npg), adr, value, size);
	//gIO.writePG(PCI1, PGSel, adr, value, size);
	//gIO.writePG(PCI2, PGSel, adr, value, size);
}

void writeTG(uint adr, uint value, uint size)
{
	gIO.writeTG(PCI1, adr, value, size);
	gIO.writeTG(PCI2, adr, value, size);
}

uint readPG(uint8_t PGSel, uint adr, uint size)
{
	int pci = (PGSel >> 1);
	int npg = PGSel & 1;

	return gIO.readPG(PCI_SEL(pci), (1<<npg), adr, size);
	//return gIO.readPG(PCI_SEL(pci), PGSel, adr, size);
}

uint readTG(int pci, uint adr, uint size)
{
	return gIO.readTG(PCI_SEL(pci), adr, size);
}

void writeDPS(int pci, uint dev_no, uint cmd, ushort data, uint size)
{
	gIO.dps_write(PCI_SEL(pci), dev_no, cmd, size, (uint8_t*)&data);
}

ushort readDPS(int pci, uint dev_no, uint cmd, uint size)
{
	ushort data = 0;
	gIO.dps_read(PCI_SEL(pci), dev_no, cmd, size, (uint8_t*)&data );
	return data;
}

uint32_t readCtrlVer(int pci)
{
	return (gIO.readIO(	PCI_SEL(pci), 0xf010) & 0xFFFF);
}

uint32_t readCtrlDate(int pci)
{
	return (gIO.readIO(	PCI_SEL(pci), 0xf014) & 0xFFFFFF);
}


/*  
* 
#define VER_DATE "2025/08/11"
#define API_VER "1.0.5" by bin.yun
	 power send  1s -> 2s period 변경
* 
* 
#define VER_DATE "2025/07/11"
#define API_VER "1.0.4" by bin.yun
     temp 관련 wait temp 시 1분간격 set temp 보내주는걸로.. os 다시 붙었을때 때문 

#define VER_DATE "2025/06/27"
#define API_VER "1.0.3" by bin.yun
	stCalFactor 운영 부분 DIAG_HSBI 로 이동
    FM 관련 Interface 추가.

#define VER_DATE "2025/05/29"
#define API_VER "1.0.2" by bin.yun
      FM Read 1024 넘을시 두번째 Read 시 Count * FM_SIZE 안해준 부분 
	버그 수정


* #define VER_DATE "2025/05/16"
  #define API_VER "1.0.1" by bin.yun    
	Contact Test시 Step들 결과 Fail Sum되게..
* 
	#define VER_DATE "2025/02/12"
	#define API_VER "1.0.0"
		Temp Ctrl 관련 API 추가
 
	Ver 1.0.0  2024/11/25
		Send Binary Func Add (Register용)
		DPS Err Print  config.xml 옵션으로.  
			-> <pgm hide-dps-err="1" />

* 
* Ver 1.0.0  2024-03-06
*   Read DPS / Net OS Send

*/


