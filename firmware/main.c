/* HIDaspx main.c */
/* �������̉��ǂ́Airuka���񂪍s���܂���
   2008�N9��12���`9��22��
   2008�N9��22�� �e�X�g���J�J�n
   2008�N10��19�� �R�[�h�̌������iCleanUp�����{�j
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usbdrv.h"
#include "hidcmd.h"

#define OPTIMIZE_SIZE		1	// r4,r5,r6,r7,r8�� global�ϐ��Ƃ��Ďg�p����.
								// usbdrv���œ����W�X�^���g�p����Ă��Ȃ����Ƃ�����.

#include "hidconfig.h"			// usbconfig.h �����include���邽�ߕ���

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

#ifndef USE_LIBUSB
/*

 �u�d�v�v
 usbHidReportDescriptor[] �̓��e���C���������ɂ́Ausbconfig.h ��
 �ȉ��̍s�ɁAusbHidReportDescriptor[] �̐�߂�T�C�Y�𐳊m�ɐݒ肷�邱��

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
#define	DEV_ID				0x55	// FUSION����̃t�@�[��.
#else
#define	DEV_ID				0x5a	// FUSION�Ȃ��̃t�@�[��.
#endif

//
//	��M�o�b�t�@.
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

// ���M�o�b�t�@�͎�M�o�b�t�@�𗬗p����.
//	�A���A�擪1�o�C�g��Report ID��u���K�v������̂�,1�o�C�g�����.
#define	usbData report.buf
#define	buffer  report.id

//
#if	INCLUDE_MONITOR_CMD

//	�������[��ǂݏ�������R�}���h�\��.
typedef struct {
	uchar hidasp_cmd;	// 62=poke 63=peek
	uchar count;		// �ǂݍ��݃o�C�g��.
	uchar *addr;		// �ǂݏ����A�h���X.
	uchar memdata[16];	// �ǂݍ��݃f�[�^. �������͏������݃f�[�^[0] �}�X�N[1]
} MonCommand_t;

static void cmd_peek_poke(MonCommand_t *cmd,uchar data0)
{
	uchar *p =cmd->addr;
	if(data0 == HIDASP_PEEK) {
		//�������[�A���ǂݏo��.
		uchar cnt=cmd->count;
		uchar i;
		for(i=0;i<cnt;i++) {
			usbData[i]= *p++;
		}
	}else{
		//�������[��������.(�P�o�C�g)
		uchar data=cmd->memdata[0];
		uchar mask=cmd->memdata[1];
		if(mask) {	//�}�X�N�t�̏�������.
			*p = (*p & mask) | data;
		}else{			//�ׂ���������.
			*p = data;
		}
	}
}

#endif



// �œK���p
#define hbyte(a) (*((uchar*)&(a)+1))
#define lbyte(a) (*((uchar*)&(a)))
inline static uint8_t byte(uint8_t t) {return t;}

// �x���p.
#define	DLY_2clk()	asm("rjmp .+0");

/* �A�Z���u���\�[�X delay.S �Ŏ��� */
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
//		2�ȏ�� 9 + (12 * wait) clk �iCALL-RET���̗p�����ꍇ�����l�j
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
		//�������1clk�Ȃ̂Ł������ nop�K�v.
					asm("nop");	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;//DLY_2clk(); :else ��rjmp�ő�p.
#endif
	}else if(wait==1) {
		do{
			USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
		} while(!(USISR&(1<<USIOIF)));
	}else{
		do {
			uchar d=wait;		// 12clk * (wait-2)
			while(d != 2) {		// 1+1: cpi,breq
				delay_7clk();	// 3+4: call,ret, code �팸�̂��߁Acall-ret �𗘗p
				d--;			// 1+2: subi,rjmp
			}
			USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
		} while(!(USISR&(1<<USIOIF)));
	}
	USICR=0;		/* SCK���|�[�g�ɖ߂��Ă��� by kuga */
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
	ISP_OUT = 0b00001011;			// PB7-4=Hi-Z,  PB3-0=Hi, LED(PWR)������
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
//		hidasp�̃��C������.
//
//	���M�o�b�t�@�Ǝ�M�o�b�t�@�����ʂȂ̂ŁA
//	�����ׂ������Ƃ̃f�[�^���g��Ȃ��悤�ɒ���.

void hidasp_main()	//uchar *data)
{
	// �{���Ȃ����.
	uchar *data = report.buf;	//��������Ək��.

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
	if ( data0 == HIDASP_SET_STATUS ) { // PORT�ւ̏o�͐���
#if	0	/* �R�[�h�ߖ�̂��߁APORTD�̏������J�b�g���� */
		/* MONITOR�@�\���g����ꍇ�ɂ́A����ő�p�ł��� */
		PORTD = (PORTD&~3)    | (data1 & 3);
#endif
		/* ISP�p�̃s����Hi-Z���� */
		/* ISP�ڍs�����S�ɍs�����߂̃R�[�h���A�t�@�[�����Ŏ��� */
		if(data[2] & 0x10) {// RST�����̏ꍇ
			ispDisconnect();
		}else{
			if(data[2] & 0x80) {// RST��Ԃ�SCK H�� SCK�p���X�v��
				ispSckPulse();
			} else {
				ispConnect();
			}
		}
#if !INCLUDE_LED_NON
		// ISP_OUT 3,2,1,0�݂̂�OR�o��
		ISP_OUT |= (data[2]&0xf) ;
#endif	/* INCLUDE_LED_NON */
		usbData[0] = 0xaa;	/* �R�}���h���s������HOST�ɒm�点��. */
	} else
#endif	/* INCLUDE_ISP_CMD */
	if ( cmd == HIDASP_CMD_TX) { // SPI
		isp_command(data+1);
	} else if ( data0 == HIDASP_SET_PAGE ) { // Page set
		page_mode = data1;
#if	INCLUDE_POLL_CMD 	// ReportID:4  POLLING PORT����������.
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
		//	page_write�J�n����page_addr��data[1]�ŏ�����.
		//
		if(data0 & (HIDASP_PAGE_TX_START & MODE_MASK)) {
			page_mode = 0x40;
			page_addr = 0;
			page_addr_h = 0;
		}
		//
		//	page_write (�܂���page_read) �̎��s.
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
		//	isp_command(Flash��������)�̎��s.
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
#if	INCLUDE_POLL_CMD 	// ReportID:4  POLLING PORT����������.
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


// �R���g���[���]�����㑱�f�[�^�������Ă���ꍇ�A�����8�o�C�g�P�ʂŎ󂯎��.
// 	�E���ʂ�report�\���̂ɓ����.
//	�E�S���󂯎������Ahidasp_main()���Ăяo��.
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
	// �S���󂯎������A�o�b�t�@���e�����s.
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
   SPI:     PB7-4 ===> [Target AVR Device](MISO��MOSI�͌���)
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
