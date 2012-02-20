/* usbasp.c
 * original file was USBasp (http://www.fischl.de/usbasp/)
 *
 * modified for avrspx by mutech, t.kuroki
 * 2006-05-20 mega128x/mega256x
 * 2006-08-26 bug fix. for non-paged avr
 * 2006-08-27 some modify
 * 2006-08-30 libusb-win32-0.1.10.1 の不具合に対処 (とりあえずgccのみ) 
 *            ライブラリをlibusb-win32-0.1.12.0 に変更
 * 2006-08-30 USBバスからのusbaspの検索の高速化
 *            libusbのバージョン表示を追加
 *
 * modified for avrspx by senshu
 * 2008-01-03 progress表示を追加
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _MSC_VER
  #include <io.h>
#endif
#ifdef __BORLANDC__
  #include <io.h>
#endif
#ifndef _WINDEF_
  #include <fcntl.h>
#endif

#include <usb.h>	/* this is libusb, see http://libusb.sourceforge.net/ */

#include "avrspx.h"
#include "usbasp.h"

//extern char * progname;

static usb_dev_handle *usbhandle;

static bool f_connect;
static int iodly_value;
unsigned char usbasp_info[4];

#if AVRSPX
void report_update(int bytes);
#endif

//----------------------------------------------------------------------------
static void setup_serial(unsigned char *dst, char *src)
{
	memset(dst, '\0', USB_CFG_SERIAL_NUMBER_LEN);
	if (src) {
		int nbytes = strlen(src);
		if (nbytes > USB_CFG_SERIAL_NUMBER_LEN)
			nbytes = USB_CFG_SERIAL_NUMBER_LEN;
		memcpy(dst, src, nbytes);
	}
}

//----------------------------------------------------------------------------
/*
 * wrapper for usb_control_msg call
 */
static int usbasp_transmit(unsigned char receive, unsigned char function_id,
			   unsigned char send[4], unsigned char * buffer, int buffersize)
{
	int nbytes;
	nbytes = usb_control_msg(usbhandle,
			   USB_TYPE_VENDOR | USB_RECIP_DEVICE | (receive << 7),
			   function_id,
			   (send[1] << 8) | send[0],
			   (send[3] << 8) | send[2],
			   buffer, buffersize,
			   10000);   //@@@ by t.k
//			   5000);

	if (nbytes < 0) {
		fprintf(stderr, "%s: error: usbasp_transmit: %s\n",
			progname, usb_strerror());
		return -1;
	}

	return nbytes;
}

//----------------------------------------------------------------------------
int usbasp_open(char *SerialNumber)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	char string[256];
	char serial[USB_CFG_SERIAL_NUMBER_LEN+1];
#ifdef __GNUC__
    int  old_fd, nul_fd;
#endif

	setup_serial(serial, SerialNumber);

    /*
     * libusb-win32-0.1.10.1 で usb_find_busses() を実行したときに
     * "found N busses"がstdoutに出力されるため一時的にstdoutをnulに切換 
     * libusb-win32-0.1.10.2/ libusb-win32-0.1.12.0で修正されている 
	 * とりあえずgccのみ (bcc/bcb6/vc8では正常に動作しない)
	 */
#ifdef __GNUC__
	old_fd = dup(fileno(stdout));           // 標準出力のバックアップを作成
    nul_fd = open("nul", O_WRONLY);         // 標準出力用に"nul"を開く
	dup2(nul_fd, fileno(stdout));           // ファイルのディスクリプタのコピーを1番に作成
#endif
	usb_init();

	usb_find_busses();                      // この中で不要な printf がある 
	usb_find_devices();

#ifdef __GNUC__
    fflush(stdout);
	dup2(old_fd, fileno(stdout));           // 標準出力を元に戻す
    close(old_fd);                          // 標準出力のバックアップを閉じる
    close(nul_fd);
#endif

	for (bus = usb_get_busses(); bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			if (dev->descriptor.idVendor == USBDEV_VENDOR &&
				dev->descriptor.idProduct == USBDEV_PRODUCT)
			{
				usbhandle = usb_open(dev);
				if (usbhandle) {
					if (SerialNumber == NULL)
						return 0;				// findfirst
					// check serial number
					if (usb_get_string_simple(usbhandle,
						dev->descriptor.iSerialNumber,
						string, sizeof(string)) < 0)
					{
						// cannot read serial number
						if (!SerialNumber[0])
							return 0;
					}
					if (strnicmp(serial, string, USB_CFG_SERIAL_NUMBER_LEN) == 0)
						return 0;

					usb_close(usbhandle);
				}
			}
		}
	}

	fprintf(stderr, "%s: usb_open(): did not find any%s USB device \"%.4s\"\n",
	   progname, SerialNumber ? " (matching)": "", serial);
	return 1;
}

void usbasp_close(void)
{
	usbasp_disconnect();
	usb_close(usbhandle);
}

//----------------------------------------------------------------------------
int usbasp_disconnect(void)
{
    int ret = 0;

	if (f_connect) {
		unsigned char temp[4];
		memset(temp, 0, sizeof(temp));
        ret = usbasp_transmit(1, USBASP_FUNC_DISCONNECT, temp, temp, sizeof(temp));
		f_connect = false;
	}
	return ret;
}

/*
 *   fck  < 12MHz : Low period>2CPU clock cycle  High period>2CPU clock cycle
 *   fck >= 12MHz : Low period>3CPU clock cycle  High period>3CPU clock cycle
 * 
 *  -d option
 *       USBaspx SCK clock            cycle      TARGET
 *    0 :   0 :   3.0MHz  fosc/4     0.33us  >  12.0MHz
 *    1 :   1 :   1.5MHz  fosc/8     0.67us  >   6.0MHz
 *    2 :   2 : 750.0kHz  fosc/16    1.33us  >   3.0MHz
 * *  3 :   3 : 375.0kHz  fosc/32    2.67us  >   1.5MHz Original High-clock mode
 *    4 :   4 : 187.5kHz  fosc/64    5.33us  > 750.0kHz
 *    5 :   5 :  93.8kHz  fosc/128  10.67us  > 375.0kHz
 * ...
 *   10 :   6 : 50.0kHz   (6-5)*10*2   20us  > 200.0kHz
 *   20 :   7 : 25.0kHz   (7-5)*10*2   40us  > 100.0kHz
 *   30 :   8 : 16.7kHz   (8-5)*10*2   60us  >  66.7kHz
 *   40 :   9 : 12.5kHz   (9-5)*10*2   80us  >  50.0kHz
 *   50 :  10 : 10.0kHz  (10-5)*10*2  100us  >  40.0kHz
 * * 60 :  11 :  8.3kHz  (11-5)*10*2  120us  >  33.3kHz Original Low-clock mode
 *   70 :  12 :  7.1kHz  (12-5)*10*2  140us  >  28.6kHz
 * ...
 * 2500 : 255 :   200Hz (255-5)*10*2    5ms  > 800Hz
 */
int usbasp_connect(int iodly)
{
	unsigned char temp[4];

	memset(temp, 0, sizeof(temp));
	iodly_value = iodly;
	if (iodly_value >= 0) {		//@@@ by t.k
		int value;

		temp[1] = 0xaa;			// magic no for Extended function
		temp[2] = 0x55;			// original firm was ignored.
		if (iodly_value <= 5) {	//@@@ hardware sck
			value = iodly_value;
		} else {
			value  = (iodly_value+5) / 10;	//@@@ per 10us
			value += 5;						// 1.. -> 6..
			if (value > 255)
				value = 255;
		}
		temp[3] = (unsigned char)value;
//@@@fprintf(stderr, "set usbasp spi delay value %d\n", value);
	}
	usbasp_transmit(1, USBASP_FUNC_CONNECT, temp, temp, sizeof(temp));

//  usleep(100000);
	delay_ms(100);

//  pgm->program_enable(pgm, p);
	f_connect = true;
	return 0;
}

int usbasp_cmd(unsigned char cmd[4],
                   unsigned char res[4])
{
	int nbytes =
		usbasp_transmit(1, USBASP_FUNC_TRANSMIT, cmd, res, sizeof(res));

	if (nbytes != 4) {
		fprintf(stderr, "%s: error: wrong responds size\n", progname);
		return -1;
	}

	return 0;
}


//----------------------------------------------------------------------------
int usbasp_program_enable(void)
{
	unsigned char res[4];
	unsigned char cmd[4];
	int nbytes;

	memset(cmd, 0, sizeof(cmd));
	memset(res, 0, sizeof(res));

	nbytes = usbasp_transmit(1, USBASP_FUNC_ENABLEPROG, cmd, res, sizeof(res));

	if ((nbytes < 1) || (res[0] != 0)) {
		fprintf(stderr, "%s: error: programm enable: target doesn't answer. %x \n",
			progname, res[0]);
		return -1;
	}

	return 0;
}

//----------------------------------------------------------------------------
/*
 * USBaspの通信ブロック単位(200bytes)で分割して読み込む
 */
int usbasp_paged_load_sub(int function, int address,
				unsigned char *buffer, int n_bytes, unsigned char flags)
{
	int n;
	unsigned char cmd[4];
	int wbytes = n_bytes;
	int blocksize;

	while (wbytes > 0) {
		if (wbytes > USBASP_READBLOCKSIZE) {
			blocksize = USBASP_READBLOCKSIZE;
			wbytes   -= USBASP_READBLOCKSIZE;
		} else {
			blocksize = wbytes;
			wbytes    = 0;
		}

		cmd[0] = address & 0xFF;
		cmd[1] = address >> 8;
		cmd[2] = 0x5A;				//@@ magic no by t.k
		cmd[3] = flags;				//@@ 

		n = usbasp_transmit(1, function, cmd, buffer, blocksize);

		if (n != blocksize) {
			fprintf(stderr, "%s: error: wrong reading bytes %x\n",
				progname, n);
			return -1;
		}

		buffer  += blocksize;
		address += blocksize;
#if AVRSPX
		report_update(n);
#else
		//@	report_progress (address, n_bytes, NULL);
#endif
	}

	return n_bytes;
}

int usbasp_paged_load(
 char dst, 
 void *buf,
 int page_size, 
 int n_bytes, 
 bool ext)
{
	int address = 0;
	unsigned char * buffer = buf;
	int wbytes = n_bytes;
	int blocksize;
	unsigned char flags;
	int function;
    int ret;

	if (dst == FLASH) {
		function = USBASP_FUNC_READFLASH;
	} else if (dst == EEPROM) {
		function = USBASP_FUNC_READEEPROM;
	} else {
		return -2;
	}

	// 64K bytes単位に処理する
	while (wbytes > 0) {
		if (wbytes > 0x10000) {
			blocksize = 0x10000;
			wbytes   -= 0x10000;
		} else {
			blocksize = wbytes;
			wbytes    = 0;
		}
		flags = 0;
		if (address & 0x10000) {	// mega128x/mega256x
			flags |= USBASPX_FLAG_ADDR_16;
		}
		if (ext) {					// mega256x FlashSize > 128*1024
			flags |= USBASPX_FLAG_ADDR_EX;
			if (address & 0x20000) {
				flags |= USBASPX_FLAG_ADDR_17;
			}
		}
		ret = usbasp_paged_load_sub(function, address, buffer, blocksize, flags);
		if (ret < 0) {
			return ret;
		}
		buffer  += blocksize;
		address += blocksize;
	}

	return n_bytes;
}
//----------------------------------------------------------------------------
/*
 * USBaspの通信ブロック単位(200bytes)で分割して書き込む
 */
int usbasp_blockwrite(
	int      function,
	int      address,
	unsigned char *buffer,
	int      n_bytes,
	int      page_size,
	unsigned char  flags
)
{
	unsigned char cmd[4];
	int blocksize, n;
	int top            = address;
	int wbytes         = n_bytes;
	unsigned char  blockflags = USBASP_BLOCKFLAG_FIRST;

	while (wbytes > 0) {
		if (wbytes > USBASP_WRITEBLOCKSIZE) {
			blocksize = USBASP_WRITEBLOCKSIZE;
			wbytes   -= USBASP_WRITEBLOCKSIZE;
		} else {
			blocksize   = wbytes;
			wbytes      = 0;
			blockflags |= USBASP_BLOCKFLAG_LAST;
		}

	    cmd[0] = address & 0xFF;
	    cmd[1] = address >> 8;
	    cmd[2] = page_size;
	    cmd[3] = blockflags | flags;
		blockflags = 0;

		n = usbasp_transmit(0, function, cmd, buffer, blocksize);
		if (n != blocksize)
			break;

		buffer  += blocksize;
		address += blocksize;

#if AVRSPX
		report_update(n);
#else
		//@	report_progress (address, n_bytes, NULL);
#endif
	}

	return address - top;
}

static bool is_blank_area(void *buf, int size)
{
	unsigned char * buffer = buf;
	int count = size;

	unsigned char d = 0xff;
	do {
		d &= *buffer++;
	} while (--count);
	return !(d + 1);
}

static bool is_blank_page(void *buf, int page_size)
{
	unsigned long * buffer = buf;
	int count = page_size / sizeof(unsigned long);

	unsigned long d = 0xffffffff;
	do {
		d &= *buffer++;
	} while (--count);
	return !(d + 1);
}

/*
 * 連続した有効なページのみを切り出して書き込む
 */
int usbasp_paged_write_sub(
	int      function,
	int      address,
	unsigned char *buffer,
	int      n_bytes,
	int      page_size,
	unsigned char  flags
)
{
	int top         = address;
	int wbytes      = n_bytes;
	int check_size  = page_size;
	int blocksize;
	unsigned char *tmpbuf;
	int tmpaddr;

	if (check_size == 0)		// ページなしの時は32Bytes単位で処理
		check_size = 32;

	while (wbytes > 0) {
		// ブランクでないページの検索
		tmpbuf  = buffer;
		tmpaddr = address;
		while (wbytes >= check_size
		       && !is_blank_page(buffer, check_size))
		{
			address += check_size;
			buffer  += check_size;
			wbytes  -= check_size;
		}
		if (wbytes > 0 && wbytes < check_size
		    && !is_blank_area(buffer, wbytes)) {
			address += wbytes;
			buffer  += wbytes;
			wbytes   = 0;
		}
		blocksize = address - tmpaddr;
		if (blocksize != 0) {
			int ret = usbasp_blockwrite(
					function,
					tmpaddr,
					tmpbuf,
					blocksize,
					page_size,
					flags
				);
			if (ret != blocksize) {
				address = tmpaddr;
				break;
			}
		}

		// ブランクページのスキップ
		while (wbytes >= check_size
		       && is_blank_page(buffer, check_size)) {
			address += check_size;
			buffer  += check_size;
			wbytes  -= check_size;
		}
		if (wbytes > 0 && wbytes < check_size
		    && is_blank_area(buffer, wbytes))
		{
			address += wbytes;
			buffer  += wbytes;
			wbytes   = 0;
		}

	}

	return address - top;
}

int usbasp_paged_write(
 char dst, 
 void *buf,
 int page_size,
 int n_bytes,
 bool ext)
{
	int function;
	int address = 0;
	int wbytes = n_bytes;
	int blocksize;
	unsigned char * buffer = buf;
	unsigned char flags;
    int ret;

	if (dst == FLASH)
		function = USBASP_FUNC_WRITEFLASH;
	else if (dst == EEPROM)
		function = USBASP_FUNC_WRITEEEPROM;
	else
		return -2;

	// 64K bytes単位に処理する
	while (wbytes > 0) {
		if (wbytes > 0x10000) {
			blocksize = 0x10000;
			wbytes   -= 0x10000;
		} else {
			blocksize = wbytes;
			wbytes    = 0;
		}
		flags = (page_size & 0xF00) >> 4;	//original TP: Mega128 fix
		if (address & 0x10000) {			// mega128x/mega256x
			flags |= USBASPX_FLAG_ADDR_16;
		}
		if (ext) {							// mega256x FlashSize > 128*1024
			flags |= USBASPX_FLAG_ADDR_EX;
			if (address & 0x20000)
				flags |= USBASPX_FLAG_ADDR_17;
		}

		ret = usbasp_paged_write_sub(
					function,
					address,
					buffer,
					blocksize,
					page_size,
					flags
				);

		if (ret != blocksize) {
			fprintf(stderr, "%s: error: wrong count at writing %x\n",
				progname, ret);
			return -1;
		}

		buffer  += blocksize;
		address += blocksize;
	}

	return n_bytes;
}
//----------------------------------------------------------------------------
/*
 * 連続した有効なページのみを切り出してベリファイ
 */
int usbasp_verify_sub(
	int      function,
	int      address,
	unsigned char *buffer,
	unsigned char *vbuf,
	int      n_bytes,
	int      page_size,
	unsigned char  flags
)
{
	int wbytes      = n_bytes;
	int check_size  = page_size;
	int blocksize;
	int tmpaddr;
    int i;

	if (check_size == 0)		// ページなしの時は32Bytes単位で処理
		check_size = 32;

	while (wbytes > 0) {
		// ブランクでないページの検索
		tmpaddr = address;
		while (wbytes >= check_size &&
			 !is_blank_page(&buffer[address], check_size))
		{
			address += check_size;
			wbytes  -= check_size;
		}
		if (wbytes > 0 && wbytes < check_size
		    && !is_blank_area(&buffer[address], wbytes))
		{
			address += wbytes;
			wbytes   = 0;
		}
		blocksize = address - tmpaddr;
		if (blocksize != 0) {
			int ret = usbasp_paged_load_sub(
							function,
							tmpaddr,
							&vbuf[tmpaddr],
							blocksize,
							flags);
			if (ret != blocksize) {
				return RC_FAIL;
			}
			for (i = blocksize; i; --i) {
				if (vbuf[tmpaddr] != buffer[tmpaddr]) {
					fprintf(stderr, "Failed at %04X:%02X-%02X\n",
							tmpaddr, buffer[tmpaddr], vbuf[tmpaddr]);
					return RC_FAIL;
				}
				++tmpaddr;
			}
		}

		// ブランクページのスキップ
		while (wbytes >= check_size
		       && is_blank_page(&buffer[address], check_size))
		{
			address += check_size;
			wbytes  -= check_size;
		}
		if (wbytes > 0 && wbytes < check_size
		    && is_blank_area(&buffer[address], wbytes))
		{
			address += wbytes;
			wbytes   = 0;
		}

	}

	return 0;
}

int usbasp_paged_verify(char dst, void *buf,
  int page_size, int n_bytes, bool ext)
{
	int function;
	int address = 0;
	int wbytes = n_bytes;
	int blocksize;
	unsigned char * buffer = buf;
	unsigned char flags;
    int ret;
	BYTE *vbuf;

	if (dst == FLASH) {
		function = USBASP_FUNC_READFLASH;
	} else if (dst == EEPROM) {
		function = USBASP_FUNC_READEEPROM;
	} else {
		return RC_FAIL;
	}

	ret = 0;
	vbuf = malloc(n_bytes);
	// 64K bytes単位に処理する
	while (wbytes > 0) {
		if (wbytes > 0x10000) {
			blocksize = 0x10000;
			wbytes   -= 0x10000;
		} else {
			blocksize = wbytes;
			wbytes    = 0;
		}
		flags = 0;
		if (address & 0x10000) {	// mega128x/mega256x
			flags |= USBASPX_FLAG_ADDR_16;
		}
		if (ext) {					// mega256x FlashSize > 128*1024
			flags |= USBASPX_FLAG_ADDR_EX;
			if (address & 0x20000)
				flags |= USBASPX_FLAG_ADDR_17;
		}
		ret = usbasp_verify_sub(
					function,
					address,
		 			buffer,
		 			vbuf,
					blocksize,
					page_size,
					flags
				);

		if (ret != 0)
			break;
	}

	free(vbuf);
	return ret;
}
//----------------------------------------------------------------------------
static int usbasp_get_info(unsigned char res[4])
{
	unsigned char cmd[4];
	int nbytes;

	memset(cmd, 0, sizeof(cmd));
	memset(res, 0, sizeof(res));

	nbytes = usbasp_transmit(1, USBASPX_FUNC_GETINFO, cmd, res, sizeof(res));
	if (nbytes != 4) {
//		fprintf(stderr, "%s: error: get info\n", progname);
		return -1;
	}

	return 0;
}
//----------------------------------------------------------------------------
static int usbasp_set_serial(char *SerialNumber)
{
	unsigned char res[4];
	unsigned char cmd[USB_CFG_SERIAL_NUMBER_LEN];
	int nbytes;

	setup_serial(cmd, SerialNumber);
	memset(res, 0, sizeof(res));

	nbytes = usbasp_transmit(1, USBASPX_FUNC_SETSERIAL, cmd, res, sizeof(res));
    if (nbytes != 1 || res[0] != 0) {
        fprintf(stderr, "%s: error: set serial: target doesn't answer. %x \n",
	       progname, res[0]);
        return -1;
    }

  return 0;
}
//----------------------------------------------------------------------------
int usbasp_list(void)
{
	char string[256];
	char product[256];
	unsigned char info[4];
	struct usb_bus *bus;
	struct usb_device *dev;
	int usbasp_found;
	const struct usb_version *version;

	usb_init();

	usb_find_busses();
	usb_find_devices();

	version = usb_get_version();

	if (version) {
		printf("libusb-win32 : \n");
		printf("  DLL(libusb0.dll)    version: %d.%d.%d.%d\n",
			version->dll.major, version->dll.minor, 
			version->dll.micro, version->dll.nano);

		printf("  Driver(libusb0.sys) version: %d.%d.%d.%d\n\n",
			version->driver.major, version->driver.minor, 
			version->driver.micro, version->driver.nano);

		if (version->dll.major != version->driver.major
			|| version->dll.minor != version->driver.minor
			|| version->dll.micro != version->driver.micro
//			|| version->dll.nano != version->driver.nano
		   )
			printf(" *** Warning : DLL, Driver Version mismatch ***\n");
	}

	usbasp_found = 0;

	for (bus = usb_get_busses(); bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			if (dev->descriptor.idVendor == USBDEV_VENDOR &&
				dev->descriptor.idProduct == USBDEV_PRODUCT)
			{
				usbhandle = usb_open(dev);
				if (usbhandle) {
					/* yeah, we found something */
					if (usb_get_string_simple(usbhandle,
						dev->descriptor.iSerialNumber,
						string, sizeof(string)) < 0) {
						strcpy(string, "[unknown]");
					}

					if (usb_get_string_simple(usbhandle,
						dev->descriptor.iProduct,
						product, sizeof(product)) < 0) {
						strcpy(product, "[unnamed product]");
					}

					if (usbasp_get_info(info) == 0) {
						fprintf(stdout,
							"%s: Found %s, Serial Number: [%s], Firmware version %X.%02X\n",
							progname, product, string, info[1], info[0]);
//	fprintf(stdout, "\t%02X %02X\n", info[2], info[3]);
					} else {
						fprintf(stdout,
							"%s: Found %s, Serial Number: [%s]\n",
							progname, product, string);
					}

                    ++usbasp_found;

					usb_close(usbhandle);
				}
			}
		}
	}

	if (!usbasp_found) {
		fprintf(stderr, "%s: did not find any USBasp device \n", progname);
		return -1;
	}
	return usbasp_found;
}
//----------------------------------------------------------------------------
int usbasp_write_serial(char *SerialNumber, char *NewSerialNumber)
{
    int ret;

	if (usbasp_open(SerialNumber) != 0) {
		return RC_FAIL;
	}

	ret = usbasp_set_serial(NewSerialNumber);

	usbasp_close();

	if (ret == 0) {
    	fprintf(stderr, "USBasp serial number set to [%s].\n", NewSerialNumber);
    	return 0;
    }

	return RC_FAIL;
}
//----------------------------------------------------------------------------

int usbasp_initialize(char *SerialNumber)
{
//    usb_set_debug(0);
	if (usbasp_open(SerialNumber) != 0)
		return RC_INIT;

	usbasp_get_info(usbasp_info);

	return 0;
}
//----------------------------------------------------------------------------
