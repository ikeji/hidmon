                                                             2008�N10��3��

        USB�ڑ������̃h���C�o�C���X�g�[���s�v��AVR���C�^�iHIDaspx�j
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[0] �͂��߂�

HIDaspx�Ƃ́A�h���C�o�̃C���X�g�[�����s�v��USB�ڑ�������AVR���C�^�ł��B

�r�l������̍쐬���ꂽ HIDasp �����ɁAiruka ����� senshu �������ŉ��ǂ��s
�Ȃ��܂����B���̐��ʂ������܂����̂ŁA2008�N 9�� 22�������J���J�n����
���BHIDaspx �́A�r�l������̌��J���Ă�����̂Ƃ͓��e���قȂ�Afirmware, 
hidspx �R�}���h���Ɍ݊����͂���܂���̂ŋ�ʂ��Ĉ����Ă��������B

�� HIDaspx�́A�G�C�`�E�A�C�E�f�B�[ �A�X�y�b�N�X�Ƃ��ǂ݂��������B

[1] �����̃t�@�C���ɂ���

2 �� exe �t�@�C���ihidspx.exe, hidspx-bcc.exe�j�̋@�\�͓����ł��B
hidspx.exe �𗘗p���āA����Ɉُ�����������ɂ́Ahidspx-bcc.exe ���g����
�݂Ă��������B�S�p�������܂ރt�H���_�ł̍�Ǝ��Ɍ��ʓI�ȏꍇ������܂��B
�i�R�}���h���͔C�ӂɉ����ł��܂����A�������Ȃ����O�����Ă��������B�j

�܂��Ahidspx �̃R�}���h�����g���Ă��܂����A�����avrspx �̕ʖ��ł��B
���̖��̂ɂ��Ă���̂� HIDaspx �ɑΉ����邱�Ƃ��������߂ł��Bhidspx �Ƃ���
���O�ł����Ă� avrspx �̋@�\�͑S�ė��p�\�ł��B�������A�@�\�ǉ���Ƃɂ��A
avrspx �̋@�\�ɈӐ}���Ȃ�����p��������\��������܂��B���̂��߁A�\���ȓ�
�쌟�؂��I����܂ł́Aavrspx �̖��̂������Ɍ��J���邱�Ƃɂ��܂����B������
���������B


.\
����bin
��	libusb0.dll
��	hidspx.exe		�c MinGW�ŃR���p�C����������
��	hidspx-bcc.exe		�c Borland C++ ver 5.5.1�ŃR���p�C����������
��	main.hex		�c HIDasp�̃t�@�[���E�F�A
��	hidspx.ini		�c hidspx�̏������t�@�C��
��	fuse.txt		�c FUSE�����ڍׂɕ\������ׂ̃e�L�X�g�t�@�C��
��	fuse_en.txt
��	fuse_j.txt
����firmware			�c HIDasp�̃t�@�[���E�F�A�\�[�X
����hidspx-src			�c hidspx�̃\�[�X(MinGW, Borland C++���p)
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
����pict			�c ��H�}�Ǝ�����

[2] ����

bin/main.hex �� Tiny2313 �ɏ�������ł��������D�q���[�Y�̐ݒ�́C�O�t���� 
12MHz �̃N���X�^���ɍ��킹�A�ȉ��̂悤�ɐݒ肵�܂��B

Low: 10001111
     ||||++++-- CKSEL[3:0] �V�X�e���N���b�N�I��
     ||++-- SUT[1:0] �N������
     |+-- CKOUT (0:PD2�ɃV�X�e���N���b�N���o��)
     +-- CKDIV8 �N���b�N���������l (1:1/1, 0:1/8)

High:11-11011
     |||||||+-- RSTDISBL (RESET�s�� 1:�L��, 0:����(PA2))
     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
     |||+-- WDTON (WDT 0:�펞ON, 1:�ʏ�)
     ||+-- SPIEN (1:ISP�֎~, 0:ISP����) ��Parallel���̂�
     |+-- EESAVE (������EEPROM�� 1:����, 0:�ێ�)
     +-- DWEN (On-Chip�f�o�b�O 1:����, 0:�L��)

Ext: -------1
            +-- SPMEN (SPM���� 1:����, 0:�L��)


[3] �g����

hidspx �Ɂu-ph�v�w��� HIDaspx �����p�ł��܂��Bhidspx.ini �� -pu �������Ă�
���΁A���̎w��͏ȗ��ł��܂��B

> hidspx -ph test.hex

-d �Ńf�B���C���w��ł��A���l�͎��Ԃɔ�Ⴕ�܂��B1MHz �� RC ���U���[�h�ł�
-d4 �ȏ���w�肵�Ă��������B�Ȃ��A���̒l�́A�����ɓ��삷�� AVR �}�C�R���ł�
�����Ȓl���w��ł��A16MHz �ȏ�̎��g���œ��삷�� AVR �}�C�R���ɂ� -d0 ���w
��ł��܂��B�w��l���������قǁAR/W �̑��x�����サ�܂��B

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

�ڍׂȎg�����́Aavrspx�̉���y�[�W���������������B

�� HIDaspx�������ɗ��p������@

AT90S2313 �̂悤�ɁA�y�[�W�����݂��T�|�[�g���Ȃ� AVR �}�C�R���ł͏����݂Ɏ�
�Ԃ�������܂��BUSB 2.0 HUB ������ɐڑ����āA���l�̏����݂��s�������ʂ�
���B

�� hidmon�ɂ��x���`�}�[�N����
AVR> bench
hid write start
hid write end, 38000 bytes/11172 ms,  3401 bytes/s

�� Write&Verify����
>timeit hidspx -d2 2kb.hex
Elapsed Time:     0:00:25.703

���ɁA1,000 �~�ȓ��Ŕ����� HUB ����āA���l�̌v�����s���Ă݂܂��B

�� hidmon�ɂ��x���`�}�[�N����
>hidmon
AVR> bench
hid write start
hid write end, 38000 bytes/2468 ms,  15397 bytes/s

�� Write&Verify����
>timeit hidspx -d2 2kb.hex
Elapsed Time:     0:00:09.546

ATtiny2313 �ɔ�ׂĒx���̂ł����AHUB �����ɔ�ׂāA1/2 �ȉ��ɒZ�k����A10�b
�����ŏ����݁��ƍ����������܂��B

USB HUB �����ɂ悢�d�������Ă��邱�Ƃ��킩��܂��Bhidmon �� 15kB/s �ȓ���
���܂����p�ӂ���̂����x����ɂȂ���܂��BHUB ���������ł���΁A����
����ė��p���Ă݂Ă͂������ł��傤���B

===============================================================================

�� ���C�Z���X�i�r�l������ɓ����j
AVR USB�ɏ�����GPL2�Ƃ��܂��D

�� �ύX����
2008-09-22 ... senshu��iruka���񂪉��������{	�������Ή����{

2008-09-24 ... senshu  firmware -d0�I�v�V�����̍œK��
               hidpsx�̃G���[���o���A�̃��b�Z�[�W�o�͂̏C���i���s���ĕ\���j

2008-09-28 ... hidspx�̃R���p�C���I�v�V������-Wall��ǉ����A�x�����b�Z�[�W��
               �΂���C�������{(senshu)
               firmware -d0�I�v�V������USBasp�݊��������߂�(iruka)
               MOSI, SCK �̋���������iHi-Z������j

2008-10-03 ... - hidspx�̃��b�Z�[�W��HIDaspx�ɓ���
               - Borland C++ �ł̌x�����b�Z�[�W��}�~(avrspx.h���C��)
               - usbtool ��ǉ�
               - kuga����̃A�h�o�C�X�ɂ��AFirmware�T�C�Y��40�o�C�g�k��(1968�o�C�g)
                 avr-size --mcu=attiny2313 main.elf
                   text    data     bss     dec     hex filename
                   1966       4      85    2055     807 main.elf
               - USB��ProductID 0x5dc (libusb device) �Ƃ̋����������ׁA
                 0x5df(HID devide)�ɕύX
               - firmware�̕ύX �i���̂Ƃ���T�|�[�g���Ă���AVR�f�o�C�X�ł́j
                 page_addr �� 256 �ȉ��Ȃ̂ŁAuint16 ���� uint8 �ɍ~�i�B
               - �ÓI���W�X�^���t�����{(�ꎞ�A���~)


# TAB size = 8�ŕҏW���Ă��܂��B

