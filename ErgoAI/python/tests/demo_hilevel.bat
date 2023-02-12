@echo OFF

REM Prefer to use forward slashes but not necessary
@set XSBARCHDIR="H:/XSB\XSB/config/x64-pc-windows"
@set ERGOROOT="H:/FLORA\flora2"

@set PYTHON="C:\Users\Michael Kifer\AppData\Local\Programs\Python\Python36\python.exe"

%PYTHON% demo_hilevel.py 
REM %PYTHON% demo_hilevel.py %XSBARCHDIR% %ERGOROOT%



