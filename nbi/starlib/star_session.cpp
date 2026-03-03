#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "star_session.h"

#ifdef _WIN32	
	#pragma comment(lib, "mswsock.lib")
	#include <mswsock.h>
	#define snprintf _snprintf
	
	const size_t HANDSHAKE_SZ =	CHUNK_SZ;			

#else
	#include <fcntl.h>
	#include <sys/stat.h>

	const size_t HANDSHAKE_SZ = CHUNK_SZ * 16;		
#endif





star_session::star_session()
{
	bWaitResponse = false;
	soc.fd = SOCK_UNUSED;
	soc.setSocketType(SOC_TCP);
	clear();
}

star_session::~star_session()
{
}

void star_session::clear()
{
	nSessionType = SESSION_NULL;
	sSessionName[0] = '\0';
	sStarPath[0] = '\0';

	cbuf.setup(TCPBUF_MAX, TCPBUF_GROW);
	packet.setup(TCPBUF_MAX, TCPBUF_GROW);
	result.setup(TCPBUF_MAX, TCPBUF_GROW);

	nPacketState	= PKT_STATE_HEADER;

	bFtMode = false;
	pFtFileHandle = NULL;
	sFtFilename[0] = '\0';
	nFtSize = 0;
	nChunkSize = CHUNK_SZ;
}

void star_session::end()
{
	soc.close();
	clear();
}

void star_session::setPath(const char* s)
{
	strncpy(sStarPath, s, sizeof(sStarPath));
	sStarPath[sizeof(sStarPath)-1] = '\0';
}

void star_session::wlog(char *fmt, ...)
{
	va_list	ap;
	char buf[2048];

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf)-1, fmt, ap);
	va_end(ap);

	printf("%s", buf);
#ifdef _WIN32
	OutputDebugString(buf);
#endif
}

char* star_session::parseXml(char* sXml)
{
	char* tok = sXml;
	char* tmp;
	int		argcnt, i;

	memset(sTag, 0, sizeof(sTag));
	memset(sTagAtts, 0, sizeof(sTagAtts));

	for (; *tok; tok++) {
		if (!isspace(*tok))
			break;
	}

	if (!*tok)
		return tok;

	nTagType = STAR_XML_CMD;

	if (*tok == '<' && *(tok + 1)) {

		tok++;
		if (*tok == '/') {
			tok++;
			nTagType = STAR_XML_CLOSE;
		}

		memset(sTag, 0, sizeof(sTag));
		for (tmp = sTag; *tok; tok++) {
			if (isspace(*tok)) {
				nTagType |= STAR_XML_HASATT;
				break;
			}
			if (*tok == '/' || *tok == '>')
				break;
			*tmp++ = (char)toupper(*tok);
		}

		if ((nTagType & STAR_XML_HASATT) != 0) {

			memset(sTagAtts, 0, sizeof(sTagAtts));
			argcnt = 0;
			do {

				tok++;
				while (isspace(*tok)) tok++;

				if ((tmp = strchr(tok, '=')) != NULL) {

					strncpy(sTagAtts[argcnt].name, tok, tmp - tok);
					for (i = 0, tok = sTagAtts[argcnt].name; *tok && i < (int)sizeof(sTagAtts[argcnt].name) - 1; tok++, i++)
						*tok = (char)toupper(*tok);
					tok = tmp + 1;
					while (isspace(*tok)) tok++;

					if (*tok == '\"') {
						tok++;
						for (i = 0, tmp = sTagAtts[argcnt].val; *tok; tok++, i++) {
							if (*tok == '\"') break;
							if (i < (int)sizeof(sTagAtts[argcnt].val) - 1)
								*tmp++ = *tok;
						}
						argcnt++;
						tok++;
					}
					else {
						for (i = 0, tmp = sTagAtts[argcnt].val; *tok; tok++, i++) {
							if (isspace(*tok)) break;
							if (i < (int)sizeof(sTagAtts[argcnt].val) - 1)
								*tmp++ = *tok;
						}
						argcnt++;
					}

				}
			} while (*tok && *tok != '/' && *tok != '>');
		}

		if (*tok == '/') {
			tok++;
			nTagType = (nTagType & STAR_XML_HASATT) | STAR_XML_CMD_CLOSE;
		}

		if (*tok != '>') {
			nTagType = STAR_XML_NOCMD;
			return sXml;
		}

	}
	return tok + 1;
}

char *star_session::getXmlAtt(const char *s)
{	
	for (int i = 0; sTagAtts[i].name[0] != '\0'; i++) {
		if (strcasecmp(s, sTagAtts[i].name) == 0)
			return sTagAtts[i].val;
	}
	return "";
}

int star_session::starRead()
{
	int err, sz = 0, org_sz;

	err = soc.bufRead();
	if (err != S_ERR_NONE && err != S_ERR_QUEUED)
		return err;

	switch(nPacketState) {

	case PKT_STATE_HEADER:

		sz = sizeof(header_t);

		if (soc.rd.cnt < sz)
			return S_ERR_QUEUED;

		memcpy(&pkt_header, soc.rd.buf, sz);
		memmove(soc.rd.buf, soc.rd.buf + sz, soc.rd.cnt - sz);
		soc.rd.cnt -= sz;

		pkt_header.magic = ntohl(pkt_header.magic);
		pkt_header.mode = ntohl(pkt_header.mode);
		pkt_header.len = ntohl(pkt_header.len);
		pkt_header.orglen = ntohl(pkt_header.orglen);

		if ((unsigned)pkt_header.magic != PKT_MAGIC) {
			wlog("%s: bad packet. purge all data.\n", __FUNCTION__);
			soc.rd.reset();
			return S_ERR_QUEUED;
		}

		nPacketState = PKT_STATE_DATA;


	case PKT_STATE_DATA:

		sz = pkt_header.len;						
		org_sz = pkt_header.orglen;					

		if (sz != 0) {
			if (soc.rd.cnt < sz)
				return S_ERR_QUEUED;
	
			err = packet.alloc(org_sz + 1);
			if (err != 0) {
				wlog("%s: malloc() error. sz=%d\n", __FUNCTION__, org_sz + 1);
				soc.rd.reset();
				nPacketState = PKT_STATE_HEADER;
				return S_ERR_GENERAL;
			}
	
			if ((pkt_header.mode & PKT_COMPRESSED) != 0) {
				decompress(soc.rd.buf, sz, packet.buf, org_sz);
			}
			else {
				memcpy(packet.buf, soc.rd.buf, sz);
			}

			memmove(soc.rd.buf, soc.rd.buf + sz, soc.rd.cnt - sz);
			soc.rd.cnt -= sz;

			packet.cnt = org_sz;
			packet.buf[packet.cnt] = '\0';
		}
		
		nPacketState = PKT_STATE_HEADER;
		return S_ERR_NONE;

	default:

		wlog("%s: garbled data ???", __FUNCTION__);	
		nPacketState = PKT_STATE_HEADER;
		soc.rd.reset();
		break;

	}

	return S_ERR_NONE;
}

int star_session::starWrite(int mode, int org_sz, void *data)
{
	header_t* hdr;
	char *dp;
	int comp_sz, tsz = org_sz + sizeof(header_t);

	if (cbuf.alloc(tsz) != 0)
		return S_ERR_NOMEM;
		
	hdr = (header_t*)cbuf.buf;
	dp = cbuf.buf + sizeof(header_t);

	if ((mode & PKT_COMPRESSED) != 0 && org_sz > 32) {
		comp_sz = compress(data, org_sz, dp, org_sz);
		if (comp_sz == 0) {						
			mode &= ~PKT_COMPRESSED;
			comp_sz = org_sz;
			memcpy(dp, data, org_sz);
		}
	}
	else {
		mode &= ~PKT_COMPRESSED;
		comp_sz = org_sz;
		memcpy(dp, data, org_sz);
	}

	hdr->magic = htonl(PKT_MAGIC);
	hdr->mode = htonl(mode);
	hdr->len = htonl(comp_sz);
	hdr->orglen = htonl(org_sz);
	
	return soc.bufWrite(cbuf.buf, sizeof(header_t) + comp_sz);
}

int star_session::starResult(int seq, int status, const char* rstr, int pktType)
{
	result.alloc(strlen(rstr));
	char *s;

	switch (status) {
		case STAR_RESULT_OK: s = "OK"; break;
		case STAR_RESULT_FAIL: s = "FAIL"; break;
		default: s = "OK"; break;
	}

	int l = snprintf(result.buf, result.max-1, "<star-p path=\"%s\"><result seq=\"%d\" cmd=\"%s\" return=\"%s\" type=\"xml\">%s</result></star-p>", 
		sStarPath, seq, sTag, s, rstr);

	int resp = starWrite(pktType, l, result.buf);
	if (resp == S_ERR_GENERAL) {
		wlog("%s: can't write\n", __FUNCTION__);
		end();
	}

	return resp;
}

bool star_session::starGetResponse(const char* cmd, bool& cmd_return)
{
	cmd_return = false;

	int ret = starRead();
	if (ret == S_ERR_GENERAL) {
		end();
		wlog("%s: remote peer accidently close connection\n", __FUNCTION__);
		return false;
	}
	if (ret == S_ERR_QUEUED)
		return false;

	char* p = parseXml(packet.buf);
	if (strcmp(sTag, "STAR-P") == 0) {
		p = parseXml(p);
		if (strcasecmp(sTag, "RESULT") == 0 && strcasecmp(getXmlAtt("cmd"), cmd) == 0) {
			if (bWaitResponse) {
				size_t ofs = atoi(getXmlAtt("ofs"));
				if (FTOffset >= ofs) {
					cmd_return = (strcasecmp(getXmlAtt("return"), "OK") == 0);
					return true;
				} else {
					return false;		
				}
			} else {
				cmd_return = (strcasecmp(getXmlAtt("return"), "OK") == 0);
				return true;
			}
		}
	}

	
	return false;
}

bool star_session::starWaitResponse(const char* cmd, int timeout)
{
	bool cmdret = false;
	for (time_t st = time(NULL); st+timeout > time(NULL); ) {
		
		if (!soc.isConnected())
			return false;

		if (starGetResponse(cmd, cmdret) && cmdret)
			return true;

		soc.msleep(1);
	}
	return false;
}

int star_session::starRecvFile()
{
	FILE *f;
	size_t sz;
	int ofs;
	char fn[256] = {0}, *dir, *name;
	char *q = NULL;
	char *p = parseXml(packet.buf);

	if (strcmp(sTag, "STAR-P") != 0) {
		starResult(0, STAR_RESULT_FAIL, "bad packet. expected file receive.");
		return 0;
	}

	q = parseXml(p);
	if (strcmp(sTag, "SEND-FILE") != 0) {
		starResult(0, STAR_RESULT_FAIL, "bad command. must be 'send-file'");
		return 0;
	}

	dir = getXmlAtt("dir");
	name = getXmlAtt("name");
	ofs = atoi(getXmlAtt("ofs"));
	sz = atoi(getXmlAtt("size"));

	if (!*name) {
		starResult(0, STAR_RESULT_FAIL, "no file name given");
		return 0;
	}

	if (*dir) {
		if (dir[strlen(dir)-1] == '/')
			snprintf(fn, sizeof(fn)-1, "%s%s", dir, name);
		else
			snprintf(fn, sizeof(fn)-1, "%s/%s", dir, name);
	}
	else
		strcpy(fn, name);

	if (ofs != -1) {

		if ((f = fopen(fn, (ofs == 0) ? "w+b" : "r+b")) == NULL) {
			starResult(0, STAR_RESULT_FAIL, "can't open file");
			return 0; 
		}

		if (fseek(f, ofs, SEEK_SET) != 0) {
			starResult(0, STAR_RESULT_FAIL, "can't set position");
			fclose(f);
			return 0;
		}

		if (fwrite(q, 1, sz, f) != sz) {
			starResult(0, STAR_RESULT_FAIL, "can't write to file");
			fclose(f);
			return 0;
		}

		fclose(f);
	}
	else {
		strncpy(sRecvFilename, fn, sizeof(sRecvFilename));
		wlog("%s: recv file complete. filename=%s\n", __FUNCTION__, sRecvFilename);
		return 1;
	}
#ifdef USE_FT_HANDSHAKE
	strcpy(sTag, "SEND-FILE");
	starResult(0, STAR_RESULT_OK);
#endif
	return 0;
}

bool star_session::starSendFile(const char* path, int mode, const char* source, const char* target, int client)
{
	FILE *f = NULL;
	char *buf = NULL, *rbuf, *sbuf, *p, *pp, fn[256];
	char dir[256];
	int l = 0;
	bool ret = false;
	size_t sz, ofs, rsz, Ack_offset;
	size_t comp_sz, send_sz;

	p = strrchr((char*)target, '/');
	if (p == NULL) {			
		strcpy(fn, target);
		strcpy(dir, "");
	}
	else {
		if (p == target + strlen(target) - 1) {	
			strcpy(dir, target);
			p = strrchr((char*)source, '/');
			if (p == NULL) 
				strcpy(fn, source);
			else
				strcpy(fn, p+1);
		}
		else {
			strcpy(fn, p+1);
			memset(dir, 0, sizeof(dir));
			strncpy(dir, target, p - target);
		}
	}
	
	if ((f = fopen(source, "rb")) == NULL) {
		wlog("%s: '%s' open error", __FUNCTION__, fn);
		return false;
	}

	if (fseek(f, 0L, SEEK_END) != 0) {
		wlog("%s: '%s' seek error", __FUNCTION__, fn);
		fclose(f);
		return false;
	}

	sz = ftell(f);
	fseek(f, 0L, SEEK_SET);
	printf("file sz %d\n", sz);
	int compressed;
	sbuf = new char[CHUNK_SZ + 1024]; 
	rbuf = new char[CHUNK_SZ + 1024]; 
	buf  = new char[CHUNK_SZ + 1024]; 
	ofs  = 0;
	FTOffset = ofs;

	soc.bufFlush();
	soc.clearTcpBuf();

	int src_mode = mode;
	while (sz > 0) {

		rsz = (sz > CHUNK_SZ) ? CHUNK_SZ : sz;
		printf("rsz %d\n", rsz);
		if (fread(rbuf, rsz, 1, f) != 1) {
			wlog("%s: file read error\n", __FUNCTION__);
			end();
			ret = false;
			goto errexit;
		}

		pp = rbuf;
		send_sz = rsz;

		comp_sz = 0;
		compressed = 0;

		mode = src_mode;
		if ((mode & PKT_COMPRESSED) != 0 && rsz > 32)
		{
			comp_sz = compress(rbuf, rsz, buf, rsz);
			if (comp_sz > 0) {
				mode &= ~PKT_COMPRESSED;
				pp = buf;
				send_sz = comp_sz;
				compressed = 1;
			}
		}

		Ack_offset = ofs;
		if (client == -1) {
			l = sprintf(sbuf, "<star-p path=\"%s\"><send-file dir=\"%s\" name=\"%s\" ofs=\"%d\" size=\"%d\" compress=\"%s\" org_sz=\"%d\">", path, dir, fn, ofs, 
				send_sz, compressed == 1 ? "yes" : "no", rsz);
		} else {
			l = sprintf(sbuf, "<star-p client=\"%d\" path=\"%s\"><send-file dir=\"%s\" name=\"%s\" ofs=\"%d\" size=\"%d\" compress=\"%s\" org_sz=\"%d\">", client, path, dir, fn, ofs, 
				send_sz, compressed == 1 ? "yes" : "no", rsz);
		}
	
		p = sbuf + l;
		memcpy(p, pp, send_sz);
		l += send_sz + sprintf(p + send_sz, "</send-file></star-p>");

		sz -= rsz;
		ofs += rsz;

		if (starWrite(mode, l, sbuf) == S_ERR_GENERAL  || soc.bufFlush() == S_ERR_GENERAL) {
			wlog("%s: failed on send2\n", __FUNCTION__);
			end();
			ret = false;
			goto errexit;
		}

#ifdef USE_FT_HANDSHAKE
		if ((Ack_offset % HANDSHAKE_SZ) == 0 && Ack_offset != 0)
		{
			bWaitResponse = true;
			FTOffset = Ack_offset;

			if (!starWaitResponse("SEND-FILE", 120)) {
				wlog("%s: failed on wait response\n", __FUNCTION__);
				end();
				ret = false;
				bWaitResponse = false;
				goto errexit;
			}
			bWaitResponse = false;
		}
#else
		bool cmdret;
		if (starGetResponse("SEND-FILE", cmdret)) {

			if (!cmdret) {
				wlog("%s: failed: ret=%s\n", __FUNCTION__, packet.buf);
				ret = false;
				goto errexit;
			}
		}
#endif
	}

	mode = src_mode & (~PKT_COMPRESSED);
	l = sprintf(buf, "<star-p path=\"%s\"><send-file dir=\"%s\" name=\"%s\" ofs=\"-1\" size=\"1\" compress=\"no\"></send-file></star-p>",
		path, dir, fn);

	
	if (starWrite(mode, l, buf) == S_ERR_GENERAL || soc.bufFlush() == S_ERR_GENERAL) {
		wlog("%s: failed on last write\n", __FUNCTION__);
		end();
		ret = false;
		goto errexit;
	}
	ret = true;		

errexit:
	fclose(f);
	delete buf;
	delete sbuf;
	delete rbuf;
	return ret;
}



#define HLOG			15
#define STRICT_ALIGN	!defined(__i386)

typedef unsigned char u8;

#if !STRICT_ALIGN
# include <limits.h>
# if USHRT_MAX == 65535
typedef unsigned short u16;
# elif UINT_MAX == 65535
typedef unsigned int u16;
# else
#  warn need 16 bit datatype when STRICT_ALIGN == 0
#  undef STRICT_ALIGN
#  define STRICT_ALIGN 1
# endif
#endif

#include <string.h>
#include <errno.h>

#define HSIZE (1 << (HLOG))

#define FRST(p) (((p[0]) << 8) + p[1])
#define NEXT(v,p) (((v) << 8) + p[2])
#define IDX(h) ((((h ^ (h << 4)) >> (3*8 - HLOG)) + h*3) & (HSIZE - 1))

#define        MAX_LIT        (1 <<  5)
#define        MAX_OFF        (1 << 13)
#define        MAX_REF        ((1 <<  8) + (1 << 3))


unsigned int star_session::compress( const void *const in_data, unsigned int in_len, void *out_data, unsigned int out_len )
{
	const u8 *htab [ HSIZE ];
	const u8 *ip = (u8 *) in_data;
	u8 *op = (u8 *) out_data;
	const u8 *in_end  = ip + in_len;
	u8 *out_end = op + out_len;
	const u8 *ref;

	unsigned int hval = FRST ( ip );
	unsigned int off;
	int lit = 0;

	for ( off = 0; off < HSIZE; off++ )
		htab [ off ] = ip;

	do
	{
		hval = NEXT ( hval, ip );
		off = IDX ( hval );
		ref = htab [ off ];
		htab [ off ] = ip;

		if ( 1
		&& ref < ip /* the next test will actually take care of this, but it is faster */
		&& ( off = ip - ref - 1 ) < MAX_OFF
		&& ip + 4 < in_end
		&& ref > (u8 *) in_data
#if STRICT_ALIGN
		&& ref [ 0 ] == ip [ 0 ]
		&& ref [ 1 ] == ip [ 1 ]
		&& ref [ 2 ] == ip [ 2 ]
#else
		&& *(u16 *)ref == *(u16 *)ip
		&& ref [ 2 ] == ip [ 2 ]
#endif
		)
		{
			/* match found at *ref++ */
			unsigned int len = 2;
			unsigned int maxlen = in_end - ip - len;
			maxlen = maxlen > MAX_REF ? MAX_REF : maxlen;

			do
				len++;
			while ( len < maxlen && ref [ len ] == ip [ len ] );

			if ( op + lit + 1 + 3 >= out_end )
				return 0;

			if ( lit )
			{
				*op++ = (unsigned char)(lit - 1);
				lit = -lit;
				do
				*op++ = ip [ lit ];
				while ( ++lit );
			}

			len -= 2;
			ip++;

			if ( len < 7 )
			{
				*op++ = (unsigned char)(( off >> 8 ) + ( len << 5 ));
			}
			else
			{
				*op++ = (unsigned char)(( off >> 8 ) + ( 7 << 5 ));
				*op++ = (unsigned char)(len - 7);
			}

			*op++ = (unsigned char)(off);

			do
			{
				hval = NEXT ( hval, ip );
				htab [ IDX ( hval ) ] = ip;
				ip++;
			}
			while ( len-- );
		}
		else
		{
			/* one more literal byte we must copy */
			lit++;
			ip++;

			if ( lit == MAX_LIT )
			{
				if ( op + 1 + MAX_LIT >= out_end )
				return 0;

				*op++ = MAX_LIT - 1;
				lit = -lit;
				do
					*op++ = ip [ lit ];
				while ( ++lit );
			}
		}
	}
	while ( ip < in_end );

	if ( lit )
	{
		if ( op + lit + 1 >= out_end )
		return 0;

		*op++ = (unsigned char)(lit - 1);
		lit = -lit;
		do
		*op++ = ip [ lit ];
		while ( ++lit );
	}

	return op - (u8 *) out_data;
}

unsigned int star_session::decompress ( const void *const in_data,  unsigned int in_len, void *out_data, unsigned int out_len )
{
	u8 const *ip = (u8 *) in_data;
	u8       *op = (u8 *) out_data;
	u8 const *const in_end  = ip + in_len;
	u8       *const out_end = op + out_len;

	do
	{
		unsigned int ctrl = *ip++;

		if ( ctrl < ( 1 << 5 ) ) /* literal run */
		{
			ctrl++;

			if ( op + ctrl > out_end )
			{
				errno = E2BIG;
				return 0;
			}

			do
				*op++ = *ip++;
			while ( --ctrl );
		}
		else /* back reference */
		{
			unsigned int len = ctrl >> 5;

			u8 *ref = op - ( ( ctrl & 0x1f ) << 8 ) - 1;

			if ( len == 7 )
				len += *ip++;

			ref -= *ip++;

			if ( op + len + 2 > out_end )
			{
				errno = E2BIG;
				return 0;
			}

			if ( ref < (u8 *) out_data )
			{
				errno = EINVAL;
				return 0;
			}

			*op++ = *ref++;
			*op++ = *ref++;

			do
				*op++ = *ref++;
			while ( --len );
		}
	}
	while ( op < out_end && ip < in_end );

	return op - (u8 *) out_data;
}

