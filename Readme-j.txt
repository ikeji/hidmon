# Windowsの標準的な環境に合わせ、TAB size = 8で編集しています。

                                                2008年 9月22日（公開開始）
                                                          ｜
                                                2009年 3月30日（最新更新）

                                      山形県立産業技術短期大学校  千秋広幸
                                 E-mail senshu(at)astro.yamatata-cit.ac.jp


        USB接続方式のドライバインストール不要なAVRライタ（HIDaspx）
        ￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣

【1】 はじめに

HIDaspx は、ドライバのインストールが不要の USB 接続方式の AVR ライタです。
別途提供される hidmon を利用すれば、汎用の USB-IO としても利用できます。

瓶詰堂さんの作成された HIDasp を元に、iruka さんと senshu が共同で改良を行
ないました。一定の成果が得られましたので、2008年 9月 22日より公開を開始しま
す。HIDaspx は、瓶詰堂さんの公開しているものとは内容が異なり、firmware, 
hidspx コマンド共に互換性はありませんので区別して扱ってください。

※ HIDaspxは、エイチ・アイ・ディー アスペックスとお読みください。

  ********************** 更新時の注意点 **********************
  hidspx の更新時、ファームウェアの日付も更新されますが、更新履歴にファーム
  ウェアに関する記載が無い場合にはファームの更新は不要です。bin フォルダの
  setup.bat で一括更新ができます。[更新ディレクトリ名] を省略した場合には、
  C:\binにコピーする仕様です。

  XXXX\hidspx-2009-0111\bin> setup [更新ディレクトリ名] <Enter>
  ================= hidspx をセットアップします ====================
 C:\binフォルダ にコピーを希望する場合は、[Y]を大文字で入力のこと。
 ↑にセットアップをしますか[Y/n]

 表示されるメッセージに従って、ご活用ください。


【2】 アーカイブの構成

.\
├─ 各種ドキュメント
├─bin
│	libusb0.dll
│	hidspx-gcc.exe	… MinGW-GCCでコンパイルしたもの
│	hidspx.exe	… Borland C++ ver 5.5.1でコンパイルしたもの
│	hidspx.ini	… hidspxの初期化ファイル
│	fuse.txt	… FUSE情報を詳細表示する為のテキストファイル（hidspxが参照）
│	fuse_j.txt	… FUSE情報を詳細表示する為のテキストファイル（日本語表示）
│	fuse_en.txt	… FUSE情報を詳細表示する為のテキストファイル（英字表示）
│	fuse_en_orig.txt… FUSE情報を詳細表示する為のテキストファイル（英語環境用）
│	hidmon.exe	… HIDaspxのテスト時に利用
│
├─bin
│  └─firmware	… main-12.hex（HIDaspx用）, main_libusb.hex（MacOS, Linux用）
│			… main-small.hex(USB-IO専用のファームウェア)
│			… main-RC.hex (RC発振モードのUSB-IO専用のファームウェア、実験レベル)
｜　　　　　　　　　　　… genserial.awk（シリアル情報生成ツール）
｜　　　　　　　　　　　… firmprog.bat main12.hexを書き込むためのBATファイル
｜　　　　　　　　　　　… prog-rc.bat  main-RC.hexを書き込むためのBATファイル
｜
├─firmware		… HIDaspのファームウェアソース
├─src			… hidspxのソース(MinGW, Borland C++兼用)
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
└─circuit		… 回路図


【3】 hidspxコマンドのインストール方法

　hidspxのインストールには、特別なインストーラは不要です。
bin ディレクトリにあるファイルを実行可能なディレクトリにコピーすることで利
用可能になります。（ファイル日付は、改良により異なることがあります）

**************************** 重要 ****************************
2008/11/13  13:30             1,787 setup.bat
2007/03/20  11:33            43,520 libusb0.dll
2009/01/10  15:15           121,344 hidspx.exe
2009/01/10  15:16            54,784 hidspx-gcc.exe
2008/09/09  12:54            17,408 usbtool.exe
2008/09/05  10:39               289 hidspx.ini
2008/12/10  08:39            20,115 fuse.txt
2008/12/07  13:08            22,152 fuse_en.txt
2008/12/10  08:41            20,115 fuse_j.txt
**************************** 重要 ****************************

これらのファイルは、相互に関係していますので、hidspx.exe だけでなく一括して
コピーしてください。

コピー漏れを防ぐ為、setup.batファイルを用意しました。

 setup.bat を実行すると c:\hidmon に必要なファイルをコピーします。
 setup c:\bin のように指定すれば、c:\bin に必要なファイルをコピーします。

環境変数が操作できる方は、hidspx\bin というディレクトリを PATH 環境変数に登
録すると、簡潔にコマンドを指定でき便利利用できます。

なお、2 つの実行ファイル（hidspx-gcc.exe, hidspx.exe）の機能は同じです。
1 ヶ月以上のテスト期間中に問題が無かったことから、Borland C++ でコンパイル
した hidspx.exe を標準実行ファイルとして公開します。動作に異常がある時には、
従来の標準にしていた hidspx-gcc.exe を使ってみてください。

===========================「補足」===========================
現在のところ、hidspx のコマンド名を使っていますが、これは avrspx の別名です。
（名前は任意に変更できますが、混乱しない名前をつけてください。設定ファイル
は、コマンド名.ini になります。）

avrspx ではなく、hidspx にしているのは 既存の avrspx の利用者に混乱を与えな
ず、HIDaspx 対応を明示するためです。hidspx は avrspx 全ての機能を利用でき、
多くの改良を行っています。しかし、意図しない副作用（不具合）が生じる可能性
も皆無とはいえません。そのため、十分な動作検証を終えるまでは、avrspx の名称
をつけずに公開ますので、ご了承ください。
==============================================================

【4】 準備

circuit フォルダある回路図（HIDaspx.png）を参考にハードウェアを製作します。
なお、近日中に専用の基板が入手可能 (http://www.wsnak.com より) になります。
回路図にも明記していますが、発振子にはクリスタルを実装してください。セラミ
ック振動子では回路が要求する精度と安定性を満たすことができません。
この後で、bin/main.hex を Tiny2313 に書き込んでください。ヒューズ設定は，外
付けの 12MHz クリスタルに合わせ、以下のように設定します。

Low: 11111111 (0xFF)
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


※ 今までは、PD2からのクロック出力を有効にしていました。この機能を利用したい
   場合には、Low=0xBF と設定します。
   
   この版では、PD2 は開放で通常のライタ、GND と接続すると USB-IO として機能
   します。ライタモードでは PB2 （LED 兼用) 端子から 1MHz の矩形波を出力し
   ますので、PD2 （12MHz）よりも広範囲に利用できます。

　hidspx を利用できる方は、工場出荷状態の ATtiny2313 に対し、以下のコマンド
で FUSE 設定が可能です。ただし、外部に発振器を接続していない時にこの設定を
行うと、発振器を接続するまで ISP 方式のライタによる読み書きが出来なくなりま
すので、ご注意ください。

> hidspx -d4 -fL0xff -fH0xdb -fX0xff <ENTER>


【5】 使い方

　以下に簡単な使用例を紹介します。詳細な使い方は、avrx-tool.txtをご覧ください。

※ 1030版以降は、FUSE Lowを0xFFに設定することをお薦めします。

■ USB-IOとして利用する
USB-IO として利用する場合には、PD2 を GND に繋いでください。USB-IO として使
っている装置に対し、hidspx を利用すると、以下のようにエラーになります。
これは、USB-IO として利用している HIDaspx を、誤ってプログラマ（ライタ）と
して操作するのを避ける為です。

 >hidspx -rp
 Error: HIDaspx is USB-IO mode.
 HIDaspx(0000) not found.


■ AVRライタ（Programmer）として利用する
USB-IO 機能を利用しない場合には、PD2 周りのハードウェア変更は不要です。
なおライタモードでは、PB2 を ターゲットマイコンの 救済用クロック (1MHz) と
して機能するため、PB2(PWR LED) の LED はやや薄く点灯します。

hidspx で「-ph」を指定すれば HIDaspx が利用できます。HIDaspx 専用に使う場合
には、hidspx.ini に -ph を書いておけば、この指定は省略できます。

> hidspx -ph -d4 test.hex

hidspx.ini では -d4 を設定しています。-d でディレイを指定でき、数値は時間に
比例して遅くなります。1MHz の RC 発振モードでは -d4 以上を指定してください。
この値は、高速に動作する AVR マイコンでは小さな値を指定でき、16MHz 以上の周
波数で動作する AVR マイコンには -d0 が指定できます。指定値が小さいほど、R/W
の速度が向上します。

-dNの指定の目安は、以下のとおりです。

No	FUSE Low	-dの値	発振周波数	備考
0	----	        -d0	16MHz以上	外部クリスタル/セラミック発振子
1	-fL11100100	-d1	8MHz		14CK+65ms
2	-fL11100010	-d2	4MHz		14CK+65ms
3	-fL01100100	-d4	1MHz		工場出荷値
4	-fL01100010	-d5	500kHz		14CK+65ms
5	-fL11100110	-d17	128kHz		14CK+65ms
6	-fL01100110	-d120	16kHz		118, 119では不安定


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

【6】 HIDaspxの高速化（USB-HUB利用の薦め）

　AT90S2313 のように、ページ書込み機能の無い AVR マイコンでは書込みに時間が
かかります。USB 2.0 HUB を介せずに接続して書込みを行った結果です。

○ hidmonによるベンチマーク結果
AVR> bench
hid write start
hid write end,   37.109 kB/   11.08 s,   3.350 kB/s

○ Write&Verify結果
>timeit hidspx -d2 2kb.hex
Elapsed Time:     0:00:25.703

次に、1,000 円以内で買える USB 2.0 に準拠した HUB を介して、同様の計測を行
ってみます。

○ hidmonによるベンチマーク結果
>hidmon
TARGET DEV_ID=55
HIDaspx is USB-IO mode.
AVR> bench
hid write start
hid write end,   37.109 kB/    2.47 s,  15.036 kB/s

○ Write&Verify結果
>timeit hidspx -d2 2kb.hex
Elapsed Time:     0:00:09.546

HUB 無しに比べて、1/2 以下に短縮され、10秒未満で書込み＆照合が完了します。

このように旧タイプの AVR マイコンも利用できますが、迅速な書込みを希望する場
合には、新タイプのマイコンを採用してください。同じメモリサイズの ATtiny2313
では 1秒程度で完了します。

これらの結果から、USB HUB が非常によい仕事をしていることがわかります。旧タ
イプの AVR マイコンに限らず、ほとんどの場合、処理速度は格段に向上します。
hidmon で 15kB/s を超える環境を用意すれば速度向上につながります。USB-HUB を
入手し、それを介して利用してみてはいかがでしょうか。

===============================================================================

【7】 利用上の注意点

　2009年 1月から公開した版では、「-ri, -rd, --atmel-avr, --avr-devices」な
どのコマンドを使うことで、Web ブラウザを使った機能を提供していますが、接続
先のサービスが停止している場合もあり、その時には白紙のページが表示されたり、
エラーが表示される場合があることをご理解ください。

【8】 ライセンス（瓶詰堂さんに同じ）
AVR USBに準じ、GPL2とします。（お願い：商用利用時には、ご一報ください。）

【9】 謝辞
　HIDaspx の開発には、多くの方から協力をいただきました。ここに、心より感謝
の意を表します。

    1. 瓶詰堂さん  …… HIDaspの開発者です。HIDaspなくしてはHIDaspxはありません。
    2. irukaさん   …… 高速化（ファームウェアとhidspx）はirukaさんの成果です。
    3. kugaさん    …… 旧AVRのサポートとコンパクト化、PD5による再接続機能。
    4. RAINさん    …… 新アイコンの作成と各種の動作テスト。
    5. 岩永さん    …… 複数のHIDaspxを区別して利用できることを確認いただきました。
    6. kumanさん   …… 早期から注目していただき、多くの助言をいただきました。
    7. TADさん、宮前さん、mituさん、akakusaさん、すんさん、kawanaさん
                   …… 動作報告や各種のコメントをいただきました。

  ----- * ----- * ----- * ----- * ----- * ----- * ----- * ----- * -----

【付録】 開発者向けのメモ

[0] 2008-10-17～10-22の作業内容

　複数台の HIDaspx を一台の PC に混乱無く利用できる機構を実現する為、シリア
ル番号の生成ツールや hidspx, hidmon の機能拡張を行いました。この機能を実現
するため、以下の作業を行っています。

 1. シリアル情報生成ツールの作成（gawkで記述）
 2. hidspxコマンドの-ph[:XXXX]にシリアル情報指定機能を追加
 3.          〃   に -ph? でリストできる機能を追加
 4. hidmon にも同様の機能を追加
 5.          〃   に -p? でリストできる機能を追加
 6. hidmon.dllに シリアル情報を指定できる機能を追加
 7. ドキュメントの見直し
 8. 実際の環境での動作確認

[2] firmware, hidspx-src の利用法

 1. CMD prompt 上で make を使ってbuildする

 2. ヘッダファイルなどを変更した場合には、make clean 後にmakeすること。
    ミスを避ける為、rebuild.bat を用意している。rebuild を実行すると、これ
    を必ず実行できる。なお、作業が済んだら、cleanup を実行すると、作業ファ
    イルを一括して削除できる。

 3. エラー無く生成されたファームウェアや実行ファイルは、binの下にコピーされる

 4. 更新漏れを防ぐ為、最後にdevelopディレクトリのbuild-all.batを実行する

[3] シリアル情報の生成ツールの使い方

HIDaspx 用のシリアルデータ（HEX ファイル）を生成するツールを用意しました。

　このツールは gawk(GNU awk というインタープリタ） を使って動作します。AVR
マイコンの利用者に広く普及している WinAVR には、gawk.exe が付属していますが、
gawk を利用できない方は、バグが少なく単独で動作する、木村さんの移植版 gawk
を入手してください。

2008年7月4日 gawk 3.1.6 日本語版バイナリバグfix
GAWKの改良版の入手先 ⇒ http://www.kt.rim.or.jp/~kbk/

http://www.kt.rim.or.jp/%7ekbk/cgi-bin/download.cgi?dl-file1=gawk-mbcs-win32-20080704.zip

入手した gawk-mbcs-win32-20080704.zip を展開し、gawk.exe を hidspx.exe と同
じディレクトリにコピーします。

 2007/10/22  15:50              280,351 ChangeLog
 2007/07/16  23:59               35,147 COPYING
 2008/07/02  16:47              189,434 mbc-diff
 2007/09/25  14:58               83,133 NEWS
 2008/07/03  16:51              729,088 gawk.exe
 2006/12/01  14:26               53,248 igawk.exe
 2008/07/03  17:22                9,588 readme.ja

1028 版以降、「-v adr= 」を指定することで、任意のアドレスに対するシリアル情
報を生成でき、また、複数のシリアル情報も瞬時に生成可能です。

usage: gawk -f genserial.awk -v t={aspx|mon88} start-number  [end-number]
￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣

使用例1：
 > gawk -f genserial.awk 0001

のように使います。これを実行すると、0001 のシリアルデータの HEX ファイル
（_0001.hex）を生成できます。

使用例2：0001～0100のシリアル情報のファイルを一括して生成します。
         指定できる値は1～9999まで、これを超えるとエラーになります。

 > gawk -f genserial.awk 1 100


使用例3：16進数や英大文字の指定も可能です。小文字は大文字に変換します。

 > gawk -f genserial.awk ABCD

このファイルを main.hex のファームとともに以下のように書き込めば、HIDaspx 
はそのシリアル番号を持ったものになります。

 > hidspx main.hex _0001.hex

※ 先頭文字に'_'を追加しましたので、以下のような操作が可能です。
 > del _*.hex
 で一括してシリアル情報用のファイルを削除できます。

使用例4：
 > gawk -f genserial.awk -v t=mon88 0001

bootmon88用のシリアル情報を生成します。

使用例5：
 > gawk -f genserial.awk -v t=1234 0001

1234(10進数)番地から始まる4桁のシリアル情報を生成します。


[3.1] hidspxでのシリアル情報指定方法

複数台の異なるシリアル情報を持った HIDaspx を使って動作の確認を行いました。
この機能により、複数の HIDaspx を区別して利用できます。（一台だけで利用する
場合には、シリアル情報設定は不要です。）

> hidspx -ph:0001  （hidspx -ph1 でも同じ意味になります）

↑のように指定すると複数の HIDaspx を区別して使うことができます。なお、-ph
と指定で初期値の 0000 を選択します。シリアル情報を設定しない場合には、従来
通りの利用が可能です。

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

[4] 機能の変更点

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


    2 と 3 は一つにできますが、ファイルを削除するコマンドは、分離するのが無
    難と考えています。この変更で不都合がある場合には、ご指摘ください。

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


[5] 参考URL

    * HIDaspxの公式URL（山形県立産業技術短期大学校内）
         http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2FHIDaspx

    * 瓶詰堂さん(HIDaspの開発元) ⇒ http://www.binzume.net/library/avr_hidasp.html

    * irukaさんのサイト
	 http://hp.vector.co.jp/authors/VA000177/html/A3C8A3C9A3C4A3E1A3F3A3F0.html

    * kumanさんの回路図を含む、実践レポート(一読をお勧めします)
	http://www.geocities.jp/kuman2600/n6programmer.html#13 (10/12追記あり)

    * Objective Development Software GmbH … AVR-USB（AVRマイコン用のUSBドライバのサイト）
	http://www.obdev.at/products/avrusb/index.html

    * Atmel社のWebサイト（AVRマイコンの製造元）
        http://www.atmel.com/products/AVR/  … データシートや公式情報はこちらから入手可能

    * AVRマイコンのユーザーコミュニティ
        http://www.avrfreaks.net/  … AVRstudio や avr-gccの各種情報はこちらから入手可能


[6] 変更履歴
■2008-09-22   senshuとirukaさんが改造を実施	高速化対応実施

■2008-09-24   senshu  firmware -d0オプションの最適化
               * hidpsxのエラー検出時、のメッセージ出力の修正（改行して表示）

■2008-09-28   * hidspxのコンパイルオプションに-Wallを追加し、警告メッセージに
                 対する修正を実施(senshu)
               * firmware -d0オプションをUSBasp互換性を高めた(iruka)
               * MOSI, SCK の競合を回避（Hi-Z化する）

■2008-10-03    * hidspxのメッセージをHIDaspxに統一
	        * Borland C++ での警告メッセージを抑止(avrspx.hを修正)
	        * usbtool を追加
	        * kugaさんのアドバイスにより、Firmwareサイズを40バイト縮小(1968バイト)
		 avr-size --mcu=attiny2313 main.elf
		   text    data     bss     dec     hex filename
		   1966       4      85    2055     807 main.elf
	        * USBのProductID 0x5dc (libusb device) との競合を避ける為、
		  0x5df(HID devide)に変更
	        * firmwareの変更 （今のところサポートしているAVRデバイスでは）
		  page_addr は 256 以下なので、uint16 から uint8 に降格。

■2008-10-06    * irukaさんの10-05までの修正分を反映（ほぼ同じ内容です）
	        * -d2以上の値を指定した時の不具合は、senshuの修正ミスでした。
		 （irukaさん、大変お手数をお掛けしました）
	        * ATtiny2313の認識が不十分だった件に関しては、このミスに起因して
		 シーケンスが見直され？、より適切なものになっています
			        * firmwareの変更  page_addr を uint16 に戻す。

■2008-10-10    * irukaさんの10-09までの修正分を反映（ほぼ同じ内容です）
	        * 2008.10.9a delay_10us関数がコンパイル環境によっては最適化されて
		  消えていたのを修正. （アセンブﾘ言語でリライト）
	        * 2008.10.9a '-d2'以上の遅延クロック数を適正化
	        * 2008.10.9 高速化: flow-controlを導入
	        * 2008.10.9 12MHzターゲットに '-d0'が使えるように変更(SCLK=2MHz)
	        * 2008.10.9 ispConnect/ispDisconnectを導入

■2008-10-12    * kugaさんのAT90Sシリーズ用のISP移行処理を追加（90Sシリーズをサポート）
	        * ATtiny2313 のFUSEビットSTUビットの値によって、認識に失敗するのを修正
		  （hidspx内のhwctrl.c spi_reset関数にdelay_ms(10)を追加しました）
	        * FUSE verifyエラー時の表示を適正化
	        * FUSE 書き換え関数の適正化（重要！）

■2008-10-14    * firmware main.c「USICR=0;	/* SCKをポートに戻しておく */」を有効化
		-- USIの初期化をmain関数から、usi_trans関数に移動(SCK発生時の互換性向上)
	        * usbHidReportDescriptorの大きさを9バイト縮小(現在 2038バイト)
	        * hidmonとの整合性を考慮し、元に戻しました(現在 2048バイト)
	        * ソース中のコメントを修正（不整合部分を修正）

■2008-10-17    * firmware (HID版に加え、libusb版を追加）
	        * rebuild.bat , cleanup.batを追加
	        * avrx-tool.txt（詳しい使い方の解説）を追加

■2008-10-20    * firmware (HID版に加え、libusb版を追加）
	        * -ph:XXXX のシリアル情報をサポート
	        * firmwareソースのクリーンアップを実施
	        * bin フォルダにシリアル情報生成ツール(genserial.awk)を追加
	        * 新アイコンに変更(RAINさん作成)

■2008-10-21    * シリアル情報生成ツールの強化
	        * 回路図ファイルを更新

■2008-10-22    * シリアル情報生成ツールの強化(WinAVR付属のgawkでも正常に動作)
	        * 説明書の見直し

■2008-10-23    * シリアル情報生成ツールの強化(数字、英大文字の利用も可能)
	        * AVR-USBのドライバを2008-10-22版に更新

■2008-10-28    * 安定性向上のため、HIDaspxファームの起動時に300m秒の時間待ちを挿入した
	        * 説明書の加筆（メモリロック部分）
	        * USB-IOモードの処理を追加した(PORTBを入力モードにする)
	        * hidmonにて提供していたファームウエアのsmall版をこの版に含めた
	        * 十分な安定性が確認できたので、Borland C++ でコンパイルしたものを 
		  hidspx.exeとし、MinGW-gcc でコンパイルしたものを hidspx-gcc.exe とした。
	        * 重複して出力されるメッセージを1度のみ、出力するようにした。

■2008-10-30    * USB-IO と Programmerモードを区別して扱うように変更しました。
	        * firmware と hidspx.exe の両方を更新する必要があります。
	        * 回路図を修正しました(PD2とPB2の扱いを変更した為）
	        * HIDaspxのページをPDF化し、付属資料に追加しました。

■2008-10-31 ...  * 付属の説明書に不整合部分を修正しました。
	        * firmware 中の uint8_t => ucharに統一（AVR-USBに合わせました）

■2008-11-05    * developディレクトリに、buil-all.batを用意した。
	        * このコマンドを実行すると、公開に必要な全てのファイルを生成します。
		  （マウスのダブルクリックでも機能します）
	        * -ph指定で、0001を1と入力可能にしました。より簡潔な利用が可能です。
	        * PORTD (PD6, PD1, PD0)を入力（プルアップあり）として初期化
		  （akakusaさんからの要望による変更）

■2008-11-06    * シリアル情報生成ツールをbootmon88対応にした。
	        * hidasp.c をbootmon88との共用にした。
	        * hidmon, bootmon88に合わせるため、hidspx-src ディレクトリを src に変更。
	        * 簡易セットアップツール（setup.bat）を追加。

■2008-11-12    * USB-IOモードで利用した場合には、エラーで中断するのではなく、警告に留めた。
                  >hidspx -rf
                   Warnning: Please check HIDaspx mode.
                * hidasp.c を最新に更新
	        * vendor, product の文字列の照合を無効にし、VID, PID とシリアル情報のみを
		  照合します（文字列照合を行いません）。

		  [理由]
		  以下の異なるvendor名やプロダクト名の装置を利用可能にするため。

                  VID=0x16c0 PID=0x05df vendor="YCIT" product="HIDaspx" serial="0000"
                  VID=0x16c0 PID=0x05df vendor="AVRetc" product="bmon" serial="0001"
                  
                  Makefileにソースファイル間の依存情報を追加

■2008-11-27 ...
        * kugaさんのアドバイスにより、何もサポートしない(NAKしか返さない) 
	  interrupt_in ENDPOINT を作る機能を追加しました。

	usbconfig.h に追加

	#define USB_CFG_HAVE_INTRIN_ENDPOINT    1
	#define USB_CFG_SUPPRESS_INTR_CODE      1
	#define USB_CFG_INTR_POLL_INTERVAL      200 //無駄なので長いinterval

	Mac OS Xや BSD OSでの動作確認が可能な方は、お試しください。
	ファームウェアは、以下のように、ほぼ限界に近いサイズになっています。
	（これを決定版としたいところです）

	   text    data     bss     dec     hex filename
	      0    1620       0    1620     654 main-small.hex
	      0    2042       0    2042     7fa main.hex
	      0    1958       0    1958     7a6 main_libusb.hex


■2008-12-03 usbdrvドライバを更新

        * AVRUSB-20081126版が出ましたので、usbdrvドライバを更新しました。
	  内容はほとんど変わっていません。ただし、以下の設定に関わるコードの
	  修正がありました。

	#define USB_CFG_HAVE_INTRIN_ENDPOINT    1
	#define USB_CFG_SUPPRESS_INTR_CODE      1
	
	  そのため、以下のように生成されるコードサイズが若干小さくなっています。

	   text    data     bss     dec     hex filename
	      0    2038       0    2038     7f6 main-12.hex
	      0    1638       0    1638     666 main-small-12.hex
	      0    2002       0    2002     7d2 main-small-RC.hex
	      0    1954       0    1954     7a2 main_libusb.hex

        * hidspxの書き込み待ち時間のマージンを拡大

        * hidspxのverifyエラー時の処理を改良。

	  今までは、1 バイトの違いを検出した段階で処理を中断していましたが、
	  この版から、エラーの発生時も、読み込んだデータ全ての照合を行うよ
	  うに仕様を変更しました。その結果、以下のように有益な情報を得るこ
	  とができます。
	  
	  この例は、12MHz で動作する ATtiny2313 に対し -d0 という規格外の条
	  件で書き込みを行ったため、多くのエラーが発生していますが、2038 バ
	  イト中 20 バイト程度のエラー率であることがわかります。
	  
	>hidspx -d0 main-12.hex
	Detected device is ATtiny2313.
	Erase Flash memory.
	Write   Flash: 2038/2038 B
	Verify  Flash:  256/2038 B
	Failed at 000B:A1->FF
	Failed at 000C:01->02
	Failed at 0146:65->00
	Failed at 0147:00->CB
	Failed at 0283:91->FF
	Failed at 0284:80->18
	Failed at 02C0:FD->FF
	Failed at 02C1:01->FA
	Failed at 03C2:E1->FF
	Failed at 03C3:F5->CF
	Failed at 0403:E7->FF
	Failed at 0404:90->0E
	Failed at 04A0:11->00
	Failed at 04A1:F0->22
	Failed at 04D9:27->00
	Failed at 04DA:80->13
	Failed at 0620:2F->00
	Failed at 0621:C0->5E
	Failed at 06B7:1B->00
	Failed at 06B8:D0->86
	Failed at 06E5:93->00
	Failed at 0727:93->FF
	Failed at 0728:99->60
	Verify  Flash: 2038/2038 B

	この照合機能（-v）は、ブートローダで書き込んだファームウェアのデー
	タ照合用にも利用することができます。なお、-d1 のように、適切な指定
	を行えば、以下のように正常に書き込みが行われます。

	>hidspx -d1 main-12.hex
	Detected device is ATtiny2313.
	Erase Flash memory.
	Write   Flash: 2038/2038 B
	Verify  Flash: 2038/2038 B
	Passed.

■2008-12-10 サポートAVRを追加
        * mega325P/3250P/324PA, PWM216,316対応を追加


■2009-01-10 -rF, -riオプションを追加

        * -rF (Read Fuse list) オプション
        このオプションを指定すると、ヒューズ設定形式で出力します。
        avrdudeなどで利用する場合にも、使いやすい形式です。
        CMD窓のコピー機能を使って、加工してください。

        > hidspx -rF
	Detected device is ATtiny2313.
        DEVICE=ATtiny2313 -fL0xE4 -fH0xDF -fX0xFF

        書き込済みの AVR のヒューズ情報設定時に利用できる形式で出力します。
        -rf は従来と同様の機能ですので、内容を確認する場合には、-rf オプシ
        ョンを使ってください。インターネットに繋がっている場合には、-ri オ
        プションが便利です。

        * -ri (Read Fuse Infomation) オプション
        このオプションを指定すると、Web ブラウザで、自動的に
        http://www.engbedded.com/cgi-bin/fc.cgi/ ページを開き、該当する AVR
        マイコンのヒューズ情報を表示します。

        > hidspx -ri (または -rI)

        現在のところ提供されていない ATmega644P は、暫定的に ATmega644 を呼
        び出すようにしました。Web ブラウザは、MS-IE, firefox,Sleipnir の 3
        種類について、それぞれ標準ブラウザに設定した時に、正常動作すること
        を確認しました。なお、-ri で情報を確認するたびに新たなページを開き
        ますので、確認後にそのページを閉じ、開き過ぎに注意してください。

        上記のWebサイトでfuse情報をサポートしているAVRマイコンの一覧
        (2009年1月10日現在)
        
        AT86RF401       AT89S51         AT89S52         AT90CAN128
        AT90CAN32       AT90CAN64       AT90PWM2        AT90PWM3
        
        AT90S1200       AT90S2313       AT90S2323       AT90S2343
        AT90S4414       AT90S4433       AT90S4434       AT90S8515
        AT90S8515comp   AT90S8535       AT90S8535comp   
        
        ATmega48        ATmega8         ATmega8515      ATmega8535
        ATmega88        ATmega103       ATmega103comp   ATmega16
        ATmega161       ATmega161comp   ATmega162       ATmega163
        ATmega164P      ATmega165       ATmega168       ATmega169
        ATmega32        ATmega323       ATmega325       ATmega3250
        ATmega328P      ATmega329       ATmega3290      ATmega406
        ATmega64        ATmega640       ATmega644       ATmega645
        ATmega6450      ATmega649       ATmega6490      ATmega128
        ATmega1280      ATmega1281      ATmega2560      ATmega2561
        
        ATtiny10        ATtiny11        ATtiny12        ATtiny13
        ATtiny15        ATtiny167       ATtiny22        ATtiny24
        ATtiny25        ATtiny26        ATtiny261       ATtiny28
        ATtiny44        ATtiny45        ATtiny461       ATtiny48
        ATtiny84        ATtiny85        ATtiny861       ATtiny88

	長期にわたり有益な情報サイトを運営している www.engbedded.com さんと、
	このアイディアを提供してくれた audin さんに感謝いたします。
	どうもありがとうございました。

■2009-01-11 hidmonの追加とヘルプメッセージの修正

        * hidspx , hidspx -? で表示するメッセージに、0110の機能追加分を反映

        * この説明書に記載した hidmon の bench テストを実行できるように、
          hidmon.exe コマンドを同梱

        * -rFオプションの仕様を以下のように拡張
        コピー＆ペーストで直接利用可能な形式を出力するに仕様とした。
        以下のように、hidspxとavrdude用のコマンド行を表示する。
        （海外ではavrdudeの使用が主流であり、参考のためにこの機能を追加した）

        ※ FUSE 設定時には、エラーを避けるため、-d10 のように速度を落として
           書き込むことをお勧めします。

        >hidspx -rF
        Detected device is ATtiny2313.
        
        DEVICE=attiny2313
        ### hidspx command line example ###
        hidspx -d10 -fL0xE4 -fH0xDF -fX0xFF
        
        ### avrdude command line example ###
         avrdude -c avrdoper -P stk500v2  -p tiny2313 -U flash:w:main.hex:i \
         -U lfuse:w:0xe4:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

        * -rd Docment view機能を追加
        このオプションを指定することにより、該当する AVR マイコンの概要を 
        Web ブラウザに表示します。この機能は、http://www.avrfreaks.net/ さ
        んが提供する情報を利用しています。

■2009-01-12 Atmel社とAVR Freaks のURLを開く機能を追加

        * 「--atmel-avr」「--avr-devices」オプションの追加

        > hidspx --atmel-avr    ... Atmel社のAVRマイコンのページを開く
        > hidspx --avr-devices  ... AVR FreaksのDevicesページを開く

          私がよく利用している「Atmel 社の AVR のページ」と AVR Freaks の 
          AVR デバイス一覧のページを開く機能を追加しました。Web ブラウザの
          ブックマークに登録しておけば済むことですが、AVR マイコンを開発す
          る全ての PC にブックマークするのは手間がかかり（複数の PC を
          利用する場合）ますので、メリットはあると思います。


■2009-01-13 -rdオプションの不具合を修正
	* 一部の AVR マイコン（AT90S2313 など）で -rd オプションを利用出来
	  なかった点を改良（以下は、改良前のメッセージ）

	>hidspx -rd
	Detected device is AT90S8515.
	Fuse bits are not accessible.
	
	>hidspx -rd
	Detected device is AT90S2313.
	Fuse bits are not accessible.

■2009-01-15 -rFオプションの改良、ユーザーブックマーク機能を追加

	* avrdude -pオプションをavrdudeで指定可能なパーツ名で出力するように修正。

	* hidspx.iniにブックマーク機能を追加
	  「hidspx -- 登録した keyword」とすれば、対応の URL を開くことがで
	  き、最大 100 件まで登録できます。登録内容は、以下のコマンドで確認
	  できます。以下のように、HELP メッセージの後方に登録したブックマー
	  クが表示できます。

	> hidspx -? | more (あるいはless) で以下のように表示できます。

  === user bookmarks ===
  --avrlibc = [http://www11.ocn.ne.jp/~akibow/avr-libc-user-manual-1.4.3/]
  --avrwiki = [http://avrwiki.jpn.ph/wiki.cgi]

	  なお、この形式で ini ファイルにブックマークを登録した場合でも、従
	  来の hidspx はコメントとして扱う為、バージョンを戻した場合でも不
	  具合は生じません。

	  setup.bat でバージョンアップする時、上書きで設定した内容を失うこ
	  とがないように hidspx-save.ini に保存する仕組みを追加しました。
	  更新時、設定内容をご確認ください。

	* hidspx.iniへの登録方法
	  以下のように「;# 」に続いて、keyword URL のペアを空白で区切って登
	  録します。

============= hidspx.ini ==================
; AVR Book Marks
;
;# avrlibc   http://www11.ocn.ne.jp/~akibow/avr-libc-user-manual-1.4.3/
;# avrwiki   http://avrwiki.jpn.ph/wiki.cgi
;# hidaspx   http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2FHIDaspx00

	  keyword は、予約済みのオプションと区別できる 8 文字程度までの英字・
	  数字から構成される文字列とし、入力容易なものをお薦めします。
	  登録は利用者の使い慣れたエディタ（メモ帳も利用可能）を利用してく
	  ださい。なお、大文字・小文字は 区別します。

	  予約語は登録できませんが、以下のもの以外が利用できます。-- 以降の
	  オプションには help を除き「-」を含めていますので、英字と数字のみ
	  で登録すれば予約語との重複を避けることができます。

	  注：URL の部分に & を含む場合、^& のように ^ を追加し、空白を含む
	  URL は %20 に置き換え、さらに実行ファイルでは空白を含まない短いフ
	  ァイル形式で登録してください。

	 --pause-on-start=msg       Pause on Port Open
	 --pause-on-exit=msg        Pause on exit
	 --list-port or -p?       List COM Port
	 --list-usbasp or -pu?    List USBasp devices
	 --set-serial=XXXXXX      Set USBasp serial number
	 --help
	 --atmel-avr
	 --avr-devices

	 また、登録できるのはhttp以外も可能です。以下のように登録すれば、
	 pdfファイルの参照や実行ファイルの起動も可能です。

;# Help       file://c:/bin/HIDaspx.pdf
;# hidspx     file://c:/bin/avrx-tool.txt

	  と登録し、該当するファイルを所定の場所にコピーしておくことで、付
	  属の文書を迅速・かつ容易に参照できます。

;# avrstudio  C:\PROGRA~1\atmel\avrtoo~1\AVRSTU~1\AVRSTU~1.exe
	  この例は、Atmel 社の AVRstudio を起動することができます。

	 これらは非常に便利に使えますが、同時に危険なコマンドも登録も可能な
	 ことを意味します。hidspx.ini を更新したあとは、必ず -? で登録状態
	 を確認し、内容を理解した上でご利用ください。

■2009-01-19 	実行時間表示ほか、各種の改良

  「--」オプションの追加を容易にするため、avrspx.c内のコードを改良しました。
        この修正によって、機能の追加が容易になると思います。また、あわせて
        --new-mode （avrdude と類似の進捗表示機能）を追加しました。これがベ
        ストは考えておりませんが、数字表示よりは穏やかに画面を書き換えるた
        め、忙しさは感じません。実行時間の表示機能を追加し、処理速度の評価
        が容易になりました。

 >hidspx -rp -onul -d4
 Detected device is ATmega88.
 RD  Flash [##################################################] 8192,   5.20s
 Passed.
 Total read/write size = 8192 B / 5.25 s (1.52 kB/s)
 
 >hidspx -rp -onul -d1
 Detected device is ATmega88.
 RD  Flash [##################################################] 8192,   1.52s
 Passed.
 Total read/write size = 8192 B / 1.61 s (4.97 kB/s)


  「--new-mode-」
	hidspx.iniに書いた設定を無効化することも可能です。
	--new-mode- のように、最後に'-'をつけると、このオプションを設定しない
	という意味になります。

  「-!」ブックマーク一覧を表示します。-?での表示はヘルプ表示が見にくいと感じた
        ので、仕様を変更しました。

  「-o」書き込みファイルの指定できます。
        従来は、'>' で書き込むファイルを指定しました。現在も使えますが、オ
        プションで指定できると処理を効率化できることが確認できたので、「-oフ
        ァイル」（空白で区切らないこと）でファイル指定を可能にしました。空
        白を含むファイルは"file 123.hex"のように指定します。
        -onul と指定すると、-rp コマンドを実行して、NUL デバイスに書き込む
        ことが可能です。（速度の計測や動作テストに便利）

	その他にもヘルプ表示を変更しています。詳細は「-?」で確認してください。

■2009-01-21  -rFオプションでの表示を改良
  
  「-rF」の改良
        誤った FUSE データの書き込みは AVR チップを使用不能にする可能性があ
        ります。そこで、-rF オプションで表示するコマンド行に -q(query) オプ
        ションを追加し、チップを特定する仕様に改良しました。

 >hidspx -rF
 Detected device is ATtiny2313.
 
 DEVICE=attiny2313
 ### hidspx command line example ###
 hidspx -qtiny2313 -d10 -fL0xE4 -fH0xDF -fX0xFF

        以下のように、ターゲットの AVR マイコンが指定したチップでなければ、
        それ以降のコマンドを実行しません。

 >hidspx -qtiny26 -rp
 Detected device is ATtiny2313.
 Invalid Device tiny26 != tiny2313

  「Makefile.gcc」の修正
	hidspx-gcc.exe にアイコン情報が欠けていたのを修正

■2009-01-21 ...	HIDaspx用のファームウェアを修正
	* Linux上でmain-12.hexを利用するとエラーになり利用できないのを修正
	  この不具合は、2008年11月27日に実施した変更の副作用です。それ以前の
	  版を使っている場合には問題なく動作します。

	#define USB_CFG_HAVE_INTRIN_ENDPOINT    1
	#define USB_CFG_SUPPRESS_INTR_CODE      1
	#define USB_CFG_INTR_POLL_INTERVAL      200 //無駄なので長いinterval
	
	  この修正は、BSD OSやMac OSでの動作に対応する為の変更ですが、正しい解決方法が
	  不明なので、以前の設定に戻しました。

	#define USB_CFG_HAVE_INTRIN_ENDPOINT    0
	#define USB_CFG_SUPPRESS_INTR_CODE      0
	#define USB_CFG_INTR_POLL_INTERVAL      10
	

 Error hidWrite(): error sending control message: Device or resource busy ...
 Error hidRead(): error sending control message: Device or resource busy ...

	* avrspx-doc.txt を追加
	  kkk さんの書かれた avrsp に対する改良点の説明です。hidspx は、こ
	  の改良点の全てを継承しています。

■2009-01-23  細かな見直し

	* ヘルプメッセージのtypoを修正
	* libusbprog.batの対象ファイルの誤りを修正
	* --new-mode の進捗表示を修正（表示のズレを修正）

 >hidspx -d1 main-12.hex main-12.eep
 Detected device is ATtiny2313.
 Erase Flash memory.
 Flash memory...
 Writing   [##################################################]   2002,   0.67s
 Verifying [##################################################]   2002,   0.39s
 Passed.
 EEPROM...
 Writing   [##################################################]      7,   0.05s
 Verifying [##################################################]      7,   0.25s
 Passed.
 Total read/write size = 4018 B / 1.52 s (2.59 kB/s)

■2009-01-24 オプション指定の不具合を修正
        * -rオプションの右側にオプションを指定するとコマンドエラーになる
          （-rf, -rFコマンドを追加した時のコードが不完全でした）

■2009-01-25 コマンド表示オプションを追加
        * --show-optionsを指定すると、iniファイルを含めたオプション一式を
          コマンドライン形式で表示します（宮前さんの提案）。
          ただし、--show-options自体は表示しません。
          また、iniファイルに設定することを配慮し、--show-options-で指定を
          解除可能にしました。

■2009-01-26 コマンド表示オプションを追加
        * --show-optionsの出力は標準エラーに変更しました。
          理由は、-rpで読み出したHEXファイルへのコマンドエコーの混入を
          避けるためです。（仕様の検討が不十分でした）
          なお、-oREAD.hexでは、この混入は起きません。

■2009-03-07 「鶏と卵」の問題を解決する方法を追記しました
        * bin\firmware\egg-write.batを追加
        * 標準出力のバッファリングを行単位に変更しました（外部ツールへの配慮）
        * HIDaspx.pdfを更新

■2009-03-17 -rIオプションを追加
	* 「Engbedded Atmel AVR(R) Fuse Calculator」の新版に対応
	* hidspx_tips.pdfの追加（HIDaspx.pdfからhidspxのtipsを分離）

■2009-03-30 空のHEXファイルを表示する機能を追加
	* hidspx_tips.pdfの更新
	* fuse_en.txtを日本語向けに修正(純英語環境用はfuse_en_orig.txt)
	* 今までは、書き込みデータを含まないファイルを指定すると HELP メッセージ
	  を表示していたが、これを以下のように修正
	- この修正で、HEX ファイルは'.' を含むファイル名である（HEX以外も可）こと、
	  EEPROM 用のファイルは、.EEP(大文字小文字は問わない) の拡張子であること
	  をチェックする。（今まではファイル名の一部に含んでいれば EEPROM 用と判
	  断していたため、ABC.EEP.HEX というファイルは、EEPROM 用のファイルに解釈
	  された）

 >cat a.hex
 :00000001FF
 
 >cat a.eep
 :00000001FF
 
 これらの書き込みを実行すると
 
 >hidspx a.hex a.eep
 a.hex : HEX File is empty.
 a.eep : HEX File is empty.
 
上記の表示を行ない書き込みをスキップします。従来の版も同じ動作ですが、usage メッ
セージを表示するため、コマンド指定を間違ったと誤認する可能性があります。（私も、
このメッセージの意味を理解するのにしばらく悩みました）

