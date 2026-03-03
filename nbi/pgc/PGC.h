#pragma once

#include "FileScanner.h"
#include "RegisterCompiler.h"
#include "PatternCompiler.h"

//typedef std::vector<std::vector<ITEM>> grammar_tree;
//typedef std::map<std::string, grammar_tree> grammar_collage;

class CPGC
{
public:
	CPGC(string file = "", int opt = -1);
	~CPGC();
	bool LoadFile(std::string strFile);
	int DoCompile(int comp_opt = -1);
	int PostCompile();
	int WriteFile(string filename = "");
	bool IsLoaded() { return m_bLoadSucceed; };
	void DefineApply();

	void Dump(string filename);

private:
	void CreateKeyword();
	void GenerateGrammar();
	int CheckGrammar(unsigned int cmd);
	
	string GetFirstKey();
	string GetKey(int & pType);
	string GetName(int& pType);
	string GetDefineName();

	bool Compile(string & strKey, ITEM &item);
	// 개별 컴파일 함수
	bool DefineComp();
	bool IFComp(bool bPassTrue);	
	bool InsertComp();
	bool BeginPattComp(int PatType);
	bool LabelPattComp(string & strKey);

	SyntaxMap m_KeywordList;
	
	// 첫번째 파일 Name
	string m_strFirstFileName;
	// 첫번째 파일 Path
	string m_strFirstFilePath;

	// File Load Succeed
	bool m_bLoadSucceed;

	// Pattern root path
	std::string m_RootPath;

	CFileScanner * m_pFScan;
	// 현재 컴파일 중인 line 의 text
	std::string m_strLine;
	// m_strLine 에서의 위치
	size_t m_pos; 

	// Define으로 정의된 내용 모음
	std::map<std::string, std::string> m_mapDefine; 

	// 입력 받은 컴파일 옵션 저장
	unsigned int m_comp_opt;
	// 현재 컴파일 진행여부 : %IFE or %IFN 에 의해 결정된다.(Defaul:true)
	bool m_bEnComp; 
	// %IFE or %IFN 컴파일 결과인 m_bEnComp의 값을 stack에 저장 한다.
	std::vector<bool> m_stackEnComp; 

	std::vector<CFileScanner*> m_vecFileScanner;

	// 문법 체계를 담고있는 구조체
	grammar_collage m_grammar_collage; 

	// grammar check index
	int m_Index;

	// 현재 상태를 나타낸다. Common, Register, Pattern 중 하나
	int m_Comp_Mode;

	// pc 값
	unsigned int m_pc;

	// Pattern 정보 
	vector<patinfo> m_vecPatInfo;

	// Label 정보
	vector<labelinfo> m_vecLabelInfo;

	CRegisterCompiler m_RegComp;
	CPatternCompiler m_PatComp;

	string m_PatName;
};

