@echo off
REM 2008-11-13 (c) written by senshu
REM set COPY=echo (copy)
set COPY=copy

set PROJECT=hidspx
REM コピー先のディレクトリをセットする
set TARGET=C:\bin

if "%1"=="" (
REM 引数なし（標準設定）
) else (
REM HELPのチェック
  if "%1"=="HELP" goto HELP
  if "%1"=="?" goto HELP
  if "%1"=="help" goto HELP
  set TARGET=%1
)

echo ================= %PROJECT% をセットアップします ====================
echo %TARGET%フォルダ にコピーを希望する場合は、[Y]を大文字で入力のこと。
SET ANS=n
SET /P ANS="↑にセットアップをしますか[Y/n]"
IF %ANS%==Y (
  GOTO START
) ELSE (
  GOTO WHERE
)

:WHERE
SET TARGET=
SET /P TARGET="希望するフォルダ名を入力してください:"
SET ANS=n
SET /P ANS="%TARGET%フォルダにセットアップをしますか？ [Y/n]"
IF %ANS%==Y (
  GOTO START
) ELSE (
  echo ================= %PROJECT% のセットアップを中断 ====================
  GOTO END
)

:START
echo 必要なファイル群を%TARGET%にコピーします。

REM Check
IF NOT EXIST %TARGET% ( 
  mkdir %TARGET%
)
echo ==== [COPY to %TARGET%] ====
echo hidspx.exe, hidspx-gcc.exe hidspx.ini, libusb0.dll, fuse.txt, usbtool.exe, hidspx-GUI.exe
echo firmware\genserial.awk , firmware\main-12.hex , firmware\firmprog.bat
echo ..\HIDaspx.pdf ..\hidspx_tips.pdf ..\Readme-j.txt ..\avrx-tool.txt
%COPY% hidspx.exe %TARGET% > NUL
%COPY% hidspx-gcc.exe %TARGET% > NUL
IF EXIST %TARGET%\hidspx.ini (
  echo **** original hidspx.ini to hidspx-save.ini ****
  echo original hidspx.ini to hidspx-save.ini
  %COPY% %TARGET%\hidspx.ini %TARGET%\hidspx-save.ini
)
%COPY% hidspx.ini %TARGET% > NUL
%COPY% libusb0.dll %TARGET% > NUL
%COPY% fuse.txt %TARGET% > NUL
%COPY% usbtool.exe %TARGET% > NUL
%COPY% hidmon.exe %TARGET% > NUL
%COPY% hidmon.dll %TARGET% > NUL
%COPY% hidspx-GUI.exe %TARGET% > NUL
%COPY% firmware\genserial.awk %TARGET% > NUL
%COPY% firmware\main-12.hex %TARGET% > NUL
%COPY% firmware\firmprog.bat %TARGET% > NUL
%COPY% ..\HIDaspx.pdf %TARGET% > NUL
%COPY% ..\hidspx_tips.pdf %TARGET% > NUL
%COPY% ..\Readme-j.txt %TARGET% > NUL
%COPY% ..\avrx-tool.txt %TARGET% > NUL
echo ==== %PROJECT% setup finished ====
goto END

:HELP
  echo これは %PROJECT%のセットアッププログラムであり、更新時にも利用できます。
  echo   setup [希望するフォルダ名]
  echo のように使います。
  echo [希望するフォルダ名]を省略すると標準とする%TARGET%にコピーします。

:END
pause
