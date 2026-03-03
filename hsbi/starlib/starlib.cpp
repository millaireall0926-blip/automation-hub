
#include "function.h"
#include <stdarg.h>

#include "compat.h"

#ifdef _WIN32

#ifdef STARLIB_EXPORTS
	#define LIB_API __declspec(dllexport) 
#else
	#define LIB_API __declspec(dllimport)
#endif
#define STDCALL __stdcall

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
//	VH_STATIC
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			printf(LIB_VER "\n");
			break;
		case DLL_PROCESS_DETACH:
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
    }
    return TRUE;
}

#else

#define LIB_API
#define STDCALL

#endif


extern "C" {           

LIB_API cchar*	STDCALL version(int n)					{ return util::do_util_version(n); }
LIB_API uint64	STDCALL vtoi(cchar *s)					{ return util::do_util_vtoi(s); }
LIB_API void	STDCALL musleep(int n)					{ util::do_util_usleep(n); }
LIB_API int		STDCALL getprocid()						{ return util::do_util_getprocid(); }
LIB_API void	STDCALL ptimer_begin(int n)				{ util::do_util_ptimer_begin(n); }
LIB_API double	STDCALL ptimer_ellapsed(int n)			{ return util::do_util_ptimer_ellapsed(n); }

LIB_API HND		STDCALL star_create() { return snl::do_star_create();	}
LIB_API void	STDCALL star_destroy(HND h)				{ snl::do_star_destroy(h); }
LIB_API cchar*	STDCALL star_data(HND h)				{ return snl::do_star_data(h); }
LIB_API int		STDCALL star_length(HND h)				{ return snl::do_star_length(h); }
LIB_API int		STDCALL star_open(HND h, cchar *host, int port) { return snl::do_star_open(h, host, port); }
LIB_API void	STDCALL star_close(HND h)				{ snl::do_star_close(h); }
LIB_API int		STDCALL star_send(HND h, int mode, cchar *buf, int len) { return snl::do_star_send(h, mode, buf, len); }
LIB_API int		STDCALL star_sendfile(HND h, cchar* star_path, int mode, cchar *target, cchar *source) { return snl::do_star_sendfile(h, star_path, mode, target, source); }
LIB_API int		STDCALL star_recv(HND h)				{ return snl::do_star_recv(h); } 
LIB_API int		STDCALL star_pkt_type(HND h)			{ return snl::do_star_pkt_type(h); }
LIB_API void	STDCALL star_chunk_size(HND h, int nSize) { return snl::do_star_chunk_size(h, nSize); }
LIB_API int		STDCALL star_ping(cchar *ip)			{ return snl::do_star_ping(ip); }
LIB_API cchar*	STDCALL star_host(HND h)				{ return snl::do_star_host(h); }
LIB_API int		STDCALL star_port(HND h)				{ return snl::do_star_port(h); }
LIB_API cchar*	STDCALL star_entity(HND h)				{ return snl::do_star_entity(h); }
LIB_API cchar*	STDCALL star_name(HND h)				{ return snl::do_star_name(h); }
LIB_API cchar*	STDCALL star_filename(HND h)			{ return snl::do_star_filename(h); }
LIB_API cchar*	STDCALL star_path(HND h)				{ return snl::do_star_path(h); }
LIB_API cchar*	STDCALL star_local_ip(HND h, int mode)	{ return snl::do_star_local_ip(h,mode); }

LIB_API HND		STDCALL shm_open(int init)				{ return shm::shmOpen(init); }
LIB_API HND		STDCALL shm_get()						{ return shm::shmGet(); }
LIB_API void	STDCALL shm_alive(int idx)				{ shm::shmAlive(idx); }
	 
LIB_API HND		STDCALL xml_docset(cchar *fn)			{ return xml::docset(fn); }
LIB_API int		STDCALL xml_parse(HND dadr, cchar *xml)	{ return xml::parse(dadr, xml); }
LIB_API int		STDCALL xml_save(HND dadr)				{ return xml::save(dadr); }
LIB_API int		STDCALL xml_saveto(HND dadr, cchar *fn)	{ return xml::saveto(dadr, fn); }
LIB_API int		STDCALL xml_load(cchar *fn, HND *doc)	{ return xml::load(fn, doc); }
LIB_API void	STDCALL xml_docfree(HND dadr)			{ xml::docfree(dadr); }
LIB_API HND		STDCALL xml_docroot(HND eh)				{ return xml::docroot(eh); }		
LIB_API cchar*	STDCALL xml_error(HND h)				{ return xml::error(h); }
LIB_API int		STDCALL xml_lines(HND h)				{ return xml::lines(h); }
LIB_API cchar*	STDCALL xml_docsrc(HND h, char *s)		{ return xml::docsrc(h, s); }
LIB_API int		STDCALL xml_docsrc_len(HND h)			{ return xml::docsrc_len(h); }
LIB_API void	STDCALL xml_condense(HND h, int x)		{ xml::condense(h, x); }
LIB_API HND		STDCALL xml_create(cchar *name)			{ return xml::create(name); }
LIB_API HND		STDCALL xml_add(HND eh1, HND eh2)		{ return xml::add(eh1, eh2); }
LIB_API void	STDCALL xml_destroy(HND eh)				{ xml::destroy(eh);  }
LIB_API HND		STDCALL xml_clone(HND eh)				{ return xml::clone(eh); }
LIB_API HND		STDCALL xml_insertafter(HND eh1, HND eh2) { return xml::insertafter(eh1, eh2); }
LIB_API HND		STDCALL xml_insertbefore(HND eh1, HND eh2) { return xml::insertbefore(eh1, eh2); }
LIB_API int		STDCALL xml_remove(HND eh)				{ return xml::remove(eh); }
LIB_API HND		STDCALL xml_root(HND dh)				{ return xml::root(dh); }
LIB_API HND		STDCALL xml_first(HND dh)				{ return xml::first(dh); }
LIB_API HND		STDCALL xml_next(HND eh)				{ return xml::next(eh); }
LIB_API HND		STDCALL xml_last(HND dh)				{ return xml::last(dh); }
LIB_API HND		STDCALL xml_prev(HND eh)				{ return xml::prev(eh); }
LIB_API HND		STDCALL xml_find(HND eh, cchar *name)	{ return xml::find(eh, name); }
LIB_API HND		STDCALL xml_child(HND eh)				{ return xml::child(eh); }
LIB_API cchar*	STDCALL xml_source(HND h, char* s)		{ return xml::source(h,s); }
LIB_API int		STDCALL xml_source_len(HND h)			{ return xml::source_len(h); }
LIB_API cchar*	STDCALL xml_name(HND eh)				{ return xml::name(eh); }
LIB_API void	STDCALL xml_setname(HND eh, cchar *text) { xml::setname(eh, text); }
LIB_API cchar*	STDCALL xml_text(HND eh)				{ return xml::text(eh); }
LIB_API void	STDCALL xml_set_text(HND eh, cchar *text) { xml::set_text(eh, text); }
LIB_API int		STDCALL xml_type(HND eh)				{ return xml::type(eh); }
LIB_API HND		STDCALL xml_att_first(HND eh)			{ return xml::att_first(eh); }
LIB_API HND		STDCALL xml_att_next(HND ah)			{ return xml::att_next(ah);  }
LIB_API cchar*	STDCALL xml_att_name(HND ah)			{ return xml::att_name(ah); }
LIB_API cchar*	STDCALL xml_att_value(HND ah)			{ return xml::att_value(ah);  }
LIB_API cchar*	STDCALL xml_att_find(HND eh, cchar *name) { return xml::att_find(eh, name); }
LIB_API void	STDCALL xml_att_sets(HND eh, cchar *name, cchar *value) { xml::att_sets(eh, name, value); }
LIB_API void	STDCALL xml_att_setn(HND eh, cchar *name, int value) { xml::att_setn(eh, name, value); }
LIB_API HND		STDCALL xml_path_handle(HND eh, cchar *path)	{ return xml::path_handle(eh, path); }
LIB_API int		STDCALL xml_path_n(HND eh, cchar *path)			{ return xml::path_n(eh, path); }
LIB_API cchar*	STDCALL xml_path_value(HND eh, cchar *path)		{ return xml::path_value(eh, path); }
LIB_API HND		STDCALL xml_path_names(HND e, int level)		{ return xml::path_names(e, level); }

#ifdef __cplusplus
}
#endif  
