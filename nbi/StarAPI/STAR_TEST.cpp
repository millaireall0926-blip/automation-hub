#include <time.h>


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
#include "eeprom.h"
#include "bitcharts.h"
#include "dps.h"
#include "DIAG_TEST.h"
#include "vec.h"
#include "nand.h"
#include "cmd.h"

#include <stdarg.h>


#define MIN_VL -1.0
#define MAX_VL  6.5

#define DR_SEL 0x2A80AA
#define IO_SEL 0x154055
#define SCAN_SEL 0x1D00
#define CLK_SEL 0x2200

ioc			g_clIO;				//	gIO
starAPI		g_clAPI;			//	gAPI
starlib		g_clLib;			//  gLIB
net			g_clNet;			//	gNET
errc		g_clErr;			//	gERR
bib			g_clBib;			//  gBIB
tgc			g_clTgc;			//	gTG
pgc			g_clPgc;			//	gPG
pds			g_clPds;			//  gPDS  
scramble	g_clScr;			//  gSCR
result		g_clRes;			//	gRES
failmem		g_clFm;				//	gFM
operation	g_clOperation;		//  gOPER
eeprom		g_clRom;			//  gROM
dps			g_clDPS;			// gDPS
vec			g_clVec;
nand		g_clNand;
command		g_clCmd;



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

void IO_MASK(uint mask0_31, uint mask32_63)
{
	gPG.writeIOMask(mask0_31, mask32_63);
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

void VIH(int type, double value, bool bCalApply)
{
	gAPI.debug("	%s type:%d in -> ", __FUNCTION__, type);
	uint32_t dev_sel = 0x3FFFFF;
	uint8_t ch_sel = 0xFF; // all channels
	
	if(type == DR) 
		dev_sel = DR_SEL;
	else if(type == IO)
		dev_sel = IO_SEL;
	else if(type == SCAN)
		dev_sel = SCAN_SEL;
	else if(type == CLK)
		dev_sel = CLK_SEL;
	else if(type == ALL)
		dev_sel = 0x3FFFFF;
	else
		return;

	gOPER.setPmonPE(PMON_VIH, type, value);

	if(!bCalApply)
		gIO.pe_set_dvh(dev_sel, ch_sel, value);
	else {
		double setValue;
		int realCh = 0;
		for(int nDev = 0; nDev < 22; nDev++) {
			if(dev_sel & (0x1 <<nDev)) {
				for(int ch = 0; ch < 8; ch++) {
					realCh = nDev * 8 + ch;
					//setValue = gOPER.m_VIHCal[realCh].gain * value + gOPER.m_VIHCal[realCh].offset;
					setValue = (value - gOPER.m_VIHCal[realCh].offset) / gOPER.m_VIHCal[realCh].gain;
					gIO.pe_set_dvh((0x1 <<nDev), (0x1<<ch), setValue, true);
				}
			}
		}
	}
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void VIH_CLK(int nCLK, double value, bool bCalApply)
{
	uint32_t dev_sel = CLK_SEL;
	uint8_t ch_sel = (0x1<<nCLK);

	unsigned int nDev[2] = {0x0200, 0x2000};
	unsigned int chBlock[2][8] = {{5,3,6,7,0,4,1,2}, {5,7,6,4,3,1,0,2}};
	
	gOPER.setPmonPE(PMON_VIH, CLK, value);

	double setValue;
	for(int nBlock = 0 ; nBlock < 2; nBlock++) {
		dev_sel = nDev[nBlock];
		ch_sel = (0x1 << chBlock[nBlock][nCLK]);
		if(!bCalApply)
			gIO.pe_set_dvh(dev_sel, ch_sel, value);
		else {
			int realCh = 0;
			for(int nDev = 0; nDev < 22; nDev++) {
				if(dev_sel & (0x1 <<nDev)) {
					realCh = nDev * 8 + chBlock[nBlock][nCLK];
					//setValue = gOPER.m_VIHCal[realCh].gain * value + gOPER.m_VIHCal[realCh].offset;
					setValue = (value - gOPER.m_VIHCal[realCh].offset) / gOPER.m_VIHCal[realCh].gain;
					gIO.pe_set_dvh((0x1 <<nDev), ch_sel, setValue, true);
				}
			}
		}
	}
}

void VIL(int type, double value, bool bCalApply)
{
	gAPI.debug("	%s type:%d in -> ", __FUNCTION__, type);
	uint32_t dev_sel = 0x3FFFFF;
	uint8_t ch_sel = 0xFF; // all channels

	if(type == DR) 
		dev_sel = 0x2A80AA;
	else if(type == IO)
		dev_sel = 0x154055;
	else if(type == SCAN)
		dev_sel = 0x001D00;
	else if(type == CLK)
		dev_sel = 0x002200;
	else if (type == ALL)
		dev_sel = 0x3FFFFF;
	else 
		return;

	double setValue;
	gOPER.setPmonPE(PMON_VIL, type, value);

	if(!bCalApply)
		gIO.pe_set_dvl(dev_sel, ch_sel, value);
	else {
		int realCh = 0;
		for(int nDev = 0; nDev < 22; nDev++) {
			if(dev_sel & (0x1 <<nDev)) {
				for(int ch = 0; ch < 8; ch++) {
					realCh = nDev * 8 + ch;
					//setValue = gOPER.m_VILCal[realCh].gain * value + gOPER.m_VILCal[realCh].offset;
					setValue = (value - gOPER.m_VILCal[realCh].offset) / gOPER.m_VILCal[realCh].gain;
					//if(realCh == 1 ) {
					//	printf("VIL Ch %d Set Value : %0.5f, %0.5f ( %0.5f, %0.5f ) \n", realCh, value, setValue, gOPER.m_VILCal[realCh].gain, gOPER.m_VILCal[realCh].offset);
					//}
					gIO.pe_set_dvl((0x1 <<nDev), (0x1<<ch), setValue, true);
				}
			}
		}
	}
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void VIL_CLK(int nCLK, double value, bool bCalApply)
{
	uint32_t dev_sel = CLK_SEL;
	uint8_t ch_sel = (0x1<<nCLK);

	double setValue;
	gOPER.setPmonPE(PMON_VIL, CLK, value);

	if(!bCalApply)
		gIO.pe_set_dvl(dev_sel, ch_sel, value);
	else {
		int realCh = 0;
		for(int nDev = 0; nDev < 22; nDev++) {
			if(dev_sel & (0x1 <<nDev)) {
				realCh = nDev * 8 + nCLK;
				setValue = (value - gOPER.m_VILCal[realCh].offset) / gOPER.m_VILCal[realCh].gain;
				gIO.pe_set_dvl((0x1 <<nDev), (0x1<<nCLK), setValue, true);
			}
		}
	}
}

void IMP(int type, double value)
{
	gAPI.debug("	%s type %d In -> ", __FUNCTION__, type);
	uint32_t dev_sel = 0x3FFFFF;
	uint8_t ch_sel = 0xFF; // all channels

	if (type == DR)
		dev_sel = 0x2A80AA;
	else if (type == IO)
		dev_sel = 0x154055;
	else if (type == SCAN)
		dev_sel = 0x001D00;
	else if (type == CLK)
		dev_sel = 0x002200;
	else if(type == ALL)
		dev_sel = 0x3FFFFF;
	else
		return;

	gIO.pe_set_imp(dev_sel, ch_sel, value);

	gAPI.debug("	%s out <- ", __FUNCTION__ );
}

void VBB(int type, uint16_t data)
{
	gAPI.debug("	%s type %d In -> ", __FUNCTION__, type);
	uint32_t dev_sel = 0x3FFFFF;
	uint8_t ch_sel = 0xFF; // all channels

	if (type == DR)
		dev_sel = 0x2A80AA;
	else if (type == IO)
		dev_sel = 0x154055;
	else if (type == SCAN)
		dev_sel = 0x001D00;
	else if (type == CLK)
		dev_sel = 0x002200;
	else if(type == ALL)
		dev_sel = 0x3FFFFF;
	else
		return;

	gIO.pe_set_vbb(dev_sel, ch_sel, data);

	gAPI.debug("	%s out <- ", __FUNCTION__ );
}

void SLR(int type, uint16_t data)
{
	gAPI.debug("	%s type %d In -> ", __FUNCTION__, type);
	uint32_t dev_sel = 0x3FFFFF;
	uint8_t ch_sel = 0xFF; // all channels

	if (type == DR)
		dev_sel = 0x2A80AA;
	else if (type == IO)
		dev_sel = 0x154055;
	else if (type == SCAN)
		dev_sel = 0x001D00;
	else if (type == CLK)
		dev_sel = 0x002200;
	else if(type == ALL)
		dev_sel = 0x3FFFFF;
	else
		return;

	gIO.pe_set_slr(dev_sel, ch_sel, data);

	gAPI.debug("	%s out <- ", __FUNCTION__ );
}

void VTH(double value)
{
	gAPI.debug("	%s in -> ", __FUNCTION__);
	uint32_t dev_sel = 0x154055;
	uint8_t ch_sel = 0xFF; // all channels

	gIO.pe_set_vt(dev_sel, ch_sel, value);
	gOPER.setPmonPE(PMON_VTH, ALL, value);
	
	gAPI.debug("	%s out <- ", __FUNCTION__);

	//if (value <= MIN_VL) { //min_vl : -4.0V
	//	value = MIN_VL;
	//	//code = 0x0;
	//}
	//else if (value >= MAX_VL) { //min_vl = 10V
	//	value = MAX_VL;
	//	//code = 0xFFFF;
	//}
	//else {
	//	code = (int)((value + 4.0) * 1000000.0 / 213.6) & 0xFFFF;
	//}

	//gIO.pe_write(dev_sel, ch_sel, PE_CH_COMP_TH, code);
	//gIO.pe_write(dev_sel, ch_sel, PE_CH_POWER_ON_CTRL, 0x0B1F);
}

void SET_PE_CAL(int type, int ch, float gain, float offset )
{
	//gIO.pe_set_cal_data(type, ch, gain, offset );
	gOPER.pe_set_cal_data(type, ch, gain, offset );
}

void TIME(int seq, int Select, float value)
{
	gOPER.dpsPowerSeq(seq, Select, value);
}

void DPS(int type, double level, double under, double over, double limit, bool bCalApply)
{
	gAPI.debug("	%s type %d in -> ", __FUNCTION__, type);
	int dpsCh = 0;
	if(type < 0x100) { /// 1, 2, 3, 4, 5
		int ch[5] = {0, 1, 4, 2, 3};
		if((type & 0xFF) < 1 || (type & 0xFF) > 5) {
			MESSAGE("DPS channel check.", ERR);
			return;
		}
		dpsCh = ch[type-1];
	}else {/// VS1, VS2, VS3, VS4, VS5
		int count = 0;
		for(int idx = 0; idx < 5; idx++) {
			if((type & (0x100<<idx)) != 0) {
				count++;
				dpsCh = idx;
			}
		}
		if(count != 1) {
			MESSAGE("DPS channel check.", ERR);
			return;
		}
		
	}

	if(level < under) MESSAGE("DPS set and under level check.", ERR);
	if(level > over) MESSAGE("DPS Set and over level check.", ERR);
	if(under >= over) MESSAGE("DPS Set, over and under level check.", ERR);
	// gIO.dps_write();
	gOPER.m_PMonArr[dpsCh].sv = level;
	gOPER.m_PMonArr[dpsCh].uv = under;
	gOPER.m_PMonArr[dpsCh].ov = over;
	gOPER.m_PMonArr[dpsCh].oc = limit;

	if(dpsCh == 4) {
		if (level > 15.0) {
			MESSAGE("VS3 Out Voltage MAX is 15.0\n", ERR);
			return;
	}
		if(bCalApply) level = (level - gOPER.m_DPSVSCal[dpsCh].offset) / gOPER.m_DPSVSCal[dpsCh].gain;
		gDPS.SetHVout( level);
		return;
	}else {
		if(bCalApply) level = (level - gOPER.m_DPSVSCal[dpsCh].offset) / gOPER.m_DPSVSCal[dpsCh].gain + gOPER.m_DPSVSCal[dpsCh].offset2;
		gDPS.SetVout(dpsCh, level, under, over, limit);
	}

	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void SRON()
{
	gAPI.debug("	%s in -> ", __FUNCTION__);
	// PE on
	//uint32_t dev_sel = 0x3FFFFF;
	//uint8_t ch_sel = 0xFF; // all channels
	uint8_t dps_sel = 0xF;
	

	//gIO.dps_run(dps_sel, 1);
	//Sleep(0.005);
	//gIO.pe_on(dev_sel, ch_sel);

	////// Swing LEVEL 에 따른 TR/TF 설정 ////////////
	if((gOPER.m_PMonArr[PMON_VIH].dr - gOPER.m_PMonArr[PMON_VIL].dr) > 0.8) //1.0) 
		gIO.pe_write(DR_SEL, 0xFF, PE_CH_DRV_SLEW , 0x1F1F);
	else
		gIO.pe_write(DR_SEL, 0xFF, PE_CH_DRV_SLEW , 0x0707);

	if((gOPER.m_PMonArr[PMON_VIH].io - gOPER.m_PMonArr[PMON_VIL].io) > 0.8) // 1.0) 
		gIO.pe_write(IO_SEL, 0xFF, PE_CH_DRV_SLEW , 0x1F1F);
	else
		gIO.pe_write(IO_SEL, 0xFF, PE_CH_DRV_SLEW , 0x0707);

	if((gOPER.m_PMonArr[PMON_VIH].sc - gOPER.m_PMonArr[PMON_VIL].sc) > 0.8) // 1.0) 
		gIO.pe_write(SCAN_SEL, 0xFF, PE_CH_DRV_SLEW , 0x1F1F);
	else
		gIO.pe_write(SCAN_SEL, 0xFF, PE_CH_DRV_SLEW , 0x0707);

	if((gOPER.m_PMonArr[PMON_VIH].ck - gOPER.m_PMonArr[PMON_VIL].ck) > 0.8) // 1.0) 
		gIO.pe_write(CLK_SEL, 0xFF, PE_CH_DRV_SLEW , 0x1F1F);
	else
		gIO.pe_write(CLK_SEL, 0xFF, PE_CH_DRV_SLEW , 0x0707);
	////// Swing LEVEL 에 따른 TR/TF 설정  END  //////


	////// VIH Level에 따른 DPS VCC 설정 ////////////
	float maxVIH = gOPER.getMaxPELevel();
	double dpsvcc = 5.0;
	if( maxVIH >= 4.0) {
		dpsvcc = maxVIH + 1.1;
		dpsvcc = (dpsvcc - gOPER.m_peDpsVccCal.offset) / gOPER.m_peDpsVccCal.gain;
	}else {
		dpsvcc = 5;
		dpsvcc = (dpsvcc - gOPER.m_peDpsVccCal.offset) / gOPER.m_peDpsVccCal.gain;
	}
	/// 0 V Set & Clamp On
	gIO.set_hv_dac(0, 2.5); // VIL CLAMP 0.0V
	gIO.set_hv_dac(1, 0.0); // VIH CLAMP 0.0V	
	gIO.dac_clamp(true);
	
	gIO.set_hv_dac(7, dpsvcc);
	////// VIH Level에 따른 DPS VCC 설정 END ////////////

	////// PE High CLAMP Level Setting /////////////////
	gIO.set_hv_dac(1, maxVIH*1.15);

	////// PE Low CLAMP Level Setting /////////////////
	float minVIL = gOPER.getMinPELevel();
	if(minVIL <= 0.5) minVIL -= 0.2;
	else minVIL*=0.85;
	minVIL += 2.5;
	gIO.set_hv_dac(0, minVIL);

	////// Power On Sequence /////////////////////////
	if(gOPER.m_PSeq[0].chBit == 0) {
		gIO.dps_run(dps_sel, 1);
		gDPS.SetHVout();
		usleep(10 * 1000);
		gIO.dps_vs3f(1);
		gIO.pe_chip_on();
		gOPER.m_flgDpsOn = dps_sel;
	} else {
		dps_sel = 0;
		for(unsigned int idx = 0; idx < gOPER.m_PSeq.size(); idx++ ) // seq
		{
			if(gOPER.m_PSeq[idx].chBit == 0) continue;
			//printf("On Seq(%d) Sel(0x%X) Delay(%f)\n", idx, gOPER.m_PSeq[idx].chBit, gOPER.m_PSeq[idx].delay);
			//for(int i = 0 ; i < 4; i++) {
			for(int i = 0 ; i < 5; i++) {
				if(gOPER.m_PSeq[idx].chBit & (0x100 << i)) { // channel
					dps_sel |= (0x1 << i);
				}
			}
			gIO.dps_run((dps_sel & 0xf), 1);	
			if(dps_sel >> 4  & 0x1){
				gDPS.SetHVout();
				usleep(10 * 1000);
				gIO.dps_vs3f(1);
			}
			if(gOPER.m_PSeq[idx].chBit & PE) { // PE On
				gIO.pe_chip_on();
			}
			usleep(gOPER.m_PSeq[idx].delay);
		}
		gOPER.m_flgDpsOn |= dps_sel;
	}

	
	//gIO.pe_chip_on();
	// gIO.pe_write(dev_sel, ch_sel, PE_CH_POWER_ON_CTRL, 0x0B1F);
	// gIO.pe_write(dev_sel, ch_sel, PE_C_PDO_RESET, 0x2201);
	gNET.m_nSendPower = SEND_PWR_READ;
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void SROF()
{
	gAPI.debug("	%s in -> ", __FUNCTION__);
	// PE off
	//uint32_t dev_sel = 0x3FFFFF;
	//uint8_t ch_sel = 0xFF; // all channels
	uint8_t dps_sel = 0xF;
	//gIO.pe_off(dev_sel, ch_sel);

	if(gOPER.m_PSeq[0].chBit == 0) {
		gIO.pe_chip_off();
		gDPS.SetHVoutZero();
		usleep(5000);
		gIO.dps_vs3f(0);
		gIO.dps_run(dps_sel, 0);
	} else {
		dps_sel = 0;
		for(int idx = gOPER.m_PSeq.size() - 1; idx > -1; idx-- ) // seq
		{
			if(gOPER.m_PSeq[idx].chBit == 0) continue;
			//for(int i = 0 ; i < 4; i++) {
			for(int i = 0 ; i < 5; i++) {
				if(gOPER.m_PSeq[idx].chBit & (0x100 << i)) { // channel
					dps_sel |= (0x1 << i);
				}
			}
			if((dps_sel >> 4) & 0x1) {
				gDPS.SetHVoutZero();
				usleep(5000);
				gIO.dps_vs3f(0);
				dps_sel &= 0xF;
			}
			gIO.dps_run(dps_sel, 0);	
			if(gOPER.m_PSeq[idx].chBit & PE) { // PE On
				gIO.pe_chip_off();
			}
			usleep(gOPER.m_PSeq[idx].delay);
		}
	}
	gOPER.m_flgDpsOn = 0;

	gIO.set_hv_dac(0, 2.5); // VIL CLAMP 0.0V
	gIO.set_hv_dac(1, 0.0); // VIH CLAMP 0.0V	
	//gIO.dac_clamp(false);

	//gIO.pe_chip_off();
	//Sleep(0.005);
	//gIO.dps_run(dps_sel, 0);
	// gIO.pe_write(dev_sel, ch_sel, PE_CH_POWER_ON_CTRL, 0x0B1);
	// gIO.pe_write(dev_sel, ch_sel, PE_C_PDO_RESET, 0x2200);
	gNET.m_nSendPower = SEND_PWR_OFF;
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void DPSON()
{
	gAPI.debug("	%s in -> ", __FUNCTION__);
	// PE on
	//uint32_t dev_sel = 0x3FFFFF;
	//uint8_t ch_sel = 0xFF; // all channels
	uint8_t dps_sel = 0xF;
	

	//gIO.dps_run(dps_sel, 1);
	//Sleep(0.005);
	//gIO.pe_on(dev_sel, ch_sel);

	////// Swing LEVEL 에 따른 TR/TF 설정 ////////////
	if((gOPER.m_PMonArr[PMON_VIH].dr - gOPER.m_PMonArr[PMON_VIL].dr) > 1.0) 
		gIO.pe_write(DR_SEL, 0xFF, PE_CH_DRV_SLEW , 0x1F1F);
	else
		gIO.pe_write(DR_SEL, 0xFF, PE_CH_DRV_SLEW , 0x0707);

	if((gOPER.m_PMonArr[PMON_VIH].io - gOPER.m_PMonArr[PMON_VIL].io) > 1.0) 
		gIO.pe_write(IO_SEL, 0xFF, PE_CH_DRV_SLEW , 0x1F1F);
	else
		gIO.pe_write(IO_SEL, 0xFF, PE_CH_DRV_SLEW , 0x0707);

	if((gOPER.m_PMonArr[PMON_VIH].sc - gOPER.m_PMonArr[PMON_VIL].sc) > 1.0) 
		gIO.pe_write(SCAN_SEL, 0xFF, PE_CH_DRV_SLEW , 0x1F1F);
	else
		gIO.pe_write(SCAN_SEL, 0xFF, PE_CH_DRV_SLEW , 0x0707);

	if((gOPER.m_PMonArr[PMON_VIH].ck - gOPER.m_PMonArr[PMON_VIL].ck) > 1.0) 
		gIO.pe_write(CLK_SEL, 0xFF, PE_CH_DRV_SLEW , 0x1F1F);
	else
		gIO.pe_write(CLK_SEL, 0xFF, PE_CH_DRV_SLEW , 0x0707);
	////// Swing LEVEL 에 따른 TR/TF 설정  END  //////


	////// VIH Level에 따른 DPS VCC 설정 ////////////
	float maxVIH = gOPER.getMaxPELevel();
	double dpsvcc = 5.0;
	if( maxVIH >= 4.0) {
		dpsvcc = maxVIH + 1.1;
		dpsvcc = (dpsvcc - gOPER.m_peDpsVccCal.offset) / gOPER.m_peDpsVccCal.gain;
	}else {
		dpsvcc = 5;
		dpsvcc = (dpsvcc - gOPER.m_peDpsVccCal.offset) / gOPER.m_peDpsVccCal.gain;
	}
	/// 0 V Set & Clamp On
	gIO.set_hv_dac(0, 2.5); // VIL CLAMP 0.0V
	gIO.set_hv_dac(1, 0.0); // VIH CLAMP 0.0V	
	gIO.dac_clamp(true);
	
	gIO.set_hv_dac(7, dpsvcc);
	////// VIH Level에 따른 DPS VCC 설정 END ////////////

	////// PE High CLAMP Level Setting /////////////////
	gIO.set_hv_dac(1, maxVIH*1.15);

	////// PE Low CLAMP Level Setting /////////////////
	float minVIL = gOPER.getMinPELevel();
	if(minVIL <= 0.5) minVIL -= 0.2;
	else minVIL*=0.85;
	minVIL += 2.5;
	gIO.set_hv_dac(0, minVIL);

	////// Power On Sequence /////////////////////////
	if(gOPER.m_PSeq[0].chBit == 0) {
		gIO.dps_run(dps_sel, 1);
		gIO.pe_chip_on();
		gOPER.m_flgDpsOn = dps_sel;
	} else {
		dps_sel = 0;
		for(unsigned int idx = 0; idx < gOPER.m_PSeq.size(); idx++ ) // seq
		{
			if(gOPER.m_PSeq[idx].chBit == 0) continue;
			//printf("On Seq(%d) Sel(0x%X) Delay(%f)\n", idx, gOPER.m_PSeq[idx].chBit, gOPER.m_PSeq[idx].delay);
			//for(int i = 0 ; i < 4; i++) {
			for(int i = 0 ; i < 5; i++) {
				if(gOPER.m_PSeq[idx].chBit & (0x100 << i)) { // channel
					dps_sel |= (0x1 << i);
				}
			}
			gIO.dps_run((dps_sel & 0xf), 1);	
			if(dps_sel >> 4  & 0x1) gIO.dps_vs3f(1);
			//if(gOPER.m_PSeq[idx].chBit & PE) { // PE On
			//	gIO.pe_chip_on();
			//}
			usleep(gOPER.m_PSeq[idx].delay);
		}
		gOPER.m_flgDpsOn |= dps_sel;
	}

	
	//gIO.pe_chip_on();
	// gIO.pe_write(dev_sel, ch_sel, PE_CH_POWER_ON_CTRL, 0x0B1F);
	// gIO.pe_write(dev_sel, ch_sel, PE_C_PDO_RESET, 0x2201);
	gNET.m_nSendPower = SEND_PWR_READ;
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

void DPSOF()
{
	gAPI.debug("	%s in -> ", __FUNCTION__);
	// PE off
	//uint32_t dev_sel = 0x3FFFFF;
	//uint8_t ch_sel = 0xFF; // all channels
	uint8_t dps_sel = 0xF;
	//gIO.pe_off(dev_sel, ch_sel);

	if(gOPER.m_PSeq[0].chBit == 0) {
		gIO.pe_chip_off();
		gIO.dps_run(dps_sel, 0);
	} else {
		dps_sel = 0;
		for(int idx = gOPER.m_PSeq.size() - 1; idx > -1; idx-- ) // seq
		{
			if(gOPER.m_PSeq[idx].chBit == 0) continue;
			//for(int i = 0 ; i < 4; i++) {
			for(int i = 0 ; i < 5; i++) {
				if(gOPER.m_PSeq[idx].chBit & (0x100 << i)) { // channel
					dps_sel |= (0x1 << i);
				}
			}
			if((dps_sel >> 4) & 0x1) {
				gDPS.SetHVout(0.0);
				gIO.dps_vs3f(0);
				dps_sel &= 0xF;
			}
			gIO.dps_run(dps_sel, 0);	
			//if(gOPER.m_PSeq[idx].chBit & PE) { // PE On
			//	gIO.pe_chip_off();
			//}
			usleep(gOPER.m_PSeq[idx].delay);
		}
	}
	gOPER.m_flgDpsOn = 0;

	gIO.set_hv_dac(0, 2.5); // VIL CLAMP 0.0V
	gIO.set_hv_dac(1, 0.0); // VIH CLAMP 0.0V	
	//gIO.dac_clamp(false);

	//gIO.pe_chip_off();
	//Sleep(0.005);
	//gIO.dps_run(dps_sel, 0);
	// gIO.pe_write(dev_sel, ch_sel, PE_CH_POWER_ON_CTRL, 0x0B1);
	// gIO.pe_write(dev_sel, ch_sel, PE_C_PDO_RESET, 0x2200);
	gNET.m_nSendPower = SEND_PWR_OFF;
	gAPI.debug("	%s out <- ", __FUNCTION__);
}

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
	if (pin > 7) return 0;
	
	int num = pin; // num = 0~3
	int adr = 271;
	if (pin > 3) {
		adr = 295;
		num = pin - 4;
	}

	adr += (num * 3);

	return gIO.readPG(PG_SYSTEM | adr, 3);
}
/************************************
	EMMC TEST
************************************/
void EMMC_TOGGLE_TEST(const char* pc)
{
	gAPI.debug("	%s %s in -> ", __FUNCTION__, pc);	
	DATA_MODE(DUT_DATA_ENABLE | DUT_DATA_BYSCAN);
	gOPER.startTest(pc);
//	gNAND.testData();
	gAPI.debug("	%s out <- ", __FUNCTION__);
}


int GET_TOGGLE_RESULT(int dno)
{
	int ret = TOGGLE_PROG;
	int curScan = gFM.readScanIndex();
	
	dut* p = gBIB.getDut(dno);
	
	if (GET_DUTDATA_COUNT() == 0) return TOGGLE_NONE;
	byte* pData = GET_DUTDATA(dno);
	byte start = *pData;	
	printf("%s dno:%d, rd:%d - First data %x\n", __FUNCTION__, dno, GET_DUTDATA_COUNT(), start);	
	for (int i = 0; i < GET_DUTDATA_COUNT(); i++)
	{	
		if (start != *pData) {
			printf("Toggle Catch(Index %d) %x -> %x \n", i, start, *pData);
			return TOGGLE_PROG;
		}
		pData++;
	}
	if (start == 0)ret = TOGGLE_LOW;
	else if (start == 0xff)ret = TOGGLE_HIGH;
	else ret = TOGGLE_PROG;
	//printf("Finish data %x\n", start);
	

	return ret;
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

void SHOW_FMDATA(int offset, int readSZ)
{
	gFM.showFmData(offset, readSZ);
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
	gPG.nextSync();
}

/************************************
	HV DAC function
************************************/
//uint8_t SET_HV_DAC(int ch, double volt)
//{
//	double setVolt = 0;
//
//	volt = (volt - gOPER.m_peDpsVccCal.offset) / gOPER.m_peDpsVccCal.gain;
//
//	if(ch == 5)
//		setVolt = (volt/2.0);
//	else
//		setVolt = (volt/4.0);
//
//	uint16_t dacCode = 0;
//    dacCode = (uint16_t)(setVolt / (DAC_SPAN / DAC_RESOLUTION)) & 0xFFF;
//	//printf("\t DAC.Set : %s[ %01X ] %s[ %6.3f(%6.3f) ] %s[ %03X ]\n", "ch", ch, "volt", volt, setVolt, "dacCode", dacCode);
//	return gIO.dac_write(DAC_C_WO_SINREG, ch, dacCode);
//}

void DAC_CLAMP(bool bOn)
{
	gIO.dac_clamp(bOn);
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
	gFM.m_nDutDataProc = DUT_DATA_NONE | DUT_DATA_BYSCAN;
	gFM.setReadDut();

	sprintf(gAPI.m_sCurStep, "%s", name);
//	sprintf(gNET.m_pShmDef->sCurStep, "%s", name);

	gPG.m_nWritePC = 0;    // PT PC Count
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

void fmDrvMode(int mode)
{
	gFM.setFmDrvMode(mode);
}

void writeCpeDelay(int stb0, int stb1)
{
	gTG.writeCpeDelay(stb0, stb1);
}

void writeDskDelay(int delay)
{	
	gTG.writeDskDelay(delay);
}

void writeClkDelay(int select, int delay)
{
	gTG.writeClkDelay(select, delay);
}

void initDelay()
{
	gTG.initDelay();
}
void READ_DPS()
{
	gAPI.m_bMonDPS = true;
	//printf("READ_DPS");
	gOPER.readDPS();
}

void READ_VOUT(int type, float& VMin, float& VMax) // 0~1,2,3
{
	int dpsCh = 0;
	if(type < 0x100) { /// 1, 2, 3, 4, 5
		int ch[5] = {0, 1, 4, 2, 3};
		if((type & 0xFF) < 1 || (type & 0xFF) > 5) {
			MESSAGE("DPS channel check.", ERR);
			return;
		}
		dpsCh = ch[type-1];
	}else {/// VS1, VS2, VS3, VS4, VS5
		int count = 0;
		for(int idx = 0; idx < 5; idx++) {
			if((type & (0x100<<idx)) != 0) {
				count++;
				dpsCh = idx;
			}
		}
		if(count != 1) {
			MESSAGE("DPS channel check.", ERR);
			return;
		}
		
	}

	if(dpsCh != 4) {
		gDPS.ReadChVout(dpsCh, VMin, VMax);
	}else
	{
		VMax = VMin = gDPS.ReadCvmVout();
	}
}

float READ_VOUT_ADC(int ch)
{
	if(ch >= VS1) {
		int nCh = 0;
		int count = 0;
		for(int n = 0 ; n < MAX_DPS_NUM ; n++){
			if(ch & (VS1 << n)) {
				count++;
				nCh = n;
			}
		}
		if(count > 1) {
			return -1.0;
			MESSAGE("DPS channel check. (READ_VOUT_ADC)", ERR);
		}
		switch(nCh) {
			case VS1: ch = 0; break;
			case VS2: ch = 1; break;
			case VS4: ch = 3; break;
			case VS5: ch = 4; break;
			case VS3: ch = 2; break;
			default : ch = nCh; break;
		}
	}
	

	if (ch < 0 || ch > 5) {
		return -1.0;
		MESSAGE("DPS channel check. (READ_VOUT_ADC)", ERR);
	}

	float volt;
	short data[8];
	gIO.adc_read(1, 7, data);

	volt = (data[ch] * 10.0 / 65535.0) * 2;
	return volt;
}

float READ_IOUT(int type, bool bCalApply) // 0~1,2,3
{
	int dpsCh = 0;
	if(type < 0x100) { /// 1, 2, 3, 4, 5
		int ch[5] = {0, 1, 4, 2, 3};
		if((type & 0xFF) < 1 || (type & 0xFF) > 5) {
			MESSAGE("DPS channel check.", ERR);
			return -1.0;
		}
		dpsCh = ch[type-1];
	}else {/// VS1, VS2, VS3, VS4, VS5
		int count = 0;
		for(int idx = 0; idx < 5; idx++) {
			if((type & (0x100<<idx)) != 0) {
				count++;
				dpsCh = idx;
			}
		}
		if(count != 1) {
			MESSAGE("DPS channel check.", ERR);
			return -1.0;
		}
		
	}

	if(dpsCh != 4) {
		return gDPS.ReadChIout(dpsCh, bCalApply);
	}else
	{
		return gDPS.ReadCvmIout();
	}
}

float READ_DPS_TEMP1(int ch)
{
	if (ch < 0 || ch > 3)
		printf("DPS Channel number is invalide. : %d\n", ch);

	vector<int> i2c_num = { 0, 2, 4, 5 };
	return gDPS.ReadTemperature_1(DPS_SINGLE_SEL, i2c_num[ch]);
}

float READ_DPS_TEMP2(int ch)
{
	if (ch < 0 || ch > 3)
		printf("DPS Channel number is invalide. : %d\n", ch);

	vector<int> i2c_num = { 0, 2, 4, 5 };
	return gDPS.ReadTemperature_2(DPS_SINGLE_SEL, i2c_num[ch]);
}

uint8_t PE_WRITE(uint8_t addr, uint16_t wdata, bool bCentral)
{
	uint8_t ch_sel = 0xFF;
	if (bCentral) ch_sel = 0x0;

	return gIO.pe_write(0x3FFFFF, ch_sel, addr, wdata);
}

uint8_t PE_READ(uint32_t nChip, int16_t ch, uint8_t addr, uint16_t* wdata)
{
	uint32_t nDev = 1 << nChip;
	uint8_t pe_ch = 0;
	if (ch > -1 && ch < 8)  pe_ch = (1 << ch);
	return gIO.pe_read(nDev, pe_ch, addr, wdata);
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

	gIO.checkBFPGA();	

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
		gIO.pe_init();
		gIO.cvm_set_config();
		gIO.cvm_write_cal_reg();
		gIO.adc_settling(1000000);
		gPG.stopPG();
		gOPER.ReadCalFactor();
		gFM.initFailMem();
		gFM.setFmOverStop(false);
		gPG.writeIOMask(0, 0);
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

	// PE Channel ON
	gIO.pe_channel_on();
	
	if (gAPI.m_nTestMode == TEST_MODE_DIAG) { // DIAG MODE
		diag_log_open();
		TEST();
		// PE Chip & Channel OFF
		diag_log_close();
		gIO.pe_channel_off();
		gIO.pe_chip_off();
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
	gIO.pe_channel_off();
	gIO.pe_chip_off();

//int lastSendWait = 1000 * 1000;
//if (gAPI.m_nTestMode == TEST_MODE_HANDLER) lastSendWait = 1000 * 5000;

	usleep(1000*1000); // Wait Message Send

	gAPI.debug("%s out <- ", __FUNCTION__);

	return 1;
}


/************************************************************

#define VER_DATE "2025/09/03"
#define API_VER "1.4.5" by bin.yun
     Alert Error시  Status Read할수있도록.


#define VER_DATE "2025/08/18"
#define API_VER "1.4.4" by bin.yun
	  VS3 SetHVout 후 5ms -> 10ms delay 후 VS3 On

#define VER_DATE "2025/08/14"
#define API_VER "1.4.3" by bin.yun
	  dut mask arg 입력받고 dut mask 기능 추가

#define VER_DATE "2025/08/11"
#define API_VER "1.4.2" by bin.yun
	  power send  1s -> 2s period 변경


#define VER_DATE "2025/07/15"
#define API_VER "1.4.1" by bin.yun
	 Board Temp Read SEQ 변경
	 Diag Baord Temp func add

#define VER_DATE "2025/07/14"
#define API_VER "1.4.0" by bin.yun
	 temp 관련 wait temp 시 1분간격 set temp 보내주는걸로.. os 다시 붙었을때 때문

#define VER_DATE "2025/05/29"
#define API_VER "1.3.9" by bin.yun
    FM Read 1024 넘을시 두번째 Read 시 Count * FM_SIZE 안해준 부분 
	버그 수정

#define VER_DATE "2025/02/12"
#define API_VER "1.3.8" by bin.yun
	Contact Test시 Step들 결과 Fail Sum되게..

#define VER_DATE	"2025/02/05"
	MOD : Handler Mode 시 readDPS 다시 넣음.. 

#define VER_DATE	"2025/02/04"
#define API_VER		"1.3.7"
	MOD : Handler Mode 시 DPS Error Clear 추가  gDPS.I2cSendByteAll(I2C_CMD_CLEAR_FAULTS);
	MOD : Handler Mode 시 readDPS 다시 뺌

#define VER_DATE	"2025/01/15"
#define API_VER		"1.3.6"
    MOD : Handler Mode 시 readDPS 살리도록.. DPS Error check용도


 #define VER_DATE	"2024/12/16"
  #define API_VER		"1.3.5"
    BUG : handler Mode시 TestMode의 Handler 모드를 되돌림 버그. busehandler flag 추가
    
* 
* API_VER "1.3.4"
	MOD : Handler Mode 시간 최대로 줄이도록. 핸들러일땐 power 없음, wait sleep 없음

API_VER "1.3.3"
	MOD : Handler Mode 추가. 
	      Handler Mode 시 DPS Init 사용 X (시간 소요 문제)

API_VER "1.3.2"
	MOD : READ_VOUT_ADC 함수 내 채널 check 수정

API_VER "1.3.1"
	ADD : all Mode 추가  contact, run 동시 진행


API_VER "1.3.0"
	ADD : FM Data Read 시 Full Size(2048) 읽으면 안됨.  io error
	1024 넘어가면 끊어서 읽는걸로

API_VER "1.2.8"
	ADD : FM OVER STOP Interface 추가

API_VER "1.2.7"
	ADD : Board Temp Log (PE Temp용)추가

API_VER "1.2.6"
	FIX : writePattern  fclose  add.
	

API_VER "1.2.5"
	FIX : Running중 DPS Error시 Test중단하도록.
	MOD : DPS Error 처리 정리 
		  1. SW Err Check, 
		  2. V Err Check,
		  3. Alert(Current) Check
		  4. Power Good Check

API_VER "1.2.4"
	ADD : Dut Pass Fail Result Invert 기능 추가
	MOD : Scan Index 128 -> 256

API_VER "1.2.3"
	MOD : FM Size 변경
	MOD : TG DeadZone warning 제거. apps 요청
	MOD : CMD PG Bridge command 추가
API_VER "1.2.2"
	VS3 추가 및 DPSON, DPSOF  API 추가

API_VER "1.2.1"
	MOD : STRB_EDGE ( tsno 0~15 로 변경)

API_VER "1.2.0"
	ADD : MEAS MPAT Default Delay Time Function 추가

API_VER "1.1.9"
	BUG : Sub Data File 저장시 Xml Handle Delete 안함..  
	      대용량 또는 다수의 sub파일 발생시 memory full문제 발생!
		  수정

API_VER "1.1.8"
	MOD : Scan Index 전송 1초 마다로 변경  Block Test시 다량 패킷으로 socket 죽는듯

API_VER "1.1.7"
	ADD : imprint 기능 변경 및 nand관련 bug들 수정.

API_VER "1.1.6"
	ADD : NAND Cycle Test 기능 추가.

API_VER "1.1.6"
	MOD : NAND 기능
	       Judge Test : MASK, Total Collection Select 가능 하도록
		   Write Test : Mask, Total Collection Select 가능 하도록
	ADD : SET_FBLOCK 추가
	MOD : Comp Mode 시 Fail DQ 만 Capture하도록.

API_VER "1.1.5"
	MOD : FM Process 변경 (DATA Mode처리하며)	      
	      H / L 처리 분리
		  FM Read시 Active, Stable 분리
		  기존 Test 호환
		  
	ADD : Nand 기능 구현
			NAND_ID					구현 / 검증
			NAND_BLOCK_TEST			구현 / 검증
			NAND_FCOUNT_SC			구현 / 검증
			NAND_FCOUNT_DETAIL_SC	구현 / 확인 
			NAND_INFO_READ_SC		구현 
			NAND_INFO_WRITE_SC		구현 / FM만 확인
			NAND_IMPRINT_SC			구현 / FM만 확인
			NAND_DATA_READ_SC		구현 / 검증
			NAND Sub File 처리		구현 

API_VER "1.1.4"
	ADD : Nand 기능 구현
			NAND_ID					구현 / 검증
			NAND_BLOCK_TEST			구현 / 검증
			NAND_FCOUNT_SC			구현 / 검증
			NAND_FCOUNT_DETAIL_SC	구현
			NAND_INFO_READ_SC		구현
			NAND_INFO_WRITE_SC		구현
			NAND_IMPRINT_SC			구현
			NAND_DATA_READ_SC		아직..
			NAND Sub File 처리		구현 / 검증


API_VER "1.1.3"
	ADD : NAND Function  구현중.
	MOD : 기존 Operation Logic 변경.


API_VER "1.1.2"
	ADD : Megro Step Function Int ARG 추가. STEPI(Name, ARG)

  API_VER "1.1.1"
	fix : Vector Memory Ctrl 기능 구현
	     vec class 생성 
		 SET_VECTOR 함수구현
 
 API_VER "1.0.8"
	fix : When step Multi excute, Result Bug Fix




* ***********************************************************/
