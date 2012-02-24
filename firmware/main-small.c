/*
 *    *******************************************************************
 *                        HIDmon専用ファームウェア.
 *
 *    ATtiny2313での使用メモリー量が最小です。
 *      text    data     bss     dec     hex filename
 *      1598       2      52    1652     674 main-small.elf
 *    自由に拡張して使用できます。
 *
 *    実装コマンドは
 *        HIDASP_TEST
 *        HIDASP_PEEK
 *        HIDASP_POKE
 *        HIDASP_SET_PAGE
 *    の4つです。
 *
 *    ReportID=4 (4byte) の読み取りに対しては、予め指定されたポート
 *    (page_addr) の内容を読み出して、即時応答します.
 *
 *    *******************************************************************
 *
 *    AVRライターの機能はありません。ライター機能と兼用で使用する場合は、
 *  main.cを使ってMakeしてください。
 *
 * 2008-11-21 ATtiny2313のRC発振器で動作を確認しました。
 * F_CPU=12800000 で makeしてください。
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#if    F_CPU == 12800000    /* RC OSC mode */
char lastTimer0Value;
#endif

#include "usbdrv.h"
#include "hidcmd.h"

#define OPTIMIZE_SIZE        1    // r4,r5,r6,r7を global変数として使用する.
// usbdrv側で同レジスタが使用されていないことが条件.
/* ------------------------------------------------------------------------- */

//    REPORT_ID.
#define ID1    1
#define ID2    2
#define ID3    3
#define ID4    4

//    REPORT_COUNT+2の値.
#define    LENGTH1  8
#define    LENGTH2 32
#define    LENGTH3 40
#define    LENGTH4  6

PROGMEM char usbHidReportDescriptor[51] = {
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

  0x85, 0x04,                    //   REPORT_ID (4)
  0x95, 0x04,                    //   REPORT_COUNT (4)
  0x09, 0x00,                    //   USAGE (Undefined)
  0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

  0xc0                           // END_COLLECTION
};

/* Note: REPORT_COUNT does not include report-ID byte */

#define    DEV_ID                0x5a    // FUSIONなしのファーム.

//
//    受信バッファ.
//
#if OPTIMIZE_SIZE
register uchar currentPosition asm("r4");
register uchar bytesRemaining asm("r5");
register uchar page_mode   asm("r6");
register uchar page_addr   asm("r7");
register uchar wait      asm("r8");
#else
static uchar currentPosition;
static uchar bytesRemaining;
static uchar page_mode;
static uchar page_addr;
static uchar wait;
#endif


#define hbyte(a) (*((uchar*)&(a)+1))
#define lbyte(a) (*((uchar*)&(a)))

//
//    送信バッファ.
//
typedef struct {
  uchar id[1];
  uchar buf[8];        // ReportID=1(LENGTH=7)以外では、このバッファの
  // 後続30バイト程度がホストに送信されることがある.
  // (先頭の9バイトのみが有効になる)
} ReportBuf_t;

static ReportBuf_t report;

// 送信バッファは受信バッファを流用する.
//    但し、先頭1バイトはReport IDを置く必要があるので,1バイトずれる.
#define    usbData report.buf
#define    buffer  report.id

//    メモリーを読み書きするコマンド構造.
typedef struct {
  uchar hidasp_cmd;    // 62=poke 63=peek
  uchar count;        // 読み込みバイト数.
  uchar *addr;        // 読み書きアドレス.
  uchar memdata[16];    // 読み込みデータ. もしくは書き込みデータ[0] マスク[1]
} MonCommand_t;

//メモリー連続読み出し.
static void cmd_peek(MonCommand_t *cmd)
{
  uchar *p =cmd->addr;
  uchar cnt=cmd->count;
  uchar i;
  for(i=0;i<cnt;i++) {
    usbData[i]= *p++;
  }
}
//メモリー書き込み.(1バイト)
static void cmd_poke(MonCommand_t *cmd)
{
  uchar *p =cmd->addr;
  uchar data=cmd->memdata[0];
  uchar mask=cmd->memdata[1];
  if(mask) {    //マスク付の書き込み.
    *p = (*p & mask) | data;
  }else{            //べた書き込み.
    *p = data;
  }
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- execute Buffer ---------------------------- */
/* ------------------------------------------------------------------------- */
//
//        monitのメイン処理.
//

void monit_main(uchar *data)
{
  uchar cmd = data[0];

  if ( cmd == HIDASP_TEST ) {
    usbData[0] = DEV_ID;
    usbData[1] = data[1];
  }
  else if ( cmd == HIDASP_PEEK ) {
    cmd_peek((MonCommand_t *)data);
  }
  else if ( cmd == HIDASP_POKE ) {
    cmd_poke((MonCommand_t *)data);
  }
  else if ( cmd == HIDASP_SET_PAGE ) {
    page_mode = data[1];
    page_addr = data[2];
  }
}


/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

// デフォルト以外の usb setup コマンドパケットをここで解釈する.
uchar usbFunctionSetup(uchar data[8])
{
  usbRequest_t    *rq = (void *)data;
  uchar            rqType = rq->bmRequestType & USBRQ_TYPE_MASK;
  uchar            bRequest = rq->bRequest;
  if(rqType == USBRQ_TYPE_CLASS){    /* class request type */
    if(    bRequest == USBRQ_HID_GET_REPORT ) {
      report.id[0] = rq->wValue.bytes[0];    /* store report ID */
      usbMsgPtr = report.id;
      // ReportID:4  POLLING PORTを実装する.
      if(report.id[0]==ID4) {
        uchar *port   = (uchar *) ((int)page_addr);
        report.buf[0] = *port;
      }
      return rq->wLength.word;
    }
    if(    bRequest == USBRQ_HID_SET_REPORT ) {
      currentPosition = 0;                // initialize position index
      bytesRemaining = rq->wLength.word;  // store the amount of data requested
      return 0xff;                        // tell driver to use usbFunctionWrite()
    }
  }
  return 0;
}


// コントロール転送が後続データを持っている場合、それを8バイト単位で受け取る.
//     ・結果はreport構造体に入れる.
//    ・全部受け取ったら、monit_main()を呼び出す.
uchar usbFunctionWrite(uchar *data, uchar len)
{
  if(    len > bytesRemaining) {             // if this is the last incomplete chunk
    len = bytesRemaining;               // limit to the amount we can store
  }
  bytesRemaining -= len;

  if(    currentPosition==0 ) {                // 最初の後続データを受け取ったら、解釈実行.
    monit_main(data+1);
  }                                        // 8byte以降のデータはとりあえず無視される.

  currentPosition += len;
  // 全部受け取った.
  if( bytesRemaining == 0 ) {
    return 1;    // return 1 if we have all data
  }
  return 0;        // continue data.
}

/*
 *     回路図は、HIDasp(x)、HIDsphのものと共通です。
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

   ---------------------------------------
   USB:     PD4   ===> USB D+
            PD3   ===> USB D- (INT1)
            PD5   ===> USB D- PULL UP
   XTAL:    NONE  (RC-OSC) 12.8MHz
   PD2:     Clock Output(12MHz)
   ---------------------------------------

*/

int main(void)
{
#if    STARTUP_DELAY
  /* Power ON delay (300ms) */
  for (wait=0; wait<150; wait++) {
    delay_10us(200);        // 2ms
  }
#endif

#if F_CPU == 12800000    /* RC OSC mode, (D-) */
  TCCR0B = 3;          /* 1/64 prescaler */
  OSCCAL=118;
#endif

#if 1
  /* PD5は USB D-, PD2はmodeジャンパ用 pullup */
  /* なお、PD2はクロック出力を無効にした時に入力ポートになります */
  PORTD |= ((1<<PD6)|(1<<PD5)|(1<<PD2)|(1<<PD1)|(1<<PD0));
  /* all outputs except (USB data, PD2, PD1, PD0) */
  DDRD = ~(USBMASK | (1<<PD6)| (1<<PD2)|(1<<PD1)|(1<<PD0));
#else
  DDRD = ~USBMASK;        /* all outputs except USB data */
#endif
  PORTB = 0xff;            /* All pullup */
  DDRB = 0;                /* PB7-0=in */

  usbInit();
  sei();
  for(;;){    /* main event loop */
    usbPoll();
  }
  return 0;
}
