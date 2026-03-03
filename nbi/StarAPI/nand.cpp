#include "nand.h"
#include "bib.h"
#include "operation.h"
#include "ioc.h"
#include "starAPI.h"
#include "pgc.h"
#include "result.h"
#include "../starlib/starlib_dif.h"
#include <algorithm>

extern failmem		g_clFm;
extern bib			g_clBib;
extern operation	g_clOperation;		//  gOPER
extern ioc			g_clIO;
extern starAPI		g_clAPI;
extern pgc			g_clPgc;
extern result		g_clRes;
extern starlib		g_clLib;

#define DATA_FILE_PATH "/star/result/data"


nand::nand()
{
	init();
}

nand::~nand()
{

}

void nand::init()
{
	
	
	m_blkReadCntofADR = 2;
	int defaultBits[] =
	{ 0, 1, 2 ,  3,  4,  5,  6,  7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	  8, 9, 10, 11, 12, 13, 14, 15, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	
	memcpy(m_nRelocatBits, defaultBits, sizeof(int) * MAX_BADR_BITS);

}

void nand::setLimit(int col, int page, int block)
{
	m_nFColLimit = col;
	m_nFPageLimit = page;
	m_nFBLKLimit = block;
	m_nMaskMode = TOTAL | MASK; // default ALL
	m_nFCountLimit = 0;


	uint adr = (1 << 9) | 35;
	gIO.writePG(adr, col, 2);
	adr = (1 << 9) | 37;
	gIO.writePG(adr, page, 2);
}

void nand::setRelocatBit(int* bits, int readCount)
{
	memcpy(m_nRelocatBits, bits, sizeof(int) * MAX_BADR_BITS);
	m_blkReadCntofADR = readCount;

}

uint nand::relocatFBLK(int block, int wr)  // wr = 0 Read, 1 = Write
{ 
	int relocat = 0;
	for (int i = 0; i < MAX_BADR_BITS; i++)
	{
		int bitPos = m_nRelocatBits[i];
		if (bitPos == 0xffff)
			continue;

		if (bitPos >= 16)			// block address is word unit
			continue;

		if (wr == 0) {
			if (block & (1 << i))
				relocat |= (1 << m_nRelocatBits[i]);
		}
		else
		{
			if (block & (1 << bitPos))
				relocat |= 1 << i;
		}
	}
	return relocat;
}

void nand::addFBLK(int dno, int block)
{	
	if (block >= MAX_BLOCK_ADR) return;//block = MAX_BLOCK_ADR - 1;
	int hAdr = block / 32;
	int lAdr = block % 32;
	dut* pDut = gBIB.getDut(dno);


	if (!(pDut->m_curFBlks[hAdr] & (1 << lAdr))) {
		pDut->m_curFBlkCnt++;
	}
	pDut->m_curFBlks[hAdr] |= (1 << lAdr);

	if (m_nMaskMode & MASK) {
		pDut->m_curJudgeCnt = 0;
	}
	else {
		if (!(pDut->m_maskFBlks[hAdr] & (1 << lAdr))) 
			pDut->m_curJudgeCnt++;			
	}


	if (pDut->m_curJudgeCnt > m_nFBLKLimit)
		pDut->m_nCurResult = STEP_FAIL;

//	printf("%s D[%d] adr %d hAdr %d lAdr %d mask %d, cur %d, CurCnt %d maskCnt %d\n",
//		__FUNCTION__, dno, block, hAdr, lAdr,
//		(pDut->m_maskFBlks[hAdr] & (1 << lAdr)),
//		(pDut->m_curFBlks[hAdr] & (1 << lAdr)),
//		pDut->m_curFBlkCnt, pDut->m_maskFBlkCnt);


}

void nand::judgeID(FM_MEM_NAND* pFm)
{
	int scan = pFm->scan;

	for (int j = 0; j < gBIB.getDutOfScanCnt(scan); j++)
	{
		int dno = gBIB.getDutOfScan(scan, j);
		dut* pDut = gBIB.getDut(dno);

		if (pDut->m_nFlashBit & pFm->dutCum) {
			pDut->m_nCurResult = STEP_FAIL;
		}

		pDut->setResult();
	}
}

void nand::readID()							// ID Check
{	
	int readCnt = gFM.readFmData();
	gFM.showFmData(0, readCnt);
//	printf("%s - FM Read : %d\n",__FUNCTION__, readCnt);
	 
	for (int i = 0; i < readCnt; i++)
	{
		FM_MEM_NAND* pfm = (FM_MEM_NAND*)gFM.getFmDataH(i);
		FM_MEM_NAND* pfmL = (FM_MEM_NAND*)gFM.getFmDataL(i);
		if (i < gFM.m_nFmHSize) judgeID(pfm);
		if (gBIB.m_bDDR) {
			if (i < gFM.m_nFmLSize) judgeID(pfmL);
		}

	}

}

void nand::readInfo()		// Fail Info Block Read 
{

	int scansz = gBIB.m_validScanVec.size();
	if (gAPI.m_nTestMode != TEST_MODE_CONTACT)
		if (scansz == 0)return;
		
	gFM.writeScanIndex(gBIB.m_validScanVec[0]);
	gPG.runPG();
	
	
	for (int i = 0; i < scansz; )
	{
		int ret = gOPER.waitTest(SLEEP_10US);
		if (ret == SYNC_END) break;
		if (ret == SYNC_DATA) { gPG.nextSync(); continue; }

		// do info read
		int blkCnts[16];
		int max = 0;
		bool useRelockAll = false;
		if (gBIB.m_nIoCnt == 16)useRelockAll = true;
		word size = 0;
		uint read = 0;
		gFM.readFmData();
		int rs = gFM.readScanIndex();
		scan* pscan = gBIB.getScan(rs);

		for (size_t t = 0; t < pscan->m_DutsScanArr.size(); t++)
		{
			int dno = pscan->m_DutsScanArr[t];
			if (gBIB.getDut(dno)->m_nConResult == STEP_FAIL)continue;
			byte* pData = gBIB.getDutpData(dno);
			read = 0;
			for (int i = 0; i < m_blkReadCntofADR; i++)
			{
				read |= (*pData << (i * 16));
				pData++;
				if (useRelockAll) {
					read |= (*pData << ((i * 16) + 8));
					pData++;
				}
			}
			size = relocatFBLK(read, 0);
			if (size > max)
				max = size;

			blkCnts[t] = size;
			if (blkCnts[t] > MAX_INFO_BLOCK)blkCnts[t] = MAX_INFO_BLOCK;
		}

		for (size_t t = 0; t < pscan->m_DutsScanArr.size(); t++)
		{
			int dno = pscan->m_DutsScanArr[t];
			dut* pDut = gBIB.getDut(dno);
			if (pDut->m_nConResult == STEP_FAIL)continue;
			byte* pData = gBIB.getDutpData(dno);
			pData += (gBIB.m_nIoCnt / 8) * m_blkReadCntofADR;
			int blkCount = blkCnts[t];
			printf("D[%d] Info Block Count : %d\n", t, blkCount);

			for (int blkIdx = 0; blkIdx < blkCount; blkIdx++)
			{
				read = 0;
				for (int i = 0; i < m_blkReadCntofADR; i++)
				{
					read |= (*pData << (i * 16));
					pData++;
					if (useRelockAll) {
						read |= (*pData << ((i * 16) + 8));
						pData++;
					}
				}
				int blockAdr = relocatFBLK(read, 0);
				if (blockAdr == 0xFFFF)continue;
				addFBLK(t, blockAdr); 
			}
//			printf("D[%d] C %d L %d R %d\n",
//				pDut->m_curFBlkCnt, m_nFBLKLimit, pDut->m_nCurResult);
			pDut->setResult();
			
		}

		i++;

		if (i >= scansz)break;
		gFM.writeScanIndex( gBIB.m_validScanVec[i]);
		gPG.nextSync();

	}

	gPG.stopPG();
}

void nand::fillInfoMem()
{
	int scanIdx = m_nScanCtrl;
	if (scanIdx >= gBIB.m_nScanSize)  return;
	byte fmArr[2][MAX_INFO_BLOCK * 4][16];

	bool useRelockAll = false;
	word addr = 0;
	uint rd = 0;

	if (gBIB.m_nIoCnt == 16)useRelockAll = true;

	memset(fmArr, 0xff, sizeof(fmArr));


	scan* pScan = gBIB.getScan(scanIdx);
	for (size_t t = 0; t < pScan->m_DutsScanArr.size(); t++)
	{
		int dno = pScan->m_DutsScanArr[t];
		dut* pDut = gBIB.getDut(dno);

		if (pDut->m_nConResult == STEP_FAIL)continue;

		int blks = pDut->m_totalFBlkCnt;

		if (blks <= 0 || blks >= 0xff)continue;
		uint d = blks;
		d |= d << 8;
		d = relocatFBLK(d, 1);
		
		int ioStart = pDut->m_IOArr[0] % MAX_IO_CNT;
		int ioPos = ((ioStart % (MAX_IO_CNT / 2)) / 8) * 2; // *2 -> becass for using Sequntial FM H/L 

		int fmSel = 0;
		if (ioStart >= 32) {
			fmSel = 1;
		}

		int blkno = 0;
		for (int i = 0; i < m_blkReadCntofADR; i++) {
			fmArr[fmSel][blkno][ioPos] = (d >> (i * 16)) & 0xff;

			if (useRelockAll) {
				fmArr[fmSel][blkno][ioPos + 1] = (d >> (i * 16 + 8)) & 0xff;
				fmArr[fmSel][blkno][ioPos + 2] = (d >> (i * 16)) & 0xff;
				fmArr[fmSel][blkno][ioPos + 3] = (d >> (i * 16 + 8)) & 0xff;
			}
			blkno++;
		}

		for (int fblkno = 0; fblkno < MAX_BLOCK_ADR; fblkno++)
		{
			bool bFind = false;
			if (m_nMaskMode & MASK)
				if (pDut->m_maskFBlks[fblkno / 32] & (1 << (fblkno % 32)))
					bFind = true;

			if (m_nMaskMode & TOTAL)
				if (pDut->m_totalFBlks[fblkno / 32] & (1 << (fblkno % 32)))
					bFind = true;
			if (!bFind) continue;

			if (blkno >= MAX_INFO_BLOCK * 4)break;
			d = relocatFBLK(fblkno, 1);

			for (int i = 0; i < m_blkReadCntofADR; i++) {
				fmArr[fmSel][blkno][ioPos] = (d >> (i * 16)) & 0xff;				
				if (useRelockAll) {
					fmArr[fmSel][blkno][ioPos + 1] = (d >> (i * 16 + 8)) & 0xff;
					fmArr[fmSel][blkno][ioPos + 2] = (d >> (i * 16)) & 0xff;
					fmArr[fmSel][blkno][ioPos + 3] = (d >> (i * 16 + 8)) & 0xff;
				}
				blkno++;
			}
		}
	}

	uint adr = 4 << 17;
	gIO.writePGData(adr, &fmArr[0][0][0], FM_MEM_SZ * MAX_INFO_BLOCK * 4);
	
	adr = 5 << 17;
	gIO.writePGData(adr, &fmArr[1][0][0], FM_MEM_SZ * MAX_INFO_BLOCK * 4);
	

	//	if (scanIdx == gBIB.m_nScanSize - 1)
	//	{
	//		for (int i = 0; i < 20; i++)
	//		{
	//			for (int j = 0; j < 16; j = j+2)
	//			{
	//				printf("%d %d : %02x%02x | %02x%02x\n",i, j, fmArr[0][i][j], fmArr[0][i][j+1], 
	//					fmArr[1][i][j], fmArr[1][i][j + 1]);
	//			}
	//			printf("\n");
	//		}
	//	}

	
}


void nand::writeInfo()					// Fail Info Block Write
{
	int scansz = gBIB.m_validScanVec.size();
	if (gAPI.m_nTestMode != TEST_MODE_CONTACT)
		if (scansz == 0)return;

	gFM.writeScanIndex( gBIB.m_validScanVec[0]);	
	m_nScanCtrl = gBIB.m_validScanVec[0];
	fillInfoMem();
	gPG.runPG();
	
	
//	printf("%s 1.readScan %d\n", __FUNCTION__, gFM.readScanIndex());

	for (int i = 0; i < scansz; )
	{		
		int ret = gOPER.waitTest(SLEEP_10US);
		if (ret == SYNC_END) break;
		if (ret == SYNC_DATA) { gPG.nextSync(); continue; }

		i++;
		if (i >= scansz)break;

		gFM.writeScanIndex( gBIB.m_validScanVec[i]);
		m_nScanCtrl = gBIB.m_validScanVec[i];
		fillInfoMem();
		printf("%s 2.readScan %d\n", __FUNCTION__, gFM.readScanIndex());
		gPG.nextSync();
	}

	
	gPG.stopPG();
}

void nand::initImp()
{
	for (int i = 0; i < gBIB.m_nDutCnt; i++)
	{
		if (gBIB.getDut(i)->m_nConResult == STEP_FAIL) continue;
		gBIB.getDut(i)->m_nImpLastBlock = 0;
		gBIB.getDut(i)->m_bImpDone = false;
	}
	
}

bool nand::fillImpMem()
{	
	int scanIdx = m_nScanCtrl;//gFM.readScanIndex();		
	if (scanIdx >= gBIB.m_nScanSize)  return true;
	byte fmArr[2][FM_DEPTH][FM_MEM_SZ];
//	printf("CUR Scan %d\n", scanIdx);
	bool twoByte = false;
	word addr = 0;
	uint rd = 0;

	if (gBIB.m_nIoCnt == 16)twoByte = true;

	memset(fmArr, 0xff, sizeof(fmArr));
	int maxWcnt = 0;
	
	scan* pScan = gBIB.getScan(scanIdx);
	for (size_t t = 0; t < pScan->m_DutsScanArr.size(); t++)
	{
		int dno = pScan->m_DutsScanArr[t];
		dut* pDut = gBIB.getDut(dno);
//		printf("dno %d -> con %d, ImpDone %d\n", dno, pDut->m_nConResult, pDut->m_bImpDone);
		if (pDut->m_nConResult == STEP_FAIL) continue;
		if (pDut->m_maskFBlkCnt == 0 && pDut->m_totalFBlkCnt == 0) pDut->m_bImpDone = true;
		if (pDut->m_bImpDone) continue; // finished Imp Block
		int mCnt = 0; // Mask Cnt
		int tCnt = 0; // Cur  Cnt
		int wCnt = 0; // Write Cnt

		

		int ioStart = pDut->m_IOArr[0] % MAX_IO_CNT;
		int ioPos = ((ioStart % MAX_IO_CNT) / 8) * 2;
//		int ioPos = ((ioStart % (MAX_IO_CNT/2)) / 8) * 2;  // *2 -> becass for using Sequntial FM H/L 
		
		int fmSel = 0;
//		if (ioStart >= 32) {
//			fmSel = 1;
//		}
				
		int fblk;
		for (fblk = pDut->m_nImpLastBlock; fblk < MAX_BLOCK_ADR; fblk++)
		{
			bool bFind = false;
			if (m_nMaskMode & MASK) 
				if (pDut->m_maskFBlks[fblk / 32] & (1 << (fblk % 32)))
				{	bFind = true; mCnt++; }
				

			if (m_nMaskMode & TOTAL) 
				if (pDut->m_totalFBlks[fblk / 32] & (1 << (fblk % 32)))
				{	bFind = true; tCnt++; }

			if (!bFind) continue;

			
			fmArr[fmSel][wCnt][ioPos] = fblk & 0xff;
			fmArr[fmSel][wCnt][ioPos + 1] = (fblk >> 8) & 0xff;

			if (twoByte) {
				fmArr[fmSel][wCnt][ioPos + 2] = fblk & 0xff;
				fmArr[fmSel][wCnt][ioPos + 3] = (fblk >> 8) & 0xff;
			}
			wCnt++;

			bool bDone = true;
			if (m_nMaskMode & MASK)
				if (mCnt < pDut->m_maskFBlkCnt) bDone = false;
			if (m_nMaskMode & TOTAL)
				if (tCnt < pDut->m_totalFBlkCnt) bDone = false;

//			printf("dut[%d] fblk %x", dno, fblk);
//			if (m_nMaskMode & MASK) printf(" Mcnt %d", mCnt);
//			if (m_nMaskMode & TOTAL) printf(" Tcnt %d", tCnt);
//			printf(" wcnt %d", wCnt);
//			printf(" [%d][%d][%d] = 0x%x", fmSel, wCnt, ioPos, fblk);
//			printf("\n");

			if (bDone) {
				pDut->m_bImpDone = true;				
				if (wCnt > maxWcnt) maxWcnt = wCnt;
				break;
			}

			if (wCnt >= (FM_DEPTH))break;
			
		
		}

		if (wCnt == 0) pDut->m_bImpDone = true; // no fial block
		pDut->m_nImpLastBlock = fblk;

		// If Finish Writting Fail Block, 
		// Fill Last Fail Block Adr to Remind FM Space		
		for (int idx = wCnt; idx < FM_DEPTH; idx++)
		{
			fmArr[fmSel][idx][ioPos] = fblk & 0xff;
			fmArr[fmSel][idx][ioPos + 1] = (fblk >> 8) & 0xff;

			if (twoByte) {
				fmArr[fmSel][idx][ioPos + 2] = fblk & 0xff;
				fmArr[fmSel][idx][ioPos + 3] = (fblk >> 8) & 0xff;
			}
		}
	}
	
//	memcpy(fmArr[1], fmArr[0], FM_MEM_SZ * FM_DEPTH);

	uint adr = 4 << 17;

	for (int i = 0; i < FM_DEPTH; i++)
		gIO.writePGData(adr + (i * (1 << 5)), &fmArr[0][i][0], FM_MEM_SZ / 2);

//	adr = 5 << 17;
//	gIO.writePGData(adr, &fmArr[1][0][0], FM_MEM_SZ * FM_DEPTH);	
	
	if (maxWcnt > 0) {
		printf("------------ FBLK Check ----------\n");
		gFM.readFmData(maxWcnt);
		gFM.showFmData(0, maxWcnt);
		printf("----------------------------------\n");
	}

	for (size_t t = 0; t < pScan->m_DutsScanArr.size(); t++)
	{
		int dno = pScan->m_DutsScanArr[t];
		dut* pDut = gBIB.getDut(dno);

		if (pDut->m_nConResult == STEP_FAIL) continue;
		if (!pDut->m_bImpDone){
			printf("dut %d false\n", dno);
			return false;
		}
	}

	return true;

}


void nand::writeImprint()					// Fail Block Imprint
{
	int scansz = gBIB.m_validScanVec.size();
	if (gAPI.m_nTestMode != TEST_MODE_CONTACT)
		if (scansz == 0)return;
	
	initImp();
	// Imp Mode On
//	gIO.writePG(1 << 9 | 34, 1, 1);
//	gFM.writeScanIndex(gBIB.m_validScanVec[0]);
	gFM.writeScanIndex( gBIB.m_validScanVec[0]);	
	m_nScanCtrl = gBIB.m_validScanVec[0];
	printf("Start writeImprint SC %d\n", m_nScanCtrl);
	bool finish = fillImpMem();
//	printf("Run\n");

	gPG.runPG();

//	printf("%s 1.readScan %d\n", __FUNCTION__, gFM.readScanIndex());

	for (int i = 0; i < scansz; )
	{
		
		int ret = gOPER.waitTest(SLEEP_10US);		
		if (ret == SYNC_END) break;  		
		if (ret == SYNC_DATA) { gPG.nextSync(); continue; }
		
		
		printf("OUT CMD : %02x scan %d\n", ret, gFM.readScanIndex() );
		// SYNC_DATA , SYNC_SCAN		 
		if (!finish) { 
			finish = fillImpMem();
			gPG.nextSync();
			continue;
		}
		else {			
			i++;
			printf("Out Scan Change %d (%d/%d) \n", gBIB.m_validScanVec[i], i, scansz);
			if (i >= scansz)break;						
			gFM.writeScanIndex( gBIB.m_validScanVec[i]);			
			m_nScanCtrl = gBIB.m_validScanVec[i];
			finish = fillImpMem();
			gPG.nextSync();
		}
	}
//	printf("Finish writeImprint\n");
	gPG.stopPG();
//	gIO.writePG(1 << 9 | 34, 1, 0);
}





void nand::judgeBlocks(FM_MEM_NAND* pFm)
{
	uint block = pFm->blkAdr[0] | pFm->blkAdr[1] << 8 | pFm->blkAdr[2] << 16;
	int scanIdx = pFm->scan;

	scan* pScan = gBIB.getScan(scanIdx);
//	printf("rsc %d fsc %d\n", gFM.readScanIndex(), scanIdx);
	for (int j = 0; j < (int)pScan->m_DutsScanArr.size(); j++)
	{
		
		int dutNo = pScan->m_DutsScanArr[j];
		dut* pDut = gBIB.getDut(dutNo);
		if (pDut->m_nConResult == STEP_FAIL) continue;
		if (pFm->dutCum & pDut->m_nFlashBit) {
//			printf("Fsc %d dno %d : adr %04x, read %04x, Comp %04x\n"
//				,scanIdx, dutNo, block, pFm->dutCum, pDut->m_nFlashBit);			
			addFBLK(dutNo, block);
		}
		
		pDut->setResult();
	}
}

int nand::readBlocks()
{
	int rCnt = gFM.readFmData();
	for (int i = 0; i < rCnt; i++)
	{
		FM_MEM_NAND* pFm = (FM_MEM_NAND*)gFM.getFmDataH(i);
		FM_MEM_NAND* pFmL = (FM_MEM_NAND*)gFM.getFmDataL(i);
		if (i < gFM.m_nFmHSize) judgeBlocks(pFm);
		if (gBIB.m_bDDR) {
			if (i < gFM.m_nFmLSize) judgeBlocks(pFmL);
		}
	}
	return rCnt;
}

void nand::testBlocks()					// Block Judge 
{

	gPG.runPG();
	
	while (1) {

		readBlocks();
//		if (rCnt != 0)	SHOW_FMDATA(0, rCnt);

		if (gOPER.checkStat() == 0) {
			readBlocks();
			break;
		}
		int sync = gOPER.checkSync();
		if (sync == SYNC_DATA || sync == SYNC_SCAN) {
//			printf("OUT : CMD %04x\n", sync);
			readBlocks();
			gPG.nextSync();
		}

		gOPER.readDPS();
		gOPER.readSysTemp();
		gOPER.doScanProc();
		 
		usleep(1);
	}
	

	gPG.stopPG();
}

void nand::initBlockFailCount()
{
	for (int i = 0; i < gBIB.m_nDutCnt; i++)
	{
		if (gBIB.getDut(i)->m_nConResult == STEP_FAIL) continue;
		gBIB.getDut(i)->m_mapBlockFails.clear();
	}
	memset(m_nPageSumArr, 0, sizeof(m_nPageSumArr));
}

int nand::readFailCount()
{
	int rCnt = gFM.readFmData();
	if (rCnt > 0)
	{
		FM_MEM_NAND* pFm = (FM_MEM_NAND*)gFM.getFmDataH(0);
		int scno = pFm->scan;
		scan * pscan = gBIB.getScan(scno);
		int rdsc = gFM.readScanIndex();
		uint blkAdr = pFm->blkAdr[0] | pFm->blkAdr[1] << 8 | pFm->blkAdr[2] << 16;

		gFM.readFCountOfDut(scno);
		for (int j = 0; j < (int)pscan->m_DutsScanArr.size(); j++)
		{
			int dutNo = pscan->m_DutsScanArr[j];
			dut* pDut = gBIB.getDut(dutNo);
			if (pDut->m_nConResult == STEP_FAIL) continue;
			pDut->m_mapBlockFails[blkAdr] += pDut->m_nCurFailCount;
			if (pDut->m_mapBlockFails[blkAdr] > m_nFCountLimit)
				addFBLK(dutNo, blkAdr);
			pDut->setResult();
//			printf("Test blk %d : dno %d, fcnt-%d blks-%d rdSC %d blkSC %d\n", 
//				blkAdr, pDut->m_nDutNo, pDut->m_mapBlockFails[blkAdr], pDut->m_mapBlockFails.size(),
//				rdsc, pFm->scan);			
		}
	

	}

	return rCnt;
}

void nand::testFailCount()					// Fail Count Each Block
{	
	int scansz = gBIB.m_validScanVec.size();
	if (gAPI.m_nTestMode != TEST_MODE_CONTACT)
		if (scansz == 0)return;
	initBlockFailCount();
	int scanIdx = 0;

	gFM.writeScanIndex( gBIB.m_validScanVec[0]);
	gPG.runPG();	

	
	while (1)
	{
		readFailCount();

		if (gOPER.checkStat() == 0) {
			readFailCount();
			break;
		}
		int sync = gOPER.checkSync();
		if (sync != SYNC_NONE) {
			readFailCount();
			if (sync == SYNC_DATA) {
				gPG.nextSync();
			}
			else if (sync == SYNC_SCAN) {
				scanIdx++;
				printf("Out Scan Change %d (%d/%d) \n", gBIB.m_validScanVec[scanIdx], scanIdx, scansz);
				if (scanIdx >= scansz)break;				
				gFM.writeScanIndex( gBIB.m_validScanVec[scanIdx]);
				gPG.nextSync();
			}
		}

		gOPER.readDPS();
		gOPER.readSysTemp();
		gOPER.doScanProc();		
	
//		usleep(1);
	}

	gPG.stopPG();
}

//<sub type="fc-d">
//	<d n="0">
//		<b a="11">
//		<p n="0"> 11, 22, 33, 44, ... </p>
//		...
//		<p n="3890">  ... </p>
//		</b>
//	</d>
//</sub>

void nand::clearFCDMem()
{
	byte pData[3096 * 8];
	memset(pData, 0, 3096 * 8);

//	for (int i = 0; i < 3096 * 8; i++)
//	{
//		pData[i] = i & 0xff;
//	}
//	printf("[BUF] adr 0x%06x size %d", (1 << 20) | (2 << 18), 1024);
//	for (int i = 0; i < 1024; i++)
//	{
//		if (i % 16 == 0)
//			printf("\n%06x : ", i);//((1 << 20) | (2 << 18)) + i);
//		printf("%02x ", *(pData + i));
//	}
//	printf("\n");
	
	for (int i = 0; i < 8; i++)
	{		
		gIO.writePGData((1 << 20) | (2 << 18) | ((i % 8) << 15), pData, 3096 * 8);
	}
	
	
}

void nand::testFailCountDetail()
{
	int scansz = gBIB.m_validScanVec.size();
	if (gAPI.m_nTestMode != TEST_MODE_CONTACT)
		if (scansz == 0)return;


	bool noFree = false;
	gRES.initSubResult(noFree);
	HND doc = gRES.m_hSubResult;
	HND hSub = gLIB.xml_path_handle(doc, "/sub");
	HND fh = gLIB.xml_create("fc-d");

	initBlockFailCount();


	byte* pData = new byte[3096 * 8];
	byte pCmp[8] = { 0 , };
	memset(pCmp, 0, 8);
	clearFCDMem();

	for (int i = 0; i < scansz; i++)
		printf("ValidScan %d / %d\n", gBIB.m_validScanVec[i], i);

	printf("Write Scan %d\n", gBIB.m_validScanVec[0]);
	gFM.writeScanIndex(gBIB.m_validScanVec[0]);
	printf("Before Run Read Scan %d\n", gFM.readScanIndex());
	gPG.runPG();
	printf("After Run Read Scan %d\n", gFM.readScanIndex());
	
	for (int i = 0; i < scansz; )
	{
		int ret = gOPER.waitTest(SLEEP_1US);
		printf("OUT CMD % x\n", ret);
		if (ret == SYNC_END) break;
		else if (ret == SYNC_DATA)  gPG.nextSync(); 	// Next Block
		else if (ret == SYNC_SCAN) {

			int scanIdx = gFM.readScanIndex();
			scan* pScan = gBIB.getScan(scanIdx);
			printf("Read Scan %d\n", scanIdx);
			for (int j = 0; j < (int)pScan->m_DutsScanArr.size(); j++)
			{
				int dutNo = pScan->m_DutsScanArr[j];
				printf("dut %d\n", dutNo);
				dut* pDut = gBIB.getDut(dutNo);
				if (pDut->m_nConResult == STEP_FAIL) continue;

				cstring str;
				bool dutAdd = false;
				//				str.Format("/d.n=%d", dutNo);
				HND dh = gLIB.xml_path_handle(fh, str.c_str());  // Dut
				if (dh == 0) {
					dh = gLIB.xml_create("d"); 
					XAI(dh, "n", dutNo);
					dutAdd = true;
					//					printf("Make %s \n", str.c_str());
				}

				bool blkAdd = false;
				str.Format("/b.a=%d", m_nFCDBlkAdr);
				HND bh = gLIB.xml_path_handle(dh, str); // Block
				if (bh == 0) {
					bh = gLIB.xml_create("b");
					XAI(bh, "a", m_nFCDBlkAdr);
					XAI(bh, "cl", m_nFCDColLimit);
					XAI(bh, "pl", m_nFCDPageLimit);
					blkAdd = true;
					//					printf("Make %s \n", str.c_str());
				}
				uint adr = (1 << 20) | (2 << 18) | ((pDut->m_IOArr[0] / 8) << 15);
				gIO.readPGData(adr, pData, 3096 * 8);
				int showsz = 1024;
//				printf("[READ] adr : 0x%06x, size : %d / (3096 * 8)", adr, showsz);
//				for (int i = 0; i < showsz; i++)
//				{
//					if (i % 16 == 0)
//						printf("\n%06x : ", adr + i);
//					printf("%02x ", *(pData + i));
//				}
//				printf("\n"); 
				int pageFailCount = 0;
				for (int page = 0; page < 3096; page++)
				{
					byte* src = pData + (page * 8);
					if (memcmp(pCmp, src, 8) == 0)continue;

					str = ""; str.Format("");
					int pageSum = 0;
					bool bAllDataCheck = false; 
					for (int col = 0; col < 8; col++) {
						int colCnt = *(src + col);
						if (m_bFCDAllData) {							
							if (colCnt > m_nFCountLimit) bAllDataCheck = true;
						}
						else
						{
							colCnt -= m_nFCDColLimit;
							if (colCnt < 0)colCnt = 0;
						}

						str.AppendFormat("%d,", colCnt);
						pageSum += colCnt;
						m_nPageSumArr[dutNo][page][col] = colCnt & 0xff;
					}

					if (pageSum == 0)continue;
					if (m_bFCDAllData) {
						if (bAllDataCheck) pageFailCount++;
					}
					else
					{
						pageFailCount++;
					}

					HND ph = gLIB.xml_create("p");
					XAI(ph, "n", page);

					if (pageSum != 0)XAI(ph, "c", pageSum);
					//m_nPageSumArr[dutNo][page] += pageSum;

					XTEXT(ph, str.c_str());
					gLIB.xml_add(bh, ph);
					gLIB.xml_destroy(ph);
				}

				if (blkAdd) {
					gLIB.xml_add(dh, bh);
					gLIB.xml_destroy(bh);
				}
				if (dutAdd) {
					gLIB.xml_add(fh, dh);
					gLIB.xml_destroy(dh);
				}
				if (pageFailCount > m_nFCDPageLimit)
					pDut->m_nCurResult = STEP_FAIL;
				pDut->setResult();
				
			}
			// Next Scan
			i++;
			printf("Out Scan Change %d (%d/%d) \n", (i >= scansz)? -1: gBIB.m_validScanVec[i], i, scansz);

			if (i >= scansz)break;
			clearFCDMem();
			gFM.writeScanIndex( gBIB.m_validScanVec[i]);
			printf("Check Scan %d\n", gFM.readScanIndex());

			gPG.nextSync();
		}
	}

	gPG.stopPG();	
	gLIB.xml_add(hSub, fh);
	gLIB.xml_destroy(fh);

	delete[] pData;

	gLIB.xml_save(doc);
	gRES.freeSubResult();

	

}

void nand::makeDataFile(int type)
{
	closeDataFile();

	for (int i = 0; i < gBIB.m_nDutCnt; i++)
	{
		dut* pDut = gBIB.getDut(i);
		if (pDut->m_nConResult == STEP_FAIL) continue;
		cstring path;
		if (type == 1) {
			path.Format("%s/D%04d.csv", DATA_FILE_PATH, i);
			pDut->m_fData = fopen(path, "wt");
		}
		else {
			path.Format("%s/D%04d.bin", DATA_FILE_PATH, i);
			pDut->m_fData = fopen(path, "wb");
		}
		
	}

}

int nand::readData(int scanNo)
{
	int rCnt = gFM.readFmData();
	
	if (rCnt > 0)
	{	
		//gFM.showFmData(0, rCnt);
		scan* pScan = gBIB.getScan(scanNo);
		int dutDataCnt = gFM.m_nDutDataSize;
		for (int j = 0; j < (int)pScan->m_DutsScanArr.size(); j++)
		{
			int dutNo = pScan->m_DutsScanArr[j];
			dut* pDut = gBIB.getDut(dutNo);
			
			//	printf("%s - Scan:%d Dut,%d DSZ:%d FSZ:%d\n", __FUNCTION__,
			//		scanNo, dutNo, dutDataCnt, rCnt);
			if (pDut->m_nConResult == STEP_FAIL || pDut->m_fData == NULL) continue;
			
			fwrite(pDut->pData, sizeof(byte), dutDataCnt, pDut->m_fData);
		}

	}
	return rCnt;
}

int nand::readMeasData(int scanNo)  // Compare Mode, DDR Actiing
{
	int rCnt = gFM.readFmData();
	if (rCnt == 0) return 0;
		
	scan* pScan = gBIB.getScan(scanNo);
	FM_MEM* pfml;
	FM_MEM* pfmh;

	uint devMask = 0;
	for (int i = 0; i < gBIB.m_nIoCnt; i++)
		devMask |= (1 << i);

	for (int i = 0; i < rCnt; i++)
	{
		pfmh = gFM.getFmDataH(i);
		pfml = gFM.getFmDataL(i);
		uint xh = pfmh->adrDrv[0] | (pfmh->adrDrv[1] << 8);
		uint xl = pfml->adrDrv[0] | (pfml->adrDrv[1] << 8);
		uint yh = pfmh->adrDrv[2] | (pfmh->adrDrv[3] << 8);
		uint yl = pfml->adrDrv[2] | (pfml->adrDrv[3] << 8);
		uint zh = pfmh->adrDrv[4];
		uint zl = pfml->adrDrv[4];
		uint drvioh = pfmh->ioDrv[0] | (pfmh->ioDrv[1] << 8);
		uint drviol = pfml->ioDrv[0] | (pfml->ioDrv[1] << 8);


		

		for (int j = 0; j < (int)pScan->m_DutsScanArr.size(); j++)
		{
			int dutNo = pScan->m_DutsScanArr[j];
			dut* pDut = gBIB.getDut(dutNo);
			if (pDut->m_nConResult == STEP_FAIL || pDut->m_fData == NULL) continue;
			uint ioh = (uint)*(pDut->pData + (i * 2));
			uint iol = (uint) *(pDut->pData + (i * 2) + 1);
			if (ioh == 0 || iol == 0)continue;

			uint devh = (ioh ^ drvioh) & devMask;
			uint devl = (iol ^ drviol) & devMask;
						
			fprintf(pDut->m_fData,
				"%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n"
				,xh,xl,yh,yl,zh,zl, drvioh, drviol, ioh,iol, devh, devl);		
			pDut->m_nCurResult = STEP_FAIL;
		}		
	}
	return rCnt;
}

void nand::testMeasData()
{
	int scansz = gBIB.m_validScanVec.size();
	printf("Valid Scan SZ %d\n", scansz);	
	if (scansz == 0)return;
	makeDataFile(1);	
	int scanIdx = 0;
	gFM.writeScanIndex(gBIB.m_validScanVec[0]);
	gPG.runPG();

	while (1)
	{
		readMeasData(gBIB.m_validScanVec[scanIdx]);

		if (gOPER.checkStat() == 0) {
			readMeasData(gBIB.m_validScanVec[scanIdx]);
			break;
		}
		int sync = gOPER.checkSync();
		if (sync != SYNC_NONE) {
			readMeasData(gBIB.m_validScanVec[scanIdx]);
			if (sync == SYNC_DATA) {
				gPG.nextSync();
			}
			else if (sync == SYNC_SCAN) {
				scanIdx++;
				if (scanIdx >= scansz)break;
				printf("Out Scan Change %d (%d/%d) \n", gBIB.m_validScanVec[scanIdx], scanIdx, scansz);				
				gFM.writeScanIndex(gBIB.m_validScanVec[scanIdx]);
				gPG.nextSync();
			}
		}

		gOPER.readDPS();
		gOPER.readSysTemp();
		gOPER.doScanProc();

		usleep(1);
	}
	gPG.stopPG();
	closeDataFile();
}


void nand::testData()
{ 
	int scansz = gBIB.m_validScanVec.size();
	printf("Valid Scan SZ %d\n",scansz);
	//if (gAPI.m_nTestMode != TEST_MODE_CONTACT)
	if (scansz == 0)return;
	makeDataFile();
	int scanIdx = 0;
	gFM.writeScanIndex( gBIB.m_validScanVec[0]);	
	gPG.runPG();
	
	while (1)
	{
		readData(gBIB.m_validScanVec[scanIdx]);
		
		if (gOPER.checkStat() == 0)	{
			readData(gBIB.m_validScanVec[scanIdx]);
			break;
		}
		int sync = gOPER.checkSync();		
		if (sync != SYNC_NONE) {
			readData(gBIB.m_validScanVec[scanIdx]);
			if (sync == SYNC_DATA) {
				gPG.nextSync();
			}
			else if (sync == SYNC_SCAN) {
				scanIdx++;
				if (scanIdx >= scansz)break;
				printf("Out Scan Change %d (%d/%d) \n", gBIB.m_validScanVec[scanIdx], scanIdx, scansz);
				gFM.writeScanIndex( gBIB.m_validScanVec[scanIdx]);
				gPG.nextSync();
			}
		}

		gOPER.readDPS();
		gOPER.readSysTemp();
		gOPER.doScanProc();

		usleep(1);
	}
	gPG.stopPG();
	closeDataFile();
}

void nand::closeDataFile()
{
	for (int i = 0; i < gBIB.m_nDutCnt; i++)
	{
		dut* pDut = gBIB.getDut(i);
		if (pDut->m_fData != NULL)
		{
			fclose(pDut->m_fData);
			pDut->m_fData = NULL;
		}
	}
}

void nand::initCycleBlock()
{
	for (int i = 0; i < gBIB.m_nDutCnt; i++) {
		dut* pDut = gBIB.getDut(i);
		if (pDut->m_nConResult == STEP_FAIL) continue;		
		memset(pDut->m_nMaxProgBlock, 0, sizeof(pDut->m_nMaxProgBlock));
		memset(pDut->m_nMaxEraseBlock, 0, sizeof(pDut->m_nMaxEraseBlock));
		memset(pDut->m_nSumProgBlock, 0, sizeof(pDut->m_nSumProgBlock));
		memset(pDut->m_nSumEraseBlock, 0, sizeof(pDut->m_nSumEraseBlock));
		pDut->m_nFBlkEraseArr.clear();
		pDut->m_nFBlkProgArr.clear();
	}

	for (int i = 0; i < 8; i++)
		m_nPrevRB[i] = GET_RB_COUNT(i);  

	m_nCycleMode = RB_MODE_ERASE; // first erase
	m_nCycleBlock = 0;
	m_nCycleTestCnt = 0;
}


void nand::addCycleFBlk(FM_MEM_NAND* pFm)
{
	uint block = pFm->blkAdr[0] | pFm->blkAdr[1] << 8 | pFm->blkAdr[2] << 16;
	int scanIdx = pFm->scan;

	scan* pScan = gBIB.getScan(scanIdx);
//	printf("rsc %d fsc %d\n", gFM.readScanIndex(), scanIdx);
	for (int j = 0; j < (int)pScan->m_DutsScanArr.size(); j++)
	{

		int dutNo = pScan->m_DutsScanArr[j];
		dut* pDut = gBIB.getDut(dutNo);
		if (pDut->m_nConResult == STEP_FAIL) continue;

		if (pFm->dutCum & pDut->m_nFlashBit) {
			printf("AddFail : Dut %d -> Blk %d\n", dutNo, block);
			if (m_nCycleMode == RB_MODE_PROG) {
				if (find(pDut->m_nFBlkProgArr.begin(), pDut->m_nFBlkProgArr.end(), block) == pDut->m_nFBlkProgArr.end())
					pDut->m_nFBlkProgArr.push_back(block);
			}
			else {
				if (find(pDut->m_nFBlkEraseArr.begin(), pDut->m_nFBlkEraseArr.end(), block) == pDut->m_nFBlkEraseArr.end())
					pDut->m_nFBlkEraseArr.push_back(block);
			}
		}
	}
}

void nand::findCycleFBlk()
{
	int rCnt = gFM.readFmData();
	for (int i = 0; i < rCnt; i++)
	{				
		FM_MEM_NAND* pFm = (FM_MEM_NAND*)gFM.getFmDataH(i);
		FM_MEM_NAND* pFmL = (FM_MEM_NAND*)gFM.getFmDataL(i);
		if (i < gFM.m_nFmHSize) addCycleFBlk(pFm);
		if (gBIB.m_bDDR) {
			if (i < gFM.m_nFmLSize) addCycleFBlk(pFmL);
		}
	}
}

void nand::initProgRB()
{
	m_bRBFirst = true;
	for (int i = 0; i < 8; i++) {
		m_nPrevRB[i] = m_nCurRB[i];
		m_nCurRB[i] = 0;
	}
}

void nand::readProgRB()
{
	for (int i = 0; i < 8; i++) 
	{		
		uint rb = GET_RB_COUNT(i);
		if (m_bRBFirst == true) {			
			if (m_nPrevRB[i] == rb)
				continue;
			else
				m_bRBFirst = false;
		}
		else {			
			if (m_nCurRB[i] < rb)  m_nCurRB[i] = rb;
		}
	}
}

void nand::setRB(int scanIdx)
{
	scan* pScan = gBIB.getScan(scanIdx);
	for (int j = 0; j < (int)pScan->m_DutsScanArr.size(); j++)
	{
		int dutNo = pScan->m_DutsScanArr[j];
		dut* pDut = gBIB.getDut(dutNo);
		if (pDut->m_nConResult == STEP_FAIL) continue;
		int rbIdx = pDut->m_IOArr[0] / 8;

		if (m_nCycleMode == RB_MODE_ERASE) {
			if (pDut->m_nMaxEraseBlock[m_nCycleBlock] < m_nCurRB[rbIdx])
				pDut->m_nMaxEraseBlock[m_nCycleBlock] = m_nCurRB[rbIdx];
			pDut->m_nSumEraseBlock[m_nCycleBlock] += m_nCurRB[rbIdx];
		}
		else {
			if (pDut->m_nMaxProgBlock[m_nCycleBlock] < m_nCurRB[rbIdx])
				pDut->m_nMaxProgBlock[m_nCycleBlock] = m_nCurRB[rbIdx];
			pDut->m_nSumProgBlock[m_nCycleBlock] += m_nCurRB[rbIdx];
		}
	}
}

void nand::cycleTest()
{
	int scansz = gBIB.m_validScanVec.size();
	if (gAPI.m_nTestMode != TEST_MODE_CONTACT)
		if (scansz == 0)return;

	
	gFM.writeScanIndex( gBIB.m_validScanVec[0]);
	for (int i = 0; i < scansz; i++)
		printf("ValidScan %d / %d\n", gBIB.m_validScanVec[i], i);
	
	gFM.writeScanIndex(gBIB.m_validScanVec[0]);
	
	gPG.runPG();
	
	int scanIdx = 0;
	initCycleBlock();

	printf("Cycle Test Start\n");
	while (1)
	{
		if (gOPER.checkStat() == 0) break;

		if (m_nCycleMode == RB_MODE_PROG)
			readProgRB();

		int sync = gOPER.checkSync();
		if (sync == SYNC_NONE) continue;
		else if (sync == SYNC_DATA) { gPG.nextSync(); continue; }
		else if (sync == SYNC_CYCLE_PROG_START) {
			printf("SYNC_CYCLE_PROG_START  CycleCnt %d\n", m_nCycleTestCnt);
			m_nCycleMode = RB_MODE_PROG;
			initProgRB();
			m_nCycleBlock = 0;

			gPG.nextSync();
		}
		else if (sync == SYNC_CYCLE_ERASE_START) {
			m_nCycleTestCnt++;
			printf("SYNC_CYCLE_ERASE_START  CycleCnt %d\n", m_nCycleTestCnt);
			m_nCycleMode = RB_MODE_ERASE;
			m_nCycleBlock = 0;
			gPG.nextSync();
		}
		else if (sync == SYNC_CYCLE_PROG_GATHER) {
			printf("SYNC_CYCLE_PROG_GATHER  Block %d\n", m_nCycleBlock);
			findCycleFBlk();
			setRB(gBIB.m_validScanVec[scanIdx]);

			initProgRB();
			m_nCycleBlock++;

			gPG.nextSync();
			if (m_nCycleBlock > MAX_BLOCK_ADR)
				break;
		}
		else if (sync == SYNC_CYCLE_ERASE_GATHER) {
			printf("SYNC_CYCLE_ERASE_GATHER  Block %d\n", m_nCycleBlock);
			findCycleFBlk();
			for (int i = 0; i < 8; i++)
				m_nCurRB[i] = GET_RB_COUNT(i);			
			setRB(gBIB.m_validScanVec[scanIdx]);
			m_nCycleBlock++;

			gPG.nextSync();
			if (m_nCycleBlock > MAX_BLOCK_ADR)
				break;
		}
		else if (sync == SYNC_SCAN) {
			scanIdx++;
			if (scanIdx >= scansz)break;			
			m_nCycleTestCnt = 0;
			printf("Out Scan Change %d (%d/%d) \n", gBIB.m_validScanVec[scanIdx], scanIdx, scansz);
			gFM.writeScanIndex( gBIB.m_validScanVec[scanIdx]);
			gPG.nextSync();
		}
	}
	
	gPG.stopPG();
}