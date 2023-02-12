@echo OFF

@set thisdir=%0\..\

REM To find this directory for your installation, start Ergo and type
REM     system{installdir=?Dir}.
@set ERGOROOT="H:\ERGO\ergosuite\Ergo"

REM Preferably use forward slashes but not necessary

REM To find this directory for your installation, start Ergo and type
REM     system{archdir=?Dir}.
@set XSBARCHDIR="H:/XSB/XSB/config/x64-pc-windows"

REM Find out where your Python executable is on your Windows system
@set PYTHON="%USERPROFILE%\AppData\Local\Programs\Python\Python36\python.exe"

REM NOTE: PYERGO requires the package 'six'. To install it, run
REM           pip install six
REM       in a terminal window.
REM       pip is likely installed in the same directory as python.

%PYTHON% %ERGOROOT%\python\pyergo_example.py %XSBARCHDIR% %ERGOROOT%



