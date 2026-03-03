#include "tgc.h"
#include "define.h"
#include "ioc.h"
#include "errc.h"
#include "net.h"

extern ioc		gIO;
extern errctrl	gERR;

void tgc::init()
{
	memset(m_nformatArr, 0, sizeof(m_nformatArr));
	memset(m_fRateArr, 0, sizeof(m_fRateArr));
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
	uint tgAdr = PG_TG_MEM | (type << 10) | (tSet << 4);
	value &= 0xffffff;
	gIO.writePG(tgAdr, value);
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


// type = BCLK1 ~ CCLK16
void tgc::writeECLK(int type, int tSet, double val)
{
	bool bChange = false;
	if (val > m_fRateArr[tSet]) gERR.addErr(ERR_TS, __FUNCTION__, 
		"ECLK TS%d  Set Over RATE. RATE %.2f, ECLK Type %d %.2f\n",
		tSet, m_fRateArr[tSet], type, val);

	/*When DeadZone Setting, Remove Message Send */
//	if((val != OPEN && val < 2.5) || (val > (m_fRateArr[tSet] - 2.5))) {
//		char strBuff[256];
//		if((type % 2) == 1)
//			sprintf(strBuff,"TS%d BCLK%d EDGE(%0.1fns) is dead zone value.", tSet, (type-BCLK1)/2+1, val);
//		else
//			sprintf(strBuff,"TS%d CCLK%d EDGE(%0.1fns) is dead zone value.", tSet, (type-CCLK1)/2+1, val);
//		MESSAGE(strBuff, MSG_TYPE_WARN);
//	}

	int nWriteVal = 0;	
	if(val == OPEN) {
		nWriteVal = (0x1 << 23);
	}else {
		if (val < 2.5) val = 2.5; // BCLK 0ns -> 2ns	
		if (val > m_fRateArr[tSet] - 2.5) val -= 2.5; // CCLK Rate ns -> Rate - 2 ns
		
		nWriteVal = calcClk(val);
	}
	writeTGMem(type, tSet, nWriteVal);
}


// ECLK(BCLK1, bedge0, bedge1, bedge2, bedge3, ... );
#define CPE_DEFAULT_DELAY 2
void tgc::writeEClk(int type, double r0, double r1, double r2, double r3, double r4,
	double r5, double r6, double r7, double r8, double r9,
	double r10, double r11, double r12, double r13,
	double r14, double r15)
{
	if (!(type >= BCLK1 && type <= CCLK16))
	{
		type -= STRB;
		type++;	
	}

	writeECLK(type, 0, r0);
	writeECLK(type, 1, r1);
	writeECLK(type, 2, r2);
	writeECLK(type, 3, r3);
	writeECLK(type, 4, r4);
	writeECLK(type, 5, r5);
	writeECLK(type, 6, r6);
	writeECLK(type, 7, r7);
	writeECLK(type, 8, r8);
	writeECLK(type, 9, r9);
	writeECLK(type, 10, r10);
	writeECLK(type, 11, r11);
	writeECLK(type, 12, r12);
	writeECLK(type, 13, r13);
	writeECLK(type, 14, r14);
	writeECLK(type, 15, r15);
	
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

	double remind0 = int(stb0) % int(rate);
	double remind1 = int(stb1) % int(rate);

	for (int i = 0; i < 16; i++)
	{
		writeECLK(1, i, remind0);
		writeECLK(2, i, remind1);		
	}

	writeCpeDelay(cpe0, cpe1);

//	printf("RATE[%d] %.2f, stb0 %.2f, stb1 %.2f, cpe0 %d, cpe1 %d\n", tsNo, rate, 
//		stb0, stb1, remind0, remind1);
}

void tgc::writeFormatBuf()
{
//	gIO.m_bDebug = true;
	gIO.writePGData(PG_FORMAT, m_nformatArr, 53);
//	gIO.m_bDebug = false;
}

void tgc::writeFormat(uint pin, uint format, uint edgeB, uint edgeC)
{
	if (pin == STRB || pin == STRB_B)
	{
		gERR.addErr(ERR_TS, __FUNCTION__, "STRB Edge Can\'t Set TSSET(). Set ECLK()");
		return;
	}
	/// Format	
	bool bOdd = false;	

	uint pos = pin / 2; // Fromat Reg Position
	if (pos % 2)bOdd = true;			
	uint idx = (pos / 2);		// Sepeate Byte Setting	
	if (pin == DRE || pin == DRE_B) 
	{
		idx = 18; bOdd = false;
	}else if (pin >= IO0 && pin <= IO0_B) {
		idx = (pin - IO0) / 4;
		// memofs=0, enumofs=90(IO0) -->  /2 = a or b(HL), one more /2 = odd even
	}
	else if (pin >= DR0 && pin <= DR44_B) {
		idx = 19 + (pin / 4);
		// memofs=19  enumofs=0(DR0) -->  /2 = a or b(HL), one more /2 = odd even
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

	if (pin == DRE || pin == DRE_B) {
		if (edgeB != ECLK_SAME) gERR.addErr(ERR_TS, __FUNCTION__, "DRE Edge Can\'t Set TSSET(). Set ECLK()");
		return;
	}

	///////////////  debuging  ////////////////
//	uint mask = 0x0f;
//	if (bOdd) mask = 0xf0;
//	printf("Arr[%d] = %x, pin %d, read : %02x write : %02x\n", idx, write, pin , read & mask, write);
	
	/// B/CCLK	
	if (edgeB == ECLK_SAME) {
		gERR.addErr(ERR_TS, __FUNCTION__, "BEdge Don\'t Selected. Must Set TSSET()");
		return;
	}

	if (pin == DRE || pin == DRE_B)
	{
		pos = 120; 
	}
	else if (pin >= IO0 && pin <= IO0_B) {
		pos = 72 + ((pin - IO0) / 2);//  /2 -->  a or b(HL)
	}
	else if (pin >= DR0 && pin <= DR44_B) {
		pos = (pin / 2);//   /2 -->  a or b(HL)
	}

	uint regAdr = PG_TG_REG | pos;

	uint val = (edgeB << 4) | edgeB;
	if (edgeC != ECLK_SAME)
		val = (edgeC << 4) | edgeB;

	gIO.writePG(regAdr, val, 1);

}


void tgc::initDelay()
{
	writeDskDelay(51);
}

void tgc::writeCpeDelay(int stb0, int stb1)
{

	gIO.writePG(PG_SYSTEM | 23, stb0, 0x1);
	usleep(1000);

	gIO.writePG(PG_SYSTEM | 24, stb1, 0x1);
	usleep(1000);

}


void tgc::writeDskDelay(int delay)
{

	gIO.writePG(PG_SYSTEM | 25, delay, 0x1);

	gIO.writePG(PG_SYSTEM | 22, 1, 0x1);
	gIO.writePG(PG_SYSTEM | 22, 0, 0x1);
}

