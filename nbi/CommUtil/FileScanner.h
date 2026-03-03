#pragma once
#include <fstream>

class CFileScanner
{
public:
	CFileScanner();
	CFileScanner(const char* file);
	~CFileScanner();

	bool Open(std::string & file);
	bool Close();
	int GerLineNumber();

	std::string GetPath();
	std::string GetFileName();

	bool EraseComment(std::string& str);
	int  ReadLine(std::string& str);

private:
	std::string m_strFile;
	std::string m_strPath;
	std::string m_strLine;

	std::ifstream m_inFile;

	bool m_bInComment;

	int m_nLine;
};

