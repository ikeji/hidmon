/* ------------------------------------------------------------------------- */
/* hidconfig.h */
/* 2008-10-18 main.c ���番��(by senshu) */

#ifndef __hidconfig_h_included__
#define __hidconfig_h_included__

//	�R���t�B�M�����[�V�����X�C�b�`:
// USE_LIBUSB �� Makefile ����w�肷��ƃ\�[�X�ύX���s�v�ŕ֗��B
//#define	USE_LIBUSB			0	// HID Class �ł͂Ȃ� libusb ���g�p����

#define	INCLUDE_FUSION		1	// �Z�����߂�����.
#define	FLOW_CONTROL		1	// FLOW_CONTROL�́AFUSION�L�����̂ݎg�p����
#define	INCLUDE_POLL_CMD 	1	// ReportID:4  POLLING PORT����������.

#define	INCLUDE_MONITOR_CMD 1	// 62:POKE(),63:PEEK()����������.
#define	INCLUDE_ISP_CMD 	1	// 02:SET_STATUS()��ISP�ڍs���[�h����������. 
#define	INCLUDE_LED_NON 	1	// 02:SET_STATUS()��LED�w��𖳌�(�L��=>0)
/* ATS90���T�|�[�g���邽�߂ɂ́AINCLUDE_ISP_CMD�̎w��͕K�{
 *
 *	ISP_CMD / MONITOR_CMD �́A���Ȃ��Ƃ��ǂ��炩�͕K�v�ł����A
 *	HIDmon���g�p���Ȃ��ꍇ�� MONITOR_CMD �͕s�v�ł�.
 *  AT90S���T�|�[�g����ɂ́AISP_CMD�̗L�����͕K�{�ł��B
 */

//	FUSION��Off�ɂ����ꍇ�́A�������[�ɗ]�T���o����̂Œǉ��@�\�̍쐬�ɕ֗�.
//	FUSION��Off�ɂ��Ă��A���C�^�[�\�t�g���������ʂ��ċ��Ō݊��œ��삵�܂�.

//	POLL_CMD �̓��C�^�[�\�t�g�ł͎g�p���܂���. HIDmon��p�ł�.
//  �����_�ł́A�S�Ă̋@�\��L���ɂ��Ă��A2048�Ɏ��܂�܂�

/* ------------------------------------------------------------------------- */

#endif /* __hidconfig_h_included__ */
