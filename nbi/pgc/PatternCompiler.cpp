
#include <string>
#include <fstream>
#include <iomanip>

#ifdef __linux__
#include <float.h>
#include <memory.h>
#endif

#include "PGComLib.h"
#include "ErrorLog.h"
#include "PatternCompiler.h"

using namespace PAT_OP;
using namespace KOPC;
using namespace OPT;

CPatternCompiler::CPatternCompiler()
{
	/////////////////////////// 0x00800000 == (1<<22)  /////////////////////////
	m_RefTimeDiv_0 = ((double)((0x00400000) - 1) * (1.0 / (100000000))) - DBL_EPSILON; 
	m_RefTimeDiv_1 = ((double)((0x00400000) - 1) * (1.0 / (100000))) - DBL_EPSILON;
	m_RefTimeDiv_2 = ((double)((0x00400000) - 1) * (1.0 / (100))) - DBL_EPSILON;
	m_pc = 0;

	m_nMode = 0;

	m_PrevOpcode = -1;

	CreateKeyword();
	GenerateGrammar();
}

CPatternCompiler::~CPatternCompiler()
{

}

bool CPatternCompiler::Compile(std::string strLine)
{
	m_strLine = strLine;
	m_pos = 0;
	memset(&m_Used, 0, sizeof(m_Used));

	bool result = true;
	string strKey;

	memset(m_inst.data, 0, sizeof(instruction_t));

	while ((strKey = GetFirstKey()) != "") {
		SyntaxMap::iterator it = m_FirstKeyword.find(strKey);
		if (it != m_FirstKeyword.end()) {
			ITEM item(it->second);
			m_Index = 0;
			switch (item.cmd) {
				case C_LONE_OP:
					//m_inst.op_command.opcode = item.bit;
					//break;
				case C_JUMP_OP:
				case C_SET_OP:
				case C_SET_OP_STI:
				case C_SET_OP_TIMER:
				case C_OPRN_OP:
				case C_MODE_OP:
					m_inst.op_command.opcode = item.bit;
					if (OpcodeComp(item.cmd) == -1) {
						result = false;
					}
					break;
				case C_OP_CTRL:
					m_inst.op_command.op_ctrl |= item.bit;
					break;
				case C_XB:
					if( XBComp() == -1) {
						result = false;
					}
					break;
				case C_XN:
					m_inst.xn.sel = item.bit;
					if (XNComp() == -1) {
						result = false;
					}
					break;
				case C_YB:
					if (YBComp() == -1) {
						result = false;
					}
					break;
				case C_YN:
					m_inst.yn.sel = item.bit;
					if (YNComp() == -1) {
						result = false;
					}
					break;
				case C_D3:
					if (D3Comp() == -1) {
						result = false;
					}
					break;
				case C_D4:
					if (D4Comp() == -1) {
						result = false;
					}
					break;
				case C_Z:
					if (ZComp() == -1) {
						result = false;
					}
					break;
				case C_X:
					if (XComp() == -1) {
						result = false;
					}
					break;
				case C_Y:
					if (YComp() == -1) {
						result = false;
					}
					break;
				case C_TP1:
					if (TP1Comp() == -1) {
						result = false;
					}
					break;
				case C_TP2:
					if (TP2Comp() == -1) {
						result = false;
					}
					break;
				case C_D7:
					if (D7Comp() == -1) {
						result = false;
					}
					break;
				case C_D8:
					if (D8Comp() == -1) {
						result = false;
					}
					break;
				case C_SA:
					if (SAComp() == -1) {
						result = false;
					}
					break;
				case C_SB:
					if (SBComp() == -1) {
						result = false;
					}
					break;
				case C_S:
					if (SComp() == -1) {
						result = false;
					}
					break;
				case C_RF:
					if (RFComp() == -1) {
						result = false;
					}
					break;
				case C_FP:
					m_inst.s_rf_fp.fp = item.bit;
					break;
				case C_CTRL:
					m_inst.ctrl.data |= item.bit;
					break;
				case C_CMD:
					if(item.opt == O_CMD0)
						m_inst.pgcmd.cmd0 |= item.bit;
					else if (item.opt == O_CMD1)
						m_inst.pgcmd.cmd1 |= item.bit;
					else if (item.opt == O_CMD2)
						m_inst.pgcmd.cmd2 |= item.bit;
					break;
				case C_CYP:
					m_inst.cyp_ts.cyp = item.bit;
					break;
				case C_TS:
					m_inst.cyp_ts.ts = item.bit;
					break;
				case C_IMM:
					m_ImmIdx = item.opt;
					m_ImmAlu = item.bit;
					if (ImmComp() == -1) {
						result = false;
					}
					break;
				case C_TPX:
					m_ImmIdx = item.opt;
					m_ImmAlu = item.bit;
					if (TPXComp() == -1) {
						result = false;
					}
					break;
				case C_NAND_CTRL:
					m_inst.nand_ctrl.data |= item.bit;
					break;
				case C_D1:
					m_DxIdx = item.bit;
					if (D1Comp() == -1) {
						result = false;
					}
					break;
				case C_D2:
					m_DxIdx = item.bit;
					if (D2Comp() == -1) {
						result = false;
					}
					break;
				case C_TPH1:
					m_DxIdx = item.bit;
					if (TPH1Comp() == -1) {
						result = false;
					}
					break;
				case C_TPH2:
					m_DxIdx = item.bit;
					if (TPH2Comp() == -1) {
						result = false;
					}
					break;
			}
		}
		else {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Unkmown keyword.");
			result = false;
		}

		if (result == false)
			return result;
	}

	m_InstList.push_back(m_inst);

	m_pc++;

	return true;
}

void CPatternCompiler::AddSTPS()
{
	memset(m_inst.data, 0, sizeof(instruction_t));
	m_inst.op_command.opcode = PAT_OP::STPS;
	m_pc++;
	m_InstList.push_back(m_inst);
}

int CPatternCompiler::PostComp(vector<patinfo>& PatInfo, vector<labelinfo> LabelInfo)
{
	// Jump op 또는 Call 처리
	size_t size = m_LabelJumpPC.size();
	size_t pc = 0;
	string strName = "";

	bool bFind = false;

	for (unsigned int i = 0; i < size; i++) {
		pc = m_LabelJumpPC[i].first;
		strName = m_LabelJumpPC[i].second;
		bFind = false;
		for (size_t idx = 0; idx < LabelInfo.size(); idx++) {
			if (LabelInfo[idx].name == strName) {
				m_InstList[pc].op_command.operand = LabelInfo[idx].pc;
				bFind = true;
				break;
			}
		}

		if (bFind) continue;

		for (size_t idx = 0; idx < PatInfo.size(); idx++) {
			if (PatInfo[idx].name == strName) {
				m_InstList[pc].op_command.operand = PatInfo[idx].start;
				bFind = true;
				break;
			}
		}

		if (bFind == false) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strName, "Function or Label is not exist.");
			return -1;
		}
	}

	return 0;
}

bool CPatternCompiler::IsKeyword(std::string & strKey)
{
	map<string, SyntaxMap>::iterator it;
	for (it = m_KeywordMapList.begin(); it != m_KeywordMapList.end(); it++) {
		if (it->second.find(strKey) != it->second.end())
			return true;
	}
	
	return false;
}

bool CPatternCompiler::IsOpcode(string& strKey)
{
	vector<string>::iterator it;
	for (it = m_OpcodeKeyword.begin(); it != m_OpcodeKeyword.end(); it++) {
		if (*it == strKey)
			return true;
	}

	return false;
}

int CPatternCompiler::WriteFile(ofstream& ofile)
{
	int size = 0;
	for (unsigned int i = 0; i < m_InstList.size(); i++) {
		ofile.write((char*)m_InstList[i].data, INST_SIZE);
		if (ofile.fail())
			return -1;

		size += INST_SIZE;
	}

	return size;
}
string CPatternCompiler::GetFirstKey()
{
	if (m_strLine.size() == 0)
		return NULL;

	string strKey;

	if (m_BufKey.size()) {
		strKey = m_BufKey.front();
		m_BufKey.erase(m_BufKey.begin());
		return strKey;
	}

	size_t size = m_strLine.size();
	while (m_pos < size)
	{
		if (isspace(m_strLine[m_pos])) {
			m_pos++;
			continue;
		}
		else if (m_strLine[m_pos] == '/') {
			strKey.push_back(m_strLine[m_pos]);
			m_pos++;
			// 공백 제거
			while (m_pos < size && isspace(m_strLine[m_pos]) ) {
				m_pos++;
			}
			// key word 추출
			while (m_pos < size && !isspace(m_strLine[m_pos]) ) {
				strKey.push_back(m_strLine[m_pos++]);
			}
			return strKey;
		}
		else if (isalpha(m_strLine[m_pos])) {
			strKey.push_back(m_strLine[m_pos++]);
			while (m_pos < size && (isalpha(m_strLine[m_pos]) || isdigit(m_strLine[m_pos]) || m_strLine[m_pos] == '_')) {
				strKey.push_back(m_strLine[m_pos++]);
			}
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

string CPatternCompiler::GetKey(int& nType)
{
	if (m_strLine.size() == 0)
		return "";

	nType = KEY_NULL;
	char* endptr;

	m_strOrgKey.clear();

	string strKey;
	size_t size = m_strLine.size();
	while (m_pos < size)
	{
		if (isspace(m_strLine[m_pos])) {
			m_pos++;
			continue;
		}
		else if (m_strLine[m_pos] == '=' || m_strLine[m_pos] == '<' || m_strLine[m_pos] == '&' || m_strLine[m_pos] == '|' || m_strLine[m_pos] == '^') {
			strKey.push_back(m_strLine[m_pos++]);
			nType = KEY_SYM;
			return strKey;
		}
		// "+" or "+1" or "-" or "-1"
		else if (m_strLine[m_pos] == '+' || m_strLine[m_pos] == '-') {
			nType = KEY_STR;
			m_strOrgKey.push_back(m_strLine[m_pos]);
			strKey.push_back(m_strLine[m_pos++]);
			while (isspace(m_strLine[m_pos])) {
				m_strOrgKey.push_back(m_strLine[m_pos]);
				m_pos++;
			}
			int nPos = (int)m_pos;
			string strTemp;
			while (m_pos < size && (isdigit(m_strLine[m_pos]) || isxdigit(m_strLine[m_pos]) || isalpha(m_strLine[m_pos]) || m_strLine[m_pos] == '#')) {
				strTemp.push_back(m_strLine[m_pos]);
				m_pos++;
			}
			if (strTemp.size() == 1 && strTemp[0] == '1') {
				strKey += strTemp;
				m_strOrgKey += strTemp;
			}
			else if (strTemp.size() > 2 && strTemp[0] == '0' && strTemp[1] == 'x')
			{
				int data = strtod(strTemp.c_str(), &endptr);
				if (data == 1) {
					strKey += '1';
					m_strOrgKey += strTemp;
				}
				else
					m_pos = nPos;
			}
			else if (strTemp.size() > 1 && strTemp[0] == '#')
			{
				int data = strtod(strTemp.substr(1).c_str(), &endptr);
				if (data == 1) {
					strKey += '1';
					m_strOrgKey += strTemp;
				}
				else
					m_pos = nPos;
			}
			else m_pos = nPos;
			return strKey;
		}
		// "/" or "/2" or "*" or "*2"
		else if (m_strLine[m_pos] == '/' || m_strLine[m_pos] == '*') {
			nType = KEY_STR;
			m_strOrgKey.push_back(m_strLine[m_pos]);
			strKey.push_back(m_strLine[m_pos++]);
			while (isspace(m_strLine[m_pos])) {
				m_strOrgKey.push_back(m_strLine[m_pos]);
				m_pos++;
			}
			int nPos = (int)m_pos;
			string strTemp;
			while (m_pos < size && (isdigit(m_strLine[m_pos]) || isxdigit(m_strLine[m_pos]) || isalpha(m_strLine[m_pos]) || m_strLine[m_pos] == '#')) {
				strTemp.push_back(m_strLine[m_pos]);
				m_pos++;
			}
			if (strTemp.size() == 1 && strTemp[0] == '2') {
				strKey += strTemp;
				m_strOrgKey += strTemp;
			}
			else if (strTemp.size() > 2 && strTemp[0] == '0' && strTemp[1] == 'x')
			{
				int data = strtod(strTemp.c_str(), &endptr);
				if (data == 2) {
					strKey += '2';
					m_strOrgKey += strTemp;
				}
				else
					m_pos = nPos;
			}
			else if (strTemp.size() > 1 && strTemp[0] == '#')
			{
				int data = strtod(strTemp.substr(1).c_str(), &endptr);
				if (data == 2) {
					strKey += '2';
					m_strOrgKey += strTemp;
				}
				else
					m_pos = nPos;
			}
			else m_pos = nPos;
			return strKey;
		}
		//// "/" or "/2"
		//else if (m_strLine[m_pos] == '/') {
		//	nType = KEY_STR;
		//	strKey.push_back(m_strLine[m_pos++]);
		//	while (isspace(m_strLine[m_pos])) {
		//		m_pos++;
		//	}
		//	int nPos = m_pos;
		//	string strTemp;
		//	while (m_pos < size && (isdigit(m_strLine[m_pos]) || isalpha(m_strLine[m_pos]))) {
		//		strTemp.push_back(m_strLine[m_pos]);
		//		m_pos++;
		//	}
		//	if (strTemp.size() == 1 && strTemp[0] == '2') {
		//		strKey += strTemp;
		//	}else m_pos = nPos;
		//	return strKey;
		//}
		//// "+" or "+1"
		//else if (m_strLine[m_pos] == '+') {
		//	nType = KEY_ADD;
		//	strKey.push_back(m_strLine[m_pos++]);
		//	while (isspace(m_strLine[m_pos])) {
		//		m_pos++;
		//	}
		//	int nPos = m_pos;
		//	string strTemp;
		//	while (m_pos < size && (isdigit(m_strLine[m_pos]) || isalpha(m_strLine[m_pos]))) {
		//		strTemp.push_back(m_strLine[m_pos]);
		//		m_pos++;
		//	}
		//	if (strTemp.size() == 1 && strTemp[0] == '1') {
		//		nType = KEY_INC;
		//		strKey += strTemp;
		//	}
		//	else m_pos = nPos;
		//	return strKey;
		//}
		//// "-" or "-1" 
		//else if (m_strLine[m_pos] == '-') {
		//	nType = KEY_SUB;
		//	strKey.push_back(m_strLine[m_pos++]);
		//	while (isspace(m_strLine[m_pos])) {
		//		m_pos++;
		//	}
		//	int nPos = m_pos;
		//	string strTemp;
		//	while (m_pos < size && (isdigit(m_strLine[m_pos]) || isalpha(m_strLine[m_pos]))) {
		//		strTemp.push_back(m_strLine[m_pos]);
		//		m_pos++;
		//	}
		//	if (strTemp.size() == 1 && strTemp[0] == '1') {
		//		nType = KEY_DEC;
		//		strKey += strTemp;
		//	}
		//	else m_pos = nPos;
		//	return strKey;
		//}
		// Advan style 16진수 처리
		else if (m_strLine[m_pos] == '#') {
			//strKey.push_back(m_strLine[m_pos++]);
			m_strOrgKey.push_back(m_strLine[m_pos]);
			m_pos++;
			strKey += "0x";
			while (m_pos < size && isxdigit(m_strLine[m_pos])) {
				m_strOrgKey.push_back(m_strLine[m_pos]);
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
				while (m_pos < size && (isdigit(m_strLine[m_pos]) || m_strLine[m_pos] == '.')) {
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
			if (strKey.find('.') != string::npos) {
				nType = KEY_FLT;
			}

			return strKey;
		}
		else if (isalpha(m_strLine[m_pos])) {
			while (m_pos < size && (isalpha(m_strLine[m_pos]) || isdigit(m_strLine[m_pos]) || m_strLine[m_pos] == '_')) {
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

string CPatternCompiler::GetName(int& pType)
{
	if (m_strLine.size() == 0)
		return NULL;

	pType = KEY_NULL;

	string strKey;
	size_t size = m_strLine.size();
	while (m_pos < size)
	{
		if (isspace(m_strLine[m_pos])) {
			m_pos++;
			continue;
		}
		else if (m_strLine[m_pos] == '#') { /// #1234
			pType = KEY_XINT;
			m_pos++;
			while ((m_pos < size) && !isxdigit(m_strLine[m_pos])) {
				strKey.push_back(m_strLine[m_pos++]);
			}
			return strKey;
		}
		else if (isdigit(m_strLine[m_pos])) { /// 1232
			pType = KEY_INT;
			while ((m_pos < size) && !isdigit(m_strLine[m_pos])) {
				strKey.push_back(m_strLine[m_pos++]);
			}
			return strKey;
		}
		else if ((m_pos < size) && (m_strLine[m_pos] == '_' || isalpha(m_strLine[m_pos]))) { /// ABC, _ABC
			pType = KEY_STR;
			strKey.push_back(m_strLine[m_pos++]);
			while ((m_pos < size) && (m_strLine[m_pos] == '_' || isdigit(m_strLine[m_pos]) || isalpha(m_strLine[m_pos]))) {
				strKey.push_back(m_strLine[m_pos++]);
			}
			return strKey;
		}
		else {
			break;
		}
	}
	return strKey;
}

int CPatternCompiler::ExitCheck(string strKey)
{
	if (CheckGrammar(C_EXIT) == -1) {
		if(strKey.size())
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalid syntax.");
		else
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Syntax ended abnormally");
		return -1;
	}
	else {
		if(strKey.size())
			m_BufKey.push_back(strKey);
	}
	return 0;
}

void CPatternCompiler::CreateXBKeyword()
{
	m_KeywordMapList["XB"]["="  ] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["XB"]["<"  ] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["XB"]["XB"] = ENCODE(C_SRC_SEL,  C_XB_ALU, AB_ALU::XB   );
	m_KeywordMapList["XB"]["0" ] = ENCODE(C_RESET,    C_XB_ALU, AB_ALU::RESET   );
	m_KeywordMapList["XB"]["/" ] = ENCODE(C_INV,      C_XB_ALU, AB_ALU::INVERT  );
	m_KeywordMapList["XB"]["+1"] = ENCODE(C_INCREASE, C_XB_ALU, AB_ALU::INCREASE);
	m_KeywordMapList["XB"]["-1"] = ENCODE(C_DECREASE, C_XB_ALU, AB_ALU::DECREASE);
	m_KeywordMapList["XB"]["*2"] = ENCODE(C_LSHIFT,   C_XB_ALU, AB_ALU::LSHIFT  );
	m_KeywordMapList["XB"]["/2"] = ENCODE(C_RSHIFT,   C_XB_ALU, AB_ALU::RSHIFT  );
	m_KeywordMapList["XB"]["+" ] = ENCODE(C_ADD,      C_XB_ALU, AB_ALU::ADD     );
	m_KeywordMapList["XB"]["-" ] = ENCODE(C_SUB,      C_XB_ALU, AB_ALU::SUB     );

	m_KeywordMapList["XB"]["XH" ] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::XH );
	m_KeywordMapList["XB"]["D1A"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D1A );
	m_KeywordMapList["XB"]["D1B"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D1B );
	m_KeywordMapList["XB"]["D1C"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D1C );
	m_KeywordMapList["XB"]["D1D"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D1D );
	m_KeywordMapList["XB"]["D1E"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D1E );
	m_KeywordMapList["XB"]["D1F"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D1F );
	m_KeywordMapList["XB"]["D1G"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D1G );
	m_KeywordMapList["XB"]["D1H"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D1H );
	m_KeywordMapList["XB"]["D2A"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D2A );
	m_KeywordMapList["XB"]["D2B"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D2B );
	m_KeywordMapList["XB"]["D2C"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D2C );
	m_KeywordMapList["XB"]["D2D"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D2D );
	m_KeywordMapList["XB"]["XOS"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::XOS );
	m_KeywordMapList["XB"]["XT" ] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::XT  );
	m_KeywordMapList["XB"]["D3B"] = ENCODE(C_XB_A, AB_ALU::ASSIGN_A, AB_A::D3B );
}

void CPatternCompiler::CreateXNKeyword()
{
	m_KeywordMapList["XN"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["XN"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["XN"]["XC"] = ENCODE(C_SRC_SEL, AN_ALU::XN, AN_A::XC);
	m_KeywordMapList["XN"]["XS"] = ENCODE(C_SRC_SEL, AN_ALU::XN, AN_A::XS);
	m_KeywordMapList["XN"]["XK"] = ENCODE(C_SRC_SEL, AN_ALU::XN, AN_A::XK);
	m_KeywordMapList["XN"]["XA"] = ENCODE(C_SRC_SEL, AN_ALU::XN, AN_A::XA);
	m_KeywordMapList["XN"]["XD"] = ENCODE(C_SRC_SEL, AN_ALU::XN, AN_A::XD);
	m_KeywordMapList["XN"]["XE"] = ENCODE(C_SRC_SEL, AN_ALU::XN, AN_A::XE);
	m_KeywordMapList["XN"]["XF"] = ENCODE(C_SRC_SEL, AN_ALU::XN, AN_A::XF);
	m_KeywordMapList["XN"]["XB"] = ENCODE(C_SRC_SEL, AN_ALU::XN, AN_A::XB);

	m_KeywordMapList["XN"]["0" ] = ENCODE(C_RESET,    C_XN_ALU, AN_ALU::RESET   );
	m_KeywordMapList["XN"]["/" ] = ENCODE(C_INV,      C_XN_ALU, AN_ALU::INVERT  );
	m_KeywordMapList["XN"]["+1"] = ENCODE(C_INCREASE, C_XN_ALU, AN_ALU::INCREASE);
	m_KeywordMapList["XN"]["-1"] = ENCODE(C_DECREASE, C_XN_ALU, AN_ALU::DECREASE);
	m_KeywordMapList["XN"]["*2"] = ENCODE(C_LSHIFT,   C_XN_ALU, AN_ALU::LSHIFT  );
	m_KeywordMapList["XN"]["/2"] = ENCODE(C_RSHIFT,   C_XN_ALU, AN_ALU::RSHIFT  );
	m_KeywordMapList["XN"]["+" ] = ENCODE(C_ADD,      C_XN_ALU, AN_ALU::ADD     );
	m_KeywordMapList["XN"]["-" ] = ENCODE(C_SUB,      C_XN_ALU, AN_ALU::SUB     );
	m_KeywordMapList["XN"]["&" ] = ENCODE(C_AND,      C_XN_ALU, AN_ALU::AND     );
	m_KeywordMapList["XN"]["|" ] = ENCODE(C_OR,       C_XN_ALU, AN_ALU::OR      );
	m_KeywordMapList["XN"]["^" ] = ENCODE(C_XOR,      C_XN_ALU, AN_ALU::XOR     );

	m_KeywordMapList["XN"]["D1A" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D1A );
	m_KeywordMapList["XN"]["D1B" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D1B );
	m_KeywordMapList["XN"]["D1C" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D1C );
	m_KeywordMapList["XN"]["D1D" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D1D );
	m_KeywordMapList["XN"]["D1E" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D1E );
	m_KeywordMapList["XN"]["D1F" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D1F );
	m_KeywordMapList["XN"]["D1G" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D1G );
	m_KeywordMapList["XN"]["D1H" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D1H );
	m_KeywordMapList["XN"]["D2A" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D2A );
	m_KeywordMapList["XN"]["D2B" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D2B );
	m_KeywordMapList["XN"]["D2C" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D2C );
	m_KeywordMapList["XN"]["D2D" ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D2D );
	m_KeywordMapList["XN"]["D3"  ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D3  );
	m_KeywordMapList["XN"]["D4"  ] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::D4  );
	m_KeywordMapList["XN"]["XMAX"] = ENCODE(C_XN_B, AN_ALU::ASSIGN_B, AN_B::XMAX);
}

void CPatternCompiler::CreateYBKeyword()
{
	m_KeywordMapList["YB"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["YB"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["YB"]["YB"] = ENCODE(C_SRC_SEL,  C_YB_ALU, AB_ALU::YB      );
	m_KeywordMapList["YB"]["0" ] = ENCODE(C_RESET,    C_YB_ALU, AB_ALU::RESET   );
	m_KeywordMapList["YB"]["/" ] = ENCODE(C_INV,      C_YB_ALU, AB_ALU::INVERT  );
	m_KeywordMapList["YB"]["+1"] = ENCODE(C_INCREASE, C_YB_ALU, AB_ALU::INCREASE);
	m_KeywordMapList["YB"]["-1"] = ENCODE(C_DECREASE, C_YB_ALU, AB_ALU::DECREASE);
	m_KeywordMapList["YB"]["*2"] = ENCODE(C_LSHIFT,   C_YB_ALU, AB_ALU::LSHIFT  );
	m_KeywordMapList["YB"]["/2"] = ENCODE(C_RSHIFT,   C_YB_ALU, AB_ALU::RSHIFT  );
	m_KeywordMapList["YB"]["+" ] = ENCODE(C_ADD,      C_YB_ALU, AB_ALU::ADD     );
	m_KeywordMapList["YB"]["-" ] = ENCODE(C_SUB,      C_YB_ALU, AB_ALU::SUB     );

	m_KeywordMapList["YB"]["YH" ] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::YH );
	m_KeywordMapList["YB"]["D1A"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D1A);
	m_KeywordMapList["YB"]["D1B"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D1B);
	m_KeywordMapList["YB"]["D1C"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D1C);
	m_KeywordMapList["YB"]["D1D"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D1D);
	m_KeywordMapList["YB"]["D1E"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D1E);
	m_KeywordMapList["YB"]["D1F"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D1F);
	m_KeywordMapList["YB"]["D1G"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D1G);
	m_KeywordMapList["YB"]["D1H"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D1H);
	m_KeywordMapList["YB"]["D2A"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D2A);
	m_KeywordMapList["YB"]["D2B"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D2B);
	m_KeywordMapList["YB"]["D2C"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D2C);
	m_KeywordMapList["YB"]["D2D"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D2D);
	m_KeywordMapList["YB"]["YOS"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::YOS);
	m_KeywordMapList["YB"]["YT" ] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::YT );
	m_KeywordMapList["YB"]["D4B"] = ENCODE(C_YB_A, AB_ALU::ASSIGN_A, AB_A::D4B);

}

void CPatternCompiler::CreateYNKeyword()
{
	m_KeywordMapList["YN"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["YN"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["YN"]["YC"] = ENCODE(C_SRC_SEL, AN_ALU::YN, AN_A::YC);
	m_KeywordMapList["YN"]["YS"] = ENCODE(C_SRC_SEL, AN_ALU::YN, AN_A::YS);
	m_KeywordMapList["YN"]["YK"] = ENCODE(C_SRC_SEL, AN_ALU::YN, AN_A::YK);
	m_KeywordMapList["YN"]["YA"] = ENCODE(C_SRC_SEL, AN_ALU::YN, AN_A::YA);
	m_KeywordMapList["YN"]["YD"] = ENCODE(C_SRC_SEL, AN_ALU::YN, AN_A::YD);
	m_KeywordMapList["YN"]["YE"] = ENCODE(C_SRC_SEL, AN_ALU::YN, AN_A::YE);
	m_KeywordMapList["YN"]["YF"] = ENCODE(C_SRC_SEL, AN_ALU::YN, AN_A::YF);
	m_KeywordMapList["YN"]["YB"] = ENCODE(C_SRC_SEL, AN_ALU::YN, AN_A::YB);

	m_KeywordMapList["YN"]["0" ] = ENCODE(C_RESET,    C_YN_ALU, AN_ALU::RESET   );
	m_KeywordMapList["YN"]["/" ] = ENCODE(C_INV,      C_YN_ALU, AN_ALU::INVERT  );
	m_KeywordMapList["YN"]["+1"] = ENCODE(C_INCREASE, C_YN_ALU, AN_ALU::INCREASE);
	m_KeywordMapList["YN"]["-1"] = ENCODE(C_DECREASE, C_YN_ALU, AN_ALU::DECREASE);
	m_KeywordMapList["YN"]["*2"] = ENCODE(C_LSHIFT,   C_YN_ALU, AN_ALU::LSHIFT  );
	m_KeywordMapList["YN"]["/2"] = ENCODE(C_RSHIFT,   C_YN_ALU, AN_ALU::RSHIFT  );
	m_KeywordMapList["YN"]["+" ] = ENCODE(C_ADD,      C_YN_ALU, AN_ALU::ADD     );
	m_KeywordMapList["YN"]["-" ] = ENCODE(C_SUB,      C_YN_ALU, AN_ALU::SUB     );
	m_KeywordMapList["YN"]["&" ] = ENCODE(C_AND,      C_YN_ALU, AN_ALU::AND     );
	m_KeywordMapList["YN"]["|" ] = ENCODE(C_OR,       C_YN_ALU, AN_ALU::OR      );
	m_KeywordMapList["YN"]["^" ] = ENCODE(C_XOR,      C_YN_ALU, AN_ALU::XOR     );

	m_KeywordMapList["YN"]["D1A" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D1A);
	m_KeywordMapList["YN"]["D1B" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D1B);
	m_KeywordMapList["YN"]["D1C" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D1C);
	m_KeywordMapList["YN"]["D1D" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D1D);
	m_KeywordMapList["YN"]["D1E" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D1E);
	m_KeywordMapList["YN"]["D1F" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D1F);
	m_KeywordMapList["YN"]["D1G" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D1G);
	m_KeywordMapList["YN"]["D1H" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D1H);
	m_KeywordMapList["YN"]["D2A" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D2A);
	m_KeywordMapList["YN"]["D2B" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D2B);
	m_KeywordMapList["YN"]["D2C" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D2C);
	m_KeywordMapList["YN"]["D2D" ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D2D);
	m_KeywordMapList["YN"]["D3"  ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D3);
	m_KeywordMapList["YN"]["D4"  ] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::D4);
	m_KeywordMapList["YN"]["YMAX"] = ENCODE(C_YN_B, AN_ALU::ASSIGN_B, AN_B::XMAX);
}

void CPatternCompiler::CreateD3Keyword()
{
	m_KeywordMapList["D3"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["D3"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["D3"]["D3" ] = ENCODE(C_SRC_SEL,  O_D3, AB_ALU::D3       );
	m_KeywordMapList["D3"]["0"  ] = ENCODE(C_RESET,    O_D3, AB_ALU::RESET    );
	m_KeywordMapList["D3"]["D3B"] = ENCODE(C_SRC_BASE, O_D3, AB_ALU::D3B      );
	m_KeywordMapList["D3"]["+1" ] = ENCODE(C_INCREASE, O_D3, AB_ALU::INCREASE );
	m_KeywordMapList["D3"]["-1" ] = ENCODE(C_DECREASE, O_D3, AB_ALU::DECREASE );
	m_KeywordMapList["D3"]["*2" ] = ENCODE(C_LSHIFT,   O_D3, AB_ALU::LSHIFT   );
	m_KeywordMapList["D3"]["/2" ] = ENCODE(C_RSHIFT,   O_D3, AB_ALU::RSHIFT   );
}

void CPatternCompiler::CreateD4Keyword()
{
	m_KeywordMapList["D4"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["D4"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["D4"]["D4" ] = ENCODE(C_SRC_SEL,  O_D4, AB_ALU::D4       );
	m_KeywordMapList["D4"]["0"  ] = ENCODE(C_RESET,    O_D4, AB_ALU::RESET    );
	m_KeywordMapList["D4"]["D4B"] = ENCODE(C_SRC_BASE, O_D4, AB_ALU::D4B      );
	m_KeywordMapList["D4"]["+1" ] = ENCODE(C_INCREASE, O_D4, AB_ALU::INCREASE );
	m_KeywordMapList["D4"]["-1" ] = ENCODE(C_DECREASE, O_D4, AB_ALU::DECREASE );
	m_KeywordMapList["D4"]["*2" ] = ENCODE(C_LSHIFT,   O_D4, AB_ALU::LSHIFT   );
	m_KeywordMapList["D4"]["/2" ] = ENCODE(C_RSHIFT,   O_D4, AB_ALU::RSHIFT   );
}

void CPatternCompiler::CreateZKeyword()
{
	m_KeywordMapList["Z"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["Z"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["Z"]["Z" ] = ENCODE(C_SRC_SEL,  O_Z, AB_ALU::Z       );
	m_KeywordMapList["Z"]["0" ] = ENCODE(C_RESET,    O_Z, AB_ALU::RESET    );
	m_KeywordMapList["Z"]["/" ] = ENCODE(C_INV,      O_Z, AB_ALU::INVERT   );
	m_KeywordMapList["Z"]["+1"] = ENCODE(C_INCREASE, O_Z, AB_ALU::INCREASE );
	m_KeywordMapList["Z"]["-1"] = ENCODE(C_DECREASE, O_Z, AB_ALU::DECREASE );
	m_KeywordMapList["Z"]["*2"] = ENCODE(C_LSHIFT,   O_Z, AB_ALU::LSHIFT   );
	m_KeywordMapList["Z"]["/2"] = ENCODE(C_RSHIFT,   O_Z, AB_ALU::RSHIFT   );
	m_KeywordMapList["Z"]["+" ] = ENCODE(C_ADD,      O_Z, AB_ALU::ADD      );
	m_KeywordMapList["Z"]["-" ] = ENCODE(C_SUB,      O_Z, AB_ALU::SUB      );

	m_KeywordMapList["Z"]["ZH"  ] = ENCODE(C_Z_A, O_Z_A, Z_A::ZH  );
	m_KeywordMapList["Z"]["ZMAX"] = ENCODE(C_Z_A, O_Z_A, Z_A::ZMAX);
}

void CPatternCompiler::CreateXKeyword()
{
	m_KeywordMapList["X"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["X"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["X"]["X" ] = ENCODE(C_SRC_SEL, O_X, X_OUT::X );
	m_KeywordMapList["X"]["XC"] = ENCODE(C_SRC_SEL, O_X, X_OUT::XC);
	m_KeywordMapList["X"]["XS"] = ENCODE(C_SRC_SEL, O_X, X_OUT::XS);
	m_KeywordMapList["X"]["XK"] = ENCODE(C_SRC_SEL, O_X, X_OUT::XK);
	m_KeywordMapList["X"]["XA"] = ENCODE(C_SRC_SEL, O_X, X_OUT::XA);
	m_KeywordMapList["X"]["XD"] = ENCODE(C_SRC_SEL, O_X, X_OUT::XD);
	m_KeywordMapList["X"]["XE"] = ENCODE(C_SRC_SEL, O_X, X_OUT::XE);
	m_KeywordMapList["X"]["XF"] = ENCODE(C_SRC_SEL, O_X, X_OUT::XF);
	m_KeywordMapList["X"]["XB"] = ENCODE(C_SRC_SEL, O_X, X_OUT::XB);

	m_KeywordMapList["X"]["YC"] = ENCODE(C_SRC_SEL, O_X, X_OUT::YC);
	m_KeywordMapList["X"]["YS"] = ENCODE(C_SRC_SEL, O_X, X_OUT::YS);
	m_KeywordMapList["X"]["YK"] = ENCODE(C_SRC_SEL, O_X, X_OUT::YK);
	m_KeywordMapList["X"]["YA"] = ENCODE(C_SRC_SEL, O_X, X_OUT::YA);
	m_KeywordMapList["X"]["YD"] = ENCODE(C_SRC_SEL, O_X, X_OUT::YD);
	m_KeywordMapList["X"]["YE"] = ENCODE(C_SRC_SEL, O_X, X_OUT::YE);
	m_KeywordMapList["X"]["YF"] = ENCODE(C_SRC_SEL, O_X, X_OUT::YF);
	m_KeywordMapList["X"]["YB"] = ENCODE(C_SRC_SEL, O_X, X_OUT::YB);

	m_KeywordMapList["X"]["XT" ] = ENCODE(C_SRC_SEL, O_X, X_OUT::XT );
	m_KeywordMapList["X"]["XOS"] = ENCODE(C_SRC_SEL, O_X, X_OUT::XOS);
	m_KeywordMapList["X"]["Z"  ] = ENCODE(C_SRC_SEL, O_X, X_OUT::Z  );
	m_KeywordMapList["X"]["RF" ] = ENCODE(C_SRC_SEL, O_X, X_OUT::RF );
}

void CPatternCompiler::CreateYKeyword()
{
	m_KeywordMapList["Y"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["Y"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["Y"]["Y" ] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::Y );
	m_KeywordMapList["Y"]["YC"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::YC);
	m_KeywordMapList["Y"]["YS"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::YS);
	m_KeywordMapList["Y"]["YK"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::YK);
	m_KeywordMapList["Y"]["YA"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::YA);
	m_KeywordMapList["Y"]["YD"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::YD);
	m_KeywordMapList["Y"]["YE"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::YE);
	m_KeywordMapList["Y"]["YF"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::YF);
	m_KeywordMapList["Y"]["YB"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::YB);

	m_KeywordMapList["Y"]["XC"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::XC);
	m_KeywordMapList["Y"]["XS"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::XS);
	m_KeywordMapList["Y"]["XK"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::XK);
	m_KeywordMapList["Y"]["XA"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::XA);
	m_KeywordMapList["Y"]["XD"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::XD);
	m_KeywordMapList["Y"]["XE"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::XE);
	m_KeywordMapList["Y"]["XF"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::XF);
	m_KeywordMapList["Y"]["XB"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::XB);

	m_KeywordMapList["Y"]["YT" ] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::YT );
	m_KeywordMapList["Y"]["YOS"] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::YOS);
	m_KeywordMapList["Y"]["Z"  ] = ENCODE(C_SRC_SEL, O_Y, Y_OUT::Z  );

}

void CPatternCompiler::CreateTP1Keyword()
{
	m_KeywordMapList["TP1"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["TP1"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["TP1"]["TP" ] = ENCODE(C_SRC_SEL,  O_TP1_ALU, TP_ALU::TP1      );
	m_KeywordMapList["TP1"]["TP1"] = ENCODE(C_SRC_SEL,  O_TP1_ALU, TP_ALU::TP1      );
	m_KeywordMapList["TP1"]["/"  ] = ENCODE(C_INV,      O_TP1_ALU, TP_ALU::INVERT   );
	m_KeywordMapList["TP1"]["+1" ] = ENCODE(C_INCREASE, O_TP1_ALU, TP_ALU::INCREASE );
	m_KeywordMapList["TP1"]["-1" ] = ENCODE(C_DECREASE, O_TP1_ALU, TP_ALU::DECREASE );
	m_KeywordMapList["TP1"]["*2" ] = ENCODE(C_LSHIFT,   O_TP1_ALU, TP_ALU::LSHIFT   );
	m_KeywordMapList["TP1"]["/2" ] = ENCODE(C_RSHIFT,   O_TP1_ALU, TP_ALU::RSHIFT   );
	m_KeywordMapList["TP1"]["+"  ] = ENCODE(C_ADD,      O_TP1_ALU, TP_ALU::ADD      );
	m_KeywordMapList["TP1"]["-"  ] = ENCODE(C_SUB,      O_TP1_ALU, TP_ALU::SUB      );
	m_KeywordMapList["TP1"]["&"  ] = ENCODE(C_AND,      O_TP1_ALU, TP_ALU::AND      );
	m_KeywordMapList["TP1"]["|"  ] = ENCODE(C_OR,       O_TP1_ALU, TP_ALU::OR       );

	m_KeywordMapList["TP1"]["TPH1" ] = ENCODE(C_OTH, O_TP1_A, TP1_A::TPH1  );
	m_KeywordMapList["TP1"]["TPH1A"] = ENCODE(C_OTH, O_TP1_A, TP1_A::TPH1A );
	m_KeywordMapList["TP1"]["TPH1B"] = ENCODE(C_OTH, O_TP1_A, TP1_A::TPH1B );
	m_KeywordMapList["TP1"]["TPH1C"] = ENCODE(C_OTH, O_TP1_A, TP1_A::TPH1C );
	m_KeywordMapList["TP1"]["TPH1D"] = ENCODE(C_OTH, O_TP1_A, TP1_A::TPH1D );
	m_KeywordMapList["TP1"]["D5"   ] = ENCODE(C_OTH, O_TP1_A, TP1_A::D5    );
	m_KeywordMapList["TP1"]["D7"   ] = ENCODE(C_OTH, O_TP1_A, TP1_A::D7    );
	m_KeywordMapList["TP1"]["XMASK"] = ENCODE(C_OTH, O_TP1_A, TP1_A::XMASK );
}

void CPatternCompiler::CreateTP2Keyword()
{
	m_KeywordMapList["TP2"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["TP2"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["TP2"]["TP2"] = ENCODE(C_SRC_SEL,  O_TP2_ALU, TP_ALU::TP2      );
	m_KeywordMapList["TP2"]["/"  ] = ENCODE(C_INV,      O_TP2_ALU, TP_ALU::INVERT   );
	m_KeywordMapList["TP2"]["+1" ] = ENCODE(C_INCREASE, O_TP2_ALU, TP_ALU::INCREASE );
	m_KeywordMapList["TP2"]["-1" ] = ENCODE(C_DECREASE, O_TP2_ALU, TP_ALU::DECREASE );
	m_KeywordMapList["TP2"]["*2" ] = ENCODE(C_LSHIFT,   O_TP2_ALU, TP_ALU::LSHIFT   );
	m_KeywordMapList["TP2"]["/2" ] = ENCODE(C_RSHIFT,   O_TP2_ALU, TP_ALU::RSHIFT   );
	m_KeywordMapList["TP2"]["+"  ] = ENCODE(C_ADD,      O_TP2_ALU, TP_ALU::ADD      );
	m_KeywordMapList["TP2"]["-"  ] = ENCODE(C_SUB,      O_TP2_ALU, TP_ALU::SUB      );
	m_KeywordMapList["TP2"]["&"  ] = ENCODE(C_AND,      O_TP2_ALU, TP_ALU::AND      );
	m_KeywordMapList["TP2"]["|"  ] = ENCODE(C_OR,       O_TP2_ALU, TP_ALU::OR       );

	m_KeywordMapList["TP2"]["TPH2" ] = ENCODE(C_OTH, O_TP2_A, TP2_A::TPH2  );
	m_KeywordMapList["TP2"]["TPH2A"] = ENCODE(C_OTH, O_TP2_A, TP2_A::TPH2A );
	m_KeywordMapList["TP2"]["TPH2B"] = ENCODE(C_OTH, O_TP2_A, TP2_A::TPH2B );
	m_KeywordMapList["TP2"]["TPH2C"] = ENCODE(C_OTH, O_TP2_A, TP2_A::TPH2C );
	m_KeywordMapList["TP2"]["TPH2D"] = ENCODE(C_OTH, O_TP2_A, TP2_A::TPH2D );
	m_KeywordMapList["TP2"]["D6"   ] = ENCODE(C_OTH, O_TP2_A, TP2_A::D6    );
	m_KeywordMapList["TP2"]["D8"   ] = ENCODE(C_OTH, O_TP2_A, TP2_A::D8    );
	m_KeywordMapList["TP2"]["YMASK"] = ENCODE(C_OTH, O_TP2_A, TP2_A::YMASK );
}

void CPatternCompiler::CreateD7Keyword()
{
	m_KeywordMapList["D7"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["D7"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["D7"]["D7"] = ENCODE(C_SRC_SEL,  O_D7, D78_ALU::D7       );
	m_KeywordMapList["D7"]["0" ] = ENCODE(C_RESET,    O_D7, D78_ALU::RESET    );
	m_KeywordMapList["D7"]["+1"] = ENCODE(C_INCREASE, O_D7, D78_ALU::INCREASE );
	m_KeywordMapList["D7"]["-1"] = ENCODE(C_DECREASE, O_D7, D78_ALU::DECREASE );
	m_KeywordMapList["D7"]["+" ] = ENCODE(C_ADD,      O_D7, D78_ALU::ADD      );
	m_KeywordMapList["D7"]["-" ] = ENCODE(C_SUB,      O_D7, D78_ALU::SUB      );
	m_KeywordMapList["D7"]["/2"] = ENCODE(C_RSHIFT,   O_D7, D78_ALU::RSHIFT   );

	m_KeywordMapList["D7"]["TP"   ] = ENCODE(C_OTH, O_D7_A, D7_A::TP1   );
	m_KeywordMapList["D7"]["TP1"  ] = ENCODE(C_OTH, O_D7_A, D7_A::TP1   );
	m_KeywordMapList["D7"]["TPH1" ] = ENCODE(C_OTH, O_D7_A, D7_A::TPH1  );
	m_KeywordMapList["D7"]["TPH1A"] = ENCODE(C_OTH, O_D7_A, D7_A::TPH1A );
	m_KeywordMapList["D7"]["TPH1B"] = ENCODE(C_OTH, O_D7_A, D7_A::TPH1B );
	m_KeywordMapList["D7"]["TPH1C"] = ENCODE(C_OTH, O_D7_A, D7_A::TPH1C );
	m_KeywordMapList["D7"]["TPH1D"] = ENCODE(C_OTH, O_D7_A, D7_A::TPH1D );
	m_KeywordMapList["D7"]["D5"   ] = ENCODE(C_OTH, O_D7_A, D7_A::D5    );
}

void CPatternCompiler::CreateD8Keyword()
{
	m_KeywordMapList["D8"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["D8"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["D8"]["D8"] = ENCODE(C_SRC_SEL,  O_D8, D78_ALU::D8       );
	m_KeywordMapList["D8"]["0" ] = ENCODE(C_RESET,    O_D8, D78_ALU::RESET    );
	m_KeywordMapList["D8"]["+1"] = ENCODE(C_INCREASE, O_D8, D78_ALU::INCREASE );
	m_KeywordMapList["D8"]["-1"] = ENCODE(C_DECREASE, O_D8, D78_ALU::DECREASE );
	m_KeywordMapList["D8"]["+" ] = ENCODE(C_ADD,      O_D8, D78_ALU::ADD      );
	m_KeywordMapList["D8"]["-" ] = ENCODE(C_SUB,      O_D8, D78_ALU::SUB      );
	m_KeywordMapList["D8"]["/2"] = ENCODE(C_RSHIFT,   O_D8, D78_ALU::RSHIFT   );

	m_KeywordMapList["D8"]["TP2"  ] = ENCODE(C_OTH, O_D8_A, D8_A::TP2   );
	m_KeywordMapList["D8"]["TPH2" ] = ENCODE(C_OTH, O_D8_A, D8_A::TPH2  );
	m_KeywordMapList["D8"]["TPH2A"] = ENCODE(C_OTH, O_D8_A, D8_A::TPH2A );
	m_KeywordMapList["D8"]["TPH2B"] = ENCODE(C_OTH, O_D8_A, D8_A::TPH2B );
	m_KeywordMapList["D8"]["TPH2C"] = ENCODE(C_OTH, O_D8_A, D8_A::TPH2C );
	m_KeywordMapList["D8"]["TPH2D"] = ENCODE(C_OTH, O_D8_A, D8_A::TPH2D );
	m_KeywordMapList["D8"]["D6"   ] = ENCODE(C_OTH, O_D8_A, D8_A::D6    );
}

void CPatternCompiler::CreateSAKeyword()
{
	m_KeywordMapList["SA"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["SA"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["SA"]["SA" ] = ENCODE(C_SRC_SEL, O_SA, SAB_ALU::SA    );
	m_KeywordMapList["SA"]["0"  ] = ENCODE(C_RESET,   O_SA, SAB_ALU::RESET );
	m_KeywordMapList["SA"]["SB" ] = ENCODE(C_SB,      O_SA, SAB_ALU::RIVAL );
	m_KeywordMapList["SA"]["S1A"] = ENCODE(C_OTH,     O_SA, SAB_ALU::REG   );
	m_KeywordMapList["SA"]["+"  ] = ENCODE(C_ADD,     O_SA, SAB_ALU::ADD   );
	m_KeywordMapList["SA"]["-"  ] = ENCODE(C_SUB,     O_SA, SAB_ALU::SUB   );
	m_KeywordMapList["SA"]["+1" ] = ENCODE(C_INCREASE,O_SA, SAB_ALU::ADD   );
	m_KeywordMapList["SA"]["-1" ] = ENCODE(C_DECREASE,O_SA, SAB_ALU::SUB   );
}

void CPatternCompiler::CreateSBKeyword()
{
	m_KeywordMapList["SB"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["SB"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["SB"]["SB" ] = ENCODE(C_SRC_SEL, O_SA, SAB_ALU::SB);
	m_KeywordMapList["SB"]["0"  ] = ENCODE(C_RESET,   O_SA, SAB_ALU::RESET);
	m_KeywordMapList["SB"]["SA" ] = ENCODE(C_SA,      O_SA, SAB_ALU::RIVAL);
	m_KeywordMapList["SB"]["S1A"] = ENCODE(C_OTH,     O_SA, SAB_ALU::REG);
	m_KeywordMapList["SB"]["+"  ] = ENCODE(C_ADD,     O_SA, SAB_ALU::ADD);
	m_KeywordMapList["SB"]["-"  ] = ENCODE(C_SUB,     O_SA, SAB_ALU::SUB);
	m_KeywordMapList["SB"]["+1" ] = ENCODE(C_INCREASE,O_SA, SAB_ALU::ADD   );
	m_KeywordMapList["SB"]["-1" ] = ENCODE(C_DECREASE,O_SA, SAB_ALU::SUB   );
}

void CPatternCompiler::CreateSKeyword()
{
	m_KeywordMapList["S"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["S"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["S"]["S" ] = ENCODE(C_SRC_SEL, O_S, 0);
	m_KeywordMapList["S"]["SA"] = ENCODE(C_SRC_SEL, O_S, 1);
	m_KeywordMapList["S"]["SB"] = ENCODE(C_SRC_SEL, O_S, 2);
}

void CPatternCompiler::CreateRFKeyword()
{
	m_KeywordMapList["RF"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["RF"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);

	m_KeywordMapList["RF"]["RF"] = ENCODE(C_SRC_SEL,  O_RF, 0);
	m_KeywordMapList["RF"]["0" ] = ENCODE(C_RESET,    O_RF, 1);
	m_KeywordMapList["RF"]["+1"] = ENCODE(C_INCREASE, O_RF, 2);
}

void CPatternCompiler::CreateDXKeyword()
{
	m_KeywordMapList["DX"]["="] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["DX"]["<"] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
}

void CPatternCompiler::CreateTPHXKeyword()
{
	m_KeywordMapList["TPH1X"]["="  ] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["TPH1X"]["<"  ] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["TPH1X"]["TP1"] = ENCODE(C_TP1,    0,        0);
	m_KeywordMapList["TPH1X"]["TP" ] = ENCODE(C_TP1,    0,        0);

	m_KeywordMapList["TPH2X"]["="  ] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["TPH2X"]["<"  ] = ENCODE(C_ASSIGN, C_ASSIGN, 0);
	m_KeywordMapList["TPH2X"]["TP2"] = ENCODE(C_TP2,    0,        0);
}

void CPatternCompiler::CreateFirstKeyword()
{
	// opcode all
	m_FirstKeyword["NOP"        ] = ENCODE(C_LONE_OP, 0, NOP         );
	//m_FirstKeyword["FLGLI1"     ] = ENCODE(C_JUMP_OP, 0, FLGLI       );
	m_FirstKeyword["JZD"        ] = ENCODE(C_JUMP_OP, 0, JZD         );
	m_FirstKeyword["JET"        ] = ENCODE(C_JUMP_OP, 0, JET         );
	m_FirstKeyword["RTN"        ] = ENCODE(C_LONE_OP, 0, RTN         );
	m_FirstKeyword["JRB"		] = ENCODE(C_OPRN_OP, 0, JRB         );
	m_FirstKeyword["STPS"       ] = ENCODE(C_LONE_OP, 0, STPS        );
	m_FirstKeyword["JMP"        ] = ENCODE(C_JUMP_OP, 0, JMP         );
	m_FirstKeyword["JSR"        ] = ENCODE(C_JUMP_OP, 0, JSR         );
	m_FirstKeyword["OUT"        ] = ENCODE(C_OPRN_OP, 0, OUT         );
	//m_FirstKeyword["REF_START"  ] = ENCODE(C_LONE_OP, 0, REF_START   );
	//m_FirstKeyword["TM_START"   ] = ENCODE(C_LONE_OP, 0, TM_START    );
	m_FirstKeyword["TIMER0"     ] = ENCODE(C_SET_OP_TIMER, 0, TIMER0); //SET Refresh Time
	m_FirstKeyword["TIMER1"     ] = ENCODE(C_SET_OP_TIMER, 0, TIMER1); //SET Jet Time
	m_FirstKeyword["REPEAT"     ] = ENCODE(C_OPRN_OP, 0, REPEAT      );
	m_FirstKeyword["PAUSE"      ] = ENCODE(C_SET_OP_TIMER, 0, PAUSE  );
	m_FirstKeyword["JNC0"       ] = ENCODE(C_JUMP_OP, 0, JNC0        );
	m_FirstKeyword["JNC1"       ] = ENCODE(C_JUMP_OP, 0, JNC1        );
	m_FirstKeyword["JNC2"       ] = ENCODE(C_JUMP_OP, 0, JNC2        );
	m_FirstKeyword["JNC3"       ] = ENCODE(C_JUMP_OP, 0, JNC3        );
	m_FirstKeyword["JNC4"       ] = ENCODE(C_JUMP_OP, 0, JNC4        );
	m_FirstKeyword["JNC5"       ] = ENCODE(C_JUMP_OP, 0, JNC5        );
	m_FirstKeyword["JNC6"       ] = ENCODE(C_JUMP_OP, 0, JNC6        );
	m_FirstKeyword["JNC7"       ] = ENCODE(C_JUMP_OP, 0, JNC7        );
	m_FirstKeyword["JNC8"       ] = ENCODE(C_JUMP_OP, 0, JNC8        );
	m_FirstKeyword["JNC9"       ] = ENCODE(C_JUMP_OP, 0, JNC9        );
	m_FirstKeyword["JNC10"      ] = ENCODE(C_JUMP_OP, 0, JNC10       );
	m_FirstKeyword["JNC11"      ] = ENCODE(C_JUMP_OP, 0, JNC11       );
	m_FirstKeyword["JNC12"      ] = ENCODE(C_JUMP_OP, 0, JNC12       );
	m_FirstKeyword["JNC13"      ] = ENCODE(C_JUMP_OP, 0, JNC13       );
	m_FirstKeyword["JNC14"      ] = ENCODE(C_JUMP_OP, 0, JNC14       );
	m_FirstKeyword["JNC15"      ] = ENCODE(C_JUMP_OP, 0, JNC15       );
	m_FirstKeyword["JNI0"       ] = ENCODE(C_JUMP_OP, 0, JNI0        );
	m_FirstKeyword["JNI1"       ] = ENCODE(C_JUMP_OP, 0, JNI1        );
	m_FirstKeyword["JNI2"       ] = ENCODE(C_JUMP_OP, 0, JNI2        );
	m_FirstKeyword["JNI3"       ] = ENCODE(C_JUMP_OP, 0, JNI3        );
	m_FirstKeyword["JNI4"       ] = ENCODE(C_JUMP_OP, 0, JNI4        );
	m_FirstKeyword["JNI5"       ] = ENCODE(C_JUMP_OP, 0, JNI5        );
	m_FirstKeyword["JNI6"       ] = ENCODE(C_JUMP_OP, 0, JNI6        );
	m_FirstKeyword["JNI7"       ] = ENCODE(C_JUMP_OP, 0, JNI7        );
	m_FirstKeyword["JNI8"       ] = ENCODE(C_JUMP_OP, 0, JNI8        );
	m_FirstKeyword["JNI9"       ] = ENCODE(C_JUMP_OP, 0, JNI9        );
	m_FirstKeyword["JNI10"      ] = ENCODE(C_JUMP_OP, 0, JNI10       );
	m_FirstKeyword["JNI11"      ] = ENCODE(C_JUMP_OP, 0, JNI11       );
	m_FirstKeyword["JNI12"      ] = ENCODE(C_JUMP_OP, 0, JNI12       );
	m_FirstKeyword["JNI13"      ] = ENCODE(C_JUMP_OP, 0, JNI13       );
	m_FirstKeyword["JNI14"      ] = ENCODE(C_JUMP_OP, 0, JNI14       );
	m_FirstKeyword["JNI15"      ] = ENCODE(C_JUMP_OP, 0, JNI15       );
	m_FirstKeyword["JNI0D"      ] = ENCODE(C_JUMP_OP, 0, JNI0D       );
	m_FirstKeyword["JNI1D"      ] = ENCODE(C_JUMP_OP, 0, JNI1D       );
	m_FirstKeyword["JNI2D"      ] = ENCODE(C_JUMP_OP, 0, JNI2D       );
	m_FirstKeyword["JNI3D"      ] = ENCODE(C_JUMP_OP, 0, JNI3D       );
	m_FirstKeyword["JNI4D"      ] = ENCODE(C_JUMP_OP, 0, JNI4D       );
	m_FirstKeyword["JNI5D"      ] = ENCODE(C_JUMP_OP, 0, JNI5D       );
	m_FirstKeyword["JNI6D"      ] = ENCODE(C_JUMP_OP, 0, JNI6D       );
	m_FirstKeyword["JNI7D"      ] = ENCODE(C_JUMP_OP, 0, JNI7D       );
	m_FirstKeyword["INC0"       ] = ENCODE(C_LONE_OP, 0, INC0        );
	m_FirstKeyword["INC1"       ] = ENCODE(C_LONE_OP, 0, INC1        );
	m_FirstKeyword["INC2"       ] = ENCODE(C_LONE_OP, 0, INC2        );
	m_FirstKeyword["INC3"       ] = ENCODE(C_LONE_OP, 0, INC3        );
	m_FirstKeyword["INC4"       ] = ENCODE(C_LONE_OP, 0, INC4        );
	m_FirstKeyword["INC5"       ] = ENCODE(C_LONE_OP, 0, INC5        );
	m_FirstKeyword["INC6"       ] = ENCODE(C_LONE_OP, 0, INC6        );
	m_FirstKeyword["INC7"       ] = ENCODE(C_LONE_OP, 0, INC7        );
	m_FirstKeyword["STI0"       ] = ENCODE(C_SET_OP_STI,  0, STI0        );
	m_FirstKeyword["STI1"       ] = ENCODE(C_SET_OP_STI,  0, STI1        );
	m_FirstKeyword["STI2"       ] = ENCODE(C_SET_OP_STI,  0, STI2        );
	m_FirstKeyword["STI3"       ] = ENCODE(C_SET_OP_STI,  0, STI3        );
	m_FirstKeyword["STI4"       ] = ENCODE(C_SET_OP_STI,  0, STI4        );
	m_FirstKeyword["STI5"       ] = ENCODE(C_SET_OP_STI,  0, STI5        );
	m_FirstKeyword["STI6"       ] = ENCODE(C_SET_OP_STI,  0, STI6        );
	m_FirstKeyword["STI7"       ] = ENCODE(C_SET_OP_STI,  0, STI7        );
	m_FirstKeyword["STI8"       ] = ENCODE(C_SET_OP_STI,  0, STI8        );
	m_FirstKeyword["STBAR"      ] = ENCODE(C_SET_OP,  0, STBAR       );
	m_FirstKeyword["STISP"      ] = ENCODE(C_SET_OP,  0, STISP       );
	m_FirstKeyword["LDI0"       ] = ENCODE(C_SET_OP,  0, LDI0        );
	m_FirstKeyword["LDI1"       ] = ENCODE(C_SET_OP,  0, LDI1        );
	m_FirstKeyword["LDI2"       ] = ENCODE(C_SET_OP,  0, LDI2        );
	m_FirstKeyword["LDI3"       ] = ENCODE(C_SET_OP,  0, LDI3        );
	m_FirstKeyword["LDI4"       ] = ENCODE(C_SET_OP,  0, LDI4        );
	m_FirstKeyword["LDI5"       ] = ENCODE(C_SET_OP,  0, LDI5        );
	m_FirstKeyword["LDI6"       ] = ENCODE(C_SET_OP,  0, LDI6        );
	m_FirstKeyword["LDI7"       ] = ENCODE(C_SET_OP,  0, LDI7        );
	m_FirstKeyword["MODE"       ] = ENCODE(C_MODE_OP, 0, MODE        );
	m_FirstKeyword["VMPA"       ] = ENCODE(C_OPRN_OP, 0, VMPA        );

	// OP CTRL
	m_FirstKeyword["I" ] = ENCODE(C_OP_CTRL, 0, OP_CTRL::I  );
	m_FirstKeyword["T0"] = ENCODE(C_OP_CTRL, 0, OP_CTRL::T0 );
	m_FirstKeyword["T1"] = ENCODE(C_OP_CTRL, 0, OP_CTRL::T1 );
	m_FirstKeyword["T" ] = ENCODE(C_OP_CTRL, 0, OP_CTRL::T1 | OP_CTRL::T0);

	// Address
	m_FirstKeyword["XB"] = ENCODE(C_XB, 0, 0);

	m_FirstKeyword["XC"] = ENCODE(C_XN, O_XN_SEL, 0);
	m_FirstKeyword["XS"] = ENCODE(C_XN, O_XN_SEL, 1);
	m_FirstKeyword["XK"] = ENCODE(C_XN, O_XN_SEL, 2);
	m_FirstKeyword["XA"] = ENCODE(C_XN, O_XN_SEL, 3);
	m_FirstKeyword["XD"] = ENCODE(C_XN, O_XN_SEL, 4);
	m_FirstKeyword["XE"] = ENCODE(C_XN, O_XN_SEL, 5);
	m_FirstKeyword["XF"] = ENCODE(C_XN, O_XN_SEL, 6);

	m_FirstKeyword["YB"] = ENCODE(C_YB, 0, 0);

	m_FirstKeyword["YC"] = ENCODE(C_YN, O_YN_SEL, 0);
	m_FirstKeyword["YS"] = ENCODE(C_YN, O_YN_SEL, 1);
	m_FirstKeyword["YK"] = ENCODE(C_YN, O_YN_SEL, 2);
	m_FirstKeyword["YA"] = ENCODE(C_YN, O_YN_SEL, 3);
	m_FirstKeyword["YD"] = ENCODE(C_YN, O_YN_SEL, 4);
	m_FirstKeyword["YE"] = ENCODE(C_YN, O_YN_SEL, 5);
	m_FirstKeyword["YF"] = ENCODE(C_YN, O_YN_SEL, 6);

	m_FirstKeyword["D3"] = ENCODE(C_D3, 0, 0);
	m_FirstKeyword["D4"] = ENCODE(C_D4, 0, 0);

	m_FirstKeyword["Z"] = ENCODE(C_Z, 0, 0);

	m_FirstKeyword["X"] = ENCODE(C_X, 0, 0);
	m_FirstKeyword["Y"] = ENCODE(C_Y, 0, 0);

	// Data
	m_FirstKeyword["TP" ] = ENCODE(C_TP1, 0, 0);
	m_FirstKeyword["TP1"] = ENCODE(C_TP1, 0, 0);
	m_FirstKeyword["TP2"] = ENCODE(C_TP2, 0, 0);

	m_FirstKeyword["TPH1" ] = ENCODE(C_TPH1, 0, TP1_A::TPH1 );
	m_FirstKeyword["TPH1A"] = ENCODE(C_TPH1, 0, TP1_A::TPH1A);
	m_FirstKeyword["TPH1B"] = ENCODE(C_TPH1, 0, TP1_A::TPH1B);
	m_FirstKeyword["TPH1C"] = ENCODE(C_TPH1, 0, TP1_A::TPH1C);
	m_FirstKeyword["TPH1D"] = ENCODE(C_TPH1, 0, TP1_A::TPH1D);
	m_FirstKeyword["D5"   ] = ENCODE(C_TPH1, 0, TP1_A::D5   );
	
	m_FirstKeyword["TPH2" ] = ENCODE(C_TPH2, 0, TP2_A::TPH2 );
	m_FirstKeyword["TPH2A"] = ENCODE(C_TPH2, 0, TP2_A::TPH2A);
	m_FirstKeyword["TPH2B"] = ENCODE(C_TPH2, 0, TP2_A::TPH2B);
	m_FirstKeyword["TPH2C"] = ENCODE(C_TPH2, 0, TP2_A::TPH2C);
	m_FirstKeyword["TPH2D"] = ENCODE(C_TPH2, 0, TP2_A::TPH2D);
	m_FirstKeyword["D6"   ] = ENCODE(C_TPH2, 0, TP2_A::D6   );

	m_FirstKeyword["D7"] = ENCODE(C_D7, 0, 0);
	m_FirstKeyword["D8"] = ENCODE(C_D8, 0, 0);

	// SCAN
	m_FirstKeyword["SA"] = ENCODE(C_SA, 0, 0);
	m_FirstKeyword["SB"] = ENCODE(C_SB, 0, 0);

	m_FirstKeyword["S"] = ENCODE(C_S, 0, 0);

	m_FirstKeyword["RF"] = ENCODE(C_RF, 0, 0);

	// FP
	m_FirstKeyword["FP0" ] = ENCODE(C_FP, 0, 0);
	m_FirstKeyword["FP1" ] = ENCODE(C_FP, 0, 1);
	m_FirstKeyword["FP2" ] = ENCODE(C_FP, 0, 2);
	m_FirstKeyword["FP3" ] = ENCODE(C_FP, 0, 3);
	m_FirstKeyword["FP4" ] = ENCODE(C_FP, 0, 4);
	m_FirstKeyword["FP5" ] = ENCODE(C_FP, 0, 5);
	m_FirstKeyword["FP6" ] = ENCODE(C_FP, 0, 6);
	m_FirstKeyword["FP7" ] = ENCODE(C_FP, 0, 7);
	m_FirstKeyword["FP8" ] = ENCODE(C_FP, 0, 8);
	m_FirstKeyword["FP9" ] = ENCODE(C_FP, 0, 9);
	m_FirstKeyword["FP10"] = ENCODE(C_FP, 0, 10);
	m_FirstKeyword["FPX" ] = ENCODE(C_FP, 0, 11);

	// Ctrl
	m_FirstKeyword["ASCROFF"] = ENCODE(C_CTRL, 0, CTRL::ASCROFF);
	m_FirstKeyword["DSCROFF"] = ENCODE(C_CTRL, 0, CTRL::DSCROFF);
	//m_FirstKeyword["TPX1"   ] = ENCODE(C_CTRL, 0, CTRL::TPX1);
	//m_FirstKeyword["TPX2"   ] = ENCODE(C_CTRL, 0, CTRL::TPX2);
	m_FirstKeyword["/X"     ] = ENCODE(C_CTRL, 0, CTRL::XINV);
	m_FirstKeyword["/Y"     ] = ENCODE(C_CTRL, 0, CTRL::YINV);
	m_FirstKeyword["/Z"     ] = ENCODE(C_CTRL, 0, CTRL::ZINV);
	m_FirstKeyword["/D"     ] = ENCODE(C_CTRL, 0, CTRL::D1INV);
	m_FirstKeyword["/D1"    ] = ENCODE(C_CTRL, 0, CTRL::D1INV);
	m_FirstKeyword["/D2"    ] = ENCODE(C_CTRL, 0, CTRL::D2INV);
//	m_FirstKeyword["BS"     ] = ENCODE(C_CTRL, 0, CTRL::BS);
//	m_FirstKeyword["SP"     ] = ENCODE(C_CTRL, 0, CTRL::SP);
//	m_FirstKeyword["M2"     ] = ENCODE(C_CTRL, 0, CTRL::M2);
//	m_FirstKeyword["RINV0"  ] = ENCODE(C_CTRL, 0, CTRL::RINV0);
//	m_FirstKeyword["RINV1"  ] = ENCODE(C_CTRL, 0, CTRL::RINV1);

	// CMD
	m_FirstKeyword["C0" ] = ENCODE(C_CMD, O_CMD0, 0x0001);
	m_FirstKeyword["C1" ] = ENCODE(C_CMD, O_CMD0, 0x0002);
	m_FirstKeyword["C2" ] = ENCODE(C_CMD, O_CMD0, 0x0004);
	m_FirstKeyword["C3" ] = ENCODE(C_CMD, O_CMD0, 0x0008);
	m_FirstKeyword["C4" ] = ENCODE(C_CMD, O_CMD0, 0x0010);
	m_FirstKeyword["C5" ] = ENCODE(C_CMD, O_CMD0, 0x0020);
	m_FirstKeyword["C6" ] = ENCODE(C_CMD, O_CMD0, 0x0040);
	m_FirstKeyword["C7" ] = ENCODE(C_CMD, O_CMD0, 0x0080);
	m_FirstKeyword["C8" ] = ENCODE(C_CMD, O_CMD0, 0x0100);
	m_FirstKeyword["C9" ] = ENCODE(C_CMD, O_CMD0, 0x0200);
	m_FirstKeyword["C10"] = ENCODE(C_CMD, O_CMD0, 0x0400);
	m_FirstKeyword["C11"] = ENCODE(C_CMD, O_CMD0, 0x0800);
	m_FirstKeyword["C12"] = ENCODE(C_CMD, O_CMD0, 0x1000);
	m_FirstKeyword["C13"] = ENCODE(C_CMD, O_CMD0, 0x2000);
	m_FirstKeyword["C14"] = ENCODE(C_CMD, O_CMD0, 0x4000);
	m_FirstKeyword["C15"] = ENCODE(C_CMD, O_CMD0, 0x8000);

	m_FirstKeyword["C16"] = ENCODE(C_CMD, O_CMD1, 0x0001);
	m_FirstKeyword["C17"] = ENCODE(C_CMD, O_CMD1, 0x0002);
	m_FirstKeyword["C18"] = ENCODE(C_CMD, O_CMD1, 0x0004);
	m_FirstKeyword["C19"] = ENCODE(C_CMD, O_CMD1, 0x0008);
	m_FirstKeyword["C20"] = ENCODE(C_CMD, O_CMD1, 0x0010);
	m_FirstKeyword["C21"] = ENCODE(C_CMD, O_CMD1, 0x0020);
	m_FirstKeyword["C22"] = ENCODE(C_CMD, O_CMD1, 0x0040);
	m_FirstKeyword["C23"] = ENCODE(C_CMD, O_CMD1, 0x0080);
	m_FirstKeyword["C24"] = ENCODE(C_CMD, O_CMD1, 0x0100);
	m_FirstKeyword["C25"] = ENCODE(C_CMD, O_CMD1, 0x0200);
	m_FirstKeyword["C26"] = ENCODE(C_CMD, O_CMD1, 0x0400);
	m_FirstKeyword["C27"] = ENCODE(C_CMD, O_CMD1, 0x0800);
	m_FirstKeyword["C28"] = ENCODE(C_CMD, O_CMD1, 0x1000);
	m_FirstKeyword["C29"] = ENCODE(C_CMD, O_CMD1, 0x2000);
	m_FirstKeyword["C30"] = ENCODE(C_CMD, O_CMD1, 0x4000);
	m_FirstKeyword["C31"] = ENCODE(C_CMD, O_CMD1, 0x8000);

	m_FirstKeyword["C32"] = ENCODE(C_CMD, O_CMD2, 0x0001);
	m_FirstKeyword["C33"] = ENCODE(C_CMD, O_CMD2, 0x0002);
	m_FirstKeyword["C34"] = ENCODE(C_CMD, O_CMD2, 0x0004);
	m_FirstKeyword["C35"] = ENCODE(C_CMD, O_CMD2, 0x0008);
	m_FirstKeyword["C36"] = ENCODE(C_CMD, O_CMD2, 0x0010);
	m_FirstKeyword["C37"] = ENCODE(C_CMD, O_CMD2, 0x0020);
	m_FirstKeyword["C38"] = ENCODE(C_CMD, O_CMD2, 0x0040);
	m_FirstKeyword["C39"] = ENCODE(C_CMD, O_CMD2, 0x0080);

	// Cycle Palette
	m_FirstKeyword["CYP0" ] = ENCODE(C_CYP, 0, CYP::CYP0 );
	m_FirstKeyword["CYP1" ] = ENCODE(C_CYP, 0, CYP::CYP1 );
	m_FirstKeyword["CYP2" ] = ENCODE(C_CYP, 0, CYP::CYP2 );
	m_FirstKeyword["CYP3" ] = ENCODE(C_CYP, 0, CYP::CYP3 );
	m_FirstKeyword["CYP4" ] = ENCODE(C_CYP, 0, CYP::CYP4 );
	m_FirstKeyword["CYP5" ] = ENCODE(C_CYP, 0, CYP::CYP5 );
	m_FirstKeyword["CYP6" ] = ENCODE(C_CYP, 0, CYP::CYP6 );
	m_FirstKeyword["CYP7" ] = ENCODE(C_CYP, 0, CYP::CYP7 );
	m_FirstKeyword["CYP8" ] = ENCODE(C_CYP, 0, CYP::CYP8 );
	m_FirstKeyword["CYP9" ] = ENCODE(C_CYP, 0, CYP::CYP9 );
	m_FirstKeyword["CYP10"] = ENCODE(C_CYP, 0, CYP::CYP10);
	m_FirstKeyword["CYP11"] = ENCODE(C_CYP, 0, CYP::CYP11);
	m_FirstKeyword["CYP12"] = ENCODE(C_CYP, 0, CYP::CYP12);
	m_FirstKeyword["CYP13"] = ENCODE(C_CYP, 0, CYP::CYP13);
	m_FirstKeyword["CYP14"] = ENCODE(C_CYP, 0, CYP::CYP14);
	m_FirstKeyword["CYP15"] = ENCODE(C_CYP, 0, CYP::CYP15);

	// Time set
	m_FirstKeyword["TS0" ] = ENCODE(C_TS, 0, TSET::TS0 );
	m_FirstKeyword["TS1" ] = ENCODE(C_TS, 0, TSET::TS1 );
	m_FirstKeyword["TS2" ] = ENCODE(C_TS, 0, TSET::TS2 );
	m_FirstKeyword["TS3" ] = ENCODE(C_TS, 0, TSET::TS3 );
	m_FirstKeyword["TS4" ] = ENCODE(C_TS, 0, TSET::TS4 );
	m_FirstKeyword["TS5" ] = ENCODE(C_TS, 0, TSET::TS5 );
	m_FirstKeyword["TS6" ] = ENCODE(C_TS, 0, TSET::TS6 );
	m_FirstKeyword["TS7" ] = ENCODE(C_TS, 0, TSET::TS7 );
	m_FirstKeyword["TS8" ] = ENCODE(C_TS, 0, TSET::TS8 );
	m_FirstKeyword["TS9" ] = ENCODE(C_TS, 0, TSET::TS9 );
	m_FirstKeyword["TS10"] = ENCODE(C_TS, 0, TSET::TS10);
	m_FirstKeyword["TS11"] = ENCODE(C_TS, 0, TSET::TS11);
	m_FirstKeyword["TS12"] = ENCODE(C_TS, 0, TSET::TS12);
	m_FirstKeyword["TS13"] = ENCODE(C_TS, 0, TSET::TS13);
	m_FirstKeyword["TS14"] = ENCODE(C_TS, 0, TSET::TS14);
	m_FirstKeyword["TS15"] = ENCODE(C_TS, 0, TSET::TS15);

	m_FirstKeyword["end_page" ] = ENCODE(C_NAND_CTRL, 0, 1);
	m_FirstKeyword["END_PAGE" ] = ENCODE(C_NAND_CTRL, 0, 1);
	m_FirstKeyword["END_page" ] = ENCODE(C_NAND_CTRL, 0, 1);
	m_FirstKeyword["end_PAGE" ] = ENCODE(C_NAND_CTRL, 0, 1);
	m_FirstKeyword["end_block"] = ENCODE(C_NAND_CTRL, 0, 2);
	m_FirstKeyword["END_BLOCK"] = ENCODE(C_NAND_CTRL, 0, 2);
	m_FirstKeyword["END_block"] = ENCODE(C_NAND_CTRL, 0, 2);
	m_FirstKeyword["end_BLOCK"] = ENCODE(C_NAND_CTRL, 0, 2);
	m_FirstKeyword["vm"       ] = ENCODE(C_NAND_CTRL, 0, 4);
	m_FirstKeyword["VM"       ] = ENCODE(C_NAND_CTRL, 0, 4);
	m_FirstKeyword["vm_inc"   ] = ENCODE(C_NAND_CTRL, 0, 8);
	m_FirstKeyword["VM_INC"   ] = ENCODE(C_NAND_CTRL, 0, 8);
	m_FirstKeyword["VM_inc"   ] = ENCODE(C_NAND_CTRL, 0, 8);
	m_FirstKeyword["vm_INC"   ] = ENCODE(C_NAND_CTRL, 0, 8);
	m_FirstKeyword["info_inc" ] = ENCODE(C_NAND_CTRL, 0, 8);
	m_FirstKeyword["INFO_INC" ] = ENCODE(C_NAND_CTRL, 0, 8);
	m_FirstKeyword["INFO_inc" ] = ENCODE(C_NAND_CTRL, 0, 8);
	m_FirstKeyword["info_INC" ] = ENCODE(C_NAND_CTRL, 0, 8);

	//m_FirstKeyword["XMAX"] = ENCODE(C_IMM, IMM_IDX::XMAX, 0xF);
	//m_FirstKeyword["YMAX"] = ENCODE(C_IMM, IMM_IDX::YMAX, 0xF);
	//m_FirstKeyword["ZMAX"] = ENCODE(C_IMM, IMM_IDX::ZMAX, 0xB);
	m_FirstKeyword["ZH"  ] = ENCODE(C_IMM, IMM_IDX::ZH,   AB_ALU::ZH_IMM);
	m_FirstKeyword["TPX1"] = ENCODE(C_TPX, IMM_IDX::TPX1, TP_ALU::TPX_IMM);
	m_FirstKeyword["TPX2"] = ENCODE(C_TPX, IMM_IDX::TPX2, TP_ALU::TPX_IMM);

	m_FirstKeyword["XH" ] = ENCODE(C_D1, 0x0, D1X_IDX::XH );
	m_FirstKeyword["D1A"] = ENCODE(C_D1, 0x0, D1X_IDX::D1A);
	m_FirstKeyword["D1B"] = ENCODE(C_D1, 0x0, D1X_IDX::D1B);
	m_FirstKeyword["D1C"] = ENCODE(C_D1, 0x0, D1X_IDX::D1C);
	m_FirstKeyword["D1D"] = ENCODE(C_D1, 0x0, D1X_IDX::D1D);
	m_FirstKeyword["D1E"] = ENCODE(C_D1, 0x0, D1X_IDX::D1E);
	m_FirstKeyword["D1F"] = ENCODE(C_D1, 0x0, D1X_IDX::D1F);
	m_FirstKeyword["D1G"] = ENCODE(C_D1, 0x0, D1X_IDX::D1G);
	m_FirstKeyword["D1H"] = ENCODE(C_D1, 0x0, D1X_IDX::D1H);
	m_FirstKeyword["XOS"] = ENCODE(C_D1, 0x0, D1X_IDX::XOS);
	m_FirstKeyword["XT" ] = ENCODE(C_D1, 0x0, D1X_IDX::XT );
	m_FirstKeyword["D3B"] = ENCODE(C_D1, 0x0, D1X_IDX::D3B);

	m_FirstKeyword["YH" ] = ENCODE(C_D2, 0x0, D2X_IDX::YH );
	m_FirstKeyword["D2A"] = ENCODE(C_D2, 0x0, D2X_IDX::D2A);
	m_FirstKeyword["D2B"] = ENCODE(C_D2, 0x0, D2X_IDX::D2B);
	m_FirstKeyword["D2C"] = ENCODE(C_D2, 0x0, D2X_IDX::D2C);
	m_FirstKeyword["D2D"] = ENCODE(C_D2, 0x0, D2X_IDX::D2D);
	m_FirstKeyword["YOS"] = ENCODE(C_D2, 0x0, D2X_IDX::YOS);
	m_FirstKeyword["YT" ] = ENCODE(C_D2, 0x0, D2X_IDX::YT );
	m_FirstKeyword["D4B"] = ENCODE(C_D2, 0x0, D2X_IDX::D4B);

}

void CPatternCompiler::CreateOpcodeKeyword()
{
	m_OpcodeKeyword.push_back("NOP");
	//m_OpcodeKeyword.push_back("FLGLI1");
	m_OpcodeKeyword.push_back("JZD");
	m_OpcodeKeyword.push_back("JET");
	m_OpcodeKeyword.push_back("RTN");
	m_OpcodeKeyword.push_back("JRB");
	m_OpcodeKeyword.push_back("STPS");
	m_OpcodeKeyword.push_back("JMP");
	m_OpcodeKeyword.push_back("JSR");
	m_OpcodeKeyword.push_back("OUT");
	//m_OpcodeKeyword.push_back("REF_START");
	//m_OpcodeKeyword.push_back("TM_START");
	m_OpcodeKeyword.push_back("TIMER0"); //SET Refresh Time
	m_OpcodeKeyword.push_back("TIMER1"); //SET Jet Time
	m_OpcodeKeyword.push_back("REPEAT");
	m_OpcodeKeyword.push_back("PAUSE");
	m_OpcodeKeyword.push_back("JNC0");
	m_OpcodeKeyword.push_back("JNC1");
	m_OpcodeKeyword.push_back("JNC2");
	m_OpcodeKeyword.push_back("JNC3");
	m_OpcodeKeyword.push_back("JNC4");
	m_OpcodeKeyword.push_back("JNC5");
	m_OpcodeKeyword.push_back("JNC6");
	m_OpcodeKeyword.push_back("JNC7");
	m_OpcodeKeyword.push_back("JNC8");
	m_OpcodeKeyword.push_back("JNC9");
	m_OpcodeKeyword.push_back("JNC10");
	m_OpcodeKeyword.push_back("JNC11");
	m_OpcodeKeyword.push_back("JNC12");
	m_OpcodeKeyword.push_back("JNC13");
	m_OpcodeKeyword.push_back("JNC14");
	m_OpcodeKeyword.push_back("JNC15");
	m_OpcodeKeyword.push_back("JNI0");
	m_OpcodeKeyword.push_back("JNI1");
	m_OpcodeKeyword.push_back("JNI2");
	m_OpcodeKeyword.push_back("JNI3");
	m_OpcodeKeyword.push_back("JNI4");
	m_OpcodeKeyword.push_back("JNI5");
	m_OpcodeKeyword.push_back("JNI6");
	m_OpcodeKeyword.push_back("JNI7");
	m_OpcodeKeyword.push_back("JNI8");
	m_OpcodeKeyword.push_back("JNI9");
	m_OpcodeKeyword.push_back("JNI10");
	m_OpcodeKeyword.push_back("JNI11");
	m_OpcodeKeyword.push_back("JNI12");
	m_OpcodeKeyword.push_back("JNI13");
	m_OpcodeKeyword.push_back("JNI14");
	m_OpcodeKeyword.push_back("JNI15");
	m_OpcodeKeyword.push_back("JNI0D");
	m_OpcodeKeyword.push_back("JNI1D");
	m_OpcodeKeyword.push_back("JNI2D");
	m_OpcodeKeyword.push_back("JNI3D");
	m_OpcodeKeyword.push_back("JNI4D");
	m_OpcodeKeyword.push_back("JNI5D");
	m_OpcodeKeyword.push_back("JNI6D");
	m_OpcodeKeyword.push_back("JNI7D");
	m_OpcodeKeyword.push_back("INC0");
	m_OpcodeKeyword.push_back("INC1");
	m_OpcodeKeyword.push_back("INC2");
	m_OpcodeKeyword.push_back("INC3");
	m_OpcodeKeyword.push_back("INC4");
	m_OpcodeKeyword.push_back("INC5");
	m_OpcodeKeyword.push_back("INC6");
	m_OpcodeKeyword.push_back("INC7");
	m_OpcodeKeyword.push_back("STI0");
	m_OpcodeKeyword.push_back("STI1");
	m_OpcodeKeyword.push_back("STI2");
	m_OpcodeKeyword.push_back("STI3");
	m_OpcodeKeyword.push_back("STI4");
	m_OpcodeKeyword.push_back("STI5");
	m_OpcodeKeyword.push_back("STI6");
	m_OpcodeKeyword.push_back("STI7");
	m_OpcodeKeyword.push_back("STI8");
	m_OpcodeKeyword.push_back("STBAR");
	m_OpcodeKeyword.push_back("STISP");
	m_OpcodeKeyword.push_back("LDI0");
	m_OpcodeKeyword.push_back("LDI1");
	m_OpcodeKeyword.push_back("LDI2");
	m_OpcodeKeyword.push_back("LDI3");
	m_OpcodeKeyword.push_back("LDI4");
	m_OpcodeKeyword.push_back("LDI5");
	m_OpcodeKeyword.push_back("LDI6");
	m_OpcodeKeyword.push_back("LDI7");
	m_OpcodeKeyword.push_back("MODE");
	m_OpcodeKeyword.push_back("VMPA");

}

void CPatternCompiler::CreateKeyword()
{
	CreateOpcodeKeyword();
	CreateFirstKeyword();
	CreateXBKeyword();
	CreateXNKeyword();
	CreateYBKeyword();
	CreateYNKeyword();
	CreateD3Keyword();
	CreateD4Keyword();
	CreateZKeyword();
	CreateXKeyword();
	CreateYKeyword();
	CreateTP1Keyword();
	CreateTP2Keyword();
	CreateD7Keyword();
	CreateD8Keyword();
	CreateSAKeyword();
	CreateSBKeyword();
	CreateSKeyword();
	CreateRFKeyword();
	CreateDXKeyword();
	CreateTPHXKeyword();
}

// return value != -1 : Pass(opt value)
// return value -1    : Fail
int CPatternCompiler::CheckGrammar(unsigned int cmd)
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

void CPatternCompiler::GenerateGrammar()
{
	//GenerateOpcodeGrammar();
	GenerateXBGrammar();
	GenerateXNGrammar();
	GenerateYBGrammar();
	GenerateYNGrammar();
	GenerateD3Grammar();
	GenerateD4Grammar();
	GenerateZGrammar();
	GenerateTP1Grammar();
	GenerateTP2Grammar();
	GenerateD7Grammar();
	GenerateD8Grammar();
	GenerateSAGrammar();
	GenerateSBGrammar();
	GenerateDXGrammar();
	GenerateTPHXGrammar();

}

//void CPatternCompiler::GenerateOpcodeGrammar()
//{
//	vector<vector<ITEM>> opcode_tree = {
//		/*0   */ { ITEM(C_ASSIGN, C_ASSIGN, 1) },
//		/*1   */ { ITEM(C_UINT,   C_UINT,   2) },
//		/*2   */ { ITEM(C_EXIT,   C_EXIT,  -1) }
//	};
//
//	m_grammar_collage["OPCODE"] = opcode_tree;
//}

void CPatternCompiler::GenerateXBGrammar()
{
	vector<vector<ITEM>> xb_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_XB,     1) },
		/*1*/ { ITEM(C_INV,      O_XB_ALU, 2), ITEM(C_RESET,    O_XB_ALU, 5) ,ITEM(C_OPERAND, O_XN_IMM, 5) ,ITEM(C_SRC_SEL, O_XB,     3), ITEM(C_XB_A,     O_XB_A,   5) },
		/*2*/ { ITEM(C_SRC_SEL,  O_XB,     5) },
		/*3*/ { ITEM(C_INCREASE, O_XB_ALU, 5), ITEM(C_DECREASE, O_XB_ALU, 5) ,ITEM(C_LSHIFT,  O_XB_ALU, 5), ITEM(C_RSHIFT,  O_XB_ALU, 5), ITEM(C_ADD, O_XB_ALU, 4), ITEM(C_SUB, O_XB_ALU, 4), ITEM(C_EXIT, C_EXIT, -1) },
		/*4*/ { ITEM(C_XB_A,     O_XB_A,   5) ,ITEM(C_OPERAND,  O_XN_IMM, 5) } ,
		/*5*/ { ITEM(C_EXIT,     C_EXIT,  -1) }
	};
	
	m_grammar_collage["XB"] = xb_tree;
}

void CPatternCompiler::GenerateXNGrammar()
{
	vector<vector<ITEM>> xn_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_XN,     1) },
		/*1*/ { ITEM(C_INV,      O_XN_ALU, 2), ITEM(C_RESET,    O_XN_ALU, 6) ,ITEM(C_OPERAND, O_XN_IMM,  6), ITEM(C_SRC_SEL, O_XN_A,  3), ITEM(C_XN_B, O_XN_B,   6) },
		/*2*/ { ITEM(C_SRC_SEL,  O_XN_A,   6) },
		/*3*/ { ITEM(C_INCREASE, O_XN_ALU, 6), ITEM(C_DECREASE, O_XN_ALU, 6) ,ITEM(C_LSHIFT,  O_XN_ALU,  6), ITEM(C_RSHIFT, O_XN_ALU, 6), ITEM(C_ADD, O_XN_ALU, 5), ITEM(C_SUB, O_XN_ALU, 5), ITEM(C_AND, O_XN_ALU, 4), ITEM(C_OR, O_XN_ALU, 4), ITEM(C_XOR, O_XN_ALU, 4), ITEM(C_EXIT, C_EXIT, -1) },
		/*4*/ { ITEM(C_XN_B,     O_XN_B,   6) },
		/*5*/ { ITEM(C_XN_B,     O_XN_B,   6), ITEM(C_OPERAND,  O_XN_IMM, 6) },
		/*6*/ { ITEM(C_EXIT,     C_EXIT,  -1) }
	};

	m_grammar_collage["XN"] = xn_tree;
}

void CPatternCompiler::GenerateYBGrammar()
{
	vector<vector<ITEM>> yb_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_YB,     1) },
		/*1*/ { ITEM(C_INV,      O_YB_ALU, 2), ITEM(C_RESET,    O_YB_ALU, 5) ,ITEM(C_OPERAND, O_YN_IMM, 5) ,ITEM(C_SRC_SEL, O_YB, 3), ITEM(C_YB_A,     O_YB_A,   5) },
		/*2*/ { ITEM(C_SRC_SEL,  O_YB,     5) },
		/*3*/ { ITEM(C_INCREASE, O_YB_ALU, 5), ITEM(C_DECREASE, O_YB_ALU, 5) ,ITEM(C_LSHIFT,  O_YB_ALU, 5), ITEM(C_RSHIFT, O_YB_ALU, 5), ITEM(C_ADD, O_YB_ALU, 4), ITEM(C_SUB, O_YB_ALU, 4), ITEM(C_EXIT, C_EXIT, -1) },
		/*4*/ { ITEM(C_YB_A,     O_YB_A,   5) ,ITEM(C_OPERAND,  O_YN_IMM, 5) },
		/*5*/ { ITEM(C_EXIT,     C_EXIT,  -1) }
	};

	m_grammar_collage["YB"] = yb_tree;
}

void CPatternCompiler::GenerateYNGrammar()
{
	vector<vector<ITEM>> yn_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_YN,     1) },
		/*1*/ { ITEM(C_INV,      O_YN_ALU, 2), ITEM(C_RESET,    O_YN_ALU, 6) ,ITEM(C_OPERAND, O_YN_IMM,  6), ITEM(C_SRC_SEL, O_YN_A, 3), ITEM(C_YN_B, O_YN_B,   6) },
		/*2*/ { ITEM(C_SRC_SEL,  O_YN_A,   6) },
		/*3*/ { ITEM(C_INCREASE, O_YN_ALU, 6), ITEM(C_DECREASE, O_YN_ALU, 6) ,ITEM(C_LSHIFT,  O_YN_ALU,  6), ITEM(C_RSHIFT, O_YN_ALU, 6), ITEM(C_ADD, O_YN_ALU, 5), ITEM(C_SUB, O_YN_ALU, 5), ITEM(C_AND, O_YN_ALU, 4), ITEM(C_OR, O_YN_ALU, 4), ITEM(C_XOR, O_YN_ALU, 4), ITEM(C_EXIT, C_EXIT, -1) },
		/*4*/ { ITEM(C_YN_B,     O_YN_B,   6) },
		/*5*/ { ITEM(C_YN_B,     O_YN_B,   6), ITEM(C_OPERAND,  O_YN_IMM, 6) },
		/*6*/ { ITEM(C_EXIT,     C_EXIT,  -1) }
	};

	m_grammar_collage["YN"] = yn_tree;
}

void CPatternCompiler::GenerateD3Grammar()
{
	vector<vector<ITEM>> d3_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_D3,   1) },
		/*1*/ { ITEM(C_RESET,    O_D3,   4), ITEM(C_SRC_SEL,  O_D3, 2), ITEM(C_SRC_BASE, O_D3, 3) },
		/*2*/ { ITEM(C_INCREASE, O_D3,   4), ITEM(C_DECREASE, O_D3, 4), ITEM(C_LSHIFT,   O_D3, 4), ITEM(C_RSHIFT, O_D3, 4), ITEM(C_EXIT, C_EXIT,-1) },
		/*3*/ { ITEM(C_RSHIFT,   O_D3,   4), ITEM(C_EXIT, C_EXIT,  -1) },
		/*4*/ { ITEM(C_EXIT,     C_EXIT,-1) }
	};

	m_grammar_collage["D3"] = d3_tree;
}

void CPatternCompiler::GenerateD4Grammar()
{
	vector<vector<ITEM>> d4_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_D4,   1) },
		/*1*/ { ITEM(C_RESET,    O_D4,   4), ITEM(C_SRC_SEL,  O_D4, 2), ITEM(C_SRC_BASE, O_D4, 3) },
		/*2*/ { ITEM(C_INCREASE, O_D4,   4), ITEM(C_DECREASE, O_D4, 4), ITEM(C_LSHIFT,   O_D4, 4), ITEM(C_RSHIFT, O_D4, 4), ITEM(C_EXIT, C_EXIT,-1) },
		/*3*/ { ITEM(C_RSHIFT,   O_D4,   4), ITEM(C_EXIT, C_EXIT,  -1) },
		/*4*/ { ITEM(C_EXIT,     C_EXIT,-1) }
	};

	m_grammar_collage["D4"] = d4_tree;
}

void CPatternCompiler::GenerateZGrammar()
{
	vector<vector<ITEM>> z_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_Z,     1) },
		/*1*/ { ITEM(C_INV,      O_Z_ALU, 2), ITEM(C_RESET,    O_Z_ALU, 5) ,ITEM(C_OPERAND, O_Z_IMM, 5) ,ITEM(C_SRC_SEL, O_Z, 3), ITEM(C_Z_A,      O_Z_A,   5) },
		/*2*/ { ITEM(C_SRC_SEL,  O_Z,     5) },
		/*3*/ { ITEM(C_INCREASE, O_Z_ALU, 5), ITEM(C_DECREASE, O_Z_ALU, 5) ,ITEM(C_LSHIFT,  O_Z_ALU, 5), ITEM(C_RSHIFT, O_Z_ALU, 5), ITEM(C_ADD, O_Z_ALU, 4), ITEM(C_SUB, O_Z_ALU, 4), ITEM(C_EXIT, C_EXIT, -1) },
		/*4*/ { ITEM(C_Z_A,      O_Z_A,   5), ITEM(C_OPERAND,  O_Z_IMM, 5) } ,
		/*5*/ { ITEM(C_EXIT,     C_EXIT,  -1) }
	};

	m_grammar_collage["Z"] = z_tree;
}

void CPatternCompiler::GenerateTP1Grammar()
{
	//vector<vector<ITEM>> tp1_tree = {
	//	/*0*/ { ITEM(C_ASSIGN,   O_TP1_ALU, 1) },
	//	/*1*/ { ITEM(C_INV,      O_TP1_ALU, 2), ITEM(C_OPERAND,  O_TP1_ALU, 5), ITEM(C_SRC_SEL, O_TP1_ALU, 3), ITEM(C_OTH,  O_TP1_A,  5) },
	//	/*2*/ { ITEM(C_SRC_SEL,  O_TP1_ALU, 5), ITEM(C_OTH,      O_TP1_A,   5) },
	//	/*3*/ { ITEM(C_INCREASE, O_TP1_ALU, 5), ITEM(C_DECREASE, O_TP1_ALU, 5) ,ITEM(C_LSHIFT,  O_TP1_ALU, 5), ITEM(C_RSHIFT, O_TP1_ALU, 5), ITEM(C_ADD, O_TP1_ALU, 4), ITEM(C_SUB, O_TP1_ALU, 4), ITEM(C_AND, O_TP1_ALU, 4), ITEM(C_OR, O_TP1_ALU, 4), ITEM(C_EXIT, C_EXIT, -1) },
	//	/*4*/ { ITEM(C_OTH,      O_TP1_A,   5) },
	//	/*5*/ { ITEM(C_EXIT,     C_EXIT,  -1) }
	//};

	vector<vector<ITEM>> tp1_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_TP1_ALU, 1) },
		/*1*/ { ITEM(C_INV,      O_TP1_ALU, 2), ITEM(C_OPERAND,  O_TP1_ALU, 5), ITEM(C_SRC_SEL, O_TP1_ALU, 3), ITEM(C_OTH,  O_TP1_A,  5) },
		/*2*/ { ITEM(C_SRC_SEL,  O_TP1_ALU, 5), ITEM(C_OTH,      O_TP1_A,   5), ITEM(C_OPERAND, O_TP1_ALU, 5)},
		/*3*/ { ITEM(C_INCREASE, O_TP1_ALU, 5), ITEM(C_DECREASE, O_TP1_ALU, 5) ,ITEM(C_LSHIFT,  O_TP1_ALU, 5), ITEM(C_RSHIFT, O_TP1_ALU, 5), ITEM(C_ADD, O_TP1_ALU, 4), ITEM(C_SUB, O_TP1_ALU, 4), ITEM(C_AND, O_TP1_ALU, 4), ITEM(C_OR, O_TP1_ALU, 4), ITEM(C_EXIT, C_EXIT, -1) },
		/*4*/ { ITEM(C_OTH,      O_TP1_A,   5), ITEM(C_OPERAND,  O_TP1_ALU, 5) },
		/*5*/ { ITEM(C_EXIT,     C_EXIT,  -1) }
	};

	m_grammar_collage["TP1"] = tp1_tree;
}

void CPatternCompiler::GenerateTP2Grammar()
{
	vector<vector<ITEM>> tp2_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_TP2_ALU, 1) },
		/*1*/ { ITEM(C_INV,      O_TP2_ALU, 2), ITEM(C_OPERAND,  O_TP2_ALU, 5), ITEM(C_SRC_SEL, O_TP2_ALU, 3), ITEM(C_OTH,    O_TP2_A,   5) },
		/*2*/ { ITEM(C_SRC_SEL,  O_TP2_ALU, 5), ITEM(C_OTH,      O_TP2_A,   5), ITEM(C_OPERAND, O_TP2_ALU, 5) },
		/*3*/ { ITEM(C_INCREASE, O_TP2_ALU, 5), ITEM(C_DECREASE, O_TP2_ALU, 5), ITEM(C_LSHIFT,  O_TP2_ALU, 5), ITEM(C_RSHIFT, O_TP2_ALU, 5), ITEM(C_ADD, O_TP2_ALU, 4), ITEM(C_SUB, O_TP2_ALU, 4), ITEM(C_AND, O_TP2_ALU, 4), ITEM(C_OR, O_TP2_ALU, 4), ITEM(C_EXIT, C_EXIT, -1) },
		/*4*/ { ITEM(C_OTH,      O_TP2_A,   5), ITEM(C_OPERAND,  O_TP2_ALU, 5) },
		/*5*/ { ITEM(C_EXIT,     C_EXIT,   -1) }
	};

	m_grammar_collage["TP2"] = tp2_tree;
}

void CPatternCompiler::GenerateD7Grammar()
{
	vector<vector<ITEM>> D7_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_D7, 1) },
		/*1*/ { ITEM(C_SRC_SEL,  O_D7, 2), ITEM(C_RESET,    O_D7, 4), ITEM(C_OTH,    O_D7_A, 4), ITEM(C_IMM, O_D7_A, 4)},
		/*2*/ { ITEM(C_INCREASE, O_D7, 4), ITEM(C_DECREASE, O_D7, 4), ITEM(C_RSHIFT, O_D7,   4), ITEM(C_ADD, O_D7, 3), ITEM(C_SUB, O_D7, 3), ITEM(C_EXIT, C_EXIT, -1) },
		/*3*/ { ITEM(C_OTH,      O_D7_A,   4) },
		/*4*/ { ITEM(C_EXIT,     C_EXIT,  -1) }
	};

	m_grammar_collage["D7"] = D7_tree;
}

void CPatternCompiler::GenerateD8Grammar()
{
	vector<vector<ITEM>> D8_tree = {
		/*0*/ { ITEM(C_ASSIGN,   O_D8, 1) },
		/*1*/ { ITEM(C_SRC_SEL,  O_D8, 2), ITEM(C_RESET,    O_D8, 4), ITEM(C_OTH,    O_D8_A, 4), ITEM(C_IMM, O_D8_A, 4) },
		/*2*/ { ITEM(C_INCREASE, O_D8, 4), ITEM(C_DECREASE, O_D8, 4), ITEM(C_RSHIFT, O_D8,   4), ITEM(C_ADD, O_D8, 3), ITEM(C_SUB, O_D8, 3), ITEM(C_EXIT, C_EXIT, -1) },
		/*3*/ { ITEM(C_OTH,      O_D8_A,   4) },
		/*4*/ { ITEM(C_EXIT,     C_EXIT,  -1) }
	};

	m_grammar_collage["D8"] = D8_tree;
}

void CPatternCompiler::GenerateSAGrammar()
{
	vector<vector<ITEM>> SA_tree = {
		/*0*/ { ITEM(C_ASSIGN,  O_SA, 1) },
		/*1*/ { ITEM(C_SRC_SEL, O_SA, 2), ITEM(C_SB,  O_SA, 2), ITEM(C_RESET,    O_SA, 4), ITEM(C_OTH,      O_SA, 4), ITEM(C_OPERAND, O_SA,  4) },
		/*2*/ { ITEM(C_ADD,     O_SA, 3), ITEM(C_SUB, O_SA, 3), ITEM(C_INCREASE, O_SA, 4), ITEM(C_DECREASE, O_SA, 4), ITEM(C_EXIT,  C_EXIT, -1) },
		/*3*/ { ITEM(C_OPERAND, O_SA, 4) },
		/*4*/ { ITEM(C_EXIT,     C_EXIT,  -1) }
	};

	m_grammar_collage["SA"] = SA_tree;
}

void CPatternCompiler::GenerateSBGrammar()
{
	vector<vector<ITEM>> SB_tree = {
		/*0*/ { ITEM(C_ASSIGN,  O_SB, 1) },
		/*1*/ { ITEM(C_SRC_SEL, O_SB, 2), ITEM(C_SA,  O_SB, 2), ITEM(C_RESET,    O_SB, 4), ITEM(C_OTH,      O_SB, 4), ITEM(C_OPERAND, O_SB,  4) },
		/*2*/ { ITEM(C_ADD,     O_SB, 3), ITEM(C_SUB, O_SB, 3), ITEM(C_INCREASE, O_SB, 4), ITEM(C_DECREASE, O_SB, 4), ITEM(C_EXIT,  C_EXIT, -1) },
		/*3*/ { ITEM(C_OPERAND, O_SB, 4) },
		/*4*/ { ITEM(C_EXIT,    C_EXIT,  -1) }
	};

	m_grammar_collage["SB"] = SB_tree;
}

void CPatternCompiler::GenerateDXGrammar()
{
	vector<vector<ITEM>> DX_tree = {
		/*0*/ { ITEM(C_ASSIGN,  O_DX, 1) },
		/*1*/ { ITEM(C_OPERAND, O_DX, 2) },
		/*2*/ { ITEM(C_EXIT,    C_EXIT,  -1) }
	};

	m_grammar_collage["DX"] = DX_tree;
}

void CPatternCompiler::GenerateTPHXGrammar()
{
	vector<vector<ITEM>> TPH1X_tree = {
		/*0*/ { ITEM(C_ASSIGN,  O_TPH1X, 1) },
		/*1*/ { ITEM(C_OPERAND, O_TPH1X, 2), ITEM(C_TP1, C_TP1, 2) },
		/*2*/ { ITEM(C_EXIT,    C_EXIT, -1) }
	};

	m_grammar_collage["TPH1X"] = TPH1X_tree;

	vector<vector<ITEM>> TPH2X_tree = {
		/*0*/ { ITEM(C_ASSIGN,  O_TPH2X, 1) },
		/*1*/ { ITEM(C_OPERAND, O_TPH2X, 2), ITEM(C_TP2, C_TP2, 2) },
		/*2*/ { ITEM(C_EXIT,    C_EXIT, -1) }
	};

	m_grammar_collage["TPH2X"] = TPH2X_tree;
}

int CPatternCompiler::OpcodeComp(int nOPType)
{
	string strName;
	string strkey;

	int nType = 0;
	int nData;
	float fData;
	char* endptr;

	if (m_Used.bOpcode == true) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Opcode already setted.");
		return -1;
	}

	m_Used.bOpcode = true;

	string fullname = m_strName + '.';

	if (m_PrevOpcode != -1) {
		if (m_inst.op_command.opcode == PAT_OP::PAUSE && m_PrevOpcode == PAT_OP::PAUSE) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "PAUSE opcod cannot be used for consecutive lines.");
			return -1;
		}
	}

	m_PrevOpcode = m_inst.op_command.opcode;

	if (nOPType == C_LONE_OP) {
		return 0;
	}

	if (nOPType == C_JUMP_OP ) {
		// 라벨 or target pc 얻어온다.
		strkey = GetName(nType);
		switch (nType) {
			case KEY_STR: 
				strName = strkey;
				if (strName.length() == 0) {
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Target label name is nothing.");
					return -1;
				}

				if(m_inst.op_command.opcode == JSR)
					fullname = strName;
				else
					fullname += strName;

				//m_LabelJumpPC.push_back(make_pair(m_pc, strName));
				m_LabelJumpPC.push_back(make_pair(m_pc, fullname));
				break;
			case KEY_INT: 
				nData = strtoul(strkey.c_str(), &endptr, 10); 
				m_inst.op_command.operand = nData;
				break;
			case KEY_XINT:
				nData = strtoul(strkey.c_str(), &endptr, 16);
				m_inst.op_command.operand = nData;
				break;
			default:
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strkey, "The  has not been set.");
				return -1;
		}
	}
	else if (nOPType == C_SET_OP_TIMER) {  /// TIMER0 TIMER1
		strkey = GetKey(nType);
		if (nType & KEY_INT) {
			fData = static_cast<float>(strtoul(strkey.c_str(), &endptr, 10));
		}
		else if (nType & KEY_XINT) {
			fData = static_cast<float>(strtoul(strkey.c_str(), &endptr, 16));
		}
		else if (nType & KEY_FLT) {
			fData = static_cast<float>(strtod(strkey.c_str(), &endptr));
		}
		else {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strkey : m_strOrgKey, "Timer value is not set.");
			return -1;
		}

		int nTemp = (int)m_pos;
		strkey = GetKey(nType);

		// 단위가 붙은경우
		if (strkey == "S" || strkey == "s") {

		}else if (strkey == "MS" || strkey == "ms")
			fData /= 1000;
		else if (strkey == "US" || strkey == "us")
			fData /= 1000000;
		else if (strkey == "NS" || strkey == "ns")
			fData /= 1000000000;
		else // 단위가 없는 경우 원래 위치로 돌려 놓는다.
			m_pos = nTemp;

		if (fData > m_RefTimeDiv_2) { // sec
			nData = static_cast<unsigned int>(fData / (TIMER_RES * 1000000000));
			m_inst.op_command.operand = nData | (0x3 << 22);
		}
		else if (fData > m_RefTimeDiv_1) { // milli-sec
			nData = static_cast<unsigned int>(fData / (TIMER_RES * 1000000));
			m_inst.op_command.operand = nData | (0x2 << 22);
		}
		else if (fData > m_RefTimeDiv_0) { // micro-sec
			nData = static_cast<unsigned int>(fData / (TIMER_RES * 1000));
			m_inst.op_command.operand = nData | (0x1 << 22);
		}
		else {
			nData = static_cast<unsigned int>(fData / TIMER_RES);
			m_inst.op_command.operand = nData;
		}
	}
	else if (nOPType == C_SET_OP_STI) {
		strkey = GetKey(nType);
		int radix = 10;
		if (nType & KEY_INT) radix = 10;
		else if (nType & KEY_XINT) radix = 16;
		else {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strkey : m_strOrgKey, "Invalide syntax.");
			return -1;
		}
		nData = strtoul(strkey.c_str(), &endptr, radix);
		m_inst.op_command.operand = (nData + 1);
	}
	else if (nOPType == C_SET_OP) { //// STBAR, STISP, LDIn
		strkey = GetKey(nType);
		if (nType & KEY_INT) {
			nData = strtoul(strkey.c_str(), &endptr, 10);
			m_inst.op_command.operand = nData;
		}
		else if (nType & KEY_XINT) {
			nData = strtoul(strkey.c_str(), &endptr, 16);
			m_inst.op_command.operand = nData;
		}
		//else if (nType & KEY_FLT) {
		//	fData = static_cast<float>(strtod(strkey.c_str(), &endptr));
		//	int nTemp = (int)m_pos;
		//
		//	strkey = GetKey(nType);
		//
		//	// 단위가 붙은경우
		//	if (strkey == "MS" || strkey == "ms")
		//		fData /= 1000;
		//	else if (strkey == "US" || strkey == "us")
		//		fData /= 1000000;
		//	else if (strkey == "NS" || strkey == "ns")
		//		fData /= 1000000000;
		//	else // 단위가 없는 경우 원래 위치로 돌려 놓는다.
		//		m_pos = nTemp;
		//
		//	if (fData > m_RefTimeDiv_1) { // milli-sec
		//		nData = static_cast<unsigned int>(fData / (TIMER_RES * 1000000));
		//		m_inst.op_command.operand = nData | (0x2<<22);
		//	}
		//	else if (fData > m_RefTimeDiv_0) { // micro-sec
		//		nData = static_cast<unsigned int>(fData / (TIMER_RES * 1000));
		//		m_inst.op_command.operand = nData | (0x1 << 22);
		//	}
		//	else {
		//		nData = static_cast<unsigned int>(fData / TIMER_RES);
		//		m_inst.op_command.operand = nData | (0x1 << 22);
		//	}
		//}
		else {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strkey : m_strOrgKey, "Invalide syntax.");
			return -1;
		}

	}
	else if (nOPType == C_OPRN_OP) {
		strkey = GetKey(nType);
		if (nType == KEY_INT) {
			nData = strtoul(strkey.c_str(), &endptr, 10);
		}
		else if (nType == KEY_XINT) {
			nData = strtoul(strkey.c_str(), &endptr, 16);
		}
		else {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strkey : m_strOrgKey, "Incorrect syntax.");
			return -1;
		}
		m_inst.op_command.operand = nData;
	}
	else if (nOPType == C_MODE_OP) {
		int count = 0;
		while (1) {
			strkey = GetName(nType);;
			if (strkey == "VM_ON") {
				m_nMode |= 0x2;
			}
			else if (strkey == "VM_OFF") {
				m_nMode &= (~0x2);
			}else if (strkey == "INFO_ON") {
				m_nMode |= 0x1;
			}
			else if (strkey == "INFO_OFF") {
				m_nMode &= (~0x1);
			}
			else {
				if (count == 0) {
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strkey, "Incorrect syntax.");
					return -1;
				}
				else {
					if (nType != KEY_NULL) {
						m_BufKey.push_back(strkey);
					}
					break;
				}
			}
			m_inst.op_command.operand = m_nMode;
			count++;
		}
	}
	else {
		//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strkey, "Invalide syntax.");
		return -1;
	}

	return 0;
}

int CPatternCompiler::XBComp()
{
	m_Index = 0;
	string strKey;

	int nData = -1;
	char* endptr;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["XB"];

	m_pgrtree = &m_grammar_collage["XB"];

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		ITEM item;

		if (it == keymap.end()) {
			if (KeyType == KEY_INT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 10);
			}
			else if (KeyType == KEY_XINT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 16);
			}
			else {
				return ExitCheck(strKey);
				//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
				//return -1;
			}
		}
		else
			item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		//case C_ASSIGN:
		//	if (CheckGrammar(item.cmd) == -1) {
		//		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
		//		return -1;
		//	}
		//	break;
		//case C_SRC_SEL:
		//	if (CheckGrammar(item.cmd) == -1) {
		//		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
		//		return -1;
		//	}
		//	break;
		case C_RESET:
		case C_INV:
		case C_INCREASE:
		case C_DECREASE:
		case C_LSHIFT:
		case C_RSHIFT:
		case C_ADD:
		case C_SUB:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.xb.alu = item.bit;
			//m_inst.xb.alu = item.cmd - C_SRC_SEL;
			break;
		case C_OPERAND:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.xb.alu == 0)
				m_inst.xb.alu = AB_ALU::IMM;
			else if (m_inst.xb.alu == AB_ALU::ADD)
				m_inst.xb.alu = AB_ALU::ADD_IMM;
			else if (m_inst.xb.alu == AB_ALU::SUB)
				m_inst.xb.alu = AB_ALU::SUB_IMM;

			m_inst.xn.imm = nData;
			break;
		case C_XB_A:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if(m_inst.xb.alu == 0 )
				m_inst.xb.alu = AB_ALU::ASSIGN_A;
			m_inst.xb.a = item.bit;
			break;
		default:
			return ExitCheck(strKey);
			//if (CheckGrammar(C_EXIT) == -1) {
			//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "");
			//	return -1;
			//}
		}
	}
	return ExitCheck();
}

int CPatternCompiler::XNComp()
{
	m_Index = 0;
	string strKey;

	int nData = -1;
	char* endptr;


	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["XN"];

	m_pgrtree = &m_grammar_collage["XN"];

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);

		ITEM item;

		if (it == keymap.end()) {
			if (KeyType == KEY_INT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 10);
			}
			else if (KeyType == KEY_XINT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 16);
			}
			else {
				return ExitCheck(strKey);
				//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
				//return -1;
			}
		}
		else
			item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.xn.a = item.bit;
			break;
		case C_RESET:
		case C_INV:
		case C_INCREASE:
		case C_DECREASE:
		case C_LSHIFT:
		case C_RSHIFT:
		case C_ADD:
		case C_SUB:
		case C_AND:
		case C_OR:
		case C_XOR:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.xn.alu = item.bit;
			//m_inst.xn.alu = item.cmd - C_SRC_SEL;
			break;
		case C_OPERAND:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.xn.alu == 0)
				m_inst.xn.alu = AN_ALU::IMM;
			else if(m_inst.xn.alu == AN_ALU::ADD)
				m_inst.xn.alu = AN_ALU::ADD_IMM;
			else if (m_inst.xn.alu == AN_ALU::SUB)
				m_inst.xn.alu = AN_ALU::SUB_IMM;
			m_inst.xn.imm = nData;
			break;
		case C_XN_B:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.xn.alu == 0)
				m_inst.xn.alu = AN_ALU::ASSIGN_B;
			m_inst.xn.b = item.bit;
			break;
		default:
			return ExitCheck(strKey);
			//if (CheckGrammar(C_EXIT) == -1) {
			//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "");
			//	return -1;
			//}
		}
	}
	return ExitCheck();
}

int CPatternCompiler::YBComp()
{
	m_Index = 0;
	string strKey;

	int nData = -1;
	char* endptr;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["YB"];

	m_pgrtree = &m_grammar_collage["YB"];

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		ITEM item;

		if (it == keymap.end()) {
			if (KeyType == KEY_INT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 10);
			}
			else if (KeyType == KEY_XINT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 16);
			}
			else {
				return ExitCheck(strKey);
				//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
				//return -1;
			}
		}
		else
			item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_RESET:
		case C_INV:
		case C_INCREASE:
		case C_DECREASE:
		case C_LSHIFT:
		case C_RSHIFT:
		case C_ADD:
		case C_SUB:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.yb.alu = item.bit;
			//m_inst.yb.alu = item.cmd - C_SRC_SEL;
			break;
		case C_OPERAND:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.yb.alu == 0)
				m_inst.yb.alu = AB_ALU::IMM;
			else if (m_inst.yb.alu == AB_ALU::ADD)
				m_inst.yb.alu = AB_ALU::ADD_IMM;
			else if(m_inst.yb.alu == AB_ALU::SUB)
				m_inst.yb.alu = AB_ALU::SUB_IMM;

			m_inst.yn.imm = nData;
			break;
		case C_YB_A:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.yb.alu == 0)
				m_inst.yb.alu = AB_ALU::ASSIGN_A;
			m_inst.yb.a = item.bit;
			break;
		default:
			return ExitCheck(strKey);
			//if (CheckGrammar(C_EXIT) == -1) {
			//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "");
			//	return -1;
			//}
		}
	}
	return ExitCheck();
}

int CPatternCompiler::YNComp()
{
	m_Index = 0;
	string strKey;

	int nData = -1;
	char* endptr;


	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["YN"];

	m_pgrtree = &m_grammar_collage["YN"];

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);

		ITEM item;

		if (it == keymap.end()) {
			if (KeyType == KEY_INT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 10);
			}
			else if (KeyType == KEY_XINT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 16);
			}
			else {
				return ExitCheck(strKey);
				//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
				//return -1;
			}
		}
		else
			item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.yn.a = item.bit;
			break;
		case C_RESET:
		case C_INV:
		case C_INCREASE:
		case C_DECREASE:
		case C_LSHIFT:
		case C_RSHIFT:
		case C_ADD:
		case C_SUB:
		case C_AND:
		case C_OR:
		case C_XOR:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.yn.alu = item.bit;
			//m_inst.yn.alu = item.cmd - C_SRC_SEL;
			break;
		case C_OPERAND:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.yn.alu == 0)
				m_inst.yn.alu = AN_ALU::IMM;
			else if (m_inst.yn.alu == AN_ALU::ADD)
				m_inst.yn.alu = AN_ALU::ADD_IMM;
			else if (m_inst.yn.alu == AN_ALU::SUB)
				m_inst.yn.alu = AN_ALU::SUB_IMM;
			m_inst.yn.imm = nData;
			break;
		case C_YN_B:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.yn.alu == 0)
				m_inst.yn.alu = AN_ALU::ASSIGN_B;
			m_inst.yn.b = item.bit;
			break;
		default:
			return ExitCheck(strKey);
			//if (CheckGrammar(C_EXIT) == -1) {
			//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "");
			//	return -1;
			//}
		}
	}
	return ExitCheck();
}

int CPatternCompiler::D3Comp()
{
	m_Index = 0;
	string strKey;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["D3"];

	m_pgrtree = &m_grammar_collage["D3"];

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		
		if (it == keymap.end()) {
			return ExitCheck(strKey);
			//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
			//return -1;
		}

		ITEM item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_SRC_BASE:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.d34.d3 = item.bit;
			break;
		case C_RESET:
		case C_INCREASE:
		case C_DECREASE:
		case C_LSHIFT:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.d34.d3 = item.bit;
			//m_inst.d34.d3 = item.cmd - C_SRC_SEL;
			break;
		case C_RSHIFT:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.d34.d3 == 0)
				m_inst.d34.d3 = item.bit;
			else
				m_inst.d34.d3 = item.bit + 1; // "D3B / 2"

			break;
		default:
			return ExitCheck(strKey);
			//if (CheckGrammar(C_EXIT) == -1) {
			//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "");
			//	return -1;
			//}
		}
	}
	return ExitCheck();
}

int CPatternCompiler::D4Comp()
{
	m_Index = 0;
	string strKey;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["D4"];

	m_pgrtree = &m_grammar_collage["D4"];

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);

		if (it == keymap.end()) {
			return ExitCheck(strKey);
			//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
			//return -1;
		}

		ITEM item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_SRC_BASE:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.d34.d4 = item.bit;
			break;
		case C_RESET:
		case C_INCREASE:
		case C_DECREASE:
		case C_LSHIFT:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.d34.d4 = item.bit;
			//m_inst.d34.d4 = item.cmd - C_SRC_SEL;
			break;
		case C_RSHIFT:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.d34.d4 == 0)
				m_inst.d34.d4 = item.bit;
			else
				m_inst.d34.d4 = item.bit + 1; // "D4B / 2"

			break;
		default:
			return ExitCheck(strKey);
			//if (CheckGrammar(C_EXIT) == -1) {
			//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "");
			//	return -1;
			//}
		}
	}
	return ExitCheck();
}

int CPatternCompiler::ZComp()
{
	m_Index = 0;
	string strKey;

	int nData = -1;
	char* endptr;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["Z"];

	m_pgrtree = &m_grammar_collage["Z"];

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		ITEM item;

		if (it == keymap.end()) {
			if (KeyType == KEY_INT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 10);
			}
			else if (KeyType == KEY_XINT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 16);
			}
			else {
				return ExitCheck(strKey);
				//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
				//return -1;
			}
		}
		else
			item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_RESET:
		case C_INV:
		case C_INCREASE:
		case C_DECREASE:
		case C_LSHIFT:
		case C_RSHIFT:
		case C_ADD:
		case C_SUB:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.z.alu = item.bit;
			//m_inst.z.alu = item.cmd - C_SRC_SEL;
			break;
		case C_OPERAND:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.z.alu == 0) {
				m_inst.z.alu = AB_ALU::ASSIGN_A;//// Z=zimm
			}
			m_inst.z.a = 1;

			
			//if (nData > 0xFF) {
			if (nData > 0xFFFF) {
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Z range is over.(0xFFFF)");
				return -1;
			}
			m_inst.z.imm = nData;
			break;
		case C_Z_A:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.z.alu == 0)
				m_inst.z.alu = AB_ALU::ASSIGN_A;
			m_inst.z.a = item.bit;
			break;
		default:
			return ExitCheck(strKey);
			//if (CheckGrammar(C_EXIT) == -1) {
			//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "");
			//	return -1;
			//}
		}
	}
	
	return ExitCheck(strKey);
}

int CPatternCompiler::XComp()
{
	m_Index = 0;
	string strKey;

	int nData;
	char* endptr;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["X"];

	if (m_Used.bXdest == true) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "X out already setted.");
		return -1;
	}
	m_Used.bXdest = true;

	/// <,= 확인
	strKey = GetKey(KeyType);
	it = keymap.find(strKey);
	ITEM item;
	if (it != keymap.end()) {
		item = it->second;
		if (item.cmd != C_ASSIGN) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
			return -1;
		}
	}
	else {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
		return -1;
	}

	/// XC, XS, .... RF
	strKey = GetKey(KeyType);
	it = keymap.find(strKey);
	if (it == keymap.end()) {
		if (KeyType == KEY_INT) {
			nData = strtol(strKey.c_str(), &endptr, 10);
		}
		else if (KeyType == KEY_XINT) {
			nData = strtol(strKey.c_str(), &endptr, 16);
		}
		else {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
			return -1;
		}
		m_inst.xy.x = X_OUT::IMMX;
		m_inst.xn.imm = nData;
	}
	else {
		item = it->second;
		m_inst.xy.x = item.bit;
	}

	return 0;
}

int CPatternCompiler::YComp()
{
	m_Index = 0;
	string strKey;

	int nData;
	char* endptr;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["Y"];

	if (m_Used.bYdest == true) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Y out already setted.");
		return -1;
	}
	m_Used.bYdest = true;

	/// <,= 확인
	strKey = GetKey(KeyType);
	it = keymap.find(strKey);
	ITEM item;
	if (it != keymap.end()) {
		item = it->second;
		if (item.cmd != C_ASSIGN) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
			return -1;
		}
	}
	else {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
		return -1;
	}

	/// YC, YS, .... Z
	strKey = GetKey(KeyType);
	it = keymap.find(strKey);
	if (it == keymap.end()) {
		if (KeyType == KEY_INT) {
			nData = strtol(strKey.c_str(), &endptr, 10);
		}
		else if (KeyType == KEY_XINT) {
			nData = strtol(strKey.c_str(), &endptr, 16);
		}
		else {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
			return -1;
		}
		m_inst.xy.y = Y_OUT::IMMY;
		m_inst.yn.imm = nData;
	}
	else {
		item = it->second;
		m_inst.xy.y = item.bit;
	}

	return 0;
}

int CPatternCompiler::TP1Comp()
{
	m_Index = 0;
	string strKey;

	int nData = -1;
	char* endptr;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["TP1"];

	m_pgrtree = &m_grammar_collage["TP1"];

	if (m_Used.bTP1) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "TP1 is already set.(TP1, TPH1, D5(IMM), and D7(IMM) are set in the same register.)");
		return -1;
	}

	m_Used.bTP1 = true;

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		ITEM item;

		if (it == keymap.end()) {
			if (KeyType == KEY_INT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 10);
			}
			else if (KeyType == KEY_XINT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 16);
			}
			else {
				return ExitCheck(strKey);
				//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
				//return -1;
			}
		}
		else
			item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_INV:
		case C_INCREASE:
		case C_DECREASE:
		case C_LSHIFT:
		case C_RSHIFT:
		case C_ADD:
		case C_SUB:
		case C_AND:
		case C_OR:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.tp1.alu = item.bit;
			break;
		case C_OPERAND:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.tp1.alu == 0)
				m_inst.tp1.alu = TP_ALU::ASSIGN_B;
			else if (m_inst.tp1.alu == TP_ALU::INVERT)
				m_inst.tp1.alu = TP_ALU::INV_ASSIGN_B;
			m_inst.tp1.a = TP1_A::IMM;
			m_inst.tp1.imm = nData;
			break;
		case C_OTH:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.tp1.alu == 0)
				m_inst.tp1.alu = TP_ALU::ASSIGN_B;
			else if (m_inst.tp1.alu == TP_ALU::INVERT)
				m_inst.tp1.alu = TP_ALU::INV_ASSIGN_B;
			m_inst.tp1.a = item.bit;
			break;
		default:
			return ExitCheck(strKey);
			//if (CheckGrammar(C_EXIT) == -1) {
			//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "");
			//	return -1;
			//}
		}
	}
	return ExitCheck();
}

int CPatternCompiler::TP2Comp()
{
	m_Index = 0;
	string strKey;

	int nData = -1;
	char* endptr;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["TP2"];

	m_pgrtree = &m_grammar_collage["TP2"];

	if (m_Used.bTP2) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "TP2 is already set.(TP2, TPH2, D6(IMM), and D8(IMM) are set in the same register.)");
		return -1;
	}
	m_Used.bTP2 = true;

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		ITEM item;

		if (it == keymap.end()) {
			if (KeyType == KEY_INT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 10);
			}
			else if (KeyType == KEY_XINT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 16);
			}
			else {
				return ExitCheck(strKey);
				//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide syntax.");
				//return -1;
			}
		}
		else
			item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_INV:
		case C_INCREASE:
		case C_DECREASE:
		case C_LSHIFT:
		case C_RSHIFT:
		case C_ADD:
		case C_SUB:
		case C_AND:
		case C_OR:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.tp2.alu = item.bit;
			break;
		case C_OPERAND:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			//m_inst.tp2.alu = TP_ALU::TP2IMM;
			if (m_inst.tp2.alu == 0)
				m_inst.tp2.alu = TP_ALU::ASSIGN_B;
			else if (m_inst.tp2.alu == TP_ALU::INVERT)
				m_inst.tp2.alu = TP_ALU::INV_ASSIGN_B;
			m_inst.tp2.a = TP2_A::IMM;
			m_inst.tp2.imm = nData;
			break;
		case C_OTH:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.tp2.alu == 0)
				m_inst.tp2.alu = TP_ALU::ASSIGN_B;
			else if (m_inst.tp2.alu == TP_ALU::INVERT)
				m_inst.tp2.alu = TP_ALU::INV_ASSIGN_B;
			m_inst.tp2.a = item.bit;
			break;
		default:
			return ExitCheck(strKey);
			//if (CheckGrammar(C_EXIT) == -1) {
			//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "");
			//	return -1;
			//}
		}
	}
	return ExitCheck();
}

int CPatternCompiler::D7Comp()
{
	m_Index = 0;
	string strKey;

	//int nData = -1;
	char* endptr;
	int radix = 0;;

	bool bImm = false;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["D7"];

	m_pgrtree = &m_grammar_collage["D7"];

	

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		ITEM item;
		if (it == keymap.end()) {
			if (KeyType == KEY_INT || KeyType == KEY_XINT) {
				item.cmd = C_IMM;
				KeyType == KEY_INT ? radix = 10 : radix = 16;
			}
			else 
				return ExitCheck(strKey);
		}
		else {
			item = it->second;
		}

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_RESET:
		case C_INCREASE:
		case C_DECREASE:
		case C_ADD:
		case C_SUB:
		case C_RSHIFT:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.d7.alu = item.bit;
			break;
		case C_OTH:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if(m_inst.d7.alu == 0)
				m_inst.d7.alu = D78_ALU::ASSIGN_B;
			m_inst.d7.a = item.bit;
			break;
		case C_IMM:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			bImm = true;

			m_inst.tp1.a = TP1_A::D7;
			m_inst.tp1.alu = TP_ALU::REG_SET;
			m_inst.tp1.imm = strtoul(strKey.c_str(), &endptr, radix);
			break;
		default:
			return ExitCheck(strKey);
		}
	}

	if (bImm) {
		if (m_Used.bTP1 == true) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "TP1 is already set.(TP1, TPH1, D5(IMM), and D7(IMM) are set in the same register.)");
			return -1;
		}
		m_Used.bTP1 = true;
	}
	else
	{
		if (m_Used.bD7 == true) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "D7 is already setted.");
			return -1;
		}
		m_Used.bD7 = true;
	}

	return ExitCheck();
}

int CPatternCompiler::D8Comp()
{
	m_Index = 0;
	string strKey;

	//int nData = -1;
	char* endptr;
	int radix = 0;

	bool bImm = false;

	int imm = -2;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["D8"];

	m_pgrtree = &m_grammar_collage["D8"];

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		ITEM item;
		if (it == keymap.end()) {
			if (KeyType == KEY_INT || KeyType == KEY_XINT) {
				item.cmd = C_IMM;
				KeyType == KEY_INT ? radix = 10 : radix = 16;
			}
			else
				return ExitCheck(strKey);
		}
		else {
			item = it->second;
		}

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_RESET:
		case C_INCREASE:
		case C_DECREASE:
		case C_ADD:
		case C_SUB:
		case C_RSHIFT:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.d8.alu = item.bit;
			break;
		case C_OTH:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.d8.alu == 0)
				m_inst.d8.alu = D78_ALU::ASSIGN_B;
			m_inst.d8.a = item.bit;
			break;
		case C_IMM:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			bImm = true;

			m_inst.tp2.a = TP2_A::D8;
			m_inst.tp2.alu = TP_ALU::REG_SET;
			m_inst.tp2.imm = strtoul(strKey.c_str(), &endptr, radix);
			break;
		default:
			return ExitCheck(strKey);
		}
	}

	if (bImm) {
		if (m_Used.bTP2 == true) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "TP2 is already set.(TP2, TPH2, D6(IMM), and D8(IMM) are set in the same register.)");
			return -1;
		}
		m_Used.bTP2 = true;
	}
	else
	{
		if (m_Used.bD8 == true) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "D8 is already setted.");
			return -1;
		}
		m_Used.bD8 = true;
	}

	return ExitCheck();
}

int CPatternCompiler::SAComp()
{
	m_Index = 0;
	string strKey;

	ITEM item;
	int nData = 0;
	char* endptr;
	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["SA"];

	m_pgrtree = &m_grammar_collage["SA"];

	if (m_Used.bSA == true) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "SA is already setted.");
		return -1;
	}
	m_Used.bSA = true;

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);

		if (it == keymap.end()) {
			if (KeyType == KEY_INT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 10);
			}
			else if (KeyType == KEY_XINT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 16);
			}
			else {
				return ExitCheck(strKey);
			}
		}else
			item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_RESET:
		case C_OTH:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.scan.sa = item.bit;
			break;
		case C_SB:
			if (CheckGrammar(item.cmd) == -1) {
				return ExitCheck(strKey);
			}
			m_inst.scan.sa = item.bit;
			break;
		case C_ADD:
		case C_SUB:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if(m_inst.scan.sa == 0)
				m_inst.scan.sa = item.bit;
			else
				m_inst.scan.sa = item.bit + 2;
			break;
		case C_INCREASE:
		case C_DECREASE:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.scan.sa == 0)
				m_inst.scan.sa = item.bit;
			else
				m_inst.scan.sa = item.bit + 2;
			m_inst.scan.imm = 1;
			break;
		case C_OPERAND:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.scan.sa == 0)
				m_inst.scan.sa = SAB_ALU::IMM;
			m_inst.scan.imm = nData;
			break;
		default:
			return ExitCheck(strKey);
		}
	}
	return ExitCheck();
}

int CPatternCompiler::SBComp()
{
	m_Index = 0;
	string strKey;

	ITEM item;
	int nData = 0;
	char* endptr;
	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["SB"];

	m_pgrtree = &m_grammar_collage["SB"];

	if (m_Used.bSB == true) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "SB is already setted.");
		return -1;
	}
	m_Used.bSB = true;

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);

		if (it == keymap.end()) {
			if (KeyType == KEY_INT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 10);
			}
			else if (KeyType == KEY_XINT) {
				item.cmd = C_OPERAND;
				nData = strtol(strKey.c_str(), &endptr, 16);
			}
			else {
				return ExitCheck(strKey);
			}
		}else
			item = it->second;

		switch (item.cmd) {
		case C_ASSIGN:
		case C_SRC_SEL:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			break;
		case C_RESET:
		case C_OTH:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			m_inst.scan.sb = item.bit;
			break;
		case C_SA:
			if (CheckGrammar(item.cmd) == -1) {
				return ExitCheck(strKey);
			}
			m_inst.scan.sb = item.bit;
			break;
		case C_ADD:
		case C_SUB:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.scan.sb == 0)
				m_inst.scan.sb = item.bit + 2;
			else
				m_inst.scan.sb = item.bit;
			break;
		case C_INCREASE:
		case C_DECREASE:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.scan.sb == 0)
				m_inst.scan.sb = item.bit;
			else
				m_inst.scan.sb = item.bit + 2;
			m_inst.scan.imm = 1;
			break;
		case C_OPERAND:
			if (CheckGrammar(item.cmd) == -1) {
				if (ExitCheck(strKey) == 0)
					return 0;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
				return -1;
			}
			if (m_inst.scan.sb == 0)
				m_inst.scan.sb = SAB_ALU::IMM;
			m_inst.scan.imm = nData;
			break;
		default:
			return ExitCheck(strKey);
		}
	}
	return ExitCheck();
}

int CPatternCompiler::SComp()
{
	m_Index = 0;
	string strKey;

	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["S"];

	if (m_Used.bS == true) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "S is already setted.");
		return -1;
	}
	m_Used.bS = true;

	strKey = GetKey(KeyType);
	if (strKey.size() == 0 || ( strKey != "<" && strKey != "=")) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
		return -1;
	}

	strKey = GetKey(KeyType);
	if (strKey.size() == 0 || (it = keymap.find(strKey)) == keymap.end()) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
		return -1;
	}

	ITEM item = it->second;
	m_inst.s_rf_fp.s = item.bit;
	
	return 0;
}

int CPatternCompiler::RFComp()
{
	m_Index = 0;
	string strKey;

	int KeyType;
	ITEM item;
	SyntaxMap::iterator it;

	SyntaxMap& keymap = m_KeywordMapList["RF"];

	//////////////////// 1st ///////////////////////////////////
	strKey = GetKey(KeyType); // '<' 확인
	it = keymap.find(strKey);
	if (it == keymap.end() || (item = it->second).cmd != C_ASSIGN) {
		if(strKey.size())
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
		else
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Syntax ended abnormally.");
		return -1;
	}
	///////////////////// 2nd //////////////////////////////////
	strKey = GetKey(KeyType); // "0" or "RF"
	it = keymap.find(strKey);
	if (it == keymap.end() ) {
		if (strKey.size())
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
		else
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Syntax ended abnormally.");
		return -1;
	}
	item = it->second;	
	switch (item.cmd)
	{
	case C_RESET:
		m_inst.s_rf_fp.rf = item.bit;
		return 0;
	case C_SRC_SEL:
		m_inst.s_rf_fp.rf = item.bit;
		break;
	default:
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
		return -1;
	}

	///////////////////// 3rd //////////////////////////////////
	strKey = GetKey(KeyType);

	it = keymap.find(strKey);
	if (it == keymap.end()) {
		if(strKey.size()) m_BufKey.push_back(strKey);
	}
	else {
		m_inst.s_rf_fp.rf = ((ITEM)it->second).bit;
	}

	return 0;
}

int CPatternCompiler::ImmComp()
{
	string strKey;
	char* endptr;
	int KeyType;
	SyntaxMap::iterator it;

	strKey = GetKey(KeyType);
	if (strKey.size() == 0 || (strKey != "<" && strKey != "=")) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
		return -1;
	}

	unsigned int value = 0;
	strKey = GetKey(KeyType);
	if (KeyType == KEY_INT) {
		value = strtoul(strKey.c_str(), &endptr, 10);
	}
	else if (KeyType == KEY_XINT)
	{
		value = strtoul(strKey.c_str(), &endptr, 16);
	}

	switch (m_ImmIdx) {
		case IMM_IDX::XMAX: 
			m_inst.xn.alu = m_ImmAlu;
			m_inst.xn.imm = value;
			break;
		case IMM_IDX::YMAX: 
			m_inst.yn.alu = m_ImmAlu;
			m_inst.yn.imm = value;
			break;
		case IMM_IDX::ZMAX: 
		case IMM_IDX::ZH:
			m_inst.z.alu = m_ImmAlu;
			m_inst.z.imm = value;
			break;
		//case IMM_IDX::TPX1: 
		//	m_inst.tp1.alu = m_ImmAlu;
		//	m_inst.tp1.imm = value;
		//	break;
		//case IMM_IDX::TPX2: 
		//	m_inst.tp2.alu = m_ImmAlu;
		//	m_inst.tp2.imm = value;
		//	break;
	}

	return 0;
}

int CPatternCompiler::TPXComp()
{
	string strKey;
	char* endptr;
	int KeyType;
	SyntaxMap::iterator it;

	size_t pos = m_pos;

	strKey = GetKey(KeyType);
	//if (strKey.size() == 0 && (strKey != "<" && strKey != "=")) {
	if (strKey != "<" && strKey != "=") {
		m_pos = pos;
		if (m_ImmIdx == IMM_IDX::TPX1)
			m_inst.ctrl.tpx1 = 1;
		else if (m_ImmIdx == IMM_IDX::TPX2)
			m_inst.ctrl.tpx2 = 1;

		return 0;
	}

	unsigned int value = 0;
	strKey = GetKey(KeyType);
	if (KeyType == KEY_INT) {
		value = strtoul(strKey.c_str(), &endptr, 10);
	}
	else if (KeyType == KEY_XINT) {
		value = strtoul(strKey.c_str(), &endptr, 16);
	}
	else {
		if(strKey.size() == 0)
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Syntax ended abnormally.");
		else 
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
		return -1;
	}

	if (m_ImmIdx == IMM_IDX::TPX1) {
		m_inst.tp1.alu = m_ImmAlu;
		m_inst.tp1.imm = value;
	}else if(m_ImmIdx == IMM_IDX::TPX2) {
			m_inst.tp2.alu = m_ImmAlu;
			m_inst.tp2.imm = value;
	}

	return 0;
}

int CPatternCompiler::D1Comp()
{
	m_Index = 0;
	string strKey;

	ITEM item;
	//int nData = 0;
	char* endptr;
	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["DX"];

	m_pgrtree = &m_grammar_collage["DX"];

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		if (it == keymap.end()) {
			if (KeyType & (KEY_INT | KEY_XINT)) {
				item.cmd = C_OPERAND;
			}
			else {
				return ExitCheck(strKey);
			}
		}
		else
			item.value = it->second;

		if (CheckGrammar(item.cmd) == -1) {
			if (ExitCheck(strKey) == 0)
				return 0;
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
			return -1;
		}

		if (item.cmd == C_ASSIGN) {
			m_inst.xb.a = m_DxIdx;
			m_inst.xb.alu = AB_ALU::REG_SET;
			continue;
		}

		if (item.cmd == C_OPERAND) {
			if(KeyType == KEY_INT)
				m_inst.xn.imm = strtoul(strKey.c_str(), &endptr, 10);
			else if (KeyType == KEY_XINT)
				m_inst.xn.imm = strtoul(strKey.c_str(), &endptr, 16);
		}
	}

	return 0;
}

int CPatternCompiler::D2Comp()
{
	m_Index = 0;
	string strKey;

	ITEM item;
	//int nData = 0;
	char* endptr;
	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["DX"];

	m_pgrtree = &m_grammar_collage["DX"];

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		if (it == keymap.end()) {
			if (KeyType & (KEY_INT | KEY_XINT)) {
				item.cmd = C_OPERAND;
			}
			else {
				return ExitCheck(strKey);
			}
		}
		else
			item.value = it->second;

		if (CheckGrammar(item.cmd) == -1) {
			if (ExitCheck(strKey) == 0)
				return 0;
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
			return -1;
		}

		if (item.cmd == C_ASSIGN) {
			m_inst.yb.a = m_DxIdx;
			m_inst.yb.alu = AB_ALU::REG_SET;
			continue;
		}

		if (item.cmd == C_OPERAND) {
			if (KeyType == KEY_INT)
				m_inst.yn.imm = strtoul(strKey.c_str(), &endptr, 10);
			else if (KeyType == KEY_XINT)
				m_inst.yn.imm = strtoul(strKey.c_str(), &endptr, 16);
		}
	}

	return 0;
}

int CPatternCompiler::TPH1Comp()
{
	m_Index = 0;
	string strKey;

	ITEM item;
	//int nData = 0;
	char* endptr;
	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["TPH1X"];

	m_pgrtree = &m_grammar_collage["TPH1X"];

	if (m_Used.bTP1) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "TPH1 is already set.(TP1, TPH1, D5(IMM), and D7(IMM) are set in the same register.)");
		return -1;
	}
	m_Used.bTP1 = true;

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		if (it == keymap.end()) {
			if (KeyType & (KEY_INT | KEY_XINT)) {
				item.cmd = C_OPERAND;
			}
			else {
				return ExitCheck(strKey);
			}
		}
		else
			item.value = it->second;

		if (CheckGrammar(item.cmd) == -1) {
			if (ExitCheck(strKey) == 0)
				return 0;
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
			return -1;
		}

		if (item.cmd == C_ASSIGN) {
			m_inst.tp1.a = m_DxIdx;
			m_inst.tp1.alu = TP_ALU::REG_SET;
			continue;
		}
		else if (item.cmd == C_OPERAND) {
			m_inst.tp1.alu = 0xf;
			if (KeyType == KEY_INT)
				m_inst.tp1.imm = strtoul(strKey.c_str(), &endptr, 10);
			else if (KeyType == KEY_XINT)
				m_inst.tp1.imm = strtoul(strKey.c_str(), &endptr, 16);
		}
		else if (item.cmd == C_TP1) {
			m_inst.tp1.alu = TP_ALU::TPH_ASSIGN_TP;
		}
	}

	return 0;
}

int CPatternCompiler::TPH2Comp()
{
	m_Index = 0;
	string strKey;

	ITEM item;
	//int nData = 0;
	char* endptr;
	int KeyType;
	SyntaxMap::iterator it;
	SyntaxMap& keymap = m_KeywordMapList["TPH2X"];

	m_pgrtree = &m_grammar_collage["TPH2X"];

	if (m_Used.bTP2) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "TPH2 is already set.(TP2, TPH2, D6(IMM), and D8(IMM) are set in the same register.)");
		return -1;
	}
	m_Used.bTP2 = true;

	while ((strKey = GetKey(KeyType)) != "") {
		it = keymap.find(strKey);
		if (it == keymap.end()) {
			if (KeyType & (KEY_INT | KEY_XINT)) {
				item.cmd = C_OPERAND;
			}
			else {
				return ExitCheck(strKey);
			}
		}
		else
			item.value = it->second;

		if (CheckGrammar(item.cmd) == -1) {
			if (ExitCheck(strKey) == 0)
				return 0;
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, m_strOrgKey.empty() ? strKey : m_strOrgKey, "Invalide syntax.");
			return -1;
		}

		if (item.cmd == C_ASSIGN) {
			m_inst.tp2.a = m_DxIdx;
			m_inst.tp2.alu = TP_ALU::REG_SET;
			continue;
		}
		else if (item.cmd == C_OPERAND) {
			m_inst.tp2.alu = 0xf;
			if (KeyType == KEY_INT)
				m_inst.tp2.imm = strtoul(strKey.c_str(), &endptr, 10);
			else if (KeyType == KEY_XINT)
				m_inst.tp2.imm = strtoul(strKey.c_str(), &endptr, 16);
		}
		else if (item.cmd == C_TP2) {
			m_inst.tp2.alu = TP_ALU::TPH_ASSIGN_TP;
		}
	}

	return 0;
}

void CPatternCompiler::SetName(string name)
{
	m_strName = name;
}

void CPatternCompiler::Dump(ofstream& ofile)
{
	ofile << "Pattern Data." << endl;
	char buf[100];
	unsigned char* ptr;
	
	for (size_t idx = 0; idx < m_InstList.size(); idx++) {
		ptr = (unsigned char *)m_InstList[idx].data;
		ofile << "PC:" << idx << endl;
		for (int i = 0; i < INST_SIZE; i += 16) {
			sprintf(buf, "%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X\n"
			, (ptr[i   ] & 0xFF), (ptr[i+ 1] & 0xFF), (ptr[i+ 2] & 0xFF), (ptr[i+ 3] & 0xFF)
			, (ptr[i+4 ] & 0xFF), (ptr[i+ 5] & 0xFF), (ptr[i+ 6] & 0xFF), (ptr[i+ 7] & 0xFF)
			, (ptr[i+8 ] & 0xFF), (ptr[i+ 9] & 0xFF), (ptr[i+10] & 0xFF), (ptr[i+11] & 0xFF)
			, (ptr[i+12] & 0xFF), (ptr[i+13] & 0xFF), (ptr[i+14] & 0xFF), (ptr[i+15] & 0xFF));
			ofile << buf;
		}
	}

}

int CPatternCompiler::GetLastOpcode()
{
	return m_PrevOpcode;
}
