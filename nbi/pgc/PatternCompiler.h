#pragma once

using namespace std;

class CPatternCompiler
{
public:
	CPatternCompiler();
	~CPatternCompiler();
	bool Compile(string strLine);
	void AddSTPS();
	void SetName(string name);
	int PostComp(vector<patinfo>& PatInfo, vector<labelinfo> LabelInfo);
	bool IsKeyword(string & strKey);
	bool IsOpcode(string& strKey);
	int WriteFile(ofstream& ofile);
	void Dump(ofstream& ofile);
	int GetLastOpcode();

private:
	string GetName(int& pType);
	string GetKey(int& nType);
	string GetFirstKey();
	int ExitCheck(string strKey = "");

	void CreateKeyword();
	void CreateFirstKeyword();
	void CreateOpcodeKeyword();
	void CreateXBKeyword();
	void CreateXNKeyword();
	void CreateYBKeyword();
	void CreateYNKeyword();
	void CreateD3Keyword();
	void CreateD4Keyword();
	void CreateZKeyword();

	void CreateXKeyword();
	void CreateYKeyword();

	void CreateTP1Keyword();
	void CreateTP2Keyword();

	void CreateD7Keyword();
	void CreateD8Keyword();

	void CreateSAKeyword();
	void CreateSBKeyword();

	void CreateSKeyword();
	void CreateRFKeyword();
	void CreateDXKeyword();
	void CreateTPHXKeyword();

	void GenerateGrammar();
	//void GenerateOpcodeGrammar();
	void GenerateXBGrammar();
	void GenerateXNGrammar();
	void GenerateYBGrammar();
	void GenerateYNGrammar();
	void GenerateD3Grammar();
	void GenerateD4Grammar();
	void GenerateZGrammar();

	void GenerateTP1Grammar();
	void GenerateTP2Grammar();

	void GenerateD7Grammar();
	void GenerateD8Grammar();

	void GenerateSAGrammar();
	void GenerateSBGrammar();
	void GenerateDXGrammar();
	void GenerateTPHXGrammar();

	int CheckGrammar(unsigned int cmd);

	int OpcodeComp(int nOPType);
	int XBComp();
	int XNComp();
	int YBComp();
	int YNComp();
	int D3Comp();
	int D4Comp();
	int ZComp();
	int XComp();
	int YComp();

	int TP1Comp();
	int TP2Comp();
	int D7Comp();
	int D8Comp();

	int SAComp();
	int SBComp();

	int SComp();
	int RFComp();

	int ImmComp();
	int TPXComp();

	int D1Comp();
	int D2Comp();

	int TPH1Comp();
	int TPH2Comp();

private:
	SyntaxMap m_FirstKeyword;
	vector<string> m_OpcodeKeyword;
	map<string, SyntaxMap> m_KeywordMapList;
	SyntaxMap m_SetKeyword;

	string m_strName;
	string m_strLine;

	vector<string> m_BufKey;

	int m_pc;

	double m_RefTimeDiv_0, m_RefTimeDiv_1, m_RefTimeDiv_2;

	// grammar check index
	int m_Index;

	// string index
	size_t m_pos;

	// 문법 체계를 담고있는 구조체
	grammar_collage m_grammar_collage;

	// 선택된 grammar tree
	grammar_tree* m_pgrtree;

	instruction_t m_inst;
	vector<instruction_t>  m_InstList;

	// Label을 타겟으로 jump opcode를 사용한경우 리스팅한다.
	vector<pair<unsigned int, string>> m_LabelJumpPC;

	int m_ImmIdx;
	int m_ImmAlu;

	int m_DxIdx;

	int m_PrevOpcode;

	// 중복 설정 확인용;
	usedinfo_t m_Used; 

	string m_strOrgKey;

	int m_nMode;
};

