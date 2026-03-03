
#include "errc.h"
#include "define.h"

errc::errc(void)
{
	m_sErr.push_back("PG ");
	m_sErr.push_back("TS ");
	m_sErr.push_back("SCR ");
	m_sErr.push_back("BIB ");
	m_sErr.push_back("PDS ");
	m_sErr.push_back("BIN ");
	m_sErr.push_back("SCAN ");
	m_sErr.push_back("MASK ");
	m_sErr.push_back("POWER ");
	m_sErr.push_back("VEC ");
	m_sErr.push_back("TEMP ");
	m_sErr.push_back("RUN-TIME ");
	m_sErr.push_back("IO ");
	m_sErr.push_back("ETC ");

	m_nPrintMode = 1;
}

errc::~errc(void)
{
	m_sErr.clear();
	m_sErrVec.clear();	
}

void errc::addErr(int type, const char* sfunc, const char* str, ...)
{
	va_list	ap;
	char buf[2048];


	va_start(ap, str);
	vsprintf(buf, str, ap);
	va_end(ap);
//	printf("%s\n", __FUNCTION__);

//	printf("add err msg : %s\n", buf);

	cstring serr;
	serr.Format("%s (%s) : %s", m_sErr[type].c_str(), sfunc, buf);
//	DBG_CRIT("%s\n", serr.c_str());
	printf("% s\n", serr.c_str());
	m_sErrVec.push_back(serr);
}

int errc::getErrCnt()
{
	return m_sErrVec.size();
}

const char* errc::getErrStr(int idx)
{
	if (idx >= (int)m_sErrVec.size())
		return NULL;

	return m_sErrVec.at(idx);
}

void errc::clearErr()
{
	m_sErrVec.clear();
}

int errc::isReady()
{
	if (m_sErrVec.size() == 0)
		return 1;
	else
		return 0;
}

void errc::print(const char* str, ...)
{
	if (m_nPrintMode == 0) return;

	va_list	ap;
	char buf[2048];

	va_start(ap, str);
	vsprintf(buf, str, ap);
	va_end(ap);

	printf("%s", buf);
}