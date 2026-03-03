

#include "STAR_TEST.h"
#include "define.h"
#include "../starlib/starlib_dif.h"
#include "ioc.h"
#include "bib.h"
#include "result.h"
#include "pgc.h"
#include "tgc.h"
#include "starAPI.h"
#include "net.h"
#include "errc.h"
#include "macro.h"
#include "failmem.h"
#include "operation.h"
#include "dps.h"
#include "DIAG_TEST.h"
#include "vec.h"
#include "nand.h"
#include "cmd.h"
#include "power.h"

#include <stdarg.h>
#include <time.h>



ioc			g_clIO;				//	gIO
starAPI		g_clAPI;			//	gAPI
starlib		g_clLib;			//  gLIB
net			g_clNet;			//	gNET
errctrl		g_clErr;			//	gERR
bib			g_clBib;			//  gBIB
tgc			g_clTgc;			//	gTG
pgc			g_clPgc;			//	gPG
pds			g_clPds;			//  gPDS  
scramble	g_clScr;			//  gSCR
result		g_clRes;			//	gRES
failmem		g_clFm;				//	gFM
operation	g_clOperation;		//  gOPER
dps			g_clDPS;			// gDPS
vec			g_clVec;
nand		g_clNand;
command		g_clCmd;
powerc		g_clPower;



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
//	gPG.writeScan(table, value);
}

void ASCAN_TABLE(int table, uint value)
{
//	gPG.writeAScan(table, value);
}

void IO_MASK(uint mask0_31, uint mask32_63)
{
//	gPG.writeIOMask(mask0_31, mask32_63);
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
	if (gAPI.m_nTestMode == TEST_MODE_CONTACT) pDut->m_nConResult = pDut->m_nCurResult;
	pDut->m_nTotalResult = pDut->m_nPrevTotal;
	pDut->m_nTotalResult &= pDut->m_nCurResult;
}

void SET_FDUT(int dno)
{
	dut* pDut = gBIB.getDut(dno);
	if (pDut == NULL)return;
	printf("%s, dno %d", __FUNCTION__, dno);
	pDut->m_nCurResult = STEP_FAIL;	
	if (gAPI.m_nTestMode == TEST_MODE_CONTACT) pDut->m_nConResult = pDut->m_nCurResult;
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
	TS
************************************/
void RATE(double r0, double r1, double r2, double r3, double r4,
	double r5, double r6, double r7, double r8, double r9,
	double r10, double r11, double r12, double r13,
	double r14, double r15)
{
	gTG.writeRate(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15);
}

void ECLK(int type, double r0, double r1, double r2, double r3, double r4,
	double r5, double r6, double r7, double r8, double r9,
	double r10, double r11, double r12, double r13,
	double r14, double r15)
{
	gTG.writeEClk(type, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15);
}

void STRB_EDGE(int tsNo, double strb0, double strb1)
{
	gTG.writeSTRBEdge(tsNo, strb0, strb1);
}

void TSSET(uint pin, uint format, uint edgeB, uint edgeC)
{
	gTG.writeFormat(pin, format, edgeB, edgeC);
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
void SET_V(int type, double volt)
{
	gPWR.setDac(type, volt);
}

double GET_V(int type)
{
	return gPWR.getAdc(type);
}

void DPS(unsigned long long type, double level, double under, double over, double limit)
{
	if (!(type > VS1 && type < VS16)) {
		MESSAGE("DPS channel check.", ERR);
		return;
	}

	double diff = 0.0;
	if (level < 0.6) {
		char strMsg[100];
		sprintf(strMsg, "DPS set level is low. Set(%f) Limit(0.6)\n", level);
		printf(strMsg);
		MESSAGE(strMsg, WARN);
		diff = 0.6 - level;
		level = 0.6;
		under += diff;
		over += diff;
	}

	if (level < under) MESSAGE("DPS set and under level check.", ERR);
	if (level > over) MESSAGE("DPS Set and over level check.", ERR);
	if (under >= over) MESSAGE("DPS Set, over and under level check.", ERR);
	unsigned int dpsCh = 1 << type;
	for (int i = 0; i < MAX_DPS_CH; i++) {
		if (!(dpsCh & (1 << i)))
			continue;
		gPWR.m_PMonArr[i].sv = level;
		gPWR.m_PMonArr[i].uv = under;
		gPWR.m_PMonArr[i].ov = over;
		gPWR.m_PMonArr[i].oc = limit;

		if (gAPI.m_nTestMode != TEST_MODE_HANDLER)
			gDPS.SetVout(i, level, under, over, limit);		
	}

}

float GET_DPS_V(int type, int cnt, bool bCalApply)
{	

	if (type > 19 || type < 0) {
		MESSAGE("DPS channel check.", ERR);
		return -1.0;
	}

	float Volt;
	gDPS.ReadChVout(type, Volt, cnt, bCalApply);

	return Volt;
}

float GET_DPS_A(int type, int cnt, bool bCalApply)
{	
	if (type > 19 || type < 0) {
		MESSAGE("DPS channel check.", ERR);
		return -1.0;
	}	
	float Curr = gDPS.ReadChIout(type, cnt, bCalApply);
	return Curr;
}

void TIME(int nSeq, float delay, std::initializer_list<uint64> values)
{	
	if (nSeq == 0)gPWR.m_PSeq.clear();
	if (nSeq != (int)gPWR.m_PSeq.size()) {
		gERR.addErr(ERR_IO, __FUNCTION__, "TIME Function : Power Seq is Wrong");
	}
	uint64 chbit = 0;
	for (int v : values)
	{
		if (v == DPS_ALL)
		{
			for (int i = VS1; i <= VS16; i++)
			{
				uint64 bit = 1 << i;
				chbit |= bit;
			}
		}else if (v == VREF_ALL)
		{
			for (int i = VREF1; i <= VREF4; i++)
			{
				uint64 bit = 1 << i;
				chbit |= bit;
			}
		}
		else if (v == HV_ALL)
		{
			for (int i = HV1; i <= HV4; i++)
			{
				uint64 bit = 1 << i;
				chbit |= bit;
			}
		}
		else {
			uint64 bit = 1 << v;
			chbit |= bit;
		}
	}
	
	stPowerSeq ps;
	ps.chBit = chbit;
	ps.delay = (delay * 1000000);
	gPWR.m_PSeq.push_back(ps);
	

}

void SRON()
{
	gPWR.setEnableDac(0xff);
	if (gPWR.m_PSeq.size() == 0) // No TIME Set
	{
		gPWR.setEnableDac(VH_ALL);
		gPWR.setEnableDac(VTH_ALL);
		gIO.dps_run(0xffff);
		gPWR.setEnableHv(0xff);
		gPWR.setEnableVref(0xff);
	}

	for (int i = 0; i < (int)gPWR.m_PSeq.size(); i++)
	{
		if (gPWR.m_PSeq[i].chBit == 0) continue;
				
		word dpsOn = gPWR.m_PSeq[i].chBit & 0xFFFF; // DPS Bit
		gIO.dps_run(dpsOn);

		if (gPWR.m_PSeq[i].chBit & 0xF000000) // HV Bit
		{
			byte onff = gPWR.m_PSeq[i].chBit >> 24 ;
			gPWR.setEnableHv(onff);
		}

		if (gPWR.m_PSeq[i].chBit & 0xF0000000) // VRef Bit
		{
			byte onff = gPWR.m_PSeq[i].chBit >> 28;
			gPWR.setEnableVref(onff);
		}

		if (gPWR.m_PSeq[i].chBit & (1ULL << VH_ALL)) // VRef Bit
		{
			gPWR.setEnableDac(VH_ALL);
		}

		if (gPWR.m_PSeq[i].chBit & (1ULL << VTH_ALL)) // VRef Bit
		{
			gPWR.setEnableDac(VTH_ALL);
		}

		usleep(gPWR.m_PSeq[i].delay);
	}

	gNET.m_nSendPower = SEND_PWR_OFF;
}
/*
Brieping
Power On by TIME Function Sequnce
*/

void SROF()
{
	gPWR.setEnableDac(0xff, false);
	if (gPWR.m_PSeq.size() == 0) // No TIME Set
	{
		gPWR.setEnableDac(VH_ALL, false);
		gPWR.setEnableDac(VTH_ALL, false);
		gIO.dps_run(0xffff, false);
		gPWR.setEnableHv(0xff, false);
		gPWR.setEnableVref(0xff, false);
	}

	for(int i = 0; i < (int)gPWR.m_PSeq.size(); i++)
	{
		if (gPWR.m_PSeq[i].chBit == 0) continue;
		word dpsOff = ~(gPWR.m_PSeq[i].chBit & 0xFFFF); // DPS Bit
		gIO.dps_run(dpsOff, false);
		if (gPWR.m_PSeq[i].chBit & 0xF000000) // HV Bit
		{
			byte onff = ~(gPWR.m_PSeq[i].chBit >> 24);
			gPWR.setEnableHv(onff, false);
		}
		if (gPWR.m_PSeq[i].chBit & 0xF0000000) // VRef Bit
		{
			byte onff = ~(gPWR.m_PSeq[i].chBit >> 28);
			gPWR.setEnableVref(onff, false);
		}
		if (gPWR.m_PSeq[i].chBit & (1ULL << VH_ALL)) // VRef Bit
		{
			gPWR.setEnableDac(VH_ALL, false);
		}
		if (gPWR.m_PSeq[i].chBit & (1ULL << VTH_ALL)) // VRef Bit
		{
			gPWR.setEnableDac(VTH_ALL, false);
		}


		usleep(gPWR.m_PSeq[i].delay);
	}

}
/*
Brieping
Power Off by TIME Function Sequnce
*/

/************************************
	PATTERN
************************************/
void SEND_MPAT(const char* file)
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, file);
	gPG.writePattern(file);
	gAPI.debug("	%s out <- ", __FUNCTION__);
}


void MEAS_MPAT(const char* pc, bool Judge, bool Show)
{
//	COMPARE_MODE();
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	gFM.writeScanIndex(0);	
	gOPER.startTest(pc);	

	int waitDelay = gOPER.m_nDefaultMpatDelay;
	if (gAPI.m_bUseHandler) waitDelay = 0;
	
	gOPER.waitTest(waitDelay);
	gOPER.stopTest();

	if (Judge) 
		JUDGE(Show);

	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void START_MPAT(const char* pc)
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	gOPER.startTest(pc);
	gAPI.debug("	%s out <- ", __FUNCTION__);
}


void WAIT_MPAT(uint wait)
{
	gAPI.debug("	%s wait:%d in -> ", __FUNCTION__, wait);
	gOPER.waitTest(gOPER.m_nDefaultMpatDelay, wait);
	gOPER.stopTest();
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void STOP_MPAT()
{
	gOPER.stopTest();
}


// ADR, Fail Data 
// Using NAND Class, to use makefile resource
void MEAS_MPAT_DATA_SC(const char* pc) 
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	COMPARE_MODE();
	bool prevDDR = gBIB.m_bDDR;
	DDR_MODE(true);
	gFM.m_nDutDataProc = DUT_DATA_ENABLE | DUT_DATA_BYSCAN;	
	gBIB.m_nResultMode |= RES_MEAS_DATA_READ;	
	gOPER.startTest(pc, NO_RUN);
	gNAND.testMeasData();

	DDR_MODE(prevDDR);
	gAPI.debug("	%s out <- ", __FUNCTION__);
}
void SET_JNCFLAG(uint val)
{
	gPG.setJnc(val);
}

void SET_REG(uint reg, uint val)
{
	gPG.setReg(reg, val);
}

void SET_TIMER(uint reg, double val)
{
	gPG.setTimer(reg, val);
}


/************************************
	NAND TEST
************************************/
void NAND_BIT_LOCATE(int* bits, int ReadCount)
{
	gNAND.setRelocatBit(bits, ReadCount);
}

void NAND_JUDGE_LIMIT(int col, int page, int block)
{
	gNAND.setLimit(col, page, block);
}


void NAND_ID(const char* pc)							// ID Check
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	NAND_MODE();	
	gNAND.setLimit(0, 0, 0);
	gOPER.startTest(pc);

	gOPER.waitTest(SLEEP_100US);
	gOPER.stopTest();

	gNAND.readID();
	gAPI.debug("	%s out <- ", __FUNCTION__);

}

void NAND_FBLIST_READ_MASK_SC(const char* pc)			// Fail Info Block Read 
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	gBIB.m_nResultMode |= RES_BLOCK_ADR;
	DATA_MODE(DUT_DATA_ENABLE | DUT_DATA_BYSCAN);
	gOPER.startTest(pc, NO_RUN);	
	gNAND.setMask(MASK);
	gNAND.readInfo();
	gAPI.debug("	%s out <- ", __FUNCTION__);

}

void NAND_FBLIST_SC(const char* pc, int Mask)					// Fail Info Block Write
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	NAND_MODE();
	gOPER.startTest(pc, NO_RUN);
	gNAND.setMask(Mask);
	gNAND.writeInfo();
	gAPI.debug("	%s out <- ", __FUNCTION__);
}


void NAND_IMPRINT_SC(const char* pc, int Mask)						// Fail Block Imprint
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	gBIB.m_nResultMode |= (RES_BLOCK_ADR | RES_BLOCK_ADR_IMP);
	DATA_MODE();
	gOPER.startTest(pc, NO_RUN);
	gNAND.setMask(Mask);
	gNAND.writeImprint();
	gAPI.debug("	%s out <- ", __FUNCTION__);
}


void NAND_BLOCK_TEST(const char* pc, int Mode)					// Block Judge 
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	int sub = 0;
	if (Mode == BOTH) sub = RES_BLOCK_ADR_BOTH;
	gBIB.m_nResultMode |= (RES_BLOCK_ADR | sub);
	
	NAND_MODE();
	gOPER.startTest(pc, NO_RUN);	
	gNAND.setMask(TOTAL);
	gNAND.testBlocks();
	
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void NAND_BLOCK_MASK(const char* pc)					// Block Judge 
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	gBIB.m_nResultMode |= RES_BLOCK_ADR;

	NAND_MODE();
	gOPER.startTest(pc, NO_RUN);
	gNAND.setMask(MASK);
	gNAND.testBlocks();

	gAPI.debug("	%s out <- ", __FUNCTION__);
}


void NAND_FCOUNT_SC(const char* pc, int limit)					// Fail Count Each Block
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	gBIB.m_nResultMode |= RES_BLOCK_COUNT;
	NAND_MODE();
	gNAND.setLimit(0, 0, 0);
	gOPER.startTest(pc, NO_RUN);
	gNAND.setMask(TOTAL);
	gNAND.m_nFCountLimit = limit;
	gNAND.testFailCount();
	gAPI.debug("	%s out <- ", __FUNCTION__);
}


void NAND_FCOUNT_DETAIL_SC(const char* pc, int block, int columnLimit , int pageLimit, bool bAllData )
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	gBIB.m_nResultMode |= RES_BLOCK_COUNT_DETAIL;
	NAND_MODE();
	gNAND.m_bFCDAllData = bAllData;
	gNAND.m_nFCDBlkAdr = block;
	gNAND.m_nFCDColLimit = columnLimit;
	gNAND.m_nFCDPageLimit = pageLimit;
	gNAND.setLimit(255, 0, 0);
	gOPER.startTest(pc, NO_RUN);
	gNAND.testFailCountDetail();
	gNAND.setLimit(0, 0, 0); 
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void NAND_DATA_READ_SC(const char* pc)
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	gBIB.m_nResultMode |= RES_DATA_READ;	
	DATA_MODE(DUT_DATA_ENABLE | DUT_DATA_BYSCAN);	
	gOPER.startTest(pc, NO_RUN);
	gNAND.testData();
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void NAND_CYCLE_TEST_SC(const char* pc, double rate)
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);
	gBIB.m_nResultMode |= RES_BLOCK_CYCLE;
	NAND_MODE();
	gNAND.setLimit(0, 0, 0);
	gOPER.startTest(pc, NO_RUN);
	gNAND.m_fBaseRate = rate;
	gNAND.cycleTest();
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

int  GET_FBLOCK_SIZE(int no, int type)
{
	dut* pDut = gBIB.getDut(no);
	if (pDut == NULL) return 0;
	int ret = pDut->m_totalFBlkCnt;
	if (type == MASK) ret = pDut->m_maskFBlkCnt;
	else if (type == CUR) ret = pDut->m_curFBlkCnt;
	else if (type == TOTAL) ret = pDut->m_totalFBlkCnt;
	return ret;
}

bool GET_FBLOCK(int no, int block, int type)
{
	dut* pDut = gBIB.getDut(no);
	if (pDut == NULL) return false;
	if (block >= MAX_BLOCK_ADR) return false;
	bool ret = false;		
	if (type == MASK) ret = (pDut->m_maskFBlks[block / 32] & (1 << (block % 32)));
	else if (type == CUR) ret = (pDut->m_curFBlks[block / 32] & (1 << (block % 32)));
	else if (type == TOTAL) ret = (pDut->m_totalFBlks[block / 32] & (1 << (block % 32)));
	return ret;
}

bool SET_FBLOCK(int no, int block, int type)
{
	dut* pDut = gBIB.getDut(no);
	if (pDut == NULL) return false;
	if (type == MASK) {
		if (!(pDut->m_maskFBlks[block / 32] & (1 << (block % 32)))) pDut->m_maskFBlkCnt++;
		pDut->m_maskFBlks[block / 32] |= (1 << (block % 32));
		
	}
	else if (type == CUR) {
		if (!(pDut->m_curFBlks[block / 32] & (1 << (block % 32)))) pDut->m_curFBlkCnt++;
		pDut->m_curFBlks[block / 32] |= (1 << (block % 32));
	}
	else if (type == TOTAL) {
		if (!(pDut->m_totalFBlks[block / 32] & (1 << (block % 32)))) pDut->m_totalFBlkCnt++;
		pDut->m_totalFBlks[block / 32] |= (1 << (block % 32));
	}

}

int GET_FPAGE_COUNT(int dno, int page)
{
	if (dno > 1546 || page > 4096) return 0;
	int ret = 0;
	for (int i = 0; i < 8; i++)
		ret += gNAND.m_nPageSumArr[dno][page][i];
	return ret;
}

int GET_FCOLUMN_COUNT(int dno, int page, int col)
{
	if (dno > 1546 || page > 4096 || col > 7) return 0;

	return gNAND.m_nPageSumArr[dno][page][col];
	
}

void ENABLE_RB(bool enable)
{
	uint value = 0;
	if (enable) value = 1;	
	gIO.writePG(PG_SYSTEM | 7, value, 1);
}

uint GET_RB_COUNT(int pin)
{
	if (pin > 15) return 0;

	int baseAdr = (pin >= 8) ? 332 : 271;
	int offset = (pin % 8) * 3;

	return gIO.readPG(PG_SYSTEM | (baseAdr + offset), 3);
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
	int fmover = 1;
	if (FM_Capture_Always) conditionCapture = 0;
	gFM.setFmMode(FM_COMP, conditionCapture);
}


void DDR_MODE(bool bDDR)
{
	gBIB.m_bDDR = bDDR;
}

void FM_OVER_STOP(bool bEnable)
{
	gFM.setFmOverStop(bEnable);
}

void FM_DRV_MODE(int mode)
{
	gIO.writePG((1 << 9) | 21, mode, 1);
}


void WRITE_SCAN(int scanIndex)
{
	gFM.writeScanIndex(scanIndex);
}


int READ_SCAN()
{
	return gFM.readScanIndex();
}

void READ_DUTDATA()
{
	gNAND.readData(gFM.readScanIndex());
	
}

byte* GET_DUTDATA(int dut)
{
	return gBIB.getDutpData(dut);
}

int GET_DUTDATA_COUNT()
{
	return gFM.m_nDutDataSize;
}

int READ_FMDATA(int read)
{
	return gFM.readFmData(read);
}

FM* GET_FMDATA_H(int pos)
{
	return (FM*)gFM.getFmDataH(pos);
}

FM* GET_FMDATA_L(int pos)
{
	return (FM*)gFM.getFmDataL(pos);
}

void SHOW_FMDATA(int offset, int readSZ, int Mode)
{
	gFM.showFmData(offset, readSZ, Mode);
}

void SHOW_FMDRV(int offset, int readSZ)
{
	gFM.showFmDrv(offset, readSZ);
}

/************************************
	VECTOR MEMORY
************************************/
bool SET_VECTOR(const char* name, int& idxCnt, int Mode)
{
	bool bRet = gVEC.loadFile(name, Mode);
	if (!bRet) return true;
		
	if (Mode == BINARY)
		gVEC.setBinary(idxCnt);
	else 
		gVEC.setText(idxCnt);

	return gVEC.m_bDone;
}

int SET_VECTOR(int* pData, int& idxCnt, int ofs)
{
	return gVEC.setBuf(ofs, idxCnt, pData);
	
}

bool NEXT_VECTOR(int& idxCnt)
{
	if (gVEC.m_bDone)return gVEC.m_bDone;

	if (gVEC.m_nMode == BINARY)
		gVEC.setBinary(idxCnt);
	else
		gVEC.setText(idxCnt);

	return gVEC.m_bDone;
}

/************************************
	OPER RATION
************************************/
void DELAY(double time)
{
	sleep(time);
}

void MESSAGE(const char* msg, int type)
{
	gNET.send_msg(type, msg);
}

void RUN_TEMP(double temp, int Mode, int timeLimit)
{
	// Setting Default State of PLC On  if State of PLC Off is Checked reqTemp
	gAPI.debug("	%s in -> ", __FUNCTION__);
	gOPER.m_bSetTemp = true;
	gOPER.m_fSetTemp = temp;
	gNET.sendTemp(1);
	if (Mode == WAIT)
		gOPER.waitTemp(timeLimit);
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

double READ_TEMP(int Mode)
{	

	double ret = gOPER.m_fReadTemp;
	if (Mode == SV)
		ret = gOPER.m_fSetTemp;		

	return ret;
	return 0;
}



void SOAK_TIME(int time)
{
	gAPI.debug("	%s in -> ", __FUNCTION__);
	gNET.sendAging(2, time);
	gNET.m_nSendPower = SEND_PWR_ZERO;
	DELAY(time);
	gNET.m_nSendPower = SEND_PWR_OFF;
	gAPI.debug("	%s out <- ", __FUNCTION__);
}


void AGING(const char* pc, int time)
{
	gAPI.debug("	%s in -> ", __FUNCTION__);
	gNET.sendAging(1, time);
	
	gOPER.startTest(pc);
	gOPER.waitTest(SLEEP_100US, time);
	gOPER.stopTest();
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void NEXT_SYNC()
{
	//gPG.nextSync();
}


/************************************
	UTIL function
************************************/
#include <iostream>
#include <chrono>
#include <cmath>
std::chrono::system_clock::time_point chrono_start;
std::chrono::system_clock::time_point chrono_end;
long long MEASURE_TIME(int mode, bool bPrint)
{
	std::chrono::nanoseconds nano;
	if (mode == START)
	{
		chrono_start = std::chrono::system_clock::now();
	}
	else if (mode == END)
	{
		chrono_end = std::chrono::system_clock::now();
		nano = chrono_end - chrono_start;		
		if (bPrint)
		{
			printf("MEAS TIME : %ld ms, %ld us, %ld ns\n", 
				nano.count() / (1000*1000), nano.count()/1000, nano.count() % 1000);
		}
	}
	return nano.count();
}

/************************************
	inner function
************************************/

void start_step(const char* name) 
{ 
	if (_bAddStepMap) {
		bool bAdd = true;
		for (int i = 0; i < gAPI.m_nStepIdx; i++)
		{
			if (strcmp(gAPI.m_sStepName[i], name) == 0)
				bAdd = false;
		}
		if (bAdd)
			sprintf(gAPI.m_sStepName[gAPI.m_nStepIdx++], "%s", name);
		return;
	}

	gAPI.debug("%s %s in -> ", __FUNCTION__, name);
//	printf("---- %s %s - Start ----\n", __FUNCTION__, name);	
	if (!gIO.checkPG(gAPI.m_nPGID)) {
		gERR.addErr(ERR_IO, __FUNCTION__, "Can't Communicate FPGA. Check Firmware");
	}
	//gFM.m_nDutDataProc = DUT_DATA_NONE | DUT_DATA_BYSCAN;
	//gFM.setReadDut();

	sprintf(gAPI.m_sCurStep, "%s", name);

	//gPG.m_nWritePC = 0;    // PT PC Count
	gBIB.initStepItem();	
	gRES.startStepRes();

	
	sprintf(gNET.m_pShmDef->sCurStep, "%s", gRES.m_sResCurStep);

	gNET.sendStartStep();
	gAPI.debug("%s %s out <- ", __FUNCTION__, name);
//	printf("---- %s %s - End ----\n", __FUNCTION__, name);

}
void end_step() 
{ 
	if (_bAddStepMap) return;
	gAPI.debug("%s in -> ", __FUNCTION__);

	gRES.endStepRes();
	gNET.sendEndStep();

	gRES.saveResOption();
	gNET.sendResOption();

	gAPI.debug("%s out <- ", __FUNCTION__); 
}

void setDefaultMpatDelay(int delay)
{
	gOPER.m_nDefaultMpatDelay = delay;
}

void writeIO(uint adr, uint value)
{
	gIO.writeIO(adr, value); 
}
uint readIO(uint adr)
{
	gIO.readIO(adr); 
}
void writePG(uint adr, uint val, int sz)
{
	gIO.writePG(adr, val, sz);
}
uint readPG(uint adr, int sz)
{
	gIO.readPG(adr, sz);
}
void writePGData(uint adr, byte* pData, int sz)
{
	gIO.writePGData(adr, pData, sz);
}
void readPGData(uint adr, byte* pData, int sz)
{
	gIO.readPGData(adr, pData, sz);
}

void writeFPGA(const char* file, bool bBibFPGA)
{
	int ret = RTN_OK;

	if (!bBibFPGA)	// System Board PG Write
		ret = gIO.cfg_fpga(file);
	else			// BIB PG Write
		ret = gIO.cfgi_fpga(file);
	
	if (ret != RTN_OK ) {
		gERR.addErr(ERR_IO, __FUNCTION__, "FPGA Upload Fail!");
		gNET.send_msg(MSG_TYPE_ERR, "FPGA Upload Fail!");
	}
}

void showIO(bool bShow)
{
	gIO.m_bDebug = bShow;
}

void showDrv(int ofs, int size)
{
	gFM.showFmDrv(ofs, size);
}

void showData(int ofs, int size)
{
	gFM.showFmData(ofs, size);
}

void writeCpeDelay(int stb0, int stb1)
{
	gTG.writeCpeDelay(stb0, stb1);
}

void writeDskDelay(int delay)
{
	gTG.writeDskDelay(delay);
}

void testFMcode()
{
	byte fmArr[FM_MEM_SZ];
	for (int i = 0; i < 32; i++)
	{
		fmArr[i] = i & 0xff;
	}

	uint adr = 4 << 17;


	gFM.readFmData(1);
	gFM.showFmDrv(0, 1);
	gFM.showFmData(0, 1);


	for (int i = 0; i < FM_MEM_SZ; i++)
	{
		printf("write %d -> 0x%x\n", i, fmArr[i]);
	}


	gIO.writePGData(adr, fmArr, 16);
	gIO.readPGData(adr, fmArr, 16);

	gFM.readFmData(1);
	gFM.showFmDrv(0, 1);
	gFM.showFmData(0, 1);


	for (int i = 0; i < FM_MEM_SZ; i++)
	{
		printf("read %d -> 0x%x\n", i, fmArr[i]);
	}
}

int main(int argc, char* argv[])
{	
	printf("DATE : %s VER : %s\n", VER_DATE, API_VER);
	gAPI.loadStarLib();
	gAPI.doArgument(argc, argv);
	gIO.open_pcie();
	if (gAPI.m_nTestMode == TEST_MODE_CMD)
	{
		gCMD.cmd();
		return 1;
	}

	gNET.shm_init();
	gAPI.loadConfig();



	if (gIO.m_nBFPGA != FPGA_DEFAULT)  
		if (!gIO.checkPGInterface())  // system PG by using Interface
			writeFPGA("/star/test/pg_mid.bit"); 

	if (!gIO.checkPG(gAPI.m_nPGID))
	{
		gNET.send_msg(MSG_TYPE_ERR, "PG_FPGA Is Not Loaded! Process PG Upload.");
		
		if (gIO.m_nBFPGA == FPGA_DEFAULT)
			writeFPGA("/star/test/pg_top.bit");
		else {
			cstring bitFile = "/star/test/pg_bib_dram.bit";
			if (gIO.m_nBFPGA == FPGA_BIB_MCP) bitFile = "/star/test/pg_bib_mcp.bit";
			writeFPGA(bitFile, true);
		}
	}
	

	if ((gAPI.m_nTestMode != TEST_MODE_HANDLER)) {		
		
		gIO.adc_settling(1000000);
		
	}

	
	if (!gAPI.m_bNoDPS && (gAPI.m_nTestMode != TEST_MODE_HANDLER))
	{
		printf("========= DPS Init ==========\n");
		gDPS.AllChannelInit();
		printf("========= DPS Init END ==========\n");
		
	}
	else {
		gDPS.I2cSendByteAll(I2C_CMD_CLEAR_FAULTS);
	}

	
	
	if (gAPI.m_nTestMode == TEST_MODE_DIAG) { // DIAG MODE
		diag_log_open();
		TEST();		
		diag_log_close();
		
		return 1;
	}

	_bAddStepMap = true;  //  Make Function Map
	CONTACT();
	TEST();
	_bAddStepMap = false;

	gNET.sendStartTest();
	SETUP();

	//testFMcode();

	gAPI.debug("%s in -> ", __FUNCTION__);

	if (gAPI.m_nTestMode == TEST_MODE_CONTACT) {	// CONTACT
		gRES.initResult();
		gNET.sendStepList();
		CONTACT();
		gRES.initCatFile();
	}
	else if (gAPI.m_nTestMode == TEST_MODE_RUN) {	// TEST
		gRES.loadResult(); 				
		TEST(); 
		gRES.binResult();
		gNET.sendBin();
	}
	else if (gAPI.m_nTestMode == TEST_MODE_INIT) {	// TEST
		// only DPS init		
		gAPI.m_nTestMode = TEST_MODE_CONTACT;
		gRES.initResult();
		gNET.sendStepList();
		MESSAGE("H/W INIT Done", INFO);
		gRES.initCatFile();
	}
	else if (gAPI.m_nTestMode == TEST_MODE_HANDLER) {	// TEST
		gRES.initResult();
		gNET.sendStepList();

		gAPI.m_bUseHandler = true;
		gAPI.m_nTestMode = TEST_MODE_CONTACT;
		CONTACT();
		gAPI.m_nTestMode = TEST_MODE_RUN;
		TEST();
		gRES.binResult();
		gNET.sendBin();
	}
	else if (gAPI.m_nTestMode == TEST_MODE_SINGLE) {	// TEST
		gRES.loadResult();		
		gRES.loadCatFile();
		for (int i = 0; i<(int)gAPI.m_singleStepVec.size(); i++ )
			gAPI.singleStep(gAPI.m_singleStepVec[i].c_str());
		gRES.binResult();
		gNET.sendBin();

	}


	if (gOPER.m_bSetTemp)
		if (gAPI.m_nTestMode != TEST_MODE_CONTACT)
			gNET.sendTemp(2);
	
	gRES.freeResult();
	gRES.freeSubResult();

	// PE Chip & Channel OFF
	gNET.sendEndTest();	


	usleep(1000*1000); // Wait Message Send

	gAPI.debug("%s out <- ", __FUNCTION__);

	return 1;
}


/************************************************************

#define VER_DATE "2025/12/30"
#define API_VER "1.0.0" by bin.yun
   Make First based on ST805 StarAPI



#define VER_DATE "2025/09/03"
#define API_VER "1.4.5" by bin.yun
     Alert Error시  Status Read할수있도록.


* ***********************************************************/
