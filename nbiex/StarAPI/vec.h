#ifndef __VECTORMEM__
#define __VECTORMEM__
#pragma once

#include <cstdio>
#include "define.h"

#define VEC_MAX (1024 * 32)


class vec
{
public:
	vec();
	~vec();

	int		m_nMode; // text, Bin
	FILE*	m_fpVec;
	int		m_nFileSize;
	int		m_nFileCur;
	int     m_nRemind;
	bool	m_bRemind; // 1 line Remind Text Write
	bool	m_bDone;

	

	bool loadFile(const char* name, int nMode); // 0=BIN, 1=Text;
	bool setBinary(int& idxCnt); //return  -1 = error, 0 = File Remind, 1= File Done
	int  setBuf(int ofs, int& idxCnt, int* pData); 
	bool setText(int& idxCnt);

	void fillBuffer(int& posH, int& posL, uint value);
	bool  writeBuffer(int sz);

	uint getValue(char* p);

	word	m_nBufH[VEC_MAX + 0x10];
	word	m_nBufL[VEC_MAX + 0x10];
	word	m_nBufRem[VEC_MAX + 0x10];
};


#endif
