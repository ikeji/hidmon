@echo off
echo RS232-RC�����̏������݂��J�n���܂��B
echo RESET ����SW�������Ă���AEnter�L�[���������Ă�������
pause
echo �������ݏI���܂ŁA��40�b�ł�
REM -pf1 �̕����́A�g�p���ɍ��킹�ďC�����Ă�������
hidspx -pf1 -ttiny2313  -fL0xFF -fH0xDB -fX0xFF main-12.hex
echo �������݊������܂���
