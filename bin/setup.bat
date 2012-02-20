@echo off
REM 2008-11-13 (c) written by senshu
REM 2009-08-07 Windows 98�p�̃`�F�b�N��ǉ�
REM 2010-06-28 libusb0.dll �̖��O��ύX
REM set COPY=echo (copy)
set COPY=copy

set PROJECT=hidspx
REM �R�s�[��̃f�B���N�g�����Z�b�g����
set TARGET=C:\bin

REM OS�̃`�F�b�N
if "%comspec%"=="" goto unknown
if "%comspec%"=="c:\command.com" goto Win9x
if "%comspec%"=="%systemroot%\system32\cmd.exe" goto WinNT

:unknown
echo OS�̎�ނ�������܂���B
goto END

:Win9x
echo ���ƂŁA�t�@�C���Q�̃R�s�[���s���Ă��������B
goto END

:WinNT

if "%1"=="" (
REM �����Ȃ��i�W���ݒ�j
) else (
REM HELP�̃`�F�b�N
  if "%1"=="HELP" goto HELP
  if "%1"=="?" goto HELP
  if "%1"=="help" goto HELP
  set TARGET=%1
)

echo ================= %PROJECT% ���Z�b�g�A�b�v���܂� ====================
echo %TARGET%�t�H���_ �ɃR�s�[����]����ꍇ�́A[Y]��啶���œ��͂̂��ƁB
SET ANS=n
SET /P ANS="���ɃZ�b�g�A�b�v�����܂���[Y/n]"
IF %ANS%==Y (
  GOTO START
) ELSE (
  GOTO WHERE
)

:WHERE
SET TARGET=
SET /P TARGET="��]����t�H���_������͂��Ă�������:"
SET ANS=n
SET /P ANS="%TARGET%�t�H���_�ɃZ�b�g�A�b�v�����܂����H [Y/n]"
IF %ANS%==Y (
  GOTO START
) ELSE (
  echo ================= %PROJECT% �̃Z�b�g�A�b�v�𒆒f ====================
  GOTO END
)

:START
echo �K�v�ȃt�@�C���Q��%TARGET%�ɃR�s�[���܂��B

REM Check
IF NOT EXIST %TARGET% ( 
  mkdir %TARGET%
)
echo ==== [COPY to %TARGET%] ====
echo hidspx.exe, hidspx-gcc.exe hidspx.ini, libusb0_.dll, fuse.txt, usbtool.exe,
echo hidspxG.exe, hidspx-GUI.exe
echo firmware\regserial.awk , firmware\addserial.awk, firmware\main-12.hex , 
echo firmware\firmprog.bat, firmware\hexcleanup.bat
echo ..\HIDaspx.pdf ..\hidspx_tips.pdf ..\Readme-j.txt ..\avrx-tool.txt
%COPY% hidspx.exe %TARGET% > NUL
%COPY% hidspx-gcc.exe %TARGET% > NUL
IF EXIST %TARGET%\hidspx.ini (
  echo **** original hidspx.ini to hidspx-save.ini ****
  echo original hidspx.ini to hidspx-save.ini
  %COPY% %TARGET%\hidspx.ini %TARGET%\hidspx-save.ini
)
%COPY% hidspx.ini %TARGET% > NUL
%COPY% libusb0_.dll %TARGET% > NUL
%COPY% fuse.txt %TARGET% > NUL
%COPY% usbtool.exe %TARGET% > NUL
%COPY% hidmon.exe %TARGET% > NUL
%COPY% hidmon.dll %TARGET% > NUL
%COPY% hidspxG.exe %TARGET% > NUL
%COPY% hidspx-GUI.exe %TARGET% > NUL
%COPY% firmware\regserial.awk %TARGET% > NUL
%COPY% firmware\addserial.awk %TARGET% > NUL
%COPY% firmware\main-12.hex %TARGET% > NUL
%COPY% firmware\firmprog.bat %TARGET% > NUL
%COPY% firmware\hexcleanup.bat %TARGET% > NUL
%COPY% ..\HIDaspx.pdf %TARGET% > NUL
%COPY% ..\hidspx_tips.pdf %TARGET% > NUL
%COPY% ..\Readme-j.txt %TARGET% > NUL
%COPY% ..\avrx-tool.txt %TARGET% > NUL
echo ==== %PROJECT% setup finished ====
goto END

:HELP
  echo ����� %PROJECT%�̃Z�b�g�A�b�v�v���O�����ł���A�X�V���ɂ����p�ł��܂��B
  echo   setup [��]����t�H���_��]
  echo �̂悤�Ɏg���܂��B
  echo [��]����t�H���_��]���ȗ�����ƕW���Ƃ���%TARGET%�ɃR�s�[���܂��B

:END
pause
