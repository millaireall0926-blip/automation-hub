#ifndef __COMMON__
#define __COMMON__
//	Division 
//		- Operation 

#include <stdio.h>
#include <map>
#include <vector>
#include "String.h"
#include "define.h"

//enum {	
//	TEST_MODE_CONTACT = 0,
//	TEST_MODE_RUN,
//	TEST_MODE_SINGLE,
//	TEST_MODE_DIAG	
//};

enum {
	TEST_MODE_LOAD = 0,
	TEST_MODE_CONTACT,
	TEST_MODE_RUN, 
	TEST_MODE_SINGLE,
	TEST_MODE_DIAG, 
	TEST_MODE_CMD
};


#pragma once
class common
{
public:
	common();
	~common();

	char m_sPgm[64];
	char m_sLot[64];
	char m_sBid[64];

	int		m_nTestMode; // 0 = Load PreRun, 1 = Contact, 2 = Run, 3=Diag
	int		m_nDiagTestSel;
	int		m_nStepIdx;
	char	m_sStepName[1024][256];
	char	m_sCurStep[128];
	uint    m_nPGID[MAX_PG];

	vector<string> m_singleStepVec;

	int loadStarLib();
	int loadConfig();
	int doArgument(int argc, char** argv);	
	void singleStep(const char* step);

	FILE* m_fDebug;
	void debug(const char* str, ...);
};

#endif

