#ifndef __BIB__
#define __BIB__

#include <map>
#include <vector>
#include "String.h"
#include "define.h"
#include "Scanner.h"

//#define MAX_BLOCK_CUM	512 //512 * 32 Bit = 16384 Address
#define MAX_BLOCK_CUM	1024 //1024 * 32 Bit = 32768 Address
#define MAX_BLOCK_ADR	(MAX_BLOCK_CUM * 32)
#define MAX_INFO_BLOCK	256

enum {
	STEP_FAIL = 0,
	STEP_PASS
};
       
/// <summary>
///  Main Mode 0x0 ~ 0xFF
///  SUB Mode  0x1000 ~ 0xF000
/// </summary>
enum {
	RES_NONE				= 0x0,
	RES_FAIL_BIT			= 0x1,
	RES_CHIP_ID				= 0x2,
	RES_BLOCK_ADR			= 0X4,
	RES_BLOCK_COUNT			= 0x8,
	RES_BLOCK_COUNT_DETAIL	= 0x10,
	RES_DATA_READ			= 0x20,
	RES_BLOCK_CYCLE			= 0x40,
	RES_MEAS_DATA_READ		= 0x80,

	RES_BLOCK_ADR_IMP		= 0x1000,
	RES_BLOCK_ADR_BOTH		= 0x2000

};

 
typedef struct stResultItem {
	bool bCon;
	int nResult;   // dut sum multi chip 4 -> f, 
	char sStep[256];
}RESULT;

class dut {
public:
	dut(int i) { m_nDutNo = i;  init(); }

	~dut() {
		for (int i = 0; i < (int)m_ResArr.size(); i++)
			delete m_ResArr[i];
		m_ResArr.clear();		
	}

	void init();

	vector<int> m_IOArr;
	vector<RESULT*> m_ResArr;
	void addRes(RESULT* pRes) { m_ResArr.push_back(pRes); }

	void setResult();
	void initCurFBlks();
	


	int m_nXpos;
	int m_nYpos;
	int m_nScanNo;
	int m_nDutNo;	
	int m_nDutMask;   // OS 에서 받은  mask 

	int m_nDataMask;  // Dut Data Mask 용


	int pBaseOfs;
	byte pData[4096 * 2 * 2]; //4k depth, ddr, max 16 io process
	byte* pBaseS;
	byte* pBaseD;
	
	int m_nPrevTotal;  // SET_PDUT 사용 시 

	int m_nTotalResult;
	int m_nCurResult;
	int m_nConResult;

	cstring m_sDutText;

	uint m_nFailBit;
	
	

	vector<int> m_FCatArr;
	vector<int> m_PCatArr;
	uint m_nFCat[32];
	uint m_nPCat[32];
	uint m_nResultBin;

	cstring m_sChipID;

	//NAND 관련
	uint m_nFlashBit;
	uint m_nCurFailCount;	

	uint m_nImpLastBlock;
	bool m_bImpDone;

	int m_maskFBlkCnt;	
	int m_curFBlkCnt;
	int m_curJudgeCnt;

	int m_totalFBlkCnt;

	int m_maskFBlks[MAX_BLOCK_CUM];
	int m_curFBlks[MAX_BLOCK_CUM];
	int m_totalFBlks[MAX_BLOCK_CUM];

	// Block Fail Count MAP
	map<int, int> m_mapBlockFails;

	// Cycle Test	
	vector<int> m_nFBlkEraseArr;
	vector<int> m_nFBlkProgArr;
	
	uint m_nMaxProgBlock[MAX_BLOCK_ADR];
	uint m_nMaxEraseBlock[MAX_BLOCK_ADR];
	uint m_nSumProgBlock[MAX_BLOCK_ADR];
	uint m_nSumEraseBlock[MAX_BLOCK_ADR];
	
	FILE* m_fData;

};

class scan {
public:
	scan() {}
	~scan() { m_DutsScanArr.clear(); }
	
	int m_nScanNo;
	int m_nScanValue;
	int m_nScoketCnt;
	vector<int> m_DutsScanArr;

};


class bib
{
public:
	bib();
	~bib();

	void clearDut();
	void clearScan();

	void setBIBSize(int col, int raw, int io, int multi);
	void setBIBRow(int row, const char* dutInfo);

	void initStepItem();
	
	scan* getScan(int x);
	dut* getDut(int pos);
	dut* getDut(int x, int y);

	byte* getDutpData(int dut);

	void  initCurResult();
	void  judge(bool bShow = false);
	void  addResult(const char* sStep);

	
	vector<dut*> m_DutArr;
	scan m_ScanArr[256];
	vector<int> m_validScanVec;


	int getScanSize() { return m_nScanSize; }
	int	getDutOfScan(int scanNo, int dutIdx);
	int	getDutOfScanCnt(int scanNo);

	void setValidScan();
	bool getValidScan(int scan);

	CScanner m_clScanner;

	int m_bDDR;
	int m_nScanSize;
	int m_nCurScanNo;	
	int m_nXsz;		// Col
	int m_nYsz;		// Row
	int m_nDutCnt;
	int m_nIoCnt;
	int m_nMultiChip;

	int m_nCompData;

	uint m_nResultMode;

	bool m_bResultInvert;

};

#endif

