setlocal
call "C:\Qt\5.15.2\msvc2019_64\bin\qtenv2.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
cd /d %~dp0
qmake
nmake all 
echo.
echo.
echo "====================���±���ɹ�===================="
echo.
echo.
endlocal
Pause