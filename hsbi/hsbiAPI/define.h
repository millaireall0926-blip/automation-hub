#ifndef __DEFINE__
#define __DEFINE__

typedef unsigned int uint;
typedef unsigned long long int uint64;
typedef unsigned short int	word;
typedef unsigned char byte;

#define MAX_PCI 0x2
#define MAX_PG  	  4
#define MAX_PG_NUM	  2

#define MAX_TDR_SCAN 2 //  MAX 값은 8 현재는 2로 적용
#define MAX_TDR_IO 128   //  

#define NO_SLEEP        0
#define SLEEP_1US       1
#define SLEEP_10US      10
#define SLEEP_100US     100

#define NO_RUN      false

#define SYNC_USER_LAST    0xEF

#define SYNC_CYCLE_PROG_START   0xF0
#define SYNC_CYCLE_PROG_GATHER   0xF1
#define SYNC_CYCLE_ERASE_START   0xF2
#define SYNC_CYCLE_ERASE_GATHER   0xF3

#define SYNC_DATA   0xFE
#define SYNC_SCAN   0xFF
#define SYNC_NONE   0x100
#define SYNC_END    0x110

#define PG_INSTRUCTION		(0)
#define PG_SYSTEM			(1<<9)
#define PG_PAT				(1<<18)
#define PG_PDS				(12<<16)
#define PG_FORMAT			(13<<16)
#define PG_SCR				(14<<16)
#define PG_SCAN				(59<<14)
#define PG_FMH				(4<<17)
#define PG_FML				(5<<17)
#define TG_REG			    (1<<16)
#define TG_MEM			    (0<<16)
#define PG_VMH				(1<<20)
#define PG_VML				((1<<20)|(1<<17))


typedef struct
{
    float gain;
    float offset;
    float offset2;
} stCalFactor;

typedef struct
{
    unsigned long long chBit;
    double delay;
}stPowerSeq;

#endif
