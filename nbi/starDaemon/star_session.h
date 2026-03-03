#ifndef _STAR_SESSION_
#define _STAR_SESSION_



#include <stdio.h>
#include <sys/types.h>

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "star_socket.h"

#define USE_FT_HANDSHAKE

// defaults
#define STAR_ADMIN_PORT		101		
#define STAR_PROBE_PORT		16922	

// session type
#define SESSION_NULL		0
#define SESSION_LISTEN		1	
#define SESSION_DEFAULT		2
#define SESSION_OS			3	
#define SESSION_PGM			4	
#define SESSION_DIAG		9	
#define SESSION_ROUTER		12

//packet
#define PKT_STATE_HEADER	0
#define PKT_STATE_DATA		1

// magic
#define PKT_MAGIC			0xC0DECAFE

// dest
#define PKT_DEST_DAEMON		SESSION_LISTEN   
#define PKT_DEST_OS			SESSION_OS
#define PKT_DEST_PGM		SESSION_PGM
#define PKT_DEST_DIAG		SESSION_DIAG
#define PKT_DEST_ROUTER		SESSION_ROUTER

// type
#define PKT_TYPE_ADMIN		0x100		
#define PKT_TYPE_RESULT		0x200
#define PKT_TYPE_REQFILE	0x400
#define PKT_TYPE_SLOTSTAT	0x800
#define PKT_TYPE_REQDONE	0x1000
#define PKT_TYPE_SENDFILE	0x2000

#define PKT_COMPRESSED		0x10000

#define STAR_XML_NOCMD		0
#define STAR_XML_CMD			1
#define STAR_XML_CMD_CLOSE	2
#define STAR_XML_CLOSE		3
#define STAR_XML_HASATT		0x80


#define CHUNK_SZ			65536		

#define STAR_RESULT_OK		1
#define STAR_RESULT_FAIL		0

class star_session
{
public:
	star_session();
	~star_session();

private:
	typedef struct {
		int		magic;
		int		mode;		
		int     len;
		int		orglen;		
	} header_t;

	typedef struct {
		char name[100];
		char val[512];
	} att_t;

public:

	star_socket	soc;
	star_buffer	packet;				
	star_buffer	result;				
	star_buffer	cbuf;				
	
	int			nPacketState;
	header_t	pkt_header;

	bool		bFtMode;			
	char		sFtFilename[1024];	
	int			nFtSize;			
	FILE*		pFtFileHandle;		
	int			nChunkSize;			
	
	int			nTagType;
	char		sTag[256];			
	att_t		sTagAtts[10];		
		
	int			nSessionType;		
	char		sSessionName[100];	
	char		sStarPath[100];		
	char		sRecvFilename[256];	

	bool		bWaitResponse;		
	size_t		FTOffset;			

public:
	char*		parseXml(char *sXml);
	char*		getXmlAtt(const char *s);

	void		end();
	void		clear();

	void		setPath(const char* s);
	int			starRead();
	int			starWrite(int mode, int sz, void* data);
	int			starResult(int seq, int status, const char* rstr = "", int pktType = PKT_TYPE_RESULT);
	bool		starGetResponse(const char* cmd, bool& cmd_return);		
	bool		starWaitResponse(const char* cmd, int timeout);
	int			starRecvFile();
	bool		starSendFile(const char* path, int mode, const char* source, const char* target, int client = -1);	
		
	inline int	getType() { return pkt_header.mode & 0xff00; }
	inline int	getDestination() { return pkt_header.mode & 0x00ff; }
	inline void	setChunkSize(int nSize) { nChunkSize = nSize;}
	
	inline bool	isConnected() {	return soc.isConnected(); }
	

private:
	unsigned int compress ( const void *const in_data, unsigned int in_len, void *out_data, unsigned int out_len );
	unsigned int decompress ( const void *const in_data,  unsigned int in_len, void *out_data, unsigned int out_len );
};

#endif 

