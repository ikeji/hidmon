                                                             2008年9月22日

■ はじめに

HIDaspとは、ドライバのインストールが不要のUSB接続方式のAVRライタです。

瓶詰堂さんの作成された HIDasp を元に、iruka さんと senshu が共同で改良を行
ったものです。

■ 同梱のファイルについて

このアーカイブは、HIDasp 高速化対応版であり、瓶詰堂さんの公開しているものと
は内容が異なります。現時点では、firmware, hidspx ともに互換性はありませんの
で区別して扱ってください。

なお、2 つの exe ファイル（hidspx.exe, hidspx-bcc.exe）の機能は同じです。
hidspx.exe を利用して、動作に異常を見つけた時には、hidspx-bcc.exe を使って
みてください。全角文字を含むフォルダでの作業時に効果的な場合があります。
（コマンド名は任意に改名できますが、混乱しない名前をつけてください。）

また、hidspx のコマンド名を使っていますが、これはavrspx の別名です。
HIDasp に対応することを示すと共に、avrspx の機能は全て利用可能です。しかし、
機能追加作業により、 avrspx の機能に意図しない副作用が生じる可能性もありま
す。そのため、十分な動作検証を終えるまでは、avrspx の名称をつけずに公開する
ことにしました。ご了承ください。


.\
├─bin
	libusb0.dll
	hidspx.exe			… MinGWでコンパイルしたもの
	hidspx-bcc.exe		… Borland C++ ver 5.5.1でコンパイルしたもの
	main.hex			… HIDaspのファームウェア
	hidspx.ini			… hidspxの初期化ファイル
	fuse.txt			… FUSE情報を詳細に表示する為のテキストファイル
	fuse_en.txt
	fuse_j.txt
├─firmware			… HIDaspのファームウェアソース
├─hidspx-src			… hidspxのソース(MinGW, Borland C++兼用)
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
└─pict			… 回路図と実装例

■ 準備

bin/main.hex を Tiny2313 に書き込んでください．ヒューズの設定は，外付けの 
12MHz のクリスタルに合わせ、以下のように設定します。

Low: 10001111
     ||||++++-- CKSEL[3:0] システムクロック選択
     ||++-- SUT[1:0] 起動時間
     |+-- CKOUT (0:PD2にシステムクロックを出力)
     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)

High:11-11011
     |||||||+-- RSTDISBL (RESETピン 1:有効, 0:無効(PA2))
     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
     |||+-- WDTON (WDT 0:常時ON, 1:通常)
     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
     |+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
     +-- DWEN (On-Chipデバッグ 1:無効, 0:有効)

Ext: -------1
            +-- SPMEN (SPM命令 1:無効, 0:有効)


■ 使い方

hidspx に「-ph」指定で HIDasp が利用できます。hidspx.ini に -pu を書いてお
けば、この指定は省略できます。

> hidspx -ph test.hex

-d でディレイを指定でき、数値は時間に比例します。1MHz の RC 発振モードでは
-d4 以上を指定してください。なお、この値は、高速に動作する AVR マイコンでは
小さな値を指定でき、それに伴い、R/W の速度が向上します。

irukaさんの計測結果をしめします。

コマンド	評価内容	改良前	高速化後
hidspx -d0 -rp >XX.hex	8kB Read	8.39秒	2.22秒(3,690B/秒)
hidspx -d0 test.hex	8kB Write/Verify	10.94秒	4.86秒(1.64kB/秒)
hidspx -d1 -rp >XX.hex	8kB Read	8.40秒	3.12秒(2,625B/秒)
hidspx -d1 test.hex	8kB Write/Verify	10.96秒	7.06秒(1.13kB/秒)
hidspx -d4 -rp >XX.hex	8kB Read	8.39秒	4.63秒(1,770B/秒)
hidspx -d4 test.hex	8kB Write/Verify	13.26秒	9.98秒(821B/秒)

詳細な使い方は、avrspxの解説ページをご覧ください。

■ ライセンス（瓶詰堂さんに同じ）
AVR USBに準じてGPL2とします．

■ History
2008-09-22 ... senshu, irukaが改造を実施
	高速化対応完了

