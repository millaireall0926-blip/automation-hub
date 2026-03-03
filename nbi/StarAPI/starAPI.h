#ifndef __STARAPI__
#define __STARAPI__

#include <stdio.h>
#include <map>
#include <vector>
#include "String.h"
enum {
	TEST_MODE_LOAD = 0,
	TEST_MODE_CONTACT,
	TEST_MODE_RUN,
	TEST_MODE_INIT,
	TEST_MODE_HANDLER,
	TEST_MODE_SINGLE,
	TEST_MODE_DIAG, 
	TEST_MODE_CMD
};



class starAPI
{
public:
	starAPI();
	~starAPI() {}

	char m_sPgm[64];
	char m_sLot[64];
	char m_sBid[64];
//	char m_sSingleStep[128];

	vector<string> m_singleStepVec;
	bool m_maskDuts[2048];

	
	bool	m_bUseNet;
	bool	m_bNoDPS;
	bool	m_bMonDPS;
	bool    m_bUseHandler;
	int		m_nTestMode; // 0 = Load PreRun, 1 = Contact, 2 = Run, 3=Diag
	int		m_nDiagTestSel; 
	int		m_nStepIdx;
	char	m_sStepName[1024][256];
	char	m_sCurStep[128];
	uint    m_nPGID;

	int doArgument(int argc, char** argv);
	int loadStarLib();
	int loadConfig();
	void singleStep(const char* step);

	FILE*   m_fDebug;
	FILE*	m_fBTemp;
	void debug(const char* str, ...);
	void logBTemp(const char* str, ...);

	void initHW();
	
	

};



#endif