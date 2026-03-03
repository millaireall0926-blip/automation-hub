#pragma once

#include "String.h"

#include <cstring>
#include <stack>
using namespace std;

/*
	scanner function

	- skip all the block comment
	- skip line comment

	- load() will fill the internal buffer from file
	- setup() will fill the internal buffer with another buffer
	- get() returns current token and advance buffer pointer
	- get(true) returns current token and do not advance buffer pointer
	- getLIne() returns current line, col for handling compiler messages

*/


#define CR (0x0D)		// skip char
#define LF (0x0A)		// dos/linux compatible line end
#define MLF (0x22)		// " if mode is macro, then check this value end of line.
#define TAB_SIZE 4

enum token_enum {
	TOK_EOF		= 0,	// end of file indicator
	TOK_EOL,			// end of line indicator
	TOK_NUM,			// binary, hex, decimal number
	TOK_ID,				// identifier that leading alphabet, and then alphabet or number follows
						// i.e.) "A380", "HELLO", "_START"
	TOK_STR,			// string that enclosed by quot or double quot
	TOK_PUNC,			// token is puncture character include double operator
 						// i.e.) "++" ,"+=", "*", "^", "=",  etc
	TOK_HEX,			// temporary token id, it will be converted as TOK_NUM
	TOK_BIN,			// temporary token id, it will be converted as TOK_NUM
	TOK_ERROR	= -1		// error during parse text
};

typedef unsigned char byte;
typedef unsigned int dword;

typedef struct tokenStruct {
	token_enum	t;		// token type
	cstring		s;		// string value
	dword		n;		// numeric value
	int			col;	// column position of this token
} token_t;

typedef struct scannerPushStruct {
	byte*		pBuf;	// text buffer
	byte*		p;		// current pointer
	int			nLine;	// current line
	cstring		sFn;	// filename
	size_t		nTokenIndex;
	vector<token_t> aToken;
} push_t;

class CScanner
{
public:
	CScanner(void);
	virtual ~CScanner(void);

public:
	bool		setup(const char* src);
	bool		load(const char* fn);

	cstring&	gets(bool bPeek = false) { return get(bPeek).s; } //token_t t = get(bPeek).; return t.s; }
	token_enum	gett(bool bPeek = false) { return get(bPeek).t; } //token_t t = get(bPeek); return t.t; }
	dword		getn(bool bPeek = false) { return get(bPeek).n; } // token_t t = get(bPeek); return t.n; }
	token_t&	get(bool bPeek = false);
	token_t&	peek() { return get(true); }
	int			getLine() { return m_nLine; }
	void		setLine(int nline) { m_nLine = nline; }
	cstring&	getFN() { return m_sFN; }
	bool		setFN(const char* fn);

	byte*		getBufPos() { return m_p; }
	void		setBufPos(byte* p ) { m_p = p; }
	byte*		findKeyword(cstring s);

	// scanner가 include를 통해 여러 개의 source를 컴파일할 수 있도록 현재 버퍼 상태를 보존하는 방법 제공
	// 단, 이 때 token 정보는 보전하지 않음.
	bool		push();
	bool		pop();

	vector<token_t> m_aToken;

private:
	cstring		m_sFN;				// filename
	size_t		m_nTokenIndex;		// current token index of token array
	int			m_nLine;			// current line
	byte*		m_buf;				// source buffer
	size_t		m_nLength;			// total length of the source
	byte*		m_p;				// current buffer pointer

	void		cleanup();
	bool		isEOF() { return (m_p >= m_buf + m_nLength); }
	bool		parseLine();
	void		addToken(token_enum t, cstring s, int col);

	stack<push_t> m_srcStack;
};
