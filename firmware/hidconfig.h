/* ------------------------------------------------------------------------- */
/* hidconfig.h */
/* 2008-10-18 main.c から分離(by senshu) */

#ifndef __hidconfig_h_included__
#define __hidconfig_h_included__

//	コンフィギュレーションスイッチ:
// USE_LIBUSB は Makefile から指定するとソース変更が不要で便利。
//#define	USE_LIBUSB			0	// HID Class ではなく libusb を使用する

#define	INCLUDE_FUSION		1	// 融合命令を実装.
#define	FLOW_CONTROL		1	// FLOW_CONTROLは、FUSION有効時のみ使用する
#define	INCLUDE_POLL_CMD 	1	// ReportID:4  POLLING PORTを実装する.

#define	INCLUDE_MONITOR_CMD 1	// 62:POKE(),63:PEEK()を実装する.
#define	INCLUDE_ISP_CMD 	1	// 02:SET_STATUS()とISP移行モードを実装する. 
#define	INCLUDE_LED_NON 	1	// 02:SET_STATUS()のLED指定を無効(有効=>0)
/* ATS90をサポートするためには、INCLUDE_ISP_CMDの指定は必須
 *
 *	ISP_CMD / MONITOR_CMD は、少なくともどちらかは必要ですが、
 *	HIDmonを使用しない場合は MONITOR_CMD は不要です.
 *  AT90Sをサポートするには、ISP_CMDの有効化は必須です。
 */

//	FUSIONをOffにした場合は、メモリーに余裕が出来るので追加機能の作成に便利.
//	FUSIONをOffにしても、ライターソフトが自動判別して旧版互換で動作します.

//	POLL_CMD はライターソフトでは使用しません. HIDmon専用です.
//  現時点では、全ての機能を有効にしても、2048に収まります

/* ------------------------------------------------------------------------- */

#endif /* __hidconfig_h_included__ */
