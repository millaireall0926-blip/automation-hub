#ifndef __ERRC__
#define __ERRC__

#include "String.h"

enum ERR_TYPE {
	ERR_PG = 0, ERR_TS = 1, ERR_SCR, ERR_BIB, ERR_PDS, ERR_BIN,
	ERR_SCAN, ERR_MASK, ERR_POWER, ERR_VEC, ERR_TEMP,
	ERR_RUNTIME, ERR_IO, ERR_ETC
};

class errc  // error ctrl
{
public:
	errc(void);
	~errc(void);

	vector<cstring> m_sErr;
	vector<cstring> m_sErrVec;

	void	addErr(int type, const char* sfunc, const char* str, ...);
	int		getErrCnt();
	const char* getErrStr(int idx);
	void	clearErr();
	int		isReady();

	int		m_nPrintMode;
	void	setPrintMode(int mode) { m_nPrintMode = mode; } // 1 = local, 2 = network
	void	print(const char* str, ...);

};

#endif


