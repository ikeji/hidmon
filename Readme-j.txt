# Windowsの標準的な環境に合わせ、TAB size = 8で編集しています。

                                                2008年 9月22日（公開開始）
                                                          ｜
                                                2011年 7月20日（最新更新）

                                      山形県立産業技術短期大学校  千秋広幸
                                 E-mail senshu(at)astro.yamatata-cit.ac.jp


            USB 接続方式の AVR ライタ（HIDaspx）用ファームウェアと
                      制御ソフトウェア「hidspx」について
        ￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣

■ 複数台のHIDaspxを所有する方へ（お願い）

--------------------------【重要】--------------------------
複数の HIDaspx を利用する場合には、シリアル番号設定は非常に重要です。また、複数
を利用しない場合でも、複数個を所有する方は以下を熟読ください。

【同一シリアル番号設定の問題点】
一台のパソコンに同一の VID/PID, シリアル番号の HIDaspx を 2台以上挿入すると不具
合が生じます。 Windows のバージョンにより、その挙動は異なりますが、最も普及して
いると思われる Windows XP においては、「挿入した瞬間 Windows が再起動、あるいは
再起動を促すメッセージを表示する」という報告があります。この現象は、USB-IO、AVR
ライタモードに関係なく発生します。AVR ライタと USB-IO を各々 1台以上同一の PC
で利用する場合などは、以下の対策を行ってください。

【対応策】
異なるシリアル番号を設定することでこの問題は解決でき、同時に使い分けも可能にな
ります。複数台の HIDaspx を所有する方は、重複しない 4 桁のシリアル番号を設定後
に利用してください。

【シリアル番号の設定方法】
「[3] シリアル番号生成ツールの使い方」をご覧の上、設定してください。
--------------------------【重要】--------------------------

【1】 はじめに

HIDaspx は、ドライバのインストールが不要の USB 接続方式の AVR ライタです。
hidmon コマンドや DLL を利用すれば、汎用の USB-IO としても利用できます。
Windows 98SE/2000/XP/Vista/7、Linux/Mac OSでの動作を確認しています。
Linux/Mac OS用のhidspxは、別途公開のアーカイブを入手してください。

瓶詰堂さんの作成された HIDasp を元に、主に iruka さんと senshu が共同で改良を行
ないました。一定の成果が得られましたので、2008年 9月 22日より公開を開始しました。
HIDaspx は、瓶詰堂さんの hidasp と同一のハードウェアですが、firmware と hidspx
コマンド共に、ソフトウェアの互換性はありません。

※ HIDaspxは、エイチ・アイ・ディー アスペックスとお読みください。

  ********************** 更新時の注意点 **********************
  hidspx の更新時、ファームウェアの日付も更新されますが、更新履歴にファームウェ
  アに関する記載が無い場合にはファームの更新は不要です。bin フォルダの setup.bat
  で一括更新ができます。[更新ディレクトリ名] を省略した場合には、C:\bin にコピー
  する仕様です。

  XXXX\hidspx-2009-0111\bin> setup [更新ディレクトリ名] <Enter>
  ================= hidspx をセットアップします ====================
 C:\binフォルダ にコピーを希望する場合は、[Y]を大文字で入力のこと。
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
│	libusb0_.dll	… LGPL(エラー時にはlibusb0.dllに改名して利用すること)
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
│			… main-12.hex（HIDaspx用, Windows/Mac/Linux兼用）
｜　　　　　　　　　　　… regserial.awk（シリアル番号生成ツール）
｜　　　　　　　　　　　… addserial.awk（シリアル番号生成ツール）
｜　　　　　　　　　　　… hexcleanup.bat  （生成したファイルのみを削除するツール）
｜　　　　　　　　　　　… firmprog.bat main12.hexを書き込むためのBATファイル
｜　　　　　　　　　　　… prog-rc.bat  main-RC.hexを書き込むためのBATファイル
│------------------------ (GPL2)  -----ここまで-----------
｜
├─firmware		… HIDaspのファームウェアソース（GPL2）
｜
├─src 		… hidspxのソース(MinGW, Borland C++兼用)
｜    └─libusb	… 元になった avrspxはBSDライセンスです。しかし、以下のように
｜        ├─bin	   libusb-win32のDLLをリンクしている点に注意してください。
｜        │  ├─amd64
｜        │  ├─ia64
｜        │  ├─w2k
｜        │  └─x86
｜        ├─examples
｜        ├─include
｜        └─lib	… GPL3ライセンス
｜            ├─bcc
｜            ├─dynamic
｜            ├─gcc
｜            ├─msvc
｜            ├─msvc_i64
｜            └─msvc_x64
｜
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
2010/06/28  10:18                2,979 setup.bat
2010/02/14  14:38               34,655 hidmon.dll
2010/06/11  20:43               37,376 libusb0_.dll
2010/04/13  12:25               32,256 hidmon.exe
2010/08/04  09:19              141,312 hidspx.exe
2010/02/15  18:17              584,192 hidspxG.exe
2010/08/04  09:19               81,408 hidspx-gcc.exe
2010/07/20  09:25               61,440 hidspx-GUI.exe
2009/11/16  16:53               17,408 usbtool.exe
2009/04/19  11:51                1,233 hidspx.ini
2010/02/14  14:38                4,936 hidmon.lib
2010/06/17  20:13               20,968 fuse.txt
2010/06/17  19:15               23,070 fuse_en.txt
2010/06/17  20:13               20,968 fuse_j.txt
**************************** 重要 ****************************

これらのファイルは、相互に関係していますので、hidspx.exe だけでなく一括してコピー
してください。(libusb0_.dll は、本来、libusb0.dll ですが、ドライバの重複を避け
るため、この名称で配布します)

コピー漏れを防ぐ為、setup.batファイルを用意しました。

 setup.bat を実行すると c:\bin に必要なファイルをコピーします。setup c:\hidspx
 のように指定すれば、c:\hidspx に必要なファイル一式をコピーできます。

環境変数が操作できる方は、hidspx\bin というディレクトリを PATH 環境変数に登録す
ると、簡潔にコマンドを指定でき便利に利用できます。

なお、2 つの実行ファイル（hidspx-gcc.exe, hidspx.exe）の機能は同じです。日本語
環境での不具合を回避するため、Borland C++ で作成した hidspx.exe を標準実行ファ
イルとして公開します。hidspx.exe の動作に異常がある時には、hidspx-gcc.exe を使
ってみてください。

===========================「補足」===========================
現在、hidspx のコマンド名を使っていますが、これは avrspx の別名です。（コマンド
名は任意に変更できますが、混乱しない名前をつけてください。また、変更した名称で
の公開は混乱の元ですので避けてください。設定ファイルは、コマンド名.ini になりま
す。）

hidspx というコマンド名にしているのは 既存の avrspx の利用者に混乱を与えず、
HIDaspx への対応を明示するためです。hidspx は avrspx 全ての機能を利用でき、多く
の改良を行っています。しかし、意図しない副作用（不具合）が生じる可能性も皆無と
はいえません。

また、2009年以降に行った hidspx の機能拡張により、hidspx と avrsp にはかなり機
能に違いが生じています。hidspx を avrspx として利用することは可能ですが、その逆
の利用では出来ない場合があります。例えば、hidspx-GUI では、avrspx は利用できま
せん。

そこで、今後も hidspx の名称で公開を続けます。

==============================================================

【4】 準備

circuit フォルダある回路図（HIDaspx.png）を参考にハードウェアを製作します。なお、
専用基板も入手可能 (http://www.wsnak.com) です。回路図にも明記していますが、発
振子にはクリスタルを実装してください。セラミック振動子では回路が要求する精度と
安定性を満たすことができません。
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
　HIDaspx の開発には、多くの方から協力をいただきました。評価の定まらない開発直
後の時期に支援・協力をいただいたのは以下の方々です。心より感謝の意を表します。

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

　複数台の HIDaspx を一台の PC に混乱無く利用できる機構を実現する為、シリアル番
号の生成ツールや hidspx, hidmon の機能拡張を行いました。この機能を実現するため、
以下の作業を行っています。

 1. シリアル番号生成ツールの作成（gawkで記述）
 2. hidspxコマンドの-ph[:XXXX]にシリアル番号指定機能を追加
 3.          〃   に -ph? でリストできる機能を追加
 4. hidmon にも同様の機能を追加
 5.          〃   に -p? でリストできる機能を追加
 6. hidmon.dllに シリアル番号を指定できる機能を追加
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

[3] シリアル番号生成ツールの使い方

HIDaspx 用のシリアルデータ（HEX ファイル）を生成するツールを用意しました。

　このツールは gawk(GNU awk というインタープリタ） を使って動作します。AVR マイ
コンの利用者に広く普及している WinAVR には、gawk.exe が付属していますが、gawk
を利用できない方は、バグが少なく単独で動作する、木村さんの移植版 gawk を入手し
てください。

2009 年11月24日版 (最新) gawk 3.1.7 日本語版バイナリ GAWKの入手先
 ⇒ http://www.kt.rim.or.jp/~kbk/gawk-3.1/

http://www.kt.rim.or.jp/%7ekbk/cgi-bin/download.cgi?dl-file1=gawk-mbcs-win32-20091124.zip

入手した gawk-mbcs-win32-20091124.zip を展開し、gawk.exe を hidspx.exe と同じデ
ィレクトリにコピーします。（タイムスタンプは変わることがあります）

 2007/10/22  15:50              280,351 ChangeLog
 2007/07/16  23:59               35,147 COPYING
 2008/07/02  16:47              189,434 mbc-diff
 2007/09/25  14:58               83,133 NEWS
 2008/07/03  16:51              729,088 gawk.exe
 2006/12/01  14:26               53,248 igawk.exe
 2008/07/03  17:22                9,588 readme.ja

2008-1028 版以降、「-v adr= 」を指定することで、任意のアドレスに対するシリアル
情報を生成でき、また、大量のシリアル番号生成も正規表現を使って、簡単に指定でき
ます。

usage: gawk -f regserial.awk -v t={aspx|mon88} 生成パターン
￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣

使用例1：
 > gawk -f regserial.awk 0001

のように実行すると、0001 のシリアル番号を設定する HEX ファイル（_0001.hex）を生
成します。

使用例2：0000～0099のシリアル番号のファイルを一括して生成します。[0-9]
         は、0～9の何れかの数字を意味します。

 > gawk -f regserial.awk 00[0-9][0-9]


使用例3：英大文字の指定も可能です。小文字は大文字に変換します。
※　文字列を大文字化している理由は、Windows のファイルシステムが、大文字小
  文字を区別しないことによるファイルの上書きを避けるためです。この方法でシ
  リアル番号が不足する場合は、生成するファイル名を工夫し、小文字でも重複し
  ないファイル名の生成を行う必要があります）

 > gawk -f regserial.awk ABCD

なお、正規表現に不慣れな方は、「-v d=1」を指定すれば、生成するファイル名を
確認できますので、事前に確認すれば、希望しないファイルの生成を回避でします。


	>gawk -v d=1 -f regserial.awk [A-Z]000
	regserial.awk: _A000.hex
	regserial.awk: _B000.hex
	regserial.awk: _C000.hex
	regserial.awk: _D000.hex
	regserial.awk: _E000.hex
	regserial.awk: _F000.hex
	regserial.awk: _G000.hex
	regserial.awk: _H000.hex
	regserial.awk: _I000.hex
	regserial.awk: _J000.hex
	regserial.awk: _K000.hex
	regserial.awk: _L000.hex
	regserial.awk: _M000.hex
	regserial.awk: _N000.hex
	regserial.awk: _O000.hex
	regserial.awk: _P000.hex
	regserial.awk: _Q000.hex
	regserial.awk: _R000.hex
	regserial.awk: _S000.hex
	regserial.awk: _T000.hex
	regserial.awk: _U000.hex
	regserial.awk: _V000.hex
	regserial.awk: _W000.hex
	regserial.awk: _X000.hex
	regserial.awk: _Y000.hex
	regserial.awk: _Z000.hex
	Start adr=0x0068,   26 generated.

このコマンド指定では、26個のシリアル情報が得られることがわかります。

こうして生成したシリアル番号ファイルを main.hex のファームと同時に書き込めば、
HIDaspx はそのシリアル番号を持ったものになります。

書き込み例）

 > hidspx main.hex _0001.hex

※ 先頭文字に'_'を追加しましたので、以下のような操作が可能です。
 > del _*.hex
 で一括してシリアル番号用のファイルを削除できます。

使用例4：
 > gawk -f regserial.awk -v t=mon88 0001

bootmon88用のシリアル番号を生成します。

使用例5：
 > gawk -f regserial.awk -v t=1234 0001

1234(10進数)番地から始まる4桁のシリアル番号を生成します。（主に開発者用）


[3.1] もう一つのシリアル番号生成ツール… addserial.awk

regserial.awk はシリアル番号だけの HEX ファイルを生成しますが、hidspx 用の GUI
ツールは、単一の HEX ファイルのみに対応であり、シリアル情報の書き込みには利用で
きません。そこで、2010年 2月 10日の版から、addserial.awk を追加しました。使い方
は regserial.awk と同じですが、HIDaspx 用の HEX ファイルを含む HEX を生成できま
す。

使用例）
bin\firmware>gawk -f addserial.awk 1 3 <Enter>
             ~~~~~~~~~~~~~~~~~~~~~~~~~
Start adr=0x0068,    3 serial file(s) Created.

bin\firmware>dir
             ~~~
:
2010/02/10  12:47                5,750 _main-12-0001.hex
2010/02/10  12:47                5,750 _main-12-0002.hex
2010/02/10  12:47                5,750 _main-12-0003.hex

0001～0003のシリアル番号を含むHIDaspx用のHEXファイルが生成できます。
なお、生成したファイルのみを一括削除する hexcleanup.bat も追加しました。

【重要】
実習などで、大量に HIDaspx を作成する場合には、必ずユニークなシリアル番号を追加
した HEX ファイルを書き込んでください。同一シリアル番号の利用は OS の誤動作を引
き起こす場合があります。


[3.2] hidspxでのシリアル番号指定方法

複数台の異なるシリアル番号を持った HIDaspx を使って動作の確認を行いました。
この機能により、複数の HIDaspx を区別して利用できます。（一台だけで利用する場合
には、シリアル番号設定は不要です。）

> hidspx -ph:0001  （hidspx -ph1 でも同じ意味になります）

↑のように指定すると複数の HIDaspx を区別して使うことができます。なお、-ph と指
定で初期値の 0000 を選択します。シリアル番号を設定しない場合には、従来通りの利
用が可能です。

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


    2 と 3 は一つにできますが、ファイルを削除するコマンドは、分離するのが無難と
    考えています。この変更で不都合がある場合には、ご指摘ください。

    また、iruka さんも書いていますが、現在、全ての機能を有効にしても 2044 バイ
    トであり、2048 以内に収まっています。

    * senshu 2008-10-18 (土) 20:23:54 New
      hidspx と firmware の整合性を再確認してみましたが、LED の制御コマンドの有
      無の判定と LED 制御のファームウェアの部分に若干の疑問点があります。

      サイズが足りなかったので、コメントアウトした部分があるのですが、これは整
      理し、LED 制御は MONITOR コマンドに統一し、ISP_CMD は必ず使えるようにする
      のがよいと考えています。

      色々検討した結果、以下の仕様にしたいと考えています。（1017版も同様）

      (1) ISP_CMD は必ず組み込む
      (2) ISP_CMDではLEDの制御は無効（ファームに任せる）
      　　⇒ ライタ制御プログラム(hidspx)で指定しても無視する
      (3) 2個のLEDをPCから制御する場合、ISP_CMDではなくMONITOR機能を利用する

      この仕様にすれば、ファームもコンパクトに維持でき、混乱を避けられます。
      問題は 9月に公開したファームを使っている場合ですが、これは新しいものに更
      新をお願いすることになります。


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
	        * -ph:XXXX のシリアル番号をサポート
	        * firmwareソースのクリーンアップを実施
	        * bin フォルダにシリアル番号生成ツール(regserial.awk)を追加
	        * 新アイコンに変更(RAINさん作成)

■2008-10-21    * シリアル番号生成ツールの強化
	        * 回路図ファイルを更新

■2008-10-22    * シリアル番号生成ツールの強化(WinAVR付属のgawkでも正常に動作)
	        * 説明書の見直し

■2008-10-23    * シリアル番号生成ツールの強化(数字、英大文字の利用も可能)
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

■2008-11-06    * シリアル番号生成ツールをbootmon88対応にした。
	        * hidasp.c をbootmon88との共用にした。
	        * hidmon, bootmon88に合わせるため、hidspx-src ディレクトリを src に変更。
	        * 簡易セットアップツール（setup.bat）を追加。

■2008-11-12    * USB-IOモードで利用した場合には、エラーで中断するのではなく、警告に留めた。
                  >hidspx -rf
                   Warnning: Please check HIDaspx mode.
                * hidasp.c を最新に更新
	        * vendor, product の文字列の照合を無効にし、VID, PID とシリアル番号のみを
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

※ 長大になったので、2009年以降の変更点は、history-j.txtに記載します。
