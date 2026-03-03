#pragma once
using namespace std;

class CRegisterCompiler
{
public:
	CRegisterCompiler();
	~CRegisterCompiler();
	bool Compile(std::string strLine);
	bool IsKeyword(std::string strKey);
	string GetISPName();
	void SetISP(unsigned int pc);
	int WriteFile(ofstream& ofile);
	void Dump(ofstream& ofile);

private:
	void CreateKeyword();
	void GenerateGrammar();
	int CheckGrammar(unsigned int cmd);

	string GetKey(int& nType);

	// 개별 컴파일 함수
	int RegComp();
	int TimeComp();
	int ISPComp();
	int ModeComp();	

private:
	SyntaxMap m_KeywordList;

	string m_strLine;

	// 문법 체계를 담고있는 구조체
	grammar_collage m_grammar_collage; 

	// 선택된 grammar tree
	grammar_tree * m_pgrtree;

	// grammar check index
	int m_Index;

	// string index
	size_t m_pos;

	// register index
	int m_RegIndex;
	// register size
	int m_RegSize;

	// register memory map
	registerset_t m_regset;

	// ISP Label name
	string m_strISP;

	// Timer 관련 변수
	double m_RefTimeDiv_0, m_RefTimeDiv_1;
	unsigned int m_TimeResBit; // 0: nsec, 1: usec, 2: msec, 3: sec
};

