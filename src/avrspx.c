/*---------------------------------------------------------------------------*/
/* AVRSP - AVR Serial Programming Controller                                 */
/*                                                                           */
/* R0.44 (C)ChaN, 2008                                                       */
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
/* R0.44  Dec  7, '08  ATmega325P/3250P/324PA, AT90PWM216/316                */
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
/* b10.12 2008-12-10 senshuがHIDaspxへの対応を追加、avrsp R0.44の変更を反映  */
/*---------------------------------------------------------------------------*/
/* b11.2  2009-08-06 RSTDISBL のビットを考慮  */
/* b11.3  2009-08-14 DWEN のビットを考慮  */
/* b11.4  2009-09-28 avrdudeとの互換性を強化（-e, Lock bit）*/
/* TAB-SIZE = 4 */

#define VERSION "b11.4"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "avrspx.h"
#if AVRSPX
#include <conio.h>
#include <time.h>	/* clock() ... TIME_DISPLAY */
#include <io.h>
#include <windows.h>
#include "usbasp.h"
#include "hidasp.h"
int hidmon_mode = 0;	/* for hidasp.c */
#endif


/*-----------------------------------------------------------------------
  Device properties
-----------------------------------------------------------------------*/
#if 0	/* avrspx.h */
typedef struct _DEVPROP {
	char	*Name;			/* Name:		Device name */
	char	ID;				/* ID:			Device ID */
	BYTE	Sign[3];		/* Signature:	Device signature bytes */
	DWORD	FlashSize;		/* FS:			Flash memory size in unit of byte */
	WORD	FlashPage;		/* PS:			Flash page size (0 is byte-by-byte) */
	DWORD	EepromSize;		/* ES:			EEPROM size in unit of byte */
	WORD	EepromPage;		/* EP:			EEPROM page size (0 is byte-by-byte) @@@ */
	WORD	FlashWait;		/* FW:			Wait time for flash write */
	WORD	EepromWait;		/* EW:			Wait time for EEPROM write */
	BYTE	PollData;		/* PV:			Polling data value */
	BYTE	LockData;		/* LB:			Default lock byte (program LB1 and LB2) */
	BYTE	LockMask;		/* LBM,			lock byte mask @@@ by senshu */
	char	FuseType;		/* FT:			Device specific fuse type */
	char	Cals;			/* Cals:		Number of calibration bytes */
	BYTE	FuseMask[3];	/* FuseMasks:	Valid fuse bit mask [low, high, ext] */
	BYTE	ISP_DISBL[3];	/* ISP_DISBL:	Valid fuse bit mask [FUSE type, RSRDISBL bit, DWEN bit] */
	WORD	DocNumber;		/* http://www.avrfreaks.net/ @@@ by senshu */
	char	*part_id;		/* Avrdude part's ID @@@ by senshu */
} DEVPROP;
#endif

const DEVPROP DevLst[] =	/* Device property list */
{
	/* Name,         ID,    Signature,              FS,  PS,   ES, EP*,FW, EW,   PV,   LB,  LBM, FT, Cals, FuseMasks,     ISP_DISBL, DocNum, part_id */
//------- AT90S
	{ "90S1200",     S1200, {0x1E, 0x90, 0x01},   1024,   0,   64,  0, 11, 11, 0xFF, 0xF9, 0x06, 0 ,0, {0},               {0, 0x00, 0x00},  8, "1200"},
	{ "90S2313",     S2313, {0x1E, 0x91, 0x01},   2048,   0,  128,  0, 11, 11, 0x7F, 0xF9, 0x06, 0 ,0, {0},               {0, 0x00, 0x00},  9, "2313"},
	{ "90S2323",     S2323, {0x1E, 0x91, 0x02},   2048,   0,  128,  0, 11, 11, 0xFF, 0xF9, 0x06, 1, 0, {0x01},            {0, 0x00, 0x00}, 30, "t22"},
	{ "90S2333",     S2333, {0x1E, 0x91, 0x05},   2048,   0,  128,  0, 11, 11, 0xFF, 0xF9, 0x06, 2, 0, {0x1F},            {0, 0x00, 0x00}, 46, "2333"},
	{ "90S2343",     S2343, {0x1E, 0x91, 0x03},   2048,   0,  128,  0, 11, 11, 0xFF, 0xF9, 0x06, 1, 0, {0x01},            {0, 0x00, 0x00}, 32, "2343"},
	{ "90S4414",     S4414, {0x1E, 0x92, 0x01},   4096,   0,  256,  0, 11, 11, 0x7F, 0xF9, 0x06, 0, 0, {0},               {0, 0x00, 0x00}, 48, "4414"},
	{ "90S4433",     S4433, {0x1E, 0x92, 0x03},   4096,   0,  256,  0, 11, 11, 0xFF, 0xF9, 0x06, 2, 0, {0x1F},            {0, 0x00, 0x00}, 34, "4433"},
	{ "90S4434",     S4434, {0x1E, 0x92, 0x02},   4096,   0,  256,  0, 11, 11, 0xFF, 0xF9, 0x06, 1, 0, {0x01},            {0, 0x00, 0x00}, 13, "4434"},
	{ "90S8515",     S8515, {0x1E, 0x93, 0x01},   8192,   0,  512,  0, 11, 11, 0x7F, 0xF9, 0x06, 0, 0, {0},               {0, 0x00, 0x00}, 37, "8515"},
	{ "90S8535",     S8535, {0x1E, 0x93, 0x03},   8192,   0,  512,  0, 11, 11, 0xFF, 0xF9, 0x06, 1, 0, {0x01},            {0, 0x00, 0x00}, 11, "8535"},
//------- ATtiny
	{ "tiny12",      T12,   {0x1E, 0x90, 0x05},   1024,   0,   64,  2,  5,  8, 0xFF, 0xF9, 0x06, 3, 1, {0xFF},            {0, 0x10, 0x00}, 15, "t12"},
	{ "tiny13",      T13,   {0x1E, 0x90, 0x07},   1024,  32,   64,  4,  6,  5, 0xFF, 0xFC, 0x3F, 5, 1, {0x7F, 0x1F},      {1, 0x01, 0x08}, 74, "t13"},
	{ "tiny15",      T15,   {0x1E, 0x90, 0x06},   1024,   0,   64,  2,  6, 11, 0xFF, 0xF9, 0x06, 3, 1, {0xD3},            {0, 0x10, 0x00}, 44, "t15"},
	{ "tiny22",      T22,   {0x1E, 0x91, 0x06},   2048,   0,  128,  0, 11, 11, 0xFF, 0xF9, 0x06, 1, 0, {0x01},            {0, 0x00, 0x00}, 25, "2343"},
	{ "tiny2313",    T2313, {0x1E, 0x91, 0x0A},   2048,  32,  128,  4,  6,  5, 0xFF, 0xFC, 0x03, 6, 2, {0xFF, 0xDF, 0x01},{1, 0x01, 0x80}, 75, "t2313"},/* DWEN */
	{ "tiny24",      T24,   {0x1E, 0x91, 0x0B},   2048,  32,  128,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x01},{1, 0x80, 0x40},139, "t24"},  /* DWEN */
	{ "tiny25",      T25,   {0x1E, 0x91, 0x08},   2048,  32,  128,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x01},{1, 0x80, 0x40},100, "t25"},  /* DWEN */
	{ "tiny26",      T26,   {0x1E, 0x91, 0x09},   2048,  32,  128,  4,  6, 10, 0xFF, 0xFC, 0x3F, 5, 4, {0xFF, 0x17}      ,{1, 0x10, 0x00}, 60, "t26"},
	{ "tiny261",     T261,  {0x1E, 0x91, 0x0C},   2048,  32,  128,  4,  6, 10, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x01},{1, 0x80, 0x40},142, "t261"}, /* DWEN */
	{ "tiny44",      T44,   {0x1E, 0x92, 0x07},   4096,  64,  256,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x01},{1, 0x80, 0x40},140, "t44"},  /* DWEN */
	{ "tiny45",      T45,   {0x1E, 0x92, 0x06},   4096,  64,  256,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x01},{1, 0x80, 0x40},101, "t45"},  /* DWEN */
	{ "tiny461",     T461,  {0x1E, 0x92, 0x08},   4096,  64,  256,  4,  6, 10, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x01},{1, 0x80, 0x40},141, "t461"}, /* DWEN */
	{ "tiny85",      T85,   {0x1E, 0x93, 0x0B},   8192,  64,  512,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x01},{1, 0x80, 0x40},102, "t85"},  /* DWEN */
	{ "tiny861",     T861,  {0x1E, 0x93, 0x0D},   8192,  64,  512,  4,  6, 10, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x01},{1, 0x80, 0x40},144, "t861"}, /* DWEN */
//------- ATmega
	{ "mega48",      M48,   {0x1E, 0x92, 0x05},   4096,  64,  256,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x01},{1, 0x80, 0x40}, 76, "m48"},  /* DWEN */
	{ "mega48P",     M48P,  {0x1E, 0x92, 0x0A},   4096,  64,  256,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x01},{1, 0x80, 0x40}, 76, "m48p"}, /* DWEN */
	{ "mega8",       M8,    {0x1E, 0x93, 0x07},   8192,  64,  512,  4,  6, 11, 0xFF, 0xFC, 0x3F, 5, 4, {0xFF, 0xDF}      ,{1, 0x80, 0x00}, 52, "m8"},
	{ "mega8515",    M8515, {0x1E, 0x93, 0x06},   8192,  64,  512,  4,  6, 11, 0xFF, 0xFC, 0x3F, 5, 4, {0xFF, 0xDF}      ,{0, 0x00, 0x00}, 63, "m8515"},
	{ "mega8535",    M8535, {0x1E, 0x93, 0x08},   8192,  64,  512,  4,  6, 11, 0xFF, 0xFC, 0x3F, 5, 4, {0xFF, 0xDF}      ,{0, 0x00, 0x00}, 64, "m8535"},
	{ "mega88",      M88,   {0x1E, 0x93, 0x0A},   8192,  64,  512,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{1, 0x80, 0x40}, 77, "m88"},  /* DWEN */
	{ "mega88P",     M88P,  {0x1E, 0x93, 0x0F},   8192,  64,  512,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{1, 0x80, 0x40}, 77, "m88p"}, /* DWEN */
	{ "mega16",      M16,   {0x1E, 0x94, 0x03},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 0x3F, 5, 4, {0xFF, 0xDF}      ,{0, 0x00, 0x00}, 56, "m16"},
	{ "mega161",     M161,  {0x1E, 0x94, 0x01},  16384, 128,  512,  0, 20,  5, 0xFF, 0xFC, 0x3F, 4, 0, {0x7F}            ,{0, 0x00, 0x00}, 41, "m161"},
	{ "mega162",     M162,  {0x1E, 0x94, 0x04},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x1E},{0, 0x00, 0x00}, 67, "m162"},
	{ "mega163",     M163,  {0x1E, 0x94, 0x02},  16384, 128,  512,  0, 18,  5, 0xFF, 0xFC, 0x3F, 5, 1, {0xEF, 0x07}      ,{0, 0x00, 0x00}, 38, "m163"},
	{ "mega164P",    M164P, {0x1E, 0x94, 0x0A},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00}, 99, "m164p"},
	{ "mega165",     M165,  {0x1E, 0x94, 0x07},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x0E},{0, 0x00, 0x00}, 79, "m165"},
	{ "mega168",     M168,  {0x1E, 0x94, 0x06},  16384, 128,  512,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{1, 0x80, 0x40}, 78, "m168"},  /* DWEN */
	{ "mega168P",    M168P, {0x1E, 0x94, 0x0B},  16384, 128,  512,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{1, 0x80, 0x40}, 78, "m168p"}, /* DWEN */
	{ "mega169",     M169,  {0x1E, 0x94, 0x05},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x0F},{2, 0x01, 0x00}, 73, "m169"},
	{ "mega323",     M323,  {0x1E, 0x95, 0x01},  32768, 128, 1024,  4, 18,  5, 0xFF, 0xFC, 0x3F, 5, 1, {0xCF, 0xEF}      ,{0, 0x00, 0x00}, 50, "m323"},
	{ "mega325/9",   M325,  {0x1E, 0x95, 0x03},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{2, 0x01, 0x00}, 83, "m325"},
	{ "mega3250/90", M3250, {0x1E, 0x95, 0x04},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{2, 0x01, 0x00}, 84, "m3250"},
/**/{ "mega325P",    M325P, {0x1E, 0x95, 0x0D},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{2, 0x01, 0x00}, 83, "m325p"},
/**/{ "mega3250P",   M3250P,{0x1E, 0x95, 0x0E},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{2, 0x01, 0x00}, 84, "m3250p"},
	{ "mega328P",    M328P, {0x1E, 0x95, 0x0F},  32768, 128, 1024,  4,  6,  5, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{1, 0x80, 0x40},  0, "m328p"}, /* DWEN */
	{ "mega32",      M32,   {0x1E, 0x95, 0x02},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 0x3F, 5, 4, {0xFF, 0xDF}      ,{0, 0x00, 0x00}, 69, "m32"},
	{ "mega324P",    M324P, {0x1E, 0x95, 0x08},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00},104, "m324p"},
/**/{ "mega324PA",   M324PA,{0x1E, 0x95, 0x11},  32768, 128, 1024,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00},104, "m324pa"},
	{ "mega603",     M603,  {0x1E, 0x96, 0x01},  65536, 256, 2048,  0, 60, 11, 0xFF, 0xF9, 0xFF, 2, 0, {0x0B}            ,{0, 0x00, 0x00},  0, "m603"},
	{ "mega644",     M644,  {0x1E, 0x96, 0x09},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00},106, "m644"},
	{ "mega644P",    M644P, {0x1E, 0x96, 0x0A},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00},106, "m644p"},
	{ "mega645/9",   M645,  {0x1E, 0x96, 0x03},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{2, 0x01, 0x00}, 87, "m645"},
	{ "mega6450/90", M6450, {0x1E, 0x96, 0x04},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{2, 0x01, 0x00}, 88, "m6450"},
	{ "mega64",      M64,   {0x1E, 0x96, 0x02},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 4, {0xFF, 0xDF, 0x03},{0, 0x00, 0x00}, 58, "m64"},
	{ "mega640",     M640,  {0x1E, 0x96, 0x08},  65536, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00}, 91, "m640"},
	{ "mega103",     M103,  {0x1E, 0x97, 0x01}, 131072, 256, 4096,  0, 60, 11, 0xFF, 0xF9, 0x06, 2, 0, {0x0B}            ,{0, 0x00, 0x00},  7, "m103"},
	{ "mega128",     M128,  {0x1E, 0x97, 0x02}, 131072, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 4, {0xFF, 0xDF, 0x03},{0, 0x00, 0x00}, 54, "m128"},
	{ "mega1280",    M1280, {0x1E, 0x97, 0x03}, 131072, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00}, 92, "m1280"},
	{ "mega1281",    M1281, {0x1E, 0x97, 0x04}, 131072, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00}, 93, "m1281"},
/**/{ "mega1284P",   M1284P,{0x1E, 0x97, 0x05}, 131072, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00},  0, "m1284p"},
	{ "mega2560",    M2560, {0x1E, 0x98, 0x01}, 262144, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00}, 95, "m2560"},
	{ "mega2561",    M2561, {0x1E, 0x98, 0x02}, 262144, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x07},{0, 0x00, 0x00}, 94, "m2561"},
//------- PWM, CAN
	{ "90PWM2/3",    PWM2,  {0x1E, 0x93, 0x81},   8192,  64,  512,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 4, {0xFF, 0xDF, 0xF7},{1, 0x80, 0x40}, 97, "pwm2"},	/* DWEN */
/**/{ "90PWM216/316",PWM216,{0x1E, 0x94, 0x83},  16384, 128,  512,  4,  6, 11, 0xFF, 0xFC, 0x3F, 6, 4, {0xFF, 0xDF, 0xF7},{1, 0x80, 0x40},  0, "pwm3"},	/* DWEN */
	{ "90CAN32",     CAN32, {0x1E, 0x95, 0x81},  32768, 256, 1024,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x0F},{1, 0x80, 0x00}, 82, "c32"},
	{ "90CAN64",     CAN64, {0x1E, 0x96, 0x81},  65536, 256, 2048,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x0F},{1, 0x80, 0x00}, 82, "c64"},
	{ "90CAN128",    CAN128,{0x1E, 0x97, 0x81}, 131072, 256, 4096,  8,  6, 11, 0xFF, 0xFC, 0x3F, 6, 1, {0xFF, 0xDF, 0x0F},{1, 0x80, 0x00}, 82, "c128"},
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
#define MAX_CMD_SIZE 3			/* @@@ by senshu */
char Command[MAX_CMD_SIZE];		/* -r -e -z Read/Erase/Test command (1st,2nd char) */

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

#define OPT_MAX 20
#define LINE_SIZE 512

#if AVRSPX

#if USER_BOOKMARKS
#define BOOKMARK_MAX 100

struct url_list {
	char *key;
	char *url;
	int open;
} user_bookmarks[BOOKMARK_MAX+1];
#endif

bool f_usbasp_list;			/* for USBasp */
bool f_aspxlist;			/* for HIDaspx */
bool f_portlist;			/* COM ports */
bool f_detail_help;
bool f_open_device_url;		/* @@@ by senshu */
bool f_open_atmel_url;		/* @@@ by senshu */
bool f_list_device;			/* @@@ by senshu */
bool f_list_adapter;		/* @@@ by senshu */
bool f_list_bookmark;		/* @@@ by senshu */
bool f_show_opts;			/* @@@ by senshu */
bool f_version;				/* @@@ by senshu */
bool f_report_mode = 1;		/* 0 = original, 1 = avrdude like */
bool f_hex_dump_mode;		/* 0 = Intel HEX, 1 = HEX dump */
bool f_show_spec;			/* @@@ by senshu */

bool f_ISP_DISBL_prog;		/* 1 = RST program enable, @@@ add by senshu */

char *out_filename = NULL;	/* @@@ by senshu */
FILE *redirect_fp;

char *msg_pause_on_start;	/* --pause-on-start=msg */
char *msg_pause_on_exit;	/* --pause-on-exit=msg */
char *new_serial;			// USBaspx

char usb_serial[8];			// Serial number of USBasp and HIDaspx device
char DeviceName[20];		// -d check device type (compared to Device->Name)

#if 1	/* 2010/01/21 10:36:43 */
int hidaspx_type;
#endif

enum {
	OPT_bool,
	OPT_web,
	OPT_str
};

struct opt_list {
	char *option;
	char *long_option;
	bool type;
	void *opt;
	char *help;
} long_opts[] = {
	{"    ",	"pause-on-start",	OPT_str,  &msg_pause_on_start,	"Pause on Port Open"},
	{"    ",	"pause-on-exit",	OPT_str,  &msg_pause_on_exit,	"Pause on exit"},
	{"    ",	"set-serial",		OPT_str,  &new_serial,			"Set USBaspx serial number"},
	{"    ",	"show-options",		OPT_bool, &f_show_opts,			"Show Option Values"},
	{"    ",	"show-spec",		OPT_bool, &f_show_spec,			"Show Setting Values"},
	{"-!  ",	"list-bookmark",	OPT_bool, &f_list_bookmark,		"List user Bookmarks"},
	{"-p? ", 	"list-port",		OPT_bool, &f_portlist,			"List COM Port"},
	{"-pu?",	"list-usbasp",		OPT_bool, &f_usbasp_list,		"List USBasp  Programmer"},
	{"-ph?",	"list-hidaspx",		OPT_bool, &f_aspxlist,			"List HIDaspx Programmer"},
	{"    ",	"list-adapter",		OPT_bool, &f_list_adapter,		"List Support Adapters"},
	{"    ",	"list-device",		OPT_bool, &f_list_device,		"List Support AVR Devices"},
	{"    ",	"new-mode",			OPT_bool, &f_report_mode,		"New progress mode"},
	{"    ",	"avr-devices",	 	OPT_web,  &f_open_device_url,	"AVR Devices list"	},
	{"    ",	"atmel-avr",	 	OPT_web,  &f_open_atmel_url,	"Atmel (AVR 8-Bit RISC)"},
	{"    ",	"version",			OPT_bool, &f_version,			NULL},
	{"-?  ",	"help",				OPT_bool, &f_detail_help,		NULL},
	{NULL, NULL, 0, 0, NULL}
};

BYTE get_fuse_lock_byte (char dst, BYTE val);


/* porgress report by senshu */
static long total_bytes = 0L;
static long total_size = 0L;
static long total_size_kb = 0L;
static double step_size = 0.0;

static int hex_file_is_empty = 0;


char report_msg[128];

/* 2009/01/11 @@@ by senshu */
enum {
	RD_DEV_OPT_f=0,	/* original fuse list */
	RD_DEV_OPT_F,	/* Extented fuse list */
	RD_DEV_OPT_l,	/* Read Fuse list */
	RD_DEV_OPT_i,	/* Web fuse list */
	RD_DEV_OPT_I,	/* Web fuse list (new cgi) */
	RD_DEV_OPT_d	/* Web document view */
};

/* 2009/08/07 by senshu */
enum {	// FUSE INDEX
	LOW=0,
	HIGH,
	EXT,
	LOCK
};

enum {
	ISP_DIS_BYTE=0,
	ISP_DIS_RST,
	ISP_DIS_DWEN
};


#define MAX_BYTES	8192

#define MAX_NUMBERS 50		/* #を出力する数 */
int error_found_count;		/* verify error found */

static int num_cnt;
static int num_save;

#if TIME_DISPLAY
static long total_rw_size = 0L;
static int start_time, job_start_time;
#endif

void report_setup(char *msg, long size)
{
	total_bytes = 0L;
	total_size = size;
	total_size_kb = (size+1023)/1024;

	if (size != 0) {
		step_size = (double)size / (double)MAX_NUMBERS;
	} else {
		step_size = 0.02;
	}

#if TIME_DISPLAY
	total_rw_size += size;
#endif

	strcpy(report_msg, msg);

	if (f_report_mode) {
		MESS(msg);
		MESS(" [");
		num_cnt = 0;
		num_save = 0;
#if TIME_DISPLAY
		start_time = clock();
#endif
		return;
	} else {
		if (total_size <= MAX_BYTES) {
			strcat(report_msg, " %4d/%4d B\r");
		} else {
			strcat(report_msg, " %4d/%4d KB\r");
		}
	}
}

void report_update(int bytes)
{
	if (error_found_count) {
		return;
	}

	if (f_report_mode) {
		int i;
		total_bytes += bytes;
		if (total_bytes <= total_size) {
			num_cnt = (int)((double)total_bytes/ step_size+0.5);

			if (num_cnt <= MAX_NUMBERS) {
				i = num_cnt - num_save;
				while (i--) {
					MESS("#");
					fflush(stderr);	/* for GCC */
				}
				num_save = num_cnt;
			}
		}
	} else {
		if (report_msg) {
			total_bytes += bytes;
			if (total_size <= MAX_BYTES) {
				fprintf(stderr, report_msg, total_bytes, total_size);
			} else {
				fprintf(stderr, report_msg, total_bytes/1024, total_size_kb);
			}
		}
	}
}

void report_finish(int count)
{
	if (f_report_mode) {
		int i;
		if (num_cnt <= MAX_NUMBERS) {
			i = MAX_NUMBERS - num_cnt;
			while (i--) MESS("#");
		}
#if TIME_DISPLAY
		fprintf(stderr, "] %6ld, %6.2lfs\n", total_size, (double)(clock() - start_time) / CLOCKS_PER_SEC);
#else
		fprintf(stderr, "] %6ld\n", total_size);
#endif
	} else {
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
	}
	if (count != 0) {
		fprintf(stderr, "Verify  %d errors.\n", count);
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

void show_version(int flag)
{
#if defined(__GNUC__) && defined(__WIN32__)
		printf("%s (%s) by t.k & senshu, GCC-MinGW %s, %s\n",  progname, VERSION, __VERSION__,  __DATE__);
#elif defined(__WIN32__)
		printf("%s (%s) by t.k & senshu, Borland C++ 5.5.1, %s\n",  progname, VERSION, __DATE__ );
#elif defined(__WIN32__)
		printf("%s (%s) by t.k & senshu for Windows, %s\n",  progname, VERSION, __DATE__ );
#endif
	if (flag){
		printf("----\n");
	}
	fflush(stdout);			// 2009/06/23
}

#if USER_BOOKMARKS
void show_bookmarks()
{
	int i, j;
	/* show user bookmarks */
	i = j =0;
	while (user_bookmarks[i].key) {
		if (j==0) {
			MESS_OUT("\n" "  === user bookmarks ===\n");
			j++;
		}
		printf("  --%-10s = [%s]\n", user_bookmarks[i].key, user_bookmarks[i].url);
		i++;
	}
	fflush(stdout);			// 2009/06/23
}
#endif

void output_usage (bool detail)
{
	int n;
#if AVRSPX
	static const char *const MesUsage[] = {
		"AVRSP - AVR Serial Programming tool R0.44 (C)ChaN, 2008  http://elm-chan.org/\n",
		"Write code and/or data  : <hex file> [<hex file>] ...\n",
		"Verify code and/or data : -v <hex file> [<hex file>] ...\n",
		"Read code, data or fuse : -r{p|e|f|F|l} [-o<output file>]\n",
		"HEX dump (Flash/Eeprom) : -r{p|e}h      [-o<output file>]\n",
        "@  -rp{h}                   Read Program(flash) memory\n",
        "@  -re{h}                   Read Eeprom\n",
        "@  -rf                      Read Fuse (use fuse.txt)\n",
        "@  -rF                      Read Fuse list (HEX style)\n",
        "@  -rl                      Read Fuse and lock bits\n",
		"Get AVR Information(Web): -r{I|i|d}\n",
        "@  -r{i|I}                  Read Fuse Information (new)\n",
        "@  -rd                      Read chip Datasheet\n",
		"Write fuse byte         : -f{l|h|x}<bin>\n",
		"Lock device             : -l[<bin>]\n",
		"Copy calibration bytes  : -c\n",
		"Erase device            : -e\n",
		"Control port [-pb1]     : -p{c|l|v|b|u|h|f}<n>[:<baud>]\n",
		"SPI control delay [-d3] : -d<n>\n",
		"Help (Detail)           : -? or -h or --help\n",

		"@\n",
		"@Miscellaneous Options:\n",
		"@  -q<device>               Check device type\n",
		"@  -t<device>               Force device type\n",
		"@  -w<num>                  Pause before exit\n",
		"@  -z                       1KHz pulse on SCK\n",

		"@\n",
		"@Supported Device:\n",
		"@AT90S 1200,2313,2323,2333,2343,4414,4433,4434,8515,8535\n",
		"@ATtiny 12,13,15,22,24,25,26,44,45,84,85,261,461,861,2313\n",
		"@ATmega 8,16,32,48,48P,64,88,88P,103,128,161,162,163,164P,165,168,168P,\n",
		"@       169,323,324P,324PA,325/9,325P,3250P,328P,3250/90,603,640,644,644P,\n",
		"@       645/9,1280,1281,2560,2561,6450/90,8515,8535\n",
		"@AT90CAN 32,64,128, AT90PWM 2,3,216,316\n",

		"\n",
		"Supported Adapter:\n",
		"AVRSP adapter (COM -pc<n>|-pv<n> / LPT -pl<n>), SPI Bridge (COM -pb<n>[:BAUD]),\n"
		"STK200 ISP dongle, Xilinx JTAG, Lattice isp, Altera ByteBlasterMV (LPT -pl<n>)\n",
		"USBasp(x) (USB -pu<:XXXX>), RSCR (COM -pf<n>),  (<n> == PORT Number)\n",
		"HIDaspx   (USB -ph<:XXXX>), HIDaspx(p) (USB -php<:XXXX>)\n",
		NULL
	};

	show_version(1);		/* @@@ by senshu */
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
	if (detail) {
		int i, j;

		/* show long options @@@ by senshu */
		i = j =0;
		while (long_opts[i].option) {
			if (j==0) {
				MESS_OUT("\n" "  === long options ===\n");
				j++;
			}
			if (long_opts[i].help && (long_opts[i].type == OPT_str)) {
				printf("  --%-14s %-s    %s\n",
					long_opts[i].long_option, long_opts[i].option, long_opts[i].help);
			}
			i++;
		}

		i = 0;
		while (long_opts[i].option) {
			if (long_opts[i].help && (long_opts[i].type == OPT_bool)) {
				printf("  --%-14s %-s    %s\n",
					long_opts[i].long_option, long_opts[i].option, long_opts[i].help);
			}
			i++;
		}

		/* show Web browser options */
		i = j =0;
		while (long_opts[i].option) {
			if (j==0) {
				MESS_OUT("\n" "  === Open URL by Web browser ===\n");
				j++;
			}
			if (long_opts[i].help && long_opts[i].type == OPT_web) {
				printf("  --%-14s %s\n", long_opts[i].long_option, long_opts[i].help);
			}
			i++;
		}
	}
#else
	static const char *const MesUsage[] = {
		"AVRSP - AVR Serial Programming tool R0.43b (C)ChaN, 2008  http://elm-chan.org/\n\n",
		"Write code and/or data  : <hex file> [<hex file>] ...\n",
		"Verify code and/or data : -V <hex file> [<hex file>] ...\n",
		"Read code, data or fuse : -R{P|E|F|i|}\n",
		"Write fuse byte         : -F{L|H|X}<bin>\n",
		"Lock device             : -L[<bin>]\n",
		"Copy calibration bytes  : -C\n",
		"Erase device            : -E\n",
		"Control port [-pc1]     : -P{C|L|V|B}<n>[:<baud>]\n",
		"SPI control delay [-d3] : -D<n>\n\n",
		"Supported Device:\n",
		"AT90S 1200,2313,2323,2333,2343,4414,4433,4434,8515,8535\n",
		"ATtiny 12,13,15,22,24,25,26,44,45,84,85,261,461,861,2313\n",
		"ATmega 8,16,32,48,48P,64,88,88P,103,128,161,162,163,164P,165,168,168P,169,323,\n"
		"       324P,324PA,325/329,325P,3250P,328P,406,603,640,644,644P,645/649,\n"
		"       1280,1281,2560,2561,3250/3290,6450/6490,8515,8535\n",
		"AT90CAN32,64,128, AT90PWM 2,3,216,316\n",
		"Supported Adapter:\n",
		"AVRSP adapter (COM/LPT), SPI Bridge (COM), STK200 ISP dongle (LPT)\n",
		"Xilinx JTAG (LPT), Lattice isp (LPT), Altera ByteBlasterMV (LPT)\n",
		NULL
	};


	for(n = 0; MesUsage[n] != NULL; n++)
		MESS(MesUsage[n]);
#endif
	fflush(stdout);			// 2009/06/23

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

	fflush(stdout);			// 2009/06/23
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

	fflush(stdout);			// 2009/06/23

}

#if AVRSPX
/* Web browser */
void open_url(char *str)
{
	char url[512];

	sprintf(url, "start %s", str);
	if (system(url) >= 0) {
		MESS(url);
		MESS("\n");
	} else {
		MESS("WARNING: Web browser start error.\n");
	}
	return;
}

void open_device_url(int number)
{
	char url[512];

	if (number == 0) {
		sprintf(url, "start http://www.avrfreaks.net/index.php?module=Freaks%%20Devices^&func=viewDev");
	} else {
		sprintf(url, "start http://www.avrfreaks.net/index.php?module=Freaks%%20Devices"
					"^&func=displayDev^&objectid=%d", number);
	}
	if (system(url) >= 0) {
		MESS("start http://www.avrfreaks.net/\n");
	} else {
		MESS("WARNING: Web browser start error.\n");
	}
	return;
}

#endif

/* Output fuse bytes and calibration byte */

void output_fuse (int mode)
{
	int n;
	FILE *fp;
	char Line[100], *cp;


#if AVRSPX		/* @@@ by senshu */
	if (mode==RD_DEV_OPT_d) {
		open_device_url((int)Device->DocNumber);
		return;
	}
	if (mode!=RD_DEV_OPT_l) {
		if( !(mode==RD_DEV_OPT_i || mode==RD_DEV_OPT_I) && Device->FuseType == 0) {
			MESS("Fuse bits are not accessible.\n");
			return;
		}
	}
#else
	if(Device->FuseType == 0) {
		MESS("Fuse bits are not accessible.\n");
		return;
	}
#endif

#if AVRSPX		/* @@@ by senshu */
	if (mode==RD_DEV_OPT_F) {

		printf("\nDEVICE=AT%s\n", Device->Name);
		printf("### %s command line example ###\n", progname);
		printf("%s -q%s -d10 -fL0x%02X", progname, Device->Name, FuseBuff[LOW]);
		if(Device->FuseType >= 5)
			printf(" -fH0x%02X", FuseBuff[HIGH]);
		if(Device->FuseType >= 6)
			printf(" -fX0x%02X", FuseBuff[EXT]);
		printf("\n\n");

/*
 PORT = avrdoper
 PROGRAMMER = stk500v2
 avrdude -c $(PROGRAMMER) -P$(PORT) -p$(DEVICE) -u  -Uflash:w:main.hex:i
 -Uhfuse:w:$(FUSE_H):m -Ulfuse:w:$(FUSE_L):m -Uefuse:w:$(FUSE_X):m
 */
		printf("### avrdude command line example ###\n");
		printf("avrdude -cavrdoper -Pstk500v2  -p%s -u -Uflash:w:main.hex:i \\\n", Device->part_id);
		printf(" -Ulfuse:w:0x%02x:m", FuseBuff[LOW]);
		if(Device->FuseType >= 5)
			printf(" -Uhfuse:w:0x%02x:m", FuseBuff[HIGH]);
		if(Device->FuseType >= 6)
			printf(" -Uefuse:w:0x%02x:m", FuseBuff[EXT] & Device->FuseMask[EXT]);
		printf("\n");
		fflush(stdout);			// 2009/06/23

		return;

	} else if (mode==RD_DEV_OPT_l) {
		char lbuf[4], hbuf[4], xbuf[4], lmask[4], hmask[4], xmask[4];

		if (Device->FuseType != 0) {
			sprintf(lbuf,  "%02X", FuseBuff[LOW]);
			sprintf(lmask, "%02X", Device->FuseMask[LOW]);

			if (Device->FuseType >= 5) {
				sprintf(hbuf,  "%02X", FuseBuff[HIGH]);
				sprintf(hmask, "%02X", Device->FuseMask[HIGH]);
			} else {
				strcpy(hbuf,  "--");
				strcpy(hmask, "--");
			}

			if (Device->FuseType >= 6) {
				sprintf(xbuf,  "%02X", FuseBuff[EXT]);
				sprintf(xmask, "%02X", Device->FuseMask[EXT]);
			} else {
				strcpy(xbuf,  "--");
				strcpy(xmask, "--");
			}
			printf("AT%s %s:%s %s:%s %s:%s %02X\n", Device->Name,
					lbuf, lmask, hbuf, hmask, xbuf, xmask, get_fuse_lock_byte(F_LOCK, 0));
		} else {
			printf("AT%s --:-- --:-- --:-- --\n", Device->Name);
		}
		fflush(stdout);			// 2009/06/23

		return;

	} else if (mode==RD_DEV_OPT_i || mode==RD_DEV_OPT_I) {
		char url[512], *chip;
		int len, r;
/*
2009/03/31現在のサポート一覧

AT86RF401
AT89S51
AT89S52
AT90CAN128
AT90CAN32
AT90CAN64
AT90PWM2
AT90PWM3
AT90S1200
AT90S2313
AT90S2323
AT90S2343
AT90S4414
AT90S4433
AT90S4434
AT90S8515
AT90S8515comp
AT90S8535
AT90S8535comp
ATmega103
ATmega103comp
ATmega128
ATmega1280
ATmega1281
ATmega16
ATmega161
ATmega161comp
ATmega162
ATmega163
ATmega164P
ATmega165
ATmega168
ATmega169
ATmega2560
ATmega2561
ATmega32
ATmega323
ATmega325
ATmega3250
ATmega328P
ATmega329
ATmega3290
ATmega406
ATmega48
ATmega64
ATmega640
ATmega644
ATmega644P
ATmega645
ATmega6450
ATmega649
ATmega6490
ATmega8
ATmega8515
ATmega8535
ATmega88
ATtiny10
ATtiny11
ATtiny12
ATtiny13
ATtiny15
ATtiny167
ATtiny22
ATtiny2313
ATtiny24
ATtiny25
ATtiny26
ATtiny261
ATtiny28
ATtiny44
ATtiny45
ATtiny461
ATtiny48
ATtiny84
ATtiny85
ATtiny861
ATtiny88

 */
		/* 対応していないものは、類似のもので代用する */
#if 0	/* 2009/08/06 M644Pがサポートされた */
		if (Device->ID == M644P) {
			chip = "mega644";
		} else
#endif
		if (Device->ID == M48P) {
			chip = "mega48";
		} else if (Device->ID == M88P) {
			chip = "mega88";
		} else if (Device->ID == M168P) {
			chip = "mega168";
		} else {
			chip = Device->Name;
		}
		len = sprintf(url, "start http://www.engbedded.com/cgi-bin/fc%s.cgi/?P=AT%s^&V_LOW=%02X",
					(mode == RD_DEV_OPT_I)?"x":"", chip, FuseBuff[LOW]);
		if(Device->FuseType >= 5)
			len += sprintf(url+len, "^&V_HIGH=%02X", FuseBuff[HIGH]);
		if(Device->FuseType >= 6)
			len += sprintf(url+len, "^&V_EXTENDED=%02X", FuseBuff[EXT]);

		sprintf(url+len, "^&O_HEX=Apply+user+values");
		r = system(url);	/* Start web browser */

		if (r >= 0) {	/* success */
			sprintf(url, "start http://www.engbedded.com/cgi-bin/fc%s.cgi/\n",
								(mode == RD_DEV_OPT_I)?"x":"");
			MESS(url);
		}
		return;
	}
#endif

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
	put_fuseval(FuseBuff[LOW], Device->FuseMask[LOW], "Low: ", fp);

	if(Device->FuseType >= 5)
		put_fuseval(FuseBuff[HIGH], Device->FuseMask[HIGH], "High:", fp);

	if(Device->FuseType >= 6)
		put_fuseval(FuseBuff[EXT], Device->FuseMask[EXT], "Ext: ", fp);

	/* Output calibration values */
	if(Device->Cals) {
		fputs("Cal:", stdout);
		for(n = 0; n < Device->Cals; n++)
			printf(" %d", CalBuff[n]);
		putchar('\n');
	}
	fflush(stdout);			// 2009/06/23

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
	int char_count;
	const BYTE *p;
	static int segment = 0;

/*

64kBを超える場合には、セグメント情報が付加される。

:020000020000FC
:020000021000EC

この情報を元に、64kBを超えるアドレスを生成する。

 */
	if (f_hex_dump_mode) {
		if (ofs == 0 && count == 2 && type ==2) {
			segment = (buffer[0] << 8) + buffer[1];
			segment *= 16;
			return;
		}
	}
	/* Byte count, Offset address and Record type */
	if (f_hex_dump_mode) {
		if (ofs % 256 == 0) {
			fprintf(fp, "\n");
		}
		fprintf(fp, "%06X  ", segment + ofs);
	} else {
		fprintf(fp, ":%02X%04X%02X", count, ofs, type);
	}
	sum = count + (ofs >> 8) + ofs + type;

	/* Data bytes */
	char_count = count;
	p = buffer;
	while(count--) {
		if (f_hex_dump_mode) {
			if (count == 8) {
				fprintf(fp, "%02X - ", *buffer);
			} else {
				fprintf(fp, "%02X ", *buffer);
			}
		} else {
			fprintf(fp, "%02X", *buffer);
		}
		sum += *buffer++;
	}
	if (f_hex_dump_mode) {
		int ch;
		fprintf(fp, " |");
		while(char_count--) {
			ch = *p++;
			if (!isgraph(ch))
				ch = '.';
			fprintf(fp, "%c", ch);
		}
		fprintf(fp, "|");
	}
	/* Check sum */
	if (f_hex_dump_mode) {
		fprintf(fp, "\n");
	} else {
		fprintf(fp, "%02X\n", (BYTE)-sum);
	}
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
	if (!f_hex_dump_mode) {
		put_hexline(fp, NULL, 0, 0, 1);	/* End block */
	}
}



/*-----------------------------------------------------------------------
  Command line analysis
-----------------------------------------------------------------------*/


int load_commands (int argc, char **argv)
{
	char *cp, c, *cmdlst[OPT_MAX], cmdbuff[LINE_SIZE];
	int cmd;
	FILE *fp;
	DWORD ln = 0;
#if AVRSPX
	char *s;
#if USER_BOOKMARKS
	int url_index = 0;
#endif

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
			if(*cp == ';' || *cp == '#') {
#if USER_BOOKMARKS
				if (strncmp(cp, ";#", 2)==0 && (url_index < BOOKMARK_MAX)) {
					char key[128], url[LINE_SIZE];
					if (sscanf(cp+2, "%s %s", key, url) == 2) {
						user_bookmarks[url_index].key  = strdup(key);
						user_bookmarks[url_index].url  = strdup(url);
						user_bookmarks[url_index].open = 0;
						url_index++;
					}
				}
#endif
				continue; //@@@ by t.k
			}
			cmdlst[cmd++] = cp; cp += strlen(cp) + 1;
            if (*(cp-2) == '\n') *(cp-2) = '\0';    //@@@ by t.k
#else
			cmdlst[cmd++] = cp; cp += strlen(cp) + 1;
#endif
		}
		fclose(fp);
#if USER_BOOKMARKS
		user_bookmarks[url_index].key = NULL;
		user_bookmarks[url_index].url = NULL;
#endif
	}

	/* Get command line parameters */
	while(--argc && (cmd < (sizeof(cmdlst) / sizeof(cmdlst[0]) - 1))) {
		cmdlst[cmd++] = *++argv;
	}
	cmdlst[cmd] = NULL;

#if 1	/* Requst from miyamae , @@@ by senshu */
	/* Show Option Values */
	{
		int i, found, opt_len;
		char * show_options = "--show-options";

		opt_len = strlen(show_options);
		found = 0;
		for (i = 0; cmdlst[i]!= NULL; i++) {
			if (strcmp(cmdlst[i], show_options) == 0) {
				found = 1;
			} else if (strncmp(cmdlst[i], show_options, opt_len) == 0) {
				if (cmdlst[i][opt_len] == '-') {
					found = 0;
				}
			}
		}

		if (found) {
			fprintf(stderr, "#=> %s ", progname);

			/* list options */
			for (i=0; i<cmd; i++){
				if (strncmp(cmdlst[i], show_options, opt_len) != 0) {

					if (strchr(cmdlst[i], ' ') != NULL) {
						fprintf(stderr, "\"%s\" ", cmdlst[i]);
					} else {
						fprintf(stderr, "%s ", cmdlst[i]);
					}
				}
			}
			fprintf(stderr, "\n");
		}
	}
#endif

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

				case 'r' :	/* -r{p|e|f}{h} */
					Command[0] = 'r';
#if AVRSPX	/* -rF, -rl (Read Lock bits) オプション @@@ by senshu*/
					if (*cp) {
						switch (*cp) {
						case 'F':
						case 'L':
						case 'I':
							/* 大文字・小文字を区別するコマンド */
							Command[1] = *cp++;
							break;

						/* -rph, -reh をチェックする */
						case 'p':
						case 'e':
							Command[1] = *cp++;
							if (*cp == 'h') {
								cp++;
								f_hex_dump_mode = 1;
							}
							break;

						default:
							Command[1] = tolower(*cp++);
							break;
						}
					}
#else
					if(*cp) Command[1] = tolower(*cp++);
#endif
					break;

				case 'f' :	/* -f{l|h|x}<bin> */
					c = tolower(*cp++);
					if(*cp <= ' ') {
						return RC_SYNTAX;
					}
					if (c == '#') {	// @@@ by senshu
						f_ISP_DISBL_prog = 1;
						c = tolower(*cp++);
					}
#if AVRSPX
					ln = strtoul_ex(cp, &cp, 0);
#else
					ln = strtoul(cp, &cp, 2);
#endif
					switch (c) {
						case 'l' :
							CmdFuse.Cmd.Flag.Low = 1;
							CmdFuse.Data[LOW] = (BYTE)ln;
							break;
						case 'h' :
							CmdFuse.Cmd.Flag.High = 1;
							CmdFuse.Data[HIGH] = (BYTE)ln;
							break;
						case 'x' :
							CmdFuse.Cmd.Flag.Extend = 1;
							CmdFuse.Data[EXT] = (BYTE)ln;
							break;
						default :
							return RC_SYNTAX;
					}
					break;

				case 'l' :	/* -l[<bin>] */
					CmdFuse.Cmd.Flag.Lock = 1;
#if AVRSPX
					CmdFuse.Data[LOCK] = (BYTE)strtoul_ex(cp, &cp, 0);
#else
					CmdFuse.Data[LOCK] = (BYTE)strtoul(cp, &cp, 2);
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
#if 1	/* 2010/01/21 10:36:43 */
							if (*cp == 'p') {	/* pro mode */
								extern void chg_vid_pid(int type);	/* 2010/02/08 9:03:00 */

							    hidaspx_type = 1;
							    chg_vid_pid(hidaspx_type);
							    ++cp;
							} else {
							    hidaspx_type = 0;
							}
#endif
							{
								char *s;
								s = cp;
								if (s) {
									if (*s == ':') s++;
									if (*cp == '?') {
									    f_aspxlist = true;
									    ++cp;
									} else if (isdigit(*s)) {
										if (s) {
											int n, l;
											l = strlen(s);
											if (l < 4 && isdigit(s[0])) {
												n = atoi(s);
												if ((0 <= n) && (n <= 999)) {
													sprintf(usb_serial, "%04d", n);
												} else {
													if (l == 1) {
														usb_serial[3] = s[0];
													} else if  (l == 2) {
														usb_serial[2] = s[0];
														usb_serial[3] = s[1];
													} else if  (l == 3) {
														usb_serial[1] = s[0];
														usb_serial[2] = s[1];
														usb_serial[3] = s[2];
													}
												}
											} else {
												strncpy(usb_serial, s, 4);
											}
										}
									} else if (*s == '\0'){
										strcpy(usb_serial, "*");		// 省略時は、"*"を選択する
									} else {
										strncpy(usb_serial, s, 4);
									}
									strupr(usb_serial);
									CtrlPort.SerialNumber = usb_serial;
									cp += strlen(cp);
								}
							}
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
							    f_usbasp_list = true;
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

				case 'o' : /* @@@ by senshu */
                    out_filename = cp;
                    cp += strlen(cp);
					break;

                case '-':   /* Parse long options --xxx */
                    {	/* @@ by senshu */
						int i = 0;

						while (long_opts[i].option) {
							int len;

							len = strlen(long_opts[i].long_option);

							if (strncmp(cp, long_opts[i].long_option, len) == 0) {

								switch (long_opts[i].type) {
								case OPT_bool:
								case OPT_web:
									if (cp[len] == '-') {
										*(bool *)long_opts[i].opt = 0;
									} else {
										*(bool *)long_opts[i].opt = 1;
									}
									break;

								case OPT_str:
									{
										if (cp[len] == '=' && cp[len+1]) {
											*(char **)long_opts[i].opt = cp+len+1;
										}
									}
									break;

								default:	/* 上記以外は無視する */
									break;
								}

#if 0
								printf("%s=[%s]\n", cp, *(char **)long_opts[i].opt);
#endif
								break;
							}
							i++;
						}
#if 0	/* for bookmarks */
						if (long_opts[i].option == NULL) {
							fprintf(stderr, "%s: '%s' : Unknown option.\n", progname, cp);
							return RC_OPT_ERR;	/* long option error */
						}
#endif

                    }
#if USER_BOOKMARKS
					{
						int i = 0;
						while (user_bookmarks[i].key) {
							if (strcmp(cp, user_bookmarks[i].key) == 0) {
								user_bookmarks[i].open = 1;
							}
							i++;
						}
					}
#endif
                    cp += strlen(cp);
                    break;

				case '!' :
					f_list_bookmark = true;			/* @@@ by senshu */
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
			char *dot;

			if((fp = fopen(cp, "rt")) == NULL) {
				fprintf(stderr, "%s : Unable to open.\n", cp);
				return RC_FILE;
			}
			dot = strrchr(cp, '.');
			/* .eep files are read as EEPROM data, others are read as program code */
			if(*dot) {
				if (strcasecmp(dot, ".EEP") == 0) {
					ln = input_hexfile(fp, DataBuff, sizeof(DataBuff), &CmdWrite.DataSize);
					if(CmdWrite.DataSize==0) {
						hex_file_is_empty++;
						fprintf(stderr, "%s : HEX File is empty.\n", cp);
					}
				} else {
					ln = input_hexfile(fp, CodeBuff, sizeof(CodeBuff), &CmdWrite.CodeSize);
					if(CmdWrite.CodeSize==0) {
						hex_file_is_empty++;
						fprintf(stderr, "%s : HEX File is empty.\n", cp);
					}
				}
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
		if (hidasp_program_enable(CtrlPort.Delay) != 0) {
			MESS("Device connection failed.(PE)\n");
			return 1;
		} else {
			return 0;
		}
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
		FuseBuff[LOW] = spi_transmit_R(
			C_RD_FLB1,			/* Type 1 : Read fuse low */
			0,
			0,
			RM_SYNC);
#else
		spi_xmit(C_RD_FLB1);			/* Type 1 : Read fuse low */
		spi_xmit(0);
		spi_xmit(0);
		FuseBuff[LOW] = spi_rcvr(RM_SYNC);
#endif
	}
	else {
#if AVRSPX
		FuseBuff[LOW] = spi_transmit_R(
			C_RD_FB1,				/* Type 2..6 : Read fuse low */
			0,
			0,
			RM_SYNC);
#else
		spi_xmit(C_RD_FB1);				/* Type 2..6 : Read fuse low */
		spi_xmit(0);
		spi_xmit(0);
		FuseBuff[LOW] = spi_rcvr(RM_SYNC);
#endif

		if(Device->FuseType >= 5) {
#if AVRSPX
			FuseBuff[HIGH] = spi_transmit_R(
				C_RD_FLB1,		/* Type 5..6 : Read fuse high */
				0x08,
				0,
				RM_SYNC);
#else
			spi_xmit(C_RD_FLB1);		/* Type 5..6 : Read fuse high */
			spi_xmit(0x08);
			spi_xmit(0);
			FuseBuff[HIGH] = spi_rcvr(RM_SYNC);
#endif

			if(Device->FuseType >= 6) {
#if AVRSPX
			FuseBuff[EXT] = spi_transmit_R(
				C_RD_FB1,		/* Type 6 : Read fuse extend */
				0x08,
				0,
				RM_SYNC);
#else
				spi_xmit(C_RD_FB1);		/* Type 6 : Read fuse extend */
				spi_xmit(0x08);
				spi_xmit(0);
				FuseBuff[EXT] = spi_rcvr(RM_SYNC);
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
BYTE get_fuse_lock_byte (char dst,		/* which fuse to be written */
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
			MESS("Flash Memory...\n");
			report_setup("Reading  ", Device->FlashSize);
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
			report_finish(0);
#else
			MESS("Reading Flash...");
			for(adr = 0; adr < Device->FlashSize; adr += PIPE_WINDOW)
				read_multi(FLASH, adr, PIPE_WINDOW, &CodeBuff[adr]);
#endif
			MESS("Passed.\n");
			if (f_hex_dump_mode) {
				output_hexfile(stdout, CodeBuff, Device->FlashSize, 16);
			} else {
				output_hexfile(stdout, CodeBuff, Device->FlashSize, 32);
			}
			break;

		case 'e' :	/* -re : read eeprom */
#if AVRSPX
			MESS("EEPROM...\n");
			report_setup("Reading  ", Device->EepromSize);
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
			report_finish(0);
#else
			MESS("Reading EEPROM...");
			ws = (Device->EepromSize < PIPE_WINDOW) ? Device->EepromSize : PIPE_WINDOW;
			for(adr = 0; adr < Device->EepromSize; adr += ws)
				read_multi(EEPROM, adr, ws, &DataBuff[adr]);
#endif
			MESS("Passed.\n");
			if (f_hex_dump_mode) {
				output_hexfile(stdout, DataBuff, Device->EepromSize, 16);
			} else {
				output_hexfile(stdout, DataBuff, Device->EepromSize, 32);
			}
			break;

		case 'f' :	/* -rf : read fuses */
			read_fuse();
			output_fuse(RD_DEV_OPT_f);
			break;

#if AVRSPX
		case 'F' :	/* -rF : read fuses @@@ by senshu */
			read_fuse();
			output_fuse(RD_DEV_OPT_F);
			break;

		case 'l' :	/* -rl : read fuse list @@@ by senshu */
			read_fuse();
			output_fuse(RD_DEV_OPT_l);
			break;

		case 'i' :	/* -ri : read fuses @@@ by senshu */
			read_fuse();
			output_fuse(RD_DEV_OPT_I);
			break;

		case 'I' :	/* -rI : read fuses @@@ by senshu */
			read_fuse();
			output_fuse(RD_DEV_OPT_I);
			break;

		case 'd' :	/* -rd : read chip datasheet docment @@@ by senshu */
			output_fuse(RD_DEV_OPT_d);
			break;
#endif

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
		MESS("Flash memory...\n");				/* Erase device before programming */
		report_setup("Writing  ", CmdWrite.CodeSize);
		if (CtrlPort.PortClass == TY_USBASP) {
			rc = usbasp_paged_write(FLASH, CodeBuff, Device->FlashPage,
							CmdWrite.CodeSize, Device->FlashSize > (128*1024));
			if (rc != CmdWrite.CodeSize) {
				report_finish(0);
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
						report_finish(0);
						fprintf(stderr, "Time out at %04X\n", (int)adr);
						return RC_FAIL;
					}
					report_update(1);
				}
			}
			spi_flush();
		}
		report_finish(0);
#else	// @@@
		MESS("Writing...");
		if(Device->FlashPage) {		/* Write flash in page mode */
			for(adr = 0; adr < CmdWrite.CodeSize; adr += Device->FlashPage) {
				write_page(adr, &CodeBuff[adr]);
			}
		} else {						/* Write flash in byte-by-byte mode */
			for(adr = 0; adr < CmdWrite.CodeSize; adr++) {
				if(write_byte(FLASH, adr, CodeBuff[adr])) {
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
		report_setup("Verifying", CmdWrite.CodeSize);

		error_found_count = 0;
		if (CtrlPort.PortClass == TY_USBASP) {
			rc = usbasp_paged_verify(FLASH, CodeBuff, Device->FlashPage,
							CmdWrite.CodeSize, Device->FlashSize > (128*1024));
			if (rc != 0) {
				error_found_count++;
				goto verify_finish;
			}
		} else if (CtrlPort.PortClass == TY_HIDASP) {
			hidasp_page_read(0, rd, 0);
			for(adr = 0; adr < CmdWrite.CodeSize; adr += PIPE_WINDOW) {
				hidasp_page_read(-1, rd, PIPE_WINDOW);
				for(n = 0; n < PIPE_WINDOW; n++) {
					if(rd[n] != CodeBuff[adr+n]) {
						if (error_found_count == 0) {
							fprintf(stderr, "\n");
						}
						fprintf(stderr, "Failed at %04X:%02X->%02X\n", (int)(adr+n), (int)CodeBuff[adr+n], (int)rd[n]);
							error_found_count++;
					}
				}
			}
		} else {
			for(adr = 0; adr < CmdWrite.CodeSize; adr += PIPE_WINDOW) {
				read_multi(FLASH, adr, PIPE_WINDOW, rd);
				for(n = 0; n < PIPE_WINDOW; n++) {
					if(rd[n] != CodeBuff[adr+n]) {
						if (error_found_count == 0) {
							fprintf(stderr, "\n");
						}
						fprintf(stderr, "Failed at %04X:%02X->%02X\n", (int)(adr+n), (int)CodeBuff[adr+n], (int)rd[n]);
						error_found_count++;
					}
				}
				report_update(PIPE_WINDOW);
			}
		}

	verify_finish:
		if (error_found_count) {
			report_finish(error_found_count);
			return RC_FAIL;
		}
		report_finish(0);
#else
		MESS("Verifying...");
		if (CtrlPort.PortClass == TY_USBASP) {
			rc = usbasp_paged_verify(FLASH, CodeBuff, Device->FlashPage,
							CmdWrite.CodeSize, Device->FlashSize > (128*1024));
			if (rc != 0) {
				report_finish(0);
				return RC_FAIL;
			}
		} else {
			for(adr = 0; adr < CmdWrite.CodeSize; adr += PIPE_WINDOW) {
				read_multi(FLASH, adr, PIPE_WINDOW, rd);
				for(n = 0; n < PIPE_WINDOW; n++) {
					if(rd[n] != CodeBuff[adr+n]) {
						report_finish(0);
						fprintf(stderr, "Failed at %04X:%02X->%02X\n", adr+n, CodeBuff[adr+n], rd[n]);
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
	MESS("EEPROM...\n");
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
		report_setup("Writing  ", CmdWrite.DataSize);
		if (CtrlPort.PortClass == TY_USBASP) {		/* Write EEPROM in page mode */
			rc = usbasp_paged_write(EEPROM, DataBuff, Device->EepromPage, CmdWrite.DataSize, false);
			if (rc != CmdWrite.DataSize) {
				report_finish(0);
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
		report_finish(0);
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
		report_setup("Verifying", CmdWrite.DataSize);
		error_found_count = 0;
		if (CtrlPort.PortClass == TY_USBASP) {		/* Read eeprom in page mode */
			rc = usbasp_paged_verify(EEPROM, DataBuff, Device->EepromPage ,
							CmdWrite.DataSize, false);
			if (rc != 0) {
				error_found_count++;
			}
		} else {
			ws = (Device->EepromSize < PIPE_WINDOW) ? Device->EepromSize : PIPE_WINDOW;
			for(adr = 0; adr < CmdWrite.DataSize; adr += ws) {
				read_multi(EEPROM, adr, ws, rd);
				for(n = 0; n < ws; n++) {
					if(rd[n] != DataBuff[adr+n]) {
						if (error_found_count == 0) {
							fprintf(stderr, "\n");
						}
						fprintf(stderr, "Failed at %04X:%02X->%02X\n", (int)(adr+n), (int)DataBuff[adr+n], (int)rd[n]);
						error_found_count++;
					}
				}
				report_update(ws);
			}
		}
		if (error_found_count) {
			report_finish(error_found_count);
			return RC_FAIL;
		}
		report_finish(0);
#else
		MESS("Verifying...");
		ws = (Device->EepromSize < PIPE_WINDOW) ? Device->EepromSize : PIPE_WINDOW;
		for(adr = 0; adr < CmdWrite.DataSize; adr += ws) {
			read_multi(EEPROM, adr, ws, rd);
			for(n = 0; n < ws; n++) {
				if(rd[n] != DataBuff[adr+n]) {
					fprintf(stderr, "Failed at %04X:%02X->%02X\n", adr+n, DataBuff[adr+n], rd[n]);
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
		if (f_ISP_DISBL_prog == 0 && (Device->ISP_DISBL[ISP_DIS_BYTE] == 0 && Device->ISP_DISBL[ISP_DIS_RST] != 0)) {
			if ((CmdFuse.Data[LOW] & Device->ISP_DISBL[ISP_DIS_RST]) == 0) {
				fprintf(stderr, "WARNING: ISP disable FUSE bit (RSTDISBL) detected.\n"
								"If you hope for the writing, Enter the -f#L0x%02x option.\n", CmdFuse.Data[LOW]);
				return RC_DEV;
			}
			vfuse = fuse = (CmdFuse.Data[LOW] & Device->FuseMask[LOW]) | Device->ISP_DISBL[ISP_DIS_RST];
		} else {
			vfuse = fuse = CmdFuse.Data[LOW] & Device->FuseMask[LOW];
		}

		/* -v : Skip programming process when verify mode */
		if (CmdWrite.Verify != 1)
			write_fuselock(F_LOW, fuse | ~Device->FuseMask[LOW]);

		if (Device->FuseType < 3) {	/* Type 1..2 : No check Lock Byte */
			fprintf(stderr, "Fuse Low byte was programmed (0x%02X).\n", fuse);
		} else {
			/* -v- : Skip verifying process when programming only mode */
			if (CmdWrite.Verify != 2)
				vfuse = get_fuse_lock_byte(F_LOW, fuse) & Device->FuseMask[LOW];

			if (vfuse != fuse)
				fprintf(stderr, "Fuse Low byte was programm error. (%02X -> %02X)\n", fuse, vfuse);
			else
				fprintf(stderr, "Fuse Low byte was programmed (0x%02X).\n", fuse);
		}
#else
		write_fuselock(F_LOW, (BYTE)(CmdFuse.Data[LOW] | ~Device->FuseMask[LOW]));
		MESS("Fuse Low byte was programmed.\n");
#endif
	}

	if(CmdFuse.Cmd.Flag.High && (Device->FuseType >= 5)) {
#if AVRSPX
		if (f_ISP_DISBL_prog == 0 && (Device->ISP_DISBL[ISP_DIS_BYTE] == 1 && Device->ISP_DISBL[ISP_DIS_RST] != 0)) {
			if ((CmdFuse.Data[HIGH] & Device->ISP_DISBL[ISP_DIS_RST]) == 0) {
				fprintf(stderr, "WARNING: ISP disable FUSE bit (RSTDISBL) detected.\n"
								"If you hope for the writing, Enter the -f#H0x%02x option.\n",
								CmdFuse.Data[HIGH] | Device->ISP_DISBL[ISP_DIS_DWEN]);
				return RC_DEV;
			}
		}
		vfuse = fuse = CmdFuse.Data[HIGH] & Device->FuseMask[HIGH];
		if ((fuse & Device->ISP_DISBL[ISP_DIS_DWEN]) == 0) {
			fprintf(stderr, "WARNING: ISP disable FUSE bit (DWEN) detected, Unprogrammed DWEN bit.\n");
			vfuse = fuse = (fuse | Device->ISP_DISBL[ISP_DIS_DWEN]);
		}

		/* -v : Skip programming process when verify mode */
		if (CmdWrite.Verify != 1) {
			write_fuselock(F_HIGH, (fuse | ~Device->FuseMask[HIGH]));
		}
		/* -v- : Skip verifying process when programming only mode */
		if (CmdWrite.Verify != 2)
			vfuse = get_fuse_lock_byte(F_HIGH, fuse) & Device->FuseMask[HIGH];

		if (vfuse != fuse)
			fprintf(stderr, "Fuse High byte was programm error. (%02X -> %02X)\n", fuse, vfuse);
		else
			fprintf(stderr, "Fuse High byte was programmed (0x%02X).\n", fuse);
#else
		write_fuselock(F_HIGH, (BYTE)(CmdFuse.Data[HIGH] | ~Device->FuseMask[HIGH]));
		MESS("Fuse High byte was programmed.\n");
#endif
	}

	if(CmdFuse.Cmd.Flag.Extend && (Device->FuseType >= 6)) {
#if AVRSPX
		if (f_ISP_DISBL_prog == 0 && (Device->ISP_DISBL[ISP_DIS_BYTE] == 2 && Device->ISP_DISBL[ISP_DIS_RST] != 0)) {
			if ((CmdFuse.Data[EXT] & Device->ISP_DISBL[ISP_DIS_RST]) == 0) {
				fprintf(stderr, "WARNING: ISP disable FUSE bit (RSTDISBL) detected.\n"
								"If you hope for the writing, Enter the -f#X0x%02x option.\n", CmdFuse.Data[EXT]);
				return RC_DEV;
			}
			vfuse = fuse = (CmdFuse.Data[EXT] & Device->FuseMask[EXT]) | Device->ISP_DISBL[ISP_DIS_RST];
		} else {
			vfuse = fuse = CmdFuse.Data[EXT] & Device->FuseMask[EXT];
		}

		/* -v : Skip programming process when verify mode */
		if (CmdWrite.Verify != 1)
			write_fuselock(F_EXTEND, fuse | ~Device->FuseMask[EXT]);

		/* -v- : Skip verifying process when programming only mode */
		if (CmdWrite.Verify != 2)
			vfuse = get_fuse_lock_byte(F_EXTEND, fuse) & Device->FuseMask[EXT];

		if (vfuse != fuse)
			fprintf(stderr, "Fuse Extend byte was programm error. (%02X -> %02X)\n",fuse,vfuse);
		else
			fprintf(stderr, "Fuse Extend byte was programmed (0x%02X).\n",fuse);
#else
		write_fuselock(F_EXTEND, (BYTE)(CmdFuse.Data[EXT] | ~Device->FuseMask[EXT]));
		MESS("Fuse Extend byte was programmed.\n");
#endif
	}

	if(CmdFuse.Cmd.Flag.Lock) {
#if AVRSPX
		fuse  = CmdFuse.Data[LOCK] ? CmdFuse.Data[LOCK] : Device->LockData;
		vfuse = fuse & Device->LockMask;

		/* -v : Skip programming process when verify mode */
		if (CmdWrite.Verify != 1)
			write_fuselock(F_LOCK, fuse | ~Device->LockMask);

		/* -v- : Skip verifying process when programming only mode */
		if (CmdWrite.Verify != 2)
			vfuse = get_fuse_lock_byte(F_LOCK, fuse) & Device->LockMask;

		if (vfuse != (fuse & Device->LockMask))
			fprintf(stderr, "Lock bits programm error. (%02X -> %02X).\n", fuse, vfuse);
		else
			fprintf(stderr, "Lock bits are programmed (0x%02X).\n", fuse);
#else
		write_fuselock(F_LOCK, (BYTE)(CmdFuse.Data[LOCK] ? CmdFuse.Data[LOCK] : Device->LockData));
		MESS("Lock bits are programmed.\n");
#endif
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
	int i;
	f_terminate = true;
#endif

	close_ifport();
	Device = NULL;

#if AVRSPX
	/* close output file */
	if (redirect_fp) {
		fclose(redirect_fp);
	}

#if USER_BOOKMARKS
	/* release User Bookmarks info. */
	i = 0;
	while (user_bookmarks[i].key) {
		if (user_bookmarks[i].key)	free(user_bookmarks[i].key);
		if (user_bookmarks[i].url)	free(user_bookmarks[i].url);
		i++;
	}
#endif

#if TIME_DISPLAY
	{	double t;
		t = (double)(clock() - job_start_time) / CLOCKS_PER_SEC;
		if (t <= 0.1) t = 0.1;
		if (total_rw_size != 0) {
			fprintf(stderr, "Total read/write size = %ld B /%5.2lf s (%4.2lf kB/s)\n",
				total_rw_size, t, total_rw_size/(t*1024.0));
		}
	}
#endif

	if (msg_pause_on_exit && msg_pause_on_exit[0]) {
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
#if 0	// 2009/06/23 hidspxG で kbhit()を呼び出すと、Access Violation error を発生する
			if (kbhit()) {
				getchar();
				break;
			}
#endif
		} while (i--);
	}
#else	 // !AVRSPX
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
#define LINE_BUFF_SIZE 512
static char out_buff[LINE_BUFF_SIZE];

int main (int argc, char **argv)
{
	int rc = 0;

#if AVRSPX
#if TIME_DISPLAY
	total_rw_size = 0;
	job_start_time = clock();
#endif
	atexit(do_exit);

	rc = load_commands(argc, argv);
	if(rc != 0) {
		if(rc == RC_SYNTAX) {
#if 0
			output_usage(true);
#else
			fprintf(stderr, "%s: Option errror, Please add '--show-options' .\n", progname);
#endif
		}
		terminate(rc);
		return rc;
	}

	if (isatty (fileno(stderr))) {
		/* stderr のバッファリングを行わない */
		setvbuf( stderr, NULL, _IONBF, 0 );
	}

    if (out_filename) {	// @@@ by senshu
		/* stdout を out_filename に変更 */
		redirect_fp = freopen(out_filename, "wt", stdout);
		if (redirect_fp == NULL) {
			MESS("-ofilename Redirect error\n");
			return RC_FILE;
		}
	} else {
		setvbuf( stdout, out_buff, _IOLBF, sizeof(out_buff) );
	}


	if (f_open_device_url) {	// @@@ by senshu
		open_device_url(0);
		terminate(rc = 0);
		return rc;
    }

	if (f_open_atmel_url) {	// @@@ by senshu
		open_url("http://www.atmel.com/products/AVR/");
		terminate(rc = 0);
		return rc;
    }

	if (f_list_device) {	// @@@ by senshu
		int i = 0;
		char name[32], *p;

		while (strcmp(DevLst[i].Name, "Locked") !=0 ) {
			strcpy(name, DevLst[i].Name);
			p = strchr(name , '/');
			if (p) *p = '\0';
			printf("%s\n", name);
			i++;
		}
		fflush(stdout);			// 2009/06/23
		terminate(rc = 0);
		return rc;
    }
#if USER_BOOKMARKS		/* @@@ by senshu */
	if (f_list_bookmark) {
		show_bookmarks();
		terminate(rc = 0);
        return rc;

	} else {
		int i , j;

		i = j = 0;
		while (user_bookmarks[i].key) {
			if (user_bookmarks[i].open) {
				open_url(user_bookmarks[i].url);
				j++;
			}
			i++;
		}
		if (j) {
			terminate(rc = 0);
			return rc;
		}
	}
#endif

    if (f_version) {		/* @@@ by senshu */
		show_version(0);
		terminate(rc = 0);
		return rc;
    }

    if (f_list_adapter) {		/* @@@ by senshu */
   		MESS_OUT(
   		" -pc<n>            \"Direct COM Port Access (giveio.sys)\"\n"
		" -pl<n>            \"Direct LPT Port Access (giveio.sys)\"\n"
		" -pv<n>            \"COM Port access (Win32API)\"\n"
		" -pb<n>            \"SPI-Bridge (COM Port)\"\n"
		" -pu<:XXXX>        \"USBasp/USBaspx\"\n"
		" -ph<:XXXX>        \"HIDaspx\"\n"
		" -php<:XXXX>       \"HIDaspx(pro)\"\n"
		" -pf<n>            \"COM Port Access (chicken & egg)\"\n"
		" -ph?              \"List HIDaspx Programmers\"\n"
		" -pu?              \"List USBasp  Programmers\"\n"
		" -p?               \"Dump COM Port List.\"\n"
		);
		terminate(rc = 0);
		return rc;
    }

    if (f_detail_help) {
		output_usage(true);
		terminate(rc = 0);
		return rc;
    }

	if (f_aspxlist) {	/* @@@ by senshu */
        if (hidasp_list(progname) < 1)
            return 1;
		terminate(rc = 0);
        return rc;
	}

	if (f_usbasp_list) {
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

    if (new_serial && new_serial[0]) {
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

	/* Programmer start @@ by senshu */
#if AVRSPX
	if (msg_pause_on_start && msg_pause_on_start[0]) {
		MESS("\n");
		MESS(msg_pause_on_start);
		getchar();
	}
#endif

	/* Read device and terminate if -R{P|E|F} command is specified */
	if(Command[0] == 'r') {
		rc = read_device(Command[1]);
#if 0
		fprintf(stderr, "hidspx (rc = %d)\n", rc);
#endif
		terminate(rc);
		return rc;
	}

	/* Erase device and terminate if -E command is specified */
	if(Command[0] == 'e') {
		rc = erase_device();
#if 1	/* 2009/09/25 */
		if(rc != 0) {
			terminate(rc);
			return rc;
		}
#else
		terminate(rc);
#endif
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

#if AVRSPX
	if (Device == NULL)
	{
		rc = RC_SYNTAX;
		if (!hex_file_is_empty && !(argc > 1)) {
			output_usage(false);
		}
		if (f_show_spec) {
			extern char *progpathname, progpath[];
			char * p, ini_file_path[MAX_PATH];
			FILE *fp;

			if (strchr(progpathname, ':') != NULL) {
				fprintf(stderr, "program  '%s'\n", progpathname);
			} else {
				fprintf(stderr, "program  '%s%s'\n", progpath, progpathname);
			}

			sprintf(ini_file_path, "%s%s", progpath, INIFILE);
			fp = fopen(ini_file_path, "rt");
			if (fp != NULL) {
				fclose(fp);
				fprintf(stderr, "ini file '%s'\n", ini_file_path);
			} else {
				fprintf(stderr, "ini file '%s' doesn't exist.\n", ini_file_path);
			}

			init_devices ();

			switch (CtrlPort.PortClass) {
				case TY_LPT:		p = "LPT"; break;
				case TY_COMM:		p = "COMM"; break;
				case TY_VCOM:		p = "VCOM"; break;
				case TY_BRIDGE:		p = "COM-SPI bridge"; break;
				case TY_AVRSP:		p = "COM(AVRSP)"; break;
				case TY_STK200:		p = "LPT(STK200)"; break;
				case TY_XILINX:		p = "LPT(XILINX)"; break;
				case TY_LATTICE:	p = "LPT(LATTICE)"; break;
				case TY_ALTERA:		p = "LPT(ALTRA)"; break;
				case TY_RSCR:		p = "COM(RSCR)"; break;
				case TY_USBASP:		p = "USBasp"; break;
				case TY_HIDASP:		p = "HIDaspx"; break;
				default:			p = "Unknown"; break;
			}
			fprintf(stderr, "Type = %s, Delay = %d\n", p, CtrlPort.Delay);
		}
	}
#else
	if(Device == NULL)
	{
		rc = RC_SYNTAX;
		output_usage(false);
	}
#endif
	terminate(rc);
	return 0;
}

