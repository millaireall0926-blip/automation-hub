#ifndef __WRAPPER_H__
#define __WRAPPER_H__


#pragma once

#include "compat.h"
#include "tinyxml.h"
#include "star_client.h"




#define CHND(x, y) ((x)(y))


//typedef unsigned int HND;
#if defined(_WIN64) || defined(__x86_64__)
typedef unsigned long long HND;
#else
typedef unsigned int HND;
//typedef unsigned long long HND;
#endif

extern "C" {

namespace snl {     // star net link
	inline	HND		do_star_create()	{ return CHND(HND, new star_client()); }
	inline	void	do_star_destroy(HND h)	{ if (h) delete (star_client*)h; }
	inline	char*	do_star_data(HND h)		{ return (h) ? ((star_client*)h)->getData() : (char*)""; }
	inline	int		do_star_length(HND h)	{ return (h) ? ((star_client*)h)->getLength() : 0; }
	inline	int		do_star_open(HND h, cchar *host, int port) { return (h) ? ((star_client*)h)->open(host, port) : 0; }
	inline	void	do_star_close(HND h)	{ if (h) ((star_client*)h)->close(); }
	inline	int		do_star_send(HND h, int mode, cchar *buf, int len) { return (h) ? ((star_client*)h)->send(mode, buf, len) : 0; }
	inline	int		do_star_sendfile(HND h, cchar* star_path, int mode, cchar *target, cchar *source) { return (h) ? ((star_client*)h)->sendFile(star_path, mode, target, source) : 0; }	
	inline	int		do_star_recv(HND h)		{
		return (h) ? ((star_client*)h)->recv() : 0;
	}
	
	inline	int		do_star_pkt_type(HND h) { return (h) ? ((star_client*)h)->getType() : 0; }
	inline	void	do_star_chunk_size(HND h, int nSize) { ((star_client*)h)->setPacketSize(nSize); }

	inline	HND		do_star_ping(cchar *ip)	{
						star_client* g = new star_client();
						return (!g->ping(ip)) ? 0 : CHND(HND, g); 
					}
		
	inline	char*	do_star_host(HND h)		{ return (h) ? ((star_client*)h)->ip : (char*)""; }
	inline	int		do_star_port(HND h)		{ return (h) ? ((star_client*)h)->port : 0; }
	inline	char*	do_star_entity(HND h)	{ return (h) ? ((star_client*)h)->entity : (char*)""; }
	inline	char*	do_star_name(HND h)		{ return (h) ? ((star_client*)h)->name : (char*)""; }
	inline	char*	do_star_filename(HND h)	{ return (h) ? ((star_client*)h)->s.sRecvFilename : (char*)""; }

	// AP로부터 SHM을 받았을 경우 처리하는 함수

	inline	char*	do_star_path(HND h)		{ return (h) ? ((star_client*)h)->getPath() : (char*)""; }
	inline	char*	do_star_local_ip(HND h, int mode)	{ return (h) ? ((star_client*)h)->getLocalIp(mode) : (char*)""; }
};

namespace util {
	char*			do_util_version(int n);
	uint64			do_util_vtoi(cchar *s);
	void			do_util_usleep(int n);

	int				do_util_getprocid();	

	void			do_util_ptimer_begin(int n);
	double			do_util_ptimer_ellapsed(int n);
};


#define NODE_DOCUMENT	0
#define NODE_ELEMENT	1
#define NODE_COMMENT	2
#define NODE_UNKNOWN	3
#define NODE_TEXT		4
#define NODE_DECLARE	5
#define NODE_TYPECNT	6

namespace xml {

	// document

	inline HND	docset(cchar *fn)				{ 	return CHND(HND, new TiXmlDocument(fn)); }
	int			parse(HND dadr, cchar *xml);
	inline int	save(HND dadr)					{ return (dadr) ? ((TiXmlDocument*)dadr)->SaveFile() : 0; }
	inline int	saveto(HND dadr, cchar *fn)		{ return (dadr) ? ((TiXmlDocument*)dadr)->SaveFile(fn) : 0; }
	int			load(cchar *fn, HND *doc);
	inline void docfree(HND dadr)				{ TiXmlDocument *doc = (TiXmlDocument*)dadr; if (doc) delete doc; }
	HND			docroot(HND eh);		// find document root from element
	cchar*		error(HND h);
	inline int	lines(HND h)					{ return (h) ? ((TiXmlDocument *)h)->Row() : 0; }
	char*		docsrc(HND h, char* s); 
	int			docsrc_len(HND h);
	inline void condense(HND h, int x)			{ if (h) ((TiXmlDocument*)h)->SetCondenseOut(x ? true : false); }

	// element

	char*		source(HND eh, char* s);
	int			source_len(HND eh);
	inline HND	create(cchar *name)				{ TiXmlElement *item = new TiXmlElement(name); return CHND(HND, item); }
	inline HND	add(HND eh1, HND eh2)			{ return CHND(HND, ((TiXmlElement *)eh1)->InsertEndChild(*((TiXmlElement *)eh2))); }
	inline void	destroy(HND eadr)				{ TiXmlElement *item = (TiXmlElement *)eadr;  if (item) delete item; }

	HND			clone(HND eh);
	HND			insertafter(HND eh1, HND eh2);
	HND			insertbefore(HND eh1, HND eh2);
	inline int	remove(HND eh)					{ TiXmlNode * p = ((TiXmlNode *)eh)->Parent(); return ((p) ? (p->RemoveChild((TiXmlNode *)eh)) : 0); }

	inline HND root(HND dh)						{ return (dh) ? CHND(HND, ((TiXmlDocument*)dh)->RootElement()) : 0; }
	inline HND first(HND dh)					{ return (dh) ? CHND( HND, ((TiXmlDocument*)dh)->FirstChild()) : 0; }
	inline HND last(HND dh)						{ return (dh) ? CHND( HND, ((TiXmlDocument*)dh)->LastChild()) : 0; }
	inline HND next(HND eh)						{ return (eh) ? CHND(HND, ((TiXmlElement *)eh)->NextSiblingElement()) : 0; }
	inline HND prev(HND eh)						{ 
		if (!eh) return 0;
		TiXmlNode* p = ((TiXmlElement *)eh)->PreviousSibling();
		if (!p) return 0;
		return CHND(HND, p->ToElement());
	}
	inline HND find(HND eh, cchar *name)		{ return (eh) ? CHND( HND, ((TiXmlElement *)eh)->FirstChild(name)) : 0; }
	inline HND child(HND eh)					{ return (eh) ? CHND( HND, ((TiXmlElement *)eh)->FirstChildElement()) : 0; }

	// element text & type

	inline cchar* name(HND eh)					{ return (eh) ? ((TiXmlElement *)eh)->Value() : ""; }
	inline void setname(HND eh, cchar *text)	{ if (eh) ((TiXmlElement *)eh)->SetValue(text); }

	cchar*		text(HND eh);
	void		set_text(HND eh, cchar *text);
	void 		setdata(HND eh, void *data);
	void*		getdata(HND eh);
	
	inline int type(HND eh)						{ return (eh) ? ((TiXmlElement *)eh)->Type() : 0; }

	// element attribute

	inline HND	att_first(HND eh)				{ return (eh) ? CHND( HND, ((TiXmlElement *)eh)->FirstAttribute() ) : 0; }
	inline HND	att_next(HND ah)				{ return (ah) ? CHND( HND, ((TiXmlAttribute *)ah)->Next() ) : 0; }
	inline const char* att_name(HND ah)			{ return (ah) ? ((TiXmlAttribute *)ah)->Name() : ""; }
	inline const char* att_value(HND ah)		{ return (ah) ? ((TiXmlAttribute *)ah)->Value() : ""; }
	inline const char* att_find(HND eh, cchar *name) { return (eh) ? ((TiXmlElement *)eh)->Attribute(name) : ""; }
	inline void att_sets(HND eh, cchar *name, cchar *value) { if (eh) ((TiXmlElement *)eh)->SetAttribute(name, value); }
	inline void att_setn(HND eh, cchar *name, int value) { if(eh) ((TiXmlElement *)eh)->SetAttribute(name, value); }

	// simple xml path (xpath)

	HND			path_find(HND eh, cchar *name);
	HND			path_handle(HND eh, cchar *path);
	int			path_n(HND eh, cchar *path);
	const char*	path_value(HND eh, cchar *path);
	HND			path_names(HND e, int level);				// returns string array

};	



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

namespace shm {
	HND			shmOpen(int init = 0);
	HND			shmGet();
	void		shmAlive(int idx); // idx 0 = pgm, 1= diag
};


} 

#endif
