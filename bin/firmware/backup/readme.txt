[3] genserial.awk (�V���A���ԍ������c�[��)�̎g����

HIDaspx �p�̃V���A���f�[�^�iHEX �t�@�C���j�𐶐�����c�[����p�ӂ��܂����B

�@���̃c�[���� gawk(GNU awk �Ƃ����C���^�[�v���^�j ���g���ē��삵�܂��BAVR �}�C�R
���̗��p�҂ɍL�����y���Ă��� WinAVR �ɂ́Agawk.exe ���t�����Ă��܂����Agawk ��
�p�ł��Ȃ����́A�o�O�����Ȃ��P�Ƃœ��삷��A�ؑ�����̈ڐA�� gawk ����肵�Ă���
�����B

2009�N9��4�� gawk 3.1.7 ���{��Ńo�C�i���o�Ofix�ς̍ŐV�� GAWK�̉��ǔł̓����
 �� http://www.kt.rim.or.jp/~kbk/gawk-3.1/

http://www.kt.rim.or.jp/%7ekbk/cgi-bin/download.cgi?dl-file1=gawk-mbcs-win32-20091124.zip

���肵�� gawk-mbcs-win32-20080704.zip ��W�J���Agawk.exe �� hidspx.exe �Ɠ����f�B
���N�g���ɃR�s�[���܂��B

 2007/10/22  15:50              280,351 ChangeLog
 2007/07/16  23:59               35,147 COPYING
 2008/07/02  16:47              189,434 mbc-diff
 2007/09/25  14:58               83,133 NEWS
 2008/07/03  16:51              729,088 gawk.exe
 2006/12/01  14:26               53,248 igawk.exe
 2008/07/03  17:22                9,588 readme.ja

2008-1028 �ňȍ~�A�u-v adr= �v���w�肷�邱�ƂŁA�C�ӂ̃A�h���X�ɑ΂���V��
�A�����𐶐��ł��A�܂��A�����̃V���A���ԍ����u���ɐ����\�ł��B

usage: gawk -f genserial.awk -v t={aspx|mon88} start-number  [end-number]
�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P

�g�p��1�F
 > gawk -f genserial.awk 0001

�̂悤�Ɏ��s����ƁA0001 �̃V���A���ԍ���ݒ肷�� HEX �t�@�C���i_0001.hex�j
�𐶐����܂��B

�g�p��2�F0001�`0100�̃V���A���ԍ��̃t�@�C�����ꊇ���Đ������܂��B
         �w��ł���l��1�`9999�܂ŁA����𒴂���ƃG���[�ɂȂ�܂��B

 > gawk -f genserial.awk 1 100


�g�p��3�F16�i����p�啶���̎w����\�ł��B�������͑啶���ɕϊ����܂��B
���@�������啶�������Ă��闝�R�́AWindows �̃t�@�C���V�X�e�����A�啶����
  ��������ʂ��Ȃ����Ƃɂ��t�@�C���̏㏑��������邽�߂ł��B���̕��@�ŃV
  ���A���ԍ����s������ꍇ�́A��������t�@�C�������H�v���A�������ł��d����
  �Ȃ��t�@�C�����̐������s���K�v������܂��j

 > gawk -f genserial.awk ABCD

���̃t�@�C���� main.hex �̃t�@�[���Ɠ����ɏ������߂΁AHIDaspx �͂��̃V���A
���ԍ������������̂ɂȂ�܂��B

 > hidspx main.hex _0001.hex

�� �擪������'_'��ǉ����܂����̂ŁA�ȉ��̂悤�ȑ��삪�\�ł��B
 > del _*.hex
 �ňꊇ���ăV���A���ԍ��p�̃t�@�C�����폜�ł��܂��B

�g�p��4�F
 > gawk -f genserial.awk -v t=mon88 0001

bootmon88�p�̃V���A���ԍ��𐶐����܂��B

�g�p��5�F
 > gawk -f genserial.awk -v t=1234 0001

1234(10�i��)�Ԓn����n�܂�4���̃V���A���ԍ��𐶐����܂��B

