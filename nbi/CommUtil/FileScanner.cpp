
#include <string>
#include <stdio.h>
#include <iostream>
#include <filesystem>

#include <vector>
#include "ErrorLog.h"
#include "FileScanner.h"

CFileScanner::CFileScanner()
{
	m_strFile = "";
	m_strPath = "";
	m_strLine = "";
	m_bInComment = false;
	m_nLine = 0;
}

CFileScanner::~CFileScanner()
{
	Close();
}

bool CFileScanner::Open(std::string& file)
{
	std::filesystem::path curPath(file);
	if (std::filesystem::exists(curPath)) {
		m_strPath = curPath.parent_path().string();
		m_strFile = curPath.filename().string();
		m_inFile.open(file);
		std::filesystem::current_path(m_strPath);
		if (!m_inFile.is_open()) {
			CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, file, "File open fail.");
			return false;
		}
	}
	else {
		CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, file, "File not exist.");
		return false;
	}
	return true;
}

bool CFileScanner::Close()
{
	if( m_inFile.is_open() )
		m_inFile.close();

	return true;
}

int CFileScanner::GerLineNumber()
{
	return m_nLine;
}

std::string CFileScanner::GetPath()
{
	return m_strPath;
}

std::string CFileScanner::GetFileName()
{
	return m_strFile;
}

bool CFileScanner::EraseComment(std::string& str)
{
	std::string::size_type ns; // "/*" 위치
	std::string::size_type ne; // "//" 위치 or "*/" 위치

	ns = str.find("/*");
	ne = std::min(str.find("//"), str.find(";"));

	// 라인주석 블럭주석 키워드가 둘 다 있는경우 먼저 나온 키워드 확인
	if (ne != std::string::npos && ns != std::string::npos) {
		if (ne < ns)
			ns = std::string::npos;
		else
			ne = std::string::npos;
	}

	// 라인주석의 경우 
	if (ne != std::string::npos) {
		str.erase(ne); // 처리 종료
	}
	// 블럭주석의 경우
	else if (ns != std::string::npos) {
		ne = str.find("*/", ns + 2);

		if (ne < ns) return false; // "*/" miss-matched

		// 현재 라인에 블럭주석 end 가 있는경우
		if (ne != std::string::npos) {
			str.erase(ns, ne - ns + 2);
			return EraseComment(str);
		}
		else {
			str.erase(ns);
			m_bInComment = true; // 처리 종료
		}
	}

	return true;
}

int  CFileScanner::ReadLine(std::string& str)
{
	if (m_inFile.eof()) return -1;

	std::string strBuf;
	std::string::size_type nPos;
	int len = 0;

	str = "";

	while (m_inFile.eof() == false ) {
		std::getline(m_inFile, strBuf);

		m_nLine++;

		if (strBuf == "") continue;

		while (isspace(strBuf[0])) strBuf.erase(0, 1);

		// Carrage Return 제거
		nPos = strBuf.find('\r');
		if (nPos != std::string::npos) strBuf.erase(nPos, 1);

		// 현재 블럭주석 상태인 경우
		if (m_bInComment) {
			//블럭주석 end(*/) 가 있는경우
			if ((nPos = strBuf.find("*/")) != std::string::npos) {
				strBuf.erase(0, nPos + 2);
				m_bInComment = false;
				while (isspace(strBuf[0])) strBuf.erase(0, 1);
			}
			else {
				// 현재 라인 제거
				strBuf.erase(0);
				continue;
			}
		}

		// 주석 제거
		if (EraseComment(strBuf) == false)
			std::clog << "error */ miss-matched" << std::endl;

		while (isspace(strBuf[0])) strBuf.erase(0, 1);

		// '\'이 있는경우 다음 라인 까지 하나의 라인으로 인식한다.
		len = (int)strBuf.length();
		if (len > 0 && strBuf[len - 1] == '\\') {
			strBuf.erase(len - 1);
			str += strBuf;
		}
		else{
			str += strBuf;
			break;
		}
	}

	return 0;
}
