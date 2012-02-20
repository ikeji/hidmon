#ifndef __usbasp_h__
#define __usbasp_h__

#define	USBDEV_VENDOR			0x03eb	/* ATMEL */
#define	USBDEV_PRODUCT			0xc7B4 	/* USBasp */

#define USBASP_FUNC_CONNECT		1
#define USBASP_FUNC_DISCONNECT	2
#define USBASP_FUNC_TRANSMIT	3
#define USBASP_FUNC_READFLASH	4
#define USBASP_FUNC_ENABLEPROG	5
#define USBASP_FUNC_WRITEFLASH	6
#define USBASP_FUNC_READEEPROM	7
#define USBASP_FUNC_WRITEEEPROM	8

#define USBASPX_FUNC_GETINFO	90
#define USBASPX_FUNC_SETSERIAL	91

#define USBASP_BLOCKFLAG_FIRST	0x01
#define USBASP_BLOCKFLAG_LAST	0x02

#define USBASPX_FLAG_ADDR_16	0x04	//@@+ for mega128x/mega256x
#define USBASPX_FLAG_ADDR_17	0x08	//@@+ for mega256x
#define USBASPX_FLAG_ADDR_EX	0x80	//@@+ for mega256x

#define USBASP_READBLOCKSIZE	200
#define USBASP_WRITEBLOCKSIZE	200

//----------------------------------------------------------------------------
#define USB_CFG_SERIAL_NUMBER_LEN	4

int usbasp_open(char *SerialNumber);
void usbasp_close(void);
int usbasp_connect(int iodly);
int usbasp_disconnect(void);
int usbasp_program_enable(void);
int usbasp_cmd(unsigned char cmd[4], unsigned char res[4]);
int usbasp_paged_load(char dst, void *buf, int page_size, int n_bytes, bool ext);
int usbasp_paged_write(char dst, void *buf, int page_size, int n_bytes, bool ext);
int usbasp_paged_verify(char dst, void *buf, int page_size, int n_bytes, bool ext);
int usbasp_list(void);
int usbasp_write_serial(char *SerialNumber, char *NewSerialNumber);
int usbasp_initialize(char *SerialNumber);

int usb_cleanup(void);	/* libusb_dyn.c , add by senshu */

#endif /* __usbasp_h__ */
