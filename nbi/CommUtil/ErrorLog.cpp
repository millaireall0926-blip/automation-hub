#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "FileScanner.h"
#include "ErrorLog.h"

CErrorLog* CErrorLog::m_pInstance = 0;

CErrorLog::CErrorLog()
{

}

CErrorLog::~CErrorLog()
{
	if (m_pInstance)
		delete m_pInstance;
}

void CErrorLog::SetFileScanner(void* pFScan)
{
	m_pFScan = pFScan;
}

void CErrorLog::AddErrorLog(LOG_TYPE type, int code, std::string msg)
{
	std::string strLog;

	if (m_pFScan) {
		strLog += ((CFileScanner*)m_pFScan)->GetFileName();
		strLog += " : ";
		strLog += std::to_string(((CFileScanner*)m_pFScan)->GerLineNumber());
		strLog += " : ";
	}

	//std::clog << m_strFile << " : ";
	switch (type)
	{
		case MESSAGE: strLog += "Message : "; break;
		case NOTICE:  strLog += "Notice : "; break;
		case WARNING: strLog += "Warning : "; break;
		case ERROR:   strLog += "Error Code :  "; break;
	}
	strLog += std::to_string(code);
	strLog += " : ";
	strLog += msg;

	// Message 저장
	m_vecErrorLog.push_back(std::make_pair(code, msg));
	// Message 화면 출력
	std::clog << strLog << std::endl;
}

void CErrorLog::AddErrorLog(LOG_TYPE type, int code, std::string strKey, std::string msg)
{
	std::string strLog;

	if(m_pFScan) {
		strLog += ((CFileScanner*)m_pFScan)->GetFileName();
		strLog += " : ";
		strLog += std::to_string(((CFileScanner*)m_pFScan)->GerLineNumber());
		strLog += " : ";
	}

	//std::clog << m_strFile << " : ";
	switch (type)
	{
	case MESSAGE: strLog += "Message : "; break;
	case NOTICE:  strLog += "Notice : "; break;
	case WARNING: strLog += "Warning : "; break;
	case ERROR:   strLog += "Error :  "; break;
	}
	strLog += std::to_string(code);
	strLog += " : ";
	strLog += strKey;
	strLog += " : ";
	strLog += msg;

	// Message 저장
	m_vecErrorLog.push_back(std::make_pair(code, msg));
	// Message 화면 출력
	std::clog << strLog << std::endl;
}

