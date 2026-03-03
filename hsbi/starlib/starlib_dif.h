#ifndef __STARLIB_DIF_H__
#define __STARLIB_DIF_H__

/*

v4.20	2021/12/15
	add: add first

*/

typedef unsigned short word;
typedef const char cchar;

#define DLIB_NAME "starlib"

#ifndef CHND
#if defined(__x86_64__)
typedef unsigned long long HND;
#else
typedef unsigned int HND;

#endif
#define CHND(x, y) ((x)(y))
#endif

#ifdef STDCALL
#undef STDCALL
#endif

// STAR constant
#define STAR_TO_DAEMON	1		
#define STAR_TO_OS		3		
#define STAR_TO_PGM		4		
#define STAR_TO_DIAG	9		
#define STAR_TO_ADMIN	10		
#define STAR_TO_ROUTER	12

// packet mode[15:8] 

#define PKT_TYPE_ADMIN			0x100		
#define PKT_TYPE_RESULT			0x200
#define PKT_TYPE_REQFILE		0x400
#define PKT_TYPE_SLOTSTAT		0x800
#define PKT_TYPE_REQDONE		0x1000
#define PKT_TYPE_SENDFILE		0x2000
#define PKT_TYPE_SENDBIN		0x4000


#define STAR_RET_CLOSED		0
#define STAR_RET_PENDING	1
#define STAR_RET_RECV		2
#define STAR_RET_REQFILE		PKT_TYPE_REQFILE

#define PKT_COMPRESSED			0x10000


typedef struct st_shmItem
{
	char sName[128];
	int nCount;
	int nPID;

}SHMITEM;

// shm define
typedef struct st_shmDev {

	SHMITEM	stList[16]; // 0=pgm, 1=diag, 2>=etc...

	int		nRunStat;  // 0=none, 1=run

	int		nCheckBid;
	int		nBid;

	int		nCmd;		// for expend function
	int		nCmdStat;

	bool	bContact;
	char	sLotID[128];
	char	sPgmPath[256];
	char	sPgmName[128];
	char	sStartTime[128];
	char	sEndTime[128];
	char	sCurFlow[128];
	char	sCurStep[128];

	double	fSetTemp;
	double	fCurTemp;

}SHMDEF;



#include <stdio.h>
#include <dlfcn.h>

typedef unsigned long long uint64;

#define LIB_FN			("/star/test/" DLIB_NAME ".so")
#define LIB_HANDLE		void*

#define LIB_OPEN(s)		dlopen(LIB_FN, RTLD_LOCAL | RTLD_LAZY)
#define LIB_FREE(h)		dlclose(h)
#define LIB_PROC(h, s)	((void *)dlsym(h, s))
#define STDCALL


#define PMAP(h, func, str) (*( void **)(&func) = LIB_PROC(h, str)); if (!func) return -2

class  starlib
{
public:

	starlib() { h = 0; }
	~starlib() { unload(); }

private:

	LIB_HANDLE	h;
	
public:
	char*	(STDCALL *version)(int n);	
	uint64	(STDCALL *vtoi)(cchar *s);
	void	(STDCALL *usleep)(int n);

	int		(STDCALL *getprocid)();
	void	(STDCALL *ptimer_begin)(int n);
	double	(STDCALL *ptimer_ellapsed)(int n);

	// star net link
			
	HND		(STDCALL *star_create)();
	void	(STDCALL *star_destroy)(HND h);	
	char*	(STDCALL *star_data)(HND h);
	int		(STDCALL *star_length)(HND h);
	int		(STDCALL *star_open)(HND h, cchar *host, int port);
	void	(STDCALL *star_close)(HND h);
	int		(STDCALL *star_send)(HND h, int mode, cchar *buf, int len);
	int		(STDCALL *star_sendfile)(HND h, cchar* star_path, int mode, cchar *target, cchar *source);	
	int		(STDCALL *star_recv)(HND h);
	int		(STDCALL *star_pkt_type)(HND h);
	void	(STDCALL *star_chunk_size)(HND h, int nSize);
	int		(STDCALL *star_ping)(cchar *ip);	
	char*	(STDCALL *star_host)(HND h);
	char*	(STDCALL *star_name)(HND h);
	char*	(STDCALL *star_filename)(HND h);
	int		(STDCALL *star_port)(HND h);
	char*	(STDCALL *star_entity)(HND h);
	char*	(STDCALL *star_path)(HND h);
	char*	(STDCALL *star_local_ip)(HND h, int mode);

	HND		(STDCALL *shm_open)(int init);
	HND		(STDCALL *shm_get)();
	void	(STDCALL *shm_alive)(int idx);
	
	HND		(STDCALL *xml_docset)(cchar *fn);
	int		(STDCALL *xml_parse)(HND dadr, cchar *xml);
	int		(STDCALL *xml_save)(HND dadr);
	int		(STDCALL *xml_saveto)(HND dadr, cchar *fn);
	int		(STDCALL *xml_load)(cchar *fn, HND *doc);
	void	(STDCALL *xml_docfree)(HND dadr);
	HND		(STDCALL *xml_docroot)(HND eh);
	char* 	(STDCALL *xml_error)(HND h);
	int		(STDCALL *xml_lines)(HND h);
	char*	(STDCALL *xml_source)(HND h, char* s);
	int		(STDCALL *xml_source_len)(HND h);
	char*	(STDCALL *xml_docsrc)(HND h, char* s);
	int		(STDCALL *xml_docsrc_len)(HND h);
	void	(STDCALL *xml_condense)(HND h, int x);
	HND		(STDCALL *xml_create)(cchar *name);
	HND		(STDCALL *xml_add)(HND eh1, HND eh2);
	void	(STDCALL *xml_destroy)(HND eh);
	HND		(STDCALL *xml_clone)(HND eh);
	HND		(STDCALL *xml_insertafter)(HND eh1, HND eh2);
	HND		(STDCALL *xml_insertbefore)(HND eh1, HND eh2);
	int		(STDCALL *xml_remove)(HND eh);			 
	HND 	(STDCALL *xml_root)(HND dh);
	HND 	(STDCALL *xml_first)(HND dh);
	HND 	(STDCALL *xml_next)(HND eh);
	HND 	(STDCALL *xml_last)(HND dh);
	HND 	(STDCALL *xml_prev)(HND eh);
	HND 	(STDCALL *xml_find)(HND eh, cchar *name);
	HND 	(STDCALL *xml_child)(HND eh);
	char*	(STDCALL *xml_name)(HND eh);
	void 	(STDCALL *xml_setname)(HND eh, cchar *text);			 
	char*	(STDCALL *xml_text)(HND eh);
	void	(STDCALL *xml_set_text)(HND eh, cchar *text);
	int 	(STDCALL *xml_type)(HND eh);
	HND		(STDCALL *xml_att_first)(HND eh);
	HND		(STDCALL *xml_att_next)(HND ah);
	char* 	(STDCALL *xml_att_name)(HND ah);
	char* 	(STDCALL *xml_att_value)(HND ah);
	char* 	(STDCALL *xml_att_find)(HND eh, cchar *name);
	void	(STDCALL *xml_att_sets)(HND eh, cchar *name, cchar *value);
	void	(STDCALL *xml_att_setn)(HND eh, cchar *name, int value);
	HND		(STDCALL *xml_path_handle)(HND eh, cchar *path);
	int		(STDCALL *xml_path_n)(HND eh, cchar *path);
	char*	(STDCALL *xml_path_value)(HND eh, cchar *path);
	HND		(STDCALL *xml_path_names)(HND e, int level);
			 
public:		 
			 
	void unload() { if (h) LIB_FREE(h); }
	int load()		
	{ 
		if (!(h = LIB_OPEN(LIB_FN)))
			return -1;
		
		PMAP(h, version,		"version");
		PMAP(h, vtoi,			"vtoi");
		PMAP(h, usleep,			"musleep");
		PMAP(h, getprocid,		"getprocid");
		PMAP(h, ptimer_begin,	"ptimer_begin");
		PMAP(h, ptimer_ellapsed,"ptimer_ellapsed");

		PMAP(h, star_create,	"star_create");
		PMAP(h, star_destroy,	"star_destroy");
		PMAP(h, star_data,		"star_data");
		PMAP(h, star_length,	"star_length");
		PMAP(h, star_open,		"star_open");
		PMAP(h, star_close,		"star_close");
		PMAP(h, star_send,		"star_send");
		PMAP(h, star_sendfile,	"star_sendfile");		
		PMAP(h, star_recv,		"star_recv");
		PMAP(h, star_pkt_type,	"star_pkt_type");
		PMAP(h, star_chunk_size,"star_chunk_size");
		PMAP(h, star_ping,		"star_ping");
		PMAP(h, star_host,		"star_host");
		PMAP(h, star_name,		"star_name");
		PMAP(h, star_filename,	"star_filename");
		PMAP(h, star_port,		"star_port");
		PMAP(h, star_entity,	"star_entity");
		PMAP(h, star_path,		"star_path");
		PMAP(h, star_local_ip,	"star_local_ip");

		
		PMAP(h, shm_open,		"shm_open");
		PMAP(h, shm_get,		"shm_get");
		PMAP(h, shm_alive,		"shm_alive");
		
		PMAP(h, xml_docset,		"xml_docset");
		PMAP(h, xml_parse,		"xml_parse");
		PMAP(h, xml_save,		"xml_save");
		PMAP(h, xml_saveto,		"xml_saveto");
		PMAP(h, xml_load,		"xml_load");
		PMAP(h, xml_docfree,	"xml_docfree");
		PMAP(h, xml_docroot,	"xml_docroot");
		PMAP(h, xml_error,		"xml_error");
		PMAP(h, xml_lines,		"xml_lines");
		PMAP(h, xml_source,		"xml_source");
		PMAP(h, xml_source_len,	"xml_source_len");
		PMAP(h, xml_docsrc,		"xml_docsrc");
		PMAP(h, xml_docsrc_len,	"xml_docsrc_len");
		PMAP(h, xml_condense,	"xml_condense");
		PMAP(h, xml_create,		"xml_create");
		PMAP(h, xml_add,		"xml_add");
		PMAP(h, xml_destroy,	"xml_destroy");
		PMAP(h, xml_clone,		"xml_clone");
		PMAP(h, xml_insertafter,"xml_insertafter");
		PMAP(h, xml_insertbefore,"xml_insertbefore");
		PMAP(h, xml_remove,		"xml_remove");
		PMAP(h, xml_root,		"xml_root");
		PMAP(h, xml_first,		"xml_first");
		PMAP(h, xml_next,		"xml_next");
		PMAP(h, xml_last,		"xml_last");
		PMAP(h, xml_prev,		"xml_prev");
		PMAP(h, xml_find,		"xml_find");
		PMAP(h, xml_child,		"xml_child");
		PMAP(h, xml_name,		"xml_name");
		PMAP(h, xml_setname,	"xml_setname");
		PMAP(h, xml_text,		"xml_text");
		PMAP(h, xml_set_text,	"xml_set_text");
		PMAP(h, xml_type,		"xml_type");
		PMAP(h, xml_att_first,	"xml_att_first");
		PMAP(h, xml_att_next,	"xml_att_next");
		PMAP(h, xml_att_name,	"xml_att_name");
		PMAP(h, xml_att_value,	"xml_att_value");
		PMAP(h, xml_att_find,	"xml_att_find");
		PMAP(h, xml_att_sets,	"xml_att_sets");
		PMAP(h, xml_att_setn,	"xml_att_setn");
		PMAP(h, xml_path_handle,"xml_path_handle");
		PMAP(h, xml_path_n,		"xml_path_n");
		PMAP(h, xml_path_value,	"xml_path_value");
		PMAP(h, xml_path_names,	"xml_path_names");
			
		return 0;
	}
};

#endif
