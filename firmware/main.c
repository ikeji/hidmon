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
#include "hidcmd.h"

/* ------------------------------------------------------------------------- */
//	コンフィギュレーションスイッチ:
#define	INCLUDE_FUSION		1	// 融合命令を実装.
#define	INCLUDE_MONITOR_CMD 1	// 62:POKE(),63:PEEK()を実装する.
#define	INCLUDE_LED_CMD 	0	// 02:SET_STATUS()を実装する.
//	↑ LED_CMDをONにしない場合はMONITOR_CMDでPin制御する必要があります.
//	   その場合ライターソフトが自動判別してMONITOR_CMDを発行します.

//	↑ メモリー容量の関係で、３つ全部Onにすることはできません.
//
//	FUSIONをOffにした場合は、メモリーに余裕が出来るので追加機能の作成に便利.
//	FUSIONをOffにしても、ライターソフトが自動判別して旧版互換で動作します.

/* ------------------------------------------------------------------------- */


//	REPORT_ID.
#define ID1    1
#define ID2    2
#define ID3    3

//	REPORT_COUNT+2の値.
#define	LENGTH1  8
#define	LENGTH2 32
#define	LENGTH3 40

PROGMEM char usbHidReportDescriptor[42] = {
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
    0x95, 0x1e,                    //   REPORT_COUNT (30)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, 0x03,                    //   REPORT_ID (3)
    0x95, 0x26,                    //   REPORT_COUNT (38)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0xc0                           // END_COLLECTION
};
/* Note: REPORT_COUNT does not include report-ID byte */

#if	INCLUDE_FUSION
#define	DEV_ID				0x55	// FUSIONありのファーム.
#else
#define	DEV_ID				0x5a	// FUSIONなしのファーム.
#endif

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
	uchar cnt=cmd->count;
	uchar *p =cmd->addr;
	for(i=0;i<cnt;i++) {
		usbData[i]=*p++;
	}
}
#endif



// 最適化用
#define hbyte(a) (*((uchar*)&(a)+1))
#define lbyte(a) (*((uchar*)&(a)))
inline static uint8_t byte(uint8_t t) {return t;}

/* ------------------------------------------------------------------------- */
/* -----------------------------  USI Transfer  ---------------------------- */
/* ------------------------------------------------------------------------- */

static uint8_t wait=60; // 160

//
//	wait:
//		0 =  2clk    3 MHz
//		1 =  4clk  1.5 MHz
//		2 =  9clk  666 kHz
//		3 = 21clk  285 kHz
//		4 = 33clk  181 kHz
//	   10 =129clk   46 kHz
//	   20 =249clk   23 kHz
//	   50 =609clk  9.8 kHz
//
//		2以上は 9 + (12 * wait) clk
//
static uint8_t usi_trans(uint8_t data){
	USIDR=data;
	USISR=(1<<USIOIF);
	if(wait==0) {
		uchar CR0=(1<<USIWM0)|(1<<USICS1)|(1<<USITC);
		USICR=CR0;
		uchar CR1=(1<<USIWM0)|(1<<USICS1)|(1<<USITC)|(1<<USICLK);
								USICR=CR1;	asm("nop");
		USICR=CR0;	asm("nop");	USICR=CR1;	asm("nop");
		USICR=CR0;	asm("nop");	USICR=CR1;	asm("nop");
		USICR=CR0;	asm("nop");	USICR=CR1;	asm("nop");
		USICR=CR0;	asm("nop");	USICR=CR1;	asm("nop");
		USICR=CR0;	asm("nop");	USICR=CR1;	asm("nop");
		USICR=CR0;	asm("nop");	USICR=CR1;	asm("nop");
		USICR=CR0;	asm("nop");	USICR=CR1;
	}else if(wait==1) {
		do{
			USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
		} while(!(USISR&(1<<USIOIF)));
	}else{
		uchar d=wait;		// 12clk * (wait-2)
		do {
			while(d != 2) {		// 1 loop = 12clk
				asm("rjmp .+0");
				asm("rjmp .+0");
				asm("rjmp .+0");
				asm("rjmp .+0");
				d--;
			}
			USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
		} while(!(USISR&(1<<USIOIF)));
	}
	return USIDR;
}

static inline void isp_command(uint8_t *data){
	uchar i;
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

void hidasp_main()	//uchar *data)
{
	// 本来なら引数.
	uchar *data = report.buf;	//こうすると縮む.

	static uchar    page_mode;
	static uint16_t page_addr;
	uchar i;
	uint8_t data0 = data[0];
	uint8_t data1 = data[1];
	uint8_t cmd   = data0 & 0xfe;
	uint8_t cmdtx = data0 & CMD_MASK;

#if 1
	usbData[0] = DEV_ID;
#else
	if ( data0 == HIDASP_TEST ) { // TEST
		usbData[0] = DEV_ID;
	}else
#endif
#if	INCLUDE_LED_CMD
	if ( data0 == HIDASP_SET_STATUS ) { // SET_LED
		PORTD = (PORTD&~3)    | (data1 & 3);
//		PORTB = (PORTB&~0x10) | (data[2]&0x1F);
		// RESETピンを保持、それ以外をOR出力
		PORTB = (PORTB&~0x1F) | (data[2]&0x1f) ;
		usbData[0] = 0xaa;
	} else
#endif
	if ( cmd == HIDASP_CMD_TX) { // SPI
		isp_command(data+1);
	} else if ( data0 == HIDASP_SET_PAGE ) { // Page set
		page_mode = data1;
		page_addr = 0;
	}
#if	INCLUDE_FUSION
	else if (cmdtx == HIDASP_PAGE_TX ) { // Page buf
		//
		//	page_write開始時にpage_addrをdata[1]で初期化.
		//
		if(data0 & (HIDASP_PAGE_TX_START & MODE_MASK)) {
			page_mode = 0x40;
			page_addr = 0;
		}
		data+=2;
		//
		//	page_write (またはpage_read) の実行.
		//
		for(i=0;i<data1;i++) {
			usi_trans(page_mode);
			usi_trans(hbyte(page_addr));
			usi_trans(lbyte(page_addr));
			usbData[i]=usi_trans(*data++);
			if (page_mode & 0x88) { // EEPROM or FlashH
				page_addr++;
				page_mode&=~0x08;
			} else {
				page_mode|=0x08;
			}
		}
		//
		//	isp_command(Flash書き込み)の実行.
		//
		if(data0 & (HIDASP_PAGE_TX_FLUSH & MODE_MASK)) {
			isp_command(data);
		}
	}
#else
	else if ( cmd == HIDASP_PAGE_TX ) { // Page buf
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
	}
#endif
	else if ( data0 == HIDASP_SET_DELAY ) { // Set wait
		wait=data1;
	}
#if	INCLUDE_MONITOR_CMD
	else if ( data0 == HIDASP_POKE ) {
		cmd_poke((MonCommand_t *)data);
	} else if ( data0 == HIDASP_PEEK ) {
		cmd_peek((MonCommand_t *)data);
	}
#endif
}


/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

// デフォルト以外の usb setup コマンドパケットをここで解釈する.
uchar usbFunctionSetup(uchar data[8])
{
	usbRequest_t	*rq = (void *)data;
	uchar			rqType = rq->bmRequestType & USBRQ_TYPE_MASK;
	uchar			bRequest = rq->bRequest;
    if(rqType == USBRQ_TYPE_CLASS){    /* class request type */
        if(	bRequest == USBRQ_HID_GET_REPORT ) {
#if	1
			report.id[0] = rq->wValue.bytes[0];    /* store report ID */
			usbMsgPtr = report.id;
			return rq->wLength.word;
#else
			report.id[0] = ID3;    /* store report ID */
			usbMsgPtr = report.id;
			return LENGTH3;
#endif
		}
        if(	bRequest == USBRQ_HID_SET_REPORT ) {
		    currentPosition = 0;                // initialize position index
        	bytesRemaining = rq->wLength.word;  // store the amount of data requested
#if	0
        	if(	bytesRemaining > sizeof(report)) { // limit to buffer size
            	bytesRemaining = sizeof(report);
			}
#endif
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
        report.id[currentPosition++] = *data++;
	}
	// 全部受け取ったら、バッファ内容を実行.
    if( bytesRemaining == 0 ) {
		hidasp_main();		//(report.buf);
	    return 1;	// return 1 if we have all data
	}
	return 0;		// continue data.
}

/*
       ATtiny2313
         ___    ___
RESET   [1  |__| 20] Vcc
PD0(NC) [2       19] PB7(SCK)
PD1(NC) [3       18] PB6(MISO)
XTAL2   [4       17] PB5(MOSI)
XTAL1   [5       16] PB4(RST)
PD2(12M)[6       15] PB3(PWR LED)
PD3     [7       14] PB2(ACC LED)
PD4     [8       13] PB1(NC)
PD5(NC) [9       12] PB0(NC)
GND     [10      11] PD6(NC)
        ~~~~~~~~~~~

   ---------------------------------------
   SPI:     PB7-4 ===> [Target AVR Device]
   USB:     PD4   ===> USB D-
            PD3   ===> USB D+
   XTAL:    XTAL1,2 => Crystal 12MHz
   PD2:     Clock Output(12MHz)
   ---------------------------------------
*/

int main(void)
{
#if	1
//	USB D+ D- の配線が瓶詰堂さんの新しい回路図用 HIDsphとも互換なのでこれを推奨.
	DDRD = ~USBMASK;   /* all outputs except USB data */
	PORTB = (1<<4)|(1<<3);	/* RESET=High, PB3 LED(PWR) ON, PB2 LED(ACC) OFF */
	DDRB = ~(1<<5);    /* all outputs except USI input data */
	USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK);
#else
	//	未使用pinを入力設定にしておく.
	DDRD = 0; 	// Ｄ＋、Ｄ－を含め、PORTDをすべて入力モードにする。
	PORTB = (1<<4)|(1<<3);	/* RESET=High, PB3 LED(PWR) ON, PB2 LED(ACC) OFF */

	// PORTB[3,2,1,0]を入力モードにする.
	DDRB = 0xd0;	// 1101_0000 input = USI input data , PB3,2,1,0
	USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK);

	// ToDo:未接続pinにプルアップ設定が必要.
#endif

    usbInit();
    sei();
    for(;;){    /* main event loop */
        usbPoll();
    }
    return 0;
}
