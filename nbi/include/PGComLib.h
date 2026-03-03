#pragma once

#include <bitset>
#include <vector>
#include <map>



#define byte  unsigned char
#define word  unsigned short
#define dword unsigned int
#define qword uint64_t

#ifdef __linux__
#define __int64 __int64_t
#endif

#define INST_SIZE 64//48
#define REG_SIZE 240 //228 + 12 
#define TIMER_RES  (0.00000001 - DBL_EPSILON)
//#define MAX_NAME_LENGTH 39
#define MAX_PATTERN_SIZE 4096 //2048

#define ENCODE(cmd,opt,val) ( ((uint64_t)cmd<<32) | ((uint64_t)opt<<16) | (val) )

union ITEM
{
	qword value;
	struct {
		dword bit : 16;
		dword opt : 16;
		dword cmd;
	};

	ITEM() : value(0) {}
	ITEM(qword val) { value = val; }
	ITEM(int cmd, int opt, int bit)
	{
		this->cmd = cmd;
		this->opt = opt;
		this->bit = bit;
	}
};

typedef std::vector<std::vector<ITEM>> grammar_tree;
typedef std::map<std::string, grammar_tree> grammar_collage;
typedef std::map < std::string, uint64_t > SyntaxMap;

//typedef union tagCMD
//{
//	qword value;
//	ITEM item;
//	//struct
//	//{
//	//	dword bit : 16;
//	//	dword opt : 16;
//	//	dword cmd;
//	//};
//}CMD;

enum COMP_TYPE {
	COMMON = 1,
	ADDR_DEF,
	REGISTER,
	PATTERN,
};

namespace PAT_TYPE {
	enum {
		PATTERN = 0,
		FUNCTION,
	};
}

enum KEY_TYPE {
	KEY_NULL = 0,
	KEY_STR  = 1, // 일반 문자 IDX0 ...
	KEY_INT  = 2, // 10진수 
	KEY_XINT = 4, // 16진수 0x1F
	KEY_FLT  = 8, // 부동소수 1.23 ...
	KEY_SYM  = 0x10, // 특수문자 =,# ...

	KEY_INV = 0x20,
	KEY_INC = 0x30,
	KEY_DEC = 0x40,
	KEY_ADD = 0x80,
	KEY_SUB = 0x90,

};

namespace REG
{
	enum
	{
		IDX0 =  0,
		IDX1 =  3,
		IDX2 =  6,
		IDX3 =  9,
		IDX4 = 12,
		IDX5 = 15,
		IDX6 = 18,
		IDX7 = 21,
		IDX8 = 24,
		IDX9 = 27,
		IDX10 = 30,
		IDX11 = 33,
		IDX12 = 36,
		IDX13 = 39,
		IDX14 = 42,
		IDX15 = 45,

		XMAX = 48,
		YMAX = 51,
		//ZMAX = 54,
		LMAX = 55,
		HMAX = 58,

		XH = 61,
		XT = 64,
		YH = 67,
		YT = 70,
		//ZH = 73,

		D1A = 74,
		D1B = 77,
		D1C = 80,
		D1D = 83,
		D1E = 86,
		D1F = 89,
		D1G = 92,
		D1H = 95,
		D2A = 98,
		D2B = 101,
		D2C = 104,
		D2D = 107,
		D3B = 110,
		D4B = 113,

		TPH1 = 116,
		TPH1A = 119,
		TPH1B = 122,
		TPH1C = 125,
		TPH1D = 128,
		TPH2 = 131,
		TPH2A = 134,
		TPH2B = 137,
		TPH2C = 140,
		TPH2D = 143,

		D5 = 146,
		D6 = 149,
		D7 = 152,
		D8 = 155,
		XOS = 158,
		YOS = 161,
		DCMR = 164,
		DCMR2 = 167,
		DBMAP = 170,
		TIMER = 172,
		PWAIT = 175,
		ISP = 178,
		REFTIME = 180,
		CFLG = 183,
		REF_MODE = 185,
		SCAN = 186,
		VMPA = 189,
		STATUS = 192,

		XMASK = 194,
		YMASK = 197,
		//ZMASK = 200,
		CCD = 201,
		RCD = 204,
		DSD = 207,

		DCMRH = 210,
		DCMRL = 213,
		//BL = 216,
		//BT = 217,
		//WL = 218,
		//RL = 219,
		//XMIN = 220,
		//YMIN = 223,
		////ZMIN = 226,
		//S1A = 227,
		//ZMAX = 228,
		//ZH = 230,
		//ZMASK = 232,
		//ZMIN = 234,

		//MODE = 216,
		BL = 217,
		BT = 218,
		WL = 219,
		RL = 220,
		XMIN = 221,
		YMIN = 224,
		//ZMIN = 227,
		S1A = 228,
		MLOOP = 229,
		ZMAX = 230,
		ZH = 232,
		ZMASK = 234,
		ZMIN = 236,
	};
}

namespace REFRESH
{
	enum
	{
		A = 0,
		B
	};
}

namespace PAT_OP 
{
	enum
	{
		NOP   = 0,
		FLGLI = 1,
		JZD   = 2,
		JET   = 3,
		RTN   = 4,
		JRB   = 5,
		STPS  = 6,
		JMP,
		JSR,
		OUT,
		REF_START, // T0
		TM_START,  //T1
		TIMER0,//SET Refresh Time
		TIMER1,//SET Jet Time
		REPEAT,
		PAUSE,
		JNC0 = 0x10,
		JNC1,
		JNC2,
		JNC3,
		JNC4,
		JNC5,
		JNC6,
		JNC7,
		JNC8,
		JNC9,
		JNC10,
		JNC11,
		JNC12,
		JNC13,
		JNC14,
		JNC15,

		JNI0 = 0x20,
		JNI1,
		JNI2,
		JNI3,
		JNI4,
		JNI5,
		JNI6,
		JNI7,
		JNI8,
		JNI9,
		JNI10,
		JNI11,
		JNI12,
		JNI13,
		JNI14,
		JNI15,

		JNI0D = 0x30,
		JNI1D,
		JNI2D,
		JNI3D,
		JNI4D,
		JNI5D,
		JNI6D,
		JNI7D,

		INC0,
		INC1,
		INC2,
		INC3,
		INC4,
		INC5,
		INC6,
		INC7,

		STI0 = 0x40,
		STI1,
		STI2,
		STI3,
		STI4,
		STI5,
		STI6,
		STI7,
		STI8,

		STBAR,
		STISP,

		LDI0,
		LDI1,
		LDI2,
		LDI3,
		LDI4,
		LDI5,
		LDI6,
		LDI7 = 82,
		MODE = 93,
		VMPA,
	};
}

namespace OP_CTRL
{
	enum
	{
		I = 1,
		T1 = 2,
		T0 = 4,
	};
}


namespace AB_A
{
	enum
	{
		XH = 0,
		YH = XH,
		D1A,
		D1B,
		D1C,
		D1D,
		D1E,
		D1F,
		D1G,
		D1H,
		D2A,
		D2B,
		D2C,
		D2D,
				
		XOS,
		XT,
		D3B,
				
		YOS = XOS,
		YT  = XT,
		D4B = D3B,
	};
}

namespace AB_ALU { // Address Base
	enum {
		/* 0*/ XB = 0,      // "XB"
		/* 0*/ YB = 0,      // "YB"
		/* 0*/ Z  = 0,      // "Z"
		/* 1*/ RESET,       // "0" 
		/* 2*/ INVERT,      // "/" 
		/* 3*/ INCREASE,    // "+1"
		/* 4*/ DECREASE,    // "-1"
		/* 5*/ LSHIFT,      // "*2"
		/* 6*/ RSHIFT,      // "*2"
		/* 7*/ ASSIGN_A,
		/* 8*/ ADD,         // "+"
		/* 9*/ SUB,         // "-"
		/* A*/ IMM,		    // "0x1234"
		/* A*/ ZH_IMM = IMM,	// "ZH<0x1234"
		/* B*/ ADD_IMM,     // "+ 0xFF"
		/* C*/ SUB_IMM,     // "- 0xFF"
		/* F*/ REG_SET = 0xF,// "D1A = 0x1234"
	};

	enum {
		/* 0*/ D3 = 0,      // "D3"
		/* 0*/ D4 = 0,      // "D4"
		/* 2*/ D3B = INVERT,// "D3B" 
		/* 2*/ D4B = INVERT,// "D4B" 
	};
}

namespace AN_A
{
	enum
	{
		XC = 0,
		XS,
		XK,
		XA,
		XD,
		XE,
		XF,
		XB,
	};

	enum
	{
		YC = 0,
		YS,
		YK,
		YA,
		YD,
		YE,
		YF,
		YB,
	};
}

namespace AN_B
{
	enum
	{
		D1A = 0,
		D1B,
		D1C,
		D1D,
		D1E,
		D1F,
		D1G,
		D1H,
		D2A,
		D2B,
		D2C,
		D2D,
		D3,
		D4,
		XMAX,
		YMAX = XMAX,
	};
}

//namespace XN_ALU {
//	enum {
//		/* 0*/ XN = 0,    // "XC, XS ..."
//		/* 1*/ RESET,     // "0" 
//		/* 2*/ INVERT,    // "/" 
//		/* 3*/ INCREASE,  // "+1"
//		/* 4*/ DECREASE,  // "-1"
//		/* 5*/ LSHIFT,    // "*2"
//		/* 6*/ RSHIFT,    // "*2"
//		/* 7*/ ASSIGN_B,
//		/* 8*/ ADD,       // "+"
//		/* 9*/ SUB,       // "-"
//		/*10*/ AND,      // "&"
//		/*11*/ OR,       // "|"
//		/*12*/ XOR,      // "^"
//
//		/*14*/ XIMM = 14,// "0x1234"...
//
//	};
//}

namespace AN_ALU {
	enum {
		/* 0*/ XN = 0,    // "XC, XS ..."
		/* 0*/ YN = 0,    // "XC, XS ..."
		/* 1*/ RESET,     // "0" 
		/* 2*/ INVERT,    // "/" 
		/* 3*/ INCREASE,  // "+1"
		/* 4*/ DECREASE,  // "-1"
		/* 5*/ LSHIFT,    // "*2"
		/* 6*/ RSHIFT,    // "*2"
		/* 7*/ ASSIGN_B,
		/* 8*/ ADD,       // "+"
		/* 9*/ SUB,       // "-"
		/*10*/ AND,       // "&"
		/*11*/ OR,        // "|"
		/*12*/ XOR,       // "^"
		/*13*/ ADD_IMM,   // "+ 0x1234"
		/*14*/ IMM = 14,  // "0x1234"
		/*15*/ SUB_IMM,   // "- 0x1234"
	};
}


namespace AN_SEL
{
	enum
	{
		XC = 0,
		XS,
		XK,
		XA,
		XD,
		XE,
		XF,
	};

	enum
	{
		YC = 0,
		YS,
		YK,
		YA,
		YD,
		YE,
		YF,
	};
}

//namespace YB_A
//{
//	enum
//	{
//		D1A = 0,
//		D1B,
//		D1C,
//		D1D,
//		D1E,
//		D1F,
//		D1G,
//		D1H,
//		D2A,
//		D2B,
//		D2C,
//		D2D,
//		YH,
//		YOS,
//		YT,
//		D4B,
//	};
//}

//namespace YB_ALU {
//	enum {
//		/* 0*/ YB = 0,    // "XB"
//		/* 1*/ RESET,     // "0" 
//		/* 2*/ INVERT,    // "/" 
//		/* 3*/ INCREASE,  // "+1"
//		/* 4*/ DECREASE,  // "-1"
//		/* 5*/ LSHIFT,    // "*2"
//		/* 6*/ RSHIFT,    // "*2"
//		/* 7*/ ASSIGN_A,
//		/* 8*/ ADD,       // "+"
//		/* 9*/ SUB,       // "-"
//
//		/*10*/ XIMM = 14,      // "0x1234"...
//
//	};
//}

//namespace YN_SEL
//{
//	enum
//	{
//		YC = 0,
//		YS,
//		YK,
//		YA,
//		YD,
//		YE,
//		YF,
//	};
//}

//namespace YN_A
//{
//	enum
//	{
//		YC = 0,
//		YS,
//		YK,
//		YA,
//		YD,
//		YE,
//		YF,
//		YB,
//	};
//}

//namespace YN_B
//{
//	enum
//	{
//		D1A = 0,
//		D1B,
//		D1C,
//		D1D,
//		D1E,
//		D1F,
//		D1G,
//		D1H,
//		D2A,
//		D2B,
//		D2C,
//		D2D,
//		D3,
//		D4,
//		YMAX,
//	};
//}

//namespace YN_ALU {
//	enum {
//		/* 0*/ YN = 0,    // "XC, XS ..."
//		/* 1*/ RESET,     // "0" 
//		/* 2*/ INVERT,    // "/" 
//		/* 3*/ INCREASE,  // "+1"
//		/* 4*/ DECREASE,  // "-1"
//		/* 5*/ LSHIFT,    // "*2"
//		/* 6*/ RSHIFT,    // "*2"
//		/* 7*/ ASSIGN_B,
//		/* 8*/ ADD,       // "+"
//		/* 9*/ SUB,       // "-"
//		/*10*/ AND,      // "&"
//		/*11*/ OR,       // "|"
//		/*12*/ XOR,      // "^"
//
//		/*14*/ YIMM = 14,// "0x1234"...
//	};
//}

namespace Z_A
{
	enum
	{
		ZH = 12,
		ZMAX = 14,
	};
}

namespace X_OUT
{
	enum
	{
		X = 0,
		XC,
		XS,
		XK,
		XA,
		XD,
		XE,
		XF,
		XB,

		YC,
		YS,
		YK,
		YA,
		YD,
		YE,
		YF,
		YB,

		XT,
		XOS,
		Z,
		IMMX,
		RF,
	};
}

namespace Y_OUT
{
	enum
	{
		Y = 0,
		YC,
		YS,
		YK,
		YA,
		YD,
		YE,
		YF,
		YB,

		XC,
		XS,
		XK,
		XA,
		XD,
		XE,
		XF,
		XB,

		YT,
		YOS,
		Z,
		IMMY,

	};
}

//namespace TP_ALU {
//	enum {
//		/* 0*/ TP1 = 0,    // "XC, XS ..."
//		/* 0*/ TP2 = 0,    // "XC, XS ..."
//
//		/* 2*/ INVERT = 2, // "/" 
//		/* 3*/ INCREASE,   // "+1"
//		/* 4*/ DECREASE,   // "-1"
//		/* 5*/ ADD,        // "+"
//		/* 6*/ SUB,        // "-"
//		/* 7*/ LSHIFT,     // "*2"
//		/* 8*/ RSHIFT,     // "*2"
//		
//		/* 9*/ ASSIGN_B,
//		
//		/*11*/ AND = 11,   // "&"
//		/*12*/ OR,         // "|"
//
//		/*14*/ TP1IMM = 14,// "0x1234"...
//		/*14*/ TP2IMM = 14,// "0x1234"...
//		/*15*/ REGSET = 15,// "0x1234"...
//	};
//}

namespace TP_ALU {
	enum {
		/* 0*/ TP1 = 0,    // "XC, XS ..."
		/* 0*/ TP2 = 0,    // "XC, XS ..."

		/* 1*/ TP1IMM = 1,// "0x1234"...
		/* 1*/ TP2IMM = 1,// "0x1234"...

		/* 2*/ INVERT = 2, // "/" 
		/* 3*/ INCREASE,   // "+1"
		/* 4*/ DECREASE,   // "-1"
		/* 5*/ LSHIFT,     // "*2"
		/* 6*/ RSHIFT,     // "*2"
		/* 7*/ ASSIGN_B,
		/* 8*/ ADD,        // "+"
		/* 9*/ SUB,        // "-"
		/*10*/ AND,		   // "&"
		/*11*/ OR,         // "|"
		/*12*/ INV_ASSIGN_B,// "/D5"
		/*13*/ TPH_ASSIGN_TP, // TPH1<TP1
		/*14*/ TPX_IMM, // TPH1<TP1
		/*15*/ REG_SET = 0xF,// "0x1234"...
	};
}

namespace TP1_A
{
	enum
	{
		TPH1 = 0,
		TPH1A,
		TPH1B,
		TPH1C,
		TPH1D,
		D5,
		D7,
		XMASK,
		IMM,
	};
}

namespace TP2_A
{
	enum
	{
		TPH2 = 0,
		TPH2A,
		TPH2B,
		TPH2C,
		TPH2D,
		D6,
		D8,
		YMASK,
		IMM,
	};
}

namespace D78_ALU {
	enum {
		/* 0*/ D7 = 0,    // "XC, XS ..."
		/* 0*/ D8 = 0,    // "XC, XS ..."
		/* 1*/ RESET,     // "0" 
		/* 2*/ ASSIGN_B,
		/* 3*/ INCREASE,  // "+1"
		/* 4*/ DECREASE,  // "-1"
		/* 5*/ ADD,       // "+"
		/* 6*/ SUB,       // "-"
		/* 7*/ RSHIFT,    // "*2"
	};
}


namespace D7_A
{
	enum
	{
		TP1 = 0,
		TPH1,
		TPH1A,
		TPH1B,
		TPH1C,
		TPH1D,
		D5,
	};
}

namespace D8_A
{
	enum
	{
		TP2 = 0,
		TPH2,
		TPH2A,
		TPH2B,
		TPH2C,
		TPH2D,
		D6,
	};
}

namespace SAB_ALU {
	enum {
		/* 0*/ SA = 0,    // "SA"
		/* 0*/ SB = 0,    // "SB"
		/* 1*/ RESET,     // "0" 
		/* 2*/ RIVAL,     // "SA<SB or SB<SA" 
		/* 3*/ IMM,
		/* 4*/ REG,       // "SA<S1A"
		/* 5*/ ADD,       // "+"
		/* 6*/ SUB,       // "-"
		
	};
}

namespace FP_SEL
{
	enum
	{
		FP0 = 1,
		FP2,
		FP3,
		FP4,
		FP5,
		FP6,
		FP7,
		FP8,
		FP9,
		FP10,
		FPX,
	};
}

namespace CTRL
{
	enum
	{
		ASCROFF = 1,
		DSCROFF = 2,
		TPX1 = 4,
		TPX2 = 8,

		XINV = 0x40,
		YINV = 0x80,
		ZINV = 0x100,
		D1INV = 0x200,
		D2INV = 0x400,
		BS = 0x800,
		SP = 0x1000,
		M2 = 0x2000,
		RINV0 = 0x4000,
		RINV1 = 0x8000,

	};
}

namespace CBIT_CMD
{
	enum
	{
		C0 = 0,
		C1,
		C2,
		C3,
		C4,
		C5,
		C6,
		C7,
		C8,
		C9,
		C10,
		C11,
		C12,
		C13,
		C14,
		C15,

		C16,
		C17,
		C18,
		C19,
		C20,
		C21,
		C22,
		C23,
		C24,
		C25,
		C26,
		C27,
		C28,
		C29,
		C30,
		C31,

		C32,
		C33,
		C34,
		C35,
		C36,
		C37,
		C38,
		C39,
	};
}

namespace CYP
{
	enum
	{
		CYP0 = 0,
		CYP1,
		CYP2,
		CYP3,
		CYP4,
		CYP5,
		CYP6,
		CYP7,
		CYP8,
		CYP9,
		CYP10,
		CYP11,
		CYP12,
		CYP13,
		CYP14,
		CYP15,
	};
}

namespace TSET
{
	enum
	{
		TS0 = 0,
		TS1,
		TS2,
		TS3,
		TS4,
		TS5,
		TS6,
		TS7,
		TS8,
		TS9,
		TS10,
		TS11,
		TS12,
		TS13,
		TS14,
		TS15,
	};
}

namespace NAND_CMD
{
	enum
	{
		end_page = 0,
		end_block,
		VM,
		VMINC, //INFOINC
	};
}

namespace KOCC //Kind of Common Command
{
	enum {
		C_EXIT = 1,
		C_ASSIGN,

		C_DEF,
		C_BEGIN_REG_BLK,
		C_END_REG_BLK,
		C_BEGIN_PAT_BLK,
		C_END_PAT_BLK,
		C_BEGIN_FUNC_BLK,
		C_END_FUNC_BLK,
		C_INSERT, // INSERT
		C_IF_INSERT, // %INSERT (조건부 컴파일에 영향 받지 않음)
		C_IFE_COMP, // %IFE 입력 옵션 조건부 컴파일  (-c AF)
		C_IFN_COMP, // %IFN 입력 옵션 조건부 컴파일  (-c AF)
		C_ENDC,

		C_ROUND_LEFT,
		C_ROUND_RIGHT,
		C_OP_UNARY, // 단항 연산자 // ".NOT."
		C_OP_INFIX, // 이항 연산자 // ".AND.", ".OR.", ".XOR."
		C_ALPHA_CHAR, // A, B, C, D, E, F ~~ Z
	};

}

namespace KORC //Kind of Register Command
{
	enum {
		C_EXIT = 1,
		C_ASSIGN,
		C_UNIT,

		C_REG_CMD,
		C_TIME_CMD,
		C_ISP_CMD,
		C_MODE_CMD,
		C_OPERAND,
		C_DBLOPERAND,

		C_SIMPLE,

		C_UINT,
		C_FLOAT,
		C_STRING,
		C_MODE,

	};
}

namespace KOPC //Kind of Pattern Command
{
	enum {
		C_EXIT = 1,
		C_ASSIGN,
		C_UNIT,

		C_OPRN_CMD,
		C_OPERAND,
		C_DBLOPERAND,

		C_SIMPLE,

		C_OPCODE,
		C_LONE_OP,
		C_JUMP_OP,
		C_SET_OP,
		C_SET_OP_STI,
		C_SET_OP_TIMER,
		C_OPRN_OP,
		C_MODE_OP,

		C_OP_CTRL,

		C_SRC_BASE,
		/// 순서 유지 중요
		C_SRC_SEL,  //13 (0xd)
		C_RESET,
		C_INV,
		C_INCREASE,
		C_DECREASE,
		C_LSHIFT,
		C_RSHIFT,
		C_OTH,
		C_ADD,
		C_SUB,
		C_AND,
		C_OR,
		C_XOR,
		/// 여기까지

		C_XB,
		C_XB_ALU,
		C_XB_A,

		C_XN,
		C_XN_ALU,
		C_XN_B,

		C_YB,
		C_YB_ALU,
		C_YB_A,

		C_YN,
		C_YN_ALU,
		C_YN_B,

		C_D3,
		C_D4,

		C_Z,
		C_Z_A,

		C_X,
		C_Y,

		C_TP1,
		C_TP2,

		C_TPH1,
		C_TPH2,

		C_D7,
		C_D8,

		C_SA,
		C_SB,
		C_S,
		C_RF,
		C_FP,

		C_CTRL,
		C_CMD,
		C_CYP,
		C_TS,
		C_NAND_CTRL,

		C_IMM,
		C_TPX,
		C_D1,
		C_D2,
		C_REG_GROUP_SET

	};

}

enum
{
	OP_AND = 0,
	OP_OR,
	OP_XOR,
	OP_NOT,
};

namespace OPT //Option Command
{
	enum {
		O_UINT = 1,
		O_INT,
		O_DOUBLE,

		O_IFE,
		O_IFN,

		O_MS,
		O_US,
		O_NS,

		O_XB,
		O_XB_ALU,
		O_XB_A,

		O_XN,
		O_XN_SEL,
		O_XN_ALU,
		O_XN_A,
		O_XN_B,
		O_XN_IMM,

		O_YB,
		O_YB_ALU,
		O_YB_A,

		O_YN,
		O_YN_SEL,
		O_YN_ALU,
		O_YN_A,
		O_YN_B,
		O_YN_IMM,

		O_D3,
		O_D4,

		O_DX,

		O_Z,
		O_Z_ALU,
		O_Z_A,
		O_Z_IMM,

		O_X,
		O_Y,

		O_TP1_ALU,
		O_TP1_A,

		O_TP2_ALU,
		O_TP2_A,

		O_TPH1X,
		O_TPH2X,

		O_D7,
		O_D7_A,

		O_D8,
		O_D8_A,

		O_SA,
		O_SB,

		O_S,
		O_RF,

		O_CMD0,
		O_CMD1,
		O_CMD2,
	};
}

namespace IMM_IDX
{
	enum {
		XMAX = 0,
		YMAX,
		ZMAX,
		ZH,
		TPX1,
		TPX2,
	};
}

namespace D1X_IDX
{
	enum {
		XH = 0,
		D1A,
		D1B,
		D1C,
		D1D,
		D1E,
		D1F,
		D1G,
		D1H,
		//XH=0xC,
		XOS=0xD,
		XT=0xE,
		D3B = 0xF,
	};
}

namespace D2X_IDX
{
	enum {
		YH = 0,
		D2A = 9,
		D2B,
		D2C,
		D2D,
		YOS = 0xD,
		YT = 0xE,
		D4B = 0xF,
	};
}

typedef struct soda{
	char token[8];
	ITEM item;
	bool result;
}soda_t;

#pragma pack(push ,1)
typedef union op_command {
	dword data[2];
	struct {
		qword opcode  : 8;
		qword op_ctrl : 8;
		qword operand : 24;
		qword flash_cmd : 16;
	};
}op_command_t;

//typedef union op_command {
//	byte data[7];
//	struct {
//		byte opcode : 8;
//		dword op_ctrl : 8;
//		dword operand : 24;
//		word flash_cmd : 16;
//	};
//}op_command_t;

typedef union xb {
	byte data;
	struct {
		byte a : 4;
		byte alu : 4;
	};
}xb_t;

typedef union xn {
	byte data[5];
	struct {
		byte  sel :  4;
		byte  a   :  4;
		dword b   :  4;
		dword alu :  4;
		dword imm : 24;
	};
}xn_t;


typedef union yb {
	byte data;
	struct {
		byte a   : 4;
		byte alu : 4;
	};
}yb_t;

typedef union yn {
	byte data[5];
	struct {
		byte  sel :  4;
		byte  a   :  4;
		dword b   :  4;
		dword alu :  4;
		dword imm : 24;
	};
}yn_t;

typedef union d34 {
	byte data;
	struct {
		byte d3 : 4;
		byte d4 : 4;
	};
}d34_t;

//typedef union z {
//	word data;
//	struct {
//		word a   :  4;
//		word alu :  4;
//		word imm :  8;
//	};
//}z_t;

typedef union z {
	byte data[3];
	struct {
		byte a   :  4;
		byte alu :  4;
		word imm : 16;
	};
}z_t;

typedef union xy {
	word data;
	struct {
		byte x;
		byte y;
	};
}xy_t;

typedef union tp1 {
	dword data;
	struct {
		dword a   :  4;
		dword alu :  4;
		dword imm : 24;
	};
}tp1_t;

typedef union tp2 {
	dword data;
	struct {
		dword a   :  4;
		dword alu :  4;
		dword imm : 24;
	};
}tp2_t;

typedef union d7 {
	byte data;
	struct {
		byte a   : 4;
		byte alu : 4;
	};
}d7_t;

typedef union d8 {
	byte data;
	struct {
		byte a   : 4;
		byte alu : 4;
	};
}d8_t;

typedef union scan {
	word data;
	struct {
		byte sa  : 4;
		byte sb  : 4;
		byte imm;
	};
}scan_t;

typedef union s_rf_fp {
	byte data;
	struct {
		byte s  : 2;
		byte rf : 2;
		byte fp : 4;
	};
}s_rf_fp_t;

typedef union ctrl {
	word data;
	struct {
		word ascroff : 1;
		word dscroff : 1;
		word tpx1    : 1;
		word tpx2    : 1;
		word reserved : 2;
		word inv_x   : 1;
		word inv_y   : 1;
		word inv_z   : 1;
		word inv_d1  : 1;
		word inv_d2  : 1;
		word bs      : 1;
		word sp      : 1;
		word m2      : 1;
		word rinv0   : 1;
		word rinv1   : 1;
	};
}ctrl_t;

typedef union pgcmd {
	byte data[5];
	struct {
		word cmd0; //c00 ~ c15
		word cmd1; //c16 ~ c31
		byte cmd2; //c32 ~ c39
	};
}pgcmd_t;

typedef union  cyp_ts{
	byte data;
	struct {
		byte cyp : 4; // cyp0 ~ 15
		byte ts  : 4; // ts0 ~ 15
	};
}cyp_ts_t;

typedef union  nand_ctrl {
	byte data;
	struct {
		byte end_page  : 1;
		byte end_block : 1;
		byte vm        : 1;
		byte vm_inc    : 1; // infoinc
	};
}nand_ctrl_t;

typedef union instruction {
	word data[INST_SIZE/2];
	struct {
		op_command_t op_command;
		xb_t xb;
		xn_t xn;
		yb_t yb;
		yn_t yn;
		d34_t d34;
		z_t z;
		xy_t xy;
		tp1_t tp1;
		tp2_t tp2;
		d7_t d7;
		d8_t d8;
		scan_t scan;
		s_rf_fp_t s_rf_fp;
		ctrl_t ctrl;
		pgcmd_t pgcmd;
		cyp_ts_t cyp_ts;
		nand_ctrl_t nand_ctrl;
	};
}instruction_t;

struct reg_byte
{
	dword idx0 : 24;
};



typedef struct byte3_st
{
	byte data[3];
}byte3_st_t;

typedef struct registerset_test
{
	byte3_st_t idx[16];

	byte3_st_t xmax;
	byte3_st_t ymax;
	byte z_reserved;// zmax;
	byte3_st_t lmax;
	byte3_st_t hmax;
	byte3_st_t xh;
	byte3_st_t xt;
	byte3_st_t yh;
	byte3_st_t yt;
	byte z_reserved2;// zh;
	byte3_st_t d1a;
	byte3_st_t d1b;
	byte3_st_t d1c;
	byte3_st_t d1d;
	byte3_st_t d1e;
	byte3_st_t d1f;
	byte3_st_t d1g;
	byte3_st_t d1h;
	byte3_st_t d2a;
	byte3_st_t d2b;
	byte3_st_t d2c;
	byte3_st_t d2d;
	byte3_st_t d3b;
	byte3_st_t d4b;
	byte3_st_t tph1;
	byte3_st_t tph1a;
	byte3_st_t tph1b;
	byte3_st_t tph1c;
	byte3_st_t tph1d;

	byte3_st_t tph2;
	byte3_st_t tph2a;
	byte3_st_t tph2b;
	byte3_st_t tph2c;
	byte3_st_t tph2d;

	byte3_st_t d5;
	byte3_st_t d6;
	byte3_st_t d7;
	byte3_st_t d8;

	byte3_st_t xos;
	byte3_st_t yos;

	byte3_st_t dcmr;
	byte3_st_t dcmr2;
	word dbmap;
	byte3_st_t timer;
	byte3_st_t pwait;

	word stisp;
	byte3_st_t reftimer;
	word cflg;
	byte mod;

	byte3_st_t scan;
	byte3_st_t vmpa;
	word status;

	byte3_st_t xmask;
	byte3_st_t ymask;
	byte z_reserved3; //zmask;

	byte3_st_t ccd;
	byte3_st_t rcd;
	byte3_st_t dsd;

	byte3_st_t DCMRH;
	byte3_st_t DCMRL;

	byte mode;

	byte BL : 8;
	byte BT : 8;
	byte WL : 8;
	byte RL : 8;

	byte3_st_t xmin;
	byte3_st_t ymin;
	byte z_reserved4; //zmin;

	byte s1a;
	byte mloop;

	word zmax;
	word zh;
	word zmask;
	word zmin;

	//int reserved[3];

}registerset_test_t;

typedef union registerset
{
	byte data[REG_SIZE];
	registerset_test_t regset;
}registerset_t;


#pragma pack ( pop )

typedef union version {
	unsigned int data;
	struct {
		unsigned int rsved : 8; // reserved
		unsigned int major : 8;
		unsigned int minor : 8;
		unsigned int build : 8;
	};
}version_t;

//typedef struct version {
//	unsigned int rsved : 8; // reserved
//	unsigned int major : 8;
//	unsigned int minor : 8;
//	unsigned int build : 8;
//}version_t;

typedef union date {
	unsigned int data[2];
	struct {
		unsigned int year : 16;
		unsigned int month : 8;
		unsigned int day : 8;
		unsigned int hour : 8;
		unsigned int min : 8;
		unsigned int sec : 8;
		unsigned int rsved : 8; // reserved
	};
}date_t;

//typedef struct date {
//	unsigned int year  : 16;
//	unsigned int month :  8;
//	unsigned int day   :  8;	
//	unsigned int hour  :  8;
//	unsigned int min   :  8;
//	unsigned int sec   :  8;
//	unsigned int rsved :  8; // reserved
//}date_t;

typedef union fileheader
{
	unsigned int data[8];
	struct {
		version_t sw_v;  // 4byte
		version_t hw_v;  // 4byte
		date_t    date;  // 8byte
		unsigned int total_size;
		unsigned int head_size;
		unsigned int reg_size;
		unsigned int pat_size;
	};
}fileheader_t;

//typedef union patinfo
//{
//	unsigned char data[48];
//	struct {
//		char name[MAX_NAME_LENGTH];
//		unsigned short start;
//		unsigned short end;
//		unsigned int ntype : 8;
//	};
//}patinfo_t;

typedef struct patinfo
{
	std::string name;
	union {
		unsigned short data;
		struct {
			unsigned short start : 14;
			unsigned short ntype : 2;
			//unsigned int end : 16;
		};
	};
}patinfo_t;

//typedef union labelinfo
//{
//	unsigned char data[48];
//	struct {
//		char name[MAX_NAME_LENGTH];
//		unsigned short pc;
//	};
//}labelinfo_t;

typedef struct labelinfo
{
	std::string name;
	unsigned short pc;
}labelinfo_t;


typedef struct usedinfo
{
	bool bOpcode;
	bool bXB;
	bool bXN;
	bool bXImm;
	bool bYB;
	bool bYN;
	bool bYImm;
	bool bD3;
	bool bD4;
	bool bZ;
	bool bXdest;
	bool bYdest;
	bool bTP1;
	bool bTP1Imm;
	bool bTP2;
	bool bTP2Imm;
	bool bD7;
	bool bD8;
	bool bSA;
	bool bSB;
	bool bSImm;
	bool bS;
	bool bRF;
	bool bFP;
	bool bCYP;
	bool bTS;
}usedinfo_t;
