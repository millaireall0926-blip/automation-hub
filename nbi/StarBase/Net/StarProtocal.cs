using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace StarBase.Net 
{
    public class StarProtocal
    {
        public const int HDR_MAGIC = unchecked((int)0xC0DECAFE);
		public static readonly int HEADER_SIZE = Marshal.SizeOf<StarHeader>();
		
	}

    public class HandlerProtocal
    {
        public const short HDR_MAGIC = unchecked((short)0xC0DE);
		public static readonly int HEADER_SIZE = 5;//Marshal.SizeOf<HandlerHeader>();
                                                   //  HEADER_SIZE 가 5이면 body가 0이 되므로..
												   //  body 0은 Header의 복사가 안됨..
												   //  type을 body로 사용.
    }

    public enum HANLDER_TYPE : int
	{
		LOAD_DONE = 0x1,
		TEST_DONE = 0x10,
		DEV_TYPE_SEL = 0x02,
		PGM_DONE = 0x20,
        REQ_DEV_TYPE = 0x03,
        DEV_TYPE_CONFIG = 0x30

	}



    public enum MSG_TYPE : int
    {
		COMM_BUF_SZ = 256 * 1024, 
		CHUNK_SZ = 64 * 1024,
		

		// read state
		STATE_HEADER = 0,
		STATE_BODY = 1,

		SESSION_NULL = 0,
		SESSION_LISTEN = 1, // for accepting connection
		SESSION_DEFAULT = 2,
		SESSION_OS = 3,		// for communicate with BIT
		SESSION_PGM = 4,    //
		SESSION_OBSERVER = 6,   // packet observer
		SESSION_PLC = 7,    // system monitor
		SESSION_DIAG = 9,   // diag		
		SESSION_MAINT = 11,    // system test client
		SESSION_ROUTER = 12,    // dist daemon (client)
		SESSION_WEB = 13,

		// packet destination
		DEST_DAEMON = SESSION_LISTEN,
		DEST_OS = SESSION_OS,
		DEST_PGM = SESSION_PGM,
		DEST_OBSERVER = SESSION_OBSERVER,
		DEST_PLC = SESSION_PLC,
		DEST_DIAG = SESSION_DIAG,
		DEST_SYSCLI = SESSION_MAINT,
		DEST_ROUTER = SESSION_ROUTER,
		DEST_WEB = SESSION_WEB,

        // packet mode
        TYPE_ADMIN		= 0x0100,
        TYPE_RESULT		= 0x0200,
        TYPE_SHM		= 0x0300,
        TYPE_REQFILE	= 0x0400,
        TYPE_SLOTSTAT	= 0x0800,
        TYPE_REQDONE	= 0x1000,
        TYPE_SENDFILE	= 0x2000,

        TYPE_COMPRESSED = 0x10000,

		// internal xml parser
		STAR_XML_NOCMD = 0,
		STAR_XML_CMD,
		STAR_XML_CMD_CLOSE,
		STAR_XML_CLOSE,
		STAR_XML_HASATT = 0x80
	}

	public enum RESULT_CMD : int 
	{
		CMD_IDENTIFY = 0,
		CMD_CU_LIST = 1,
		CMD_CU_LIST_AUTO,
		CMD_VZONE_LIST,
		CMD_PLC_CONFIG,
		CMD_VZONE_SET,
		CMD_SEND_FILE,
		CMD_STATUS,
		CMD_PROC,
		CMD_PGM,
		CMD_DIAG,
		CMD_ROM,
		CMD_CM_CTRL,
		CMD_REQ_FILE,
		CMD_UNKNOWN
	}

	enum ERR_TYPE
	{
		// system operation error code
		S_ERR_NONE = 1,
		S_ERR_GENERAL = 0,
		S_ERR_NOMEM = -1,
		S_ERR_QUEUED = -2
	}

	[Serializable, StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct StarHeader
    {
        public int Magic;
        public int Mode;
        public int Len;
        public int OrgLen;
    }

    [Serializable, StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct HandlerHeader
	{
		public short Magic;
		public short Len;
		public byte Type;
		
	}



}
