#ifndef __NAND__
#define __NAND__

#define MAX_BADR_BITS 32
#include "failmem.h"

enum {
	RB_MODE_ERASE = 0,
	RB_MODE_PROG =1
};

#pragma once
class nand
{
public:
	nand();
	~nand();
	

	int m_nFBLKLimit;
	int m_nFPageLimit;
	int m_nFColLimit;

	int m_nRelocatBits[32];	
	int m_blkReadCntofADR; // One Block Adr Read Count of FM

	int m_nMaskMode;
	int m_nFCountLimit;    // Block Fail Count Judge Limit

	int m_nScanCtrl;      // using Valid Scan, Ctrl Scan This Value

	void init();
	void setLimit(int col, int page, int Block);
	void setRelocatBit(int* bits, int readCount = 2);
	unsigned int relocatFBLK(int block, int wr); // wr = 0 Read, 1 = Write
	void addFBLK(int dut, int block);
	void setMask(int Mode) { m_nMaskMode = Mode; }

	// ID Check
	void judgeID(FM_MEM_NAND* pFm);
	void readID();						

	// Fail Info Block Read 
	void readInfo();			
	
	// Fail Info Block Write
	void writeInfo();	
	void fillInfoMem();

	// Fail Block Imprint
	void initImp();
	bool fillImpMem();	
	void writeImprint();			
		
	// Block Judge 
	int readBlocks();
	void judgeBlocks(FM_MEM_NAND* pFm);
	void testBlocks();					

	// Fail Count Each Block
	void initBlockFailCount();
	int readFailCount();
	void testFailCount();				
	
	// Dut, Page
	byte m_nPageSumArr[1536][4096][8]; 
	int m_nFCDBlkAdr;
	int m_bFCDAllData;     // incldue Pass or only Fail 
	int m_nFCDColLimit;    // if  1 page Fail  > limit  = Fail!
	int m_nFCDPageLimit;    // if  1 page Fail  > limit  = Fail!
	void clearFCDMem();
	void testFailCountDetail();

	// Dump File
	void makeDataFile();
	int readData(int scanNo);
	void testData();
	void closeDataFile();
	

	// Cycl Count 	
	int m_nCycleMode;   // 0 = Erase, 1 = Prg 
	int m_nCycleTestCnt;
	bool m_bRBFirst; // Prg Check Start 
	uint m_nCurRB[8];
	uint m_nPrevRB[8];	
	int m_nCycleBlock;	
	double m_fBaseRate;
	void initCycleBlock();
	void initProgRB();
	void readProgRB();
	void addCycleFBlk(FM_MEM_NAND* pFm);  // program = true, erase = false
	void findCycleFBlk(); // program = true, erase = false
	void setRB(int scan);
	void cycleTest();
};

#endif
