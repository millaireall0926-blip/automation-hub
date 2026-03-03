#pragma once

#include <string>
#include <stdarg.h>

#include <vector>
//#include <List>
#include <algorithm>
#include <memory>

using namespace std;

class cstring : public string
{
public:
	cstring() {}
	cstring(char* s) : std::string(s) {}
	cstring(char* s, char* e) : std::string(s, e) {}
	cstring(const char* s) : std::string(s) {}
	cstring(const char* s, int n) : std::string(s, n) {}
	cstring(const char* s, int p, int n) : std::string(s, p, n) {}
	cstring(std::string s) : std::string(s) {}
	cstring(int n1, int n2): std::string(n1, n2) {}

public:
	operator const char*() { return c_str(); }
	operator const unsigned char*() { return (unsigned char*)c_str(); }
	operator char*() { return (char*)c_str(); }
	operator unsigned char*() { return (unsigned char*)c_str(); }
	operator void*() { return (void*)c_str(); }
	operator const char* () const throw() { return c_str(); }

	char & operator[](unsigned int i) { return (*(std::string *)this)[i]; }
	char & operator[](int i) { return (*(std::string *)this)[i]; }

	cstring MakeUpper() 
	{ 
		std::transform(this->begin(), this->end(), this->begin(), ::toupper);
		return *this;
	}

	cstring MakeLower() 
	{ 
		std::transform(this->begin(), this->end(), this->begin(), ::tolower);
		return *this;
	}

	void Format(const char* format, ...)
	{

		va_list args;
		va_start(args, format);
		size_t size = vsnprintf( nullptr, 0, format, args) + 1; 
		va_end(args);

		va_start(args, format);
		unique_ptr<char[]> buf( new char[ size ] ); 
		vsnprintf( buf.get(), size, format, args);
		*this = string(buf.get(), buf.get() + size ); // remove '\0'	

//		printf("foramt (%d) : %s \n", this->GetLength(), this->c_str());
		va_end(args);

	}

	void AppendFormat(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		size_t size = vsnprintf(nullptr, 0, format, args) + 1;
		va_end(args);

		va_start(args, format);
		unique_ptr<char[]> buf(new char[size]);
		vsnprintf(buf.get(), size, format, args);
		this->insert(this->end() - 1, buf.get(), buf.get() + size - 1); // remove '\0'

		va_end(args);
	}


	int GetLength()
	{
		return this->size();
	}

	cstring Mid(int n)
	{
		return this->substr(n);
	}

	cstring Mid(int n, int c)
	{
		return this->substr(n, c); 
	}

	cstring Left(int c)
	{
		return this->substr(0, c);
	}

	cstring Right(int c)
	{
		return this->substr(GetLength() - c);
	}

	cstring TrimLeft()
	{
		cstring s = *this;
		size_t startpos = s.find_first_not_of(" \n\r\t");
		return (startpos == std::string::npos) ? "" : s.substr(startpos);
	}

	cstring TrimRight()
	{
		cstring s = *this;
		size_t endpos = s.find_last_not_of(" \n\r\t");
		return (endpos == std::string::npos) ? "" : s.substr(0, endpos+1);
	}

	cstring Trim()
	{
		return TrimRight().TrimLeft();
	}

	int Split(vector<cstring>&a, cstring delim)
	{
		int i = 0;
		for (size_t start = this->find_first_not_of(delim), end = start; start != string::npos; i++) {
			end = this->find_first_of(delim, start);
			a.push_back(this->substr(start, end-start));
			start = this->find_first_not_of(delim, end);
		}
		return i;
	}

	int Replace(cstring search, cstring replace)
	{
		int n = 0;
		for (size_t pos = 0; (pos = this->find(search.c_str(), pos)) != string::npos; pos += replace.length(), n++)
			this->replace(pos, search.length(), replace.c_str());
		return n;
	}

	bool CompareNoCase(cstring s)
	{
		size_t sz = s.size();
		if (this->size() != sz)
			return false;
		for (size_t i = 0; i < sz; i++)
			if (tolower((*this).at(i)) != tolower(s.at(i)) )
				return false;
		
		return true;
	}

	cstring ExpandTab(int tab_sz)
	{
		char* p = *this;
		int col = 0;
		cstring s = "";

		while (*p) {
			if (*p == '\t') {
				for (int tab_exp = (col % tab_sz != 0) ? tab_sz - (col % tab_sz) : tab_sz; tab_exp != 0; tab_exp--) {
					col++;
					s += " ";
				}
			}
			else {
				s += *p;
				col++;
			}
			p++;
		}
		*this = s;
		return *this;
	}

};
