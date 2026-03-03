#ifndef __RESULT__
#define __RESULT__

#include "../starlib/starlib_dif.h"

class result
{
public:
	result();
	~result();

	void init();

	void initResult();
	void freeResult();
	void loadResult();

	void initSubResult(bool bUseFree = true);
	void freeSubResult();

	void initCatFile();

	void makeCycleResult();

	void resultAddStepList();

	HND	 m_hResult;
	HND  m_hSubResult;

	char m_sResCurStep[128];
	
	char m_sDate[64];

	void binResult();

	void loadCatFile();

	int startStepRes();
	int endStepRes();

	void saveResOption();


};
#endif

