                                                             2008年10月20日

        USB接続方式のドライバインストール不要なAVRライタ（HIDaspx）
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[0] はじめに

HIDaspxとは、ドライバのインストールが不要のUSB接続方式のAVRライタです。

瓶詰堂さんの作成された HIDasp を元に、iruka さんと senshu が共同で改良を行
ないました。一定の成果が得られましたので、2008年 9月 22日より公開を開始しま
す。HIDaspx は、瓶詰堂さんの公開しているものとは内容が異なり、firmware, 
hidspx コマンド共に互換性はありませんので区別して扱ってください。

※ HIDaspxは、エイチ・アイ・ディー アスペックスとお読みください。

[1] 同梱のファイルについて

2 つの exe ファイル（hidspx.exe, hidspx-bcc.exe）の機能は同じです。
hidspx.exe を利用して、動作に異常を見つけた時には、hidspx-bcc.exe を使って
みてください。全角文字を含むフォルダでの作業時に効果的な場合があります。
（コマンド名は任意に改名できますが、混乱しない名前をつけてください。）

また、hidspx のコマンド名を使っていますが、これはavrspx の別名です。
この名称にしているのは HIDaspx に対応することを示すためです。hidspx という
名前であっても avrspx の機能は全て利用可能です。しかし、機能追加作業により、
avrspx の機能に意図しない副作用が生じる可能性もあります。そのため、十分な動
作検証を終えるまでは、avrspx の名称をつけずに公開することにしました。ご了承
ください。


.\
├─bin
│	libusb0.dll
│	hidspx.exe	… MinGWでコンパイルしたもの
│	hidspx-bcc.exe	… Borland C++ ver 5.5.1でコンパイルしたもの
│	main.hex	… HIDaspのファームウェア
│	hidspx.ini	… hidspxの初期化ファイル
│	fuse.txt	… FUSE情報を詳細に表示する為のテキストファイル
│	fuse_en.txt
│	fuse_j.txt
├─bin
│  └─firmware	… main.hex（HIDaspx用）, main_libusb.hex（MacOS, Linux用）

├─firmware		… HIDaspのファームウェアソース
├─hidspx-src		… hidspxのソース(MinGW, Borland C++兼用)
│  └─libusb
│      ├─bin
│      ├─examples
│      ├─include
│      └─lib
│          ├─bcc
│          ├─dynamic
│          ├─gcc
│          ├─msvc
│          └─msvc_x64
└─pict		… 回路図と実装例

[2] 準備

bin/main.hex を Tiny2313 に書き込んでください．ヒューズの設定は，外付けの 
12MHz のクリスタルに合わせ、以下のように設定します。

Low: 10111111 (0xBF)
     ||||++++-- CKSEL[3:0] システムクロック選択
     ||++-- SUT[1:0] 起動時間
     |+-- CKOUT (0:PD2にシステムクロックを出力)
     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)

High:11-11011 (0xDB)
     |||||||+-- RSTDISBL (RESETピン 1:有効, 0:無効(PA2))
     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
     |||+-- WDTON (WDT 0:常時ON, 1:通常)
     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
     |+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
     +-- DWEN (On-Chipデバッグ 1:無効, 0:有効)

Ext: -------1 (0xFF)
            +-- SPMEN (SPM命令 1:無効, 0:有効)


[3] 使い方

　詳細な使い方は、avrsp.txtをご覧ください。

hidspx に「-ph」指定で HIDaspx が利用できます。hidspx.ini に -ph を書いてお
けば、この指定は省略できます。

> hidspx -ph test.hex

-d でディレイを指定でき、数値は時間に比例します。1MHz の RC 発振モードでは
-d4 以上を指定してください。なお、この値は、高速に動作する AVR マイコンでは
小さな値を指定でき、16MHz 以上の周波数で動作する AVR マイコンには -d0 が指
定できます。指定値が小さいほど、R/W の速度が向上します。

iruka さんによる計測結果を示します。（最新の版では、これよりも性能が向上し
ていることがあります）

	コマンド	評価内容		改良前	高速化後
-------------------------------------------------------------------------
hidspx -d0 -rp >XX.hex	8kB Read		8.39秒	2.22秒(3,690B/秒)
hidspx -d0 test.hex	8kB Write/Verify	10.94秒	4.86秒(1.64kB/秒)
hidspx -d1 -rp >XX.hex	8kB Read		8.40秒	3.12秒(2,625B/秒)
hidspx -d1 test.hex	8kB Write/Verify	10.96秒	7.06秒(1.13kB/秒)
hidspx -d4 -rp >XX.hex	8kB Read		8.39秒	4.63秒(1,770B/秒)
hidspx -d4 test.hex	8kB Write/Verify	13.26秒	9.98秒(821B/秒)
-------------------------------------------------------------------------

詳細な使い方は、avrspxの解説ページをご覧ください。

■ HIDaspxを高速に利用する方法

AT90S2313 のように、ページ書込みをサポートしない AVR マイコンでは書込みに時
間がかかります。USB 2.0 HUB を介せずに接続して、同様の書込みを行った結果で
す。

○ hidmonによるベンチマーク結果
AVR> bench
hid write start
hid write end, 38000 bytes/11172 ms,  3401 bytes/s

○ Write&Verify結果
>timeit hidspx -d2 2kb.hex
Elapsed Time:     0:00:25.703

次に、1,000 円以内で買える HUB を介して、同様の計測を行ってみます。

○ hidmonによるベンチマーク結果
>hidmon
AVR> bench
hid write start
hid write end, 38000 bytes/2468 ms,  15397 bytes/s

○ Write&Verify結果
>timeit hidspx -d2 2kb.hex
Elapsed Time:     0:00:09.546

ATtiny2313 に比べて遅いのですが、HUB 無しに比べて、1/2 以下に短縮され、10秒
未満で書込み＆照合が完了します。

USB HUB が非常によい仕事をしていることがわかります。hidmon で 15kB/s を超える
環境を用意するのが速度向上につながります。HUB をお持ちであれば、それを介して
利用してみてはいかがでしょうか。

===============================================================================

■ ライセンス（瓶詰堂さんに同じ）
AVR USBに準じてGPL2とします．

■ 開発者向けのメモ

2008-10-20
シリアル番号の生成ツールとともに、-ph:XXXXのサポートが完了しました。

■ シリアル番号の生成ツールの使い方

Gawk （WinAVR に付属）と avrasm2(Atmel) を使って、HIDaspx 用のシリアルデー
タ（HEX ファイル）を生成するツールを作成しました。

 > gawk -f genserial.awk 0001

のように使います。これを実行すると、瞬時に 0001 のシリアルデータの HEX ファ
イル（__serial.hex）を生成できます。このファイルを main.hex のファームとと
もに書き込めば、HIDaspx はそのシリアル番号を持ったものになります。

 > hidspx main.hex __serial.hex

シリアル番号の使い方

> hidspx -ph:0001

のように指定すると複数の HIDaspx を区別して使うことができます。なお、-ph と
だけ指定すると、初期値の 0000 が選ばれるので、HIDaspx を一台だけ利用してい
る場合には、シリアル番号を意識する必要はありません。

■ シリアル番号の使用例

0123の番号を持ったHIDaspxを使って動作の確認を行いました。

>hidspx -ph -r
HIDaspx(0000) not found. ---- -ph では「0000」を探します

>hidspx -ph? ---- 接続されている HIDaspxを表示させます
Manufacturer: [YCIT], Product: [HIDaspx], serial number: [0123]

>hidspx -ph:1234 -r ---- 番号を指定してHIDaspxを使います
HIDaspx(1234) not found. ---- 1234のHIDaspxは存在しません

>hidspx -ph:0123 -r ---- 正しく指定すれば、動作します
Detected device is ATtiny2313.
Device Signature  = 1E-91-0A
Flash Memory Size = 2048 bytes
Flash Memory Page = 32 bytes x 64 pages
EEPROM Size       = 128 bytes

■ 機能の変更点

　複数の開発者による開発は発散しがちですが、HIDaspx に関しては、奇跡的？に
上手く機能しています。

       4454 Oct 16 20:19 Makefile
       4540 Oct 16 20:22 Makefile.unix
         52 Oct 16 20:31 cleanup.bat
        144 Oct 17 08:41 rebuild.bat
       1137 Oct 18 18:03 hidconfig.h
      15153 Oct 18 18:04 usbconfig.h

    今まで main.c に書いていた「機能選択用の定義」を「hidconfig.h」に分離し、
    他のソースからも参照可能にしました。これによって、hidconfig.h のみを修
    正するだけで、最適なコードの作成が可能になります。

    私の追加したbatファイルの簡単な使い方を説明しておきます。

    0. 設定フラグは、hidconfig.h, usbconfig.h に設定する
       （main.c には設定フラグはありません）
    1. 通常は、make を実行する。
    2. エラーが無くなり、正しく機能することがわかれば、rebuildを実行すると、
    clean後に再コンパイルし、2種類のHEXファイルは ../bin/firmwareにコピーされる
    3. 作業が終われば、cleanupにて作業ファイルを削除する


    2 と 3 は一つにできますが、ファイルを削除するようなコマンドは、分離する
    のが無難と考えています。この変更で不都合がある場合には、ご指摘ください。

    また、iruka さんも書いていますが、現在、全ての機能を有効にしても 2044 
    バイトであり、2048 以内に収まっています。

    * senshu 2008-10-18 (土) 20:23:54 New
      hidspx と firmware の整合性を再確認してみましたが、LED の制御コマンド
      の有無の判定と LED 制御のファームウェアの部分に若干の疑問点があります。

      サイズが足りなかったので、コメントアウトした部分があるのですが、これ
      は整理し、LED 制御は MONITOR コマンドに統一し、ISP_CMD は必ず使えるよ
      うにするのがよいと考えています。

      色々検討した結果、以下の仕様にしたいと考えています。（1017版も同様）

      (1) ISP_CMD は必ず組み込む
      (2) ISP_CMDではLEDの制御は無効（ファームに任せる）
      　　⇒ ライタ制御プログラム(hidspx)で指定しても無視する
      (3) 2個のLEDをPCから制御する場合、ISP_CMDではなくMONITOR機能を利用する

      この仕様にすれば、ファームもコンパクトに維持でき、混乱を避けられます。
      問題は 9月に公開したファームを使っている場合ですが、これは新しいもの
      に更新をお願いすることになります。



■ 変更履歴
2008-09-22 ... senshuとirukaさんが改造を実施	高速化対応実施

2008-09-24 ... senshu  firmware -d0オプションの最適化
               hidpsxのエラー検出時、のメッセージ出力の修正（改行して表示）

2008-09-28 ... hidspxのコンパイルオプションに-Wallを追加し、警告メッセージに
               対する修正を実施(senshu)
               firmware -d0オプションをUSBasp互換性を高めた(iruka)
               MOSI, SCK の競合を回避（Hi-Z化する）

2008-10-03 ...	- hidspxのメッセージをHIDaspxに統一
		- Borland C++ での警告メッセージを抑止(avrspx.hを修正)
		- usbtool を追加
		- kugaさんのアドバイスにより、Firmwareサイズを40バイト縮小(1968バイト)
		 avr-size --mcu=attiny2313 main.elf
		   text    data     bss     dec     hex filename
		   1966       4      85    2055     807 main.elf
		- USBのProductID 0x5dc (libusb device) との競合を避ける為、
		  0x5df(HID devide)に変更
		- firmwareの変更 （今のところサポートしているAVRデバイスでは）
		  page_addr は 256 以下なので、uint16 から uint8 に降格。

2008-10-06 ...	- irukaさんの10-05までの修正分を反映（ほぼ同じ内容です）
		- -d2以上の値を指定した時の不具合は、senshuの修正ミスでした。
		 （irukaさん、大変お手数をお掛けしました）
		- ATtiny2313の認識が不十分だった件に関しては、このミスに起因して
		 シーケンスが見直され？、より適切なものになっています
				- firmwareの変更  page_addr を uint16 に戻す。

2008-10-10 ...	- irukaさんの10-09までの修正分を反映（ほぼ同じ内容です）
		- 2008.10.9a delay_10us関数がコンパイル環境によっては最適化されて
		  消えていたのを修正. （アセンブﾘ言語でリライト）
		- 2008.10.9a '-d2'以上の遅延クロック数を適正化
		- 2008.10.9 高速化: flow-controlを導入
		- 2008.10.9 12MHzターゲットに '-d0'が使えるように変更(SCLK=2MHz)
		- 2008.10.9 ispConnect/ispDisconnectを導入

2008-10-12 ...	- kugaさんのAT90Sシリーズ用のISP移行処理を追加（90Sシリーズをサポート）
		- ATtiny2313 のFUSEビットSTUビットの値によって、認識に失敗するのを修正
		  （hidspx内のhwctrl.c spi_reset関数にdelay_ms(10)を追加しました）
		- FUSE verifyエラー時の表示を適正化
		- FUSE 書き換え関数の適正化（重要！）

2008-10-14 ...	- firmware main.c「USICR=0;		/* SCKをポートに戻しておく */」を有効化
		-- USIの初期化をmain関数から、usi_trans関数に移動(SCK発生時の互換性向上)
		- usbHidReportDescriptorの大きさを9バイト縮小(現在 2038バイト)
		- hidmonとの整合性を考慮し、元に戻しました(現在 2048バイト)
		- ソース中のコメントを修正（不整合部分を修正）

2008-10-17 ...	- firmware (HID版に加え、libusb版を追加）
		- rebuild.bat , cleanup.batを追加
		- avrx-tool.txt（詳しい使い方の解説）を追加

2008-10-20 ...	- firmware (HID版に加え、libusb版を追加）
		- -ph:XXXX のシリアル番号をサポート
		- firmwareソースのクリーンアップを実施
		- bin フォルダにシリアル番号生成ツール(genserial.awk)を追加
		- 新アイコンに変更(RAINさん作成)


# TAB size = 8で編集しています。

