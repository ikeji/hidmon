/* USBasp main.c */
/* 高速化の改良は、irukaさんが行いました
   2008年9月12日～9月22日
   2008年9月22日 テスト公開開始
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usbdrv.h"

/* ------------------------------------------------------------------------- */

#define	INCLUDE_MONITOR_CMD 0	// 62:POKE(),63:PEEK()を実装する.
#define	INCLUDE_LED_CMD 	1	// 02:LEDコマンドを実装する.

/* ------------------------------------------------------------------------- */

#define ID4    4
#define	SIZE4 32
#define ID5    5
#define	SIZE5 40
#define ID6    6
#define	SIZE6 48

PROGMEM char usbHidReportDescriptor[69] = {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)

    0x85, 0x01,                    //   REPORT_ID (1)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, 0x02,                    //   REPORT_ID (2)
    0x95, 0x0e,                    //   REPORT_COUNT (14)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, 0x03,                    //   REPORT_ID (3)
    0x95, 0x16,                    //   REPORT_COUNT (22)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, 0x04,                    //   REPORT_ID (4)
    0x95, 0x1e,                    //   REPORT_COUNT (30)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, 0x05,                    //   REPORT_ID (5)
    0x95, 0x26,                    //   REPORT_COUNT (38)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, 0x06,                    //   REPORT_ID (6)
    0x95, 0x2e,                    //   REPORT_COUNT (46)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0xc0                           // END_COLLECTION
};
/* Note: REPORT_COUNT does not include report-ID byte */


static uchar hidStatus;			   // HID Report ID
//static uchar requestType;

//
//	受信バッファ.
//
static uchar currentPosition, bytesRemaining; // Receive Data Pointer

typedef struct {
	uchar id[1];
	uchar buf[39];
} ReportBuf_t;

static ReportBuf_t report;

// 送信バッファは受信バッファを流用する.
//	但し、先頭1バイトはReport IDを置く必要があるので,1バイトずれる.
#define	usbData report.buf
#define	buffer  report.id

//
#if	INCLUDE_MONITOR_CMD

//	メモリーを読み書きするコマンド構造.
typedef struct {
	uchar hidasp_cmd;	// 62=poke 63=peek
	uchar count;		// 読み込みバイト数.
	uchar *addr;		// 読み書きアドレス.
	uchar memdata[16];	// 読み込みデータ. もしくは書き込みデータ[0] マスク[1]
} MonCommand_t;

static void cmd_poke(MonCommand_t *cmd)
{
	uchar *p=cmd->addr;
	if(cmd->memdata[1]) {	//マスク付の書き込み.
		*p = (*p & cmd->memdata[1]) | cmd->memdata[0];
	}else{			//べた書き込み.
		*p = cmd->memdata[0];
	}
}
static void cmd_peek(MonCommand_t *cmd)
{
	uchar i;
	uchar *p=cmd->addr;
	for(i=0;i<(cmd->count);i++) {
		cmd->memdata[i]=*p++;
	}
}
#endif


// 最適化用
#define hbyte(a) (*((uchar*)&(a)+1))
#define lbyte(a) (*((uchar*)&(a)))
inline static uint8_t byte(uint8_t t) {return t;}

static void delay(uchar d) {
	do {	// 4clock loop = 0.33usec * N
		asm("nop");
		asm("nop");
	} while(d--);
}
static uint8_t wait=60; // 160

static uint8_t usi_trans(uint8_t data){
	USIDR=data;
	USISR=(1<<USIOIF);
	do{
		if(wait == 0) {
#if 0	// この命令は無くとも 12MHzで動作することを確認
			asm("nop");
#endif
		}
		else {
			delay(wait);
		}
		USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
	} while(!(USISR&(1<<USIOIF)));
	return USIDR;
}


static inline void isp_command(uint8_t *data){
	int i;
	for (i=0;i<4;i++) {
		usbData[i]=usi_trans(data[i]);
	}
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- execute Buffer ---------------------------- */
/* ------------------------------------------------------------------------- */
//
//		hidaspのメイン処理.
//
//	送信バッファと受信バッファが共通なので、
//	書き潰したあとのデータを使わないように注意.

#define HIDASP_TEST 1
#define HIDASP_SET_STATUS 2
#define HIDASP_CMD_TX 16
#define HIDASP_SET_PAGE 20
#define HIDASP_PAGE_TX 22
#define HIDASP_SET_DELAY 60
#define HIDASP_POKE 62
#define HIDASP_PEEK 63

void hidasp_main(uchar *data)
{
	static uchar page_mode;
	static uint16_t page_addr;
	uchar i;
	uint8_t data0 = data[0];
	uint8_t data1 = data[1];
	uint8_t cmd   = data0 & 0xfe;

	if ( data[0] == HIDASP_TEST ) { // TEST
		usbData[0] = data1;
	}
#if	INCLUDE_LED_CMD
	else if ( data0 == HIDASP_SET_STATUS ) { // SET_LED
		PORTD = (PORTD&~3)    | (data1 & 3);
//		PORTB = (PORTB&~0x10) | (data[2]&0x1F);
		// RESETピンを保持、それ以外をOR出力
		PORTB = (PORTB&~0x1F) | (data[2]&0x1f) ;
	}
#endif
	else if ( cmd == HIDASP_CMD_TX) { // SPI
		isp_command(data+1);
	} else if ( data0 == HIDASP_SET_PAGE ) { // Page set
		page_mode = data1;
		page_addr = 0;
	} else if ( cmd == HIDASP_PAGE_TX ) { // Page buf
		for(i=0;i<data1;i++) {
			usi_trans(page_mode);
			usi_trans(hbyte(page_addr));
			usi_trans(lbyte(page_addr));
			usbData[i]=usi_trans(data[i+2]);
			if (page_mode & 0x88) { // EEPROM or FlashH
				page_addr++;
				page_mode&=~0x08;
			} else {
				page_mode|=0x08;
			}
		}
	} else if ( data0 == HIDASP_SET_DELAY ) { // Set wait
		wait=data1;
	}
#if	INCLUDE_MONITOR_CMD
	else if ( data0 == HIDASP_POKE ) {
		cmd_poke((MonCommand_t *)data);
	} else if ( data0 == HIDASP_PEEK ) {
		cmd_peek((MonCommand_t *)data);
	}
#endif

#if 0	// 使うのはコントロール転送のみ
	if (data[0]&1) {
		usbSetInterrupt(usbData, 8);
	}
#endif

//	return 8;
}


/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

// デフォルト以外の usb setup コマンドパケットをここで解釈する.
uchar usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	uchar           rqType = rq->bmRequestType & USBRQ_TYPE_MASK;
    if(rqType == USBRQ_TYPE_CLASS){    /* class request type */
        if(	rq->bRequest == USBRQ_HID_GET_REPORT ) {
        		report.id[0] = ID5;             /* report ID */
        		usbMsgPtr = report.id;
        		return SIZE5;
		}
        if(	rq->bRequest == USBRQ_HID_SET_REPORT ) {
            hidStatus = rq->wValue.bytes[0];    /* store report ID */
		    currentPosition = 0;                // initialize position index
        	bytesRemaining = rq->wLength.word;  // store the amount of data requested
        	if(	bytesRemaining > sizeof(report)) { // limit to buffer size
            	bytesRemaining = sizeof(report);
			}
          	return 0xff;						// tell driver to use usbFunctionWrite()
        }
    }
    return 0;
}


// コントロール転送が後続データを持っている場合、それを8バイト単位で受け取る.
// 	・結果はreport構造体に入れる.
//	・全部受け取ったら、hidasp_main()を呼び出す.
uchar usbFunctionWrite(uchar *data, uchar len)
{
    uchar i;
    if(	len > bytesRemaining) {             // if this is the last incomplete chunk
        len = bytesRemaining;               // limit to the amount we can store
	}
    bytesRemaining -= len;
    for(i = 0; i < len; i++) {
        report.id[currentPosition++] = data[i];
	}
	// 全部受け取ったら、バッファ内容を実行.
    if( bytesRemaining == 0 ) {
		if( hidStatus > 0 ) {
			hidasp_main(report.buf);
		}
	    return 1;	// return 1 if we have all data
	}
	return 0;		// continue data.
}

/*
       ATtiny2313
       ___    ___
RESET [1  |__| 20]Vcc
      [2       19]PB7(SCK)
      [3       18]PB6(MISO)
XTAL2 [4       17]PB5(MOSI)
XTAL1 [5       16]PB4(RST)
      [6       15]
PD3   [7       14]
PD4   [8       13]
      [9       12]
GND   [10      11]
       ~~~~~~~~~~

   ---------------------------------------
   SPI:     PB7-4 ===> [Target AVR Device]
   USB:     PD4   ===> USB D-
            PD3   ===> USB D+
   XTAL:    XTAL1,2 => Crystal 12MHz
   ---------------------------------------
*/

int main(void)
{

/* Port Direction設定は、以下より3択 */
#if	1
//	USB D+ D- の配線が瓶詰堂さんの新しい回路図用 HIDsphとも互換なのでこれを推奨.
    DDRD = ~USBMASK;   /* all outputs except USB data */
	PORTB = (1<<4)|(1<<3);	/* RESET=High, PB3 LED(PWR) ON, PB2 LED(ACC) OFF */
    DDRB = ~(1<<5);    /* all outputs except USI input data */
	USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK);
#endif

#if	0
//	USBの配線が D+ = PD2, D- = PD3 (AVRUSBのサンプル回路図)の場合.
//	PORTD = 0x00;
    DDRD = ~USBMASK;   /* all outputs except USB data */
//	PORTB = 0x08;      /* no pullups on USB pins */ // LED off
    DDRB = ~(1<<5);    /* all outputs except USI input data */
#endif

#if	0
//	USB D+ D- の配線が瓶詰堂さん旧オリジナル2007-3.
	DDRD = ~(1 << 2);
	DDRB = ~(USBMASK|(1<<5));    /* all outputs except USB data */
#endif

	hidStatus = -1;
    usbInit();
    sei();
    for(;;){    /* main event loop */
        usbPoll();
    }
    return 0;
}
