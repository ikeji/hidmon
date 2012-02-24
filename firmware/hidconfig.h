/* ------------------------------------------------------------------------- */
/* hidconfig.h */
/* 2008-10-18 ... main.c ���番��(by senshu) */
/* 2008-10-28 ... STARTUP_DELAY, USB_IO_MODE_ENABLE��ǉ�
 */

#ifndef __hidconfig_h_included__
#define __hidconfig_h_included__

// Add by ikeji
#define PROGRAMMER_MODE_SWITCH 0  // Switch to programmer mode by PIND2
#define ENABLE_ISP_LED 0  // Disable LED on PINB2 and PINB3

//	�R���t�B�M�����[�V�����X�C�b�`:
// USE_LIBUSB �� Makefile ����w�肷��ƃ\�[�X�ύX���s�v�B
//#define	USE_LIBUSB			0	// HID Class �ł͂Ȃ� libusb ���g�p����(84�o�C�g�̐ߖ�)

// Add by senshu
#define	STARTUP_DELAY		1	// ���Z�b�g����300m�b�̑҂�������(14�o�C�g�̑���)
#define	USB_IO_MODE_ENABLE	1	// ���Z�b�g���APORTB����̓��[�h�ɂ���(12�o�C�g�̑���)
#define	FLOW_CONTROL		1	// FLOW_CONTROL�́AFUSION�L�����̂ݎg�p����(12�o�C�g�̑���)

// Add by iruka
#define	INCLUDE_FUSION		1	// �Z�����߂�����.(76�o�C�g�̑���)
#define	INCLUDE_POLL_CMD 	1	// ReportID:4  POLLING PORT����������.(32�o�C�g�̑���)

// �����́A�K���w�肷�邱��
#define	INCLUDE_MONITOR_CMD 1	// 62:POKE(),63:PEEK()����������.
#define	INCLUDE_ISP_CMD 	1	// 02:SET_STATUS()��ISP�ڍs���[�h����������.

/* ATS90���T�|�[�g���邽�߂ɂ́AINCLUDE_ISP_CMD�̎w��͕K�{
 *
 *	ISP_CMD / MONITOR_CMD �́A���Ȃ��Ƃ��ǂ��炩�͕K�v�ł����A
 *	HIDmon���g�p���Ȃ��ꍇ�� MONITOR_CMD �͕s�v�ł�.
 *  AT90S���T�|�[�g����ɂ́AISP_CMD�̗L�����͕K�{�ł��B
 */

//	FUSION��Off�ɂ����ꍇ�́A�������[�ɗ]�T���o����̂Œǉ��@�\�̍쐬�ɕ֗�.
//	FUSION��Off�ɂ��Ă��A���C�^�[�\�t�g���������ʂ��ċ��Ō݊��œ��삵�܂�.

//	POLL_CMD �̓��C�^�[�\�t�g�ł͎g�p���܂���. HIDmon��p�ł�.
//  �����_�ł́A�S�Ă̋@�\��L���ɂ��Ă��A2048�Ɏ��܂�܂��B

/* ------------------------------------------------------------------------- */

#endif /* __hidconfig_h_included__ */
