#include <string>
#include <vector>
#include <stack>
#include <map>
#include <fstream>
#include <iostream>
#include <filesystem>
#ifdef __linux__
#include <string.h>
#endif

#include "PGComLib.h"
#include "ErrorLog.h"
#include "PGC.h"

using namespace std;
using namespace KOCC;
//using namespace OPT;

CPGC::CPGC(string file, int opt)
	:m_bLoadSucceed(true)
	, m_pFScan(NULL)
	, m_pos(0)
	, m_bEnComp(true)
	, m_Index(0)
	, m_Comp_Mode(COMP_TYPE::COMMON)
	, m_pc(0)
	//,m_comp_opt(0)
{
	CreateKeyword();
	GenerateGrammar();

	if (LoadFile(file)) {
		m_strFirstFilePath = m_pFScan->GetPath();
		m_strFirstFileName = m_pFScan->GetFileName();
	}

	m_comp_opt = opt;
	//if (opt != -1) m_comp_opt = opt;
	//else m_comp_opt = 0;
}

CPGC::~CPGC()
{
	if (m_pFScan != NULL)
	{
		delete m_pFScan;
	}

	while (m_vecFileScanner.size()) {
		delete m_vecFileScanner.back();
		m_vecFileScanner.pop_back();
	}
}

bool CPGC::LoadFile(string strFile)
{
	// 절대경로를 만듬
	string absPath;
	if (filesystem::path(strFile).is_absolute()) {
		absPath = strFile;
	}
	else {
		absPath = filesystem::current_path().string();
#ifdef __linux__
		absPath += "/";
#else
		absPath += "\\";
#endif
		absPath += strFile;
	}

	// 이미 파일을 open 했었다면 vector에 저장하고 새로 생성 후 open한다.
	if (m_pFScan) m_vecFileScanner.push_back(m_pFScan);
	m_pFScan = new CFileScanner;
	CErrorLog::GetInstance()->SetFileScanner(m_pFScan);
	return m_bLoadSucceed = m_pFScan->Open(absPath);
}

int CPGC::DoCompile(int comp_opt)
{
	if (comp_opt != -1) {
		m_RootPath = filesystem::current_path().string();
		m_comp_opt = comp_opt;
	}

	string strKey;
	int nType = 0;
	bool result = true;
	while (m_pFScan->ReadLine(m_strLine) != -1) 
	{
		m_pos = 0;
		if (m_strLine.size() == 0)
			continue;
		strKey = GetFirstKey();
		SyntaxMap::iterator it = m_KeywordList.find(strKey);
		// 현재 클래스에서 처리
		if (it != m_KeywordList.end()) {
			ITEM item(it->second);
			result = Compile(strKey, item);
		}
		else {
			// 컴파일 하지 않는경우 continue
			if (!m_bEnComp) continue;

			//DefineApply();

			// 라벨 확인 및 처리 함수 호출
			if (strKey.back() == ':') {
				result = LabelPattComp(strKey);
				if (result == false)
					return -1;
				strKey = GetKey(nType);
				// 라벨만 존재하는경우 continue
				if (strKey.length() == 0)
					continue;
			}

			//개별 컴파일러 에서 처리
			switch (m_Comp_Mode) {
				case COMP_TYPE::ADDR_DEF:
					break;
				case COMP_TYPE::REGISTER:
					result = m_RegComp.Compile(m_strLine);
					break;
				case COMP_TYPE::PATTERN:
					DefineApply();
					result = m_PatComp.Compile(m_strLine);
					m_pc++;
					if (m_pc > MAX_PATTERN_SIZE) {
						result = false;
						CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Pattern size is over.");
					}
					break;
				default :
					result = false;
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
					break;
			}
		}
		// 개별 컴파일 결과 Error 인 경우
		if (result == false) {
			//CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
			return -1;
		}
	}

	if (!m_vecFileScanner.empty()) {
		delete m_pFScan;
		m_pFScan = m_vecFileScanner.back();
		m_vecFileScanner.pop_back();
		m_RootPath = m_pFScan->GetPath();
		CErrorLog::GetInstance()->SetFileScanner(m_pFScan);
		filesystem::current_path(m_RootPath);
		//string tempPath = filesystem::current_path().string();
		//cout << tempPath << endl;
	}
	else {
		// 컴파일 후 에러 확인. %ENDC,
		if (m_stackEnComp.size() != 0) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "There is no '%ENDC' syntax.");
			return -1;
		}
		// START...END, FUNC...ENDFUNC, REGISTER...ENDREGISTER
		if (m_Comp_Mode != COMP_TYPE::COMMON) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Pair does not match.");
			return -1;
		}
	}
	return 0;
}

int CPGC::PostCompile()
{
	string strName = m_RegComp.GetISPName();
	bool bFind = false;
	if (strName.length()) {
		for (size_t i = 0; i < m_vecPatInfo.size(); i++) {
			if (strName == m_vecPatInfo[i].name) {
				m_RegComp.SetISP(m_vecPatInfo[i].start);
				bFind = true;
				break;
			}
		}
		if (bFind == false) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strName, "ISP Function not Exist.");
			return -1;
		}
	}

	// 라벨 및 Func 호출 처리
	return m_PatComp.PostComp(m_vecPatInfo, m_vecLabelInfo);
	//return 0;
}

int CPGC::WriteFile(string filename)
{
	//// 파일이름이 없는경우  pattern 파일이름에서 확장자만 변경
	if (filename == "") { 
		filename = m_strFirstFilePath + "/" + m_strFirstFileName;
		size_t pos = filename.rfind('.') + 1;
		filename.erase(pos);
		filename += "mpa";
	}

	////파일이 이미 존재 하는지 확인
	std::filesystem::path curPath(filename);
	if (std::filesystem::exists(curPath) == true) {
		std::filesystem::remove(curPath);
		if (std::filesystem::exists(curPath) == true) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Out file remove fail.(Already exist.)");
			return -1;
		}
	}
	
	///// File Open 
	ofstream ofile;
	ofile.open(filename,std::ios_base::binary);
	if (ofile.is_open() == false) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, filename, "File open fail.");
		return -1;
	}


	int size = 0;
	unsigned short name_size = 0;

	////// Header Write
	fileheader_t header;
	memset(header.data, 0, sizeof(fileheader_t));

	header.sw_v.major = 0;
	header.sw_v.minor = 1;
	header.sw_v.build = 0;

	std::time_t tNow = std::time(NULL);
	std::tm local_time;
	local_time = *std::localtime(&tNow);

	header.date.year = local_time.tm_year + 1900;
	header.date.month = local_time.tm_mon + 1;
	header.date.day = local_time.tm_mday;
	header.date.hour = local_time.tm_hour;
	header.date.min = local_time.tm_min;
	header.date.sec = local_time.tm_sec;

	header.head_size = sizeof(fileheader_t);
	header.reg_size = sizeof(registerset_test_t);

	ofile.write((char*)header.data, sizeof(fileheader_t));

	/////// Write Register
	size = m_RegComp.WriteFile(ofile);
	if (size < 0) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "File (register) write fail.");
		return -1;
	}
	header.reg_size = size;

	/////// Write Pattern
	size = m_PatComp.WriteFile(ofile);
	if (size < 0) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "File (pattern) write fail.");
		return -1;
	}
	header.pat_size = size;

	//////// Info Write ///////////////////////////////////////////////////////////
	///////////////// Pattern Info write /////////
	size = (int)m_vecPatInfo.size();
	ofile.write((char*)&size, sizeof(size));
	for (unsigned int i = 0; i < m_vecPatInfo.size(); i++) {
		name_size = (unsigned short)m_vecPatInfo[i].name.size();
		ofile.write((char*)&m_vecPatInfo[i].data, sizeof(m_vecPatInfo[i].data));
		ofile.write((char*)&name_size, sizeof(name_size));
		ofile.write(m_vecPatInfo[i].name.c_str(), m_vecPatInfo[i].name.size());
	}
	///////////////// Label Info write ///////////
	size = (int)m_vecLabelInfo.size();
	ofile.write((char*)&size, sizeof(size));
	for (size_t i = 0; i < m_vecLabelInfo.size(); i++) {
		name_size = (int)m_vecLabelInfo[i].name.size();
		ofile.write((char*)&m_vecLabelInfo[i].pc, sizeof(m_vecLabelInfo[i].pc));
		ofile.write((char*)&name_size, sizeof(name_size));
		ofile.write(m_vecLabelInfo[i].name.c_str(), m_vecLabelInfo[i].name.size());
	}
	///////////////////////////////////////////////////////////////////////////////

	////// Header Rewrite
	header.total_size = header.head_size + header.reg_size + header.pat_size;
	ofile.seekp(0);
	ofile.write((char*)header.data, sizeof(fileheader_t));

	////// File Close
	ofile.close();

	return 0;
}

void CPGC::DefineApply()
{
	m_strLine += " "; // 마지막 키워드를 처리하기 위해 추가
	string strKey;
	size_t size = m_strLine.size();
	unsigned int pos = 0; // 첫번째 문자부터 replace되는 경우 -1이 될 수 있어야 한다.
	int repos = -1;
	int count = 0;
	bool bFirst = true;
	std::map<std::string, std::string>::iterator it;
	while (pos < size)
	{
		if (isspace(m_strLine[pos]) || 
			m_strLine[pos] == '<' || m_strLine[pos] == '=' || 
			m_strLine[pos] == '+' || m_strLine[pos] == '-' || 
			m_strLine[pos] == '*' || 
			m_strLine[pos] == '&' || m_strLine[pos] == '|' || m_strLine[pos] == '^' ) {
			if (strKey.length() > 0) {
				if ( bFirst && m_PatComp.IsOpcode(strKey)) {
					strKey.clear();
					repos = -1;
					pos++;
					bFirst = false;
					continue;
				}
				else {
					it = m_mapDefine.find(strKey);
					if (it != m_mapDefine.end()) {
						m_strLine.replace(repos, count, it->second.c_str(), it->second.length());
						pos = repos - 1;
						size = m_strLine.size();
					}
				}
				strKey.clear();
			}

			repos = -1;
		}
		else {
			strKey += m_strLine[pos];
			if (repos == -1) {
				repos = pos;
				count = 1;
			}
			else
				count++;
		}
		pos++;
	}

	if (strKey.length() > 0) {
		it = m_mapDefine.find(strKey);
		if (it != m_mapDefine.end()) {
			m_strLine.replace(repos, count, it->second.c_str(), it->second.length());
			pos = repos - 1;
		}
		strKey.clear();
	}
}

bool CPGC::Compile(string & strKey, ITEM& item)
{
	bool result = true;
	int nType;

	switch (item.cmd) {
		case C_DEF:
			if (m_bEnComp) result = DefineComp();
			else m_pos = m_strLine.length(); // 현재 라인 Skip
			break;
		case C_BEGIN_REG_BLK:
			if (m_bEnComp) {
				if (m_Comp_Mode == COMMON)
					m_Comp_Mode = REGISTER;
				else {
					result = false;
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Pair does not match.");
				}
				strKey = GetName(nType);
				if (strKey != "") {
					result = false;
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invaide syntax.");
				}
			}
			else m_pos = m_strLine.length(); // 현재 라인 Skip
			break;
		case C_END_REG_BLK:
			if (m_bEnComp) {
				if (m_Comp_Mode == REGISTER)
					m_Comp_Mode = COMMON;
				else {
					result = false;
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Pair does not match.");
				}
			}
			else m_pos = m_strLine.length(); // 현재 라인 Skip
			break;
		case C_BEGIN_PAT_BLK:
			if (m_bEnComp) {
				if (m_Comp_Mode == COMMON) {
					m_Comp_Mode = PATTERN;
					result = BeginPattComp(PAT_TYPE::PATTERN);
				}
				else {
					result = false;
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Pair does not match.");
				}
			}
			else m_pos = m_strLine.length(); // 현재 라인 Skip
			break;
		case C_END_PAT_BLK:
			if (m_bEnComp) {
				if (m_Comp_Mode == PATTERN) {
					m_Comp_Mode = COMMON;
					m_PatComp.AddSTPS();
					m_pc++;
					if (m_pc > MAX_PATTERN_SIZE) {
						result = false;
						CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Pattern size is over.");
					}
				}
				else {
					result = false;
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Pair does not match.");
				}
			}
			else m_pos = m_strLine.length(); // 현재 라인 Skip
			break;
		case C_BEGIN_FUNC_BLK:
			if (m_bEnComp) {
				if (m_Comp_Mode == COMMON) {
					m_Comp_Mode = PATTERN;
					result = BeginPattComp(PAT_TYPE::FUNCTION);
				}
				else {
					result = false;
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Pair does not match.");
				}
			}
			else m_pos = m_strLine.length(); // 현재 라인 Skip
			break;
		case C_END_FUNC_BLK:
			if (m_bEnComp) {
				if (m_Comp_Mode == PATTERN) {
					m_Comp_Mode = COMMON;
					if (m_PatComp.GetLastOpcode() != PAT_OP::RTN) {
						result = false;
						CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "The FUNCION to ENDFUNCTION block must end in RTN opcode.");
					}
				}
				else {
					result = false;
					CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Pair does not match.");
				}
			}
			else m_pos = m_strLine.length(); // 현재 라인 Skip
			break;
		case C_IF_INSERT: InsertComp(); break; 
		case C_IFE_COMP: result = IFComp(true); break;
		case C_IFN_COMP: result = IFComp(false); break;
		case C_ENDC: {
			if (m_stackEnComp.size() != 0) {
				m_bEnComp = m_stackEnComp.back();
				m_stackEnComp.pop_back();
			}
			else {
				result = false;
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Pair does not match.");
			}
			break;
		}
		default: {
			result = false;
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Invalide keyword.");
			break;
		}
	}

	return result;

	//if (m_bEnComp) {
	//	switch (item.cmd) {
	//		case C_DEF: result = DefineComp();		break;
	//		case C_BEGIN_REG_BLK:
	//			if (m_Comp_Mode == COMMON)
	//				m_Comp_Mode = REGISTER;
	//			else {
	//				result = false;
	//				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "REGISTER", "Pair does not match.");
	//			}
	//			break;
	//		case C_BEGIN_PAT_BLK:
	//			if (m_Comp_Mode == COMMON)
	//				m_Comp_Mode = PATTERN;
	//			else {
	//				result = false;
	//				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "START", "Pair does not match.");
	//			}
	//			break;
	//		case C_END_REG_BLK:
	//			if (m_Comp_Mode == REGISTER)
	//				m_Comp_Mode = COMMON; 
	//			else {
	//				result = false;
	//				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "ENDREGISTER", "Pair does not match.");
	//			}
	//			break;
	//		case C_END_PAT_BLK:	
	//			if (m_Comp_Mode == PATTERN)
	//				m_Comp_Mode = COMMON;
	//			else {
	//				result = false;
	//				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "ENDREGISTER", "Pair does not match.");
	//			}
	//			break;
	//	}
	//}
	//
	//switch (item.cmd) {
	//	case C_IF_INSERT: break;
	//	case C_IFE_COMP: result = IFComp(true); break;
	//	case C_IFN_COMP: result = IFComp(false); break;
	//	case C_ENDC: {
	//		if (m_stackEnComp.size() != 0) {
	//			m_bEnComp = m_stackEnComp.back();
	//			m_stackEnComp.pop_back();
	//		}
	//		else {
	//			result = false;
	//			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "%IFE(N), %ENDC", "Pair does not match.");
	//		}
	//		break;
	//	}
	//	default: {
	//		result = false;
	//		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "%IFE(N), %ENDC", "Pair does not match.");
	//		break;
	//	}
	//}
	//return result;
}

void CPGC::CreateKeyword()
{
	m_KeywordList["SDEF"       ] = ENCODE(C_DEF,            C_DEF,            0);
	m_KeywordList["REGISTER"   ] = ENCODE(C_BEGIN_REG_BLK,  C_BEGIN_REG_BLK,  0);
	m_KeywordList["ENDREGISTER"] = ENCODE(C_END_REG_BLK,    C_END_REG_BLK,    0);
	m_KeywordList["START"      ] = ENCODE(C_BEGIN_PAT_BLK,  C_BEGIN_PAT_BLK,  0);
	m_KeywordList["END"        ] = ENCODE(C_END_PAT_BLK,    C_END_PAT_BLK,    0);
	m_KeywordList["FUNC"       ] = ENCODE(C_BEGIN_FUNC_BLK, C_BEGIN_FUNC_BLK, 0);
	m_KeywordList["FUNCTION"   ] = ENCODE(C_BEGIN_FUNC_BLK, C_BEGIN_FUNC_BLK, 0);
	m_KeywordList["ENDFUNC"    ] = ENCODE(C_END_FUNC_BLK,   C_END_FUNC_BLK,   0);
	m_KeywordList["ENDFUNCTION"] = ENCODE(C_END_FUNC_BLK,   C_END_FUNC_BLK,   0);
	m_KeywordList["INSERT"     ] = ENCODE(C_INSERT,         C_INSERT,         0);
	m_KeywordList["%INSERT"    ] = ENCODE(C_IF_INSERT,      C_IF_INSERT,      0);
	m_KeywordList["%IFE"       ] = ENCODE(C_IFE_COMP,       C_IFE_COMP,       0);
	m_KeywordList["%IFN"       ] = ENCODE(C_IFN_COMP,       C_IFN_COMP,       0);
	m_KeywordList["%ENDC"      ] = ENCODE(C_ENDC,           C_ENDC,           0);


	m_KeywordList["("    ] = ENCODE(C_ROUND_LEFT,  0,      0 );
	m_KeywordList[")"    ] = ENCODE(C_ROUND_RIGHT, 0,      0 );
	m_KeywordList[".AND."] = ENCODE(C_OP_INFIX,    1, OP_AND );
	m_KeywordList[".OR." ] = ENCODE(C_OP_INFIX,    2, OP_OR  );
	m_KeywordList[".XOR."] = ENCODE(C_OP_INFIX,    2, OP_XOR );
	m_KeywordList[".NOT."] = ENCODE(C_OP_UNARY,    3, OP_NOT );
	m_KeywordList["A"    ] = ENCODE(C_ALPHA_CHAR,  4,      0 );
	m_KeywordList["B"    ] = ENCODE(C_ALPHA_CHAR,  4,      1 );
	m_KeywordList["C"    ] = ENCODE(C_ALPHA_CHAR,  4,      2 );
	m_KeywordList["D"    ] = ENCODE(C_ALPHA_CHAR,  4,      3 );
	m_KeywordList["E"    ] = ENCODE(C_ALPHA_CHAR,  4,      4 );
	m_KeywordList["F"    ] = ENCODE(C_ALPHA_CHAR,  4,      5 );
	m_KeywordList["G"    ] = ENCODE(C_ALPHA_CHAR,  4,      6 );
	m_KeywordList["H"    ] = ENCODE(C_ALPHA_CHAR,  4,      7 );
	m_KeywordList["I"    ] = ENCODE(C_ALPHA_CHAR,  4,      8 );
	m_KeywordList["J"    ] = ENCODE(C_ALPHA_CHAR,  4,      9 );
	m_KeywordList["K"    ] = ENCODE(C_ALPHA_CHAR,  4,     10 );
	m_KeywordList["L"    ] = ENCODE(C_ALPHA_CHAR,  4,     11 );
	m_KeywordList["M"    ] = ENCODE(C_ALPHA_CHAR,  4,     12 );
	m_KeywordList["N"    ] = ENCODE(C_ALPHA_CHAR,  4,     13 );
	m_KeywordList["O"    ] = ENCODE(C_ALPHA_CHAR,  4,     14 );
	m_KeywordList["P"    ] = ENCODE(C_ALPHA_CHAR,  4,     15 );
	m_KeywordList["Q"    ] = ENCODE(C_ALPHA_CHAR,  4,     16 );
	m_KeywordList["R"    ] = ENCODE(C_ALPHA_CHAR,  4,     17 );
	m_KeywordList["S"    ] = ENCODE(C_ALPHA_CHAR,  4,     18 );
	m_KeywordList["T"    ] = ENCODE(C_ALPHA_CHAR,  4,     19 );
	m_KeywordList["U"    ] = ENCODE(C_ALPHA_CHAR,  4,     20 );
	m_KeywordList["V"    ] = ENCODE(C_ALPHA_CHAR,  4,     21 );
	m_KeywordList["W"    ] = ENCODE(C_ALPHA_CHAR,  4,     22 );
	m_KeywordList["X"    ] = ENCODE(C_ALPHA_CHAR,  4,     23 );
	m_KeywordList["Y"    ] = ENCODE(C_ALPHA_CHAR,  4,     24 );
	m_KeywordList["Z"    ] = ENCODE(C_ALPHA_CHAR,  4,     25 );
}

void CPGC::GenerateGrammar()
{
	//vector<vector<ITEM>> if_tree = {
	//	/*0 "("*/ { ITEM(C_ROUND_LEFT, C_ROUND_LEFT, 0), ITEM(C_ALPHA_CHAR,  C_ALPHA_CHAR,  1), ITEM(C_OP_UNARY, C_OP_UNARY, 4), },
	//	/*1 "A"*/ { ITEM(C_OP_INFIX,   C_OP_INFIX,   3), ITEM(C_ROUND_RIGHT, C_ROUND_RIGHT, 2), ITEM(C_EXIT,     C_EXIT,    -1), },
	//	/*2 ")"*/ { ITEM(C_OP_INFIX,   C_OP_INFIX,   3), ITEM(C_ROUND_RIGHT, C_ROUND_RIGHT, 2), ITEM(C_EXIT,     C_EXIT,    -1), },
	//	/*3 "&"*/ { ITEM(C_ROUND_LEFT, C_ROUND_LEFT, 0), ITEM(C_ALPHA_CHAR,  C_ALPHA_CHAR,  1), ITEM(C_OP_UNARY, C_OP_UNARY, 4), },
	//	/*4 "!"*/ { ITEM(C_ALPHA_CHAR, C_ALPHA_CHAR, 1), }
	//};

	vector<vector<ITEM>> if_tree = {
		/*0 "("  */ { ITEM(C_ROUND_LEFT, C_ROUND_LEFT, 0), ITEM(C_ALPHA_CHAR,  C_ALPHA_CHAR,  1), ITEM(C_OP_UNARY, C_OP_UNARY, 3) },
		/*1 "A)" */ { ITEM(C_OP_INFIX,   C_OP_INFIX,   2), ITEM(C_ROUND_RIGHT, C_ROUND_RIGHT, 1), ITEM(C_EXIT,     C_EXIT,    -1) },
		/*2 "&"  */ { ITEM(C_ROUND_LEFT, C_ROUND_LEFT, 0), ITEM(C_ALPHA_CHAR,  C_ALPHA_CHAR,  1), ITEM(C_OP_UNARY, C_OP_UNARY, 3) },
		/*3 "!"  */ { ITEM(C_ALPHA_CHAR, C_ALPHA_CHAR, 1), }
	};
	
	m_grammar_collage["%IF"] = if_tree;
}

string CPGC::GetFirstKey()
{
	if (m_strLine.size() == 0)
		return NULL;

	string strKey;
	size_t size = m_strLine.size();
	while (m_pos < size)
	{
		if (isspace(m_strLine[m_pos])) {
			m_pos++;
			continue;
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

string CPGC::GetName(int& pType)
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
			if (strKey.size() > 2) {
				if(strKey[0] == '0' && strKey[1] == 'x')
					pType = KEY_XINT;
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

string CPGC::GetDefineName()
{
	if (m_strLine.size() == 0)
		return NULL;

	string strKey;
	size_t size = m_strLine.size();
	while (m_pos < size)
	{
		if (isspace(m_strLine[m_pos])) {
			m_pos++;
			continue;
		}
		else if ((m_pos < size) && (m_strLine[m_pos] == '/' || m_strLine[m_pos] == '_' || isalpha(m_strLine[m_pos]))) { /// ABC, _ABC
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

string CPGC::GetKey(int & pType)
{
	if (m_strLine.size() == 0)
		return NULL;

	pType = KEY_STR;

	string strKey;
	size_t size = m_strLine.size();
	while (m_pos < size)
	{
		if (isspace(m_strLine[m_pos])) {
			m_pos++;
			continue;
		}
		else if (m_strLine[m_pos] == '.') {
			strKey.push_back(m_strLine[m_pos++]);
			while ((m_pos < size) && !isspace(m_strLine[m_pos]) && m_strLine[m_pos]!= '.') {
				strKey.push_back(m_strLine[m_pos++]);
			}

			if(m_strLine[m_pos] == '.')
				strKey.push_back(m_strLine[m_pos++]);

			return strKey;
		}
		else if (m_strLine[m_pos] == '%') {
			strKey.push_back(m_strLine[m_pos++]);
			while ((m_pos < size) && !isspace(m_strLine[m_pos])) {
				strKey.push_back(m_strLine[m_pos++]);
			}

			return strKey;
		}
		else if (m_strLine[m_pos] == '(' || m_strLine[m_pos] == ')') {
			strKey.push_back(m_strLine[m_pos++]);

			return strKey;
		}
		else if (isalpha(m_strLine[m_pos])) {
			while ((m_pos < size) && (isalpha(m_strLine[m_pos]) || isdigit(m_strLine[m_pos]))) {
				strKey.push_back(m_strLine[m_pos++]);
			}
			return strKey;
		}
		else if (isdigit(m_strLine[m_pos])) {
			while (m_pos < size && isdigit(m_strLine[m_pos])) {
				strKey.push_back(m_strLine[m_pos++]);
			}
			pType = KEY_INT;
			return strKey;
		}
		else if (m_strLine[m_pos] == '0') {
			strKey.push_back(m_strLine[m_pos++]);
			if (m_strLine[m_pos] == 'x') {
				strKey.push_back(m_strLine[m_pos++]);
				while (m_pos < size && isxdigit(m_strLine[m_pos])) {
					strKey.push_back(m_strLine[m_pos++]);
				}
				pType = KEY_XINT;
				return strKey;
			}
			else {
				while (m_pos < size && isdigit(m_strLine[m_pos])) {
					strKey.push_back(m_strLine[m_pos++]);
				}
				pType = KEY_INT;
				return strKey;
			}

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

bool CPGC::DefineComp()
{
	//string strDef = GetKey(nType);
	string strDef = GetDefineName();

	size_t size = m_strLine.length();
	if (strDef.size()) {
		// Register Reserved 키워드 확인
		if (m_RegComp.IsKeyword(strDef)) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strDef, " is reserved keyword.");
			return false;
		}
		// Pattern Reserved 키워드 확인
		if (strDef != "NOP" && m_PatComp.IsOpcode(strDef)) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strDef, " is reserved keyword.");
			return false;
		}
		// 이미 정의되어 있는지 확인
		if (m_mapDefine.find(strDef) != m_mapDefine.end()) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strDef, " is already defiend.");
			return false;
		}

		while (isspace(m_strLine[m_pos]) && m_pos < size) m_pos++;

		if (m_pos < size) {
			m_mapDefine[strDef] = m_strLine.substr(m_pos);
			m_pos += m_mapDefine[strDef].size();
		}
		else {
			m_mapDefine[strDef] = "";
		}
	}
	else {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "SDEF", "Syntax Error.");
		return false;
	}
	return true;
}

bool CPGC::IFComp(bool bPassTrue)
{
	m_Index = 0;

	string strKey;// = GetKey();

	SyntaxMap::iterator it;

	unsigned int usedBitAlpha = 0;
	vector<soda_t> vecPost;
	stack<soda_t> stkAbra;

	soda_t soda;

	int nRound = 0;

	int nType;

	while ((strKey = GetKey(nType)) != "") {
		it = m_KeywordList.find(strKey);
		if (it == m_KeywordList.end()) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Unknown Keyword.");
			return false;
		}
		ITEM item = it->second;
		if (CheckGrammar(item.cmd) != -1) {
			switch (item.cmd)
			{
				case C_ALPHA_CHAR: {
					if (usedBitAlpha & (1 << item.bit)) {
						CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is already used.");
						return false;
					}
					usedBitAlpha |= (1 << item.bit);
					/////////////////////////////////////////////////// 
					memset(&soda, 0, sizeof(soda_t));
					strcpy(soda.token, strKey.c_str());
					soda.item = item;
					vecPost.push_back(soda);
					/////////////////////////////////////////////////// 
					break;//
				}
				case C_OP_UNARY:
				case C_OP_INFIX: {
					/////////////////////////////////////////////////// 
					soda_t soda;
					memset(&soda, 0, sizeof(soda_t));
					strcpy(soda.token, strKey.c_str());
					soda.item = item;
					if (stkAbra.empty()) {
						stkAbra.push(soda);
					}
					else {
						soda_t top;
						top = stkAbra.top();
						if (top.item.opt < item.opt) {
							stkAbra.push(soda);
						}
						else {
							while (!(stkAbra.empty()))
							{
								top = stkAbra.top();
								if (top.item.cmd == C_ROUND_LEFT) {
									break;
								}
								vecPost.push_back(top);
								stkAbra.pop();
							}
							stkAbra.push(soda);
						}
					}
					/////////////////////////////////////////////////// 
					break;
				}
				case C_ROUND_LEFT: {
					++nRound;
					soda_t soda;
					memset(&soda, 0, sizeof(soda_t));
					strcpy(soda.token, strKey.c_str());
					soda.item = item;
					stkAbra.push(soda);
					break;
				}
				case C_ROUND_RIGHT: {
					if (--nRound < 0) {
						CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "miss matched.");
						return false;
					}
					soda_t top;
					while (!stkAbra.empty()) {
						top = stkAbra.top();
						if (top.item.cmd == C_ROUND_LEFT) {
							stkAbra.pop();
							break;
						}
						vecPost.push_back(top);
						stkAbra.pop();
					}
					break;
				}
			}
		}
	}
	
	/// 괄호 miss match 체크
	if (nRound != 0) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "( or )", "miss matched.");
		return false;
	}

	//// EXIT Check
	if (CheckGrammar(KOCC::C_EXIT) == -1) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Syntax ended abnormally.");
		return false;
	}

	// stkAbra 내 남아있는 연산자 처리
	while (!stkAbra.empty()) {
		soda_t top = stkAbra.top();
		vecPost.push_back(top);
		stkAbra.pop();
	}

	/// Calculate
	stack<soda_t> Stack;
	soda_t data1, data2, data3;
	for(unsigned int i = 0 ; i < vecPost.size() ; i++)
	{
		soda = vecPost[i];
		switch (soda.item.cmd) {
			case C_ALPHA_CHAR:{
				soda.result = m_comp_opt & (1 << soda.item.bit) ? true : false;
				Stack.push(soda);
				break;
			}
			case C_OP_INFIX: {
				data1 = Stack.top();Stack.pop();
				data2 = Stack.top();Stack.pop();

				// 연산자 계산
				switch (soda.item.bit) {
					case OP_AND: 
						data3.result = data1.result && data2.result;
						break;
					case OP_OR: 
						data3.result = data1.result || data2.result;
						break;
					case OP_XOR:
						data3.result = data1.result == data2.result;
						break;
				}
				Stack.push(data3);
				break;
			}
			case C_OP_UNARY: {
				data1 = Stack.top(); Stack.pop();
				data3.result = !data1.result;
				Stack.push(data3);
				break;
			}
		}
	}

	if (Stack.size() != 1) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "SW bug.");
		return false;
	}

	m_stackEnComp.push_back(m_bEnComp);
	m_bEnComp = (bPassTrue == Stack.top().result ? true : false);

	return true;
}

bool CPGC::InsertComp()
{
	string strFile = m_strLine.substr(m_pos);
	while (strFile.size() != 0 && isspace(strFile[0])) strFile.erase(0, 1);
	if (strFile != "") {
		if(LoadFile(strFile)) {
			if(DoCompile() == -1) return false;
		}
	}
	return true;
}

bool CPGC::BeginPattComp(int PatType)
{
	int nType;
	patinfo info;
	char* endptr;
	//memset(info.data, 0, sizeof(patinfo));
	info.start = m_pc;
	info.ntype = PatType;

	//string strKey = GetKey(nType);
	string strKey = GetName(nType);
	switch (nType)
	{
		case KEY_XINT:
			//strncpy(info.name, strKey.c_str(), strKey.size());
			info.name = strKey;
			info.start = m_pc = strtol(strKey.c_str(), &endptr, 16);
			break;
		case KEY_INT:
			//strncpy(info.name, strKey.c_str(), strKey.size());
			info.name = strKey;
			info.start = m_pc = strtol(strKey.c_str(), &endptr, 10);
			break;
		case KEY_STR:
		{
			if (strKey.size()) {
				//if (strKey.length() > MAX_NAME_LENGTH) {
				//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Pattern Name length is over.(MAX 39)");
				//	return false;
				//}
				//strncpy(info.name, strKey.c_str(), strKey.size());
				info.name = strKey;
				info.start = m_pc;
			}
			else {
				info.start = m_pc;
			}
			break;
		}

		default: {
			if(nType == KEY_NULL)
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Pattern name or pc is empty.");
			else
				CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
			return false;
		}

	}

	if (strKey.size()) {
		//if (strKey.length() > MAX_NAME_LENGTH) {
		//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Pattern Name length is over.(MAX 39)");
		//	return false;
		//}
		//strncpy(info.name, strKey.c_str(), strKey.size());
		info.name = strKey;
		info.start = m_pc;
	}
	else {
		info.start = m_pc;
	}

	for (uint16_t idx = 0; idx < m_vecPatInfo.size(); idx++) {
		if (m_vecPatInfo[idx].name == info.name) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Pattern name is duplicated.");
			return false;
		}
	}

	m_vecPatInfo.push_back(info);

	// 추가 키워드가 있는지 확인 
	strKey = GetKey(nType);
	if (strKey.length() != 0) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, " is invalide syntax.");
		return false;
	}

	m_PatName = info.name;
	m_PatComp.SetName(m_PatName);

	return true;
}

bool CPGC::LabelPattComp(string& strKey)
{
	if (strKey.length() < 2) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Label Name is nothing.");
		return false;
	}

	//if (strKey.length() > MAX_NAME_LENGTH + 1) {
	//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Label Name length is over.(MAX 39)");
	//	return false;
	//}

	size_t pos = m_strLine.find(strKey);
	m_strLine.erase(pos, strKey.length());
	m_strLine.insert(pos, strKey.length(), ' ');

	pos = strKey.find(':');
	strKey.erase(pos, 1);

	labelinfo info;
	//memset(info, 0, sizeof(labelinfo));
	info.pc = 0;
	//strncpy(info.name, strKey.c_str(), strKey.length());
	info.name = m_PatName + "." + strKey;
	info.pc = m_pc;
	for (unsigned int i = 0; i < m_vecLabelInfo.size(); i++) {
		if (m_vecLabelInfo[i].name == info.name) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, strKey, "Already defined label name.");
			return false;
		}
	}
	m_vecLabelInfo.push_back(info);

	return true;
}

// return value : > 0 : Pass(opt value)
// return value :  -1 : Fail
int CPGC::CheckGrammar(unsigned int cmd)
{
	grammar_collage::iterator it = m_grammar_collage.find("%IF");
	if (it == m_grammar_collage.end()) 
		return -1;

	grammar_tree& grtree = it->second;
	int opt = 0;

	if (m_Index != -1 && it->second.size() > static_cast<unsigned int>(m_Index)) {

		// CMD Find
		for(unsigned int i = 0 ; i < grtree[m_Index].size(); i++) {
			if (grtree[m_Index][i].cmd == cmd) {
				opt = grtree[m_Index][i].opt;
				m_Index = grtree[m_Index][i].bit;
				return opt;
			}
		}
	}
	return -1;
}

void CPGC::Dump(string filename)
{

	//// 파일이름이 없는경우  pattern 파일이름에서 확장자만 변경
	if (filename == "") 
		filename = m_strFirstFilePath + "/" + m_strFirstFileName;

	size_t pos = filename.rfind('.');
	if (pos != std::string::npos)
		filename.erase(pos);

	filename += ".dump";

	///// File Open 
	ofstream ofile;
	ofile.open(filename);
	if (ofile.is_open() == false) {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, filename, "Dump file open fail.");
		return;
	}

	/////// Dump Register
	m_RegComp.Dump(ofile);
	//if (size < 0) {
	//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "File (register) write fail.");
	//	return -1;
	//}

	/////// Dump Pattern
	m_PatComp.Dump(ofile);
	//if (size < 0) {
	//	CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "File (pattern) write fail.");
	//	return -1;
	//}

	////// File Close
	ofile.close();

	//return 0;
}
