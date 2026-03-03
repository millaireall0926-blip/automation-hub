#include <vector>
#include <map>
#include "String.h"
#include "Scanner.h"
#include "bib.h"
#include "errctrl.h"
#include "pgc.h"
#include "nand.h"
#include "common.h"
#include "define.h"
#include <math.h>
#include "result.h"
#include "STAR_HSBI.h"

#define RESULT_FILE		"/star/result/result.xml"
#define SUB_RESULT_FILE "/star/result/sub.xml"
#define CAT_FILE		"/star/result/cat.bin"



#define XS(h, id) gLIB.xml_att_find(h, id)
#define XI(h, id) atoi(gLIB.xml_att_find(h, id))
#define XB(h, id) (atoi(gLIB.xml_att_find(h, id)) == 1)
#define XSI(h, id, def) gLIB.csi(gLib->xml_att_find(h, id), def)
#define XF(h, id) atof(gLIB.xml_att_find(h, id))

#define XAS(h, id, v) gLIB.xml_att_sets(h, id, v)
#define XAI(h, id, v) gLIB.xml_att_setn(h, id, v)
#define XTEXT(h, v) gLIB.xml_set_text(h, v)


extern starlib	gLIB;
extern bib		gBIB;
extern errctrl	gERR;
extern pgc      gPG;
extern common	gCOM;
extern nand		gNAND;

result::result()
{
    init();
}

result::~result()
{

}

void result::init()
{
	m_hResult = 0;
	m_hSubResult = 0;
	memset(m_sResCurStep, 0, sizeof(m_sResCurStep));
}


void result::initResult()
{
	
	//int ret = remove(RESULT_FILE);
	//	printf("ret %d\n", ret);

	remove(RESULT_FILE);

	HND doc = gLIB.xml_docset(RESULT_FILE);
	gLIB.xml_parse(doc, "<test-result><info/><bib/></test-result>");
	m_hResult = doc;
	HND hResult = gLIB.xml_path_handle(doc, "/test-result");


	//	printf("%s %d %d %d\n", __FUNCTION__, doc, hBib, hLayout);
	cstring str;

	//////////////////  test-info ///////////////
	HND hInfo = gLIB.xml_path_handle(doc, "/test-result/info");
	time_t t = time(NULL);

	struct tm* tp = localtime(&t);
	strftime(m_sDate, sizeof(m_sDate), "%Y%m%dT%H%M%S", tp);
	XAS(hInfo, "date", m_sDate);
	XAS(hInfo, "pgm", gCOM.m_sPgm);
	XAS(hInfo, "lot", gCOM.m_sLot);
	XAS(hInfo, "bid", gCOM.m_sBid);


	//////////////////  bib ///////////////
	HND hBib = gLIB.xml_path_handle(doc, "/test-result/bib");
	HND hLayout = gLIB.xml_create("layout");
	XAI(hLayout, "x", gBIB.m_nXsz);
	XAI(hLayout, "y", gBIB.m_nYsz);
	XAI(hLayout, "io", gBIB.m_nIoCnt);
	XAI(hLayout, "m", gBIB.m_nMultiChip);
	XAI(hLayout, "sc", gBIB.m_nScanSize);
	

	for (int y = gBIB.m_nYsz - 1; y >= 0; y--)
	{
		HND e = gLIB.xml_create("line");
		int st = gBIB.m_nXsz * y;
		str.Format("");
		for (int x = 0; x < gBIB.m_nXsz; x++)
		{
			int dno = st + x;
			dut* pDut = gBIB.getDut(dno);
			if (pDut == NULL) {
				gERR.addErr(ERR_BIB, __FUNCTION__, "BIB Line Test Error x%d, y%d = idx%d", x, y, dno);
				continue;
			}
			//str.AppendFormat("%d=%d", pDut->m_nBIBScanNo, pDut->m_IOArr.at(0));
			if (gBIB.m_nIoCnt == 1) 
				str.AppendFormat("%d=%d", pDut->m_nScanNo, pDut->m_IOArr.at(0));
			else 
				str.AppendFormat("%d=%d-%d", pDut->m_nScanNo, pDut->m_IOArr.at(0), pDut->m_IOArr.at(pDut->m_IOArr.size() -1));

			if (x != gBIB.m_nXsz - 1)
				str.AppendFormat(",");
			//			printf("x%d str:%s\n", x, str.c_str());
		}
		XTEXT(e, str.c_str());
		gLIB.xml_add(hLayout, e);
		gLIB.xml_destroy(e);
	}
	gLIB.xml_add(hBib, hLayout);
	gLIB.xml_destroy(hLayout);

	//////////////////  scan ///////////////
	HND hScan = gLIB.xml_create("scanpin");
	for (int i = 0; i < gBIB.m_nScanSize; i++)
	{
		unsigned int pin = gBIB.getScan(i)->m_nScanValue;
		HND e = gLIB.xml_create("pin");
		XAI(e, "no", i);
		str.Format("0x%08X", pin);
		XTEXT(e, str.c_str());
		gLIB.xml_add(hScan, e);
		gLIB.xml_destroy(e);
	}
	gLIB.xml_add(hResult, hScan);
	gLIB.xml_destroy(hScan);


	gLIB.xml_save(doc);

	//////////////// etc //////////////
	resultAddStepList();
	
}

void result::freeResult()
{
	if (m_hResult > 0)
	{
		gLIB.xml_docfree(m_hResult);
		m_hResult = 0;
		printf("-- Release Result Handle -- \n");
	}
}

void result::loadResult()
{
	////////////////  load step  ////////////

	if (m_hResult)return;
	gLIB.xml_load(RESULT_FILE, &m_hResult);
	if (m_hResult <= 0) {
		// error no result file
		printf("%s load Fail\n", RESULT_FILE);
		return;
	}

	cstring str;
	str.Format("/test-result/info");
	HND h = gLIB.xml_path_handle(m_hResult, str.c_str());
	sprintf(gCOM.m_sLot, gLIB.xml_att_find(h, "lot"));

	str.Format("/test-result");  
	h = gLIB.xml_path_handle(m_hResult, str.c_str());
	HND hStep = gLIB.xml_last(h);
	if (strcmp(gLIB.xml_name(hStep), "bin") == 0)
		hStep = gLIB.xml_prev(hStep);

	HND eh = gLIB.xml_child(hStep);
	
	char sText[1024];
	if (eh) {			

		sprintf(sText, "%s", gLIB.xml_text(eh));		
		int cnt = 0;
		bool finish = false;
		for (char* tok = strtok(sText, ","); tok != NULL; tok = strtok(NULL, ","))
		{
			if (finish)break;
			uint val = strtol(tok, NULL, 16);
			for (int i = 0; i < 32; i++)
			{
				int idx = cnt * 32 + i;
				if (idx >= gBIB.m_nDutCnt) { finish = true; break; }
				int comp = 1 << i;
				if ((val & comp) == 0) gBIB.getDut(idx)->m_nConResult = STEP_FAIL;
				else gBIB.getDut(idx)->m_nConResult = STEP_PASS;				
			}
			cnt++;
		}
		eh = gLIB.xml_next(eh);
	}


	if (eh) {
		sprintf(sText, "%s", gLIB.xml_text(eh));
		int cnt = 0;
		bool finish = false;
		for (char* tok = strtok(sText, ","); tok != NULL; tok = strtok(NULL, ","))
		{
			if (finish)break;
			uint val = strtol(tok, NULL, 16);
			for (int i = 0; i < 32; i++)
			{
				int idx = cnt * 32 + i;
				if (idx >= gBIB.m_nDutCnt) { finish = true; break; }
				int comp = 1 << i;
				if ((val & comp) == 0) gBIB.getDut(idx)->m_nCurResult = STEP_FAIL;
				else gBIB.getDut(idx)->m_nCurResult = STEP_PASS;
			}
			cnt++;
		}
		eh = gLIB.xml_next(eh);
	}


	if (eh) {
		sprintf(sText, "%s", gLIB.xml_text(eh));
		int cnt = 0;
		bool finish = false;
		for (char* tok = strtok(sText, ","); tok != NULL; tok = strtok(NULL, ","))
		{
			if (finish)break;
			uint val = strtol(tok, NULL, 16);
			for (int i = 0; i < 32; i++)
			{
				int idx = cnt * 32 + i;
				if (idx >= gBIB.m_nDutCnt) { finish = true; break; }
				int comp = 1 << i;
				if ((val & comp) == 0) gBIB.getDut(idx)->m_nTotalResult = STEP_FAIL;
				else gBIB.getDut(idx)->m_nTotalResult = STEP_PASS;
			}
			cnt++;
		}
		eh = gLIB.xml_next(eh);
	}

	gBIB.setValidScan();
}

void result::initCatFile()
{
	FILE* f = NULL;
	if ((f = fopen(CAT_FILE, "wb")) == NULL) return;


	for (int i = 0; i < gBIB.m_nDutCnt; i++)
	{
		dut* pDut = gBIB.getDut(i);
		fwrite(pDut->m_nPCat, sizeof(pDut->m_nPCat), 1, f);
		fwrite(pDut->m_nFCat, sizeof(pDut->m_nFCat), 1, f);

	}
	fclose(f);
}

void result::binResult()
{
	if (m_hResult <= 0) return;

	HND hBin = gLIB.xml_path_handle(m_hResult, "/test-result/bin");
	if (hBin) {
		gLIB.xml_remove(hBin);
		gLIB.xml_save(m_hResult);
	}

	hBin = gLIB.xml_create("bin");
	cstring sBin = ""; sBin.Format("");
	for (int i = 0; i < gBIB.m_nDutCnt; i++)
	{
		sBin.AppendFormat("%d,", gBIB.getDut(i)->m_nResultBin);
	}

	XTEXT(hBin, sBin.c_str());
	HND h = gLIB.xml_path_handle(m_hResult, "/test-result");
	gLIB.xml_add(h, hBin);
	gLIB.xml_destroy(hBin);
	gLIB.xml_save(m_hResult);
}

void result::loadCatFile()
{
	FILE* f = NULL;
	if ((f = fopen(CAT_FILE, "rb")) == NULL) return;
	fseek(f, 0, SEEK_SET);

	for (int i = 0; i < gBIB.m_nDutCnt; i++)
	{
		dut* pDut = gBIB.getDut(i);
		fread(pDut->m_nPCat, sizeof(pDut->m_nPCat), 1,  f);
		fread(pDut->m_nFCat, sizeof(pDut->m_nFCat), 1, f);

		for (int j = 0; j < 32 * 32; j++)
		{
			int idx = j / 32;
			int rem = j % 32;
			if (pDut->m_nFCat[idx] & (1 << rem))pDut->m_FCatArr.push_back(j);
			if (pDut->m_nPCat[idx] & (1 << rem))pDut->m_PCatArr.push_back(j);
		}
	}
	fclose(f);

}

void result::resultAddStepList()
{
	if (m_hResult <= 0) return;
	HND h = gLIB.xml_path_handle(m_hResult, "/test-result/step-list");

	if (h)return;

	

	HND e = gLIB.xml_create("step-list");
	cstring str;
	str.Format("");
	for (int i = 0; i < gCOM.m_nStepIdx; i++)
	{
		str.AppendFormat("%s", gCOM.m_sStepName[i]);
		if (i <= gCOM.m_nStepIdx - 1)
			str.AppendFormat(",");
	}

	XTEXT(e, str.c_str());
//	printf("Step List %d %s\n", gAPI.m_nStepIdx, str.c_str());
	h = gLIB.xml_path_handle(m_hResult, "/test-result");
	gLIB.xml_add(h, e);
	gLIB.xml_destroy(e);
	gLIB.xml_save(m_hResult);
}

int result::startStepRes()
{
	if (m_hResult <= 0) return 0;

	cstring str;
	str.Format("/test-result/step.id=%s", gCOM.m_sCurStep);
	HND hStep = gLIB.xml_path_handle(m_hResult, str.c_str());
	strcpy(m_sResCurStep, gCOM.m_sCurStep);

	

	if (hStep) {
		int idx = 1;
		while (1)
		{
			char sIdx[16];
			sprintf(sIdx, "~RE%d", idx);
			strcpy(m_sResCurStep, gCOM.m_sCurStep);
			strcat(m_sResCurStep, sIdx);			
			str.Format("/test-result/step.id=%s", m_sResCurStep);			
			hStep = gLIB.xml_path_handle(m_hResult, str);
			if (hStep == 0)break;
			idx++;
		}
	}

	hStep = gLIB.xml_create("step");

	XAS(hStep, "id",  m_sResCurStep);

	int con = 0;
	if (gCOM.m_nTestMode == 1)con = 1;
	XAI(hStep, "con", con );

	time_t t = time(NULL);
	char ts[40];
	struct tm* tp = localtime(&t);
	strftime(ts, sizeof(ts), "%Y%m%dT%H%M%S", tp);

	XAS(hStep, "start", ts);

	HND h = gLIB.xml_path_handle(m_hResult, "/test-result");
	gLIB.xml_add(h, hStep);
	gLIB.xml_destroy(hStep);
	gLIB.xml_save(m_hResult);

	return 1;
}

int result::endStepRes()
{
	if (m_hResult <= 0) return 0;
	
	cstring str;
	str.Format("/test-result/step.id=%s", m_sResCurStep);
	HND hStep = gLIB.xml_path_handle(m_hResult, str.c_str());
	if (hStep <= 0) return 0;
	
	time_t t = time(NULL);
	char ts[40];
	struct tm* tp = localtime(&t);
	strftime(ts, sizeof(ts), "%Y%m%dT%H%M%S", tp);

	XAS(hStep, "end", ts);

	cstring sCon = ""; sCon.Format("");
	cstring sCur = ""; sCur.Format("");
	cstring sTotal = ""; sTotal.Format("");
	

	uint nCon = 0;
	uint nCur = 0;
	uint nTotal = 0;
	int i = 0;
	for (i = 0; i < gBIB.m_nDutCnt; i++)
	{
		if (i != 0 && (i % 32 == 0)) {
			sCon.AppendFormat("0x%08x,", nCon);
			nCon = 0;
			sCur.AppendFormat("0x%08x,", nCur);
			nCur = 0;
			sTotal.AppendFormat("0x%08x,", nTotal);
			nTotal = 0;

		}
		if (gBIB.getDut(i)->m_nConResult)
			nCon |= 1 << (i % 32);
		if (gBIB.getDut(i)->m_nCurResult)
			nCur |= 1 << (i % 32);
		if (gBIB.getDut(i)->m_nTotalResult)
			nTotal |= 1 << (i % 32);
		

//		printf("dno[%d] cur %d tot %d cum0x%08x\n", i, gBIB.getDut(i)->m_nCurResult, gBIB.getDut(i)->m_nTotalResult, nTotal);
	}	

	sCon.AppendFormat("0x%08x,", nCon);	
	sCur.AppendFormat("0x%08x,", nCur);
	sTotal.AppendFormat("0x%08x,", nTotal);

	HND h = gLIB.xml_create("con");
	XTEXT(h, sCon.c_str());
	gLIB.xml_add(hStep, h);
	gLIB.xml_destroy(h);
	gLIB.xml_save(m_hResult);
	
	h = gLIB.xml_create("cur");
	XTEXT(h, sCur.c_str());
	gLIB.xml_add(hStep, h);
	gLIB.xml_destroy(h);
	gLIB.xml_save(m_hResult);
	
	h = gLIB.xml_create("total");
	XTEXT(h, sTotal.c_str());
	gLIB.xml_add(hStep, h);
	gLIB.xml_destroy(h);
	gLIB.xml_save(m_hResult);
	


	return 1;
}

void result::saveResOption()
{
	cstring str;
	HND hStep;
	if (m_hResult != 0) {
		str.Format("/test-result/step.id=%s", m_sResCurStep);
		hStep = gLIB.xml_path_handle(m_hResult, str.c_str());		
	}

	int i = 0;
	if (gBIB.m_nResultMode & RES_FAIL_BIT) {		
		HND fh = gLIB.xml_create("fbit");
		for (i = 0; i < gBIB.m_nDutCnt; i++)
		{
			dut* pDut = gBIB.getDut(i);
			if (pDut->m_nCurResult == STEP_FAIL)
			{
				if ((gCOM.m_nTestMode == TEST_MODE_CONTACT) || pDut->m_nConResult == STEP_PASS)
				{
					HND dh = gLIB.xml_create("d");
					XAI(dh, "no", i);
					cstring xbit;
					xbit.Format("%x", pDut->m_nFailBit);
					XAS(dh, "fb", xbit);
					gLIB.xml_add(fh, dh);
					gLIB.xml_destroy(dh);			
				}
			}
		}
		gLIB.xml_add(hStep, fh);
		gLIB.xml_destroy(fh);
		gLIB.xml_save(m_hResult);
	}

	if (gBIB.m_nResultMode & RES_CHIP_ID) {
		HND fh = gLIB.xml_create("chipid");
		for (i = 0; i < gBIB.m_nDutCnt; i++)
		{
			dut* pDut = gBIB.getDut(i);
			if (pDut->m_sChipID == "")continue;

			HND dh = gLIB.xml_create("d");
			XAI(dh, "no", i);
			XTEXT(dh, pDut->m_sChipID.c_str());
			gLIB.xml_add(fh, dh);
			gLIB.xml_destroy(dh);				
		}
		gLIB.xml_add(hStep, fh);
		gLIB.xml_destroy(fh);
		gLIB.xml_save(m_hResult);
	}

	//printf("m_nResultMode %04x\n", gBIB.m_nResultMode);

	if (gBIB.m_nResultMode & RES_BLOCK_ADR)
	{
		HND fh = gLIB.xml_create("sub");
		XAS(fh, "id"	, m_sResCurStep);
		XAS(fh, "lot", gCOM.m_sLot);		
		XAS(fh, "type", "blk");

		gLIB.xml_add(hStep, fh);
		gLIB.xml_destroy(fh);
		gLIB.xml_save(m_hResult);

		// save sub file
		initSubResult();
	}
	
	if (gBIB.m_nResultMode & RES_BLOCK_COUNT)
	{
		HND fh = gLIB.xml_create("sub");				
		XAS(fh, "id", m_sResCurStep);
		XAS(fh, "lot", gCOM.m_sLot);		
		XAS(fh, "type", "fc");
		gLIB.xml_add(hStep, fh);
		gLIB.xml_destroy(fh);
		gLIB.xml_save(m_hResult);

		// save sub file
		initSubResult();
	}

	if (gBIB.m_nResultMode & RES_BLOCK_COUNT_DETAIL)
	{
		HND fh = gLIB.xml_create("sub");				
		XAS(fh, "id", m_sResCurStep);
		XAS(fh, "lot", gCOM.m_sLot);		
		XAS(fh, "type", "fc-d");
		gLIB.xml_add(hStep, fh);
		gLIB.xml_destroy(fh);
		gLIB.xml_save(m_hResult);
		
		// RES_BLOCK_COUNT_DETAIL Save Sub File When Test Process
		
	}

	if (gBIB.m_nResultMode & RES_DATA_READ)
	{
		HND fh = gLIB.xml_create("sub");
		XAS(fh, "id", m_sResCurStep);
		XAS(fh, "lot", gCOM.m_sLot);
		XAS(fh, "type", "data");
		cstring str = ""; str.Format("");
		for (int i = 0; i < gBIB.m_nDutCnt; i++)
		{
			
			dut* pDut = gBIB.getDut(i);
			if (pDut->m_nConResult == STEP_FAIL)continue;
			str.AppendFormat("%d,", i);
			
		}
		XTEXT(fh, str);
		
		gLIB.xml_add(hStep, fh);
		gLIB.xml_destroy(fh);
		gLIB.xml_save(m_hResult);
	}

	if (gBIB.m_nResultMode & RES_BLOCK_CYCLE)
	{
		HND fh = gLIB.xml_create("sub");
		XAS(fh, "id", m_sResCurStep);
		XAS(fh, "lot", gCOM.m_sLot);
		XAS(fh, "type", "cycle");
		gLIB.xml_add(hStep, fh);
		gLIB.xml_destroy(fh);
		gLIB.xml_save(m_hResult);

		makeCycleResult();
	}
}

void result::makeCycleResult()
{
	FILE* pf = fopen("/star/result/sub.txt", "wt");
	if (pf == NULL)return;
	if (gNAND.m_nCycleTestCnt == 0)gNAND.m_nCycleTestCnt = 1;
	fprintf(pf, "Item\n", m_sResCurStep);
	fprintf(pf, "Step,%s\n", m_sResCurStep);
	fprintf(pf, "CycleCount,%d\n", gNAND.m_nCycleTestCnt);
	fprintf(pf, "BlockCount,%d\n", gNAND.m_nCycleBlock);

	for (int i = 0; i < gBIB.m_nDutCnt; i++)
	{
		dut* pDut = gBIB.getDut(i);
		if (pDut->m_nConResult == STEP_FAIL)continue;

		fprintf(pf, "\n");

		fprintf(pf, "Dut,%d\n", i);
		fprintf(pf, "EraseFailCount,%d\n", pDut->m_nFBlkEraseArr.size());
		fprintf(pf, "EraseFail,");
		for (int j = 0; j < (int)pDut->m_nFBlkEraseArr.size(); j++)
			fprintf(pf, "%d,", pDut->m_nFBlkEraseArr.at(j));
		fprintf(pf, "\n");

		fprintf(pf, "ProgramFailCount,%d\n", pDut->m_nFBlkProgArr.size());
		fprintf(pf, "ProgramFail,");
		for (int j = 0; j < (int)pDut->m_nFBlkProgArr.size(); j++)
			fprintf(pf, "%d,", pDut->m_nFBlkProgArr.at(j));
		fprintf(pf, "\n");


		fprintf(pf, "MaxEraseRB,");
		for (int j = 0; j < gNAND.m_nCycleBlock; j++)
			fprintf(pf, "%.2f,", ((double)pDut->m_nMaxEraseBlock[j]) * gNAND.m_fBaseRate US);
		fprintf(pf, "\n");

		fprintf(pf, "SumEraseRB,");
		for (int j = 0; j < gNAND.m_nCycleBlock; j++)
			fprintf(pf, "%.2f,", ((double)pDut->m_nSumEraseBlock[j]) * gNAND.m_fBaseRate US);
		fprintf(pf, "\n");
				
		fprintf(pf, "AverageEraseRB,");
		for (int j = 0; j < gNAND.m_nCycleBlock; j++)
			fprintf(pf, "%.2f,", (((double)(pDut->m_nSumEraseBlock[j] / gNAND.m_nCycleTestCnt) * gNAND.m_fBaseRate US)));
		fprintf(pf, "\n");


		fprintf(pf, "MaxProgramRB,");
		for (int j = 0; j < gNAND.m_nCycleBlock; j++)                 // NS           ,  MS
			fprintf(pf, "%.2f,", ((double)pDut->m_nMaxProgBlock[j]) * gNAND.m_fBaseRate US);
		fprintf(pf, "\n");

		fprintf(pf, "SumProgramRB,");
		for (int j = 0; j < gNAND.m_nCycleBlock; j++)
			fprintf(pf, "%.2f,", ((double)pDut->m_nSumProgBlock[j]) * gNAND.m_fBaseRate US);
		fprintf(pf, "\n");

		fprintf(pf, "AverageProgramRB,");
		for (int j = 0; j < gNAND.m_nCycleBlock; j++)
			fprintf(pf, "%.2f,", (((double)(pDut->m_nSumProgBlock[j] / gNAND.m_nCycleTestCnt)) * gNAND.m_fBaseRate US));
		fprintf(pf, "\n");
		
	}
	fflush(pf);
	fclose(pf);

}

void result::freeSubResult()
{
	if (m_hSubResult > 0)
	{
		gLIB.xml_docfree(m_hSubResult);
		m_hSubResult = 0;
		printf("-- Release Sub Handle -- \n");
	}
}

void result::initSubResult(bool bUseFree)
{
	freeSubResult();
	remove(SUB_RESULT_FILE);

	HND doc = gLIB.xml_docset(SUB_RESULT_FILE);
	gLIB.xml_parse(doc, "<sub></sub>");
	m_hSubResult = doc;
	HND hSub = gLIB.xml_path_handle(doc, "/sub");
	XAS(hSub, "id", m_sResCurStep);	
	XAS(hSub, "lot", gCOM.m_sLot);
		
	if (gBIB.m_nResultMode & RES_BLOCK_ADR)
	{
		XAS(hSub, "type", "blk");
		HND fh = gLIB.xml_create("blk");
		XAI(fh, "mask", gNAND.m_nMaskMode);
		for (int i = 0; i < gBIB.m_nDutCnt; i++)
		{
			dut* pDut = gBIB.getDut(i);
			if (pDut->m_nConResult == STEP_FAIL)continue;
			if (pDut->m_maskFBlkCnt == 0 && pDut->m_curFBlkCnt == 0 && pDut->m_totalFBlkCnt == 0) continue;

			int blkCnt = 0;
			int wCnt = 0; // Write Cnt

			cstring sBlks = ""; sBlks.Format("");
			if (gBIB.m_nResultMode & RES_BLOCK_ADR_IMP)
			{
				for (int adr = 0; adr < MAX_BLOCK_ADR; adr++)
				{
					if (gNAND.m_nMaskMode & MASK) {
						if (pDut->m_maskFBlks[adr / 32] & (1 << (adr % 32))) {
							sBlks.AppendFormat(".%04x ", adr); // org mask
							wCnt++;
						}
					}
					if (gNAND.m_nMaskMode & TOTAL) {
						if (pDut->m_totalFBlks[adr / 32] & (1 << (adr % 32))) {
							sBlks.AppendFormat("%04x ", adr);// org fail
							wCnt++;
						}
					}

					if (wCnt >= (pDut->m_maskFBlkCnt + pDut->m_totalFBlkCnt))
						break;
				}
			}
			else {
				for (int adr = 0; adr < MAX_BLOCK_ADR; adr++)
				{
					if (pDut->m_curFBlks[adr / 32] & (1 << (adr % 32))) {
						//					printf("dno %d adr %x", i, adr);
						if (gNAND.m_nMaskMode & TOTAL) {
							if (!(pDut->m_maskFBlks[adr / 32] & (1 << (adr % 32))))
							{
								if (!(pDut->m_totalFBlks[adr / 32] & (1 << (adr % 32)))) {
									pDut->m_totalFBlks[adr / 32] |= (1 << (adr % 32));
									pDut->m_totalFBlkCnt++;
									sBlks.AppendFormat("!%04x ", adr);// new Fail
								}
								else {
									sBlks.AppendFormat("%04x ", adr);// org fail
								}
								wCnt++;
							}
							else
							{
								if (gBIB.m_nResultMode & RES_BLOCK_ADR_BOTH)
								{
									sBlks.AppendFormat(".%04x ", adr); // org mask
									wCnt++;
								}
							}
						}
						if (gNAND.m_nMaskMode & MASK) {
							if (!(pDut->m_maskFBlks[adr / 32] & (1 << (adr % 32)))) {
								pDut->m_maskFBlks[adr / 32] |= (1 << (adr % 32));
								pDut->m_maskFBlkCnt++;
								sBlks.AppendFormat("*%04x ", adr); // new mask
							}
							else {
								sBlks.AppendFormat(".%04x ", adr); // org mask
							}
							wCnt++;
						}
//						printf("\n");

						blkCnt++;
					}

					if (blkCnt >= pDut->m_curFBlkCnt) break;

				}
			}

			if (wCnt != 0)
			{
				HND dh = gLIB.xml_create("d");
				XAI(dh, "n", i);
				XAI(dh, "c", wCnt);
				XTEXT(dh, sBlks.c_str());
				gLIB.xml_add(fh, dh);
				gLIB.xml_destroy(dh);
			}
		}

		gLIB.xml_add(hSub, fh);
		gLIB.xml_destroy(fh);
	}
	else if (gBIB.m_nResultMode & RES_BLOCK_COUNT)
	{
		XAS(hSub, "type", "fc");
		HND fh = gLIB.xml_create("fc");		
		XAI(fh, "mask", gNAND.m_nMaskMode);
		for (int i = 0; i < gBIB.m_nDutCnt; i++)
		{
			dut* pDut = gBIB.getDut(i);
			//printf("RES_BLOCK_COUNT dno %d\n", pDut->m_nDutNo);
			if (pDut->m_nConResult == STEP_FAIL)continue;			
			int blkCnt = (int)pDut->m_mapBlockFails.size();
			//printf("-> dno %d sz %d\n", pDut->m_nDutNo, blkCnt);
			if (blkCnt == 0) continue;
			cstring sBlks = ""; sBlks.Format("");

			
			for (auto iter = pDut->m_mapBlockFails.begin(); iter != pDut->m_mapBlockFails.end(); iter++)
			{
				sBlks.AppendFormat("%04x:%04x ", iter->first, iter->second);
			}

			HND dh = gLIB.xml_create("d");
			XAI(dh, "n", i);
			XAI(dh, "c", blkCnt);
			XTEXT(dh, sBlks.c_str());
			gLIB.xml_add(fh, dh);
			gLIB.xml_destroy(dh);
		}
		gLIB.xml_add(hSub, fh);
		gLIB.xml_destroy(fh);


	}
	else if (gBIB.m_nResultMode & RES_BLOCK_COUNT_DETAIL)
	{
		XAS(hSub, "type", "fc-d");
	}

//	int len = gLIB.xml_docsrc_len(doc);
//	char* scheck = new char[len+10];
//	gLIB.xml_docsrc(doc, scheck);
//	printf("sub file :  %s\n", scheck);
//	delete[] scheck;
	gLIB.xml_save(doc);
	m_hSubResult = doc;

	if (bUseFree)
		freeSubResult();
}

