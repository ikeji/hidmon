                                                             2008�N9��22��

�� �͂��߂�

HIDasp�Ƃ́A�h���C�o�̃C���X�g�[�����s�v��USB�ڑ�������AVR���C�^�ł��B

�r�l������̍쐬���ꂽ HIDasp �����ɁAiruka ����� senshu �������ŉ��ǂ��s
�������̂ł��B

�� �����̃t�@�C���ɂ���

���̃A�[�J�C�u�́AHIDasp �������Ή��łł���A�r�l������̌��J���Ă�����̂�
�͓��e���قȂ�܂��B�����_�ł́Afirmware, hidspx �Ƃ��Ɍ݊����͂���܂����
�ŋ�ʂ��Ĉ����Ă��������B

�Ȃ��A2 �� exe �t�@�C���ihidspx.exe, hidspx-bcc.exe�j�̋@�\�͓����ł��B
hidspx.exe �𗘗p���āA����Ɉُ�����������ɂ́Ahidspx-bcc.exe ���g����
�݂Ă��������B�S�p�������܂ރt�H���_�ł̍�Ǝ��Ɍ��ʓI�ȏꍇ������܂��B
�i�R�}���h���͔C�ӂɉ����ł��܂����A�������Ȃ����O�����Ă��������B�j

�܂��Ahidspx �̃R�}���h�����g���Ă��܂����A�����avrspx �̕ʖ��ł��B
HIDasp �ɑΉ����邱�Ƃ������Ƌ��ɁAavrspx �̋@�\�͑S�ė��p�\�ł��B�������A
�@�\�ǉ���Ƃɂ��A avrspx �̋@�\�ɈӐ}���Ȃ�����p��������\���������
���B���̂��߁A�\���ȓ��쌟�؂��I����܂ł́Aavrspx �̖��̂������Ɍ��J����
���Ƃɂ��܂����B���������������B


.\
����bin
	libusb0.dll
	hidspx.exe			�c MinGW�ŃR���p�C����������
	hidspx-bcc.exe		�c Borland C++ ver 5.5.1�ŃR���p�C����������
	main.hex			�c HIDasp�̃t�@�[���E�F�A
	hidspx.ini			�c hidspx�̏������t�@�C��
	fuse.txt			�c FUSE�����ڍׂɕ\������ׂ̃e�L�X�g�t�@�C��
	fuse_en.txt
	fuse_j.txt
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

�� ����

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


�� �g����

hidspx �Ɂu-ph�v�w��� HIDasp �����p�ł��܂��Bhidspx.ini �� -pu �������Ă�
���΁A���̎w��͏ȗ��ł��܂��B

> hidspx -ph test.hex

-d �Ńf�B���C���w��ł��A���l�͎��Ԃɔ�Ⴕ�܂��B1MHz �� RC ���U���[�h�ł�
-d4 �ȏ���w�肵�Ă��������B�Ȃ��A���̒l�́A�����ɓ��삷�� AVR �}�C�R���ł�
�����Ȓl���w��ł��A����ɔ����AR/W �̑��x�����サ�܂��B

iruka����̌v�����ʂ����߂��܂��B

�R�}���h	�]�����e	���ǑO	��������
hidspx -d0 -rp >XX.hex	8kB Read	8.39�b	2.22�b(3,690B/�b)
hidspx -d0 test.hex	8kB Write/Verify	10.94�b	4.86�b(1.64kB/�b)
hidspx -d1 -rp >XX.hex	8kB Read	8.40�b	3.12�b(2,625B/�b)
hidspx -d1 test.hex	8kB Write/Verify	10.96�b	7.06�b(1.13kB/�b)
hidspx -d4 -rp >XX.hex	8kB Read	8.39�b	4.63�b(1,770B/�b)
hidspx -d4 test.hex	8kB Write/Verify	13.26�b	9.98�b(821B/�b)

�ڍׂȎg�����́Aavrspx�̉���y�[�W���������������B

�� ���C�Z���X�i�r�l������ɓ����j
AVR USB�ɏ�����GPL2�Ƃ��܂��D

�� History
2008-09-22 ... senshu, iruka�����������{
	�������Ή�����

