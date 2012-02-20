/* HIDaspx main.c */
/* 高速化の改良は、irukaさんが行いました
   2008年9月12日〜9月22日
   2008年9月22日 テスト公開開始
   2008年10月19日 コードの見直し（CleanUpを実施）
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usbdrv.h"
#include "hidcmd.h"

#define OPTIMIZE_SIZE		1	// r4,r5,r6,r7,r8を global変数として使用する.
								// usbdrv側で同レジスタが使用されていないことが条件.

#include "hidconfig.h"			// usbconfig.h からもincludeするため分離

//	REPORT_ID.
#define ID1    1
#define ID2    2
#define ID3    3
#define ID4    4

//	REPORT_COUNT+2の値.
#define	LENGTH1  8
#define	LENGTH2 32
#define	LENGTH3 40
#define	LENGTH4  6

#ifndef USE_LIBUSB
/*

 「重要」
 usbHidReportDescriptor[] の内容を修正した時には、usbconfig.h の
 以下の行に、usbHidReportDescriptor[] の占めるサイズを正確に設定すること

 #define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH    42

*/
PROGMEM char usbHidReportDescriptor[] = {
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

#if INCLUDE_POLL_CMD
    0x85, 0x04,                    //   REPORT_ID (4)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
#endif

    0xc0                           // END_COLLECTION
};
#endif

/* Note: REPORT_COUNT does not include report-ID byte */

#if	INCLUDE_FUSION
#define	DEV_ID				0x55	// FUSIONありのファーム.
#else
#define	DEV_ID				0x5a	// FUSIONなしのファーム.
#endif

//
//	受信バッファ.
//
#if OPTIMIZE_SIZE
register uchar currentPosition asm("r4");
register uchar bytesRemaining asm("r5");
register uchar page_mode   asm("r6");
register uchar page_addr   asm("r7");
register uchar page_addr_h asm("r8");
register uint8_t wait      asm("r9");
#else
static uchar currentPosition;
static uchar bytesRemaining; // Receive Data Pointer
static uchar page_mode;
static uchar page_addr;
static uchar page_addr_h;
static uint8_t wait;
#endif

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

static void cmd_peek_poke(MonCommand_t *cmd,uchar data0)
{
	uchar *p =cmd->addr;
	if(data0 == HIDASP_PEEK) {
		//メモリー連続読み出し.
		uchar cnt=cmd->count;
		uchar i;
		for(i=0;i<cnt;i++) {
			usbData[i]= *p++;
		}
	}else{
		//メモリー書き込み.(１バイト)
		uchar data=cmd->memdata[0];
		uchar mask=cmd->memdata[1];
		if(mask) {	//マスク付の書き込み.
			*p = (*p & mask) | data;
		}else{			//べた書き込み.
			*p = data;
		}
	}
}

#endif



// 最適化用
#define hbyte(a) (*((uchar*)&(a)+1))
#define lbyte(a) (*((uchar*)&(a)))
inline static uint8_t byte(uint8_t t) {return t;}

// 遅延用.
#define	DLY_2clk()	asm("rjmp .+0");

/* アセンブリソース delay.S で実現 */
void delay_10us(uchar d);
void delay_7clk(void);

//	PORTB PIN Setting
#define	ISP_DDR		DDRB
#define	ISP_OUT		PORTB
#define	ISP_DDR_DEFAULT	0x0f	/* PB7-4=in PB3-0=out */
#define	ISP_OUT_DEFAULT	0x0b	/* PB7-4=0, ISP_RDY=0 */

//	PORTB PIN ASSIGN
#define	ISP_SCK		7			/* Target SCK */
#define	ISP_MOSI	6			/* Target MISO */
#define	ISP_MISO	5			/* Target MOSI */
#define	ISP_RST		4			/* Target RESET */
#define	ISP_RDY		3			/* Green LED */
#define	ISP_RED		2			/* RED LED */


/* ------------------------------------------------------------------------- */
/* -----------------------------  USI Transfer  ---------------------------- */
/* ------------------------------------------------------------------------- */


//
//	wait:
//		0 =  3clk    2 MHz
//		1 =  4clk  1.5 MHz
//		2 =  9clk  666 kHz
//		3 = 21clk  285 kHz
//		4 = 33clk  181 kHz
//	   10 =129clk   46 kHz
//	   20 =249clk   23 kHz
//	   50 =609clk  9.8 kHz
//
//		2以上は 9 + (12 * wait) clk （CALL-RETを採用した場合も同様）
//
static uint8_t usi_trans(uint8_t data){
	USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK);	//by kuga
	USIDR=data;
	USISR=(1<<USIOIF);
	if(wait==0) {
#if 1	// by kuga
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
#else
		uchar CR0=(1<<USIWM0)|(1<<USICS1)|(1<<USITC);
		USICR=CR0;
		uchar CR1=(1<<USIWM0)|(1<<USICS1)|(1<<USITC)|(1<<USICLK);
		//↑これは1clkなので↓直後に nop必要.
					asm("nop");	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;//DLY_2clk(); :else のrjmpで代用.
#endif
	}else if(wait==1) {
		do{
			USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
		} while(!(USISR&(1<<USIOIF)));
	}else{
		do {
			uchar d=wait;		// 12clk * (wait-2)
			while(d != 2) {		// 1+1: cpi,breq
				delay_7clk();	// 3+4: call,ret, code 削減のため、call-ret を利用
				d--;			// 1+2: subi,rjmp
			}
			USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
		} while(!(USISR&(1<<USIOIF)));
	}
	USICR=0;		/* SCKをポートに戻しておく by kuga */
	return USIDR;
}

static inline void isp_command(uint8_t *data){
	uchar i;
	for (i=0;i<4;i++) {
		usbData[i]=usi_trans(data[i]);
	}
}

#if INCLUDE_ISP_CMD

static	void ispConnect(void)
{
	/* all ISP pins are inputs before */
	/* now set output pins */
	ISP_DDR = (1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI) | ISP_DDR_DEFAULT;

	/* reset device */
	ISP_OUT &= ~(1 << ISP_RST);		/* RST low */
	ISP_OUT &= ~(1 << ISP_SCK);		/* SCK low */

	/* positive reset pulse > 2 SCK (target) */
	delay_10us(6);					//@@x  ispDelay();
	ISP_OUT |= (1 << ISP_RST);		/* RST high */
	delay_10us(100);				//@@x  ispDelay(); -> 1ms
#if INCLUDE_LED_NON
	ISP_OUT &= ~((1 << ISP_RST)|(1 << ISP_RDY));	/* RST low , LED Low*/
#else
	ISP_OUT &= ~((1 << ISP_RST));	/* RST low */
#endif
}

static	void ispDisconnect(void)
{
	/* set all ISP pins inputs */
	//ISP_DDR &= ~((1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI));
	ISP_DDR = ISP_DDR_DEFAULT;		/* PB7-4=in PB3-0=out */

	/* switch pullups off */
	//ISP_OUT &= ~((1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI));
#if INCLUDE_LED_NON
	ISP_OUT = 0b00001011;			// PB7-4=Hi-Z,  PB3-0=Hi, LED(PWR)を除く
#else
	ISP_OUT = 0b00001111;			// PB7-4=Hi-Z,  PB3-0=Hi
#endif
}

static	void ispSckPulse(void)
{
	ISP_OUT |= (1 << ISP_SCK);		/* SCK high */
	delay_10us(100);				//@@x  ispDelay(); -> 1ms
	ISP_OUT &= ~(1 << ISP_SCK);		/* SCK Low */
}
#endif


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
#if	INCLUDE_ISP_CMD
	if ( data0 == HIDASP_SET_STATUS ) { // PORTへの出力制御
#if	0	/* コード節約のため、PORTDの処理をカットする */
		/* MONITOR機能が使える場合には、それで代用できる */
		PORTD = (PORTD&~3)    | (data1 & 3);
#endif
		/* ISP用のピンをHi-Z制御 */
		/* ISP移行を完全に行うためのコードを、ファーム側で持つ */
		if(data[2] & 0x10) {// RST解除の場合
			ispDisconnect();
		}else{
			if(data[2] & 0x80) {// RST状態でSCK Hは SCKパルス要求
				ispSckPulse();
			} else {
				ispConnect();
			}
		}
#if !INCLUDE_LED_NON
		// ISP_OUT 3,2,1,0のみをOR出力
		ISP_OUT |= (data[2]&0xf) ;
#endif	/* INCLUDE_LED_NON */
		usbData[0] = 0xaa;	/* コマンド実行完了をHOSTに知らせる. */
	} else
#endif	/* INCLUDE_ISP_CMD */
	if ( cmd == HIDASP_CMD_TX) { // SPI
		isp_command(data+1);
	} else if ( data0 == HIDASP_SET_PAGE ) { // Page set
		page_mode = data1;
#if	INCLUDE_POLL_CMD 	// ReportID:4  POLLING PORTを実装する.
		page_addr = data[2];
		page_addr_h = data[3];
#else
		page_addr = 0;
		page_addr_h = 0;
#endif	/* INCLUDE_POLL_CMD */
	}
#if	INCLUDE_FUSION
	else if (cmdtx == HIDASP_PAGE_TX ) { // Page buf
#if FLOW_CONTROL
        usbDisableAllRequests();
#endif /* FLOW_CONTROL */
		//
		//	page_write開始時にpage_addrをdata[1]で初期化.
		//
		if(data0 & (HIDASP_PAGE_TX_START & MODE_MASK)) {
			page_mode = 0x40;
			page_addr = 0;
			page_addr_h = 0;
		}
		//
		//	page_write (またはpage_read) の実行.
		//
		for(i=0;i<data1;i++) {
			usi_trans(page_mode);
			usi_trans(page_addr_h);
			usi_trans(page_addr);
			usbData[i]=usi_trans(data[i+2]);
			if (page_mode & 0x88) { // EEPROM or FlashH
				page_addr++;
				if(page_addr==0) {page_addr_h++;}
				page_mode&=~0x08;
			} else {
				page_mode|=0x08;
			}
		}
		//
		//	isp_command(Flash書き込み)の実行.
		//
		if(data0 & (HIDASP_PAGE_TX_FLUSH & MODE_MASK)) {
			isp_command(data+i+2);
		}
#if FLOW_CONTROL
        usbEnableAllRequests();
#endif /* FLOW_CONTROL */
	}
#else	/* not INCLUDE_FUSION */
	else if ( cmd == HIDASP_PAGE_TX ) { // Page buf
		for(i=0;i<data1;i++) {
			usi_trans(page_mode);
			usi_trans(page_addr_h);
			usi_trans(page_addr);
			usbData[i]=usi_trans(data[i+2]);
			if (page_mode & 0x88) { // EEPROM or FlashH
				page_addr++;
				if(page_addr==0) {page_addr_h++;}
				page_mode&=~0x08;
			} else {
				page_mode|=0x08;
			}
		}
	}
#endif	/* INCLUDE_FUSION */
	else if ( data0 == HIDASP_SET_DELAY ) { // Set wait
		wait=data1;
	}
#if	INCLUDE_MONITOR_CMD
	else if ( cmd == HIDASP_POKE ) {
		cmd_peek_poke((MonCommand_t *)data,data0);
	}
#endif /* INCLUDE_MONITOR_CMD */
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
			report.id[0] = rq->wValue.bytes[0];    /* store report ID */
			usbMsgPtr = report.id;
#if	INCLUDE_POLL_CMD 	// ReportID:4  POLLING PORTを実装する.
			if(	report.id[0]==ID4) {
				report.buf[0] = *( (uchar *)page_addr );
			}
#endif
			return rq->wLength.word;
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
RESET    [1  |__| 20] Vcc
PD0(NC)  [2       19] PB7(SCK)
PD1(NC)  [3       18] PB6(MISO)
XTAL2    [4       17] PB5(MOSI)
XTAL1    [5       16] PB4(RST)
PD2(12M) [6       15] PB3(BUSY LED)
PD3      [7       14] PB2(READY LED)
PD4      [8       13] PB1(NC)
PD5(PUP) [9       12] PB0(NC)
GND      [10      11] PD6(NC)
         ~~~~~~~~~~~

   ---------------------------------------
   SPI:     PB7-4 ===> [Target AVR Device](MISOとMOSIは交差)
   USB:     PD4   ===> USB D-
            PD3   ===> USB D+
            PD5   ===> USB D- PULL UP
   XTAL:    XTAL1,2 => Crystal 12MHz
   PD2:     Clock Output(12MHz)
   ---------------------------------------
*/

int main(void)
{
	PORTD |= (1<<PD5);		/* PD5 USB D- pullup */
	DDRD = ~USBMASK;        /* all outputs except USB data */
	ISP_OUT = ISP_OUT_DEFAULT;			/* PB3 LED(PWR) ON, PB2 LED(ACC) OFF */
	ISP_DDR = ISP_DDR_DEFAULT;	/* PB7-4=in PB3-0=out */
    usbInit();
    sei();
    for(;;){    /* main event loop */
        usbPoll();
    }
    return 0;
}
