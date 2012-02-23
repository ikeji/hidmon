@echo off
echo RS232-RC方式の書き込みを開始します。
echo RESET 解除SWを押してから、Enterキーを押下してください
pause
echo 書き込み終了まで、約40秒です
REM -pf1 の部分は、使用環境に合わせて修正してください
hidspx -pf1 -ttiny2313  -fL0xFF -fH0xDB -fX0xFF main-12.hex
echo 書き込み完了しました
