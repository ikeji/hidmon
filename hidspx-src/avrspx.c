/*---------------------------------------------------------------------------*/
/* AVRSP - AVR Serial Programming Controller                                 */
/*                                                                           */
/* R0.43b (C)ChaN, 2007                                                       */
/*---------------------------------------------------------------------------*/
/* R0.28  Apr 30, '04  Migration from MS-DOS based AVRSS/AVRXS R0.27         */
/* R0.28b May  1, '04  Fixed some value in the device property table         */
/* R0.28c May  3, '04  Fixed unable to communicate via ByteBlasterMV         */
/* R0.28d May 22, '04  WinNT/2k/XP are supported (GIVEIO is required)        */
/* R0.29  Jun 25, '04  AT90CAN128 is supported                               */
/* R0.30  Sep  1, '04  ATmega165 is supported and some updates...            */
/* R0.31  Nov 11, '04  ATmega325/3250/645/6450 are supported                 */
/* R0.32  Feb 11, '05  90PWM2/3                                              */
/* R0.33  Feb 15, '05  tiny25/45/85                                          */
/* R0.34  Mar 12, '05  mega640/1280/2560/641/1281                            */
/* R0.35  Apr 26, '05  Supported serial to SPI bridge                        */
/* R0.36  May 20, '05  Improved read performance on SPI bridge               */
/* R0.37  Aug 10, '05  tiny24/44/84, Fixed some parms for tiny25/45/85       */
/* R0.38  Jan 30, '06  AT90CAN32/64, Extended -w switch                      */
/* R0.39  Mar 15, '06  ATmega644, Fixed number of cals for tiny2313          */
/* R0.40  Mar 18, '07  ATmega164P/324P/644P, ATtiny261/461/861               */
/* R0.41a Jun 21, '07  Fixed some value in the device property table         */
/* R0.42  Aug 08, '07  ATmega48P/88P/168P/328P                               */
/* R0.43  Dec 16, '07  Improved programming time on SPI bridge (ser2spi_r4)  */
/* R0.43b Dec 18, '07  Supported old SPI bridge for backward compatibility   */
/*---------------------------------------------------------------------------*/
/* R0.43b Base                                                                */
/*   b2   2006-05-06  USBasp support, by mutech,t.kuroki                     */
/*   b3   2006-05-06  USBaspで旧タイプのMCUでの読み書き改善                  */
/*   b4   2006-05-08  USBasp -d オプション対応                               */
/*   b5   2006-05-08  USBasp -d- オプション対応                              */
/*   b6   2006-05-09  iniファイルの空白行の扱いをオリジナルと同じ動作に      */
/* R0.39 Base                                                                */
/*   b7   2006-05-15  Original R0.39に合わせて変更                           */
/*                    USBaspの複数接続に対応(Serial Number追加)              */
/*   b8   2006-05-18  -q<device>オプション追加                               */
/*             USBaspxでのUSBASP_FUNC_CONNECTでリターン値を返す変更に対応    */
/*   b9   2006-05-20  USBaspx mega128x/mega256x対応 (未チェック)             */
/*                    Fuse Write 後に Verify を追加                          */
/*  b9.2  2006-05-20 Verifyを必要な領域だけにして高速化                      */
/*  b9.3  2006-05-24 Verify正常終了時のMESSAGEが抜けていたのを修正           */
/*  b9.4  2006-05-28 BCCでコンパイル可能なようにソース修正                   */
/*  b9.5  2006-05-28 Visual C++ 2005 Express Edition対応                     */
/*  b10   2006-06-04 added TY_RSCR(denshiken).                               */
/*                   Solving the chicken and egg problem                     */
/* b10.1  2006-06-05 -p? port list.                                          */
/* b10.2  2006-06-06 TY_RSCR関連を若干修正                                   */
/* b10.3  2006-06-07 TY_RSCR関連を若干修正 (終了にdelayを追加)               */
/* b10.4  2006-06-08 TY_RSCR (終了時に出力バッファが０になるまで待機)        */
/* b10.5  2006-06-10 --pause-on-start,--pause-on-exit                        */
/* b10.6  2006-08-26 ページ書込みでないAVRのbug fix.                         */
/* b10.7  2006-08-27 ソースの整理                                            */
/* b10.8  2006-08-30 libusb-win32-0.1.10.1 の不具合に対処                    */
/*                   ライブラリをlibusb-win32-0.1.12.0 に変更                */
/* b10.9  2006-08-30 USBバスからのusbaspの検索の高速化                       */
/* b10.10 2006-08-31 libusbのバージョン表示を追加                            */
/*---------------------------------------------------------------------------*/
/* b10.11 2008-01-01 senshuがavrspとavrspx(b10.10)のソースを参考に作成       */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "avrspx.h"
#if AVRSPX
#include <conio.h>
#include <windows.h>
#include "usbasp.h"
#include "hidasp.h"
#endif


/*-----------------------------------------------------------------------
  Device properties
-----------------------------------------------------------------------*/

const DEVPROP DevLst[] =	/* Device property list */
{
	/* Name,         ID,    Signature,              FS,  PS,   ES, EP, FW, EW,   PV,   LB, FT, Cals, FuseMasks */
//------- AT90S
	{ "90S1200",     S1200, {0x1E, 0x90, 0x01},   1024,   0,   64,  0, 11, 11, 0xFF, 0xF9, 0 },
	{ "90S2313",     S2313, {0x1E, 0x91, 0x01},   2048,   0,  128,  0, 11, 11, 0x7F, 0xF9, 0 },
	{ "90S2323",     S2323, {0x1E, 0x91, 0x02},   2048,   0,  128,  0, 11, 11, 0xFF, 0xF9, 1, 0, {0x01} },
	{ "90S2333",     S2333, {0x1E, 0x91, 0x05},   2048,   0,  128,  0, 11, 11, 0xFF, 0xF9, 2, 0, {0x1F} },
	{ "90S2343",     S2343, {0x1E, 0x91, 0x03},   2048,   0,  128,  0, 11, 11, 0xFF, 0xF9, 1, 0, {0x01} },
	{ "90S4414",     S4414, {0x1E, 0x92, 0x01},   4096,   0,  256,  0, 11, 11, 0x7F, 0xF9, 0 },
	{ "90S4433",     S4433, {0x1E, 0x92, 0x03},   4096,   0,  256,  0, 11, 11, 0xFF, 0xF9, 2, 0, {0x1F} },
	{ "90S4434",     S4434, {0x1E, 0x92, 0x02},   4096,   0,  256,  0, 11, 11, 0xFF, 0xF9, 1, 0, {0x01} },
	{ "90S8515",     S8515, {0x1E, 0x93, 0x01},   8192,   0,  512,  0, 11, 11, 0x7F, 0xF9, 0 },
	{ "90S8535",     S8535, {0x1E, 0x93, 0x03},   8192,   0,  512,  0, 11, 11, 0xFF, 0xF9, 1, 0, {0x01} },
//------- ATtiny
	{ "tiny12",      T12,   {0x1E, 0x90, 0x05},   1024,   0,   64,  2,  5,  8, 0xFF, 0xF9, 3, 1, {0xFF} },
	{ "tiny13",      T13,   {0x1E, 0x90, 0x07},   1024,  32,   64,  4,  6,  5, 0xFF, 0xFC, 5, 1, {0x7F, 0x1F} },
	{ "tiny15",      T15,   {0x1E, 0x90, 0x06},   1024,   0,   64,  2,  6, 11, 0xFF, 0xF9, 3, 1, {0xD3} },
	{ "tiny22",      T22,   {0x1E, 0x91, 0x06},   2048,   0,  128,  0, 11, 11, 0xFF, 0xF9, 1, 0, {0x01} },
	{ "tiny2313",    T2313, {0x1E, 0x91, 0x0A},   2048,  32,  128,  4,  6,  5, 0xFF, 0xFC, 6, 2, {0xFF, 0xDF, 0x01} },
	{ "tiny24",      T24,   {0x1E, 0x91, 0x0B},   2048,  32,  128,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
	{ "tiny25",      T25,   {0x1E, 0x91, 0x08},   2048,  32,  128,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
	{ "tiny26",      T26,   {0x1E, 0x91, 0x09},   2048,  32,  128,  4,  6, 10, 0xFF, 0xFC, 5, 4, {0xFF, 0x17} },
	{ "tiny261",     T261,  {0x1E, 0x91, 0x0C},   2048,  32,  128,  4,  6, 10, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
	{ "tiny44",      T44,   {0x1E, 0x92, 0x07},   4096,  64,  256,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
	{ "tiny45",      T45,   {0x1E, 0x92, 0x06},   4096,  64,  256,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
	{ "tiny461",     T461,  {0x1E, 0x92, 0x08},   4096,  64,  256,  4,  6, 10, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
	{ "tiny84",      T84,   {0x1E, 0x93, 0x0C},   8192,  64,  512,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
	{ "tiny85",      T85,   {0x1E, 0x93, 0x0B},   8192,  64,  512,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
	{ "tiny861",     T861,  {0x1E, 0x93, 0x0D},   8192,  64,  512,  4,  6, 10, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
//------- ATmega
	{ "mega48",      M48,   {0x1E, 0x92, 0x05},   4096,  64,  256,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
	{ "mega48P",     M48P,  {0x1E, 0x92, 0x0A},   4096,  64,  256,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x01} },
	{ "mega8",       M8,    {0x1E, 0x93, 0x07},   8192,  64,  512,  4,  6, 11, 0xFF, 0xFC, 5, 4, {0xFF, 0xDF} },
	{ "mega8515",    M8515, {0x1E, 0x93, 0x06},   8192,  64,  512,  4,  6, 11, 0xFF, 0xFC, 5, 4, {0xFF, 0xDF} },
	{ "mega8535",    M8535, {0x1E, 0x93, 0x08},   8192,  64,  512,  4,  6, 11, 0xFF, 0xFC, 5, 4, {0xFF, 0xDF} },
	{ "mega88",      M88,   {0x1E, 0x93, 0x0A},   8192,  64,  512,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega88P",     M88P,  {0x1E, 0x93, 0x0F},   8192,  64,  512,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega16",      M16,   {0x1E, 0x94, 0x03},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 5, 4, {0xFF, 0xDF} },
	{ "mega161",     M161,  {0x1E, 0x94, 0x01},  16384, 128,  512,  0, 20,  5, 0xFF, 0xFC, 4, 0, {0x7F} },
	{ "mega162",     M162,  {0x1E, 0x94, 0x04},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x1E} },
	{ "mega163",     M163,  {0x1E, 0x94, 0x02},  16384, 128,  512,  0, 18,  5, 0xFF, 0xFC, 5, 1, {0xEF, 0x07} },
	{ "mega164P",    M164P, {0x1E, 0x94, 0x0A},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega165",     M165,  {0x1E, 0x94, 0x07},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x0E} },
	{ "mega168",     M168,  {0x1E, 0x94, 0x06},  16384, 128,  512,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega168P",    M168P, {0x1E, 0x94, 0x0B},  16384, 128,  512,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega169",     M169,  {0x1E, 0x94, 0x05},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x0F} },
	{ "mega323",     M323,  {0x1E, 0x95, 0x01},  32768, 128, 1024,  4, 18,  5, 0xFF, 0xFC, 5, 1, {0xCF, 0xEF} },
	{ "mega325/9",   M325,  {0x1E, 0x95, 0x03},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega3250/90", M3250, {0x1E, 0x95, 0x04},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega328P",    M328P, {0x1E, 0x95, 0x0F},  32768, 128, 1024,  4,  6,  5, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega32",      M32,   {0x1E, 0x95, 0x02},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 5, 4, {0xFF, 0xDF} },
	{ "mega324P",    M324P, {0x1E, 0x95, 0x08},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega603",     M603,  {0x1E, 0x96, 0x01},  65536, 256, 2048,  0, 60, 11, 0xFF, 0xF9, 2, 0, {0x0B} },
	{ "mega644",     M644,  {0x1E, 0x96, 0x09},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega644P",    M644P, {0x1E, 0x96, 0x0A},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega645/9",   M645,  {0x1E, 0x96, 0x03},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega6450/90", M6450, {0x1E, 0x96, 0x04},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega64",      M64,   {0x1E, 0x96, 0x02},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 6, 4, {0xFF, 0xDF, 0x03} },
	{ "mega640",     M640,  {0x1E, 0x96, 0x08},  65536, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega103",     M103,  {0x1E, 0x97, 0x01}, 131072, 256, 4096,  0, 60, 11, 0xFF, 0xF9, 2, 0, {0x0B} },
	{ "mega128",     M128,  {0x1E, 0x97, 0x02}, 131072, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 6, 4, {0xFF, 0xDF, 0x03} },
	{ "mega1280",    M1280, {0x1E, 0x97, 0x03}, 131072, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega1281",    M1281, {0x1E, 0x97, 0x04}, 131072, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega2560",    M2560, {0x1E, 0x98, 0x01}, 262144, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
	{ "mega2561",    M2561, {0x1E, 0x98, 0x02}, 262144, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x07} },
//------- PWM, CAN
	{ "90PWM2/3",    PWM2,  {0x1E, 0x93, 0x81},   8192,  64,  512,  4,  6, 11, 0xFF, 0xFC, 6, 4, {0xFF, 0xDF, 0xF7} },
	{ "90CAN32",     CAN32, {0x1E, 0x95, 0x81},  32768, 256, 1024,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x0F} },
	{ "90CAN64",     CAN64, {0x1E, 0x96, 0x81},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x0F} },
	{ "90CAN128",    CAN128,{0x1E, 0x97, 0x81}, 131072, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 6, 1, {0xFF, 0xDF, 0x0F} },
//------- Misc
	{ "Locked",      L0000, {0x00, 0x01, 0x02} },	/* Locked Device */
	{ NULL,          N0000 }						/* Unknown */
};

const DEVPROP *Device = NULL;		/* Pointer to the current device property */



/*-----------------------------------------------------------------------
  Global variables (initialized by load_commands())
-----------------------------------------------------------------------*/

BYTE CodeBuff[MAX_FLASH];		/* Program code R/W buffer */
BYTE DataBuff[MAX_EEPROM];		/* EEPROM data R/W buffer */

BYTE CalBuff[4];				/* Calibration bytes read buffer */
BYTE FuseBuff[3];				/* Fuse bytes read buffer */
BYTE SignBuff[4];				/* Device signature read buffer */

/*---------- Command Parameters ------------*/

char Command[2];				/* -r -e -z Read/Erase/Test command (1st,2nd char) */

struct {						/* Code/Data write command (hex file) */
	DWORD CodeSize;				/* Loaded program code size (.hex) */
	DWORD DataSize;				/* Loaded EEPROM data size (.eep) */
	char Verify;				/* -v 1:Verify only (skip programming), 2:Skip verify (programmig only) */
	char CopyCal;				/* -c Copy calibration bytes into end of flash */
} CmdWrite;

struct {						/* Fuse write command */
	union {						/* which fuse? */
		char Flags;
		struct {
			int Low		: 1;	/* -fl */
			int High	: 1;	/* -fh */
			int Extend	: 1;	/* -fx */
			int Lock	: 1;	/* -l */
		} Flag;
	} Cmd;
	BYTE Data[4];				/* fuse bytes to be written {Low,High,Extend,Lock} */
} CmdFuse;

int Pause;						/* -w Pause before exiting program */

char ForcedName[20];			/* -t Forced device type (compared to Device->Name)*/


#if AVRSPX
/* for USBasp */
bool f_usblist;
bool f_detail_help;
char usb_serial[8];
char new_serial[8];
char DeviceName[20];			// -d check device type (compared to Device->Name)
bool f_portlist;
char *msg_pause_on_start = "";	/* --pause-on-start=msg */
char *msg_pause_on_exit = "";	/* --pause-on-exit=msg */

/* porgress report by senshu */
static long total_bytes = 0L;
static long total_size = 0L;
static long total_size_kb = 0L;

char report_msg[128];

#define MAX_BYTES	8192

void report_setup(char *msg, long size)
{
	total_bytes = 0L;
	total_size = size;
	total_size_kb = (size+1023)/1024;

	strcpy(report_msg, msg);
	if (total_size <= MAX_BYTES) {
		strcat(report_msg, " %4d/%4d B\r");
	} else {
		strcat(report_msg, " %4d/%4d KB\r");
	}
}

void report_update(int bytes)
{
	if (report_msg) {
		total_bytes += bytes;
		if (total_size <= MAX_BYTES) {
			fprintf(stderr, report_msg, total_bytes, total_size);
		} else {
			fprintf(stderr, report_msg, total_bytes/1024, total_size_kb);
		}
	}
}

void report_finish()
{
	int last;
	if (report_msg[0]) {
		if (total_size <= MAX_BYTES) {
			fprintf(stderr, report_msg, total_size, total_size);
			fprintf(stderr, "\n");
		} else {
			last = strlen(report_msg);
			report_msg[last-1] = '\0';
			strcat(report_msg, "  (%dbytes)\n");
			fprintf(stderr, report_msg, total_size_kb, total_size_kb, total_bytes);
		}
	}
	report_msg[0] ='\0';
	total_bytes = 0L;
}

#endif

/*---------- Hardware Control ------------*/

#if AVRSPX
// by t.k
PORTPROP CtrlPort = {
	TY_BRIDGE,	/* Port class */
	 1,			/* Port number (1..)  */
	-1, 		/* Baud rate (for SPI bridge) */
	-1,			/* -d .Delay (SPI control delay) */
	NULL,		/* usbasp serial no. by t.k     */
	NULL,		/* Information strings1 */
	NULL		/* Information strings1 */
};
#else
PORTPROP CtrlPort = {	TY_COMM, 1,	115200, /* -p .PortClass .PortNum .Baud */
						3, 					/* -d .Delay (SPI control delay) */
						NULL, NULL };
#endif




/*-----------------------------------------------------------------------
  Messages
-----------------------------------------------------------------------*/

int detail_help = 0;

void output_usage (bool detail)
{
	int n;
#if AVRSPX
	static const char *const MesUsage[] = {
		"AVRSP - AVR Serial Programming tool R0.43b (C)ChaN,2007  http://elm-chan.org/\n",
		"Write code and/or data  : <hex file> [<hex file>] ...\n",
		"Verify code and/or data : -v <hex file> [<hex file>] ...\n",
		"Read code, data or fuse : -r{p|e|f}\n",
		"Write fuse byte         : -f{l|h|x}<bin>\n",
		"Lock device             : -l[<bin>]\n",
		"Copy calibration bytes  : -c\n",
		"Erase device            : -e\n",
		"Control port [-pb1]     : -p{c|l|v|b|u|f}<n>[:<baud>]\n",
        "@  -pc<n>                   Direct COM Port Access (giveio.sys)\n",
        "@  -pl<n>                   Direct LPT Port Access (giveio.sys)\n",
        "@  -pv<n>                   COM Port access (Win32API)\n",
        "@  -pb<n>                   SPI-Bridge (COM Port)\n",
        "@  -pu[:XXXX]               USBasp/USBaspx\n",
        "@  -ph                      HIDasp\n",
        "@  -pf<n>                   COM Port Access (chicken & egg)\n",
        "@  -p?                      Dump COM Port List.\n",
		"SPI control delay [-d3] : -d<n>\n",
		"Help (Detail)           : -? or -h or --help\n",

		"@\n",
		"@Miscellaneous Options:\n",
		"@  -q<device>               Check device type\n",
		"@  -t<device>               Force device type\n",
		"@  -w<num>                  Pause before exit\n",
		"@  -z                       1KHz pulse on SCK\n",
		"@  --pause-on-start=msg       Pause on Port Open\n",
		"@  --pause-on-exit=msg        Pause on exit\n",
		"@  --list-port or -p?       List COM Port\n",
		"@  --list-usbasp or -pu?    List USBasp devices\n",
		"@  --set-serial=XXXXXX      Set USBasp serial number\n",

		"@\n",
		"@Supported Device:\n",
		"@AT90S 1200,2313,2323,2333,2343,4414,4433,4434,8515,8535\n",
		"@ATtiny 12,13,15,22,24,25,26,44,45,84,85,261,461,861,2313\n",
		"@ATmega 8,16,32,48,48P,64,88,88P,103,128,161,162,163,164P,165,168,168P,\n",
		"@       169,323,324P,325/9,328P,3250/90,603,640,644,644P,645/9,1280,1281,\n",
		"@       2560,2561,6450/90,8515,8535\n",
		"@AT90CAN 32,64,128, AT90PWM 2,3\n",

		"\n",
		"Supported Adapter:\n",
		"AVRSP adapter (COM -pc<n>|-pv<n> / LPT -pl<n>), SPI Bridge (COM -pb<n>[:BAUD]),\n"
		"STK200 ISP dongle, Xilinx JTAG, Lattice isp, Altera ByteBlasterMV (LPT -pl<n>)\n",
		"USBasp(x) (USB -pu<n>[:XXXX]), RSCR (COM -pf<n>),  (<n> == PORT Number)\n",
		"HIDaspx   (USB -ph<n>)\n",
		NULL
	};


#if defined(__GNUC__) && defined(__WIN32__)
		printf("%s (b10.11) by t.k & senshu, GCC-MinGW %s, %s\n",  progname, __VERSION__,  __DATE__);
		printf("----\n");
#elif defined(__WIN32__)
		printf("%s (b10.11) by t.k & senshu, Borland C++ 5.5.1, %s\n",  progname, __DATE__ );
		printf("----\n");
#elif defined(__WIN32__)
		printf("%s (b10.11) by t.k & senshu for Windows, %s\n",  progname, __DATE__ );
		printf("----\n");
#endif
	for(n = 0; MesUsage[n] != NULL; n++) {
		const char *s = MesUsage[n];

		if (detail) {
	        if (*s == '@') {
	            if (detail) MESS_OUT(s+1);
			} else {
				MESS_OUT(s);
			}
		} else {
	        if (*s != '@') {
				MESS_OUT(s);
			}
		}
	}

#else
	static const char *const MesUsage[] = {
		"AVRSP - AVR Serial Programming tool R0.43b (C)ChaN,2007  http://elm-chan.org/\n\n",
		"Write code and/or data  : <hex file> [<hex file>] ...\n",
		"Verify code and/or data : -V <hex file> [<hex file>] ...\n",
		"Read code, data or fuse : -R{P|E|F}\n",
		"Write fuse byte         : -F{L|H|X}<bin>\n",
		"Lock device             : -L[<bin>]\n",
		"Copy calibration bytes  : -C\n",
		"Erase device            : -E\n",
		"Control port [-pc1]     : -P{C|L|V|B}<n>[:<baud>]\n",
		"SPI control delay [-d3] : -D<n>\n\n",
		"Supported Device:\n",
		"AT90S 1200,2313,2323,2333,2343,4414,4433,4434,8515,8535\n",
		"ATtiny 12,13,15,22,24,25,26,44,45,84,85,261,461,861,2313\n",
		"ATmega 8,16,32,48,48P,64,88,88P,103,128,161,162,163,164P,165,168,168P,169,323,324P,325/9,328P,3250/90,603,640,644,644P,645/9,1280,1281,2560,2561,6450/90,8515,8535\n",
		"AT90CAN 32,64,128, AT90PWM 2,3\n\n",
		"Supported Adapter:\n",
		"AVRSP adapter (COM/LPT), SPI Bridge (COM), STK200 ISP dongle (LPT)\n",
		"Xilinx JTAG (LPT), Lattice isp (LPT), Altera ByteBlasterMV (LPT)\n",
		NULL
	};


	for(n = 0; MesUsage[n] != NULL; n++)
		MESS(MesUsage[n]);
#endif
}


/* Output the device information */

void output_deviceinfo (void)
{
	printf("Device Signature  = %02X-%02x-%02X\n",
			Device->Sign[0], Device->Sign[1], Device->Sign[2]);
	printf("Flash Memory Size = %d bytes\n", (int)Device->FlashSize);
	if(Device->FlashPage)
		printf("Flash Memory Page = %d bytes x %d pages\n",
				(int)Device->FlashPage, (int)(Device->FlashSize / Device->FlashPage));
	printf("EEPROM Size       = %d bytes\n", (int)Device->EepromSize);
}




/* Output a fuse byte and its description if present */

void put_fuseval (BYTE val, BYTE mask, const char *head, FILE *fp)
{
	int	n;
	char Line[100];


	fputs(head, stdout);
	for(n = 1; n <= 8; n++) {
		putchar((mask & 0x80) ? ((val & 0x80) ? '1' : '0') : '-');
		val <<= 1; mask <<= 1;
	}
	putchar('\n');

	if(fp == NULL) return;
	while (1) {	/* seek to the fuse header */
		if(fgets(Line, sizeof(Line), fp) == NULL) return;
		if(strstr(Line, head) == Line) break;
	}
	do {		/* output fuse bit descriptions */
		if(fgets(Line, sizeof(Line), fp) == NULL) return;
		fputs(Line, stdout);
	} while (Line[0] != '\n');

}




/* Output fuse bytes and calibration byte */

void output_fuse ()
{
	int n;
	FILE *fp;
	char Line[100], *cp;


	if(Device->FuseType == 0) {
		MESS("Fuse bits are not accessible.\n");
		return;
	}

	/* Open FUSE.TXT and seek to the device */
	fp = open_cfgfile(FUSEFILE);
	if(fp == NULL) {
		MESS("WARNING: Missing fuse description file.\n");
	} else {
		while (1) {
			if(fgets(Line, sizeof(Line), fp) == NULL) break;
			if((Line[0] != 'D') || ((cp = strstr(Line, Device->Name)) == NULL)) continue;
			if(strlen(cp) == strlen(Device->Name) + 1) break;
		}
	}

	MESS("\n");
	put_fuseval(FuseBuff[0], Device->FuseMask[0], "Low: ", fp);

	if(Device->FuseType >= 5)
		put_fuseval(FuseBuff[1], Device->FuseMask[1], "High:", fp);

	if(Device->FuseType >= 6)
		put_fuseval(FuseBuff[2], Device->FuseMask[2], "Ext: ", fp);

	/* Output calibration values */
	if(Device->Cals) {
		fputs("Cal:", stdout);
		for(n = 0; n < Device->Cals; n++)
			printf(" %d", CalBuff[n]);
		putchar('\n');
	}

	if(fp != NULL) fclose(fp);	/* Close FUSE.TXT */
}




/*-----------------------------------------------------------------------
  Hex format manupilations
-----------------------------------------------------------------------*/


/* Pick a hexdecimal value from hex record */

DWORD get_valh (
	char **lp,	/* pointer to line read pointer */
	int count, 	/* number of digits to get (2,4,6,8) */
	BYTE *sum	/* byte check sum */
) {
	DWORD val = 0;
	BYTE n;


	do {
		n = *(*lp)++;
		if((n -= '0') >= 10) {
			if((n -= 7) < 10) return 0xFFFFFFFF;
			if(n > 0xF) return 0xFFFFFFFF;
		}
		val = (val << 4) + n;
		if(count & 1) *sum += (BYTE)val;
	} while(--count);
	return val;
}




/* Load Intel/Motorola hex file into data buffer */

long input_hexfile (
	FILE *fp,			/* input stream */
	BYTE *buffer,		/* data input buffer */
	DWORD buffsize,		/* size of data buffer */
	DWORD *datasize		/* effective data size in the input buffer */
) {
	char line[600];			/* line input buffer */
	char *lp;				/* line read pointer */
	long lnum = 0;			/* input line number */
	WORD seg = 0, hadr = 0;	/* address expantion values for intel hex */
	DWORD addr, count, n;
	BYTE sum;


	while(fgets(line, sizeof(line), fp) != NULL) {
		lnum++;
		lp = &line[1]; sum = 0;

		if(line[0] == ':') {	/* Intel Hex format */
			if((count = get_valh(&lp, 2, &sum)) > 0xFF) return lnum;	/* byte count */
			if((addr = get_valh(&lp, 4, &sum)) > 0xFFFF) return lnum;	/* offset */

			switch (get_valh(&lp, 2, &sum)) {	/* block type? */
				case 0x00 :	/* data */
					addr += (seg << 4) + (hadr << 16);
					while(count--) {
						n = get_valh(&lp, 2, &sum);		/* pick a byte */
						if(n > 0xFF) return lnum;
						if(addr >= buffsize) continue;	/* clip by buffer size */
						buffer[addr++] = (BYTE)n;		/* store the data */
						if(addr > *datasize)			/* update data size information */
							*datasize = addr;
					}
					break;

				case 0x01 :	/* end */
					if(count != 0) return lnum;
					break;

				case 0x02 :	/* segment base [19:4] */
					if(count != 2) return lnum;
					seg = (WORD)get_valh(&lp, 4, &sum);
					if(seg == 0xFFFF) return lnum;
					break;

				case 0x03 :	/* program start address (segment:offset) */
					if(count != 4) return lnum;
					get_valh(&lp, 8, &sum);
					break;

				case 0x04 :	/* high address base [31:16] */
					if(count != 2) return lnum;
					hadr = (WORD)get_valh(&lp, 4, &sum);
					if(hadr == 0xFFFF) return lnum;
					break;

				case 0x05 :	/* program start address (linear) */
					if(count != 4) return lnum;
					get_valh(&lp, 8, &sum);
					break;

				default:	/* invalid block */
					return lnum;
			} /* switch */
			if(get_valh(&lp, 2, &sum) > 0xFF) return lnum;	/* get check sum */
			if(sum) return lnum;							/* test check sum */
			continue;
		} /* if */

		if(line[0] == 'S') {	/* Motorola S format */
			if((*lp >= '1')&&(*lp <= '3')) {

				switch (*lp++) {	/* record type? (S1/S2/S3) */
					case '1' :
						count = get_valh(&lp, 2, &sum) - 3;
						if(count > 0xFF) return lnum;
						addr = get_valh(&lp, 4, &sum);
						if(addr > 0xFFFF) return lnum;
						break;
					case '2' :
						count = get_valh(&lp, 2, &sum) - 4;
						if(count > 0xFF) return lnum;
						addr = get_valh(&lp, 6, &sum);
						if(addr > 0xFFFFFF) return lnum;
						break;
					default :
						count = get_valh(&lp, 2, &sum) - 5;
						if(count > 0xFF) return lnum;
						addr = get_valh(&lp, 8, &sum);
						if(addr == 0xFFFFFFFF) return lnum;
				}
				while(count--) {
					n = get_valh(&lp, 2, &sum);
					if(n > 0xFF) return lnum;
					if(addr >= buffsize) continue;	/* clip by buffer size */
					buffer[addr++] = (BYTE)n;		/* store the data */
					if(addr > *datasize)			/* update data size information */
						*datasize = addr;
				}
				if(get_valh(&lp, 2, &sum) > 0xFF) return lnum;	/* get check sum */
				if(sum != 0xFF) return lnum;					/* test check sum */
			} /* switch */
			continue;
		} /* if */

		if(line[0] >= ' ') return lnum;
	} /* while */

	return feof(fp) ? 0 : -1;
}




/* Put an Intel Hex data block */

void put_hexline (
	FILE *fp,			/* output stream */
	const BYTE *buffer,	/* pointer to data buffer */
	WORD ofs,			/* block offset address */
	BYTE count,			/* data byte count */
	BYTE type			/* block type */
) {
	BYTE sum;

	/* Byte count, Offset address and Record type */
	fprintf(fp, ":%02X%04X%02X", count, ofs, type);
	sum = count + (ofs >> 8) + ofs + type;

	/* Data bytes */
	while(count--) {
		fprintf(fp, "%02X", *buffer);
		sum += *buffer++;
	}

	/* Check sum */
	fprintf(fp, "%02X\n", (BYTE)-sum);
}




/* Output data in Intel Hex format */

void output_hexfile (
	FILE *fp,			/* output stream */
	const BYTE *buffer,	/* pointer to data buffer */
	DWORD datasize,		/* number of bytes to be output */
	BYTE blocksize		/* HEX block size (1,2,4,..,128) */
) {
	WORD seg = 0, ofs = 0;
	BYTE segbuff[2], d, n;
	DWORD bc = datasize;


	while(bc) {
		if((ofs == 0) && (datasize > 0x10000)) {
			segbuff[0] = (BYTE)(seg >> 8); segbuff[1] = (BYTE)seg;
			put_hexline(fp, segbuff, 0, 2, 2);
			seg += 0x1000;
		}
		if(bc >= blocksize) {	/* full data block */
			for(d = 0xFF, n = 0; n < blocksize; n++) d &= *(buffer+n);
			if(d != 0xFF) put_hexline(fp, buffer, ofs, blocksize, 0);
			buffer += blocksize;
			bc -= blocksize;
			ofs += blocksize;
		} else {				/* fractional data block */
			for(d = 0xFF, n = 0; n < bc; n++) d &= *(buffer+n);
			if(d != 0xFF) put_hexline(fp, buffer, ofs, (BYTE)bc, 0);
			bc = 0;
		}
	}

	put_hexline(fp, NULL, 0, 0, 1);	/* End block */
}



/*-----------------------------------------------------------------------
  Command line analysis
-----------------------------------------------------------------------*/


int load_commands (int argc, char **argv)
{
	char *cp, c, *cmdlst[20], cmdbuff[256];
	int cmd;
	FILE *fp;
	DWORD ln;
#if AVRSPX
	char *s;

	setup_commands_ex(argc, argv);		//@@@ by t.k
#endif

	/* Clear data buffers */
	memset(CodeBuff, 0xFF, sizeof(CodeBuff));
	memset(DataBuff, 0xFF, sizeof(DataBuff));

	cmd = 0; cp = cmdbuff;

	/* Import ini file as command line parameters */
	fp = open_cfgfile(INIFILE);
	if(fp != NULL) {
		while(fgets(cp, cmdbuff + sizeof(cmdbuff) - cp, fp) != NULL) {
			if(cmd >= (sizeof(cmdlst) / sizeof(cmdlst[0]) - 1)) break;
			if(*cp <= ' ') break;
#if AVRSPX
			if(*cp == ';' || *cp == '#') continue; //@@@ by t.k
			cmdlst[cmd++] = cp; cp += strlen(cp) + 1;
            if (*(cp-2) == '\n') *(cp-2) = '\0';    //@@@ by t.k
#else
			cmdlst[cmd++] = cp; cp += strlen(cp) + 1;
#endif
		}
		fclose(fp);
	}

	/* Get command line parameters */
	while(--argc && (cmd < (sizeof(cmdlst) / sizeof(cmdlst[0]) - 1)))
		cmdlst[cmd++] = *++argv;
	cmdlst[cmd] = NULL;

	/* Analyze command line parameters... */
	for(cmd = 0; cmdlst[cmd] != NULL; cmd++) {
		cp = cmdlst[cmd];

		if(*cp == '-') {	/* Command switches... */
			cp++;
			switch (tolower(*cp++)) {
				case 'v' :	/* -v, -v- */
					if(*cp == '-') {
						CmdWrite.Verify = 2; cp++;
					} else {
						CmdWrite.Verify = 1;
					}
					break;

				case 'c' :	/* -c */
					CmdWrite.CopyCal = 1; break;

				case 'e' :	/* -e */
					Command[0] = 'e'; break;

				case 'z' :	/* -z */
					Command[0] = 'z'; break;

				case 'r' :	/* -r{p|e|f} */
					Command[0] = 'r';
					if(*cp) Command[1] = tolower(*cp++);
					break;

				case 'f' :	/* -f{l|h|x}<bin> */
					c = tolower(*cp++);
					if(*cp <= ' ') return RC_SYNTAX;
#if AVRSPX
					ln = strtoul_ex(cp, &cp, 0);
#else
					ln = strtoul(cp, &cp, 2);
#endif
					switch (c) {
						case 'l' :
							CmdFuse.Cmd.Flag.Low = 1;
							CmdFuse.Data[0] = (BYTE)ln;
							break;
						case 'h' :
							CmdFuse.Cmd.Flag.High = 1;
							CmdFuse.Data[1] = (BYTE)ln;
							break;
						case 'x' :
							CmdFuse.Cmd.Flag.Extend = 1;
							CmdFuse.Data[2] = (BYTE)ln;
							break;
						default :
							return RC_SYNTAX;
					}
					break;

				case 'l' :	/* -l[<bin>] */
					CmdFuse.Cmd.Flag.Lock = 1;
#if AVRSPX
					CmdFuse.Data[3] = (BYTE)strtoul_ex(cp, &cp, 0);
#else
					CmdFuse.Data[3] = (BYTE)strtoul(cp, &cp, 2);
#endif
					break;

				case 'p' :	/* -p{c|l|v|b}<num> */
					switch (tolower(*cp++)) {
						case 'c' :
							CtrlPort.PortClass = TY_COMM;
							break;
						case 'l' :
							CtrlPort.PortClass = TY_LPT;
							break;
						case 'v' :
							CtrlPort.PortClass = TY_VCOM;
							break;
						case 'b' :
							CtrlPort.PortClass = TY_BRIDGE;
							break;
#if AVRSPX
						case 'h' :
							CtrlPort.PortClass = TY_HIDASP;
							break;
						case 'u' :
							CtrlPort.PortClass = TY_USBASP;	//@@@ by t.k
							s = strchr(cp, ':');
							if (s) {
								strncpy(usb_serial, s+1, sizeof(usb_serial));
								CtrlPort.SerialNumber = usb_serial;
								cp += strlen(cp);
							} else  {
								CtrlPort.SerialNumber = NULL;
							}
							if (*cp == '?') {
							    f_usblist = true;
							    ++cp;
                            }
							break;
						case 'f' :
							CtrlPort.PortClass = TY_RSCR;	//@@@ by t.k
							break;
						case '?' :
							f_portlist = true;
							break;
#endif
						default :
							return RC_SYNTAX;
					}
					CtrlPort.PortNum = (WORD)strtoul(cp, &cp, 10);
					if(*cp == ':')
						CtrlPort.Baud = strtoul(cp+1, &cp, 10);
					break;

				case 'd' :	/* -d<num> */
#if AVRSPX
					if(*cp == '-')	//@@@ by t.k
					{
						CtrlPort.Delay = -1;	// default value (3)
						cp++;
					} else {
						CtrlPort.Delay = (WORD)strtoul(cp, &cp, 10);
					}
#else
					CtrlPort.Delay = (WORD)strtoul(cp, &cp, 10);
#endif
					break;

				case 'w' :	/* -w<num> (pause before exit) */
#if AVRSPX
					Pause = strtoul(cp, &cp, 10);
#else
					Pause = strtoul(cp, &cp, 2) + 1;
#endif
					break;

				case 't' :	/* -t<device> (force device type) */
					for(ln = 0; ln < sizeof(ForcedName); ln++, cp++) {
						if((ForcedName[ln] = *cp) == '\0') break;
					}
					break;
#if AVRSPX

				case 'q' :	/* -q<device> (check device type) */
					for(ln = 0; ln < sizeof(DeviceName); ln++, cp++) {
						if((DeviceName[ln] = *cp) == '\0') break;
					}
					break;

				case 'i' : /*@@ by t.k -i<cfgfile.ini> */
					/* already exec. (setup_commnads_ex) */
                    cp += strlen(cp);
					break;

                case '-':   /* --xxx */
                    if (strncmp(cp, "pause-on-start", 14) == 0) {
						cp += 14;
						if (*cp == '=')
							msg_pause_on_start = cp + 1;
						else
							msg_pause_on_start = "";
					} else if (strncmp(cp, "pause-on-exit", 13) == 0) {
						cp += 13;
						if (*cp == '=')
							msg_pause_on_exit = cp + 1;
						else
							msg_pause_on_exit = "";
					} else if (strncmp(cp, "list-port", 9) == 0) {
						f_portlist = true;
					} else if (strncmp(cp, "set-serial", 10) == 0) {
                        s = strchr(cp, '=');
                        if (s)
                            strncpy(new_serial, s+1, sizeof(new_serial));
                    } else if (strcmp(cp, "list-usbasp") == 0) {
					    f_usblist = true;
                    } else if (strcmp(cp, "help") == 0) {
					    f_detail_help = true;
                    }
                    cp += strlen(cp);
                    break;

                case '?':  /* thru down */
                case 'h':
				    f_detail_help = true;
                    cp += strlen(cp);
					break;
#endif

				default :	/* invalid command */
					return RC_SYNTAX;
			} /* switch */
			if(*cp >= ' ') return RC_SYNTAX;	/* option trails garbage */
		} /* if */

		else {	/* HEX Files (Write command) */
			if((fp = fopen(cp, "rt")) == NULL) {
				fprintf(stderr, "%s : Unable to open.\n", cp);
				return RC_FILE;
			}
			/* .eep files are read as EEPROM data, others are read as program code */
			if((strstr(cp, ".EEP") == NULL) && (strstr(cp, ".eep") == NULL)) {
				ln = input_hexfile(fp, CodeBuff, sizeof(CodeBuff), &CmdWrite.CodeSize);
			} else {
				ln = input_hexfile(fp, DataBuff, sizeof(DataBuff), &CmdWrite.DataSize);
			}
			fclose(fp);
			if(ln) {
				if(ln < 0) {
					fprintf(stderr, "%s : File access failure.\n", cp);
				} else {
					fprintf(stderr, "%s (%ld) : Hex format error.\n", cp, ln);
				}
				return RC_FILE;
			}
		} /* else */

	} /* for */

	return 0;
}



/*-----------------------------------------------------------------------
  Device control functions
-----------------------------------------------------------------------*/


/* Put the device to ISP mode */

int enter_ispmode ()
{
	BYTE rd;
	int tried, scan;

#if AVRSPX
	if (CtrlPort.PortClass == TY_HIDASP) // ### k-k
	{
		if (hidasp_program_enable(CtrlPort.Delay) != 0)
		{
			MESS("Device connection failed.(PE)\n");
			return 1;
		}
		return 0;
	} else if (CtrlPort.PortClass == TY_USBASP) {	//@@@ by t.k
		if (usbasp_program_enable() != 0) {
			MESS("Device connection failed.\n");
			return 1;
		} else {
			return 0;
		}
	} else if (f_write_only_programmer)	{//@@@ by t.k
		// TY_RSCR
		wronly_universal_command(
			C_EN_PRG1,
			C_EN_PRG2,
			0,
			0);
		return 0;
	}
#endif

	spi_reset();	/* Reset device */

	for(tried = 1; tried <= 3; tried++) {
		for(scan = 1; scan <= 32; scan++) {
			spi_xmit(C_EN_PRG1);			/* 1st cmd */
			spi_xmit(C_EN_PRG2);			/* 2nd cmd */
			rd = spi_rcvr(RM_SYNC);			/* 3rd cmd and read echo back */
			spi_xmit(0);					/* 4th cmd */
			if(rd == C_EN_PRG2) return 0;	/* Was 2nd command echoed back? */
			if(tried <= 2) break;			/* first 2 attempts are for 1200, final attempt is for others */
			spi_clk();						/* shift scan point */
		}
	}

	MESS("Device connection failed.\n");
	return 1;
}




/* Read a byte from device */

BYTE read_byte (
	BYTE src,	/* Read from.. FLASH/EEPROM/SIGNATURE */
	DWORD adr	/* Address */
) {
	BYTE cmd;


	switch (src) {
		case FLASH :
			cmd = (BYTE)((adr & 1) ? C_RD_PRGH : C_RD_PRGL);
			adr >>= 1;							/* !!! @@@ */
			if((Device->FlashSize > (128*1024))
				&& ((adr & 65535) == 0)) {		/* Load extended address if needed */
#if AVRSPX
				spi_transmit(
					C_LD_ADRX,					/* extended address command */
					0,							/* 0 */
					(BYTE)(adr >> 16),			/* address extended */
					0);							/* 0 */
#else
				spi_xmit(C_LD_ADRX);			/* extended address command */
				spi_xmit(0);					/* 0 */
				spi_xmit((BYTE)(adr >> 16));	/* address extended */
				spi_xmit(0);					/* 0 */
#endif
			}
			break;
		case EEPROM :
			cmd = C_RD_EEP;
			break;
		case SIGNATURE :
			cmd = C_RD_SIG;
			break;
		default :
			return 0xFF;
	}
#if AVRSPX
	return spi_transmit_R(
		cmd,						/* Read command */
		(BYTE)(adr >> 8),			/* Address high */
		(BYTE)adr,					/* Address low */
		RM_SYNC);					/* Receive data */
#else
	spi_xmit(cmd);					/* Eead command */
	spi_xmit((BYTE)(adr >> 8));		/* Address high */
	spi_xmit((BYTE)adr);			/* Address low */
	return spi_rcvr(RM_SYNC);		/* Receive data */
#endif
}



/* Read fuse bytes from device into FuseBuff,CalBuff */

void read_fuse ()
{
	int n;


	if(Device->FuseType == 0) return;	/* Type 0 : No fuse */

	if(Device->FuseType == 1) {
#if AVRSPX
		FuseBuff[0] = spi_transmit_R(
			C_RD_FLB1,			/* Type 1 : Read fuse low */
			0,
			0,
			RM_SYNC);
#else
		spi_xmit(C_RD_FLB1);			/* Type 1 : Read fuse low */
		spi_xmit(0);
		spi_xmit(0);
		FuseBuff[0] = spi_rcvr(RM_SYNC);
#endif
	}
	else {
#if AVRSPX
		FuseBuff[0] = spi_transmit_R(
			C_RD_FB1,				/* Type 2..6 : Read fuse low */
			0,
			0,
			RM_SYNC);
#else
		spi_xmit(C_RD_FB1);				/* Type 2..6 : Read fuse low */
		spi_xmit(0);
		spi_xmit(0);
		FuseBuff[0] = spi_rcvr(RM_SYNC);
#endif

		if(Device->FuseType >= 5) {
#if AVRSPX
			FuseBuff[1] = spi_transmit_R(
				C_RD_FLB1,		/* Type 5..6 : Read fuse high */
				0x08,
				0,
				RM_SYNC);
#else
			spi_xmit(C_RD_FLB1);		/* Type 5..6 : Read fuse high */
			spi_xmit(0x08);
			spi_xmit(0);
			FuseBuff[1] = spi_rcvr(RM_SYNC);
#endif

			if(Device->FuseType >= 6) {
#if AVRSPX
			FuseBuff[2] = spi_transmit_R(
				C_RD_FB1,		/* Type 6 : Read fuse extend */
				0x08,
				0,
				RM_SYNC);
#else
				spi_xmit(C_RD_FB1);		/* Type 6 : Read fuse extend */
				spi_xmit(0x08);
				spi_xmit(0);
				FuseBuff[2] = spi_rcvr(RM_SYNC);
#endif
			}
		}
	}

	for(n = 0; n < Device->Cals; n++) {	/* Read calibration bytes if present */
#if AVRSPX
		CalBuff[n] = spi_transmit_R(
			C_RD_CAL,
			0,
			(BYTE)n,
			RM_SYNC);
#else
		spi_xmit(C_RD_CAL);
		spi_xmit(0);
		spi_xmit((BYTE)n);
		CalBuff[n] = spi_rcvr(RM_SYNC);
#endif
	}
}




/* Write a byte into memory */

int write_byte (
	char dst,	/* Write to.. FLASH/EEPROM */
	DWORD adr,	/* Address */
	BYTE wd		/* Data to be written */
) {
	int n;


	switch (dst) {
		case FLASH :
			if(wd == 0xFF) return 0;	/* Skip if the value is 0xFF */

		case FLASH_NS :
#if AVRSPX
			spi_transmit(
				(BYTE)((adr & 1) ? C_WR_PRGH : C_WR_PRGL),
				(BYTE)(adr >> 9),
				(BYTE)(adr >> 1),
				wd);
#else
			spi_xmit((BYTE)((adr & 1) ? C_WR_PRGH : C_WR_PRGL));
			spi_xmit((BYTE)(adr >> 9));
			spi_xmit((BYTE)(adr >> 1));
			spi_xmit(wd);
#endif
			if((Device->PollData == wd)				/* Write data is equal to poll data */
				|| (Device->ID == S1200)			/* 90S1200 cannot be polled */
#if AVRSPX
				|| (CtrlPort.PortClass == TY_RSCR)
#endif
				|| (CtrlPort.PortClass == TY_VCOM))	/* Preventing read access for USB serial */
			{	delay_ms(Device->FlashWait);
				return 0;
			}
			break;

		case EEPROM :
#if AVRSPX
			spi_transmit(
				C_WR_EEP,
				(BYTE)(adr >> 8),
				(BYTE)adr,
				wd);
#else
			spi_xmit(C_WR_EEP);
			spi_xmit((BYTE)(adr >> 8));
			spi_xmit((BYTE)adr);
			spi_xmit(wd);
#endif
			if((Device->PollData == wd) || (Device->PollData == (BYTE)~wd)
				|| (Device->ID == S1200)
#if AVRSPX
				|| (CtrlPort.PortClass == TY_RSCR)
#endif
				|| (CtrlPort.PortClass == TY_VCOM))
			{	delay_ms(Device->EepromWait);
				return 0;
			}
			break;

		default:
			return 1;
	}

	for(n = 0; n < 200; n++) {	/* Wait for end of programming (Polling) */
		if(read_byte(dst, adr) == wd) return 0;
	}

	return 1;	/* Polling time out */
}




/* Write Fuse or Lock byte */

void write_fuselock (
	char dst,		/* which fuse to be written */
	BYTE val		/* fuse value */
) {
	switch (dst) {
		case F_LOW :	/* Fuse Low byte */
			if(Device->FuseType <= 2) {		/* Type 1,2 */
#if AVRSPX
				spi_transmit(
					C_WR_FLB,
					(BYTE)(val & 0xBF),
					0,
					0);
#else
				spi_xmit(C_WR_FLB);
				spi_xmit((BYTE)(val & 0xBF));
				spi_xmit(0);
				spi_xmit(0);
#endif
			} else {						/* Type 3..6 */
#if AVRSPX
				spi_transmit(
					C_WR_FLB,
					C_WR_FLBL,
					0,
					val);
#else
				spi_xmit(C_WR_FLB);
				spi_xmit(C_WR_FLBL);
				spi_xmit(0);
				spi_xmit(val);
#endif
			}
			break;

		case F_HIGH :	/* Fuse High byte */
#if AVRSPX
			spi_transmit(
				C_WR_FLB,
				C_WR_FLBH,
				0,
				val);
#else
			spi_xmit(C_WR_FLB);
			spi_xmit(C_WR_FLBH);
			spi_xmit(0);
			spi_xmit(val);
#endif
			break;

		case F_EXTEND :	/* Fuse Extend byte */
#if AVRSPX
			spi_transmit(
				C_WR_FLB,
				C_WR_FLBX,
				0,
				val);
#else
			spi_xmit(C_WR_FLB);
			spi_xmit(C_WR_FLBX);
			spi_xmit(0);
			spi_xmit(val);
#endif
			break;

		case F_LOCK :	/* Device Lock byte */
			if(Device->FuseType <= 3) {		/* Type 0..3 */
#if AVRSPX
				spi_transmit(
					C_WR_FLB,
					(BYTE)(C_WR_FLBK | val),
					0,
					0);
#else
				spi_xmit(C_WR_FLB);
				spi_xmit((BYTE)(C_WR_FLBK | val));
				spi_xmit(0);
				spi_xmit(0);
#endif
			} else {						/* Type 4..6 */
#if AVRSPX
				spi_transmit(
					C_WR_FLB,
					C_WR_FLBK,
					0,
					val);
#else
				spi_xmit(C_WR_FLB);
				spi_xmit(C_WR_FLBK);
				spi_xmit(0);
				spi_xmit(val);
#endif
			}
	} /* switch */
	delay_ms(20);	/* Wait for 20ms */
	spi_flush();
}


#if AVRSPX
/* Verify Fuse or Lock byte */
BYTE verify_fuselock (char dst,		/* which fuse to be written */
					 BYTE val)		/* fuse value */
{
	BYTE fuse;

	if (Device->FuseType == 0)
		return val;		/* Type 0 : No fuse */

	fuse = val;
	switch (dst)
	{
	  case F_LOW :	/* Fuse Low byte */
		if (Device->FuseType == 1) {
			fuse = spi_transmit_R(
					C_RD_FLB1,		/* Type 1 : Read fuse low */
					0,
					0,
					RM_SYNC);
		} else {
			fuse = spi_transmit_R(
					C_RD_FB1,		/* Type 2..6 : Read fuse low */
					0,
					0,
					RM_SYNC);
		}
		break;

	  case F_HIGH :	/* Fuse High byte */
		if (Device->FuseType >= 5) {
			fuse = spi_transmit_R(
					C_RD_FLB1,		/* Type 5..6 : Read fuse high */
					0x08,
					0,
					RM_SYNC);
		}
		break;

	  case F_EXTEND :	/* Fuse Extend byte */
		if (Device->FuseType >= 6) {
			fuse = spi_transmit_R(
					C_RD_FB1,		/* Type 6 : Read fuse extend */
					0x08,
					0,
					RM_SYNC);
		}
		break;

	  case F_LOCK :	/* Device Lock byte */
		if (Device->FuseType >= 1) {
			fuse = spi_transmit_R(
					C_RD_FLB1,		/* Type 1 : Read fuse lock */
					0,
					0,
					RM_SYNC);
		}
		break;
	}

    return fuse;
}
#endif


/* Issue chip erase command and re-enter ISP mode */

int erase_memory ()
{
#if AVRSPX
	spi_transmit(
		C_ERASE1,	/* Issue a Chip Erase command */
		C_ERASE2,
		0,
		0);
#else
	spi_xmit(C_ERASE1);	/* Issue a Chip Erase command */
	spi_xmit(C_ERASE2);
	spi_xmit(0);
	spi_xmit(0);
#endif

	delay_ms(40);		/* Wait for 40ms */

	if(Device->ID == T12) {		/* This is to avoid Tiny12's bug */
		if(enter_ispmode()) return RC_DEV;
		write_byte(FLASH_NS, 0, 0xFF);	/* Dummy write */

#if AVRSPX
		spi_transmit(
			C_ERASE1,			/* Issue 2nd Chip Erase command */
			C_ERASE2,
			0,
			0);
#else
		spi_xmit(C_ERASE1);		/* Issue 2nd Chip Erase command */
		spi_xmit(C_ERASE2);
		spi_xmit(0);
		spi_xmit(0);
#endif

		delay_ms(40);			/* Wait for 40ms */
	}
	spi_flush();

	/* Re-enter ISP mode and return its status */
	return enter_ispmode() ? RC_DEV : 0;
}




/* Initialize control port and device if needed */

int init_devices ()
{
	int res;

	/* Execute initialization if not initialized yet */
	if(Device == NULL) {
		res = open_ifport(&CtrlPort);		/* Open interface port and show port information */
		if(CtrlPort.Info1)
			MESS(CtrlPort.Info1);
		if(CtrlPort.Info2)
			MESS(CtrlPort.Info2);
		if(res) return RC_INIT;				/* return if failed open_ifport() */
#if AVRSPX
		if (msg_pause_on_start && *msg_pause_on_start) {
			MESS("\r");
			MESS(msg_pause_on_start);
			getchar();
		}
#endif

		if(enter_ispmode()) return RC_DEV;

		/* read device signature */
#if AVRSPX
		if (CtrlPort.PortClass == TY_RSCR) {	// by t.k
			SignBuff[0] = SignBuff[1] = SignBuff[2] = SignBuff[3] = 0;
		} else {
	 		read_multi(SIGNATURE, 0, 4, SignBuff);
 		}
#else
		read_multi(SIGNATURE, 0, 4, SignBuff);
#endif

		/* search device */
		for(Device = DevLst; Device->ID != N0000; Device++) {
			if(ForcedName[0]) {
				if(strcmp(ForcedName, Device->Name) == 0) break;
			} else {
				if(memcmp(SignBuff, Device->Sign, 3) == 0) break;
			}
		}

		/* Show found device type */
		switch (Device->ID) {
			case N0000 :
				fprintf(stderr, "Unknown device (%02X-%02X-%02X).\n",
						SignBuff[0], SignBuff[1], SignBuff[2]	// @@@
				);
				break;

			case L0000 :
				fprintf(stderr, "Locked device or Synchronization failed.\n");
				break;

			default :
				fprintf(stderr, "Detected device is AT%s.\n", Device->Name);
#if AVRSPX
				if (DeviceName[0]) {	//@@+ check device by t.k
					DEVPROP *dev = search_device(DeviceName);
					if (dev->ID != Device->ID) {
                        fprintf(stderr, "Invalid Device %s != %s\n",
							DeviceName, Device->Name);
						return RC_DEV;
					}
				}
				break;
#endif
		}
	} /* if */

	return 0;
}




/*-----------------------------------------------------------------------
  Programming functions
-----------------------------------------------------------------------*/


/* -e command */

int erase_device ()
{
	int rc;


	rc = init_devices();
	if(rc != 0) return rc;

	if(Device->ID == N0000) return RC_DEV;		/* Abort if unknown device */

	rc = erase_memory();
	if(rc != 0) return rc;
	MESS("Erased.\n");

	return 0;
}




/* -z command */

int test_ctrlport ()
{
	int n;


	n = open_ifport(&CtrlPort);		/* Open interface port and show port information */
	if(CtrlPort.Info1)
		MESS(CtrlPort.Info1);
	if(CtrlPort.Info2)
		MESS(CtrlPort.Info2);
	if(n) return RC_INIT;			/* return if failed open_ifport() */

	MESS("1000Hz test pulse on SCK. This takes a time...");
	for (n = 0; n < 10000; n++) {
		delay_ms(1);
		spi_clk();
	}
	spi_flush();

	return 0;
}




/* -r command */

int read_device (char cmd)
{
	DWORD adr, ws;
	int rc;


	rc = init_devices();
	if(rc != 0) return rc;
	if(Device->ID <= L0000) return RC_DEV;		/* Abort if unknown device or locked device */

#if AVRSPX
	if (CtrlPort.PortClass == TY_RSCR) {		// by t.k
		MESS("rscr was write_only adapter.\n");
		return RC_FAIL;
	}
#endif

	switch (cmd) {
		case 'p' :	/* -rp : read program memory */
#if AVRSPX
			report_setup("Read    Flash:", Device->FlashSize);
			if (CtrlPort.PortClass == TY_USBASP) {
				usbasp_paged_load(FLASH, CodeBuff, Device->FlashPage,
							 Device->FlashSize, Device->FlashSize > (128*1024));
			} else if (CtrlPort.PortClass == TY_HIDASP && Device->FlashSize <= (128*1024)) {
				hidasp_page_read(0, CodeBuff, Device->FlashSize);
			} else {
	 			for(adr = 0; adr < Device->FlashSize; adr += PIPE_WINDOW) {
					read_multi(FLASH, adr, PIPE_WINDOW, &CodeBuff[adr]);
					report_update(PIPE_WINDOW);
				}
			}
			report_finish();
#else
			MESS("Reading Flash...");
			for(adr = 0; adr < Device->FlashSize; adr += PIPE_WINDOW)
				read_multi(FLASH, adr, PIPE_WINDOW, &CodeBuff[adr]);
#endif
			MESS("Passed.\n");
			output_hexfile(stdout, CodeBuff, Device->FlashSize, 32);
			break;

		case 'e' :	/* -re : read eeprom */
#if AVRSPX
			report_setup("Read   EEPROM:", Device->EepromSize);
			if (CtrlPort.PortClass == TY_USBASP) {
				usbasp_paged_load(EEPROM, DataBuff, Device->EepromPage,
								 Device->EepromSize, false);
			} else {
				ws = (Device->EepromSize < PIPE_WINDOW) ? Device->EepromSize : PIPE_WINDOW;
				for(adr = 0; adr < Device->EepromSize; adr += ws) {
					read_multi(EEPROM, adr, ws, &DataBuff[adr]);
					report_update(ws);
				}
			}
			report_finish();
#else
			MESS("Reading EEPROM...");
			ws = (Device->EepromSize < PIPE_WINDOW) ? Device->EepromSize : PIPE_WINDOW;
			for(adr = 0; adr < Device->EepromSize; adr += ws)
				read_multi(EEPROM, adr, ws, &DataBuff[adr]);
#endif
			MESS("Passed.\n");
			output_hexfile(stdout, DataBuff, Device->EepromSize, 32);
			break;

		case 'f' :	/* -rf : read fuses */
			read_fuse();
			output_fuse();
			break;

		default :
			output_deviceinfo();
			break;
	}

	return 0;
}




/* .hex file write command */

int write_flash ()
{
	DWORD adr;
	BYTE rd[PIPE_WINDOW];
	int rc, n;


	rc = init_devices();
	if(rc != 0) return rc;
	if(Device->ID <= L0000) return RC_DEV;	/* Abort if unknown device or locked device */

#if AVRSPX
	if(CmdWrite.CodeSize > Device->FlashSize) {
		MESS("Flash write error: program size > memory size.\n");
		return RC_FAIL;
	}
#else
	MESS("Flash: ");
	if(CmdWrite.CodeSize > Device->FlashSize) {
		MESS("error: program size > memory size.\n");
		return RC_FAIL;
	}
#endif

	if(CmdWrite.Verify != 1) {	/* -v : Skip programming process when verify only mode */

#if AVRSPX
		MESS("Erase Flash memory.\n");				/* Erase device before programming */
#else
		MESS("Erasing...");						/* Erase device before programming */
#endif
		rc = erase_memory();
		if(rc != 0) return rc;

		if(CmdWrite.CopyCal && Device->Cals) {	/* -c : Copy calibration bytes */
			read_fuse();
			for(n = 0; n < Device->Cals; n++)
				CodeBuff[Device->FlashSize - 1 - n] = CalBuff[n];
			CmdWrite.CodeSize = Device->FlashSize;
		}

#if AVRSPX
		report_setup("Write   Flash:", CmdWrite.CodeSize);
		if (CtrlPort.PortClass == TY_USBASP) {
			rc = usbasp_paged_write(FLASH, CodeBuff, Device->FlashPage,
							CmdWrite.CodeSize, Device->FlashSize > (128*1024));
			if (rc != CmdWrite.CodeSize) {
				report_finish();
				return RC_FAIL;
			}
		} else {
			if(Device->FlashPage) {		/* Write flash in page mode */
				for(adr = 0; adr < CmdWrite.CodeSize; adr += Device->FlashPage) {
					write_page(adr, &CodeBuff[adr]);
					report_update(Device->FlashPage);
				}
			} else {						/* Write flash in byte-by-byte mode */
				for(adr = 0; adr < CmdWrite.CodeSize; adr++) {
					if(write_byte(FLASH, adr, CodeBuff[adr])) {
						report_finish();
						fprintf(stderr, "Time out at %04X\n", (int)adr);
						return RC_FAIL;
					}
					report_update(1);
				}
			}
			spi_flush();
		}
		report_finish();
#else	// @@@
		MESS("Writing...");
		if(Device->FlashPage) {		/* Write flash in page mode */
			for(adr = 0; adr < CmdWrite.CodeSize; adr += Device->FlashPage) {
				write_page(adr, &CodeBuff[adr]);
			}
		} else {						/* Write flash in byte-by-byte mode */
			for(adr = 0; adr < CmdWrite.CodeSize; adr++) {
				if(write_byte(FLASH, adr, CodeBuff[adr])) {
					report_finish();
					fprintf(stderr, "Time out at %04X\n", adr);
					return RC_FAIL;
				}
			}
		}
		spi_flush();
#endif
	} /* if */

	if(CmdWrite.Verify != 2) {	/* -v- : Skip verifying process when programming only mode */
#if AVRSPX
		report_setup("Verify  Flash:", CmdWrite.CodeSize);

		if (CtrlPort.PortClass == TY_USBASP) {
			rc = usbasp_paged_verify(FLASH, CodeBuff, Device->FlashPage,
							CmdWrite.CodeSize, Device->FlashSize > (128*1024));
			if (rc != 0) {
				report_finish();
				return RC_FAIL;
			}
		} else if (CtrlPort.PortClass == TY_HIDASP) {
			hidasp_page_read(0, rd, 0);
			for(adr = 0; adr < CmdWrite.CodeSize; adr += PIPE_WINDOW) {
				hidasp_page_read(-1, rd, PIPE_WINDOW);
				for(n = 0; n < PIPE_WINDOW; n++) {
					if(rd[n] != CodeBuff[adr+n]) {
						fprintf(stderr, "\nFailed at %04X:%02X-%02X\n",
							(int)(adr+n), (int)CodeBuff[adr+n], (int)rd[n]);
						return RC_FAIL;
					}
				}
			}
		} else {
			for(adr = 0; adr < CmdWrite.CodeSize; adr += PIPE_WINDOW) {
				read_multi(FLASH, adr, PIPE_WINDOW, rd);
				for(n = 0; n < PIPE_WINDOW; n++) {
					if(rd[n] != CodeBuff[adr+n]) {
						report_finish();
						fprintf(stderr, "\nFailed at %04X:%02X-%02X\n",
							(int)(adr+n), (int)CodeBuff[adr+n], (int)rd[n]);
						return RC_FAIL;
					}
				}
				report_update(PIPE_WINDOW);
			}
		}
		report_finish();
#else
		MESS("Verifying...");

		if (CtrlPort.PortClass == TY_USBASP) {
			rc = usbasp_paged_verify(FLASH, CodeBuff, Device->FlashPage,
							CmdWrite.CodeSize, Device->FlashSize > (128*1024));
			if (rc != 0) {
				report_finish();
				return RC_FAIL;
			}
		} else {
			for(adr = 0; adr < CmdWrite.CodeSize; adr += PIPE_WINDOW) {
				read_multi(FLASH, adr, PIPE_WINDOW, rd);
				for(n = 0; n < PIPE_WINDOW; n++) {
					if(rd[n] != CodeBuff[adr+n]) {
						report_finish();
						fprintf(stderr, "Failed at %04X:%02X-%02X\n", adr+n, CodeBuff[adr+n], rd[n]);
						return RC_FAIL;
					}
				}
			}
		}
#endif
	} /* if */

	MESS("Passed.\n");

	return 0;
}




/* .eep file write command */

int write_eeprom ()
{
	DWORD adr, n, ws;
	BYTE rd[PIPE_WINDOW];
	int rc;


	rc = init_devices();
	if(rc != 0) return rc;
	if(Device->ID <= L0000) return RC_DEV;	/* Abort if unknown device or locked device */

#if AVRSPX
	if(CmdWrite.DataSize > Device->EepromSize) {
		MESS("EEPROM write error: data size > memory size.\n");
		return RC_FAIL;
	}
#else
	MESS("EEPROM: ");
	if(CmdWrite.DataSize > Device->EepromSize) {
		MESS("error: data size > memory size.\n");
		return RC_FAIL;
	}
#endif

	if(CmdWrite.Verify != 1) {	/* -v : Skip programming process when verify mode */
#if AVRSPX
		report_setup("Write  EEPROM:", CmdWrite.DataSize);
		if (CtrlPort.PortClass == TY_USBASP) {		/* Write EEPROM in page mode */
			rc = usbasp_paged_write(EEPROM, DataBuff, Device->EepromPage, CmdWrite.DataSize, false);
			if (rc != CmdWrite.DataSize) {
				report_finish();
				return (RC_FAIL);
			}
		} else {
			for(adr = 0; adr < CmdWrite.DataSize; adr++) {	/* Write EEPROM without erase */
				if(write_byte(EEPROM, adr, DataBuff[adr])) {
					fprintf(stderr, "Time out at %04X\n", (int)adr);
					return RC_FAIL;
				}
				report_update(1);
			}
			spi_flush();
		}
		report_finish();
#else
		MESS("Writing...");
		for(adr = 0; adr < CmdWrite.DataSize; adr++) {	/* Write EEPROM without erase */
			if(write_byte(EEPROM, adr, DataBuff[adr])) {
				fprintf(stderr, "Time out at %04X\n", adr);
				return RC_FAIL;
			}
		}
		spi_flush();
#endif
	}

	if(CmdWrite.Verify != 2) {	/* -v- : Skip verifying process when programming only mode */
#if AVRSPX
		report_setup("Verify EEPROM:", CmdWrite.DataSize);
		if (CtrlPort.PortClass == TY_USBASP) {		/* Read eeprom in page mode */
			rc = usbasp_paged_verify(EEPROM, DataBuff, Device->EepromPage ,
							CmdWrite.DataSize, false);
			if (rc != 0) {
				report_finish();
				return RC_FAIL;
			}
		} else {
			ws = (Device->EepromSize < PIPE_WINDOW) ? Device->EepromSize : PIPE_WINDOW;
			for(adr = 0; adr < CmdWrite.DataSize; adr += ws) {
				read_multi(EEPROM, adr, ws, rd);
				for(n = 0; n < ws; n++) {
					if(rd[n] != DataBuff[adr+n]) {
						fprintf(stderr, "\nFailed at %04X:%02X-%02X\n",
							(int)(adr+n), (int)DataBuff[adr+n], (int)rd[n]);
						return RC_FAIL;
					}
				}
				report_update(ws);
			}
		}
		report_finish();
#else
		MESS("Verifying...");
		ws = (Device->EepromSize < PIPE_WINDOW) ? Device->EepromSize : PIPE_WINDOW;
		for(adr = 0; adr < CmdWrite.DataSize; adr += ws) {
			read_multi(EEPROM, adr, ws, rd);
			for(n = 0; n < ws; n++) {
				if(rd[n] != DataBuff[adr+n]) {
					fprintf(stderr, "Failed at %04X:%02X-%02X\n", adr+n, DataBuff[adr+n], rd[n]);
					return RC_FAIL;
				}
			}
		}
#endif
	}

	MESS("Passed.\n");

	return 0;
}




/* -f{l|h|x}, -l command */

int write_fuse ()
{
	int	rc;
#if AVRSPX
	BYTE fuse, vfuse;
#endif


	rc = init_devices();
	if(rc != 0) return rc;
	if(Device->ID <= L0000) return RC_DEV;		/* Abort if unknown device or locked device */

	if(CmdFuse.Cmd.Flag.Low && (Device->FuseType > 0)) {
#if AVRSPX
		vfuse = fuse = CmdFuse.Data[0] & Device->FuseMask[0];

		/* -v : Skip programming process when verify mode */
		if (CmdWrite.Verify != 1)
			write_fuselock(F_LOW, fuse | ~Device->FuseMask[0]);

		/* -v- : Skip verifying process when programming only mode */
		if (CmdWrite.Verify != 2)
			vfuse = verify_fuselock(F_LOW, fuse) & Device->FuseMask[0];

		if (vfuse != fuse)
			fprintf(stderr, "Fuse Low byte was programm error. (%02X %02X)\n", fuse, vfuse);
		else
#else
		write_fuselock(F_HIGH, (BYTE)(CmdFuse.Data[0] | ~Device->FuseMask[0]));
#endif
		write_fuselock(F_LOW, (BYTE)(CmdFuse.Data[0] | ~Device->FuseMask[0]));
		MESS("Fuse Low byte was programmed.\n");
	}

	if(CmdFuse.Cmd.Flag.High && (Device->FuseType >= 5)) {
#if AVRSPX
		vfuse = fuse = CmdFuse.Data[1] & Device->FuseMask[1];

		/* -v : Skip programming process when verify mode */
		if (CmdWrite.Verify != 1)
			write_fuselock(F_HIGH, fuse | ~Device->FuseMask[1]);

		/* -v- : Skip verifying process when programming only mode */
		if (CmdWrite.Verify != 2)
			vfuse = verify_fuselock(F_HIGH, fuse) & Device->FuseMask[1];
		if (vfuse != fuse)
			fprintf(stderr, "Fuse High byte was programm error. (%02X %02X)\n", fuse, vfuse);
		else
#else
		write_fuselock(F_HIGH, (BYTE)(CmdFuse.Data[1] | ~Device->FuseMask[1]));
#endif
		MESS("Fuse High byte was programmed.\n");
	}

	if(CmdFuse.Cmd.Flag.Extend && (Device->FuseType >= 6)) {
#if AVRSPX
		vfuse = fuse = CmdFuse.Data[2] & Device->FuseMask[2];

		/* -v : Skip programming process when verify mode */
		if (CmdWrite.Verify != 1)
			write_fuselock(F_EXTEND, fuse | ~Device->FuseMask[2]);

		/* -v- : Skip verifying process when programming only mode */
		if (CmdWrite.Verify != 2)
			vfuse = verify_fuselock(F_EXTEND, fuse) & Device->FuseMask[2];
		if (vfuse != fuse)
			fprintf(stderr, "Fuse Extend byte was programm error. (%02X %02X)\n",fuse,vfuse);
		else
#else
		write_fuselock(F_EXTEND, (BYTE)(CmdFuse.Data[2] | ~Device->FuseMask[2]));
#endif
		MESS("Fuse Extend byte was programmed.\n");
	}

	if(CmdFuse.Cmd.Flag.Lock) {
#if AVRSPX
		fuse  = CmdFuse.Data[3] ? CmdFuse.Data[3] : Device->LockData;
		vfuse = fuse;

		/* -v : Skip programming process when verify mode */
		if (CmdWrite.Verify != 1)
			write_fuselock(F_LOCK, fuse);

		/* -v- : Skip verifying process when programming only mode */
		if (CmdWrite.Verify != 2)
			vfuse = verify_fuselock(F_LOCK, fuse);
		if (vfuse != fuse)
			fprintf(stderr, "Lock bits programm error. (%02X %02X)\n", fuse, vfuse);
		else
#else
		write_fuselock(F_LOCK, (BYTE)(CmdFuse.Data[3] ? CmdFuse.Data[3] : Device->LockData));
#endif
		MESS("Lock bits are programmed.\n");
	}

	return 0;
}




/* Terminate process */
#if AVRSPX
static bool f_terminate;
#endif

void terminate (int rc)
{
#if AVRSPX
	f_terminate = true;
#if DEBUG
	printf("Pause = %d, rc = %d\n", Pause, rc);
#endif
#endif

	close_ifport();
	Device = NULL;

#if AVRSPX
	if (msg_pause_on_exit && *msg_pause_on_exit) {
		MESS("\n");
		MESS(msg_pause_on_exit);
 		getchar();
	} else if((Pause == 0)) {
		return;
	} else if((Pause == 1)) {
		MESS("\nType Enter to exit...");
		getchar();
	} else if(Pause >= 2) {
		int i;
		i = Pause;
		do {
#if DEBUG
			fprintf(stderr, "Pause = %3d\r", i);
#endif
			Sleep(1000);
			if (kbhit()) {
				getchar();
				break;
			}
		} while (i--);
	}
#else
	if((Pause == 1) || ((Pause == 2)&&(rc != 0))) {
		MESS("\nType Enter to exit...");
		getchar();
	}
#endif
}

#if AVRSPX
void do_exit(void)
{
	if (!f_terminate)
		terminate(-1);
}
#endif


/*-----------------------------------------------------------------------
  Main
-----------------------------------------------------------------------*/


int main (int argc, char **argv)
{
	int rc = 0;

#if AVRSPX
	atexit(do_exit);

	rc = load_commands(argc, argv);
	if(rc != 0) {
		if(rc == RC_SYNTAX) output_usage(true);
		terminate(rc);
		return rc;
	}

    if (f_detail_help) {
		output_usage(true);
		terminate(rc = 0);
		return rc;
    }

    if (f_usblist) {
        if (usbasp_list() < 1)
            return 1;
		terminate(rc = 0);
        return rc;
    }

    if (f_portlist) {
		dump_port_list();
		terminate(rc = 0);
        return rc;
    }

    if (new_serial[0]) {
		rc = usbasp_write_serial(CtrlPort.SerialNumber, new_serial);
        terminate(rc);
		return rc;
    }

	/* Read device and terminate if -R{P|E|F} command is specified */
	if (CtrlPort.PortClass == TY_RSCR) {	// by t.k
		// write only
		CmdWrite.Verify = 2;
	}

#else
	if(rc = load_commands(argc, argv)) {
		if(rc == RC_SYNTAX) output_usage(true);
		terminate(rc);
		return rc;
	}
#endif

	/* Read device and terminate if -R{P|E|F} command is specified */
	if(Command[0] == 'r') {
		rc = read_device(Command[1]);
		terminate(rc);
		return rc;
	}

	/* Erase device and terminate if -E command is specified */
	if(Command[0] == 'e') {
		rc = erase_device();
		terminate(rc);
		return rc;
	}

	/* Timing test if -Z command is specified */
	if(Command[0] == 'z') {
		rc = test_ctrlport();
		terminate(rc);
		return rc;
	}

	/* Write into device if any file is loaded */
	if(CmdWrite.CodeSize) {
		rc = write_flash();
		if(rc != 0) {
			terminate(rc);
			return rc;
		}
	}
	if(CmdWrite.DataSize) {
		rc = write_eeprom();
		if(rc != 0) {
			terminate(rc);
			return rc;
		}
	}

	/* Write fuse,lock if -F{L|H|X}, -L are specified */
	if(CmdFuse.Cmd.Flags) {
		rc = write_fuse();
		if(rc != 0) {
			terminate(rc);
			return rc;
		}
	}

	if(Device == NULL) {
		rc = RC_SYNTAX;
		output_usage(false);
	}
	terminate(rc);
	return 0;
}

