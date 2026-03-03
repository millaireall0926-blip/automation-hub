#ifndef __PGC__
#define __PGC__

#include <map>
#include "String.h"

struct registerset_test;

#pragma once
class pgc
{
public:
	pgc() {
		m_nWritePC = 0; m_nDefualtPC = 0;
	}
	~pgc() {}

	map<cstring, int> m_mapLabelPC;
	int m_nDefualtPC;
	int m_nWritePC;

	cstring m_sPatternName;
	cstring m_sRunPC;
	int m_nRunPC;

	void writePattern(const char* file);

	void runPG();
	void stopPG();

    void writeReg(registerset_test* pReg);

    void setPC(const char* spc);
	void setPC(int npc);

    void setJnc(unsigned int jnc);

	bool isRun();
	bool isSync();
	unsigned int getSyncID();
	void nextSync();

	void writeScan(int idx, unsigned int value);
	void writeAScan(int idx, unsigned int value);
	unsigned int readScan(int idx);

	void setReg(int idx, uint value);
	void setTimer(int sel, double dTime);

	void writeIOMask(uint mask0_31, uint mask32_63, uint mask64_95, uint mask96_127);
};

class pds 
{
public: 
	pds() { 
		memset(m_nSCOption, 0, sizeof(m_nSCOption)); 
		memset(m_nASCOption, 0, sizeof(m_nASCOption));
	}
	~pds() {}

	CScanner m_Scanner;
	
	enum {
		PDS_ADDR = 1, PDS_CLK = 0x2 , PDS_IO = 0x4 , 
		PDS_SCAN = 0x8, PDS_STRB = 0x10, PDS_DRE = 0x20
	};

	int m_nSCOption[16];
	int m_nASCOption[16];

	int getNum(char* p, int &num);
	int getSource(cstring str, int dest, int& value, int& SCOption);
	void setPDS(int dest, const char* source);
	void writeSCOption();


};

class scramble 
{
public:
	scramble() {}
	~scramble() {}

	CScanner m_Scanner;
	uint	 m_nDestAdr;
	uint	 m_nSrcArr[4]; // source 
	uint	 m_nLogArr[3]; // loginc

	int getLogic(cstring logic);
	int getReg(cstring reg, int& num, int sel = 0); // sel 0=Source, 1=Dest
	enum {
		REG_X = 0, REG_Y,  REG_D, REG_NONE
	};
	int setDest();
	
	
	void enableScr(bool bEnable, int type);
	void setScr(const char* format);
	void writeScr(bool invert, int count); 
};

#endif

