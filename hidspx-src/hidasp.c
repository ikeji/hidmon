/* hidasp.c
 */

#define DEBUG 		   0
#define PKTDUMP		   0		// HID Reportパケットをダンプする.
#define PRODUCT_DUMP   0		// 製造者,製品名を表示.
#define	TRANSFER_BENCH 0		// 転送速度ベンチ.

#include <windows.h>
#include <stdio.h>
#include "usbhid.h"
#include "hidasp.h"

#pragma comment(lib, "setupapi.lib")

//  obdev
#define MY_VID 0x16c0			/* 5824 in dec, stands for VOTI */
#define MY_PID 0x05dc			/* 1500 in dec, obdev's free PID */

#define	MY_Manufacturer	"YCIT"
#define	MY_Product		"HIDaspx"


// HID API (from w2k DDK)
_HidD_GetAttributes HidD_GetAttributes;
_HidD_GetHidGuid HidD_GetHidGuid;
_HidD_GetPreparsedData HidD_GetPreparsedData;
_HidD_FreePreparsedData HidD_FreePreparsedData;
_HidP_GetCaps HidP_GetCaps;
_HidP_GetValueCaps HidP_GetValueCaps;

//
_HidD_GetFeature HidD_GetFeature;
_HidD_SetFeature HidD_SetFeature;
_HidD_GetManufacturerString HidD_GetManufacturerString;
_HidD_GetProductString HidD_GetProductString;

HINSTANCE hHID_DLL = NULL;		// hid.dll handle
HANDLE hHID = NULL;				// USB-IO dev handle
HIDP_CAPS Caps;


//  HID Report のパケットはサイズ毎に6種類用意されている.
#define	REPORT_ID1			1	// 8  REPORT_COUNT(6)
#define	REPORT_ID2			2	// 16 REPORT_COUNT(14)
#define	REPORT_ID3			3	// 24 REPORT_COUNT(22)
#define	REPORT_ID4			4	// 32 REPORT_COUNT(30)
#define	REPORT_ID5			5	// 40 REPORT_COUNT(38)
#define	REPORT_ID6			6	// 48 REPORT_COUNT(46)

//
// Report_IDから、サイズを得るためのテーブル.
//                          ID    1  2  3  4  5  6
static char report_size_tab[7] = { 0, 7, 15, 23, 31, 39, 47 };

//  デフォルトのパケットサイズは、39 (ReportID=5) を使用する.
#define	REPORT_ID		REPORT_ID5	// 省略値.
#define	REPORT_LENGTH_OVERRIDE	39	// REPORT_COUNT+1の値.

static int hidWrite(HANDLE hHID, char *buf, int Length, ULONG * sz,
					int id);
static int hidRead(HANDLE hHID, char *buf, int Length, ULONG * sz, int id);

////////////////////////////////////////////////////////////////////////
//             hid.dll をロード
static int LoadHidDLL()
{
	hHID_DLL = LoadLibrary("hid.dll");
	if (!hHID_DLL) {
#if 1
		fprintf(stderr, "Error at Load 'hid.dll'\n");
#else
		MessageBox(NULL, "Error at Load 'hid.dll'", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetAttributes =
		(_HidD_GetAttributes) GetProcAddress(hHID_DLL,
											 "HidD_GetAttributes");
	if (!HidD_GetAttributes) {
#if 1
		fprintf(stderr, "Error at HidD_GetAttributes\n");
#else
		MessageBox(NULL, "Error at HidD_GetAttributes", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetHidGuid =
		(_HidD_GetHidGuid) GetProcAddress(hHID_DLL, "HidD_GetHidGuid");
	if (!HidD_GetHidGuid) {
#if 1
		fprintf(stderr, "Error at HidD_GetHidGuid\n");
#else
		MessageBox(NULL, "Error at HidD_GetHidGuid", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetPreparsedData =
		(_HidD_GetPreparsedData) GetProcAddress(hHID_DLL,
												"HidD_GetPreparsedData");
	HidD_FreePreparsedData =
		(_HidD_FreePreparsedData) GetProcAddress(hHID_DLL,
												 "HidD_FreePreparsedData");
	HidP_GetCaps =
		(_HidP_GetCaps) GetProcAddress(hHID_DLL, "HidP_GetCaps");
	HidP_GetValueCaps =
		(_HidP_GetValueCaps) GetProcAddress(hHID_DLL, "HidP_GetValueCaps");

//
	HidD_GetFeature =
		(_HidD_GetFeature) GetProcAddress(hHID_DLL, "HidD_GetFeature");
	HidD_SetFeature =
		(_HidD_SetFeature) GetProcAddress(hHID_DLL, "HidD_SetFeature");
	HidD_GetManufacturerString = (_HidD_GetManufacturerString)
		GetProcAddress(hHID_DLL, "HidD_GetManufacturerString");
	HidD_GetProductString = (_HidD_GetProductString)
		GetProcAddress(hHID_DLL, "HidD_GetProductString");

#if	DEBUG
	printf("_HidD_GetFeature= %x\n", (int) HidD_GetFeature);
	printf("_HidD_SetFeature= %x\n", (int) HidD_SetFeature);
#endif
	return 1;
}

////////////////////////////////////////////////////////////////////////
// ディバイスの情報を取得
static void GetDevCaps()
{
	PHIDP_PREPARSED_DATA PreparsedData;
	HIDP_VALUE_CAPS *VCaps;
	char buf[1024];

	VCaps = (HIDP_VALUE_CAPS *) (&buf);

	HidD_GetPreparsedData(hHID, &PreparsedData);
	HidP_GetCaps(PreparsedData, &Caps);
	HidP_GetValueCaps(HidP_Input, VCaps, &Caps.NumberInputValueCaps,
					  PreparsedData);
	HidD_FreePreparsedData(PreparsedData);

#if DEBUG
	fprintf(stderr, " Caps.OutputReportByteLength = %d\n",
			Caps.OutputReportByteLength);
	fprintf(stderr, " Caps.InputReportByteLength = %d\n",
			Caps.InputReportByteLength);
#endif


#if	REPORT_LENGTH_OVERRIDE
	//複数のREPORT_COUNTが存在するときは下記Capsが 0 のままなので、上書きする.
	Caps.OutputReportByteLength = REPORT_LENGTH_OVERRIDE;
	Caps.InputReportByteLength = REPORT_LENGTH_OVERRIDE;
#endif
}

static char *uni_to_string(char *t, unsigned short *u)
{
	char *buf = t;
	int c;
	// short を char 配列に入れなおす.
	while (1) {
		c = *u++;
		if (c == 0)
			break;
		if (c & 0xff00)
			c = '?';
		*t++ = c;
	}

	*t = 0;
	return buf;
}

//
//  名前チェック : 成功=1  失敗=0 読み取り不能=(-1)
//
static int check_product_string(HANDLE handle)
{
	int len1, len2;
	unsigned short unicode[512];
	char string1[256];
	char string2[256];

	if (!HidD_GetManufacturerString(handle, unicode, sizeof(unicode))) {
		return -1;
	}
	uni_to_string(string1, unicode);

	if (!HidD_GetProductString(handle, unicode, sizeof(unicode))) {
		return -1;
	}
	uni_to_string(string2, unicode);

#if	PRODUCT_DUMP
	fprintf(stderr, "iManufacturer:%s\n", string1);
	fprintf(stderr, "iProduct:%s\n", string2);
#endif

#ifdef	MY_Manufacturer
	if (strcmp(string1, MY_Manufacturer) != 0)
		return 0;
#endif

#ifdef	MY_Product
	if (strcmp(string2, MY_Product) != 0)
		return 0;
#endif

	return 1;					//合致した.
}

////////////////////////////////////////////////////////////////////////
// HIDディバイス一覧からUSB-IOを検索
static int OpenTheHid()
{
	int f = 0;
	int i = 0;
	ULONG Needed, l;
	GUID HidGuid;
	HDEVINFO DeviceInfoSet;
	HIDD_ATTRIBUTES DeviceAttributes;
	SP_DEVICE_INTERFACE_DATA DevData;
	PSP_INTERFACE_DEVICE_DETAIL_DATA DevDetail;
	//SP_DEVICE_INTERFACE_DETAIL_DATA *MyDeviceInterfaceDetailData;

	DeviceAttributes.Size = sizeof(HIDD_ATTRIBUTES);
	DevData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	HidD_GetHidGuid(&HidGuid);
#if 1							/* For vista */
	DeviceInfoSet =
		SetupDiGetClassDevs(&HidGuid, NULL, NULL,
							DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
#else
	DeviceInfoSet =
		SetupDiGetClassDevs(&HidGuid, "", NULL,
							DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
#endif

	while (SetupDiEnumDeviceInterfaces
		   (DeviceInfoSet, 0, &HidGuid, i++, &DevData)) {
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DevData, NULL, 0,
										&Needed, 0);
		l = Needed;
		DevDetail =
			(SP_DEVICE_INTERFACE_DETAIL_DATA *) GlobalAlloc(GPTR, l + 4);
		DevDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DevData, DevDetail,
										l, &Needed, 0);

		hHID = CreateFile(DevDetail->DevicePath,
						  GENERIC_READ | GENERIC_WRITE,
						  FILE_SHARE_READ | FILE_SHARE_WRITE,
						  NULL, OPEN_EXISTING,
//                        FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING,
						  0, NULL);

		GlobalFree(DevDetail);

		if (hHID == INVALID_HANDLE_VALUE)	// Can't open a device
			continue;
		HidD_GetAttributes(hHID, &DeviceAttributes);

		// HIDaspかどうか調べる.
		if (DeviceAttributes.VendorID == MY_VID
			&& DeviceAttributes.ProductID == MY_PID
			&& (check_product_string(hHID) == 1)) {
			f = 1;				// 発見された.
			break;
		} else {
			// 違ったら閉じる
			CloseHandle(hHID);
			hHID = NULL;
		}
	}
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
	return f;
}

//  メモリーダンプ.
void memdump(char *msg, char *buf, int len)
{
	int j;
	fprintf(stderr, "%s", msg);
	for (j = 0; j < len; j++) {
		fprintf(stderr, " %02x", buf[j] & 0xff);
		if (j == 31)
			fprintf(stderr, "\n +");
	}
	fprintf(stderr, "\n");
}

//  先頭24バイトまでのダンプ.
void pktdump(char *msg, char *buf, int id)
{
	int len = report_size_tab[id & 7];
	memdump(msg, buf, len);
}

#if	TRANSFER_BENCH				// 転送速度ベンチ.
#define CHECK_COUNT 1000
#else
#define CHECK_COUNT 4
#endif

int hidasp_init(char *string)
{
	ULONG sz;
	char wr_data[128], rd_data[128];
	int i, r;

	LoadHidDLL();
	if (OpenTheHid() == 0) {
#if DEBUG
		fprintf(stderr, "ERROR: fail to OpenTheHid()\n");
#endif
		return 1;
	}

	GetDevCaps();
	Sleep(100);

#if DEBUG
	fprintf(stderr, "HIDASP Connection check!\n");
#endif

	wr_data[0] = 0;
	wr_data[1] = HIDASP_TEST;	// Connection test
	wr_data[2] = 0;
	wr_data[3] = 0;

	for (i = 0; i < CHECK_COUNT; i++) {
		int j;
		wr_data[2] = i;
		for (j = 3; j < 31; j++) {
			wr_data[j] = i;
		}
#if DEBUG
		fprintf(stderr, "HIDasp write %2d.\n", i);
#endif
		r = hidWrite(hHID, wr_data, Caps.OutputReportByteLength, &sz, 0);
#if DEBUG
//      memdump("wr",wr_data,32);
#endif
//      Sleep(10);  /* wait */

		memset(rd_data, 0, 32);
		r = hidRead(hHID, rd_data, Caps.InputReportByteLength, &sz, 0);
#if DEBUG
//      memdump("rd",rd_data,32);
#endif
#if DEBUG
		fprintf(stderr, "HIDasp read  %2d. %d\n", i, rd_data[1]);
#endif
		if (r == 0) {
			fprintf(stderr, "ERR. fail to Read().\n");
			return 1;
		}
#if 0
		if (rd_data[1] == i) {
			continue;
		}
#else							/* !!! */
		if (rd_data[1] == i) {
			 Sleep(10);  /* wait */
		} else {
			return 0;
		}
#endif
	}
#if DEBUG
	fprintf(stderr, "OK.\n");
#endif
	return 0;
}

int hidasp_program_enable(int delay)
{
	unsigned char buf[128];
	unsigned char res[4];
	ULONG sz;
	int i;

	// エラー時にはリトライするように修正 by senshu(2008-9-16)
	for (i = 0; i < 3; i++) {
		Sleep(2);
		buf[0] = 0;
		buf[1] = HIDASP_SET_STATUS;	// LED CONT
		buf[2] = 0;					// PORTD
		buf[3] = HIDASP_RST_H_GREEN;	// RESET HIGH
		hidWrite(hHID, buf, Caps.OutputReportByteLength, &sz, 0);
		Sleep(100);				// 10 => 100

		buf[0] = 0;
		buf[1] = HIDASP_SET_STATUS;	// LED CONT
		buf[2] = 0;					// PORTD
		buf[3] = HIDASP_RST_L_BOTH;	// RESET LOW
		hidWrite(hHID, buf, Caps.OutputReportByteLength, &sz, 0);

		buf[0] = 0;
		buf[1] = HIDASP_SET_DELAY;	// SET_DELAY
		buf[2] = delay;			// delay value
		buf[3] = 0;
		hidWrite(hHID, buf, Caps.OutputReportByteLength, &sz, 0);
		Sleep(30);				// 30

		buf[0] = 0xAC;
		buf[1] = 0x53;
		buf[2] = 0x00;
		buf[3] = 0x00;
		hidasp_cmd(buf, res);

		if (res[2] == 0x53) {
#if DEBUG
			fprintf(stderr, "hidasp_program_enable() == OK\n");
#endif
			return 0;
		} else {
#if DEBUG
			fprintf(stderr, "hidasp_program_enable() == NG\n");
#endif
			Sleep(100);
		}
	}
	return 1;
}


void hidasp_close()
{
	if (hHID) {
		ULONG sz;
		unsigned char buf[128];

		buf[0] = 0x00;
		buf[1] = HIDASP_NOP;
		buf[2] = 0x00;
		buf[3] = 0x00;
		hidasp_cmd(buf, NULL);	// AVOID BUG!

		buf[0] = 0;
		buf[1] = HIDASP_SET_STATUS;
		buf[2] = 0;
		buf[3] = HIDASP_RST_H_GREEN;			// RESET HI
		hidWrite(hHID, buf, Caps.OutputReportByteLength, &sz, 0);
		CloseHandle(hHID);
	}
	if (hHID_DLL) {
		FreeLibrary(hHID_DLL);
	}
	hHID = NULL;
	hHID_DLL = NULL;
}

int hidasp_cmd(const unsigned char cmd[4], unsigned char res[4])
{
	ULONG sz = 0;
	char buf[128];
	int r;

	buf[0] = 0;
	if (res != NULL) {
		buf[1] = HIDASP_CMD_TX_1;
	} else {
		buf[1] = HIDASP_CMD_TX;
	}

#if 0
	Sleep(2);
	if (cmd[0] == 0x38 && cmd[2] == 0) {	// BUG!
		buf[8] = 0;
		buf[9] = 123;			// dummy
		hidWrite(hHID, buf + 8, Caps.OutputReportByteLength, &sz, 0);
		Sleep(2);
	}
#endif
//  Sleep(2);   // !!!
	memcpy(buf + 2, cmd, 4);
	r = hidWrite(hHID, buf, Caps.OutputReportByteLength, &sz, REPORT_ID1);
#if DEBUG
	fprintf(stderr, "hidasp_cmd %02X, cmd: %02X %02X %02X %02X ", buf[1],
			cmd[0], cmd[1], cmd[2], cmd[3]);
#endif

	if (res != NULL) {
		r = hidRead(hHID, buf, Caps.InputReportByteLength, &sz, 0);
		memcpy(res, buf + 1, 4);
#if DEBUG
		fprintf(stderr, " --> res: %02X %02X %02X %02X\n", res[0], res[1],
				res[2], res[3]);
#endif
	}

	return 1;
}

int hidasp_page_write(long addr, const unsigned char *wd, int pagesize)
{
	int n, l;
	ULONG sz = 0;
	char buf[128];

	// set page
	buf[0] = 0x00;
	buf[1] = HIDASP_SET_PAGE;	// Set Page mode
	buf[2] = 0x40;				// Flash
	buf[3] = 0x00;
	buf[4] = (char) (addr & 0xFF);
	hidWrite(hHID, buf, Caps.OutputReportByteLength, &sz, REPORT_ID1);

	// Load the page data into page buffer
	n = 0;
	while (n < pagesize) {
		l = Caps.OutputReportByteLength - 3;	// MAX
		if (pagesize - n < l)
			l = pagesize - n;
		buf[0] = 0x00;
		buf[1] = HIDASP_PAGE_TX;	// PageBuf
		buf[2] = l;				// Len
		memcpy(buf + 3, wd + n, l);
#if DEBUG
		Sleep(2);
#endif
		{
			int report_id = REPORT_ID1;
			if (l < 7) {
				report_id = REPORT_ID1;
			} else if (l < 15) {
				report_id = REPORT_ID2;
			} else if (l < 23) {
				report_id = REPORT_ID3;
			} else if (l < 31) {
				report_id = REPORT_ID4;
			} else if (l < 37) {
				report_id = REPORT_ID5;
			} else {
				report_id = REPORT_ID6;
			}
			hidWrite(hHID, buf, Caps.OutputReportByteLength, &sz,
					 report_id);
		}

#if DEBUG
		fprintf(stderr, "  p: %02x %02x %02x %02x\n",
				buf[1] & 0xff, buf[2] & 0xff, buf[3] & 0xff,
				buf[4] & 0xff);
#endif
		n += l;
	}

	return 0;
}

int hidasp_page_read(long addr, unsigned char *wd, int pagesize)
{
	int n, l;
	ULONG sz = 0;
	char buf[128];

	// set page
	buf[0] = 0x00;
	buf[1] = HIDASP_SET_PAGE;	// Set Page mode
	buf[2] = 0x20;				// FlashRead
	buf[3] = 0x00;
	if (addr >= 0)
		hidWrite(hHID, buf, Caps.OutputReportByteLength, &sz, 0);

	// Load the page data into page buffer
	n = 0;
	while (n < pagesize) {
		l = Caps.InputReportByteLength - 1;	// MAX
		if (pagesize - n < l)
			l = pagesize - n;
		buf[0] = 0x00;
		buf[1] = HIDASP_PAGE_RD;	// PageRead
		buf[2] = l;				// Len
		memset(buf + 3, 0, l);
#if DEBUG
		Sleep(2);
#endif
		hidWrite(hHID, buf, Caps.OutputReportByteLength, &sz, REPORT_ID1);
		hidRead(hHID, buf, Caps.InputReportByteLength, &sz, 0);
		memcpy(wd + n, buf + 1, l);

#if 1
		report_update(l);
#else
		if (n % 128 == 0) {
			report_update(128);
		}
#endif

#if DEBUG
		fprintf(stderr, "  p: %02x %02x %02x %02x\n",
				buf[1] & 0xff, buf[2] & 0xff, buf[3] & 0xff,
				buf[4] & 0xff);
#endif
		n += l;
	}

	return 0;
}

static int hidRead(HANDLE h, char *buf, int Length, ULONG * sz, int id)
{
	int rc;
	if (id == 0) {
		id = REPORT_ID;
	}
	buf[0] = id;
	Length = report_size_tab[id];
	rc = HidD_GetFeature(h, buf, Length);
#if	PKTDUMP
//  fprintf(stderr, "  HidD_GetFeature(%x,%x,%x)=%d\n", h,buf,Length,rc);
	pktdump("RD", buf, id);
#endif
	return rc;
}

static int hidWrite(HANDLE h, char *buf, int Length, ULONG * sz, int id)
{
	int rc;
	if (id == 0) {
		id = REPORT_ID;
	}
	buf[0] = id;
	Length = report_size_tab[id];
	rc = HidD_SetFeature(h, buf, Length);
#if	PKTDUMP
//  fprintf(stderr, "  HidD_SetFeature(%x,%x,%x)=%d\n", h,buf,Length,rc);
	pktdump("WR", buf, id);
#endif
	return rc;
}
