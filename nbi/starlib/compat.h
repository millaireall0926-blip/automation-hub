#ifndef _COMPAT_H_
#define _COMPAT_H_

#define LIB_NAME	"StarLib"
#define LIB_VERSION	"1.00"
#define LIB_RELDATE	"2023/06/13"
#define LIB_AUTHOR	"uiop"
#define LIB_VER		LIB_NAME " " LIB_VERSION " " LIB_RELDATE " by " LIB_AUTHOR

//version history macros

#define VH_START static char* g_version_info = "%File-Version:" LIB_VERSION "%"; static char *g_version_history = "%File-History:\n" 
#define HEADER(x) "\n" #x "\n"
#define L(x) "\t" #x "\n"
#define VH_END "%";
#define VH_STATIC printf("%s%s",g_version_info,g_version_history);

#define VH_COMPILED "Compiled on " __DATE__ "\n"

VH_START

VH_COMPILED
/*
* HEADER("v1.00 2023/6/13")
	L("mod : organize source ") 
* 
HEADER("v1.00 2021/12/15")
	L("add: first ") 
	
*/
VH_END


#undef VH_START 
#undef VH_COMPILED 
#undef HEADER 
#undef L 
#undef VH_END

typedef unsigned short word;
typedef const char cchar;

#ifdef _WIN32

#define   WIN32_LEAN_AND_MEAN		
#include <windows.h>
#include <comdef.h>
#include <atlbase.h>

#define ATT_PACK 
typedef unsigned __int64 uint64;
#define snprintf _snprintf

#else

typedef unsigned long long uint64;
#define stricmp	strcasecmp
#define _vsnprintf	vsnprintf

#endif

#endif 

