#include <vector>
#include <map>
#include "String.h"
#include "define.h"
#include "Scanner.h"
#include "bib.h"
#include "common.h"
#include "ioc.h"
#include "errctrl.h"
#include "failmem.h"

extern ioc 		gIO;
extern common 	gCOM;
extern errctrl 	gERR;
extern failmem	gFM;
extern bib		gBIB;

bib::bib()
{
	m_nScanSize = 0;
	m_nCurScanNo = 0;
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

scan* bib::getScan(int idx)
{	
	if (idx < 0 || idx > 255)idx = 0; // 0~255

	return &m_ScanArr[idx];
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

		if (gCOM.m_nTestMode == TEST_MODE_CONTACT) {
			pDut->m_nConResult = STEP_PASS;
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
	}
}

void  bib::judge(bool bShow)
{
	m_nResultMode |= RES_FAIL_BIT;

	for (int i = 0; i < m_nScanSize; i++) {
		gFM.readFmComp(i);
		if (bShow)
			gFM.showFmComp(i);
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

		byte* pbH[MAX_PG];
		byte* pbL[MAX_PG];
		
		for(int npg = 0; npg < MAX_PG; npg++) {
			pbH[npg] = gFM.getFmCompH(npg, sc);
			pbL[npg] = gFM.getFmCompL(npg, sc);
		}
		
		//int iost = pDut->m_IOArr[0];
		
		for (int j = 0; j < (int)pDut->m_IOArr.size(); j++)
		{
			//byte comp = pbH[iost / 8];
			int io = pDut->m_IOArr[j];
			int pg = pDut->m_nPGNo;

			//if(pDut->m_nDutNo == 168) {
			//	printf("Dut %d IO %d(%d) bH[%02x][%02X] result[%d]\n", pDut->m_nDutNo, io, io / 8, pbH[pg][io / 8], pbH[pg][io / 8] & (1 << (io % 8)), pDut->m_nCurResult);
			//}
			if(m_bDDR == 0 && m_bJudgeFirtst == 0) {
				if (pbL[pg][io / 8] & (1 << (io % 8))) {
					pDut->m_nCurResult = STEP_FAIL;
					if (m_nIoCnt != 0) pDut->m_nFailBit |= (1 << (io % m_nIoCnt));
				}
			}else{
				if (pbH[pg][io / 8] & (1 << (io % 8))) {
					pDut->m_nCurResult = STEP_FAIL;
					if (m_nIoCnt != 0) pDut->m_nFailBit |= (1 << (io % m_nIoCnt));
				}
			}
			
			if (m_bDDR){
				if (pbL[pg][io / 8] & (1 << (io % 8))) {
					pDut->m_nCurResult = STEP_FAIL;
					if (m_nIoCnt != 0) pDut->m_nFailBit |= (1 << (io % m_nIoCnt));
//					if (m_nIoCnt != 0) pDut->m_nFailBit |= ((1 << (io % m_nIoCnt)) << 16);
				}
			}
		}
		pDut->setResult();
	}

	gIO.writePG(PCI1, PG_ALL, (1 << 9) | 28, 0, 1);
	gIO.writePG(PCI2, PG_ALL, (1 << 9) | 28, 0, 1);
}

void dut::setResult()
{
	if (gCOM.m_nTestMode == TEST_MODE_CONTACT)
	{
		if (gBIB.m_bResultInvert)
			m_nCurResult == STEP_FAIL ? m_nCurResult = STEP_PASS: m_nCurResult = STEP_FAIL;

		m_nConResult &= m_nCurResult;
		m_nTotalResult &= m_nCurResult;
	}
	else if (gCOM.m_nTestMode == TEST_MODE_RUN || gCOM.m_nTestMode == TEST_MODE_SINGLE)
	{
		if (gBIB.m_bResultInvert)
			m_nCurResult == STEP_FAIL ? m_nCurResult = STEP_PASS : m_nCurResult = STEP_FAIL;

		m_nCurResult &= m_nConResult;
		m_nTotalResult &= m_nCurResult;
	}
//	printf("dno[%d] con %d, cur %d, total %d\n", m_nDutNo, m_nConResult, m_nCurResult, m_nTotalResult);
}

void dut::init() {
	m_nMask = 0;
	m_nResultBin = 0;
	memset(m_nFCat, 0, sizeof(m_nFCat));
	memset(m_nPCat, 0, sizeof(m_nPCat));
	m_sChipID = "";
	m_nDutNo = -1;
	m_nPGNo = -1;
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
	int pinArr[128];
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
			pDut->m_IOArr[i] = (pinArr[i] % MAX_IO_CNT);
		pDut->m_nXpos = dno - stno;
		pDut->m_nYpos = row;
		pDut->m_nPGNo = pinArr[0] / MAX_IO_CNT;

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

void bib::initStepItem()
{
	m_nResultMode = RES_NONE;
	m_nCurScanNo = 0;	
	m_bResultInvert = false;
	initCurResult();
	for (int i = 0; i < m_nDutCnt; i++)
		getDut(i)->initCurFBlks();
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
		if (gCOM.m_nTestMode == TEST_MODE_CONTACT) pRes->bCon = true;
		else pRes->bCon = false;
		pRes->nResult = 0; // init fail

		getDut(i)->addRes(pRes);
		getDut(i)->m_nCurResult = pRes->nResult;
	}
}
