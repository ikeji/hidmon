# Windowsの標準的な環境に合わせ、TAB size = 8で編集しています。

                                                2008年 9月22日（公開開始）
                                                          ｜
                                                2009年 9月28日（最新更新）

                                      山形県立産業技術短期大学校  千秋広幸
                                 E-mail senshu(at)astro.yamatata-cit.ac.jp


        USB接続方式のドライバインストール不要なAVRライタ（HIDaspx）
        ￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣

【1】 はじめに

HIDaspx は、ドライバのインストールが不要の USB 接続方式の AVR ライタです。
hidmon コマンドや DLL を利用すれば、汎用の USB-IO としても利用できます。
Windows 98SE/2000/XP/Vista、Linux/Mac OSでの動作を確認しています。
Linux/Mac OS用のhidspxは、別途公開のアーカイブを入手してください。

瓶詰堂さんの作成された HIDasp を元に、主に iruka さんと senshu が共同で改良を行な
いました。一定の成果が得られましたので、2008年 9月 22日より公開を開始しました。
HIDaspx は、瓶詰堂さんの hidasp と同一のハードウェアですが、firmware と hidspx コ
マンド共に、ソフトウェアの互換性はありません。

※ HIDaspxは、エイチ・アイ・ディー アスペックスとお読みください。

  ********************** 更新時の注意点 **********************
  hidspx の更新時、ファームウェアの日付も更新されますが、更新履歴にファーム
  ウェアに関する記載が無い場合にはファームの更新は不要です。bin フォルダの
  setup.bat で一括更新ができます。[更新ディレクトリ名] を省略した場合には、
  C:\binにコピーする仕様です。

  XXXX\hidspx-2009-0111\bin> setup [更新ディレクトリ名] <Enter>
  ================= hidspx をセットアップします ====================
 C:\binフォルダ にコピーを希望する場合は、[Y]を大文字で入力のこと。
-+
 ↑にセットアップをしますか[Y/n]

 表示されるメッセージに従って、ご活用ください。


【2】 アーカイブの構成

最新の情報は、以下のURLを参照してください。

http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2FHIDaspx00

.\
├─ 各種ドキュメント	… BSDライセンス(*1)
│	Readme-j.txt	… このファイルです。
│	HIDaspx.pdf	… HIDaspxに関する情報集
│	hidspx_tips.pdf	… hidspx関連のノウハウ集
│	avrspx-doc.txt	… avrspxコマンドの改良過程
│	avrx-tool.txt	… hidspxコマンドの使い方
│	Changelog.txt
│	License.txt
│	Readme.txt
├─bin
│	libusb0.dll	… LGPL
│------------------------ (BSDライセンス) -----ここから-----------
│	hidspx-gcc.exe	… MinGW-GCCでコンパイルしたもの
│	hidspx.exe	… Borland C++ ver 5.5.1でコンパイルしたもの
│	hidspx.ini	… hidspxの初期化ファイル
│	fuse.txt	… FUSE情報を詳細表示する為のテキストファイル（hidspxが参照）
│	fuse_j.txt	… FUSE情報を詳細表示する為のテキストファイル（日本語表示）
│	fuse_en.txt	… FUSE情報を詳細表示する為のテキストファイル（英字表示）
│	fuse_en_orig.txt… FUSE情報を詳細表示する為のテキストファイル（英語環境用）
│	hidmon.exe	… HIDaspxのテスト時に利用
│	hidmon.dll	… HIDaspxを外部プログラムから利用する時に利用
│	hidspx-GUI.exe	… hidspx用GUIフロントエンド(実行には.NET Frameworkが必要)
│	hidspxG.exe	… hidspx用GUIフロントエンド(実行には.NET Frameworkは不要)
│------------------------ (BSDライセンス)  -----ここまで-----------
│
├─bin			… 各実行ファイル（BSDライセンス）
│  └─firmware
│------------------------ (GPL2 *2) -----ここから-----------
│			… main-12.hex（HIDaspx用）, main_libusb.hex（MacOS, Linux用）
│			… main-small.hex(USB-IO専用のファームウェア)
│			… main-RC.hex (RC発振モードのUSB-IO専用のファームウェア、実験レベル)
｜　　　　　　　　　　　… genserial.awk（シリアル情報生成ツール）
｜　　　　　　　　　　　… firmprog.bat main12.hexを書き込むためのBATファイル
｜　　　　　　　　　　　… prog-rc.bat  main-RC.hexを書き込むためのBATファイル
│------------------------ (GPL2)  -----ここまで-----------
｜
├─firmware		… HIDaspのファームウェアソース（GPL2）
├─src			… hidspxのソース(MinGW, Borland C++兼用)… BSDライセンス
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

------------------------------------------------------------------------------------
参考

*1 hidspx の元になっている avrspx や avrsp は、BSD ライセンスを採用しており、その
派生ソフト hidspx も同様です。BSD ライセンスとはオープンソースソフトウェアで使用
されているライセンス体系のひとつです。無保証・免責を宣言し、再配布する際に著作権
表示を行なうことのみを条件とする、極めて制限の緩いライセンスです。詳細は以下の URL
を参照してください。

http://ja.wikipedia.org/wiki/BSD%E3%83%A9%E3%82%A4%E3%82%BB%E3%83%B3%E3%82%B9

*2 V-USB は複数のライセンスを採用できますが、HIDaspx では無償利用可能な PID/VID
を適用し、 そのファームウェアには GPL2 を採用しています。GPL は複雑なので、詳細は
以下の URL を参照ください。

http://www.gnu.org/licenses/gpl-faq.ja.html

------------------------------------------------------------------------------------


【3】 hidspxコマンドのインストール方法

　hidspx のインストールには、特別なインストーラは不要です。bin ディレクトリにある
ファイルを実行可能なディレクトリにコピーすることで利用可能になります。（ファイル
日付は、改良により異なることがあります）

**************************** 重要 ****************************
2009/08/07  09:22                2,787 setup.bat
2009/05/21  14:29               32,780 hidmon.dll
2007/03/20  11:33               43,520 libusb0.dll
2009/05/21  14:29               16,384 hidmon.exe
2009/08/07  08:54               70,144 hidspx.exe
2009/07/16  09:32              237,056 hidspxG.exe
2009/08/07  08:54               43,008 hidspx-gcc.exe
2009/05/11  16:48               61,440 hidspx-GUI.exe
2008/09/09  12:54                9,216 usbtool.exe
2009/04/19  11:51                1,233 hidspx.ini
2009/05/21  14:29                4,936 hidmon.lib
2008/12/10  08:39               20,115 fuse.txt
2009/03/26  11:14               22,155 fuse_en.txt
2008/12/07  13:08               22,152 fuse_en_orig.txt
2008/12/10  08:41               20,115 fuse_j.txt
              15 個のファイル             607,041 バイト

**************************** 重要 ****************************

これらのファイルは、相互に関係していますので、hidspx.exe だけでなく一括してコピー
してください。

コピー漏れを防ぐ為、setup.batファイルを用意しました。

 setup.bat を実行すると c:\bin に必要なファイルをコピーします。setup c:\hidspx の
 ように指定すれば、c:\hidspx に必要なファイル一式をコピーできます。

環境変数が操作できる方は、hidspx\bin というディレクトリを PATH 環境変数に登録する
と、簡潔にコマンドを指定でき便利に利用できます。

なお、2 つの実行ファイル（hidspx-gcc.exe, hidspx.exe）の機能は同じです。数ヶ月間
のテストを行ない問題が無かったことから、Borland C++ でコンパイルした hidspx.exe
を標準実行ファイルとして公開します。動作に異常がある時には、hidspx-gcc.exe を使っ
てみてください。

===========================「補足」===========================
現在のところ、hidspx のコマンド名を使っていますが、これは avrspx の別名です。
（コマンド名は任意に変更できますが、混乱しない名前をつけてください。また、変更し
た名称での公開は混乱の元ですので、避けてください。設定ファイルは、コマンド名.ini
になります。）

hidspx というコマンド名にしているのは 既存の avrspx の利用者に混乱を与えず、HIDaspx
への対応を明示するためです。hidspx は avrspx 全ての機能を利用でき、多くの改良を行
っています。しかし、意図しない副作用（不具合）が生じる可能性も皆無とはいえません。

また、2009年以降に行った hidspx の機能拡張により、hidspx と avrsp には機能的な違
いが生じています。hidspx を avrspx として利用することは可能ですが、その逆の利用で
は出来ない場合があります。例えば、hidspx-GUI では、avrspx は利用できません。

そこで、当面は、hidspx の名称で公開を続けます。

==============================================================

【4】 準備

circuit フォルダある回路図（HIDaspx.png）を参考にハードウェアを製作します。なお、
専用基板も入手可能 (http://www.wsnak.com) です。回路図にも明記していますが、発振
子にはクリスタルを実装してください。セラミック振動子では回路が要求する精度と安定
性を満たすことができません。
この後で、bin/main-12.hex を Tiny2313 に書き込んでください。ヒューズ設定は，外付
けの 12MHz クリスタルに合わせ、以下のように設定します。ただし、外部に発振器を接続
していない時にこの設定を行うと、発振器を接続するまで ISP 方式のライタによる読み書
きが出来なくなりますので、ご注意ください。

Low: 11111111 (0xFF)
     ||||++++-- CKSEL[3:0] システムクロック選択
     ||++-- SUT[1:0] 起動時間
     |+-- CKOUT (0:PD2にシステムクロックを出力)
     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)

High:11-11011 (0xDB)
     |||||||+-- RSTDISBL (RESETピン 1:有効, 0:無効(PA2))
     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
     |||+-- WDTON (WDT 0:常時ON, 1:通常)
     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ(*2)
     |+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
     +-- DWEN (On-Chipデバッグ 1:無効, 0:有効)

Ext: -------1 (0xFF)
            +-- SPMEN (SPM命令 1:無効, 0:有効)


※ 初期の HIDaspx では、PD2 からのクロック出力を有効にしていました。
   従来通り、PD2 をクロック出力にしたい場合は、Low=0xBF と設定します。

   現在は、PD2 を開放すると AVR ライタモード、GND に接続すると USB-IO として機能
   します。ライタモードでは PB2 （LED 兼用) 端子から 1MHz の矩形波を出力します。
   PD2 からの12MHz クロックよりも広範で安定なクロック信号として利用できます。

　ファームウェアの書き込みに hidspx を利用できる方は、工場出荷状態の ATtiny2313
に対し、以下のコマンドを実行することで FUSE 設定が可能です。ただし、外部に発振器
を接続していない時にこの設定を行うと、発振器を接続するまで ISP 方式のライタによる
読み書きが出来なくなりますので、ご注意ください。

> hidspx -d4 -fL0xff -fH0xdb -fX0xff <ENTER>

【FUSE情報の補足】… 以下のFUSE情報の変更は、十分注意してください。
*1 SPIEN (1:ISP禁止, 0:ISP許可)
   このbitを1にするとISPが無効になるので、hidspxでは1のWriteを行いません。

*2 RSTDISBL (RESETピン 1:有効, 0:無効)
   RSTピンを無効(0)でI/Oに割当てると、ISPが使用できなくなります。


【5】 使い方

　以下に簡単な使用例を紹介します。詳細な使い方は、avrx-tool.txtをご覧ください。

※ 1030版以降は、FUSE Lowを0xFFに設定することをお薦めします。

■ USB-IOとして利用する
USB-IO として利用する場合には、PD2 を GND に繋いでください。USB-IO モードの状態で
hidspx を利用すると、以下のような警告を表示します。これは、USB-IO として利用して
いる HIDaspx を誤ってプログラマ（ライタ）として操作することを避ける為です。

 >hidspx -rp
 Error: HIDaspx is USB-IO mode.
 HIDaspx(0000) not found.


■ AVRライタ（Programmer）として利用する
USB-IO 機能を利用しない場合には、PD2 周りのハードウェア変更は不要です。
なおライタモードでは、PB2 を ターゲットマイコンの 救済用クロック (1MHz) を出力す
るため、PB2(PWR LED) の LED はやや薄暗く点灯します。

hidspx で「-ph」を指定すれば HIDaspx が利用できます。HIDaspx 専用に使う場合には、
hidspx.ini に -ph を書いておけば、この指定は省略できます。

> hidspx -ph -d4 test.hex

hidspx.ini では -d4 を設定しています。-d でディレイを指定でき、数値は時間に比例し
て遅くなります。1MHz の RC 発振モードでは -d4 以上を指定してください。
この値は、高速に動作する AVR マイコンでは小さな値を指定でき、16MHz 以上の周波数で
動作する AVR マイコンには -d0 が指定できます。指定値が小さいほど、R/W の速度が向
上します。

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

　AT90S2313 のように、ページ書込み機能の無い AVR マイコンでは書込みに時間がかかり
ます。USB 2.0 HUB を介せずに接続して書込みを行った結果です。

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

このように旧タイプの AVR マイコンも利用できますが、迅速な書込みを希望する場合には、
新タイプのマイコンを採用してください。同じメモリサイズの ATtiny2313 では 1秒程度
で完了します。

これらの結果から、USB HUB が非常によい仕事をしていることがわかります。旧タイプの
AVR マイコンに限らず、ほとんどの場合、処理速度は格段に向上します。
hidmon で 15kB/s を超える環境を用意すれば速度向上につながります。USB-HUB を入手し、
それを介して利用してみてはいかがでしょうか。

===============================================================================

【7】 利用上の注意点

　2009年 1月以降の版では、「-ri, -rd, --atmel-avr, --avr-devices」などのコマンド
を使うことで、Web ブラウザを使った機能を提供していますが、接続先のサービスが停止
している場合もあり、その時には白紙のページが表示されたり、エラーが表示される場合
があることをご理解ください。

【8】各ソフトウェアのライセンス
hidspx は BSD ライセンス、HIDaspx のファームウェア（瓶詰堂さんに同じ）は V-USB に
従い GPL2 です。なお、V-USB のライブラリに含まれる License.txt には、以下の記述が
あります。
--------------------------------------------------------------------------------
OBJECTIVE DEVELOPMENT GmbH's V-USB driver software is distributed under the
terms and conditions of the GNU GPL version 2 or the GNU GPL version 3. It is
your choice whether you apply the terms of version 2 or version 3. The full
text of GPLv2 is included below. In addition to the requirements in the GPL,
we STRONGLY ENCOURAGE you to do the following:

(1) Publish your entire project on a web site and drop us a note with the URL.
Use the form at http://www.obdev.at/vusb/feedback.html for your submission.

(2) Adhere to minimum publication standards. Please include AT LEAST:
    - a circuit diagram in PDF, PNG or GIF format
    - full source code for the host software
    - a Readme.txt file in ASCII format which describes the purpose of the
      project and what can be found in which directories and which files
    - a reference to http://www.obdev.at/vusb/

(3) If you improve the driver firmware itself, please give us a free license
to your modifications for our commercial license offerings.
--------------------------------------------------------------------------------
「参考訳」

OBJECTIVE DEVELOPMENT 社の V-USB ドライバーソフトウェアは、GNU GPL バージョン 2
または GNU GPL バージョン 3 の条件のもとで配布されます。 バージョン 2 またはバー
ジョン 3 の用語を適用するかは選択できます。 GPLv2 のフルテキストは下に含まれます。
GPL の要件に加え 以下のことに従ってください。

(1) URL によってウェブサイトの全体のプロジェクトを出版し、私達に手紙を書いてくだ
    さい。提出のための http://www.obdev.at/vusb/feedback.html でフォームを使って
    ください。

(2) 標準的な公開は、最低でも以下のものを含めてください。
    - PDF、PNG、またはGIFフォーマットの回路図
    - ホストのソフトウェアの完全なソースコード
    - プロジェクトの目的を説明する ASCII フォーマットの Readme.txt ファイルと
      ファイル構成の説明
    - http://www.obdev.at/vusb/への参照

(3) もしドライバーファームウェア自身を改善した場合には、私達の商業用のライセンス
    提供物のための部分修正に関し、私達に自由なライセンスを与えること。
--------------------------------------------------------------------------------


【9】 謝辞
　HIDaspx の開発には、多くの方から協力をいただきました。評価の定まらない開発直後
の時期に支援・協力をいただいたのは以下の方々です。心より感謝の意を表します。

    ・瓶詰堂さん  …… HIDaspの開発者です。HIDaspなくしてはHIDaspxはありません。
    ・irukaさん   …… 高速化（ファームウェアとhidspx）はirukaさんの成果です。
    ・kugaさん    …… 旧AVRのサポートとコンパクト化、PD5による再接続機能の実現。
    ・RAINさん    …… 新アイコンの作成と各種の動作テストで協力していただきました。
    ・akakusaさん …… USB接続の低価格AVRライタを実現して欲しい、とう実現不可能と
                        思われた難題の提起と動作検証で協力していただきました。
    ・岩永さん    …… 複数のHIDaspxの製作し、実用性を検証していただきました。
    ・kumanさん   …… 早期から注目していただき、多くの助言をいただきました。
    ・TADさん、宮前さん、mituさん、すんさん、kawanaさん
                   …… 動作報告や各種のコメントをいただきました。
     ※ （順不同）

  ----- * ----- * ----- * ----- * ----- * ----- * ----- * ----- * -----

【付録】 開発者向けのメモ

[0] 2008-10-17～10-22の作業内容

　複数台の HIDaspx を一台の PC に混乱無く利用できる機構を実現する為、シリアル番号
の生成ツールや hidspx, hidmon の機能拡張を行いました。この機能を実現するため、以
下の作業を行っています。

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

　このツールは gawk(GNU awk というインタープリタ） を使って動作します。AVR マイコ
ンの利用者に広く普及している WinAVR には、gawk.exe が付属していますが、gawk を利
用できない方は、バグが少なく単独で動作する、木村さんの移植版 gawk を入手してくだ
さい。

2008年7月4日 gawk 3.1.6 日本語版バイナリバグfix
GAWKの改良版の入手先 ⇒ http://www.kt.rim.or.jp/~kbk/

http://www.kt.rim.or.jp/%7ekbk/cgi-bin/download.cgi?dl-file1=gawk-mbcs-win32-20080704.zip

入手した gawk-mbcs-win32-20080704.zip を展開し、gawk.exe を hidspx.exe と同じディ
レクトリにコピーします。

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

■2009-04-03 hidspx-GUI.exeを追加
	* ゆきの研究室で公開されている「avrdude-GUI」を参考に作成しました。
	* まだ出来たばかりでチェックも不十分ですので、FUSEの書き込みは、
	  もう少し動作テストを行ってから有効にします。

■2009-04-06 hidspx-GUI.exeを追加
	* FUSEの書き込みとHEXファイルとhidspx.exeのドロップを可能にしました。
	* ISPではFUSEを操作できない場合でも認識を可能にしました（AT90S2313など）。


■2009-04-07 hidspx-GUI.exeのBUGを修正
	* コマンド起動時のファイル指定の不具合を修正
	* 起動直後はFUSE READボタンをフォーカスする

■2009-04-09
	hidspx-GUI.exeの機能を拡張
	* 実行の様子を示すログウインドウの追加
	* 各種の修正

	hidspx-gcc.exeをhidspx-GUIとの組合せた時に発生する表示の不具合を修正
	（'#'を出力する都度、fflushすることで対応）

■2009-04-10
	hidspx-GUI.exe
	* コマンド引数にてワイルドカード文字を展開
	* ログ窓の表示フォントを等幅フォントに変更
	* エラー処理を強化

	hidspx.exe, hidspx-gcc.exe
	* --new-mode を標準にした（旧モードは --new-mode- を指定する）

■2009-04-11
	hidspx.exe, hidspx-gcc.exe
	* -rph, -reh （16進ダンプ機能の追加）

使用例
>hidspx -reh
Detected device is ATtiny2313.
EEPROM...
Reading   [##################################################]    128,   0.22s
Passed.

0000  1F C0 1F C1 37 C0 36 C0 - 35 C0 35 C0 33 C0 32 C0  |    7 6 5 5 3 2 |
0010  31 C0 30 C0 2F C0 2E C0 - 2D C0 2C C0 2B C0 2A C0  |1 0 / . - , + * |
0020  29 C0 28 C0 27 C0 02 23 - 24 23 22 22 02 23 23 03  |) ( '  #$#"" ## |
0030  22 25 05 02 23 24 23 22 - 22 02 23 26 22 23 44 00  |"%  #$#"" #&"#D |
0040  11 24 1F BE CF ED CD BF - 10 E0 A0 E6 B0 E0 E2 E3  | $              |
0050  F3 E0 03 C0 C8 95 31 96 - 0D 92 A4 37 B1 07 D1 F7  |      1    7    |
0060  10 E0 A4 E7 B0 E0 01 C0 - 1D 92 A6 38 B1 07 E1 F7  |           8    |
0070  1A D1 5E C1 C5 CF 1F 92 - 0F 92 0F B6 0F 92 11 24  |  ^            $|
Total read/write size = 128 B / 0.30 s (0.42 kB/s)

■2009-04-12
	hidspx-GUI.exe (ver 0.3.2)
	* -rph, -reh などの表示で極端に速度が低下するのを改善
          大量の文字列連結を効率化しました（2桁の速度向上）
        * 高速化の副作用（文字化け）の改善
        * ログテキスト窓の表示を適正化
	* エラー処理を修正

■2009-04-13
	hidspx.exe, hidspx-gcc.exe
	* -rph, -reh （64kB超の16進ダンプに対応）

	hidspx-GUI.exe (ver 0.3.6)
	* Flashの16進ダンプ用のRead(SCRN)ボタンを追加
	* CLR LOGボタンを追加
	* TAB オーダーの適正化
	* ログテキストエリアの背景色を変更

■2009-04-15
	hidspx.exe, hidspx-gcc.exe
	* -rph, -reh （ASCII文字表示形式を変更、上記のスタイルに戻した）

	hidspx-GUI.exe (ver 0.4.2)
	* Simpleモードでの処理を強化
	1. Simpleモードでも違和感のないボタン配置に工夫しました。
	2. Simpleモードは終了時に記憶します。
	3. SimpleModeでは、「隠すのではなく、機能を無効にする」に変更
	4. Simpleモードでは、Flash/EEPROMの書き込みを判断して書き込む

	* 共通の改良点
	1. 5秒以上時間を要する場合、ボタン操作をロックする
	2. hidspx実行中でも、定期的に画面を再描画する
	3. 起動直後にライタが接続されていれば、AVRマイコンを自動で認識
	4. Flash/EEPROMの両方に[HEX dump]...画面への表示を追加。
	5. ボタンをスリム化し、ログ表示エリヤを拡大（全体の大きさは同じです）
	6. FuseCalcの新版対応のExtFuse表示を追加
	7. エラーの検出を強化(-rp, -re にて例外処理を追加)

■2009-04-19
        setup.batの内容を修正（hidspx_tips.pdfファイルもコピーする）
        hidspx.ini user_bookmarkのリンク切れを修正

	hidspx.exe, hidspx-gcc.exe
	* FuseやLock bit書き込み時、実際に書き込んだ値を表示する

	hidspx-GUI.exe (ver 0.4.7)
        1. FuseのRead時は、-rfコマンドも合わせて実行
        2. Fuse Calcは、hidspx 内部のコマンドを呼び出す（mega644p対策）
        3. ボタンの大きさを均等化（イメージ貼り付けにより、大きさを画一化）
        4. 一時ファイルが消えない場合があったのを修正
        5. Command Executeのコマンド補完を実現

■2009-04-20
	hidspx-GUI.exe (ver 0.4.8)
        1. Command Executeにて、user bookmarkを指定時の時間待ちを追加（約3秒間）


■2009-05-25
	説明文の見直し（主にライセンス関係）

■2009-06-23
	hidspx-gcc.exeにて、-wオプションを指定した場合、メッセージ出力が遅くなるのを修正
	hidspxGコマンドで、-w3のように指定した場合、内部エラーになるのを修正

■2009-08-07
	RST ピンを無効化ビットの書き込みをチェックし、警告します。RST ピンの無効
	化（ポートへの割り付け）は、ISP （AVRSIP-mkII や hidspx での操作）が無効
	になるので、これを避けるため、チェック機構を追加しました。
	※ IO ピンが不足し、何としても IO ピンに割当てたい場合に限り、この書き込
	   みを行ってください。この FUSE 情報を書き込むと ISP は無効になり、解除
	   には高電圧パラレルプログラマが必要になります。

	例）
	>hidspx -fH0x07
	Detected device is ATtiny26.
	WARN: RST PIN disable detected. If you hope for the writing,
	Enter the -f#H0x07 option.

	-f#のように指定すれば、この書き込みを実行できる。（細心の注意が必要です）

	なお、RSTDISBLビットは少ピンAVRマイコンにのみ、実装されています。
	※ この改良に関しては、je1htm氏とkuga氏、kawana氏に助言をいただきました。

■2009-08-17
	(1) AVR マイコンのいくつかは DWEN(debugWire ENable) という FUSE ビット が
	    利用できます。debugWire は、AVR Dragon などデバッグツールを利用するこ
	    とで効率的な開発が可能になります、しかし、この FUSE ビットを書き込む
	    と ISP （In Sysytem Programming）が無効になり、簡易な AVR ライタ
	    （AVRISP-mkII や hidspx など） からの操作が不能になります。

	    hidspx を利用して AVR マイコンのプログラム開発を行う場合には DWEN ビ
	    ットを有効にする必要はなく、DWEN ビットをプログラムできるのは危険と考
	    えました。そこで、DWEN ビットのプログラムを禁止する機構を追加しました。
	    ※ DWEN ビットの解除には、AVR Dragon や高電圧パラレルプログラマが必要
	       です。

	例）
	> hidspx -rf

	Detected device is ATtiny2313.
	:
	High:11-11101
	     |||||||+-- RSTDISBL (RESETピン 1:有効, 0:無効(PA2))
	     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
	     |||+-- WDTON (WDT 0:常時ON, 1:通常)
	     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
	     |+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
	     +-- DWEN (On-Chipデバッグ 1:無効, 0:有効)
	:
	>hidspx -fH0x5d ... DWENビットのプログラムを指示
	Detected device is ATtiny2313.
	WARNING: ISP disable FUSE bit (DWEN) detected, Unprogrammed DWEN bit.
	Fuse High byte was programmed (0xDD).
	⇒ DWENビットはプログラムされず、無視する(0xddを書き込んでいる)

	※ この改良に関しては、kuga氏、すん氏に助言をいただきました。

	(2) hidspx-GUI.exe (ver 0.5.0) の改良
	    - メニューバーの処理をサポート
	    - -dオプションの扱いを修正

	(3) hidspxG.exe の改良
	    - -dオプションの扱いを修正

■2009-08-26
	(1) vusb-20090822.tar.gz にて提供されるusbdrvに差し替えました。
	    この変更により、HIDaspx用ファームウェアが更新されています。
	(2) hidspxG.exeとhidspx-GUIの複数起動時は '1' を返し、終了します。
	    （今までは、不定の値を返していました）

■2009-09-16
	(1) 同梱のPDFファイルの内容を刷新しました。URLのリンク部分を点検し、
	    クリックすることで、該当のページを開くことができるように配慮しました。
	(2) 添付回路図を見直し、誤解の生じにくいものに修正しました。
	(3) hidspx-GUI.exeのログ窓にて、右クリックメニューを有効にしました。

■2009-09-25
	(1) hidspx の -e コマンドを他のFUSEオプションと同様の扱いにしました。
	    従来は、-e を実行すると、hidspx の処理を終了していましたが、この版か
	    らはエラーが起きない限り、継続して処理を行います。この変更により、こ
	    れは、avrdude との互換性が増し、avrdude からの移行を容易になりました。

	(2) -rF オプションでのavrdude向けの表示を変更しました。Efuseの表示がhidspx
	    の場合と異なる点に注目してください。

	>hidspx -rF
	Detected device is ATmega168.

	DEVICE=ATmega168
	### hidspx command line example ###
	hidspx -qmega168 -d10 -fL0xFF -fH0xDD -fX0xF8
	                                        ~~~~~
	### avrdude command line example ###
	avrdude -cavrdoper -Pstk500v2  -pm168 -u -Uflash:w:main.hex:i \
	 -Ulfuse:w:0xff:m -Uhfuse:w:0xdd:m -Uefuse:w:0x00:m
	                                             ~~~~~

	(3) hidspx-GUI.exeのEfuse右の表示をavrdude互換表示を行います。
	    （従来はマスク値でしたが、わかりにくかったので改めました）

■2009-09-28
	(1) -l<hex> により、Lock bitを設定時、未定義ビットの扱いをavrdudeと
	    同様に、'0' として扱ってもエラーにはしない仕様にしました。
	    設定されている値の確認は -rl オプションで確認できます。

	>hidspx -rl （最後のカラムに表示します）
	Detected device is ATmega168.
	ATmega168 FF:FF DD:DF F8:07 CF

	(2) --show-options でオプション一覧を表示する時、空白を含む引数は、
	    ""で囲むようにしました。（CMDへのCopy and Pasteを可能にする為）

	※ バージョン表記を以下のように改めました。（b11.3 => b11.4）
	hidspx (b11.4) by t.k & senshu, Borland C++ 5.5.1, Sep 28 2009

