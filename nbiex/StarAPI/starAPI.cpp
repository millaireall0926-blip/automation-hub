#include "starAPI.h"
#include "define.h"
#include "../starlib/starlib_dif.h"
#include "STAR_TEST.h"
#include "ioc.h"
#include "pgc.h"
#include "net.h"
#include "errc.h"


#include <stdio.h>
#include <map>
extern starlib	g_clLib;
extern ioc		g_clIO;
//extern pgc	g_clPgc;
extern net		g_clNet;
extern errctrl	g_clErr;

extern std::map<std::string, void (*)()> _mapStepFunc;
extern bool _bAddStepMap;

#define DEBUG_FILE "/star/result/debug.log"
#define CONFIG_FN "/star/test/config.xml"
#define BTEMP_FILE "/star/result/btemp.csv"

#define PRINT_DEBUG 0

starAPI::starAPI()
{
	m_nTestMode = 0;
	m_nStepIdx = 0;
	m_bUseNet = false;
	m_bNoDPS = false;
	m_bMonDPS = false;
	m_bUseHandler = false;

	m_fDebug = fopen(DEBUG_FILE, "wt");
	m_fBTemp = fopen(BTEMP_FILE, "wt");
	memset(m_maskDuts, 0, sizeof(m_maskDuts));
}

void starAPI::debug(const char* str, ...)
{
	va_list	ap;
	char buf[2048];
	va_start(ap, str);
	vsprintf(buf, str, ap);
	va_end(ap);

	if (m_fDebug != NULL) {
		fprintf(m_fDebug, "%s\n", buf);
		fflush(m_fDebug);
	}

#if PRINT_DEBUG
	printf("%s\n", buf);
#endif
}

void starAPI::logBTemp(const char* str, ...)
{
	va_list	ap;
	char buf[2048];
	va_start(ap, str);
	vsprintf(buf, str, ap);
	va_end(ap);

	if (m_fBTemp != NULL) {
		fprintf(m_fBTemp, "%s\n", buf);
		fflush(m_fBTemp);
	}
}


int starAPI::loadStarLib()
{
	switch (gLIB.load()) {
	case -1:
		printf("can't open starlib.so\n");
		return -1;
	case -2:
		printf("incompatible version of starlib.so\n");
		return -2;
	}

	
	return 1;
}

int starAPI::loadConfig()
{
	HND h;
	if (!gLIB.xml_load(CONFIG_FN, &h)) {
		printf("config file '%s' not found\n", CONFIG_FN);
		return 0;
	}

	HND fh = gLIB.xml_path_handle(h, "/config/fpga");
	m_nPGID = 0;
	if (fh < 0) return 0;

	char id[64];
	strcpy(id, gLIB.xml_att_find(fh, "id"));
	char* p, * q;
	p = id;
	q = p + strlen(id);
	m_nPGID =  strtoul(p, &q, 16);
	printf("config FPGA : s:%s, d:%x\n", id, m_nPGID);
	
	return 1;
}



int starAPI::doArgument(int argc, char** argv)
{
	char* ptr;

	int pathLen = 10;

	if (strncmp(argv[0], "/star/pgm/", pathLen) != 0) 
		pathLen = 0;	
	sprintf(m_sPgm, argv[0] + pathLen);

	for (int i = 1; i < argc; i++)
	{
		if (strncmp(argv[i], "-load", 5) == 0) {  //run
			m_nTestMode = TEST_MODE_LOAD;
		}
		else if (strncmp(argv[i], "-con", 4) == 0) {  // contact 
			m_nTestMode = TEST_MODE_CONTACT;
		}
		else if (strncmp(argv[i], "-run", 4) == 0) {  //run
			m_nTestMode = TEST_MODE_RUN;		
		}		
		else if (strncmp(argv[i], "-handle", 7) == 0) {  //run	
			m_nTestMode = TEST_MODE_HANDLER;
		}else if (strncmp(argv[i], "-init", 5) == 0) {  //run	
			m_nTestMode = TEST_MODE_INIT;
		}
		else if (strncmp(argv[i], "-single=", 8) == 0) {  //run
			m_nTestMode = TEST_MODE_SINGLE;
			ptr = argv[i] + 8;
			for (char* tok = strtok(ptr, ","); tok != NULL; tok = strtok(NULL, ","))
			{
				cstring str;
				str.Format("%s", tok);
				m_singleStepVec.push_back(str);
			}
			//strcpy(m_sSingleStep, ptr);
		}
		else if (strncmp(argv[i], "-diag", 5) == 0) {  //diag
			m_nTestMode = TEST_MODE_DIAG;
			gNET.setType(1);
		}	
		else if (strncmp(argv[i], "-cmd", 4) == 0) {  //diag
			m_nTestMode = TEST_MODE_CMD;			
		}
		else if (strncmp(argv[i], "-darg=", 6) == 0) {  // diag argument
			ptr = argv[i] + 6;
			char* stop;
			//m_nDiagTestSel = atoi(ptr);
			m_nDiagTestSel = strtol(ptr, &stop, 16);
		//	printf("darg = 0x%x", m_nDiagTestSel);
		}
		else if (strncmp(argv[i], "-lot=", 5) == 0) {  // contact 
			ptr = argv[i] + 5;
			strcpy(m_sLot, ptr);
		}
		else if (strncmp(argv[i], "-net", 4) == 0) {  // contact 
			m_bUseNet = true;
		}
		else if (strncmp(argv[i], "-sim", 4) == 0) {  // contact 
			gIO.m_bSim = true;
		}
		else if (strncmp(argv[i], "-nodps", 6) == 0) {  // contact 
			m_bNoDPS = true;
		}
		else if (strncmp(argv[i], "-mon", 4) == 0) {  // contact 
			m_bMonDPS = true;
		}
		else if (strncmp(argv[i], "-mask=", 5) == 0)
		{
			char* maskStr = argv[i] + 6; // "-mask=" 건너뛰기
			char sText[1024];
			strncpy(sText, maskStr, sizeof(sText) - 1);
			sText[sizeof(sText) - 1] = '\0';
		
			// '*' 이전까지만 사용
			char* starPos = strchr(sText, '*');
			if (starPos)
				*starPos = '\0';
			int bitCount = 0;
			for (char* tok = strtok(sText, ","); tok != NULL; tok = strtok(NULL, ","))
			{
				if (tok == NULL)break;
		
				unsigned long value = strtoul(tok, NULL, 16); // 16진수 변환
						
				for (int bit = 0; bit < 32; bit++)
				{
					m_maskDuts[bitCount++] = (value >> bit) & 1;
				}
			}
		
			printf("Total bits: %d\n", bitCount);
			for (int i = 0; i < bitCount; i++)
			{
				printf("%d", m_maskDuts[i]);
				if ((i + 1) % 32 == 0) printf(" ");
			}
			printf("\n");
			
		}
	}

	return 1;
}

void starAPI::singleStep(const char* step)
{
	start_step(step);

	int nFind = 0; // 1 = No ARG, 2 = ARG, 3 = No Allow
	auto iter = _mapStepFunc.find(step);
	if (iter != _mapStepFunc.end()) nFind = 1;
	

	auto iterI = _mapStepIFunc.find(step);
	if (iterI != _mapStepIFunc.end()) nFind += 2;

	if (nFind == 0) {
		gERR.addErr(ERR_RUNTIME, __FUNCTION__, "STEP(%s)/STEPI(%s) Not Found", step);
		return;
	}

	if (nFind == 1)
		_mapStepFunc[step]();
	else if (nFind == 2)
		_mapStepIFunc[step](_mapStepIArg[step]);
	end_step();
}

void starAPI::initHW()
{
	//gPG.stopPG();
}