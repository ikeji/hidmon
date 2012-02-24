/* ------------------------------------------------------------------------- */
/* hidconfig.h */
/* 2008-10-18 ... main.c から分離(by senshu) */
/* 2008-10-28 ... STARTUP_DELAY, USB_IO_MODE_ENABLEを追加
 */

#ifndef __hidconfig_h_included__
#define __hidconfig_h_included__

// Add by ikeji
#define PROGRAMMER_MODE_SWITCH 0  // Switch to programmer mode by PIND2
#define ENABLE_ISP_LED 0  // Disable LED on PINB2 and PINB3

//	コンフィギュレーションスイッチ:
// USE_LIBUSB は Makefile から指定するとソース変更が不要。
//#define	USE_LIBUSB			0	// HID Class ではなく libusb を使用する(84バイトの節約)

// Add by senshu
#define	STARTUP_DELAY		1	// リセット時に300m秒の待ちを実装(14バイトの増加)
#define	USB_IO_MODE_ENABLE	1	// リセット時、PORTBを入力モードにする(12バイトの増加)
#define	FLOW_CONTROL		1	// FLOW_CONTROLは、FUSION有効時のみ使用する(12バイトの増加)

// Add by iruka
#define	INCLUDE_FUSION		1	// 融合命令を実装.(76バイトの増加)
#define	INCLUDE_POLL_CMD 	1	// ReportID:4  POLLING PORTを実装する.(32バイトの増加)

// これらは、必ず指定すること
#define	INCLUDE_MONITOR_CMD 1	// 62:POKE(),63:PEEK()を実装する.
#define	INCLUDE_ISP_CMD 	1	// 02:SET_STATUS()とISP移行モードを実装する.

/* ATS90をサポートするためには、INCLUDE_ISP_CMDの指定は必須
 *
 *	ISP_CMD / MONITOR_CMD は、少なくともどちらかは必要ですが、
 *	HIDmonを使用しない場合は MONITOR_CMD は不要です.
 *  AT90Sをサポートするには、ISP_CMDの有効化は必須です。
 */

//	FUSIONをOffにした場合は、メモリーに余裕が出来るので追加機能の作成に便利.
//	FUSIONをOffにしても、ライターソフトが自動判別して旧版互換で動作します.

//	POLL_CMD はライターソフトでは使用しません. HIDmon専用です.
//  現時点では、全ての機能を有効にしても、2048に収まります。

/* ------------------------------------------------------------------------- */

#endif /* __hidconfig_h_included__ */
