#ifndef __TGC__
#define __TGC__

#include "STAR_TEST.h"

#define RATE_RESOLUTION 4.0	//(4 NS)
#define CLK_RESOLUTION	2.0	//(2 NS)

#define MIN_EDGE_WIDTH  2.0

#define TG_MEM_RATE		0 
#define TG_MEM_STRB		1
#define TG_MEM_STRB_B	2 
#define TG_MEM_DRE 
#define TG_MEM_STRB_B	2 




class tgc
{
public: 
	tgc() { init(); }
	~tgc() {}

	uint calcRate(double rate);
	uint calcClk(double clk);
	
	double m_fRateArr[16];
	void init();
	void writeTGMem(int type, int tSet, int value);

	void writeRate(int tSet, double val);
	void writeRate(double r0, double r1, double r2, double r3, double r4,
		double r5, double r6, double r7, double r8, double r9,
		double r10, double r11, double r12, double r13,
		double r14, double r15);

	void writeECLK(int type, int tSet, double val);
	void writeEClk(int type, double r0, double r1, double r2, double r3, double r4,
		double r5, double r6, double r7, double r8, double r9,
		double r10, double r11, double r12, double r13,
		double r14, double r15);

	byte m_nformatArr[64];
	void writeFormat(uint pin, uint format, uint edgeB, uint edgeC = 0xf0);  // foramt, select = b/cclk
	void writeFormatBuf();
	void writeSTRBEdge(int tsNo, double stb0, double stb1);

	void initDelay();
	void writeCpeDelay(int stb0, int stb1);
	void writeDskDelay(int delay);
	
};

#endif
