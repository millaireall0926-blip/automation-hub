#include "define.h"
#include "errctrl.h"
#include "tgc.h"
#include "ioc.h"
#include "STAR_HSBI.h"

#define EDGEC_WIDTH			2.422		// NS
#define EDGEC_VERNIE		(0x1F << 19)

extern ioc  gIO;
extern errctrl gERR;

void tgc::init()
{
	memset(m_nformatArr, 0, sizeof(m_nformatArr));
	memset(m_fRateArr, 0, sizeof(m_fRateArr));
	memset(m_iCpeFine, 0, sizeof(m_iCpeFine));
}

uint tgc::calcRate(double rate)
{
	uint retVal = 0;

	retVal = uint(rate / RATE_RESOLUTION) & 0xffffff;
	return retVal;
}

uint tgc::calcClk(double clk)
{
	uint retVal = 0;

	retVal = uint(clk / CLK_RESOLUTION) & 0xffffff;
	return retVal;
}


void tgc::writeTGMem(int type, int tSet, int value)
{
	uint tgAdr = TG_MEM | (type << 10) | (tSet << 4);
	value &= 0xffffff;
	gIO.writeTG(PCI1, tgAdr, value);
    gIO.writeTG(PCI2, tgAdr, value);
}


void tgc::writeFormatBuf()
{
    for(int nPCI = 0 ; nPCI < MAX_PCI; nPCI++) {
    //	gIO.m_bDebug = true;
        gIO.writePGData(PCI_SEL(nPCI), PG_ALL, PG_FORMAT, m_nformatArr, 41);
        //gIO.c2p_write(PCI_SEL(nPCI), PG_ALL, PG_FORMAT, 53, m_nformatArr);
    //	gIO.m_bDebug = false;
    }
}

void tgc::writeFormat(uint pin, uint format)
{
	if (pin == STRB || pin == STRB_B)
	{
		gERR.addErr(ERR_TS, __FUNCTION__, "STRB Edge Can\'t Set TSSET(). Set ECLK()");
		return;
	}
	/// Format	
	bool bOdd = false;
	
	uint pos = pin / 2; // A, B Part Gather
	
	if ((pos) % 2)bOdd = true;
	//if (pin >= SCAN0 && pin <= SCAN31_B) pos+=2;
	if (pin >= SCAN0 && pin <= SCAN7_B) pos+=2;
	uint idx = (pos / 2);		// Sepeate Byte Setting
	
	if (pin == DRE || pin == DRE_B) 
	{
		idx = 36; bOdd = false;
	}

	uint write = 0;
	uint read = m_nformatArr[idx];
	if (bOdd) {
		write = (read & 0x0f) | (format << 4);
	}
	else {
		write = (read & 0xf0) | format ;
	}
	m_nformatArr[idx] = write;

	//if (pin == DRE || pin == DRE_B) {
	//	if (edgeB != ECLK_SAME) gERR.addErr(ERR_TS, __FUNCTION__, "DRE Edge Can\'t Set TSSET(). Set ECLK()");
	//	return;
	//}

	///////////////  debuging  ////////////////
//	uint mask = 0x0f;
//	if (bOdd) mask = 0xf0;
//	printf("Arr[%d] = %x, pin %d, read : %02x write : %02x\n", idx, write, pin , read & mask, write);
	
	/// B/CCLK	
	//if (edgeB == ECLK_SAME) {
	//	gERR.addErr(ERR_TS, __FUNCTION__, "BEdge Don\'t Selected. Must Set TSSET()");
	//	return;
	//}

	pos = pin / 2; // A, B Part Gather
	//if (pin >= SCAN0 && pin <= SCAN31_B)pos+=2;
	if (pin >= SCAN0 && pin <= SCAN7_B)pos+=2;
	//uint regAdr = TG_REG | pos;
	//uint regAdr = PG_FORMAT | pos;
//
	//uint val = (edgeB << 4) | edgeB;
	//if (edgeC != ECLK_SAME)
	//	val = (edgeC << 4) | edgeB;
	//
    //gIO.writePG(PCI1, PG_ALL, regAdr, val, 1);
	//gIO.writePG(PCI2, PG_ALL, regAdr, val, 1);


}

void tgc::writeRate(int tSet, double val)
{
	m_fRateArr[tSet] = val;
	int nWriteVal = calcRate(val);
	writeTGMem(TG_MEM_RATE, tSet, nWriteVal);
}

void tgc::writeRate(double r0, double r1, double r2, double r3, double r4,
	double r5, double r6, double r7, double r8, double r9,
	double r10, double r11, double r12, double r13,
	double r14, double r15)
{
	writeRate(0, r0);
	writeRate(1, r1);
	writeRate(2, r2);
	writeRate(3, r3);
	writeRate(4, r4);
	writeRate(5, r5);
	writeRate(6, r6);
	writeRate(7, r7);
	writeRate(8, r8);
	writeRate(9, r9);
	writeRate(10, r10);
	writeRate(11, r11);
	writeRate(12, r12);
	writeRate(13, r13);
	writeRate(14, r14);
	writeRate(15, r15);
    
	///////////////  debuging  ////////////////
//    gERR.print("RATE :");
//    for (int i = 0; i < 16; i++)
//        gERR.print("%.2f ", m_fRateArr[i]);
//    gERR.print("\n");
}

//void tgc::writeECLK(int type, int tSet, double val)
//{
//	bool bChange = false;
//	if (val > m_fRateArr[tSet]) gERR.addErr(ERR_TS, __FUNCTION__, 
//		"ECLK TS%d  Set Over RATE. RATE %.2f, ECLK Type %d %.2f\n",
//		tSet, m_fRateArr[tSet], type, val);
//
//	/*When DeadZone Setting, Remove Message Send */
////	if((val != OPEN && val < 2.5) || (val > (m_fRateArr[tSet] - 2.5))) {
////		char strBuff[256];
////		if((type % 2) == 1)
////			sprintf(strBuff,"TS%d BCLK%d EDGE(%0.1fns) is dead zone value.", tSet, (type-BCLK1)/2+1, val);
////		else
////			sprintf(strBuff,"TS%d CCLK%d EDGE(%0.1fns) is dead zone value.", tSet, (type-CCLK1)/2+1, val);
////		MESSAGE(strBuff, MSG_TYPE_WARN);
////	}
//
//	int nWriteVal = 0;	
//	if(val == OPEN) {
//		nWriteVal = (0x1 << 23);
//	}else {
//		if (val < 2.5) val = 2.5; // BCLK 0ns -> 2ns	
//		if (val > m_fRateArr[tSet] - 2.5) val -= 2.5; // CCLK Rate ns -> Rate - 2 ns
//		
//		nWriteVal = calcClk(val);
//	}
//	writeTGMem(type, tSet, nWriteVal);
//}


// ECLK(BCLK1, bedge0, bedge1, bedge2, bedge3, ... );
#define CPE_DEFAULT_DELAY 0
//void tgc::writeEClk(int type, double r0, double r1, double r2, double r3, double r4,
//	double r5, double r6, double r7, double r8, double r9,
//	double r10, double r11, double r12, double r13,
//	double r14, double r15)
//{
//	if (!(type >= BCLK1 && type <= CCLK16))
//	{
//		type -= STRB;
//		type++;	
//	}
//
//	writeECLK(type, 0, r0);
//	writeECLK(type, 1, r1);
//	writeECLK(type, 2, r2);
//	writeECLK(type, 3, r3);
//	writeECLK(type, 4, r4);
//	writeECLK(type, 5, r5);
//	writeECLK(type, 6, r6);
//	writeECLK(type, 7, r7);
//	writeECLK(type, 8, r8);
//	writeECLK(type, 9, r9);
//	writeECLK(type, 10, r10);
//	writeECLK(type, 11, r11);
//	writeECLK(type, 12, r12);
//	writeECLK(type, 13, r13);
//	writeECLK(type, 14, r14);
//	writeECLK(type, 15, r15);
//	
//}

void tgc::writeEdge(int type, int tSet, double val, bool bCedge)
{
	bool bChange = false;
	if (val > m_fRateArr[tSet]) gERR.addErr(ERR_TS, __FUNCTION__, 
		"ECLK TS%d  Set Over RATE. RATE %.2f, ECLK Type %d %.2f\n",
		tSet, m_fRateArr[tSet], type, val);

	int nWriteVal = 0;	
	if(val == OPEN) {
		nWriteVal = (0x1 << 23);
		//printf("OPEN ");
	}else {
		//if (val < 2.5) val = 2.5; // BCLK 0ns -> 2ns	
		//if (val > m_fRateArr[tSet] - 2.5) val -= 2.5; // CCLK Rate ns -> Rate - 2 ns
		
		if(bCedge && (val > EDGEC_WIDTH)) {
			nWriteVal = (calcClk(val - EDGEC_WIDTH) | EDGEC_VERNIE);
		}
		else {
			nWriteVal = calcClk(val);
		}
		
		//nWriteVal = calcClk(val);

		//printf("0x%X ", nWriteVal);
	}
	writeTGMem(type, tSet, nWriteVal);

}

void tgc::writeStbEdge(int type, int tSet, double val, bool bCedge)
{
	bool bChange = false;
	if (val > m_fRateArr[tSet]) gERR.addErr(ERR_TS, __FUNCTION__, 
		"ECLK TS%d  Set Over RATE. RATE %.2f, ECLK Type %d %.2f\n",
		tSet, m_fRateArr[tSet], type, val);

	int nWriteVal = 0;	
	if(val == OPEN) {
		nWriteVal = (0x1 << 23);
		//printf("OPEN ");
	}else {
		//if (val < 2.5) val = 2.5; // BCLK 0ns -> 2ns	
		//if (val > m_fRateArr[tSet] - 2.5) val -= 2.5; // CCLK Rate ns -> Rate - 2 ns
		
		if(bCedge && (val > EDGEC_WIDTH)) {
			nWriteVal = (calcClk(val - EDGEC_WIDTH) | EDGEC_VERNIE);
		}
		else {
			nWriteVal = (calcClk(val) << 19);
		}

		//nWriteVal = calcClk(val);

		//printf("0x%X ", nWriteVal);
	}
	writeTGMem(type, tSet, nWriteVal);

}

void tgc::writeEdge(int type, double e0, double e1, double e2, double e3, double e4,
		double e5,  double e6,  double e7,  double e8, double e9,
		double e10, double e11, double e12, double e13,
		double e14, double e15, bool bCedge)
{
	writeEdge(type, 0, e0, bCedge);
	writeEdge(type, 1, e1, bCedge);
	writeEdge(type, 2, e2, bCedge);
	writeEdge(type, 3, e3, bCedge);
	writeEdge(type, 4, e4, bCedge);
	writeEdge(type, 5, e5, bCedge);
	writeEdge(type, 6, e6, bCedge);
	writeEdge(type, 7, e7, bCedge);
	writeEdge(type, 8, e8, bCedge);
	writeEdge(type, 9, e9, bCedge);
	writeEdge(type, 10, e10, bCedge);
	writeEdge(type, 11, e11, bCedge);
	writeEdge(type, 12, e12, bCedge);
	writeEdge(type, 13, e13, bCedge);
	writeEdge(type, 14, e14, bCedge);
	writeEdge(type, 15, e15, bCedge);
}

void tgc::writeSTRBEdge(int tsNo, double stb0, double stb1)
{
	if (tsNo > 16 || tsNo < 1)
	{
		gERR.addErr(ERR_TS, __FUNCTION__, "STRB_EDGE TSNo %d Error\n", tsNo);
		return;
	}

	int cpe0 = CPE_DEFAULT_DELAY;
	int cpe1 = CPE_DEFAULT_DELAY;
	//int RateIndex = 15;
	double rate = m_fRateArr[tsNo];
	if (rate <= 0) return;
	
	cpe0 += int(floor(stb0 / rate));
	cpe1 += int(floor(stb1 / rate));

	//double remind0 = int(stb0) % int(rate);
	//double remind1 = int(stb1) % int(rate);

	double remind0 = stb0 - (rate*(cpe0-CPE_DEFAULT_DELAY));
	double remind1 = stb1 - (rate*(cpe1-CPE_DEFAULT_DELAY));

	int cpe_tune0 = 0;
	int cpe_tune1 = 0;

	for (int i = 0; i < 16; i++)
	{
		//writeECLK(1, i, remind0);
		//writeECLK(2, i, remind1);

		writeStbEdge(1, i, remind0, true); //stb 0
		writeStbEdge(2, i, remind1, true); //stb 1

		// CPE_TUNE 추가
		if(remind0 < (rate/2.0)) 
			cpe_tune0 = m_iCpeFine[0][0];
		else
			cpe_tune0 = m_iCpeFine[0][1];

		if(remind1 < (rate/2.0)) 
			cpe_tune1 = m_iCpeFine[1][0];
		else
			cpe_tune1 = m_iCpeFine[1][1];


		//printf("CPE Tune Write: %d %d\n", cpe_tune0, cpe_tune1);

		//gIO.writeTG(PCI1, (0x1<<9 | 25), cpe_tune0, 1);
		//gIO.writeTG(PCI2, (0x1<<9 | 25), cpe_tune0, 1);

		//gIO.writeTG(PCI1, (0x1<<9 | 39), cpe_tune1, 1);
		//gIO.writeTG(PCI2, (0x1<<9 | 39), cpe_tune1, 1);

		writePG(0x1<<9 | 25, cpe_tune0, 1);
		writePG(0x1<<9 | 39, cpe_tune1, 1);
	}

	// PG Register에 Write
	writeCpeDelay(cpe0, cpe1);

//	printf("RATE[%d] %.2f, stb0 %.2f, stb1 %.2f, cpe0 %d, cpe1 %d\n", tsNo, rate, 
//		stb0, stb1, remind0, remind1);
}

void tgc::writeCpeDelay(int stb0, int stb1)
{

	gIO.writePG(PCI1, PG_ALL, PG_SYSTEM | 23, stb0, 0x1);
    gIO.writePG(PCI2, PG_ALL, PG_SYSTEM | 23, stb0, 0x1);
	usleep(1000);

	gIO.writePG(PCI1, PG_ALL, PG_SYSTEM | 24, stb1, 0x1);
    gIO.writePG(PCI2, PG_ALL, PG_SYSTEM | 24, stb1, 0x1);
	usleep(1000);

}
