#ifndef __TGC__
#define __TGC__

//#define RATE_RESOLUTION 4.0	//(4 NS)
//#define CLK_RESOLUTION	2.0	//(2 NS)

#define RATE_RESOLUTION 0.078125	//(4 NS)
#define CLK_RESOLUTION	RATE_RESOLUTION

#define MIN_EDGE_WIDTH  2.0

#define TG_MEM_RATE		0 
#define TG_MEM_STRB		1
#define TG_MEM_STRB_B	2 
#define TG_MEM_IO   	5 
#define TG_MEM_DRE      7
#define TG_MEM_DR       9
#define TG_MEM_CLK      11
#define TG_MEM_SCAN 	19

//////////////////////////////////////////
//		 TSET EDGE OPEN 			    //
//////////////////////////////////////////
#define OPEN        (-1.0)


#pragma once
class tgc
{
    byte m_nformatArr[41];

public:
    tgc() { init(); }
	~tgc() {}

    uint calcRate(double rate);
    uint calcClk(double clk);

    double m_fRateArr[16];
    int m_iCpeFine[2][2];
    void init();
    void writeTGMem(int type, int tSet, int value);

    void writeRate(int tSet, double val);
	void writeRate(double r0, double r1, double r2, double r3, double r4,
		double r5, double r6, double r7, double r8, double r9,
		double r10, double r11, double r12, double r13,
		double r14, double r15);

//  void writeECLK(int type, int tSet, double val);
//	void writeEClk(int type, double r0, double r1, double r2, double r3, double r4,
//		double r5, double r6, double r7, double r8, double r9,
//		double r10, double r11, double r12, double r13,
//		double r14, double r15);

    void writeEdge(int type, int tSet, double val = OPEN, bool bCedge = false);
    void writeStbEdge(int type, int tSet, double val = OPEN, bool bCedge = false);
    void writeEdge(int type, double e0, double e1, double e2, double e3, double e4,
		double e5,  double e6,  double e7,  double e8, double e9,
		double e10, double e11, double e12, double e13,
		double e14, double e15, bool bCedge);

    void writeFormatBuf();
    void writeFormat(uint pin, uint format);
    void writeSTRBEdge(int tsNo, double stb0, double stb1);


    void writeCpeDelay(int stb0, int stb1);

};

#endif

