#ifndef __FAILMEM__
#define __FAILMEM__

#include "define.h"

#define FM_DEPTH		2048
#define FM_MEM_SZ		32

#define COMP_MEM_SZ		16
#define FM_SCAN_SZ		256

#define MAX_IO_CNT		128 //224

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
    byte ioComp[16];    // 16 byte
    byte ioDrv[2];      //  2 byte
    byte adrDrv[5];     //  5byte
    byte scan;          //  1byte
	byte reserved[8];
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

	FM_MEM m_fmDataHArr[MAX_PG][FM_DEPTH + 10];
	FM_MEM m_fmDataLArr[MAX_PG][FM_DEPTH + 10];

	byte m_fmCompHArr[MAX_PG][FM_SCAN_SZ][COMP_MEM_SZ];
	byte m_fmCompLArr[MAX_PG][FM_SCAN_SZ][COMP_MEM_SZ];
	
	void initFailMem();

	void setFmMode(int compare = 0, int conditionCapture = 0, int fmOverContinue = 0);
	void setFmDrvMode(int mode);	//[0]address [1]pds_address

	void clearFmMemory();

	void writeScanIndex(int idx);
	int readScanIndex();

	void clearFmComp();
	void clearFmCount(int nPG = -1);

	void readFmComp(int scanIdx);
	byte* getFmCompH(int npg, int scanIdx);
	byte* getFmCompL(int npg, int scanIdx);

	void showFmDrv(int nPG, int ofs, int size);
	void showFmData(int nPG, int ofs, int size);
	void showFmComp(int scanIdx);

	int m_nCurScan;

	int m_nFmHeadPosH[MAX_PCI*MAX_PG_NUM];
	int m_nFmTailPosH[MAX_PCI*MAX_PG_NUM];
	int m_nFmStartCntH[MAX_PCI*MAX_PG_NUM];
	int m_nFmEndCntH[MAX_PCI*MAX_PG_NUM];

	int m_nFmHeadPosL[MAX_PCI*MAX_PG_NUM];
	int m_nFmTailPosL[MAX_PCI*MAX_PG_NUM];
	int m_nFmStartCntL[MAX_PCI*MAX_PG_NUM];
	int m_nFmEndCntL[MAX_PCI*MAX_PG_NUM];

	int readFmCnt();
	int readFmDataH(int nPG); 
	int readFmDataL(int nPG);
	int readFmData(int nPG, int fcRead = 0); // Force Read Count


	int readFmDataHDiag(int nPG); 
	int readFmDataLDiag(int nPG);
	int readFmDataDiag(int nPG, int fcRead = 0); // Force Read Count

	typedef void	(failmem::* pReadDut)(dut* pDut, int ofs, int cnt);
	pReadDut readDut;
	void setReadDut();
	void read16io(dut* pDut, int ofs, int cnt);
	void read8io(dut* pDut, int ofs, int cnt);
	void read4io(dut* pDut, int ofs, int cnt);
	void read1io(dut* pDut, int ofs, int cnt);
	void doDutData();
	void doDutDataAllScan();

	FM_MEM* getFmDataH(int nPG, int pos); // sel 0=sd, 1=dd
	FM_MEM* getFmDataL(int nPG, int pos); // sel 0=sd, 1=dd

	int m_nDutDataSize; 

	int m_nFmDataMode;	// Fail Data Mode  0=Data, 1=Comp, 2=Nand
	int m_nFmDrvMode;	// 0 = XYZ, 1=DRV0~47

	int m_nFmHSize;
	int m_nFmLSize;

	int m_nFmCurCntH[MAX_PG];
	int m_nFmCurCntL[MAX_PG];

	int m_nDutDataProc;	// Dut Data Process : Make File / Show Select in DATA_MODE

	int m_nFmReadSize; // tatal
};

#endif // !

