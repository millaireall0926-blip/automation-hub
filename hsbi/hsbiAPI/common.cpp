#include "common.h"
#include "errctrl.h"
#include "../starlib/starlib_dif.h"
#include "define.h"
#include "net.h"
#include "dps.h"
#include "ioc.h"
#include <string.h>

#define DEBUG_FILE "/star/result/debug.log"
#define CONFIG_FN "/star/test/config.xml"

extern starlib	gLIB;
extern net      gNET;
extern errctrl	gERR;
extern dps	    gDPS;
extern ioc		gIO;

extern void start_step(const char * name);
extern void end_step();

extern std::map<std::string, void (*)()> _mapStepFunc;
extern std::map<std::string, void (*)(int)> _mapStepIFunc;
extern std::map<std::string, int> _mapStepIArg;

extern bool _bAddStepMap;

common::common()
{
	m_nTestMode = 0;
	m_nStepIdx = 0;
	m_fDebug = fopen(DEBUG_FILE, "wt");
	m_nPGID[0] = 0xaabbccd4;
	m_nPGID[1] = 0xaabbccd5;
	m_nPGID[2] = 0xaabbccd6;
	m_nPGID[3] = 0xaabbccd7;
}

common::~common()
{

}

void common::debug(const char* str, ...)
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
}


int common::loadStarLib()
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

int common::loadConfig()
{
	HND h;
	if (!gLIB.xml_load(CONFIG_FN, &h)) {
		printf("config file '%s' not found\n", CONFIG_FN);
		return 0;
	}

	HND fh = gLIB.xml_path_handle(h, "/config/fpga");
	//m_nPGID = 0;
	m_nPGID[0] = 0xaabbccd4;
	if (fh < 0) return 0;

	char id[64];
	strcpy(id, gLIB.xml_att_find(fh, "pg1-id"));
	char* p, * q;
	p = id;
	q = p + strlen(id);
	m_nPGID[0] =  strtoul(p, &q, 16);
	printf("config FPGA : s:%s, d:%x\n", id, m_nPGID[0]);

	HND eh = gLIB.xml_path_handle(h, "/config/pgm");
	if (eh)
	{
		if (atoi(gLIB.xml_att_find(eh, "hide-dps-err"))) {
			gDPS.m_bHideDPSErr = true;
			gIO.m_bHideDPSErr = true;				
		 }

	}

	
	return 1;
}

int common::doArgument(int argc, char** argv)
{
	char* ptr;

	int pathLen = 10;

	if (strncmp(argv[0], "/star/pgm/", pathLen) != 0)
		pathLen = 0;
	sprintf(m_sPgm, argv[0] + pathLen);

	for (int i = 1; i < argc; i++)
	{		
		if (strncmp(argv[i], "-con", 4) == 0) {  // contact 
			m_nTestMode = TEST_MODE_CONTACT;
		}
		else if (strncmp(argv[i], "-run", 4) == 0) {  //run
			m_nTestMode = TEST_MODE_RUN;
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
		else if (strncmp(argv[i], "-sim", 4) == 0) {  // simulation
			printf("sim test\n");
			gIO.m_bSim = true;
		}
	}

	return 1;
}

void common::singleStep(const char* step)
{
	start_step(step);

	printf("singleStep start\n");

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

	printf("singleStep end\n");
}
