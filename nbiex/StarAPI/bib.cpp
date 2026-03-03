#include "bib.h"
#include "errc.h"
#include "failmem.h"
#include "starAPI.h"
#include "ioc.h"
#include "nand.h"

#include <algorithm>

extern errctrl g_clErr;
extern failmem g_clFm;
extern starAPI g_clAPI;
extern ioc	   g_clIO;

extern bib	   g_clBib;


void dut::setResult()
{
	if (gAPI.m_nTestMode == TEST_MODE_CONTACT)
	{
		if (gBIB.m_bResultInvert)
			m_nCurResult == STEP_FAIL ? m_nCurResult = STEP_PASS: m_nCurResult = STEP_FAIL;
		
		if (m_nDutMask)	m_nCurResult = STEP_FAIL;
		

		m_nConResult &= m_nCurResult;
		m_nTotalResult &= m_nCurResult;
	}
	else if (gAPI.m_nTestMode == TEST_MODE_RUN || gAPI.m_nTestMode == TEST_MODE_SINGLE)
	{
		if (gBIB.m_bResultInvert)
			m_nCurResult == STEP_FAIL ? m_nCurResult = STEP_PASS : m_nCurResult = STEP_FAIL;
		if (m_nDutMask)	m_nCurResult = STEP_FAIL;

		m_nCurResult &= m_nConResult;
		m_nTotalResult &= m_nCurResult;
	}
	//printf("%s : dno[%d] cur %d tot %d mask %d\n", __FUNCTION__,
	//	m_nDutNo, m_nCurResult, m_nTotalResult, m_nDutMask);
}

void dut::init() {
	m_nDutMask = 0;
	m_nDataMask = 0;
	m_nResultBin = 0;
	memset(m_nFCat, 0, sizeof(m_nFCat));
	memset(m_nPCat, 0, sizeof(m_nPCat));
	m_sChipID = "";
	m_nDutNo = -1;
	m_FCatArr.clear();
	m_PCatArr.clear();


	memset(m_maskFBlks, 0, MAX_BLOCK_CUM * sizeof(int));
	memset(m_curFBlks, 0, MAX_BLOCK_CUM * sizeof(int));
	memset(m_totalFBlks, 0, MAX_BLOCK_CUM * sizeof(int));

	m_maskFBlkCnt = 0;
	m_curFBlkCnt = 0;
	m_curJudgeCnt = 0;
	m_totalFBlkCnt = 0;

	m_nConResult = STEP_PASS;   // init fail
	m_nCurResult = STEP_PASS;   // init fail
	m_nTotalResult = STEP_PASS; // init fail

	m_fData = NULL;
}

void dut::initCurFBlks()
{
	memset(m_curFBlks, 0, MAX_BLOCK_CUM * sizeof(int));
	m_curFBlkCnt = 0;
	m_curJudgeCnt = 0;
}

bib::bib()
{
	m_nScanSize = 0;
	m_nCurScanNo = 0;
	m_nXsz = 0;
	m_nYsz = 0;
	m_nMultiChip = 1;

	m_bDDR = false;
	m_bResultInvert = false;
}

bib::~bib()
{ 
	clearDut(); 
}

void bib::clearDut()
{
	for (int i = 0; i < (int)m_DutArr.size(); i++)
	{
		dut* pDut = m_DutArr.at(i);
		delete pDut;
	}
	m_DutArr.clear();
}

void bib::clearScan()
{
	memset(m_ScanArr, 0, sizeof(m_ScanArr));

}

void bib::setBIBSize(int col, int row, int io, int multi)
{
	m_nXsz = col;
	m_nYsz = row;
	m_nIoCnt = io;
	m_nDutCnt = col * row;
	m_nScanSize = 0;
	m_nMultiChip = multi;
	
	for (int i = 0; i < m_nDutCnt; i++)
	{
		dut* pDut = new dut(i);
		pDut->m_IOArr.resize(m_nIoCnt);
		int mc = gBIB.m_nMultiChip;
		if (mc == 0)mc = 1;
		if (gAPI.m_maskDuts[i / mc + i % mc])
			pDut->m_nDutMask = 1;

		m_DutArr.push_back(pDut);
	}
}

void bib::setBIBRow(int row, const char* dutInfo)
{
	char pBuf[8192];
	
	strcpy(pBuf, dutInfo);
		
	
	char* pstr;

	int stno = row * m_nXsz;
	int dno = stno;
	int scan_no, st, et, cnt, pinCnt;
	int pinArr[64];
	bool err = false;

	cstring str = "";
	token_t tok;

	for (pstr = strtok(pBuf, ","); pstr != NULL; pstr = strtok(NULL, ",")) {
		m_clScanner.setup(pstr);

		st = -1; cnt = 0;
		et = -1; pinCnt = 0;
		scan_no = 0;
		memset(pinArr, 0, sizeof(pinArr));
		while (1) {
			tok = m_clScanner.get();
//			printf("tok : %s\n", tok.s.c_str());
			if (tok.t == TOK_EOF)		// reached end of file, then exit
				break;
			if (tok.t == TOK_EOL)		// skip end of line
				continue;
			if (cnt == 0) // scan
			{
				if (tok.t != TOK_NUM)
				{
					gERR.addErr(ERR_BIB, __FUNCTION__, "context error");
					err = true;
					break;
				}
				scan_no = tok.n;
				cnt++;
			}
			else if (cnt == 1)
			{
				if (tok.s != "=")
				{
					gERR.addErr(ERR_BIB, __FUNCTION__, "context error");
					err = true;
					break;
				}
				cnt++;
			}
			else if (cnt > 1)
			{
				if (cnt % 2 == 0) { // num
					if (tok.t != TOK_NUM) {
						gERR.addErr(ERR_BIB, __FUNCTION__, "context error");
						err = true;
						break;
					}
					if (st == -1)
						st = tok.n;
					else
						et = tok.n;

					if (et != -1)
					{
						for (int i = st; i <= et; i++) {
							pinArr[pinCnt++] = i;
						}
						st = -1; et = -1;
					}
				}
				else {
					if (tok.s == "|") {
						if (st != -1) {
							pinArr[pinCnt++] = st;
							st = -1;
						}
					}
					else if (tok.s == "-") {

					}
					else {
						gERR.addErr(ERR_BIB, __FUNCTION__, "context error");
						err = true;
						break;
					}
				}
				cnt++;
				if (pinCnt > 63)break;
			}
//			printf("cnt : %d\n", cnt);
		}
		if (st != -1)
			pinArr[pinCnt++] = st;
		if (pinCnt != m_nIoCnt) {
			gERR.addErr(ERR_BIB, __FUNCTION__, "IO Count Size Over");
			err = true;
		}
		if (dno >= m_nDutCnt) {
			gERR.addErr(ERR_BIB, __FUNCTION__, "Dut Count Size Over");
			err = true;
		}
		if (err)return;

		dut* pDut = getDut(dno);
		pDut->m_nDutNo = dno;
		pDut->m_nScanNo = scan_no;
		if (scan_no >= m_nScanSize)
			m_nScanSize = scan_no + 1;
		for (int i = 0; i < m_nIoCnt; i++)
			pDut->m_IOArr[i] = pinArr[i];
		pDut->m_nXpos = dno - stno;
		pDut->m_nYpos = row;

		scan* pScan = getScan(scan_no);

		if (find(pScan->m_DutsScanArr.begin(), pScan->m_DutsScanArr.end(), dno) == pScan->m_DutsScanArr.end()) {			

			pScan->m_DutsScanArr.push_back(dno);
		}
		
		dno++;
	}
}

int bib::getDutOfScan(int scanNo, int DutIdx)
{
	if (scanNo >= m_nScanSize) return -1;
	scan* pScan = getScan(scanNo);
	if (DutIdx >= (int)pScan->m_DutsScanArr.size())
		return -1;
	return pScan->m_DutsScanArr[DutIdx];
}

int bib::getDutOfScanCnt(int scanNo)
{
	if (scanNo >= m_nScanSize) return -1;
	scan* pScan = getScan(scanNo);	
	return (int)pScan->m_DutsScanArr.size();
}




scan* bib::getScan(int idx)
{	
	if (idx < 0 || idx > 255)idx = 0; // 0~255

	return &m_ScanArr[idx];
}

dut* bib::getDut(int pos)
{
	if (pos >= (int)m_DutArr.size())return NULL;
	return m_DutArr[pos];
}

dut* bib::getDut(int x, int y)
{
	for (int i = 0; i < (int)m_DutArr.size(); i++)
	{
		dut* pDut = m_DutArr[i];
		if (pDut->m_nXpos == (int)x && pDut->m_nYpos == (int)y)
			return pDut;
	}

	return NULL;
}


byte* bib::getDutpData(int dut)
{
	if (dut >= (int)m_DutArr.size())return NULL;
	return getDut(dut)->pData;

}

void  bib::initCurResult()
{
	for (int i = 0; i < m_nDutCnt; i++)
	{
		dut* pDut = m_DutArr[i];
		if (pDut->m_nDutNo == -1)
		{
			gERR.addErr(ERR_BIB, __FUNCTION__, "BIB Is Not Set! Call First BIB_SIZE..");
			return;
		}

		if (gAPI.m_nTestMode == TEST_MODE_CONTACT) {
			//pDut->m_nConResult = STEP_PASS;
			pDut->m_nCurResult = STEP_PASS;			
			pDut->m_nTotalResult = STEP_PASS;
		}
		else {
			if (pDut->m_nConResult)
				pDut->m_nCurResult = STEP_PASS;
			else
				pDut->m_nCurResult = STEP_FAIL;
		}		

		pDut->m_nPrevTotal = pDut->m_nTotalResult;

		if (pDut->m_nDutMask)
		{
			pDut->m_nConResult = STEP_FAIL;
			pDut->m_nCurResult = STEP_FAIL;
			pDut->m_nPrevTotal = STEP_FAIL;
			pDut->m_nTotalResult = STEP_FAIL;
		}
		//printf("%s : dno[%d] cur %d tot %d mask %d\n", __FUNCTION__,
		//	pDut->m_nDutNo, pDut->m_nCurResult, 
		//	pDut->m_nTotalResult, pDut->m_nDutMask);
	}
}

void  bib::judge(bool bShow)
{
	m_nResultMode |= RES_FAIL_BIT;

	for (int i = 0; i < m_nScanSize; i++) {
		gFM.readFmComp(i);
		if (bShow) gFM.showFmComp(i);
	}

	for (int i = 0; i < m_nDutCnt; i++)
	{
		dut* pDut = m_DutArr[i];
		if (pDut->m_nDutNo == -1)
		{			
			gERR.addErr(ERR_BIB, __FUNCTION__, "BIB Is Not Set! Just Called BIB_SIZE..");
			return;
		}

		pDut->m_nFailBit = 0;
		int sc = pDut->m_nScanNo;
   
		byte* pbH = gFM.getFmCompH(sc);
		byte* pbL = gFM.getFmCompL(sc);
		
				
		for (int j = 0; j < (int)pDut->m_IOArr.size(); j++)
		{
		
			int io = pDut->m_IOArr[j];

			if (pbH[io / 8] & (1 << (io % 8))) {
				pDut->m_nCurResult = STEP_FAIL;
				if (m_nIoCnt != 0) pDut->m_nFailBit |= (1 << (io % m_nIoCnt));
			}
			if (m_bDDR){
				if (pbL[io / 8] & (1 << (io % 8))) {
					pDut->m_nCurResult = STEP_FAIL;
					if (m_nIoCnt != 0) pDut->m_nFailBit |= (1 << (io % m_nIoCnt));				
				}
			}
		}
		pDut->setResult();
	}

	gIO.writePG((1 << 9) | 28, 0, 1);
}



void bib::setValidScan()
{
	m_validScanVec.clear();
	for (int i = 0; i < m_nDutCnt; i++)
	{
		dut* pdut = getDut(i);
		if (pdut->m_nConResult == 0)continue;
		
		auto it = find(m_validScanVec.begin(), m_validScanVec.end(), pdut->m_nScanNo);
		if (it == m_validScanVec.end()) {			
			m_validScanVec.push_back(pdut->m_nScanNo);
		}
	}

	sort(m_validScanVec.begin(), m_validScanVec.end());

	printf("Valid Scan Count %d\n", m_validScanVec.size());
}

bool bib::getValidScan(int scan)
{
	auto it = find(m_validScanVec.begin(), m_validScanVec.end(), scan);
	if (it != m_validScanVec.end()) return true;	
	return false;
}

void bib::addResult(const char* sStep)
{
	for (int i = 0; i < (int)m_DutArr.size(); i++)
	{
		RESULT* pRes = new RESULT;
		sprintf(pRes->sStep, "%s", sStep);
		if (gAPI.m_nTestMode == TEST_MODE_CONTACT) pRes->bCon = true;
		else pRes->bCon = false;
		pRes->nResult = 0; // init fail

		getDut(i)->addRes(pRes);
		getDut(i)->m_nCurResult = pRes->nResult;

		//printf("%s : dno[%d] cur %d tot %d mask %d\n", __FUNCTION__,
		//	getDut(i)->m_nDutNo, getDut(i)->m_nCurResult,
		//	getDut(i)->m_nTotalResult, getDut(i)->m_nDutMask);
	}
}

void bib::initStepItem()
{
	m_nResultMode = RES_NONE;
	m_nCurScanNo = 0;	
	m_bResultInvert = false;
	
	initCurResult();
	for (int i = 0; i < m_nDutCnt; i++)
		getDut(i)->initCurFBlks();
}

