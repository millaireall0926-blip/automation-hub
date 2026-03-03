#pragma once

#include "ErrorSD.h"

class CErrorLog
{
private:
	CErrorLog();
	virtual ~CErrorLog();
public:
	
	static CErrorLog * GetInstance() {
		if (m_pInstance) {
			return m_pInstance;
		}
		else {
			m_pInstance = new CErrorLog;
			return m_pInstance;
		}
	}
	void SetFileScanner(void* pFScan);
	void AddErrorLog(LOG_TYPE type, int code, std::string msg);
	void AddErrorLog(LOG_TYPE type, int code, std::string strKey, std::string msg);

private:
	void* m_pFScan; // 현재 사용되는 CFileSCanner 포인터
	static CErrorLog* m_pInstance;
	std::vector< std::pair< int, std::string > > m_vecErrorLog;	
};

