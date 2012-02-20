/* rscr.c
 * Solving the chicken and egg problem. (denshiken)
 * http://homepage2.nifty.com/denshiken/AVW009.html
 *
 * 2006-06-04 by mutech, t.kuroki
 *
 *  RS232C DSUB-9P TXD(pin3) ---R_10k---*--- SCK  (AVR)
 *                                      |
 *                                     R_30k
 *                                      |
 *                                      *--- MOSI (AVR)
 *                                      |
 *                                   C_0.01uF
 *                                      |
 *                                     GND
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "avrspx.h"
//----------------------------------------------------------------------------
#define MAX_TMPBUF	1024

static HANDLE hComm = INVALID_HANDLE_VALUE;
static int baudrate;
static double delay_ticks;

static BYTE tmpbuf[MAX_TMPBUF];
//----------------------------------------------------------------------------
static char f_init;
static int tmpcnt;

void rscr_flush_buf(void)
{
	DWORD cnt;

	if (tmpcnt) {
		while ((tmpcnt & 3) != 0) {
			tmpbuf[tmpcnt++] = 0xff;
		}
#if 0
		{
			int n;
			for (n = 0; n < tmpcnt; ++n)
				fprintf(stdout, " %02X", tmpbuf[n]);
			fprintf(stdout, "\n");
		}
#endif
		WriteFile(hComm, tmpbuf, tmpcnt, &cnt, NULL);
		tmpcnt = 0;
	}
}

void rscr_send_byte(BYTE c)
{
	int n;

	n = 8;
	if (!f_init) {
		f_init = 1;
		c <<= 1;
		--n;
	}
//fprintf(stdout,"[%02X]", c);
	do {
		if (tmpcnt >= MAX_TMPBUF)
			rscr_flush_buf();
		tmpbuf[tmpcnt++] = (c & 0x80) ? 0x00 : 0xff;
		c <<= 1;
	} while (--n);
}

int rscr_sendbuff(void)
{
	int progress;
	int n, m;

	f_init = 0;
	tmpcnt = 0;
	progress = -1;
// ※データは1bitシフトした書き方になる
// 例:0x20なら0x40になり、さらに反転されて $FF,$00,$FF,$FF と表記
	for (n = 0; n < cmdbuf_count; ++n) {
		m = n * 50 / cmdbuf_count;
		if (progress != m) {
			progress = m;
			if ((progress % 5) == 0)
				fprintf(stderr, "%d", progress / 5);
			else
				fprintf(stderr, ".");
		}
		rscr_send_byte(cmdbuf[n]);
	}
	return 0;
}

//----------------------------------------------------------------------------
void rscr_wait(int cnt)
{
//fprintf(stderr, "  -> %d", cnt);
    do {
		wronly_universal_command(
			C_RD_PRGL,		// read Flash
			0,
			0,
			0);
	} while (--cnt > 0);
}

void rscr_delay_ms(WORD dly)
{
//fprintf(stderr, "delay_ms %d", dly);
    if (dly > 1)
    	rscr_wait((int)(dly / delay_ticks + 0.7));
}

//----------------------------------------------------------------------------
int rscr_setup(HANDLE h, int baud)
{
	wronly_programmer_init();
	hComm = h;
	baudrate = baud;
	delay_ticks = 10000.0 * 4 * 8 / baudrate;
	rscr_wait(1);
	return 0;
}

int rscr_commit(void)
{
	int n, cnt;

	rscr_wait(32);

	fprintf(stderr, "Progress : ");

	rscr_sendbuff();
	rscr_flush_buf();
	FlushFileBuffers(hComm);
// cp210xドライバでは FlushFileBuffersが効かない、またClose時にもCommitが効かない
	cnt = 10;
	do {
		n = get_outque_count(hComm);
//fprintf(stderr, "OutQue %d\n", n);
		Sleep(100);
	} while (n > 0 && --cnt > 0);

	fprintf(stderr, " done.\n");
	return 0;
}

void rscr_close(void)
{
// 20 00 00 00 read flash
// AC 53 00 00 enter isp mode
	if (cmdbuf_count > 8)
		rscr_commit();
	wronly_programmer_close();
}
//----------------------------------------------------------------------------
