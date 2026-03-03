/*

	function collection. 
	c-interface(gstlib.*) and gc-interface(cs-def.cpp) use this file as low level interface

*/

#include "function.h"
#include <ctype.h>


#include "compat.h"
#include "cs_string.h"
#include "cs_array.h"

#ifdef _WIN32 
	#include <conio.h>
	#include <windows.h>
	#include <winioctl.h>
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <errno.h>
	#include <stdarg.h>
	#include <unistd.h>
	#define __USE_LINUX_IOCTL_DEFS
	#include <sys/ioctl.h>
	#include <fcntl.h>
	#include <termio.h>
	#include <sys/time.h>
	#include <syslog.h>
	#include <sys/mman.h> 

	#include <sys/types.h>
	#include <sys/stat.h>

	#define DEVNAME "/dev/" PIO_DEV_NAME

#endif

#ifndef _WIN32
typedef unsigned int HANDLE;
typedef unsigned int DWORD;

#endif

static char* starlib_file_version = "%File-Version:" LIB_VERSION "%";

void printInfo(int e, cchar *fmt, ...)
{
	va_list	ap;
	char dt[20], buf[1024]={0};
	time_t t = time(NULL);
	struct tm *tp = localtime(&t);

	strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", tp);

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf)-1, fmt, ap);
	va_end(ap);

#ifdef _WIN32
	char str[1024] = {0};
	snprintf(str, sizeof(str), "%s %s %s", dt, (e) ? "[E]" : "[I]", buf);
	OutputDebugString(str);
#else
	if (e)
		syslog(LOG_ERR, buf);
	else
		syslog(LOG_INFO, buf);
#endif
	printf("%s %s %s", dt, (e) ? "[E]" : "[I]", buf);
	fflush(stdout);
}

#ifndef _WIN32
	int lp_adr = 0;
	int lp_bound = 0;
	int lp_offset = 0;
	int lp_expect = 0;
	int lp_read = 0;
#endif

namespace util {

static char* ver_strings[] = {
	LIB_VER,
	LIB_NAME,
	LIB_VERSION,
	LIB_RELDATE,
	LIB_AUTHOR,
	""
};

char* do_util_version(int n) 
{
	VH_STATIC
	if (!*starlib_file_version)
		return NULL;

	if (n < 0 || n > 4)
		return ver_strings[0];

	printf("STAR Library Version : %s\n", ver_strings[0]);
	return ver_strings[n];
}

#define A2H(x) ((x) >= 'A') ? ((x) - 'A' + 10) : ((x) - '0')

uint64 do_util_vtoi(cchar *s)
{
	uint64 v = 0;
	register int c;
	register int mode = 10;				

	if (*s == '$') {
		mode = 16;
		s++;
	}
	else {
		if (strlen(s) > 2) {
			if ((*s == '0' && toupper(*(s+1)) == 'X')) {
				mode = 16;			// hex
				s += 2;
			}
			else if (*s == '0' && toupper(*(s+1)) == 'B') {
				mode = 1;			// binary
				s += 2;
			}
		}
	}

	for (; *s; s++) {
		if (*s == '`') continue;
		c = toupper(*s);

		if (mode == 16 && isxdigit(c)) {
			v = (v << 4) | (A2H(c));
		}
		else if (mode == 1 && (c == '0' || c == '1')) {
			v = (v << 1) | (c - '0');
		}
		else if (isdigit(c)) {
			v = v * 10 + (c - '0');
		}
		else {
			break;
		}
	}

	return v;
}

void do_util_usleep(int usec)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = usec;
	select(0, NULL, NULL, NULL, &tv);
}

int do_util_getprocid()
{
#ifdef _WIN32
	return GetCurrentProcessId();
#else
	return getpid();
#endif

}

#ifdef _WIN32

DWORD oldtv[16];

void do_util_ptimer_begin(int n)
{
	if (n < 0 || n > 16)
		return;
	oldtv[n] = GetTickCount();
}

double do_util_ptimer_ellapsed(int n) 
{
	if (n < 0 || n > 16)
		return -1.0;

	double ct = (double)GetTickCount();
	double ot = (double)oldtv[n];

	if (ct < ot) {	// 49.7일 마다 발생하는 wrap에 걸렸음
		ct = ct + 4294967295.0 - ot;
	}
	else
		ct -= ot;

	return ct / 1000.0;
}

#else

struct timeval oldtv[16];

void do_util_ptimer_begin(int n)
{
	if (n < 0 || n > 16)
		return;
	struct timezone tz;
	gettimeofday(&oldtv[n], &tz);
}

double do_util_ptimer_ellapsed(int n)
{
	if (n < 0 || n > 16)
		return -1.0;

	struct timezone tz;
	struct timeval tv;
	gettimeofday(&tv, &tz);
	
	double x = tv.tv_sec - oldtv[n].tv_sec;
	double y = tv.tv_usec - oldtv[n].tv_usec;
	
	if (y < 0.0) {
		x--;
		y += 1e6;
	}

	return (x + y / 1e6);
}

#endif

};		// end of namespace util


namespace xml {

cchar* error(HND h)
{
	if (!h)
		return "";

	static char es[256];
	TiXmlDocument* p = (TiXmlDocument*)h;
	sprintf(es, "%s(%d,%d): %s", p->Value(), p->ErrorRow(), p->ErrorCol(), p->ErrorDesc());
	return es;
}

int parse(HND dadr, cchar *xml)
{
	if (!dadr)
		return false;

	TiXmlDocument *doc = (TiXmlDocument*)dadr;
	doc->Parse(xml);
	if ( doc->Error() ) {
		printInfo(1, "xml: (%d,%d) '%s': %s\n", doc->ErrorRow(), doc->ErrorCol(), doc->Value(), doc->ErrorDesc() );
		return false;
	}

	return true;
}

int load(cchar *fn, HND *d)
{
	if (*fn == '\0') {
		printInfo(1, "xml: file name is null\n");
		return false;
	}

	TiXmlDocument* doc = new TiXmlDocument(fn);
	if (doc == NULL) {
		printInfo(1, "xml: create document failed\n");
		return false;
	}

	*d = CHND(HND, doc);
	doc->Clear();


	FILE* f = fopen( fn, "rb" );
	if (!f) {
		printInfo(1, "xml: file '%s' not found\n", fn);
		return false;
	}
	fclose(f);
	if (!doc->LoadFile()) {
		printf( "xml: %s(%d,%d): %s\n", fn, doc->ErrorRow(), doc->ErrorCol(), doc->ErrorDesc());
		return false;
	}

	return true;
}


HND docroot(HND eh)
{
	TiXmlNode *p = ((TiXmlElement *)eh)->Parent();
	return (p == NULL) ? CHND(HND, eh) : root( CHND(HND, p));		// find root recursively
}

HND clone(HND eh)
{
	return (eh) ? CHND(HND, ((TiXmlElement*)eh)->Clone()) : 0;
}

HND insertbefore(HND eh1, HND eh2)
{
	if (!eh1 || !eh2) {
		printInfo(1, "xml: one of handles (%x, %x) is null\n", eh1, eh2);
		return 0;
	}

	TiXmlNode *item = (TiXmlNode *)eh1;
	return CHND(HND, item->Parent()->InsertBeforeChild(item, *((TiXmlElement *)eh2)));
}

HND insertafter(HND eh1, HND eh2)
{
	if (!eh1 || !eh2) {
		printInfo(1, "xml: one of handles (%x, %x) is null\n", eh1, eh2);
		return 0;
	}

	TiXmlNode *item = (TiXmlNode *)eh1;
	return CHND(HND, item->Parent()->InsertAfterChild(item, *((TiXmlElement *)eh2)));
}

int docsrc_len(HND h)
{
	if (!h)
		return 0;

	TiXmlPrinter printer;
	TiXmlDocument* p = (TiXmlDocument*)h;
	if (p->IsCondenseOut())
		printer.SetStreamPrinting();
	p->Accept(&printer);
	return printer.Size();
}

char* docsrc(HND h, char* s)
{
	s[0] =  '\0'; 
	if (!h)
		return 0;

	TiXmlPrinter printer;
	TiXmlDocument* p = (TiXmlDocument*)h;
	if (p->IsCondenseOut())
		printer.SetStreamPrinting();
	p->Accept(&printer);
	strcpy(s, printer.CStr());
	return s;
}

char* source(HND eh, char* s)
{
	s[0] = '\0';
	if (eh) {
		TiXmlElement* p = (TiXmlElement *)eh;
		TiXmlPrinter printer;
		if (p->IsCondenseOut())
			printer.SetStreamPrinting();
		p->Accept(&printer);
		strcpy(s, printer.CStr());
	}
	return s; 
}

int source_len(HND eh)
{
	if (!eh)
		return 0;

	TiXmlPrinter printer;
	TiXmlElement* p = (TiXmlElement *)eh;
	if (p->IsCondenseOut())
		printer.SetStreamPrinting();
	p->Accept(&printer);
	return printer.Size();
}

const char* text(HND eh)			// element text, returns first sibling
{
	TiXmlNode *p = ((TiXmlNode *)eh), *q;
	TiXmlText *r;

	if (p) {
		for (q = p->FirstChild(); q; q = q->NextSibling()) {
			if ((r = q->ToText()))
				return r->Value();
		}
	}

	return "";
}

void set_text(HND eh, cchar *text)
{
	TiXmlNode *p = ((TiXmlNode *)eh), *q;
	TiXmlText *r;

	if (!p)
		return;

	// look for first text child.

	for (q = p->FirstChild(); q; q = q->NextSibling()) {
		if ((r = q->ToText())) {
			r->SetValue(text);
			return;
		}
	}

	// if fail, add text element to this element

	TiXmlText s(text);
	p->InsertEndChild(s);
}

HND path_find(HND eh, cchar *name)
{
	int n, flag = 0;
	char *src = (char*)name;
	char *p, *q;
	char current[100], next[256];
	TiXmlNode *ele = (TiXmlNode *)eh;

	if (eh == 0 || *name == '\0')
		return 0;
	// ignore first char if the *src has '/' and then get next position of '/' (path seperator)

	p = (*src == '/') ? strchr(++src, '/') : strchr(src, '/');
	
	memset(current, 0, sizeof(current));
	if (p == NULL) {				// no more child element
		strcpy(current, src);
		next[0] = '\0';
		flag = 1;
	}
	else {
		if (p - src >= 100)			// boundery check for safe strcpy
			return 0;

		strncpy(current, src, p-src);
		strcpy(next, p+1);
	}

	// look for array index

	p = strchr(current, '[');
	q = strrchr(current, ']');
	if (p && q) {
		n = atoi(p+1);
		*p = '\0';
	}
	else
		n = 0;	

	for (ele = ele->FirstChild(current); n > 0 && ele; ele = ele->NextSibling(current), n--) {
		;
	}

	if (flag)
		return CHND(HND, ele);

	return path_find(CHND(HND,	ele), next);
}

//	/test-data/timing-group/timing-file    : return numberof element in child of timing-file
//	/test-data/timing-group[1]/timing-file    : return number of element in timing-file of second timing-group

int path_n(HND eh, cchar *path)
{
	HND fe = path_find(eh, path);
	TiXmlNode *e = (TiXmlNode *)fe;
	int n;

	if (!e)
		return -1;

	for (e = e->FirstChild(), n = 0; e; e = e->NextSibling(), n++)
		;

	return n;
}


//	/test-data/timing-group[2]/timing-file    : return PCDATA value of timing-file of 3rd timing-group
//	/test-data/timing-group/timing-file    : return PCDATA value of timing-file
//	/test-data/timing-group/timing-file.id : return attribute value of timing-file.id

const char* path_value(HND eh, cchar *path)
{
	char *p, name[256];

	strcpy(name, path);
	if ((p = strrchr(name, '.')) == NULL) {
		return text(path_find(eh, name));
	}

	*p = '\0';
	HND fe = path_find(eh, name);

	if (fe) {

		for (HND a = att_first(fe); a; a = att_next(a)) {
			if (strcmp(att_name(a), p+1) == 0)
				return att_value(a);
		}
	}

	return "";
}

//	/test-data/timing-group/timing-file
//	/test-data/timing-group/timing-file.id=51bi

// attribute 참조는 항상 마지막이어야 한다.

HND path_handle(HND eh, cchar *path)
{
	char name[256], s[256];
	char *p;
	TiXmlNode* ele;

	strcpy(name, path);
	if ((p = strrchr(name, '.')) == NULL)
		return path_find(eh, name);

	*p = '\0';
	strcpy(s, p+1);

	if ((p = strchr(s, '=')) == NULL)
		return 0;

	*p = '\0';
	HND fe = path_find(eh, name);

	if (fe) {
		ele = (TiXmlNode*)fe;
		for (; ele; ele = ele->NextSiblingElement()) {
			for (HND a = att_first(CHND(HND, ele)); a; a = att_next(a)) {
				if (strcmp(att_name(a), s) == 0 && strcmp(att_value(a), p+1) == 0) {
					return CHND(HND, ele);
				}
			}
		}
	}

	return 0;
}

void path_names_r(HND sarr, char *str, HND eh, int level, int olevel)
{
	char name[256] = {0}, oname[256] = {0}, buf[512] = {0};
	int cnt = 0;
	HND i = eh;

	if (!i) return;

	oname[0] = '\0';
	name[0] = '\0';

	for (; i; i = xml::next(i)) {
		snprintf(name, sizeof(name)-1, "%s/%s", str, xml::name(i));
		if (strcmp(name, oname) == 0)
			cnt++;
		else {
			cnt = 0;
		}
		if (cnt > 0) {
			snprintf(buf, sizeof(buf)-1, "%s[%d]", name, cnt);
		}
		else
			snprintf(buf, sizeof(buf)-1, "%s", name);

		((tool::array<tool::string>*)sarr)->push(buf);
		strcpy(oname, name);

		if (xml::child(i) && level < olevel) {
			char t[200] = {0};
			if (cnt > 0)
				snprintf(t, sizeof(t)-1, "%s/%s[%d]", str, xml::name(i), cnt);
			else
				snprintf(t, sizeof(t)-1, "%s/%s", str, xml::name(i));

			path_names_r(sarr, t, xml::child(i), ++level, olevel);
		}
	}
}

// user must delete returned string array
HND path_names(HND e, int level)
{

	HND s = CHND(unsigned int, new tool::array<tool::string>(0));

	path_names_r(s, "", e, 0, level);

	return s;
}


};	

#ifndef _WIN32
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

SHMDEF* pSHM;
#define SHMID "STAR_SHM"

namespace shm {

#ifndef _WIN32
	HND shmOpen(int init)
	{ 
		printf("%s powieu\n", __FUNCTION__);


		key_t	key;
		int		flag = 0;
		int		shmid;

		char* s = SHMID;
		int		k = 0;
		while (*s)
			k += (k << 3) + *s++;

		key = ftok("/dev/null", k % 256);



		if (errno == -1)
			return NULL;
		int sz = sizeof(SHMDEF);
		printf("%s: %s(sz %d) k=%d error=%d\n", __func__, SHMID, sz, k, errno);

		if (shmget(key, sz, 0666) == -1 && errno == ENOENT) {	// not exist
			if ((shmid = shmget(key, sz, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
				printf("FATAL :shmalloc(0) create error\n");
				return NULL;
			}
			if ((pSHM = (SHMDEF*)shmat(shmid, 0, 0)) == (void*)-1) {
				printf("FATAL :shmat() : attach error\n");
				return NULL;
			}
			if (init)
				memset(pSHM, 0, sz);
		}
		else {
			if ((shmid = shmget(key, sz, 0)) == -1) {
				printf("FATAL :shmget() : can't get\n");
				return NULL;
			}
			if ((pSHM = (SHMDEF*)shmat(shmid, 0, 0)) == (void*)-1) {
				printf("FATAL :shmat() : attach error\n");
				return NULL;
			}
		}
		printf("shm OK%x\n", pSHM);
		return shmGet();
	}
	
	HND shmGet()
	{
		return CHND(HND, pSHM);
	}

	void shmAlive(int idx)
	{
		if (pSHM == 0)return;

		SHMDEF* p = (SHMDEF*)pSHM;

		p->stList[idx].nPID = getpid();
		p->stList[idx].nCount++;
	}
#else
	HND shmOpen(int init) { printf("%s 11\n", __FUNCTION__); return 0; }
	HND shmGet() { return 0; }
	void shmAlive(int type) {}
#endif
//		
};
