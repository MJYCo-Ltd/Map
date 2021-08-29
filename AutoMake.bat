setlocal
del /f /s Makefile
call "C:\Qt\5.15.2\msvc2019_64\bin\qtenv2.bat"
call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars64.bat"
cd /d %~dp0
qmake
nmake all 
echo.
echo.
echo "====================重新编译成功===================="
echo.
echo.
endlocal
Pause