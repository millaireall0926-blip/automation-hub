#ifndef __PGC__
#define __PGC__


#include <map>
#include <vector>
#include "Scanner.h"
#include "String.h"
#include "STAR_TEST.h"
using namespace std;

#define MAX_MC_BYTE 64
#define PATTERN_MAX (4096)

typedef struct byte3_st
{
	byte data[3];
}byte3_st_t;


typedef union version {
	unsigned int data;
	struct {
		unsigned int rsved : 8; // reserved
		unsigned int major : 8;
		unsigned int minor : 8;
		unsigned int build : 8;
	};
}version_t;

typedef union date {
	unsigned int data[2];
	struct {
		unsigned int year : 16;
		unsigned int month : 8;
		unsigned int day : 8;
		unsigned int hour : 8;
		unsigned int min : 8;
		unsigned int sec : 8;
		unsigned int rsved : 8; // reserved
	};
}date_t;

#pragma pack(push, 1)    // 1����Ʈ ũ��� ����
typedef union fileheader
{
	unsigned int data[8];
	struct {
		version_t sw_v;  // 4byte
		version_t hw_v;  // 4byte
		date_t    date;  // 8byte
		unsigned int total_size;
		unsigned int head_size;
		unsigned int reg_size;
		unsigned int pat_size;
	};
}fileheader_t;

typedef struct registerset_test
{
	byte3_st_t idx[16];

	byte3_st_t xmax;
	byte3_st_t ymax;
	byte z_reserved; // no used
	byte3_st_t lmax;
	byte3_st_t hmax;
	byte3_st_t xh;
	byte3_st_t xt;
	byte3_st_t yh;
	byte3_st_t yt;
	byte z_reserved2; // no used
	byte3_st_t d1a;
	byte3_st_t d1b;
	byte3_st_t d1c;
	byte3_st_t d1d;
	byte3_st_t d1e;
	byte3_st_t d1f;
	byte3_st_t d1g;
	byte3_st_t d1h;
	byte3_st_t d2a;
	byte3_st_t d2b;
	byte3_st_t d2c;
	byte3_st_t d2d;
	byte3_st_t d3b;
	byte3_st_t d4b;
	byte3_st_t tph1;
	byte3_st_t tph1a;
	byte3_st_t tph1b;
	byte3_st_t tph1c;
	byte3_st_t tph1d;

	byte3_st_t tph2;
	byte3_st_t tph2a;
	byte3_st_t tph2b;
	byte3_st_t tph2c;
	byte3_st_t tph2d;

	byte3_st_t d5;
	byte3_st_t d6;
	byte3_st_t d7;
	byte3_st_t d8;

	byte3_st_t xos;
	byte3_st_t yos;

	byte3_st_t dcmr;
	byte3_st_t dcmr2;
	word dbmap;
	byte3_st_t timer;
	byte3_st_t pwait;

	word stisp;
	byte3_st_t reftimer;
	word cflg;
	byte mod;

	byte3_st_t scan;
	byte3_st_t vmpa;
	word status;

	byte3_st_t xmask;
	byte3_st_t ymask;
	byte z_reserved3; // no used

	byte3_st_t ccd;
	byte3_st_t rcd;
	byte3_st_t dsd;

	byte3_st_t DCMRH;
	byte3_st_t DCMRL;

	byte mode; // no used

	byte BL : 8;
	byte BT : 8;
	byte WL : 8;
	byte RL : 8;

	byte3_st_t xmin;
	byte3_st_t ymin;
	byte z_reserved4; // no used

	byte s1a;
	byte mloop;

	word zmax;
	word zh;
	word zmask;
	word zmin;

	//int reserved[3];
	word reserved;

}registerset_test_t;
#pragma pack(pop)   


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

	void writeReg(registerset_test_t* pReg);

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

	void writeIOMask(uint mask0_31, uint mask32_63);

};

#endif // ! __PGC__

