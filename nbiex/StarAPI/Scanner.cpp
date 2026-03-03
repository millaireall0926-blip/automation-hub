#include "Scanner.h"

CScanner::CScanner(void)
{
	m_buf = NULL;
	cleanup();
}

CScanner::~CScanner(void)
{
	cleanup();
}

void CScanner::cleanup()
{
	if (m_buf) {
		delete m_buf;
		m_buf = NULL;
	}

	m_nLine = 0;
	m_nTokenIndex = 0;

	m_aToken.clear();
}

bool CScanner::setup(const char* src)
{
	m_nLength = strlen(src);

	cleanup();
	m_buf = new byte[m_nLength + 10];
	if (m_buf == NULL)
		return false;

	memcpy(m_buf, src, m_nLength);
	m_p = m_buf;
	return true;
}

bool CScanner::load(const char* fn)
{
	cleanup();
	setFN(fn);

	FILE* f = NULL;
	if ((f = fopen(fn, "rb")) == 0) {  
		cstring s;
		s.Format("can't open file '%s'\n", fn);
		addToken(TOK_ERROR, s, 0);
		addToken(TOK_ERROR, "", 0);
		return false;
	}
	
	fseek(f, 0L, SEEK_END);
	m_nLength = ftell(f);
	m_buf = new byte[m_nLength + 10];
	if (m_buf == NULL) {
		fclose(f);
		return false;
	}

	fseek(f, 0L, SEEK_SET);
	if (fread(m_buf, m_nLength, 1, f) != 1) {
		cstring s;
		s.Format("can't read file '%s'\n", fn);
		addToken(TOK_ERROR, s, 0);
		fclose(f);
		return false;
	}
	fclose(f);
	m_p = m_buf;
	return true;
}

token_t& CScanner::get(bool bPeek)
{
	if (m_nTokenIndex >= m_aToken.size()) 
		parseLine();
	
	if (bPeek)
		return m_aToken[m_nTokenIndex];
	else
		return m_aToken[m_nTokenIndex++];
}

void CScanner::addToken(token_enum t, cstring s, int col)
{
	token_t tok;
	char *p, *q;

	tok.col = col;
	tok.s = s;
	tok.t = t;
	if (t == TOK_HEX) {
		p = s;
		if(*p == '$')
			p++;				// $ 처리를 이상하게 함.
		q = p + s.size();
		tok.n = strtoul(p, &q, 16);
		tok.t = TOK_NUM;
	}
	else if (t == TOK_BIN) {
		p = s;
		q = p + s.size();
		tok.n = strtoul(p, &q, 2);
		tok.t = TOK_NUM;
	}
	else if (t == TOK_NUM) {
		p = s;
		q = p + s.size();
		tok.n = strtoul(p, &q, 10);
		tok.t = TOK_NUM;
	}
	m_aToken.push_back(tok);
}

bool CScanner::parseLine()
{
	static int bInCommentBlock = false;
	cstring line, s;

	// skip comment block across multiple lines
	if (bInCommentBlock) {
		while (bInCommentBlock) {
			while (!isEOF() && *m_p != LF )
				line += *m_p++;
			m_p++;
			m_nLine++;

			int n = line.find("*/");
			if (n != (int)string::npos) {
				line.erase(0, n+2);
				bInCommentBlock = false;
				break;
			}
			line = "";
		}
	} 
	else {
		while (!isEOF() && *m_p != LF)
			line += *m_p++;
		m_p++;
		m_nLine++;
	}

	// if line has comment block begin mark, remove it
	int n = line.find("/*");
	if (n != (int)string::npos) {
		line.erase(n, s.size()-n+2);
		bInCommentBlock = true;
	}

	// do tab expansion for calculating precise column position
	line.ExpandTab(TAB_SIZE);	
	line = line.TrimRight();

	

	// now start to pase 

	m_aToken.clear();
	m_nTokenIndex = 0;

	byte* p = line;

	int col = 0;
	int colsave = 0;

	while (*p ) {
		byte c = *p;

		if (c == CR) {
			p++;
		}
		else if (c == ' ') {
			col++;
			p++;
		}
		else if (c == '/' && *(p+1) == '/') {	// should not parse after line comment 
			addToken(TOK_EOL, "", 0);
			break;
		}
		else if (c == ';') {					// should not parse after line comment 
			addToken(TOK_EOL, "", 0);
			break;
		}
		else if (
			(c == '=' && *(p+1) == '=') ||
			(c == '!' && *(p+1) == '=') ||
			(c == '>' && *(p+1) == '=') ||
			(c == '<' && *(p+1) == '=') ||
			(c == '&' && *(p+1) == '&') ||
			(c == '|' && *(p+1) == '|') ||
			(c == '+' && *(p+1) == '+') ||
			(c == '-' && *(p+1) == '-') ||
			(c == '+' && *(p+1) == '=') ||
			(c == '-' && *(p+1) == '=') ||
			(c == '<' && *(p+1) == '<') ||
			(c == '>' && *(p+1) == '>') ) {
			colsave = col;
			s += *p++;
			s += *p++;
			col += 2;
			addToken(TOK_PUNC, s, colsave);
		}
		else if (c == '_' || isalpha(c) || c == '~') {
			colsave = col;
			s += *p++;
			while (isalnum(*p) || *p == '_') {
				s += *p++;
				col++;
			}
			s.MakeUpper();					// id is case insensitive!
			addToken(TOK_ID, s, colsave);
			s = "";
		}
		else if (c == '$' || (c == '0' && toupper(*(p+1)) == 'X')) {	// hex digits
			colsave = col;
			s += *p++;
			s += *p++;
			col += 2;
			while (isxdigit(*p) || *p == '`') {
				if (isxdigit(*p))
					s += *p;
				p++;
				col++;
			}
			addToken(TOK_HEX, s, colsave);
			s = "";
		}
		else if (c == '0' && toupper(*(p+1)) == 'B') {				// binary digits
			colsave = col;
			s += *p++;
			s += *p++;
			col += 2;
			while (*p == '1' || *p == '0' || *p =='`') {
				if (*p == '1' || *p == '0')
					s += *p;
				p++;
				col++;
			}
			addToken(TOK_BIN, s, colsave);
			s = "";
		}
		else if (isdigit(c)) {	// number
			colsave = col;
			s += *p++;
			while (isdigit(*p) || *p == '`') {
				if (isdigit(*p))
					s += *p;
				p++;
				col++;
			}
			addToken(TOK_NUM, s, colsave);
			s = "";
		}
		else if (c == '\'' || c == '\"') {		// string
			char lastch = c;
			colsave = col;
			p++;
			bool bOk = false;
			while (*p) {
				if ( *p == '\\' && (*(p+1) == '\'' || *(p+1) == '\"')) {
					s += *(p+1);
					p += 2;
					col += 2;
				}
				else if (*p == lastch) {
					p++;
					col++;
					bOk = true;
					break;
				}
				else {
					s += *p++;
					col++;
				}
				// TODO: 여러 줄에 걸쳐 있는 string의 처리.
			}

			if (!*p && !bOk) {
				addToken(TOK_ERROR, "unexpected line end with incomplete strings", 0);
				return false;
			}
			addToken(TOK_STR, s, colsave);
		}
		else if (ispunct(c)) {			// otherwise
			colsave = col;
			s += *p++;
			col++;
			addToken(TOK_PUNC, s, colsave);
			s = "";
		}
		else {							// maybe it's korean or UTF-8 characters. it's not allowed, only allowed in the comments
			addToken(TOK_ERROR, "invalid character found in this line", 0);
			return false;
		}
	}

	addToken(TOK_EOL, "", 0);

	if (isEOF()) 
		addToken(TOK_EOF, "", 0);

	return true;
}

byte* CScanner::findKeyword(cstring s)
{
	return (byte*)strstr((const char*)m_p, s);
}

bool CScanner::push()
{
	push_t pt;
	
	pt.pBuf = new byte[m_nLength+10];

	if (pt.pBuf == NULL)
		return false;

	memcpy(pt.pBuf, m_buf, m_nLength+1);

	for(size_t i=0; i<m_aToken.size(); i++)
		pt.aToken.push_back(m_aToken[i]);
	
	byte* p = pt.pBuf;
	size_t nLength = strlen((char*)p);
	size_t nCurrentLength = strlen((char*)m_p);
	pt.p = (p + (nLength-nCurrentLength));
	pt.nLine = m_nLine;
	pt.sFn = m_sFN;
	pt.nTokenIndex = m_nTokenIndex;

	m_srcStack.push(pt);

	return true;
}

bool CScanner::pop()
{
	if (m_srcStack.empty())
		return false;

	push_t pt = m_srcStack.top();
	m_srcStack.pop();

	if (pt.pBuf ==NULL)
		return false;

	cleanup();		// Delete worked scanner and reset.

	for(size_t i=0; i<pt.aToken.size(); i++)
		m_aToken.push_back(pt.aToken[i]);

	m_buf = pt.pBuf;
	m_nLength = strlen((char*)m_buf);
	m_sFN = pt.sFn;
	m_p = pt.p;
	m_nLine = pt.nLine;
	m_nTokenIndex = pt.nTokenIndex;

	return true;
}


bool CScanner::setFN(const char* fn)
{
	m_sFN = fn;
	return true;
}