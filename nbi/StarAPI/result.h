#ifndef __RESULT__
#define __RESULT__

#include "../starlib/starlib_dif.h"

class result
{
public:
	result() { init(); }
	~result() {}

	void init();

	void initResult();
	void loadResult();
	void freeResult();

	void initSubResult(bool bUseFree = true);
	void freeSubResult();

	void makeCycleResult();
	

	void resultAddStepList();

	HND	 m_hResult;
	HND  m_hSubResult;


	char m_sResCurStep[128];

	char m_sDate[64];	

	int startStepRes();
	int endStepRes();

	void binResult();
	void initCatFile();
	void loadCatFile();


	void saveResOption();

};
#endif

