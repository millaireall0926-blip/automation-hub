#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>

#ifdef __linux__
#include <float.h>
#include <memory.h>
#endif

#include "PGComLib.h"
#include "ErrorLog.h"
#include "RegisterCompiler.h"


//#ifndef DBL_EPSILON
//#define DBL_EPSILON    __DBL_EPSILON__ 
//#endif

using namespace REG;
using namespace KORC;
using namespace OPT;

CRegisterCompiler::CRegisterCompiler() : m_pgrtree(NULL), m_RegIndex(-1), m_RegSize(-1)
{
	m_RefTimeDiv_0 = (((1 << 22) - 1) * (1.0 / (100000000))) - DBL_EPSILON;
	m_RefTimeDiv_1 = (((1 << 22) - 1) * (1.0 / (100000))) - DBL_EPSILON;
	m_TimeResBit = 0;
	memset(m_regset.data, 0, REG_SIZE);
	CreateKeyword();
	GenerateGrammar();
}

CRegisterCompiler::~CRegisterCompiler()
{

}

bool CRegisterCompiler::IsKeyword(string strKey)
{
	if (m_KeywordList.find(strKey) != m_KeywordList.end())
		return true;
	else
		return false;
}

string CRegisterCompiler::GetISPName()
{
	return m_strISP;
}

void CRegisterCompiler::SetISP(unsigned int pc)
{
	m_regset.data[ISP] = pc & 0xFF;
	m_regset.data[ISP+1] = (pc>>8) & 0xFF;
	//m_regset.regset.stisp = pc;
}

int CRegisterCompiler::WriteFile(ofstream& ofile)
{
	ofile.write((char*)m_regset.data, REG_SIZE);
	if (ofile.fail())
		return -1;
	else
		return REG_SIZE;
}

void CRegisterCompiler::CreateKeyword()
{
	m_KeywordList["="    ] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	//// REGISTER Command
	m_KeywordList["IDX0" ] = ENCODE(C_REG_CMD, IDX0,  3);
	m_KeywordList["IDX1" ] = ENCODE(C_REG_CMD, IDX1,  3);
	m_KeywordList["IDX2" ] = ENCODE(C_REG_CMD, IDX2,  3);
	m_KeywordList["IDX3" ] = ENCODE(C_REG_CMD, IDX3,  3);
	m_KeywordList["IDX4" ] = ENCODE(C_REG_CMD, IDX4,  3);
	m_KeywordList["IDX5" ] = ENCODE(C_REG_CMD, IDX5,  3);
	m_KeywordList["IDX6" ] = ENCODE(C_REG_CMD, IDX6,  3);
	m_KeywordList["IDX7" ] = ENCODE(C_REG_CMD, IDX7,  3);
	m_KeywordList["IDX8" ] = ENCODE(C_REG_CMD, IDX8,  3);
	m_KeywordList["IDX9" ] = ENCODE(C_REG_CMD, IDX9,  3);
	m_KeywordList["IDX10"] = ENCODE(C_REG_CMD, IDX10, 3);
	m_KeywordList["IDX11"] = ENCODE(C_REG_CMD, IDX11, 3);
	m_KeywordList["IDX12"] = ENCODE(C_REG_CMD, IDX12, 3);
	m_KeywordList["IDX13"] = ENCODE(C_REG_CMD, IDX13, 3);
	m_KeywordList["IDX14"] = ENCODE(C_REG_CMD, IDX14, 3);
	m_KeywordList["IDX15"] = ENCODE(C_REG_CMD, IDX15, 3);

	m_KeywordList["XMAX"] = ENCODE(C_REG_CMD, XMAX, 3);
	m_KeywordList["YMAX"] = ENCODE(C_REG_CMD, YMAX, 3);
	//m_KeywordList["ZMAX"] = ENCODE(C_REG_CMD, ZMAX, 1);
	m_KeywordList["ZMAX"] = ENCODE(C_REG_CMD, ZMAX, 2);
	m_KeywordList["LMAX"] = ENCODE(C_REG_CMD, LMAX, 3);
	m_KeywordList["HMAX"] = ENCODE(C_REG_CMD, HMAX, 3);

	m_KeywordList["XH"] = ENCODE(C_REG_CMD, XH, 3);
	m_KeywordList["XT"] = ENCODE(C_REG_CMD, XT, 3);
	m_KeywordList["YH"] = ENCODE(C_REG_CMD, YH, 3);
	m_KeywordList["YT"] = ENCODE(C_REG_CMD, YT, 3);
	//m_KeywordList["ZH"] = ENCODE(C_REG_CMD, ZH, 1);
	m_KeywordList["ZH"] = ENCODE(C_REG_CMD, ZH, 2);

	m_KeywordList["D1A"] = ENCODE(C_REG_CMD, D1A, 3);
	m_KeywordList["D1B"] = ENCODE(C_REG_CMD, D1B, 3);
	m_KeywordList["D1C"] = ENCODE(C_REG_CMD, D1C, 3);
	m_KeywordList["D1D"] = ENCODE(C_REG_CMD, D1D, 3);
	m_KeywordList["D1E"] = ENCODE(C_REG_CMD, D1E, 3);
	m_KeywordList["D1F"] = ENCODE(C_REG_CMD, D1F, 3);
	m_KeywordList["D1G"] = ENCODE(C_REG_CMD, D1G, 3);
	m_KeywordList["D1H"] = ENCODE(C_REG_CMD, D1H, 3);
	m_KeywordList["D2A"] = ENCODE(C_REG_CMD, D2A, 3);
	m_KeywordList["D2B"] = ENCODE(C_REG_CMD, D2B, 3);
	m_KeywordList["D2C"] = ENCODE(C_REG_CMD, D2C, 3);
	m_KeywordList["D2D"] = ENCODE(C_REG_CMD, D2D, 3);
	m_KeywordList["D3B"] = ENCODE(C_REG_CMD, D3B, 3);
	m_KeywordList["D4B"] = ENCODE(C_REG_CMD, D4B, 3);

	m_KeywordList["TPH1" ] = ENCODE(C_REG_CMD, TPH1 , 3);
	m_KeywordList["TPH1A"] = ENCODE(C_REG_CMD, TPH1A, 3);
	m_KeywordList["TPH1B"] = ENCODE(C_REG_CMD, TPH1B, 3);
	m_KeywordList["TPH1C"] = ENCODE(C_REG_CMD, TPH1C, 3);
	m_KeywordList["TPH1D"] = ENCODE(C_REG_CMD, TPH1D, 3);
	m_KeywordList["TPH2" ] = ENCODE(C_REG_CMD, TPH2 , 3);
	m_KeywordList["TPH2A"] = ENCODE(C_REG_CMD, TPH2A, 3);
	m_KeywordList["TPH2B"] = ENCODE(C_REG_CMD, TPH2B, 3);
	m_KeywordList["TPH2C"] = ENCODE(C_REG_CMD, TPH2C, 3);
	m_KeywordList["TPH2D"] = ENCODE(C_REG_CMD, TPH2D, 3);

	m_KeywordList["D5"   ] = ENCODE(C_REG_CMD, D5,    3);
	m_KeywordList["D6"   ] = ENCODE(C_REG_CMD, D6,    3);
	m_KeywordList["D7"   ] = ENCODE(C_REG_CMD, D7,    3);
	m_KeywordList["D8"   ] = ENCODE(C_REG_CMD, D8,    3);
	m_KeywordList["XOS"  ] = ENCODE(C_REG_CMD, XOS,   3);
	m_KeywordList["YOS"  ] = ENCODE(C_REG_CMD, YOS,   3);
	m_KeywordList["DCMR" ] = ENCODE(C_REG_CMD, DCMR,  3);
	m_KeywordList["DCMR2"] = ENCODE(C_REG_CMD, DCMR2, 3);
	m_KeywordList["DBMAP"] = ENCODE(C_REG_CMD, DBMAP, 2);
	
	m_KeywordList["SCAN"    ] = ENCODE(C_REG_CMD, SCAN, 3);
	m_KeywordList["VMPA"    ] = ENCODE(C_REG_CMD, VMPA, 3);
	m_KeywordList["STATUS"  ] = ENCODE(C_REG_CMD, STATUS, 2);

	m_KeywordList["XMASK"] = ENCODE(C_REG_CMD, XMASK, 3);
	m_KeywordList["YMASK"] = ENCODE(C_REG_CMD, YMASK, 3);
	//m_KeywordList["ZMASK"] = ENCODE(C_REG_CMD, ZMASK, 1);
	m_KeywordList["ZMASK"] = ENCODE(C_REG_CMD, ZMASK, 2);
	m_KeywordList["CCD"  ] = ENCODE(C_REG_CMD, CCD,   3);
	m_KeywordList["RCD"  ] = ENCODE(C_REG_CMD, RCD,   3);
	m_KeywordList["DSD"  ] = ENCODE(C_REG_CMD, DSD,   3);

	m_KeywordList["DCMRH"] = ENCODE(C_REG_CMD, DCMRH, 3);
	m_KeywordList["DCMRL"] = ENCODE(C_REG_CMD, DCMRL, 3);
	m_KeywordList["BL"   ] = ENCODE(C_REG_CMD, BL,    1);
	m_KeywordList["BT"   ] = ENCODE(C_REG_CMD, BT,    1);
	m_KeywordList["WL"   ] = ENCODE(C_REG_CMD, WL,    1);
	m_KeywordList["RL"   ] = ENCODE(C_REG_CMD, RL,    1);
	m_KeywordList["XMIN" ] = ENCODE(C_REG_CMD, XMIN,  3);
	m_KeywordList["YMIN" ] = ENCODE(C_REG_CMD, YMIN,  3);
	//m_KeywordList["ZMIN" ] = ENCODE(C_REG_CMD, ZMIN,  1);
	m_KeywordList["ZMIN" ] = ENCODE(C_REG_CMD, ZMIN,  2);
	m_KeywordList["S1A"  ] = ENCODE(C_REG_CMD, S1A  , 1);
	//m_KeywordList["MLOOP"] = ENCODE(C_REG_CMD, MLOOP, 3);


	//// Timer Command
	m_KeywordList["TIME"   ] = ENCODE(C_TIME_CMD, TIMER,   3);
	m_KeywordList["PWAIT"  ] = ENCODE(C_TIME_CMD, PWAIT,   3);
	m_KeywordList["REFTIME"] = ENCODE(C_TIME_CMD, REFTIME, 3);
	m_KeywordList["CFLG"   ] = ENCODE(C_REG_CMD,  CFLG,    2);

	//// ISP Command
	m_KeywordList["ISP"     ] = ENCODE(C_ISP_CMD, ISP, 2);

	//// MODE Command
	m_KeywordList["MODE"    ] = ENCODE(C_MODE_CMD, REF_MODE, 1);
	m_KeywordList["REFMODE" ] = ENCODE(C_MODE_CMD, REF_MODE, 1);
	//m_KeywordList["REFRESHA"] = ENCODE(C_REFMODE, C_REFMODE, REFRESH::A);
	//m_KeywordList["REFRESHB"] = ENCODE(C_REFMODE, C_REFMODE, REFRESH::B);

	/// UNIT
	m_KeywordList["MS"] = ENCODE(C_UNIT,  O_MS,  0);
	m_KeywordList["US"] = ENCODE(C_UNIT,  O_US,  0);
	m_KeywordList["NS"] = ENCODE(C_UNIT,  O_NS,  0);
}

void CRegisterCompiler::GenerateGrammar()
{
	vector<vector<ITEM>> reg_tree = {
		/*0 "="  */ { ITEM(C_ASSIGN, C_ASSIGN, 1) },
		/*1 "12" */ { ITEM(C_UINT,   C_UINT,   2) },
		/*2 EXIT */ { ITEM(C_EXIT,   C_EXIT,  -1) }
	};

	vector<vector<ITEM>> time_tree = {
		/*0 "="        */ { ITEM(C_ASSIGN, C_ASSIGN, 1) },
		/*1 "12 0.0"   */ { ITEM(C_UINT,   C_UINT,   2), ITEM(C_FLOAT,   C_FLOAT,   2) },
		/*2 "MS,US,NS" */ { ITEM(C_UNIT,   C_UNIT,   3), ITEM(C_EXIT,    C_EXIT,   -1) },
		/*3  EXIT      */ { ITEM(C_EXIT,   C_EXIT,  -1) }
	};

	vector<vector<ITEM>> isp_tree = {
		/*0 "="      */ { ITEM(C_ASSIGN, C_ASSIGN, 1), },
		/*1 "12,REF" */ { ITEM(C_UINT,   C_UINT,   2), ITEM(C_STRING,  C_STRING,  2) },
		/*2  EXIT    */ { ITEM(C_EXIT,   C_EXIT,  -1), }
	};

	m_grammar_collage["Register"] = reg_tree;
	m_grammar_collage["Time"    ] = time_tree;
	m_grammar_collage["isp"     ] = isp_tree;

}

bool CRegisterCompiler::Compile(string strLine)
{
	m_strLine = strLine;
	m_pos = 0;

	bool result = true;
	string strKey;

	int KeyType;
	while ((strKey = GetKey(KeyType)) != "") {
		SyntaxMap::iterator it = m_KeywordList.find(strKey);
		if (it != m_KeywordList.end()) {
			ITEM item(it->second);
			m_Index = 0;
			m_RegIndex = item.opt;
			m_RegSize = item.bit;
			switch (item.cmd) {
				case C_REG_CMD:
					if (RegComp() == -1) result = false;
					break;
				case C_TIME_CMD:
					if (TimeComp() == -1) result = false;
					break;
				case C_ISP_CMD: 
					if (ISPComp() == -1) result = false;
					break;
				case C_MODE_CMD:
					if (ModeComp() == -1) result = false;
					break;
				default:
					result = false;
					break;
			}
		}
		else { // Unknown keyword
			result = false;
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Unknown keword.");
			break;
		}
	}

	return result;
}

string CRegisterCompiler::GetKey(int& nType)
{
	if (m_strLine.size() == 0)
		return "";

	nType = KEY_NULL;

	string strKey;
	size_t size = m_strLine.size();
	while (m_pos < size)
	{
		if (isspace(m_strLine[m_pos])) {
			m_pos++;
			continue;
		}
		else if (m_strLine[m_pos] == '=') {
			strKey.push_back(m_strLine[m_pos++]);
			nType = KEY_SYM;
			return strKey;
		}
		// Advan style 16진수 처리
		else if (m_strLine[m_pos] == '#') {
			//strKey.push_back(m_strLine[m_pos++]);
			m_pos++;
			strKey += "0x";
			while (m_pos < size && isxdigit(m_strLine[m_pos])) {
				strKey.push_back(m_strLine[m_pos++]);
			}
			nType = KEY_XINT;
			return strKey;
		}
		else if (m_strLine[m_pos] == '0') {
			strKey.push_back(m_strLine[m_pos++]);
			// c style 16진수 처리
			if (m_strLine[m_pos] == 'x') {
				strKey.push_back(m_strLine[m_pos++]);
				while (m_pos < size && isxdigit(m_strLine[m_pos])) {
					strKey.push_back(m_strLine[m_pos++]);
				}
				nType = KEY_XINT;
			}
			// 10 진수 or 부동 소수점 처리
			else {
				while (m_pos < size && (isdigit(m_strLine[m_pos]) || m_strLine[m_pos] == '.') ) {
					strKey.push_back(m_strLine[m_pos++]);
				}
				nType = KEY_INT;
			}
			return strKey;
		}
		// 10진수 처리 or 부동 소수점
		else if (isdigit(m_strLine[m_pos])) {
			nType = KEY_INT;
			strKey.push_back(m_strLine[m_pos++]);
			while (m_pos < size && (isdigit(m_strLine[m_pos]) || m_strLine[m_pos] == '.')) {
				strKey.push_back(m_strLine[m_pos++]);
			}
			// FLOAT type 체크
			if(strKey.find('.') != string::npos)
				nType = KEY_FLT;

			return strKey;
		}
		else if (isalpha(m_strLine[m_pos])) {
			while (m_pos < size && (isalpha(m_strLine[m_pos]) || isdigit(m_strLine[m_pos]))) {
				strKey.push_back(m_strLine[m_pos++]);
			}
			nType = KEY_STR;
			return strKey;
		}
		else {
			while (!isspace(m_strLine[m_pos]) && m_pos < size) {
				strKey.push_back(m_strLine[m_pos++]);
			}
			return strKey;
		}
	}

	return strKey;
}

int CRegisterCompiler::RegComp()
{
	string strKey;
	char* stopptr = NULL;
	__int64 operand = -1;
	SyntaxMap::iterator it;
	m_pgrtree = &m_grammar_collage["Register"];

	int KeyType;
	while ((strKey = GetKey(KeyType)) != "") {
		// Operand가 아닌경우
		//if (!(KeyType & (KEY_INT | KEY_XINT | KEY_FLT))) {
		if (!(KeyType & (KEY_INT | KEY_XINT))) {
			it = m_KeywordList.find(strKey);
			if (it == m_KeywordList.end()) {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Unknown Keyword.");
				return -1;
			}
			ITEM item = it->second;
			if (CheckGrammar(item.cmd) == -1) {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
				return -1;
			}
		}
		// Operand Process
		else {
			if (CheckGrammar(C_UINT) != -1) {
				int nBase = 10;
				if (KeyType == KEY_XINT) nBase = 16;
				operand = strtoll(strKey.c_str(), &stopptr, nBase);
				if (m_RegIndex >= IDX0 && m_RegIndex <= IDX15) operand++;
				// Register 설정
				for (int i = 0; i < m_RegSize; i++) {
					m_regset.data[m_RegIndex + i] = (operand >> (i * 8)) & 0xFF;
				}
			}
			else {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
				return -1;
			}
		}
	}

	if (CheckGrammar(C_EXIT) == -1) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Syntax ended abnormally.");
		return -1;
	}

	return 0;
}


int CRegisterCompiler::TimeComp()
{
	string strKey;
	char* stopptr = NULL;
	double operand = 0;
	unsigned int value = 0;
	SyntaxMap::iterator it;
	m_pgrtree = &m_grammar_collage["Time"];

	int KeyType;
	while ((strKey = GetKey(KeyType)) != "") {
		// Operand가 아닌경우
		if (!(KeyType & (KEY_INT | KEY_XINT | KEY_FLT))) {
			it = m_KeywordList.find(strKey);
			if (it == m_KeywordList.end()) {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Unknown Keyword.");
				return -1;
			}
			ITEM item = it->second;
			if (CheckGrammar(item.cmd) == -1) {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
				return -1;
			}
			else {
				if (item.cmd == C_UNIT)	{
					switch (item.opt) {
					case O_NS: operand /= 1000; //fall through
					case O_US: operand /= 1000; //fall through
					case O_MS: operand /= 1000; //fall through
						if (operand > m_RefTimeDiv_1) {
							m_TimeResBit = 2; // milli-sec
							value = static_cast<unsigned int>(operand  / (TIMER_RES * 1000000));
						}
						else if (operand > m_RefTimeDiv_0) {
							m_TimeResBit = 1; // micro-sec
							value = static_cast<unsigned int>(operand  / (TIMER_RES * 1000));
						}
						else {
							m_TimeResBit = 0; // nano-sec
							value = static_cast<unsigned int>(operand  /  TIMER_RES);
						}
						value |= (m_TimeResBit << 22);
						// Register 설정
						for (int i = 0; i < m_RegSize; i++) {
							m_regset.data[m_RegIndex + i] = (value >> (i * 8)) & 0xFF;
						}
						break;
					}
				}
			}
		}
		// Operand Process
		else {
			if (CheckGrammar(C_UINT) != -1) {
				operand = strtold(strKey.c_str(), &stopptr);
				if (operand > m_RefTimeDiv_1) {
					m_TimeResBit = 2; // milli-sec
					value = static_cast<unsigned int>((operand) / (TIMER_RES * 1000000));
				}
				else if (operand > m_RefTimeDiv_0) {
					m_TimeResBit = 1; // micro-sec
					value = static_cast<unsigned int>((operand)/ (TIMER_RES * 1000));
				}
				else {
					m_TimeResBit = 0; // nano-sec
					value = static_cast<unsigned int>((operand) / TIMER_RES);
				}
				value |= (m_TimeResBit << 22);
				// Register 설정
				for (int i = 0; i < m_RegSize; i++) {
					m_regset.data[m_RegIndex + i] = (value >> (i * 8)) & 0xFF;
				}
			}
			else {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
				return -1;
			}
		}
	}

	if (CheckGrammar(C_EXIT) == -1) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Syntax ended abnormally.");
		return -1;
	}

	return 0;
}

int CRegisterCompiler::ISPComp()
{
	string strKey;
	char* stopptr = NULL;
	__int64 operand = -1;
	SyntaxMap::iterator it;
	m_pgrtree = &m_grammar_collage["isp"];

	int KeyType;
	while ((strKey = GetKey(KeyType)) != "") {
		it = m_KeywordList.find(strKey);
		if (it != m_KeywordList.end()) {
			ITEM item = it->second;
			if (CheckGrammar(item.cmd) == -1) {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
				return -1;
			}
			continue;
		}
		else {
			if (KeyType & (KEY_INT | KEY_XINT)) {
				if (CheckGrammar(C_UINT) == -1) {
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
					return -1;
				}
				int nBase;
				nBase = (KeyType & KEY_INT) ? 10 : 16;
				//if (strKey.size() > 2) {
				//	if (strKey[0] == '0' && strKey[1] == 'x') {
				//		nBase = 16;
				//	}
				//}
				operand = strtoll(strKey.c_str(), &stopptr, nBase);
				// Register 설정
				for (int i = 0; i < m_RegSize; i++) {
					m_regset.data[m_RegIndex + i] = (operand >> (i * 8)) & 0xFF;
				}
			}
			else if (KeyType == KEY_STR) {
				if (CheckGrammar(C_STRING) == -1) {
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
					return -1;
				}
				m_strISP = strKey;
				continue;
			}
			else {
				if (CheckGrammar(C_UINT) == -1) {
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
					return -1;
				}
			}

		}

		// Operand가 아닌경우
		if (!(KeyType & (KEY_INT | KEY_XINT | KEY_FLT))) {
			it = m_KeywordList.find(strKey);
			if (it == m_KeywordList.end()) {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Unknown Keyword.");
				return -1;
			}
			ITEM item = it->second;
			if (CheckGrammar(item.cmd) == -1) {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
				return -1;
			}
		}
		// Operand Process
		else {
			if (CheckGrammar(C_UINT) != -1) {
				int nBase = 10;
				if (strKey.size() > 2) {
					if (strKey[0] == '0' && strKey[1] == 'x') {
						nBase = 16;
					}
				}
				operand = strtoll(strKey.c_str(), &stopptr, nBase);
				// Register 설정
				for (int i = 0; i < m_RegSize; i++) {
					m_regset.data[m_RegIndex + i] = (operand >> (i * 8)) & 0xFF;
				}
			}
			else {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
				return -1;
			}
		}
	}

	if (CheckGrammar(C_EXIT) == -1) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Syntax ended abnormally.");
		return -1;
	}

	return 0;
}

int CRegisterCompiler::ModeComp()
{
	string strKey;
	SyntaxMap::iterator it;

	int KeyType;
	if((strKey = GetKey(KeyType)) == "") {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Syntax ended abnormally.");
		return -1;
	}
	else {
		if(strKey == "REFRESHA")
			m_regset.data[m_RegIndex] = 0;
		else if(strKey == "REFRESHB")
			m_regset.data[m_RegIndex] = 1;
		else {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
			return -1;
		}
	}

	return 0;
}

// return value != -1 : Pass(opt value)
// return value -1    : Fail
int CRegisterCompiler::CheckGrammar(unsigned int cmd)
{
	grammar_tree& grtree = *m_pgrtree;
	int opt = 0;

	if (m_Index != -1 && grtree.size() > static_cast<unsigned int>(m_Index)) {
		// CMD Find
		for (unsigned int i = 0; i < grtree[m_Index].size(); i++) {
			if (grtree[m_Index][i].cmd == cmd) {
				opt = grtree[m_Index][i].opt;
				m_Index = grtree[m_Index][i].bit;
				return opt;
			}
		}
	}
	return -1;
}

void CRegisterCompiler::Dump(ofstream& ofile)
{
	ofile << "Register Setting Data."  << endl;
	char buf[100];

	for (int i = 0; i < REG_SIZE; i+=16) {
		sprintf(buf, "%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X\n"
			, (m_regset.data[i   ] & 0xFF), (m_regset.data[i+ 1] & 0xFF), (m_regset.data[i+ 2] & 0xFF), (m_regset.data[i+ 3] & 0xFF)
			, (m_regset.data[i+4 ] & 0xFF), (m_regset.data[i+ 5] & 0xFF), (m_regset.data[i+ 6] & 0xFF), (m_regset.data[i+ 7] & 0xFF)
			, (m_regset.data[i+8 ] & 0xFF), (m_regset.data[i+ 9] & 0xFF), (m_regset.data[i+10] & 0xFF), (m_regset.data[i+11] & 0xFF)
			, (m_regset.data[i+12] & 0xFF), (m_regset.data[i+13] & 0xFF), (m_regset.data[i+14] & 0xFF), (m_regset.data[i+15] & 0xFF));
		ofile << buf;
	}
	
}