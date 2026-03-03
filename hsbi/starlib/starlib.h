#ifndef __STARLIB_HEADER__
#define __STARLIB_HEADER__


#ifndef CHND
#if defined(_WIN64) || defined(__x86_64__)
typedef unsigned long long HND;
#else
typedef unsigned int HND;
//typedef unsigned long long HND;
#endif
#define CHND(x, y) ((x)(y))
#endif

//#ifndef CHND
//	typedef unsigned int HND;
//	#define CHND(x, y) ((x)(y))
//#endif

#ifdef STDCALL
#undef STDCALL
#endif

#ifdef _WIN32
	typedef unsigned __int64 uint64;
	#define STDCALL __stdcall

#else
	typedef unsigned long long uint64;
	#define STDCALL
#endif



	// shm define


typedef unsigned short word;
typedef unsigned int uint;
typedef const char cchar;


extern "C" {         


#define STAR_TO_DAEMON	1		// to daemon
#define STAR_TO_OS		3		// for communicate with BIT
#define STAR_TO_PGM		4		// test pgm
#define STAR_TO_SCM		7		// system monitor
#define STAR_TO_DIAG	9		// diagnostics
#define STAR_TO_SYSCLI	11		// system test client
#define STAR_TO_ADMIN	10		// administration endpoint
#define STAR_TO_ROUTER	12

// packet mode[15:8] packet type

#define PKT_TYPE_ADMIN			0x100
#define PKT_TYPE_RESULT			0x200
#define PKT_TYPE_SHM			0x300
#define PKT_TYPE_REQFILE		0x400

// star_recv() returns

#define STAR_RET_CLOSED		0
#define STAR_RET_PENDING		1
#define STAR_RET_RECV		2
#define STAR_RET_SHM			PKT_TYPE_SHM
#define STAR_RET_REQFILE		PKT_TYPE_REQFILE

// packet mode[16] compressed?

#define PKT_COMPRESSED			0x10000


extern char*	STDCALL version(int n);
extern uint64	STDCALL vtoi(cchar *s);
extern void		STDCALL musleep(int n);

extern int		STDCALL getprocid();
extern void		STDCALL ptimer_begin(int n);
extern double	STDCALL ptimer_ellapsed(int n);

extern HND		STDCALL star_create();
extern void		STDCALL star_destroy(HND h);
extern char*	STDCALL star_data(HND h);
extern int		STDCALL star_length(HND h);
extern int		STDCALL star_open(HND h, cchar *host, int port);
extern void		STDCALL star_close(HND h);
extern int		STDCALL star_send(HND h, int mode, cchar *buf, int len);
extern int		STDCALL star_sendfile(HND h, cchar* star_path, int mode, cchar *target, cchar *source);
extern int		STDCALL star_recv(HND h);
extern int		STDCALL star_pkt_type(HND h);
extern int		STDCALL star_chunk_size(HND h, int nSize);				
extern int		STDCALL star_ping(cchar *ip);

extern char*	STDCALL star_host(HND h);
extern char*	STDCALL star_name(HND h);
extern char*	STDCALL star_filename(HND h);
extern int		STDCALL star_port(HND h);
extern char*	STDCALL star_entity(HND h);
extern char*	STDCALL star_path(HND h);
extern char*	STDCALL star_local_ip(HND h, int mode);

extern HND		STDCALL shm_open(int init);
extern HND		STDCALL shm_get();
extern void		STDCALL shm_alive(int idx);
				 
extern HND		STDCALL xml_docset(cchar *fn);
extern int		STDCALL xml_parse(HND dadr, cchar *xml);
extern int		STDCALL xml_save(HND dadr);
extern int		STDCALL xml_saveto(HND dadr, cchar *fn);
extern int		STDCALL xml_load(cchar *fn, HND *doc);
extern void		STDCALL xml_docfree(HND dadr);
extern HND		STDCALL xml_docroot(HND eh);
extern cchar*	STDCALL xml_error(HND h);
extern int		STDCALL xml_lines(HND h);
extern cchar*	STDCALL xml_source(HND h, char* s);
extern int		STDCALL xml_source_len(HND h);
extern cchar*	STDCALL xml_docsrc(HND h, char* s);
extern int		STDCALL xml_docsrc_len(HND h);
extern void		STDCALL xml_condense(HND h, int x);
extern HND		STDCALL xml_create(cchar *name);
extern HND		STDCALL xml_add(HND eh1, HND eh2);
extern void		STDCALL xml_destroy(HND eh);
extern HND		STDCALL xml_clone(HND eh);
extern HND		STDCALL xml_insertafter(HND eh1, HND eh2);
extern HND		STDCALL xml_insertbefore(HND eh1, HND eh2);
extern int		STDCALL xml_remove(HND eh);
extern HND 		STDCALL xml_root(HND dh);
extern HND 		STDCALL xml_first(HND dh);
extern HND 		STDCALL xml_next(HND eh);
extern HND 		STDCALL xml_last(HND dh);
extern HND 		STDCALL xml_prev(HND eh);
extern HND 		STDCALL xml_find(HND eh, cchar *name);
extern HND 		STDCALL xml_child(HND eh);
extern cchar*	STDCALL xml_name(HND eh);
extern void 	STDCALL xml_setname(HND eh, cchar *text);
extern cchar*	STDCALL xml_text(HND eh);
extern void		STDCALL xml_set_text(HND eh, cchar *text);
extern int 		STDCALL xml_type(HND eh);
extern HND		STDCALL xml_att_first(HND eh);
extern HND		STDCALL xml_att_next(HND ah);
extern cchar*	STDCALL xml_att_name(HND ah);
extern cchar*	STDCALL xml_att_value(HND ah);
extern cchar*	STDCALL xml_att_find(HND eh, cchar *name);
extern void		STDCALL xml_att_sets(HND eh, cchar *name, cchar *value);
extern void		STDCALL xml_att_setn(HND eh, cchar *name, int value);
extern HND		STDCALL xml_path_handle(HND eh, cchar *path);
extern int		STDCALL xml_path_n(HND eh, cchar *path);
extern cchar*	STDCALL xml_path_value(HND eh, cchar *path);
extern HND		STDCALL xml_path_names(HND e, int level);
				
#ifdef __cplusplus
}
#endif 

#endif 
