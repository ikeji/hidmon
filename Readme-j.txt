# Windows�̕W���I�Ȋ��ɍ��킹�ATAB size = 8�ŕҏW���Ă��܂��B

                                                2008�N 9��22���i���J�J�n�j
                                                          �b
                                                2009�N 3��30���i�ŐV�X�V�j

                                      �R�`�����Y�ƋZ�p�Z����w�Z  ��H�L�K
                                 E-mail senshu(at)astro.yamatata-cit.ac.jp


        USB�ڑ������̃h���C�o�C���X�g�[���s�v��AVR���C�^�iHIDaspx�j
        �P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P

�y1�z �͂��߂�

HIDaspx �́A�h���C�o�̃C���X�g�[�����s�v�� USB �ڑ������� AVR ���C�^�ł��B
�ʓr�񋟂���� hidmon �𗘗p����΁A�ėp�� USB-IO �Ƃ��Ă����p�ł��܂��B

�r�l������̍쐬���ꂽ HIDasp �����ɁAiruka ����� senshu �������ŉ��ǂ��s
�Ȃ��܂����B���̐��ʂ������܂����̂ŁA2008�N 9�� 22�������J���J�n����
���BHIDaspx �́A�r�l������̌��J���Ă�����̂Ƃ͓��e���قȂ�Afirmware, 
hidspx �R�}���h���Ɍ݊����͂���܂���̂ŋ�ʂ��Ĉ����Ă��������B

�� HIDaspx�́A�G�C�`�E�A�C�E�f�B�[ �A�X�y�b�N�X�Ƃ��ǂ݂��������B

  ********************** �X�V���̒��ӓ_ **********************
  hidspx �̍X�V���A�t�@�[���E�F�A�̓��t���X�V����܂����A�X�V�����Ƀt�@�[��
  �E�F�A�Ɋւ���L�ڂ������ꍇ�ɂ̓t�@�[���̍X�V�͕s�v�ł��Bbin �t�H���_��
  setup.bat �ňꊇ�X�V���ł��܂��B[�X�V�f�B���N�g����] ���ȗ������ꍇ�ɂ́A
  C:\bin�ɃR�s�[����d�l�ł��B

  XXXX\hidspx-2009-0111\bin> setup [�X�V�f�B���N�g����] <Enter>
  ================= hidspx ���Z�b�g�A�b�v���܂� ====================
 C:\bin�t�H���_ �ɃR�s�[����]����ꍇ�́A[Y]��啶���œ��͂̂��ƁB
 ���ɃZ�b�g�A�b�v�����܂���[Y/n]

 �\������郁�b�Z�[�W�ɏ]���āA�����p���������B


�y2�z �A�[�J�C�u�̍\��

.\
���� �e��h�L�������g
����bin
��	libusb0.dll
��	hidspx-gcc.exe	�c MinGW-GCC�ŃR���p�C����������
��	hidspx.exe	�c Borland C++ ver 5.5.1�ŃR���p�C����������
��	hidspx.ini	�c hidspx�̏������t�@�C��
��	fuse.txt	�c FUSE�����ڍו\������ׂ̃e�L�X�g�t�@�C���ihidspx���Q�Ɓj
��	fuse_j.txt	�c FUSE�����ڍו\������ׂ̃e�L�X�g�t�@�C���i���{��\���j
��	fuse_en.txt	�c FUSE�����ڍו\������ׂ̃e�L�X�g�t�@�C���i�p���\���j
��	fuse_en_orig.txt�c FUSE�����ڍו\������ׂ̃e�L�X�g�t�@�C���i�p����p�j
��	hidmon.exe	�c HIDaspx�̃e�X�g���ɗ��p
��
����bin
��  ����firmware	�c main-12.hex�iHIDaspx�p�j, main_libusb.hex�iMacOS, Linux�p�j
��			�c main-small.hex(USB-IO��p�̃t�@�[���E�F�A)
��			�c main-RC.hex (RC���U���[�h��USB-IO��p�̃t�@�[���E�F�A�A�������x��)
�b�@�@�@�@�@�@�@�@�@�@�@�c genserial.awk�i�V���A����񐶐��c�[���j
�b�@�@�@�@�@�@�@�@�@�@�@�c firmprog.bat main12.hex���������ނ��߂�BAT�t�@�C��
�b�@�@�@�@�@�@�@�@�@�@�@�c prog-rc.bat  main-RC.hex���������ނ��߂�BAT�t�@�C��
�b
����firmware		�c HIDasp�̃t�@�[���E�F�A�\�[�X
����src			�c hidspx�̃\�[�X(MinGW, Borland C++���p)
��  ����libusb
��      ����bin
��      ����examples
��      ����include
��      ����lib
��          ����bcc
��          ����dynamic
��          ����gcc
��          ����msvc
��          ����msvc_x64
����circuit		�c ��H�}


�y3�z hidspx�R�}���h�̃C���X�g�[�����@

�@hidspx�̃C���X�g�[���ɂ́A���ʂȃC���X�g�[���͕s�v�ł��B
bin �f�B���N�g���ɂ���t�@�C�������s�\�ȃf�B���N�g���ɃR�s�[���邱�Ƃŗ�
�p�\�ɂȂ�܂��B�i�t�@�C�����t�́A���ǂɂ��قȂ邱�Ƃ�����܂��j

**************************** �d�v ****************************
2008/11/13  13:30             1,787 setup.bat
2007/03/20  11:33            43,520 libusb0.dll
2009/01/10  15:15           121,344 hidspx.exe
2009/01/10  15:16            54,784 hidspx-gcc.exe
2008/09/09  12:54            17,408 usbtool.exe
2008/09/05  10:39               289 hidspx.ini
2008/12/10  08:39            20,115 fuse.txt
2008/12/07  13:08            22,152 fuse_en.txt
2008/12/10  08:41            20,115 fuse_j.txt
**************************** �d�v ****************************

�����̃t�@�C���́A���݂Ɋ֌W���Ă��܂��̂ŁAhidspx.exe �����łȂ��ꊇ����
�R�s�[���Ă��������B

�R�s�[�R���h���ׁAsetup.bat�t�@�C����p�ӂ��܂����B

 setup.bat �����s����� c:\hidmon �ɕK�v�ȃt�@�C�����R�s�[���܂��B
 setup c:\bin �̂悤�Ɏw�肷��΁Ac:\bin �ɕK�v�ȃt�@�C�����R�s�[���܂��B

���ϐ�������ł�����́Ahidspx\bin �Ƃ����f�B���N�g���� PATH ���ϐ��ɓo
�^����ƁA�Ȍ��ɃR�}���h���w��ł��֗����p�ł��܂��B

�Ȃ��A2 �̎��s�t�@�C���ihidspx-gcc.exe, hidspx.exe�j�̋@�\�͓����ł��B
1 �����ȏ�̃e�X�g���Ԓ��ɖ�肪�����������Ƃ���ABorland C++ �ŃR���p�C��
���� hidspx.exe ��W�����s�t�@�C���Ƃ��Č��J���܂��B����Ɉُ킪���鎞�ɂ́A
�]���̕W���ɂ��Ă��� hidspx-gcc.exe ���g���Ă݂Ă��������B

===========================�u�⑫�v===========================
���݂̂Ƃ���Ahidspx �̃R�}���h�����g���Ă��܂����A����� avrspx �̕ʖ��ł��B
�i���O�͔C�ӂɕύX�ł��܂����A�������Ȃ����O�����Ă��������B�ݒ�t�@�C��
�́A�R�}���h��.ini �ɂȂ�܂��B�j

avrspx �ł͂Ȃ��Ahidspx �ɂ��Ă���̂� ������ avrspx �̗��p�҂ɍ�����^����
���AHIDaspx �Ή��𖾎����邽�߂ł��Bhidspx �� avrspx �S�Ă̋@�\�𗘗p�ł��A
�����̉��ǂ��s���Ă��܂��B�������A�Ӑ}���Ȃ�����p�i�s��j��������\��
���F���Ƃ͂����܂���B���̂��߁A�\���ȓ��쌟�؂��I����܂ł́Aavrspx �̖���
�������Ɍ��J�܂��̂ŁA���������������B
==============================================================

�y4�z ����

circuit �t�H���_�����H�}�iHIDaspx.png�j���Q�l�Ƀn�[�h�E�F�A�𐻍삵�܂��B
�Ȃ��A�ߓ����ɐ�p�̊������\ (http://www.wsnak.com ���) �ɂȂ�܂��B
��H�}�ɂ����L���Ă��܂����A���U�q�ɂ̓N���X�^�����������Ă��������B�Z���~
�b�N�U���q�ł͉�H���v�����鐸�x�ƈ��萫�𖞂������Ƃ��ł��܂���B
���̌�ŁAbin/main.hex �� Tiny2313 �ɏ�������ł��������B�q���[�Y�ݒ�́C�O
�t���� 12MHz �N���X�^���ɍ��킹�A�ȉ��̂悤�ɐݒ肵�܂��B

Low: 11111111 (0xFF)
     ||||++++-- CKSEL[3:0] �V�X�e���N���b�N�I��
     ||++-- SUT[1:0] �N������
     |+-- CKOUT (0:PD2�ɃV�X�e���N���b�N���o��)
     +-- CKDIV8 �N���b�N���������l (1:1/1, 0:1/8)

High:11-11011 (0xDB)
     |||||||+-- RSTDISBL (RESET�s�� 1:�L��, 0:����(PA2))
     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
     |||+-- WDTON (WDT 0:�펞ON, 1:�ʏ�)
     ||+-- SPIEN (1:ISP�֎~, 0:ISP����) ��Parallel���̂�
     |+-- EESAVE (������EEPROM�� 1:����, 0:�ێ�)
     +-- DWEN (On-Chip�f�o�b�O 1:����, 0:�L��)

Ext: -------1 (0xFF)
            +-- SPMEN (SPM���� 1:����, 0:�L��)


�� ���܂ł́APD2����̃N���b�N�o�͂�L���ɂ��Ă��܂����B���̋@�\�𗘗p������
   �ꍇ�ɂ́ALow=0xBF �Ɛݒ肵�܂��B
   
   ���̔łł́APD2 �͊J���Œʏ�̃��C�^�AGND �Ɛڑ������ USB-IO �Ƃ��ċ@�\
   ���܂��B���C�^���[�h�ł� PB2 �iLED ���p) �[�q���� 1MHz �̋�`�g���o�͂�
   �܂��̂ŁAPD2 �i12MHz�j�����L�͈͂ɗ��p�ł��܂��B

�@hidspx �𗘗p�ł�����́A�H��o�׏�Ԃ� ATtiny2313 �ɑ΂��A�ȉ��̃R�}���h
�� FUSE �ݒ肪�\�ł��B�������A�O���ɔ��U���ڑ����Ă��Ȃ����ɂ��̐ݒ��
�s���ƁA���U���ڑ�����܂� ISP �����̃��C�^�ɂ��ǂݏ������o���Ȃ��Ȃ��
���̂ŁA�����ӂ��������B

> hidspx -d4 -fL0xff -fH0xdb -fX0xff <ENTER>


�y5�z �g����

�@�ȉ��ɊȒP�Ȏg�p����Љ�܂��B�ڍׂȎg�����́Aavrx-tool.txt���������������B

�� 1030�ňȍ~�́AFUSE Low��0xFF�ɐݒ肷�邱�Ƃ����E�߂��܂��B

�� USB-IO�Ƃ��ė��p����
USB-IO �Ƃ��ė��p����ꍇ�ɂ́APD2 �� GND �Ɍq���ł��������BUSB-IO �Ƃ��Ďg
���Ă��鑕�u�ɑ΂��Ahidspx �𗘗p����ƁA�ȉ��̂悤�ɃG���[�ɂȂ�܂��B
����́AUSB-IO �Ƃ��ė��p���Ă��� HIDaspx ���A����ăv���O���}�i���C�^�j��
���đ��삷��̂������ׂł��B

 >hidspx -rp
 Error: HIDaspx is USB-IO mode.
 HIDaspx(0000) not found.


�� AVR���C�^�iProgrammer�j�Ƃ��ė��p����
USB-IO �@�\�𗘗p���Ȃ��ꍇ�ɂ́APD2 ����̃n�[�h�E�F�A�ύX�͕s�v�ł��B
�Ȃ����C�^���[�h�ł́APB2 �� �^�[�Q�b�g�}�C�R���� �~�ϗp�N���b�N (1MHz) ��
���ċ@�\���邽�߁APB2(PWR LED) �� LED �͂�┖���_�����܂��B

hidspx �Łu-ph�v���w�肷��� HIDaspx �����p�ł��܂��BHIDaspx ��p�Ɏg���ꍇ
�ɂ́Ahidspx.ini �� -ph �������Ă����΁A���̎w��͏ȗ��ł��܂��B

> hidspx -ph -d4 test.hex

hidspx.ini �ł� -d4 ��ݒ肵�Ă��܂��B-d �Ńf�B���C���w��ł��A���l�͎��Ԃ�
��Ⴕ�Ēx���Ȃ�܂��B1MHz �� RC ���U���[�h�ł� -d4 �ȏ���w�肵�Ă��������B
���̒l�́A�����ɓ��삷�� AVR �}�C�R���ł͏����Ȓl���w��ł��A16MHz �ȏ�̎�
�g���œ��삷�� AVR �}�C�R���ɂ� -d0 ���w��ł��܂��B�w��l���������قǁAR/W
�̑��x�����サ�܂��B

-dN�̎w��̖ڈ��́A�ȉ��̂Ƃ���ł��B

No	FUSE Low	-d�̒l	���U���g��	���l
0	----	        -d0	16MHz�ȏ�	�O���N���X�^��/�Z���~�b�N���U�q
1	-fL11100100	-d1	8MHz		14CK+65ms
2	-fL11100010	-d2	4MHz		14CK+65ms
3	-fL01100100	-d4	1MHz		�H��o�גl
4	-fL01100010	-d5	500kHz		14CK+65ms
5	-fL11100110	-d17	128kHz		14CK+65ms
6	-fL01100110	-d120	16kHz		118, 119�ł͕s����


iruka ����ɂ��v�����ʂ������܂��B�i�ŐV�̔łł́A����������\�����サ
�Ă��邱�Ƃ�����܂��j

	�R�}���h	�]�����e		���ǑO	��������
-------------------------------------------------------------------------
hidspx -d0 -rp >XX.hex	8kB Read		8.39�b	2.22�b(3,690B/�b)
hidspx -d0 test.hex	8kB Write/Verify	10.94�b	4.86�b(1.64kB/�b)
hidspx -d1 -rp >XX.hex	8kB Read		8.40�b	3.12�b(2,625B/�b)
hidspx -d1 test.hex	8kB Write/Verify	10.96�b	7.06�b(1.13kB/�b)
hidspx -d4 -rp >XX.hex	8kB Read		8.39�b	4.63�b(1,770B/�b)
hidspx -d4 test.hex	8kB Write/Verify	13.26�b	9.98�b(821B/�b)
-------------------------------------------------------------------------

�y6�z HIDaspx�̍������iUSB-HUB���p�̑E�߁j

�@AT90S2313 �̂悤�ɁA�y�[�W�����݋@�\�̖��� AVR �}�C�R���ł͏����݂Ɏ��Ԃ�
������܂��BUSB 2.0 HUB ������ɐڑ����ď����݂��s�������ʂł��B

�� hidmon�ɂ��x���`�}�[�N����
AVR> bench
hid write start
hid write end,   37.109 kB/   11.08 s,   3.350 kB/s

�� Write&Verify����
>timeit hidspx -d2 2kb.hex
Elapsed Time:     0:00:25.703

���ɁA1,000 �~�ȓ��Ŕ����� USB 2.0 �ɏ������� HUB ����āA���l�̌v�����s
���Ă݂܂��B

�� hidmon�ɂ��x���`�}�[�N����
>hidmon
TARGET DEV_ID=55
HIDaspx is USB-IO mode.
AVR> bench
hid write start
hid write end,   37.109 kB/    2.47 s,  15.036 kB/s

�� Write&Verify����
>timeit hidspx -d2 2kb.hex
Elapsed Time:     0:00:09.546

HUB �����ɔ�ׂāA1/2 �ȉ��ɒZ�k����A10�b�����ŏ����݁��ƍ����������܂��B

���̂悤�ɋ��^�C�v�� AVR �}�C�R�������p�ł��܂����A�v���ȏ����݂���]�����
���ɂ́A�V�^�C�v�̃}�C�R�����̗p���Ă��������B�����������T�C�Y�� ATtiny2313
�ł� 1�b���x�Ŋ������܂��B

�����̌��ʂ���AUSB HUB �����ɂ悢�d�������Ă��邱�Ƃ��킩��܂��B���^
�C�v�� AVR �}�C�R���Ɍ��炸�A�قƂ�ǂ̏ꍇ�A�������x�͊i�i�Ɍ��サ�܂��B
hidmon �� 15kB/s �𒴂������p�ӂ���Α��x����ɂȂ���܂��BUSB-HUB ��
���肵�A�������ė��p���Ă݂Ă͂������ł��傤���B

===============================================================================

�y7�z ���p��̒��ӓ_

�@2009�N 1��������J�����łł́A�u-ri, -rd, --atmel-avr, --avr-devices�v��
�ǂ̃R�}���h���g�����ƂŁAWeb �u���E�U���g�����@�\��񋟂��Ă��܂����A�ڑ�
��̃T�[�r�X����~���Ă���ꍇ������A���̎��ɂ͔����̃y�[�W���\�����ꂽ��A
�G���[���\�������ꍇ�����邱�Ƃ����������������B

�y8�z ���C�Z���X�i�r�l������ɓ����j
AVR USB�ɏ����AGPL2�Ƃ��܂��B�i���肢�F���p���p���ɂ́A����񂭂������B�j

�y9�z �ӎ�
�@HIDaspx �̊J���ɂ́A�����̕����狦�͂����������܂����B�����ɁA�S��芴��
�̈ӂ�\���܂��B

    1. �r�l������  �c�c HIDasp�̊J���҂ł��BHIDasp�Ȃ����Ă�HIDaspx�͂���܂���B
    2. iruka����   �c�c �������i�t�@�[���E�F�A��hidspx�j��iruka����̐��ʂł��B
    3. kuga����    �c�c ��AVR�̃T�|�[�g�ƃR���p�N�g���APD5�ɂ��Đڑ��@�\�B
    4. RAIN����    �c�c �V�A�C�R���̍쐬�Ɗe��̓���e�X�g�B
    5. ��i����    �c�c ������HIDaspx����ʂ��ė��p�ł��邱�Ƃ��m�F���������܂����B
    6. kuman����   �c�c �������璍�ڂ��Ă��������A�����̏��������������܂����B
    7. TAD����A�{�O����Amitu����Aakakusa����A���񂳂�Akawana����
                   �c�c ����񍐂�e��̃R�����g�����������܂����B

  ----- * ----- * ----- * ----- * ----- * ----- * ----- * ----- * -----

�y�t�^�z �J���Ҍ����̃���

[0] 2008-10-17�`10-22�̍�Ɠ��e

�@������� HIDaspx ������ PC �ɍ����������p�ł���@�\����������ׁA�V���A
���ԍ��̐����c�[���� hidspx, hidmon �̋@�\�g�����s���܂����B���̋@�\������
���邽�߁A�ȉ��̍�Ƃ��s���Ă��܂��B

 1. �V���A����񐶐��c�[���̍쐬�igawk�ŋL�q�j
 2. hidspx�R�}���h��-ph[:XXXX]�ɃV���A�����w��@�\��ǉ�
 3.          �V   �� -ph? �Ń��X�g�ł���@�\��ǉ�
 4. hidmon �ɂ����l�̋@�\��ǉ�
 5.          �V   �� -p? �Ń��X�g�ł���@�\��ǉ�
 6. hidmon.dll�� �V���A�������w��ł���@�\��ǉ�
 7. �h�L�������g�̌�����
 8. ���ۂ̊��ł̓���m�F

[2] firmware, hidspx-src �̗��p�@

 1. CMD prompt ��� make ���g����build����

 2. �w�b�_�t�@�C���Ȃǂ�ύX�����ꍇ�ɂ́Amake clean ���make���邱�ƁB
    �~�X�������ׁArebuild.bat ��p�ӂ��Ă���Brebuild �����s����ƁA����
    ��K�����s�ł���B�Ȃ��A��Ƃ��ς񂾂�Acleanup �����s����ƁA��ƃt�@
    �C�����ꊇ���č폜�ł���B

 3. �G���[�����������ꂽ�t�@�[���E�F�A����s�t�@�C���́Abin�̉��ɃR�s�[�����

 4. �X�V�R���h���ׁA�Ō��develop�f�B���N�g����build-all.bat�����s����

[3] �V���A�����̐����c�[���̎g����

HIDaspx �p�̃V���A���f�[�^�iHEX �t�@�C���j�𐶐�����c�[����p�ӂ��܂����B

�@���̃c�[���� gawk(GNU awk �Ƃ����C���^�[�v���^�j ���g���ē��삵�܂��BAVR
�}�C�R���̗��p�҂ɍL�����y���Ă��� WinAVR �ɂ́Agawk.exe ���t�����Ă��܂����A
gawk �𗘗p�ł��Ȃ����́A�o�O�����Ȃ��P�Ƃœ��삷��A�ؑ�����̈ڐA�� gawk
����肵�Ă��������B

2008�N7��4�� gawk 3.1.6 ���{��Ńo�C�i���o�Ofix
GAWK�̉��ǔł̓���� �� http://www.kt.rim.or.jp/~kbk/

http://www.kt.rim.or.jp/%7ekbk/cgi-bin/download.cgi?dl-file1=gawk-mbcs-win32-20080704.zip

���肵�� gawk-mbcs-win32-20080704.zip ��W�J���Agawk.exe �� hidspx.exe �Ɠ�
���f�B���N�g���ɃR�s�[���܂��B

 2007/10/22  15:50              280,351 ChangeLog
 2007/07/16  23:59               35,147 COPYING
 2008/07/02  16:47              189,434 mbc-diff
 2007/09/25  14:58               83,133 NEWS
 2008/07/03  16:51              729,088 gawk.exe
 2006/12/01  14:26               53,248 igawk.exe
 2008/07/03  17:22                9,588 readme.ja

1028 �ňȍ~�A�u-v adr= �v���w�肷�邱�ƂŁA�C�ӂ̃A�h���X�ɑ΂���V���A����
��𐶐��ł��A�܂��A�����̃V���A�������u���ɐ����\�ł��B

usage: gawk -f genserial.awk -v t={aspx|mon88} start-number  [end-number]
�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P

�g�p��1�F
 > gawk -f genserial.awk 0001

�̂悤�Ɏg���܂��B��������s����ƁA0001 �̃V���A���f�[�^�� HEX �t�@�C��
�i_0001.hex�j�𐶐��ł��܂��B

�g�p��2�F0001�`0100�̃V���A�����̃t�@�C�����ꊇ���Đ������܂��B
         �w��ł���l��1�`9999�܂ŁA����𒴂���ƃG���[�ɂȂ�܂��B

 > gawk -f genserial.awk 1 100


�g�p��3�F16�i����p�啶���̎w����\�ł��B�������͑啶���ɕϊ����܂��B

 > gawk -f genserial.awk ABCD

���̃t�@�C���� main.hex �̃t�@�[���ƂƂ��Ɉȉ��̂悤�ɏ������߂΁AHIDaspx 
�͂��̃V���A���ԍ������������̂ɂȂ�܂��B

 > hidspx main.hex _0001.hex

�� �擪������'_'��ǉ����܂����̂ŁA�ȉ��̂悤�ȑ��삪�\�ł��B
 > del _*.hex
 �ňꊇ���ăV���A�����p�̃t�@�C�����폜�ł��܂��B

�g�p��4�F
 > gawk -f genserial.awk -v t=mon88 0001

bootmon88�p�̃V���A�����𐶐����܂��B

�g�p��5�F
 > gawk -f genserial.awk -v t=1234 0001

1234(10�i��)�Ԓn����n�܂�4���̃V���A�����𐶐����܂��B


[3.1] hidspx�ł̃V���A�����w����@

������̈قȂ�V���A������������ HIDaspx ���g���ē���̊m�F���s���܂����B
���̋@�\�ɂ��A������ HIDaspx ����ʂ��ė��p�ł��܂��B�i��䂾���ŗ��p����
�ꍇ�ɂ́A�V���A�����ݒ�͕s�v�ł��B�j

> hidspx -ph:0001  �ihidspx -ph1 �ł������Ӗ��ɂȂ�܂��j

���̂悤�Ɏw�肷��ƕ����� HIDaspx ����ʂ��Ďg�����Ƃ��ł��܂��B�Ȃ��A-ph
�Ǝw��ŏ����l�� 0000 ��I�����܂��B�V���A������ݒ肵�Ȃ��ꍇ�ɂ́A�]��
�ʂ�̗��p���\�ł��B

>hidspx -ph -r
HIDaspx(0000) not found. ---- -ph �ł́u0000�v��T���܂�

>hidspx -ph? ---- �ڑ�����Ă��� HIDaspx��\�������܂�
Manufacturer: [YCIT], Product: [HIDaspx], serial number: [0123]

>hidspx -ph:1234 -r ---- �ԍ����w�肵��HIDaspx���g���܂�
HIDaspx(1234) not found. ---- 1234��HIDaspx�͑��݂��܂���

>hidspx -ph:0123 -r ---- �������w�肷��΁A���삵�܂�
Detected device is ATtiny2313.
Device Signature  = 1E-91-0A
Flash Memory Size = 2048 bytes
Flash Memory Page = 32 bytes x 64 pages
EEPROM Size       = 128 bytes

[4] �@�\�̕ύX�_

�@�����̊J���҂ɂ��J���͔��U�������ł����AHIDaspx �Ɋւ��ẮA��ՓI�H��
��肭�@�\���Ă��܂��B

       4454 Oct 16 20:19 Makefile
       4540 Oct 16 20:22 Makefile.unix
         52 Oct 16 20:31 cleanup.bat
        144 Oct 17 08:41 rebuild.bat
       1137 Oct 18 18:03 hidconfig.h
      15153 Oct 18 18:04 usbconfig.h

    ���܂� main.c �ɏ����Ă����u�@�\�I��p�̒�`�v���uhidconfig.h�v�ɕ������A
    ���̃\�[�X������Q�Ɖ\�ɂ��܂����B����ɂ���āAhidconfig.h �݂̂��C
    �����邾���ŁA�œK�ȃR�[�h�̍쐬���\�ɂȂ�܂��B

    ���̒ǉ�����bat�t�@�C���̊ȒP�Ȏg������������Ă����܂��B

    0. �ݒ�t���O�́Ahidconfig.h, usbconfig.h �ɐݒ肷��
       �imain.c �ɂ͐ݒ�t���O�͂���܂���j
    1. �ʏ�́Amake �����s����B
    2. �G���[�������Ȃ�A�������@�\���邱�Ƃ��킩��΁Arebuild�����s����ƁA
    clean��ɍăR���p�C�����A2��ނ�HEX�t�@�C���� ../bin/firmware�ɃR�s�[�����
    3. ��Ƃ��I���΁Acleanup�ɂč�ƃt�@�C�����폜����


    2 �� 3 �͈�ɂł��܂����A�t�@�C�����폜����R�}���h�́A��������̂���
    ��ƍl���Ă��܂��B���̕ύX�ŕs�s��������ꍇ�ɂ́A���w�E���������B

    �܂��Airuka ����������Ă��܂����A���݁A�S�Ă̋@�\��L���ɂ��Ă� 2044 
    �o�C�g�ł���A2048 �ȓ��Ɏ��܂��Ă��܂��B

    * senshu 2008-10-18 (�y) 20:23:54 New
      hidspx �� firmware �̐��������Ċm�F���Ă݂܂������ALED �̐���R�}���h
      �̗L���̔���� LED ����̃t�@�[���E�F�A�̕����Ɏ኱�̋^��_������܂��B

      �T�C�Y������Ȃ������̂ŁA�R�����g�A�E�g��������������̂ł����A����
      �͐������ALED ����� MONITOR �R�}���h�ɓ��ꂵ�AISP_CMD �͕K���g�����
      ���ɂ���̂��悢�ƍl���Ă��܂��B

      �F�X�����������ʁA�ȉ��̎d�l�ɂ������ƍl���Ă��܂��B�i1017�ł����l�j

      (1) ISP_CMD �͕K���g�ݍ���
      (2) ISP_CMD�ł�LED�̐���͖����i�t�@�[���ɔC����j
      �@�@�� ���C�^����v���O����(hidspx)�Ŏw�肵�Ă���������
      (3) 2��LED��PC���琧�䂷��ꍇ�AISP_CMD�ł͂Ȃ�MONITOR�@�\�𗘗p����

      ���̎d�l�ɂ���΁A�t�@�[�����R���p�N�g�Ɉێ��ł��A������������܂��B
      ���� 9���Ɍ��J�����t�@�[�����g���Ă���ꍇ�ł����A����͐V��������
      �ɍX�V�����肢���邱�ƂɂȂ�܂��B


[5] �Q�lURL

    * HIDaspx�̌���URL�i�R�`�����Y�ƋZ�p�Z����w�Z���j
         http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2FHIDaspx

    * �r�l������(HIDasp�̊J����) �� http://www.binzume.net/library/avr_hidasp.html

    * iruka����̃T�C�g
	 http://hp.vector.co.jp/authors/VA000177/html/A3C8A3C9A3C4A3E1A3F3A3F0.html

    * kuman����̉�H�}���܂ށA���H���|�[�g(��ǂ������߂��܂�)
	http://www.geocities.jp/kuman2600/n6programmer.html#13 (10/12�ǋL����)

    * Objective Development Software GmbH �c AVR-USB�iAVR�}�C�R���p��USB�h���C�o�̃T�C�g�j
	http://www.obdev.at/products/avrusb/index.html

    * Atmel�Ђ�Web�T�C�g�iAVR�}�C�R���̐������j
        http://www.atmel.com/products/AVR/  �c �f�[�^�V�[�g��������͂����炩�����\

    * AVR�}�C�R���̃��[�U�[�R�~���j�e�B
        http://www.avrfreaks.net/  �c AVRstudio �� avr-gcc�̊e����͂����炩�����\


[6] �ύX����
��2008-09-22   senshu��iruka���񂪉��������{	�������Ή����{

��2008-09-24   senshu  firmware -d0�I�v�V�����̍œK��
               * hidpsx�̃G���[���o���A�̃��b�Z�[�W�o�͂̏C���i���s���ĕ\���j

��2008-09-28   * hidspx�̃R���p�C���I�v�V������-Wall��ǉ����A�x�����b�Z�[�W��
                 �΂���C�������{(senshu)
               * firmware -d0�I�v�V������USBasp�݊��������߂�(iruka)
               * MOSI, SCK �̋���������iHi-Z������j

��2008-10-03    * hidspx�̃��b�Z�[�W��HIDaspx�ɓ���
	        * Borland C++ �ł̌x�����b�Z�[�W��}�~(avrspx.h���C��)
	        * usbtool ��ǉ�
	        * kuga����̃A�h�o�C�X�ɂ��AFirmware�T�C�Y��40�o�C�g�k��(1968�o�C�g)
		 avr-size --mcu=attiny2313 main.elf
		   text    data     bss     dec     hex filename
		   1966       4      85    2055     807 main.elf
	        * USB��ProductID 0x5dc (libusb device) �Ƃ̋����������ׁA
		  0x5df(HID devide)�ɕύX
	        * firmware�̕ύX �i���̂Ƃ���T�|�[�g���Ă���AVR�f�o�C�X�ł́j
		  page_addr �� 256 �ȉ��Ȃ̂ŁAuint16 ���� uint8 �ɍ~�i�B

��2008-10-06    * iruka�����10-05�܂ł̏C�����𔽉f�i�قړ������e�ł��j
	        * -d2�ȏ�̒l���w�肵�����̕s��́Asenshu�̏C���~�X�ł����B
		 �iiruka����A��ς��萔�����|�����܂����j
	        * ATtiny2313�̔F�����s�\�����������Ɋւ��ẮA���̃~�X�ɋN������
		 �V�[�P���X����������H�A���K�؂Ȃ��̂ɂȂ��Ă��܂�
			        * firmware�̕ύX  page_addr �� uint16 �ɖ߂��B

��2008-10-10    * iruka�����10-09�܂ł̏C�����𔽉f�i�قړ������e�ł��j
	        * 2008.10.9a delay_10us�֐����R���p�C�����ɂ���Ă͍œK�������
		  �����Ă����̂��C��. �i�A�Z���u،���Ń����C�g�j
	        * 2008.10.9a '-d2'�ȏ�̒x���N���b�N����K����
	        * 2008.10.9 ������: flow-control�𓱓�
	        * 2008.10.9 12MHz�^�[�Q�b�g�� '-d0'���g����悤�ɕύX(SCLK=2MHz)
	        * 2008.10.9 ispConnect/ispDisconnect�𓱓�

��2008-10-12    * kuga�����AT90S�V���[�Y�p��ISP�ڍs������ǉ��i90S�V���[�Y���T�|�[�g�j
	        * ATtiny2313 ��FUSE�r�b�gSTU�r�b�g�̒l�ɂ���āA�F���Ɏ��s����̂��C��
		  �ihidspx����hwctrl.c spi_reset�֐���delay_ms(10)��ǉ����܂����j
	        * FUSE verify�G���[���̕\����K����
	        * FUSE ���������֐��̓K�����i�d�v�I�j

��2008-10-14    * firmware main.c�uUSICR=0;	/* SCK���|�[�g�ɖ߂��Ă��� */�v��L����
		-- USI�̏�������main�֐�����Ausi_trans�֐��Ɉړ�(SCK�������̌݊�������)
	        * usbHidReportDescriptor�̑傫����9�o�C�g�k��(���� 2038�o�C�g)
	        * hidmon�Ƃ̐��������l�����A���ɖ߂��܂���(���� 2048�o�C�g)
	        * �\�[�X���̃R�����g���C���i�s�����������C���j

��2008-10-17    * firmware (HID�łɉ����Alibusb�ł�ǉ��j
	        * rebuild.bat , cleanup.bat��ǉ�
	        * avrx-tool.txt�i�ڂ����g�����̉���j��ǉ�

��2008-10-20    * firmware (HID�łɉ����Alibusb�ł�ǉ��j
	        * -ph:XXXX �̃V���A�������T�|�[�g
	        * firmware�\�[�X�̃N���[���A�b�v�����{
	        * bin �t�H���_�ɃV���A����񐶐��c�[��(genserial.awk)��ǉ�
	        * �V�A�C�R���ɕύX(RAIN����쐬)

��2008-10-21    * �V���A����񐶐��c�[���̋���
	        * ��H�}�t�@�C�����X�V

��2008-10-22    * �V���A����񐶐��c�[���̋���(WinAVR�t����gawk�ł�����ɓ���)
	        * �������̌�����

��2008-10-23    * �V���A����񐶐��c�[���̋���(�����A�p�啶���̗��p���\)
	        * AVR-USB�̃h���C�o��2008-10-22�łɍX�V

��2008-10-28    * ���萫����̂��߁AHIDaspx�t�@�[���̋N������300m�b�̎��ԑ҂���}������
	        * �������̉��M�i���������b�N�����j
	        * USB-IO���[�h�̏�����ǉ�����(PORTB����̓��[�h�ɂ���)
	        * hidmon�ɂĒ񋟂��Ă����t�@�[���E�G�A��small�ł����̔łɊ܂߂�
	        * �\���Ȉ��萫���m�F�ł����̂ŁABorland C++ �ŃR���p�C���������̂� 
		  hidspx.exe�Ƃ��AMinGW-gcc �ŃR���p�C���������̂� hidspx-gcc.exe �Ƃ����B
	        * �d�����ďo�͂���郁�b�Z�[�W��1�x�̂݁A�o�͂���悤�ɂ����B

��2008-10-30    * USB-IO �� Programmer���[�h����ʂ��Ĉ����悤�ɕύX���܂����B
	        * firmware �� hidspx.exe �̗������X�V����K�v������܂��B
	        * ��H�}���C�����܂���(PD2��PB2�̈�����ύX�����ׁj
	        * HIDaspx�̃y�[�W��PDF�����A�t�������ɒǉ����܂����B

��2008-10-31 ...  * �t���̐������ɕs�����������C�����܂����B
	        * firmware ���� uint8_t => uchar�ɓ���iAVR-USB�ɍ��킹�܂����j

��2008-11-05    * develop�f�B���N�g���ɁAbuil-all.bat��p�ӂ����B
	        * ���̃R�}���h�����s����ƁA���J�ɕK�v�ȑS�Ẵt�@�C���𐶐����܂��B
		  �i�}�E�X�̃_�u���N���b�N�ł��@�\���܂��j
	        * -ph�w��ŁA0001��1�Ɠ��͉\�ɂ��܂����B���Ȍ��ȗ��p���\�ł��B
	        * PORTD (PD6, PD1, PD0)����́i�v���A�b�v����j�Ƃ��ď�����
		  �iakakusa���񂩂�̗v�]�ɂ��ύX�j

��2008-11-06    * �V���A����񐶐��c�[����bootmon88�Ή��ɂ����B
	        * hidasp.c ��bootmon88�Ƃ̋��p�ɂ����B
	        * hidmon, bootmon88�ɍ��킹�邽�߁Ahidspx-src �f�B���N�g���� src �ɕύX�B
	        * �ȈՃZ�b�g�A�b�v�c�[���isetup.bat�j��ǉ��B

��2008-11-12    * USB-IO���[�h�ŗ��p�����ꍇ�ɂ́A�G���[�Œ��f����̂ł͂Ȃ��A�x���ɗ��߂��B
                  >hidspx -rf
                   Warnning: Please check HIDaspx mode.
                * hidasp.c ���ŐV�ɍX�V
	        * vendor, product �̕�����̏ƍ��𖳌��ɂ��AVID, PID �ƃV���A�����݂̂�
		  �ƍ����܂��i������ƍ����s���܂���j�B

		  [���R]
		  �ȉ��̈قȂ�vendor����v���_�N�g���̑��u�𗘗p�\�ɂ��邽�߁B

                  VID=0x16c0 PID=0x05df vendor="YCIT" product="HIDaspx" serial="0000"
                  VID=0x16c0 PID=0x05df vendor="AVRetc" product="bmon" serial="0001"
                  
                  Makefile�Ƀ\�[�X�t�@�C���Ԃ̈ˑ�����ǉ�

��2008-11-27 ...
        * kuga����̃A�h�o�C�X�ɂ��A�����T�|�[�g���Ȃ�(NAK�����Ԃ��Ȃ�) 
	  interrupt_in ENDPOINT �����@�\��ǉ����܂����B

	usbconfig.h �ɒǉ�

	#define USB_CFG_HAVE_INTRIN_ENDPOINT    1
	#define USB_CFG_SUPPRESS_INTR_CODE      1
	#define USB_CFG_INTR_POLL_INTERVAL      200 //���ʂȂ̂Œ���interval

	Mac OS X�� BSD OS�ł̓���m�F���\�ȕ��́A���������������B
	�t�@�[���E�F�A�́A�ȉ��̂悤�ɁA�قڌ��E�ɋ߂��T�C�Y�ɂȂ��Ă��܂��B
	�i���������łƂ������Ƃ���ł��j

	   text    data     bss     dec     hex filename
	      0    1620       0    1620     654 main-small.hex
	      0    2042       0    2042     7fa main.hex
	      0    1958       0    1958     7a6 main_libusb.hex


��2008-12-03 usbdrv�h���C�o���X�V

        * AVRUSB-20081126�ł��o�܂����̂ŁAusbdrv�h���C�o���X�V���܂����B
	  ���e�͂قƂ�Ǖς���Ă��܂���B�������A�ȉ��̐ݒ�Ɋւ��R�[�h��
	  �C��������܂����B

	#define USB_CFG_HAVE_INTRIN_ENDPOINT    1
	#define USB_CFG_SUPPRESS_INTR_CODE      1
	
	  ���̂��߁A�ȉ��̂悤�ɐ��������R�[�h�T�C�Y���኱�������Ȃ��Ă��܂��B

	   text    data     bss     dec     hex filename
	      0    2038       0    2038     7f6 main-12.hex
	      0    1638       0    1638     666 main-small-12.hex
	      0    2002       0    2002     7d2 main-small-RC.hex
	      0    1954       0    1954     7a2 main_libusb.hex

        * hidspx�̏������ݑ҂����Ԃ̃}�[�W�����g��

        * hidspx��verify�G���[���̏��������ǁB

	  ���܂ł́A1 �o�C�g�̈Ⴂ�����o�����i�K�ŏ����𒆒f���Ă��܂������A
	  ���̔ł���A�G���[�̔��������A�ǂݍ��񂾃f�[�^�S�Ă̏ƍ����s����
	  ���Ɏd�l��ύX���܂����B���̌��ʁA�ȉ��̂悤�ɗL�v�ȏ��𓾂邱
	  �Ƃ��ł��܂��B
	  
	  ���̗�́A12MHz �œ��삷�� ATtiny2313 �ɑ΂� -d0 �Ƃ����K�i�O�̏�
	  ���ŏ������݂��s�������߁A�����̃G���[���������Ă��܂����A2038 �o
	  �C�g�� 20 �o�C�g���x�̃G���[���ł��邱�Ƃ��킩��܂��B
	  
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

	���̏ƍ��@�\�i-v�j�́A�u�[�g���[�_�ŏ������񂾃t�@�[���E�F�A�̃f�[
	�^�ƍ��p�ɂ����p���邱�Ƃ��ł��܂��B�Ȃ��A-d1 �̂悤�ɁA�K�؂Ȏw��
	���s���΁A�ȉ��̂悤�ɐ���ɏ������݂��s���܂��B

	>hidspx -d1 main-12.hex
	Detected device is ATtiny2313.
	Erase Flash memory.
	Write   Flash: 2038/2038 B
	Verify  Flash: 2038/2038 B
	Passed.

��2008-12-10 �T�|�[�gAVR��ǉ�
        * mega325P/3250P/324PA, PWM216,316�Ή���ǉ�


��2009-01-10 -rF, -ri�I�v�V������ǉ�

        * -rF (Read Fuse list) �I�v�V����
        ���̃I�v�V�������w�肷��ƁA�q���[�Y�ݒ�`���ŏo�͂��܂��B
        avrdude�Ȃǂŗ��p����ꍇ�ɂ��A�g���₷���`���ł��B
        CMD���̃R�s�[�@�\���g���āA���H���Ă��������B

        > hidspx -rF
	Detected device is ATtiny2313.
        DEVICE=ATtiny2313 -fL0xE4 -fH0xDF -fX0xFF

        �������ς݂� AVR �̃q���[�Y���ݒ莞�ɗ��p�ł���`���ŏo�͂��܂��B
        -rf �͏]���Ɠ��l�̋@�\�ł��̂ŁA���e���m�F����ꍇ�ɂ́A-rf �I�v�V
        �������g���Ă��������B�C���^�[�l�b�g�Ɍq�����Ă���ꍇ�ɂ́A-ri �I
        �v�V�������֗��ł��B

        * -ri (Read Fuse Infomation) �I�v�V����
        ���̃I�v�V�������w�肷��ƁAWeb �u���E�U�ŁA�����I��
        http://www.engbedded.com/cgi-bin/fc.cgi/ �y�[�W���J���A�Y������ AVR
        �}�C�R���̃q���[�Y����\�����܂��B

        > hidspx -ri (�܂��� -rI)

        ���݂̂Ƃ���񋟂���Ă��Ȃ� ATmega644P �́A�b��I�� ATmega644 ����
        �яo���悤�ɂ��܂����BWeb �u���E�U�́AMS-IE, firefox,Sleipnir �� 3
        ��ނɂ��āA���ꂼ��W���u���E�U�ɐݒ肵�����ɁA���퓮�삷�邱��
        ���m�F���܂����B�Ȃ��A-ri �ŏ����m�F���邽�тɐV���ȃy�[�W���J��
        �܂��̂ŁA�m�F��ɂ��̃y�[�W����A�J���߂��ɒ��ӂ��Ă��������B

        ��L��Web�T�C�g��fuse�����T�|�[�g���Ă���AVR�}�C�R���̈ꗗ
        (2009�N1��10������)
        
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

	�����ɂ킽��L�v�ȏ��T�C�g���^�c���Ă��� www.engbedded.com ����ƁA
	���̃A�C�f�B�A��񋟂��Ă��ꂽ audin ����Ɋ��ӂ������܂��B
	�ǂ������肪�Ƃ��������܂����B

��2009-01-11 hidmon�̒ǉ��ƃw���v���b�Z�[�W�̏C��

        * hidspx , hidspx -? �ŕ\�����郁�b�Z�[�W�ɁA0110�̋@�\�ǉ����𔽉f

        * ���̐������ɋL�ڂ��� hidmon �� bench �e�X�g�����s�ł���悤�ɁA
          hidmon.exe �R�}���h�𓯍�

        * -rF�I�v�V�����̎d�l���ȉ��̂悤�Ɋg��
        �R�s�[���y�[�X�g�Œ��ڗ��p�\�Ȍ`�����o�͂���Ɏd�l�Ƃ����B
        �ȉ��̂悤�ɁAhidspx��avrdude�p�̃R�}���h�s��\������B
        �i�C�O�ł�avrdude�̎g�p���嗬�ł���A�Q�l�̂��߂ɂ��̋@�\��ǉ������j

        �� FUSE �ݒ莞�ɂ́A�G���[������邽�߁A-d10 �̂悤�ɑ��x�𗎂Ƃ���
           �������ނ��Ƃ������߂��܂��B

        >hidspx -rF
        Detected device is ATtiny2313.
        
        DEVICE=attiny2313
        ### hidspx command line example ###
        hidspx -d10 -fL0xE4 -fH0xDF -fX0xFF
        
        ### avrdude command line example ###
         avrdude -c avrdoper -P stk500v2  -p tiny2313 -U flash:w:main.hex:i \
         -U lfuse:w:0xe4:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

        * -rd Docment view�@�\��ǉ�
        ���̃I�v�V�������w�肷�邱�Ƃɂ��A�Y������ AVR �}�C�R���̊T�v�� 
        Web �u���E�U�ɕ\�����܂��B���̋@�\�́Ahttp://www.avrfreaks.net/ ��
        �񂪒񋟂�����𗘗p���Ă��܂��B

��2009-01-12 Atmel�Ђ�AVR Freaks ��URL���J���@�\��ǉ�

        * �u--atmel-avr�v�u--avr-devices�v�I�v�V�����̒ǉ�

        > hidspx --atmel-avr    ... Atmel�Ђ�AVR�}�C�R���̃y�[�W���J��
        > hidspx --avr-devices  ... AVR Freaks��Devices�y�[�W���J��

          �����悭���p���Ă���uAtmel �Ђ� AVR �̃y�[�W�v�� AVR Freaks �� 
          AVR �f�o�C�X�ꗗ�̃y�[�W���J���@�\��ǉ����܂����BWeb �u���E�U��
          �u�b�N�}�[�N�ɓo�^���Ă����΍ςނ��Ƃł����AAVR �}�C�R�����J����
          ��S�Ă� PC �Ƀu�b�N�}�[�N����͎̂�Ԃ�������i������ PC ��
          ���p����ꍇ�j�܂��̂ŁA�����b�g�͂���Ǝv���܂��B


��2009-01-13 -rd�I�v�V�����̕s����C��
	* �ꕔ�� AVR �}�C�R���iAT90S2313 �Ȃǁj�� -rd �I�v�V�����𗘗p�o��
	  �Ȃ������_�����ǁi�ȉ��́A���ǑO�̃��b�Z�[�W�j

	>hidspx -rd
	Detected device is AT90S8515.
	Fuse bits are not accessible.
	
	>hidspx -rd
	Detected device is AT90S2313.
	Fuse bits are not accessible.

��2009-01-15 -rF�I�v�V�����̉��ǁA���[�U�[�u�b�N�}�[�N�@�\��ǉ�

	* avrdude -p�I�v�V������avrdude�Ŏw��\�ȃp�[�c���ŏo�͂���悤�ɏC���B

	* hidspx.ini�Ƀu�b�N�}�[�N�@�\��ǉ�
	  �uhidspx -- �o�^���� keyword�v�Ƃ���΁A�Ή��� URL ���J�����Ƃ���
	  ���A�ő� 100 ���܂œo�^�ł��܂��B�o�^���e�́A�ȉ��̃R�}���h�Ŋm�F
	  �ł��܂��B�ȉ��̂悤�ɁAHELP ���b�Z�[�W�̌���ɓo�^�����u�b�N�}�[
	  �N���\���ł��܂��B

	> hidspx -? | more (���邢��less) �ňȉ��̂悤�ɕ\���ł��܂��B

  === user bookmarks ===
  --avrlibc = [http://www11.ocn.ne.jp/~akibow/avr-libc-user-manual-1.4.3/]
  --avrwiki = [http://avrwiki.jpn.ph/wiki.cgi]

	  �Ȃ��A���̌`���� ini �t�@�C���Ƀu�b�N�}�[�N��o�^�����ꍇ�ł��A�]
	  ���� hidspx �̓R�����g�Ƃ��Ĉ����ׁA�o�[�W������߂����ꍇ�ł��s
	  ��͐����܂���B

	  setup.bat �Ńo�[�W�����A�b�v���鎞�A�㏑���Őݒ肵�����e��������
	  �Ƃ��Ȃ��悤�� hidspx-save.ini �ɕۑ�����d�g�݂�ǉ����܂����B
	  �X�V���A�ݒ���e�����m�F���������B

	* hidspx.ini�ւ̓o�^���@
	  �ȉ��̂悤�Ɂu;# �v�ɑ����āAkeyword URL �̃y�A���󔒂ŋ�؂��ēo
	  �^���܂��B

============= hidspx.ini ==================
; AVR Book Marks
;
;# avrlibc   http://www11.ocn.ne.jp/~akibow/avr-libc-user-manual-1.4.3/
;# avrwiki   http://avrwiki.jpn.ph/wiki.cgi
;# hidaspx   http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2FHIDaspx00

	  keyword �́A�\��ς݂̃I�v�V�����Ƌ�ʂł��� 8 �������x�܂ł̉p���E
	  ��������\������镶����Ƃ��A���͗e�ՂȂ��̂����E�߂��܂��B
	  �o�^�͗��p�҂̎g�����ꂽ�G�f�B�^�i�����������p�\�j�𗘗p���Ă�
	  �������B�Ȃ��A�啶���E�������� ��ʂ��܂��B

	  �\���͓o�^�ł��܂��񂪁A�ȉ��̂��̈ȊO�����p�ł��܂��B-- �ȍ~��
	  �I�v�V�����ɂ� help �������u-�v���܂߂Ă��܂��̂ŁA�p���Ɛ����̂�
	  �œo�^����Η\���Ƃ̏d��������邱�Ƃ��ł��܂��B

	  ���FURL �̕����� & ���܂ޏꍇ�A^& �̂悤�� ^ ��ǉ����A�󔒂��܂�
	  URL �� %20 �ɒu�������A����Ɏ��s�t�@�C���ł͋󔒂��܂܂Ȃ��Z���t
	  �@�C���`���œo�^���Ă��������B

	 --pause-on-start=msg       Pause on Port Open
	 --pause-on-exit=msg        Pause on exit
	 --list-port or -p?       List COM Port
	 --list-usbasp or -pu?    List USBasp devices
	 --set-serial=XXXXXX      Set USBasp serial number
	 --help
	 --atmel-avr
	 --avr-devices

	 �܂��A�o�^�ł���̂�http�ȊO���\�ł��B�ȉ��̂悤�ɓo�^����΁A
	 pdf�t�@�C���̎Q�Ƃ���s�t�@�C���̋N�����\�ł��B

;# Help       file://c:/bin/HIDaspx.pdf
;# hidspx     file://c:/bin/avrx-tool.txt

	  �Ɠo�^���A�Y������t�@�C��������̏ꏊ�ɃR�s�[���Ă������ƂŁA�t
	  ���̕�����v���E���e�ՂɎQ�Ƃł��܂��B

;# avrstudio  C:\PROGRA~1\atmel\avrtoo~1\AVRSTU~1\AVRSTU~1.exe
	  ���̗�́AAtmel �Ђ� AVRstudio ���N�����邱�Ƃ��ł��܂��B

	 �����͔��ɕ֗��Ɏg���܂����A�����Ɋ댯�ȃR�}���h���o�^���\��
	 ���Ƃ��Ӗ����܂��Bhidspx.ini ���X�V�������Ƃ́A�K�� -? �œo�^���
	 ���m�F���A���e�𗝉�������ł����p���������B

��2009-01-19 	���s���ԕ\���ق��A�e��̉���

  �u--�v�I�v�V�����̒ǉ���e�Ղɂ��邽�߁Aavrspx.c���̃R�[�h�����ǂ��܂����B
        ���̏C���ɂ���āA�@�\�̒ǉ����e�ՂɂȂ�Ǝv���܂��B�܂��A���킹��
        --new-mode �iavrdude �Ɨގ��̐i���\���@�\�j��ǉ����܂����B���ꂪ�x
        �X�g�͍l���Ă���܂��񂪁A�����\�����͉��₩�ɉ�ʂ����������邽
        �߁A�Z�����͊����܂���B���s���Ԃ̕\���@�\��ǉ����A�������x�̕]��
        ���e�ՂɂȂ�܂����B

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


  �u--new-mode-�v
	hidspx.ini�ɏ������ݒ�𖳌������邱�Ƃ��\�ł��B
	--new-mode- �̂悤�ɁA�Ō��'-'������ƁA���̃I�v�V������ݒ肵�Ȃ�
	�Ƃ����Ӗ��ɂȂ�܂��B

  �u-!�v�u�b�N�}�[�N�ꗗ��\�����܂��B-?�ł̕\���̓w���v�\�������ɂ����Ɗ�����
        �̂ŁA�d�l��ύX���܂����B

  �u-o�v�������݃t�@�C���̎w��ł��܂��B
        �]���́A'>' �ŏ������ރt�@�C�����w�肵�܂����B���݂��g���܂����A�I
        �v�V�����Ŏw��ł���Ə������������ł��邱�Ƃ��m�F�ł����̂ŁA�u-o�t
        �@�C���v�i�󔒂ŋ�؂�Ȃ����Ɓj�Ńt�@�C���w����\�ɂ��܂����B��
        �����܂ރt�@�C����"file 123.hex"�̂悤�Ɏw�肵�܂��B
        -onul �Ǝw�肷��ƁA-rp �R�}���h�����s���āANUL �f�o�C�X�ɏ�������
        ���Ƃ��\�ł��B�i���x�̌v���⓮��e�X�g�ɕ֗��j

	���̑��ɂ��w���v�\����ύX���Ă��܂��B�ڍׂ́u-?�v�Ŋm�F���Ă��������B

��2009-01-21  -rF�I�v�V�����ł̕\��������
  
  �u-rF�v�̉���
        ����� FUSE �f�[�^�̏������݂� AVR �`�b�v���g�p�s�\�ɂ���\������
        ��܂��B�����ŁA-rF �I�v�V�����ŕ\������R�}���h�s�� -q(query) �I�v
        �V������ǉ����A�`�b�v����肷��d�l�ɉ��ǂ��܂����B

 >hidspx -rF
 Detected device is ATtiny2313.
 
 DEVICE=attiny2313
 ### hidspx command line example ###
 hidspx -qtiny2313 -d10 -fL0xE4 -fH0xDF -fX0xFF

        �ȉ��̂悤�ɁA�^�[�Q�b�g�� AVR �}�C�R�����w�肵���`�b�v�łȂ���΁A
        ����ȍ~�̃R�}���h�����s���܂���B

 >hidspx -qtiny26 -rp
 Detected device is ATtiny2313.
 Invalid Device tiny26 != tiny2313

  �uMakefile.gcc�v�̏C��
	hidspx-gcc.exe �ɃA�C�R����񂪌����Ă����̂��C��

��2009-01-21 ...	HIDaspx�p�̃t�@�[���E�F�A���C��
	* Linux���main-12.hex�𗘗p����ƃG���[�ɂȂ藘�p�ł��Ȃ��̂��C��
	  ���̕s��́A2008�N11��27���Ɏ��{�����ύX�̕���p�ł��B����ȑO��
	  �ł��g���Ă���ꍇ�ɂ͖��Ȃ����삵�܂��B

	#define USB_CFG_HAVE_INTRIN_ENDPOINT    1
	#define USB_CFG_SUPPRESS_INTR_CODE      1
	#define USB_CFG_INTR_POLL_INTERVAL      200 //���ʂȂ̂Œ���interval
	
	  ���̏C���́ABSD OS��Mac OS�ł̓���ɑΉ�����ׂ̕ύX�ł����A�������������@��
	  �s���Ȃ̂ŁA�ȑO�̐ݒ�ɖ߂��܂����B

	#define USB_CFG_HAVE_INTRIN_ENDPOINT    0
	#define USB_CFG_SUPPRESS_INTR_CODE      0
	#define USB_CFG_INTR_POLL_INTERVAL      10
	

 Error hidWrite(): error sending control message: Device or resource busy ...
 Error hidRead(): error sending control message: Device or resource busy ...

	* avrspx-doc.txt ��ǉ�
	  kkk ����̏����ꂽ avrsp �ɑ΂�����Ǔ_�̐����ł��Bhidspx �́A��
	  �̉��Ǔ_�̑S�Ă��p�����Ă��܂��B

��2009-01-23  �ׂ��Ȍ�����

	* �w���v���b�Z�[�W��typo���C��
	* libusbprog.bat�̑Ώۃt�@�C���̌����C��
	* --new-mode �̐i���\�����C���i�\���̃Y�����C���j

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

��2009-01-24 �I�v�V�����w��̕s����C��
        * -r�I�v�V�����̉E���ɃI�v�V�������w�肷��ƃR�}���h�G���[�ɂȂ�
          �i-rf, -rF�R�}���h��ǉ��������̃R�[�h���s���S�ł����j

��2009-01-25 �R�}���h�\���I�v�V������ǉ�
        * --show-options���w�肷��ƁAini�t�@�C�����܂߂��I�v�V�����ꎮ��
          �R�}���h���C���`���ŕ\�����܂��i�{�O����̒�āj�B
          �������A--show-options���͕̂\�����܂���B
          �܂��Aini�t�@�C���ɐݒ肷�邱�Ƃ�z�����A--show-options-�Ŏw���
          �����\�ɂ��܂����B

��2009-01-26 �R�}���h�\���I�v�V������ǉ�
        * --show-options�̏o�͕͂W���G���[�ɕύX���܂����B
          ���R�́A-rp�œǂݏo����HEX�t�@�C���ւ̃R�}���h�G�R�[�̍�����
          �����邽�߂ł��B�i�d�l�̌������s�\���ł����j
          �Ȃ��A-oREAD.hex�ł́A���̍����͋N���܂���B

��2009-03-07 �u�{�Ɨ��v�̖�������������@��ǋL���܂���
        * bin\firmware\egg-write.bat��ǉ�
        * �W���o�͂̃o�b�t�@�����O���s�P�ʂɕύX���܂����i�O���c�[���ւ̔z���j
        * HIDaspx.pdf���X�V

��2009-03-17 -rI�I�v�V������ǉ�
	* �uEngbedded Atmel AVR(R) Fuse Calculator�v�̐V�łɑΉ�
	* hidspx_tips.pdf�̒ǉ��iHIDaspx.pdf����hidspx��tips�𕪗��j

��2009-03-30 ���HEX�t�@�C����\������@�\��ǉ�
	* hidspx_tips.pdf�̍X�V
	* fuse_en.txt����{������ɏC��(���p����p��fuse_en_orig.txt)
	* ���܂ł́A�������݃f�[�^���܂܂Ȃ��t�@�C�����w�肷��� HELP ���b�Z�[�W
	  ��\�����Ă������A������ȉ��̂悤�ɏC��
	- ���̏C���ŁAHEX �t�@�C����'.' ���܂ރt�@�C�����ł���iHEX�ȊO���j���ƁA
	  EEPROM �p�̃t�@�C���́A.EEP(�啶���������͖��Ȃ�) �̊g���q�ł��邱��
	  ���`�F�b�N����B�i���܂ł̓t�@�C�����̈ꕔ�Ɋ܂�ł���� EEPROM �p�Ɣ�
	  �f���Ă������߁AABC.EEP.HEX �Ƃ����t�@�C���́AEEPROM �p�̃t�@�C���ɉ���
	  ���ꂽ�j

 >cat a.hex
 :00000001FF
 
 >cat a.eep
 :00000001FF
 
 �����̏������݂����s�����
 
 >hidspx a.hex a.eep
 a.hex : HEX File is empty.
 a.eep : HEX File is empty.
 
��L�̕\�����s�Ȃ��������݂��X�L�b�v���܂��B�]���̔ł���������ł����Ausage ���b
�Z�[�W��\�����邽�߁A�R�}���h�w����Ԉ�����ƌ�F����\��������܂��B�i�����A
���̃��b�Z�[�W�̈Ӗ��𗝉�����̂ɂ��΂炭�Y�݂܂����j

