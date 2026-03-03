#ifndef __FAILMEM__
#define __FAILMEM__

#include "define.h"
#include "bib.h"

#define FM_MAX_CNT		65536
#define FM_DEPTH		2048
#define FM_MEM_SZ		32

#define COMP_MEM_SZ		8
#define FM_SCAN_SZ		256

#define MAX_IO_CNT		64

enum {
	FM_DATA = 0,
	FM_COMP = 1,
	FM_NAND = 2,
	FM_STB_ALWAYS = 0,
	FM_STB_FAIL = 1
};

enum {
	FM_CNT_STABLE = 0,
	FM_CNT_ACTIVE = 1,

	FM_CNT_NO_CLEAR = 0,
	FM_CNT_CLEAR = 1
};


typedef struct {
    byte ioComp[8];     // 8byte
    byte ioDrv[2];      // 2byte
    byte adrDrv[5];     // 5byte
    byte scan;          // 1byte
	byte reserved[16];
}FM_MEM;

typedef struct {
	byte dutCum;		// 1byte (8io bits )
	byte blkAdr[3];     // 2byte
	byte dummy[4];		// 5byte
	byte ioDrv[2];      // 2byte
	byte adrDrv[5];		// 5byte
	byte scan;          // 1byte
	byte reserved[16];

}FM_MEM_NAND;


class failmem
{
public:
	failmem();
	~failmem(); 

	FM_MEM m_fmDataHArr[FM_DEPTH + 10];
	FM_MEM m_fmDataLArr[FM_DEPTH + 10];

	byte m_fmCompHArr[FM_SCAN_SZ][COMP_MEM_SZ];
	byte m_fmCompLArr[FM_SCAN_SZ][COMP_MEM_SZ];


	void initFailMem();

	//int compare			=  [1:0]	2'b00 strb_dq, 2'b01 fail_dq, 2'b11:nand
	//int conditionCapture	=  [2:3]	2'b00 always  2'b01 fail, 2'b11 nand
	//int fmOverContinue	=  [0]		0: normal 1 : stop
	// default dataTrough	
	void setFmMode(int compare = 0, int conditionCapture = 0);
	void setFmDrvMode(int mode);	//[0]address [1]pds_address
	void setFmOverStop(bool overStop);


	void clearFmMemory();
	void clearFmComp();
	void clearFmCount();

	void showFmDrv(int ofs, int size);
	void showFmData(int ofs, int size);
	void showFmComp(int scanIdx);

	void writeScanIndex(int idx);
	int readScanIndex();

	void readFmComp(int scanIdx);
	byte* getFmCompH(int scanIdx);
	byte* getFmCompL(int scanIdx);

	int getFmReadCnt();

	int readFmCnt();
	int readFmDataH(); 
	int readFmDataL();
	int readFmData(int fcRead = 0); // Force Read Count


	typedef void	(failmem::* pReadDut)(dut* pDut, int ofs, int cnt);
	pReadDut readDut;

	void setReadDut();
	void read16io(dut* pDut, int ofs, int cnt);
	void read8io(dut* pDut, int ofs, int cnt);
	void read4io(dut* pDut, int ofs, int cnt);
	void read1io(dut* pDut, int ofs, int cnt);
	void doDutData();
	void doDutDataAllScan();

	FM_MEM* getFmDataH(int pos); // sel 0=sd, 1=dd
	FM_MEM* getFmDataL(int pos); // sel 0=sd, 1=dd

	void clearFCount();
	void readFCountOfDut(int sc);

	
	

	int m_nDutDataProc;	// Dut Data Process : Make File / Show Select in DATA_MODE
						// 0 = None, 1 = File, 2 = Only Show
	int m_nDutDataSize; 

	int m_nFmDataMode;	// Fail Data Mode  0=Data, 1=Comp, 2=Nand
	int m_nFmDrvMode;	// 0 = XYZ, 1=DRV0~47
	int m_nCurScan;

	int m_nFmHeadPosH;
	int m_nFmTailPosH;
	int m_nFmStartCntH;
	int m_nFmEndCntH;

	int m_nFmHeadPosL;
	int m_nFmTailPosL;
	int m_nFmStartCntL;
	int m_nFmEndCntL;


	int m_nFmHSize;
	int m_nFmLSize;

	int m_nFmCurCntH;
	int m_nFmCurCntL;

	int m_nFmReadSize; // tatal

};

#endif // !

