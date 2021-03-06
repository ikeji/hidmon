#ifndef __AVRSPX_H
#define __AVRSPX_H

#include <windows.h>
//----------------------------------------------------------------------------
#ifdef __BORLANDC__
# pragma warn -8004
# pragma warn -8012
# pragma warn -8057
# pragma warn -8060
# pragma warn -8065
#define strcasecmp(p, q) stricmp(p, q)
#endif

#ifdef _MSC_VER
# pragma warning(disable : 4244)
# pragma warning(disable : 4267)
# pragma warning(disable : 4996)
#endif

#ifndef bool
# define bool	char
# define true	1
# define false	0
#endif

#ifndef _WINDEF_
typedef unsigned char   BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
#endif /* _WINDEF_ */



#define MESS_OUT(str)	fputs(str, stdout)	// add by senshu
#define MESS(str)	fputs(str, stderr)

#define FUSEFILE "fuse.txt"
#if 0
#define INIFILE "avrsp.ini"
#endif


/* Program error codes */

enum {
	RC_FAIL = 1,
	RC_FILE,
	RC_INIT,
	RC_DEV,
	RC_SYNTAX,
	RC_OPT_ERR,
};


/* Buffer size for flash/eeprom/fuse */

#define BASE_FLASH	0			/* Flash base offset in hex file */
#define	MAX_FLASH	(256*1024)	/* Flash buffer size (256K) */
#define	BASE_EEPROM	0x810000	/* EEPROM base offset in hex file */
#define	MAX_EEPROM	(  4*1024)	/* EEPROM buffer size (4K) */
#define BASE_FUSE	0x820000	/* Fuse base offset in hex file */
#define BASE_LOCK	0x830000	/* Lock bits base offset in hex file */
#define	MAX_FUSE	3			/* Fuse buffer size (3) */


/* SPI bridge pipelined operation */

#define	PIPE_WINDOW	256			/* Pipe window for SPI bridge (must be power of 2) */


/* Device property structure */

enum _devid {	/* Device ID */
	N0000, L0000,	/* Unknown, Locked */
	S1200, S2313, S4414, S8515, S2333, S4433, S4434, S8535, S2323, S2343,
	T12, T13, T15, T167, T22, T2313, T24, T25, T26, T261,
	T43U, T44, T45, T461, T48, T84, T85, T861, T87, T88,
	M103, M128, M1280, M1281, M1284P, M16, M161, M162, M163, M164P, M165, M168, M168P,
	M169, M2560, M2561, M32, M323, M324P, M324PA, M325, M3250, M3250P, M325P, M328P,
	M48, M48P, M603, M64, M640, M644, M644P, M645, M6450, M8, M8515, M8535, M88, M88P,
	CAN128, CAN32, CAN64, PWM2, PWM216,
};

#if 1	/* by senshu */
//#define N0000       0x000000
//#define L0000       0x000102
#define AT90S1200   0x1E9001
#define TINY12      0x1E9005
#endif

typedef struct _DEVPROP {
	char	*Name;			/* Device name */
	char	ID;				/* Device ID */
	BYTE	Sign[3];		/* Device signature bytes */
	DWORD	FlashSize;		/* Flash memory size in unit of byte */
	WORD	FlashPage;		/* Flash page size (0 is byte-by-byte) */
	DWORD	EepromSize;		/* EEPROM size in unit of byte */
	WORD	EepromPage;		/* EEPROM page size (0 is byte-by-byte) @@@ */
	WORD	FlashWait;		/* Wait time for flash write */
	WORD	EepromWait;		/* Wait time for EEPROM write */
	BYTE	PollData;		/* Polling data value */
	BYTE	LockData;		/* Default lock byte (program LB1 and LB2) */
	BYTE	LockMask;		/* lock byte mask */
	char	FuseType;		/* Device specific fuse type */
	char	Cals;			/* Number of calibration bytes */
	BYTE	FuseMask[3];	/* Valid fuse bit mask [low, high, ext] */
	BYTE	ISP_DISBL[3];	/* ISP_DISBL:	Valid fuse bit mask [FUSE type, RSTDISBL bit, DWEN bit] */
	WORD	DocNumber;		/* http://www.avrfreaks.net/ @@@ by senshu */
	char	*part_id;		/* Avrdude part's ID @@@ by senshu */
} DEVPROP;

#if 1
extern const DEVPROP DevLst[];	/* Device property list */
#endif

/* Device programming commands */

#define C_EN_PRG1	0xAC
#define C_EN_PRG2	0x53
#define C_ERASE1	0xAC
#define C_ERASE2	0x80
#define C_LD_ADRX	0x4D
#define C_WR_PRGL	0x40
#define C_WR_PRGH	0x48
#define C_WR_PAGE	0x4C
#define C_RD_PRGL	0x20
#define C_RD_PRGH	0x28
#define C_WR_EEP	0xC0
#define C_RD_EEP	0xA0
#define C_WR_FLB	0xAC
#define C_WR_FLBL	0xA0
#define C_WR_FLBH	0xA8
#define C_WR_FLBX	0xA4
#define C_WR_FLBK	0xE0
#define C_RD_FLB1	0x58
#define C_RD_FB1	0x50
#define C_RD_SIG	0x30
#define C_RD_CAL	0x38


/* Byte read/write ID */

#define FLASH		0
#define FLASH_NS	1
#define EEPROM		2
#define SIGNATURE	3


/* Fuse write ID */

#define F_LOW		0
#define	F_HIGH		1
#define	F_EXTEND	2
#define	F_LOCK		3


/* spi_rcvr() argument */

#define	RM_SYNC		0
#define	RM_ASYNC	1


/* Physical port properties */

typedef struct {
	WORD	PortClass;		/* Port class */
	WORD	PortNum;		/* Port number (1..)  */
#if 1
	int		Baud;			/* Baud rate (for SPI bridge) */
	int		Delay;			/* I/O delay */
	char    *SerialNumber;	/*@@@ usbasp serial no. by t.k */
#else
	DWORD	Baud;			/* Baud rate (for SPI bridge) */
	WORD	Delay;			/* I/O delay */
#endif
	char	*Info1, *Info2;	/* Information strings, returned by open_ifport() */
} PORTPROP;

#if 1
#define DEFAULT_DELAY_VALUE	4		//@@@ by t.k (3 => 4 by sensh)
#define DEFAULT_BAUDRATE	115200	//@@@ by t.k
#endif

enum _portclass {	/* Port class */
	TY_LPT, TY_COMM, TY_VCOM, TY_BRIDGE, TY_AVRSP,
	TY_STK200, TY_XILINX, TY_LATTICE, TY_ALTERA
#if 1
	,TY_USBASP, TY_RSCR, TY_HIDASP
#endif
};




/* Prototypes for hardware control functions */

int open_ifport (PORTPROP *);
void close_ifport (void);
void spi_reset (void);
void spi_clk (void);
void spi_xmit (BYTE);
BYTE spi_rcvr (BYTE);
void spi_delayedget (BYTE *, DWORD);
void delay_ms (WORD);
int spi_flush (void);
FILE *open_cfgfile(char *);
void read_multi (BYTE, DWORD, DWORD, BYTE*);
void write_page (DWORD, const BYTE*);

extern const DEVPROP *Device;

//----------------------------------------------------------------------------
// by t.k
// avrspx.c
void report_setup(char *msg, long size);
void report_update(int bytes);
void report_finish(int err_count);

// hwctrl.c
void spi_transmit(BYTE cmd1, BYTE cmd2,BYTE cmd3, BYTE cmd4);
int spi_transmit_R(BYTE cmd1, BYTE cmd2,BYTE cmd3, BYTE mode);

// utils.c
extern char progname[];
extern char inifilename[];
#undef INIFILE
#define INIFILE inifilename

unsigned long strtoul_ex(const char *s, char **endptr, int base);
int setup_commands_ex(int argc, char **argv);
DEVPROP *search_device(char *Name);
DEVPROP *get_device(BYTE *signature);
int get_outque_count(HANDLE hComm);
void dump_port_list(void);

//----------------------------------------------------------------------------
// by t.k
// wronly.c
extern bool  f_write_only_programmer;
extern int   cmdbuf_count;
extern BYTE *cmdbuf;
int wronly_universal_command(BYTE cmd1, BYTE cmd2,BYTE cmd3, BYTE cmd4);
void wronly_programmer_init(void);
void wronly_programmer_close(void);

// rscr.c
int rscr_setup(HANDLE h, int baud);
void rscr_close(void);
void rscr_delay_ms(WORD dly);

//----------------------------------------------------------------------------
#endif	// end of __AVRSPX_H
