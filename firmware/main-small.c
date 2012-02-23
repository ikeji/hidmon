/*
 *	*******************************************************************
 *						HIDmon��p�t�@�[���E�F�A.
 *
 *	ATtiny2313�ł̎g�p�������[�ʂ��ŏ��ł��B
 *  	text    data     bss     dec     hex filename
 *      1598       2      52    1652     674 main-small.elf
 *	���R�Ɋg�����Ďg�p�ł��܂��B
 *
 *	�����R�}���h��
 *		HIDASP_TEST
 *		HIDASP_PEEK
 *		HIDASP_POKE
 *		HIDASP_SET_PAGE
 *	��4�ł��B
 *
 *	ReportID=4 (4byte) �̓ǂݎ��ɑ΂��ẮA�\�ߎw�肳�ꂽ�|�[�g
 *	(page_addr) �̓��e��ǂݏo���āA�����������܂�.
 *
 *	*******************************************************************
 *
 *	AVR���C�^�[�̋@�\�͂���܂���B���C�^�[�@�\�ƌ��p�Ŏg�p����ꍇ�́A
 *  main.c���g����Make���Ă��������B
 *
 * 2008-11-21 ATtiny2313��RC���U��œ�����m�F���܂����B
 * F_CPU=12800000 �� make���Ă��������B
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#if	F_CPU == 12800000	/* RC OSC mode */
char lastTimer0Value;
#endif

#include "usbdrv.h"
#include "hidcmd.h"

#define OPTIMIZE_SIZE		1	// r4,r5,r6,r7�� global�ϐ��Ƃ��Ďg�p����.
								// usbdrv���œ����W�X�^���g�p����Ă��Ȃ����Ƃ�����.
/* ------------------------------------------------------------------------- */

//	REPORT_ID.
#define ID1    1
#define ID2    2
#define ID3    3
#define ID4    4

//	REPORT_COUNT+2�̒l.
#define	LENGTH1  8
#define	LENGTH2 32
#define	LENGTH3 40
#define	LENGTH4  6

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

#define	DEV_ID				0x5a	// FUSION�Ȃ��̃t�@�[��.

//
//	��M�o�b�t�@.
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
//	���M�o�b�t�@.
//
typedef struct {
	uchar id[1];
	uchar buf[8];		// ReportID=1(LENGTH=7)�ȊO�ł́A���̃o�b�t�@��
						// �㑱30�o�C�g���x���z�X�g�ɑ��M����邱�Ƃ�����.
						// (�擪��9�o�C�g�݂̂��L���ɂȂ�)
} ReportBuf_t;

static ReportBuf_t report;

// ���M�o�b�t�@�͎�M�o�b�t�@�𗬗p����.
//	�A���A�擪1�o�C�g��Report ID��u���K�v������̂�,1�o�C�g�����.
#define	usbData report.buf
#define	buffer  report.id

//	�������[��ǂݏ�������R�}���h�\��.
typedef struct {
	uchar hidasp_cmd;	// 62=poke 63=peek
	uchar count;		// �ǂݍ��݃o�C�g��.
	uchar *addr;		// �ǂݏ����A�h���X.
	uchar memdata[16];	// �ǂݍ��݃f�[�^. �������͏������݃f�[�^[0] �}�X�N[1]
} MonCommand_t;

//�������[�A���ǂݏo��.
static void cmd_peek(MonCommand_t *cmd)
{
	uchar *p =cmd->addr;
	uchar cnt=cmd->count;
	uchar i;
	for(i=0;i<cnt;i++) {
		usbData[i]= *p++;
	}
}
//�������[��������.(1�o�C�g)
static void cmd_poke(MonCommand_t *cmd)
{
	uchar *p =cmd->addr;
	uchar data=cmd->memdata[0];
	uchar mask=cmd->memdata[1];
	if(mask) {	//�}�X�N�t�̏�������.
		*p = (*p & mask) | data;
	}else{			//�ׂ���������.
		*p = data;
	}
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- execute Buffer ---------------------------- */
/* ------------------------------------------------------------------------- */
//
//		monit�̃��C������.
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

// �f�t�H���g�ȊO�� usb setup �R�}���h�p�P�b�g�������ŉ��߂���.
uchar usbFunctionSetup(uchar data[8])
{
	usbRequest_t	*rq = (void *)data;
	uchar			rqType = rq->bmRequestType & USBRQ_TYPE_MASK;
	uchar			bRequest = rq->bRequest;
    if(rqType == USBRQ_TYPE_CLASS){    /* class request type */
        if(	bRequest == USBRQ_HID_GET_REPORT ) {
			report.id[0] = rq->wValue.bytes[0];    /* store report ID */
			usbMsgPtr = report.id;
		 	// ReportID:4  POLLING PORT����������.
			if(report.id[0]==ID4) {
				uchar *port   = (uchar *) ((int)page_addr);
				report.buf[0] = *port;
			}
			return rq->wLength.word;
		}
        if(	bRequest == USBRQ_HID_SET_REPORT ) {
			currentPosition = 0;                // initialize position index
        	bytesRemaining = rq->wLength.word;  // store the amount of data requested
          	return 0xff;						// tell driver to use usbFunctionWrite()
        }
    }
    return 0;
}


// �R���g���[���]�����㑱�f�[�^�������Ă���ꍇ�A�����8�o�C�g�P�ʂŎ󂯎��.
// 	�E���ʂ�report�\���̂ɓ����.
//	�E�S���󂯎������Amonit_main()���Ăяo��.
uchar usbFunctionWrite(uchar *data, uchar len)
{
    if(	len > bytesRemaining) {             // if this is the last incomplete chunk
        len = bytesRemaining;               // limit to the amount we can store
	}
    bytesRemaining -= len;

	if(	currentPosition==0 ) {				// �ŏ��̌㑱�f�[�^���󂯎������A���ߎ��s.
		monit_main(data+1);
	}										// 8byte�ȍ~�̃f�[�^�͂Ƃ肠�������������.

	currentPosition += len;
	// �S���󂯎����.
    if( bytesRemaining == 0 ) {
	    return 1;	// return 1 if we have all data
	}
	return 0;		// continue data.
}

/*
 * 	��H�}�́AHIDasp(x)�AHIDsph�̂��̂Ƌ��ʂł��B
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
   SPI:     PB7-4 ===> [Target AVR Device](MISO��MOSI�͌���)
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
#if	STARTUP_DELAY
	/* Power ON delay (300ms) */
	for (wait=0; wait<150; wait++) {
		delay_10us(200);		// 2ms
	}
#endif

#if F_CPU == 12800000	/* RC OSC mode, (D-) */
	TCCR0B = 3;          /* 1/64 prescaler */
	OSCCAL=118;
#endif

#if 1
	/* PD5�� USB D-, PD2��mode�W�����p�p pullup */
	/* �Ȃ��APD2�̓N���b�N�o�͂𖳌��ɂ������ɓ��̓|�[�g�ɂȂ�܂� */
	PORTD |= ((1<<PD6)|(1<<PD5)|(1<<PD2)|(1<<PD1)|(1<<PD0));
	/* all outputs except (USB data, PD2, PD1, PD0) */
	DDRD = ~(USBMASK | (1<<PD6)| (1<<PD2)|(1<<PD1)|(1<<PD0));
#else
	DDRD = ~USBMASK;        /* all outputs except USB data */
#endif
	PORTB = 0xff;			/* All pullup */
	DDRB = 0;				/* PB7-0=in */

    usbInit();
    sei();
    for(;;){    /* main event loop */
        usbPoll();
    }
    return 0;
}
