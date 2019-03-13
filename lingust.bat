echo Setting up environment for Qt usage...
set PATH=C:\Qt\5.12.1\msvc2017_64\bin;%PATH%
C:\Qt\5.12.1\msvc2017_64\bin\lupdate zebra.pro -ts zebra_ru.ts
pause
C:\Qt\5.12.1\msvc2017_64\bin\linguist.exe
pause
C:\Qt\5.12.1\msvc2017_64\bin\lrelease  zebra_ru.ts -qm zebra_ru.qm
pause
