#include "failmem.h"
#include "ioc.h"
#include "errc.h"
#include "STAR_TEST.h"

extern ioc g_clIO;
extern errc g_clErr;
extern bib	g_clBib;


failmem::failmem()
{
	m_nDutDataProc = DUT_DATA_NONE | DUT_DATA_BYSCAN;
}

failmem::~failmem()
{

}

void failmem::initFailMem()
{
	setFmMode();
	clearFmComp();
	clearFmCount();
	setFmDrvMode(0);
	readDut = &failmem::read8io;
	m_nDutDataSize = 0;
	

}



void failmem::setFmMode(int compare, int conditionCapture)
{
	uint adr = 1 << 9 | 18;
	uint val = FM_DATA;
	m_nFmDataMode = compare;
	if (compare == FM_COMP) {
		val |= 1; // 0b0001
	}
	else if (compare == FM_NAND) 
		val |= 3; // 0b0011
	
	if (compare == FM_NAND) val |= (3 << 2);
	if (conditionCapture) val |= 4; // 0b0100
	else val &= ~4;
	//gIO.m_bDebug = 1;
	gIO.writePG(adr, val, 1);

}

void failmem::setFmOverStop(bool overStop)
{
	//gIO.m_bDebug = 0;
	uint adr = 1 << 9 | 26;
	uint val = 0;
	if (overStop) // 0 = continue, 1 = stop
		val = 1;

	gIO.writePG(adr, val, 1);
}


void failmem::setFmDrvMode(int mode)
{
	m_nFmDrvMode = mode;
	uint adr = 1 << 9 | 21;
	gIO.writePG(adr, mode, 1);
}

void failmem::clearFmMemory()
{
	byte fmbuf[32];
	memset(fmbuf, 0, 32);
	memset(m_fmDataHArr, 0, sizeof(m_fmDataHArr));
	memset(m_fmDataLArr, 0, sizeof(m_fmDataLArr));

	for (int i = 0; i < FM_DEPTH; i++)
	{
		gIO.writePGData((0x4 << 17) | (i << 5), fmbuf, 32);
		gIO.writePGData((0x5 << 17) | (i << 5), fmbuf, 32);
	}
	clearFmCount();
}


void failmem::clearFmCount()
{
	uint adr = (1 << 9) | 19;
	gIO.writePG(adr, 1, 1);
	gIO.writePG(adr, 0, 1);	

	m_nFmHeadPosH = 0;
	m_nFmTailPosH = 0;
	m_nFmStartCntH = 0;
	m_nFmEndCntH = 0;

	m_nFmHeadPosL = 0;
	m_nFmTailPosL = 0;
	m_nFmStartCntL = 0;
	m_nFmEndCntL = 0;
}

void failmem::clearFmComp()
{
	uint adr = (1 << 9) | 27;
	gIO.writePG(adr, 1, 1);
	gIO.writePG(adr, 0, 1);
}

void failmem::writeScanIndex(int idx)
{
	uint adr = (1 << 9) | 11;
	gIO.writePG(adr, idx, 1);
	adr = (1 << 9) | 12;
	gIO.writePG(adr, 1, 1);
	gIO.writePG(adr, 0, 1);
	m_nCurScan = idx;
//	printf("%s Write Scan %d\n", __FUNCTION__, idx);
}

int failmem::readScanIndex()
{
	uint adr = (1 << 9) | 328;
	uint idx = gIO.readPG(adr, 1);
	if (idx > 255)
	{
//		gERR.print("scan index read error cnt : 0x%x\n", idx);
		idx = 0;
	}
	m_nCurScan = idx;
	return idx;
}

void failmem::readFmComp(int scanIdx)
{
	if (scanIdx >= FM_SCAN_SZ)return;
	//    printf("=====Scan %d======\n", scanIdx);
	//    gIO.showDBG(1);
	uint adrSc, adrH, adrL;
	adrSc = (1 << 9) | 28; // scan Sel
	gIO.writePG(adrSc, scanIdx, 1);

	adrH = PG_SYSTEM | 262; // H sdr
	adrL = PG_SYSTEM | 286; // L ddr

	gIO.readPGData(adrH, m_fmCompHArr[scanIdx], COMP_MEM_SZ);
	gIO.readPGData(adrL, m_fmCompLArr[scanIdx], COMP_MEM_SZ);


}

byte* failmem::getFmCompH(int scanIdx)
{
	return m_fmCompHArr[scanIdx];
}

byte* failmem::getFmCompL(int scanIdx)
{
	return m_fmCompLArr[scanIdx];
}

int failmem::getFmReadCnt()
{
	return m_nFmReadSize;
}

int failmem::readFmDataH()
{
//	int readszH = gIO.readPG((1 << 9) | 312, 1) | (gIO.readPG((1 << 9) | 311, 1) << 8); //read fm index
//	printf("%s H : %d\n", __FUNCTION__, readszH);
	
	uint adrS = 4 << 17;  // H

	byte* pDestS = (byte*)&m_fmDataHArr;

	m_nFmHSize = 0;
	if (m_nFmCurCntH != m_nFmEndCntH)
	{
		m_nFmStartCntH = m_nFmEndCntH;
		m_nFmEndCntH = m_nFmCurCntH;

		m_nFmHeadPosH = m_nFmStartCntH % FM_DEPTH;
		m_nFmTailPosH = m_nFmEndCntH % FM_DEPTH;

		int realCnt = 0;
		bool bTailOver = false;
		if (m_nFmHeadPosH == m_nFmTailPosH)
			realCnt = FM_DEPTH;
		else if (m_nFmHeadPosH > m_nFmTailPosH) {
			realCnt = (FM_DEPTH - m_nFmHeadPosH) + m_nFmTailPosH;
			bTailOver = true;
		}
		else
			realCnt = m_nFmTailPosH - m_nFmHeadPosH;

//		printf("%s : start %d end %d Head %d Tail %d sz %d (%d)\n", __FUNCTION__,
//			m_nFmStartCntH, m_nFmEndCntH, m_nFmHeadPosH, m_nFmTailPosH,
//			realCnt, bTailOver);

		int ofs = FM_MEM_SZ * m_nFmHeadPosH;

		if (bTailOver)
		{
			int r1Adr = (FM_DEPTH - m_nFmHeadPosH) * FM_MEM_SZ;
			int r1Cnt = (FM_DEPTH - m_nFmHeadPosH);
			gIO.readPGData(adrS | r1Adr, pDestS, r1Cnt * FM_MEM_SZ);
			int r2Cnt = m_nFmTailPosH;
			gIO.readPGData(adrS, pDestS + (r1Cnt * FM_MEM_SZ), r2Cnt * FM_MEM_SZ);

//			printf("readAdr 1 : %x, 2: %x\n", adrS | r1Adr, adrS);
		}
		else {
			int SeperateCnt = FM_DEPTH / 2; // 1024 보다 크면 끊어읽음
			int r1Adr = m_nFmHeadPosH;

			if (realCnt <= SeperateCnt) {
				gIO.readPGData(adrS | r1Adr, pDestS, realCnt * FM_MEM_SZ);
//				printf("readAdr : %x\n", adrS | r1Adr);
			}
			else {
				int r2Adr = r1Adr + (FM_DEPTH - SeperateCnt) * FM_MEM_SZ;
				int r2Cnt = realCnt - SeperateCnt;
				gIO.readPGData(adrS | r1Adr, pDestS, SeperateCnt * FM_MEM_SZ);
				gIO.readPGData(adrS | r2Adr , pDestS + (SeperateCnt * FM_MEM_SZ), r2Cnt * FM_MEM_SZ);

				//printf("fm Read 1st adr : %x(%d) sz %d\n", r1Adr, r1Adr, SeperateCnt * FM_MEM_SZ);
				//printf("fm Read 2st adr : %x(%d) sz %d\n", r2Adr, r2Adr, r2Cnt * FM_MEM_SZ);
			}


		}
		m_nFmHSize = realCnt;
	}

	return m_nFmHSize;
}

int failmem::readFmDataL()
{
//	int readszL = gIO.readPG((1 << 9) | 315, 1) | (gIO.readPG((1 << 9) | 314, 1) << 8); //read fm index
//	printf("%s L : %d\n", __FUNCTION__, readszL);

	uint adrD = 5 << 17;  // L

	byte* pDestD = (byte*)&m_fmDataLArr;

	m_nFmLSize = 0;
	if (m_nFmCurCntL != m_nFmEndCntL)
	{
		m_nFmStartCntL = m_nFmEndCntL;
		m_nFmEndCntL = m_nFmCurCntL;

		m_nFmHeadPosL = m_nFmStartCntL % FM_DEPTH;
		m_nFmTailPosL = m_nFmEndCntL % FM_DEPTH;

		int realCnt = 0;
		bool bTailOver = false;
		if (m_nFmHeadPosL == m_nFmTailPosL)
			realCnt = FM_DEPTH;
		else if (m_nFmHeadPosL > m_nFmTailPosL) {
			realCnt = (FM_DEPTH - m_nFmHeadPosL) + m_nFmTailPosL;
			bTailOver = true;
		}
		else
			realCnt = m_nFmTailPosL - m_nFmHeadPosL;

//		printf("%s : start %d end %d Head %d Tail %d sz %d (%d)\n", __FUNCTION__,
//			m_nFmStartCntL, m_nFmEndCntL, m_nFmHeadPosL, m_nFmTailPosL,
//			realCnt, bTailOver);

		int ofs = FM_MEM_SZ * m_nFmHeadPosL;

		if (bTailOver)
		{
			int r1Adr = (FM_DEPTH - m_nFmHeadPosL) * FM_MEM_SZ;
			int r1Cnt = (FM_DEPTH - m_nFmHeadPosL);
			gIO.readPGData(adrD | r1Adr, pDestD, r1Cnt * FM_MEM_SZ);
			int r2Cnt = m_nFmTailPosL;
			gIO.readPGData(adrD, pDestD + r1Cnt, r2Cnt * FM_MEM_SZ);

//			printf("readAdr 1 : %x, 2: %x\n", adrD | r1Adr, adrD);
		}
		else {
			int SeperateCnt = FM_DEPTH / 2; // 1024 보다 크면 끊어읽음
			int r1Adr = m_nFmHeadPosL;

			if (realCnt <= SeperateCnt) {
				gIO.readPGData(adrD | r1Adr, pDestD, realCnt * FM_MEM_SZ);
//				printf("readAdr : %x\n", adrD | r1Adr);
			}
			else {
				int r2Adr = r1Adr + (FM_DEPTH - SeperateCnt) * FM_MEM_SZ;
				int r2Cnt = realCnt - SeperateCnt;
				gIO.readPGData(adrD | r1Adr, pDestD, SeperateCnt * FM_MEM_SZ);
				gIO.readPGData(adrD | r2Adr, pDestD + (SeperateCnt * FM_MEM_SZ), r2Cnt * FM_MEM_SZ);
			}


		}
//		else {
//			int r1Adr = m_nFmHeadPosL;
//			gIO.readPGData(adrD | r1Adr, pDestD, realCnt * FM_MEM_SZ);
////			printf("readAdr : %x\n", adrD | r1Adr);
//		}
		m_nFmLSize = realCnt;
	}

	return m_nFmLSize;
}

int failmem::readFmCnt()
{
	int rdh1 = gIO.readPG((1 << 9) | 311, 2);
	int rdh2 = gIO.readPG((1 << 9) | 311, 2);
	int rdl1 = gIO.readPG((1 << 9) | 314, 2);
	int rdl2 = gIO.readPG((1 << 9) | 314, 2);
	

	int h = ((rdh2 >> 8) & 0xff) | ((rdh2 & 0xff) << 8); 
	int l = ((rdl2 >> 8) & 0xff) | ((rdl2 & 0xff) << 8);
//	printf("%s h : %d, l : %d\n", __FUNCTION__, h, l);
	
	m_nFmCurCntH = h;
	m_nFmCurCntL = l;

	if (rdh1 != rdh2 || rdh1 != rdh2)
	{
		printf("%s ACTIVE = h: %d -> %d, l: %d -> %d\n", __FUNCTION__, 
			((rdh1 >> 8) & 0xff) | ((rdh1 & 0xff) << 8), h, 
			((rdl1 >> 8) & 0xff) | ((rdl1 & 0xff) << 8), l);
		return FM_CNT_ACTIVE;
	}
	
	return  FM_CNT_STABLE;
		
}

int failmem::readFmData(int fcRead)
{
	if (fcRead != 0) {		
		m_nFmCurCntH = fcRead;
		m_nFmCurCntL = fcRead;
	}else if (readFmCnt() == FM_CNT_ACTIVE) return 0;

	readFmDataH();
	readFmDataL();
	int h = m_nFmCurCntH;
	int l = m_nFmCurCntL;
//	if (readFmCnt() != FM_CNT_ACTIVE)    
	if ((m_nFmCurCntH != 0 || m_nFmCurCntL != 0) /*&& (clear == FM_CNT_CLEAR)*/)
		clearFmCount();      // clear 안하면 FM Cnt position 안맞는것으로 보임.

	m_nFmReadSize = m_nFmHSize;
	if (m_nFmHSize < m_nFmLSize) m_nFmReadSize = m_nFmLSize;

	if (m_nDutDataProc & DUT_DATA_ENABLE)
	{
		if (m_nDutDataProc & DUT_DATA_ALLSCAN)
			doDutDataAllScan();
		else
			doDutData();
	}

	

	return m_nFmReadSize;
}

void failmem::setReadDut()
{
	if (gBIB.m_nIoCnt == 0)
	{
		gERR.addErr(ERR_RUNTIME, __FUNCTION__, "IO Count is Not Setting. BIBSIZE() Call First");
		return;
	}

	for (int i = 0; i < gBIB.m_nScanSize; i++)
	{
		scan* pScan = gBIB.getScan(i);
		for (size_t j = 0; j < pScan->m_DutsScanArr.size(); j++)
		{
			int dno = pScan->m_DutsScanArr[j];
			dut* pDut = gBIB.getDut(dno);
			int start_pos = pDut->m_IOArr[0]; 
			int io_idx = (start_pos % MAX_IO_CNT) / 8;
			pDut->m_nFlashBit = 1 << io_idx;
			if (gBIB.m_nIoCnt == 16)
				pDut->m_nFlashBit |= 1 << (io_idx + 1);
			
			pDut->pBaseS = (byte*)&m_fmDataHArr[0].ioComp[io_idx];
			pDut->pBaseD = (byte*)&m_fmDataLArr[0].ioComp[io_idx];
			pDut->pBaseOfs = 0;
		}
	}
	if (gBIB.m_nIoCnt == 1)
		readDut = &failmem::read1io;
	else if (gBIB.m_nIoCnt == 4)
		readDut = &failmem::read4io;
	else if (gBIB.m_nIoCnt == 8)
		readDut = &failmem::read8io;
	else if (gBIB.m_nIoCnt == 16)
		readDut = &failmem::read16io;	
}

void failmem::read16io(dut* pDut, int ofs, int cnt)
{
	word* pSingle = (word*)pDut->pBaseS + ((ofs*FM_MEM_SZ) / 2);
	word* pDual = (word*)pDut->pBaseD + ((ofs*FM_MEM_SZ) / 2);
	word* pDest = (word*)pDut->pData;

	int sz = ofs + cnt;

	for (int i = ofs; i < sz; i++)
	{
		*(pDest) = *(pSingle);
		pDest++;
		pSingle += (FM_MEM_SZ / 2);

		if (gBIB.m_bDDR) {
			*(pDest) = *(pDual);
			pDest++;
			pDual += (FM_MEM_SZ / 2);
		}		
	}
}

void failmem::read8io(dut* pDut, int ofs, int cnt)
{
	byte* pSingle = pDut->pBaseS + (ofs * FM_MEM_SZ);
	byte* pDual =	pDut->pBaseD + (ofs * FM_MEM_SZ);
	byte* pDest =	pDut->pData;

	int sz = ofs + cnt;

	for (int i = ofs; i < sz; i++)
	{
		*(pDest) = *(pSingle);
		pDest++;
		pSingle += FM_MEM_SZ;

		if (gBIB.m_bDDR) {
			*(pDest) = *(pDual);			
			pDest++;
			pDual += FM_MEM_SZ;
		}
	}
}

void failmem::read4io(dut* pDut, int ofs, int cnt)
{
	byte* pSingle = pDut->pBaseS + (ofs * FM_MEM_SZ);
	byte* pDual =	pDut->pBaseD + (ofs * FM_MEM_SZ);
	byte* pDest =	pDut->pData;
	*pDest = 0;

	int sz = ofs + cnt;
	int dest_pos = 0;

//		printf("sc %d dno %d[%d,%d,%d,%d] : \n", pDut->m_nScanNo, pDut->m_nDutNo, pDut->m_IOArr[0], pDut->m_IOArr[1], pDut->m_IOArr[2], pDut->m_IOArr[3]);
	
	for (int i = ofs; i < sz; i++)
	{
		for (int nio = 0; nio < 4; nio++)
		{
			int iomask = 1 << (pDut->m_IOArr[nio] % 8);
			byte c = *pSingle & iomask;
			if (c) *(pDest) |= 1 << (nio + (dest_pos * 4));			

//				printf("src %02x dest %02x mask %02x(%x) \n", *pSingle, *pDest, iomask, c);
		}

		dest_pos++;
		pSingle += FM_MEM_SZ;

		if (gBIB.m_bDDR) {
			for (int nio = 0; nio < 4; nio++)
			{
				int iomask = 1 << (pDut->m_IOArr[nio] % 8);
				byte c = *pDual & iomask;
				if (c) *(pDest) |= 1 << (nio + (dest_pos * 4));
			}
			dest_pos++;
			pDual += FM_MEM_SZ;
		}

		if (dest_pos >= 2) 
		{	
//				printf("=%02x \n", *pDest);
			pDest++;
			*pDest = 0;
			dest_pos = 0;
		}
	}
//	printf("\n");

}

void failmem::read1io(dut* pDut, int ofs, int cnt)
{
	byte* pSingle = pDut->pBaseS + (ofs * FM_MEM_SZ);
	byte* pDual =   pDut->pBaseD + (ofs * FM_MEM_SZ);
	byte* pDest = pDut->pData;
	*pDest = 0;
	int sz = ofs + cnt;
	int dest_pos = 0;
	
	int iomask = (1 << (pDut->m_IOArr[0] % 8)) & 0xff;
	byte c;
//	if (pDut->m_nDutNo == 0 || pDut->m_nDutNo == 1)
//		printf("D%d SZ %d io Pos %d  io %d bit %x \n",
//			pDut->m_nDutNo, cnt, pDut->m_IOArr[0], iomask);
	for (int i = ofs; i < sz; i++)
	{
		c = *pSingle & iomask;
		
//		if ((pDut->m_nDutNo == 0 || pDut->m_nDutNo == 1) && i <100)
//			printf("%x = %x & %x\n", c, *pSingle, iomask);
		if (c) {
			*(pDest) |= 1 << dest_pos;			
		}
		dest_pos++;
		pSingle += FM_MEM_SZ;
		if (gBIB.m_bDDR) {
			c = *pDual & iomask;
			if (c) *(pDest) |= 1 << dest_pos;							
			dest_pos++;
			pDual += FM_MEM_SZ;
		}

		if (dest_pos > 7) {
//			if ((pDut->m_nDutNo == 0 || pDut->m_nDutNo == 1) && i < 100)
//				printf("%02x\n", *(pDest));
			pDest++;
			*pDest = 0;
			dest_pos = 0;
			
		}	
	}
	if (pDut->m_nDutNo == 0 || pDut->m_nDutNo == 1)
		printf("\n");
}


void failmem::doDutDataAllScan()
{
	int scanIdx = -1;	
	int st = -1;
	int cnt = 0;

	dut* pDut = NULL;
	for (int i = 0; i < m_nFmReadSize; i++)
	{		
//		printf("idx : %d\n", i);
		if (getFmDataH(i)->scan != scanIdx) {			
			
//			printf("sc: %d\n", scanIdx);			
			if (scanIdx != -1) {
				scan* pScan = gBIB.getScan(scanIdx);
				for (size_t i = 0; i < pScan->m_DutsScanArr.size(); i++)
				{
//					showFmData(st, cnt);
					int dno = pScan->m_DutsScanArr[i];
					pDut = g_clBib.getDut(dno);
					if (pDut->m_nDataMask || pDut->m_nConResult != STEP_FAIL)
						(this->*readDut)(pDut, st, cnt);
				}
//				printf("sc[%d] st %d  cnt %d\n", scanIdx, st, cnt);			
			}			
			scanIdx = getFmDataH(i)->scan;
			st = i;
			cnt = 1;
						
		}else cnt++;		

		if (i == (m_nFmReadSize - 1)) {
			if (cnt != 0 && st != -1 && pDut != NULL)
			{
				scan* pScan = gBIB.getScan(scanIdx);
				for (size_t i = 0; i < pScan->m_DutsScanArr.size(); i++)
				{
					int dno = pScan->m_DutsScanArr[i];
					pDut = g_clBib.getDut(dno);
					if (pDut->m_nDataMask || pDut->m_nConResult == STEP_FAIL)
						continue;
					(this->*readDut)(pDut, st, cnt);					
				}
//				printf("last sc[%d] st %d  cnt %d\n", scanIdx, st, cnt);
			}
		}
	}

//	printf("cnt %d\n", cnt);

	m_nDutDataSize = 0;
	if (gBIB.m_nIoCnt == 0)return;
	if (gBIB.m_nIoCnt >= 8)
		m_nDutDataSize = cnt * (gBIB.m_nIoCnt / 8);
	else if (gBIB.m_nIoCnt < 8)
		m_nDutDataSize = cnt / (8 / gBIB.m_nIoCnt);
//	printf("m_nDutDataSize %d\n", m_nDutDataSize);

	if (gBIB.m_bDDR) m_nDutDataSize *= 2;
	
		
}

void failmem::doDutData()
{
	if (m_nCurScan >= gBIB.m_nScanSize) return;

	scan* pScan = gBIB.getScan(m_nCurScan);
	for (size_t i = 0; i < pScan->m_DutsScanArr.size(); i++)
	{
		int dno = pScan->m_DutsScanArr[i];
		dut* pDut = g_clBib.getDut(dno);		
		if (pDut->m_nDataMask || pDut->m_nConResult == STEP_FAIL)continue;
		
		(this->*readDut)(pDut, 0, m_nFmReadSize);
	}	

	m_nDutDataSize = 0;
	if (gBIB.m_nIoCnt == 0)return;
	if (gBIB.m_nIoCnt >= 8)
		m_nDutDataSize = m_nFmReadSize * (gBIB.m_nIoCnt / 8) ;
	else if (gBIB.m_nIoCnt < 8)
		m_nDutDataSize = m_nFmReadSize / (8 / gBIB.m_nIoCnt);
	
	if (gBIB.m_bDDR) m_nDutDataSize *= 2;
}


FM_MEM* failmem::getFmDataH(int pos) // sel 0=sd, 1=dd
{
	if (pos >= FM_DEPTH) {
		gERR.addErr(ERR_RUNTIME, __FUNCTION__, "Select FM Index %d! FM MAX Depth 2048.", pos);
		pos = 0;
	}
	return &m_fmDataHArr[pos];
}

FM_MEM* failmem::getFmDataL(int pos) // sel 0=sd, 1=dd
{
	if (pos >= FM_DEPTH) {
		gERR.addErr(ERR_RUNTIME, __FUNCTION__, "Select FM Index %d! FM MAX Depth 2048.", pos);
		pos = 0;
	}
	return &m_fmDataLArr[pos];
}

void failmem::clearFCount()
{
	int dutCnt = 8;
	for (int i = 0; i < dutCnt; i++)
	{
		uint adr = (1 << 9) | 9;
		gIO.writePG(adr, i, 1);
		adr = (1 << 9) | 10;
		gIO.writePG(adr, 1, 1);
		gIO.writePG(adr, 0, 1);

	}
}

void failmem::readFCountOfDut(int sc)
{
//	int sc = readScanIndex();
	if (sc >= gBIB.m_nScanSize) return;

	scan* pScan = gBIB.getScan(sc);	
	for (size_t t = 0; t< pScan->m_DutsScanArr.size(); t++)
	{
		int dno = pScan->m_DutsScanArr[t];
		dut* pDut = gBIB.getDut(dno);		
		pDut->m_nCurFailCount = 0;
		if (pDut->m_nConResult == STEP_FAIL) continue;
		int selBit = (pDut->m_IOArr[0] % MAX_IO_CNT)/8;
		
		for (int i = 0; i < (gBIB.m_nIoCnt / 8); i++)
		{
			uint adr = (1 << 9) | 9;
			gIO.writePG(adr, selBit, 1);
			adr = (1 << 9) | 319;
			int rd = gIO.readPG(adr, 4);					
			
			pDut->m_nCurFailCount += rd;
//			printf("Fcnt sc %d dno %d rd %d FCNT %d\n", sc, dno, rd, pDut->m_nCurFailCount);
			selBit++;
		}
		
	}

	clearFCount();

	//for (size_t t = 0; t < pScan->m_DutsScanArr.size(); t++)
	//{
	//	int dno = pScan->m_DutsScanArr[t];
	//	dut* pDut = gBIB.getDut(dno);
	//	pDut->m_nCurFailCount = 0;
	//	if (pDut->m_nConResult == STEP_FAIL) continue;
	//	int selBit = (pDut->m_IOArr[0] % MAX_IO_CNT) / 8;
	//
	//	for (int i = 0; i < (gBIB.m_nIoCnt / 8); i++)
	//	{
	//		uint adr = (1 << 9) | 9;
	//		gIO.writePG(adr, selBit, 1);
	//		adr = (1 << 9) | 319;
	//		int rd = gIO.readPG(adr, 4);
	//		
	//		pDut->m_nCurFailCount += rd;
	//		printf("[2] sc %d dno %d rd %d FCNT %d\n", sc, dno, rd, pDut->m_nCurFailCount);
	//		selBit++;
	//	}
	//
	//}
	//

}



void failmem::showFmDrv(int ofs, int size)
{
	int loopCnt = ofs + size;
	if (loopCnt > FM_DEPTH) return;
	FM_MEM* pfml; 
	FM_MEM* pfmh;

//	#define FH(v)  (i >= m_nFmHSize) ? 0 : v  // SHOW FAIL H
//	#define FL(v)  (i >= m_nFmLSize) ? 0 : v  // SHOW FAIL H
	#define FH(v)  v  // SHOW FAIL H
	#define FL(v)  v  // SHOW FAIL H

	printf("%s: ofs %d,  sz %d\n", __FUNCTION__, ofs, loopCnt);
	if (m_nFmDrvMode == 0) {
		printf("ofs	scan	xh:xl		yh:yl		zh:zl	dh:dl\n");
		for (int i = ofs; i < loopCnt; i++) {
			pfmh = getFmDataH(i);
			pfml = getFmDataL(i);			
			printf("0x%04x:	%02x	%04x:%04x	%04x:%04x	%02x:%02x	%04x:%04x\n"
				, i, FH(pfmh->scan)
				, FH (pfmh->adrDrv[0] | (pfmh->adrDrv[1] << 8)), FL (pfml->adrDrv[0] | (pfml->adrDrv[1] << 8))
				, FH (pfmh->adrDrv[2] | (pfmh->adrDrv[3] << 8)), FL (pfml->adrDrv[2] | (pfml->adrDrv[3] << 8))
				, FH (pfmh->adrDrv[4]), FL (pfml->adrDrv[4])
				, FH (pfmh->ioDrv[0] | (pfmh->ioDrv[1] << 8)), FL (pfml->ioDrv[0] | (pfml->ioDrv[1] << 8)));
		}
	}
	else {
		printf("ofs	scan	ah	:al     dh:dl\n");
		for (int i = ofs; i < loopCnt; i++) {
			pfmh = getFmDataH(i);
			pfml = getFmDataL(i);

			printf("0x%04x:	%02x	%08x:%08x	%04x:%04x\n"
				, i, FH (pfmh->scan)
				, FH (pfmh->adrDrv[0] | (pfmh->adrDrv[1] << 8) | (pfmh->adrDrv[2] << 16) | (pfmh->adrDrv[3] << 24))
				, FL (pfml->adrDrv[0] | (pfml->adrDrv[1] << 8) | (pfml->adrDrv[2] << 16) | (pfml->adrDrv[3] << 24))
				, FH (pfmh->ioDrv[0] | (pfmh->ioDrv[1] << 8))
				, FL (pfml->ioDrv[0] | (pfml->ioDrv[1] << 8)));
		}
	}
}

void failmem::showFmData(int ofs, int size)
{
	int loopCnt = ofs + size;
	if (loopCnt > FM_DEPTH)return;
	FM_MEM* pfml;
	FM_MEM* pfmh;
	printf("%s: ofs %d,  sz %d\n", __FUNCTION__, ofs, loopCnt);
	for (int i = ofs; i < loopCnt; i++) {
		pfmh = getFmDataH(i);
		pfml = getFmDataL(i);
		printf("0x%04x(%02d): ", i, pfmh->scan);
		for (int j = 0; j < 8; j += 2)
		{
			printf("%04x:%04x ", pfmh->ioComp[j] | (pfmh->ioComp[j + 1] << 8), pfml->ioComp[j] | (pfml->ioComp[j + 1] << 8));
		}
		printf("\n");

	}
}

void failmem::showFmComp(int scanIdx)
{
	printf("sc %d : ", scanIdx);
	for (int i = 0; i < COMP_MEM_SZ / 2; i++)
	{
		printf("%02x%02x:%02x%02x ", m_fmCompHArr[scanIdx][i * 2 + 1], m_fmCompHArr[scanIdx][i * 2],
			m_fmCompLArr[scanIdx][i * 2 + 1], m_fmCompLArr[scanIdx][i * 2]);
	}
	printf("\n");
}

