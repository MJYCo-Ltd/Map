setlocal
git pull
del /f /s Makefile*
call "C:\Qt\5.15.2\msvc2015_64\bin\qtenv2.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
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