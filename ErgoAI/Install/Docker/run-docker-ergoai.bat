@echo OFF

REM Run dockerized ErgoAI
REM    run-docker-ergoai.sh  [-admin] [-gui] [ostype]
REM ostype must be ubuntu, centos, or nothing (then ubuntu is assumed)

if [%1] == [-admin] (
    @set entrypoint = --entrypoint /bin/bash
    @set adminflag = 1
    shift
)
if [%1] == [-gui] (
    @set gui = %1
    shift
) else @set gui=

@set ostype = %1

if [%ostype%] == [] (
    @set ostype = ubuntu
)
if NOT [%ostype%] == [ubuntu] (if NOT [%ostype%] == [centos] @goto OS_usage)

REM  mount volume
REM  To mount external volume, uncomment below. Change paths as needed.
REM MOUNTCMD = --volume %USERHOME%/stuff:/home/ergouser/stuff
@set MOUNTCMD2 = --volume %USERHOME%/docker/ergouser/.xsb:/home/ergouser/.xsb/

@set MYDISPLAY = 127.0.0.1:0

if [%gui%] == [] (
    @set PRIVILEGED=
    @set X11FLAGS=
) else (
    xhost + 127.0.0.1
    REM Generally, --privileged is discouraged, but we need it to avoid warnings
    REM about connecting to dbus for graphical apps. Our container is pretty
    REM safe because one cannot log into root and the default user is non-system
    @set PRIVILEGED = --privileged
    @set X11FLAGS = -e DISPLAY=%MYDISPLAY% -v //c/windows/temp/.X11-unix:/tmp/.X11-unix
)

@set TTYFLAGS = --tty

REM --net=host works only on Linux
@set RUNCMD = sudo docker run --net=host %PRIVILEGED% %entrypoint% %MOUNTCMD% %MOUNTCMD2% %X11FLAGS% --interactive %TTYFLAGS% coherent:ergoai-%ostype%%gui%

@echo.
@echo  %RUNCMD%
@echo
%RUNCMD%

if NOT [%gui%] == [] xhost -



:OS_usage
@echo.
@echo.
@echo +++++ OS type must be ubuntu or centos
@echo.
goto end

:end

@echo.


REM Local Variables:
REM coding-system-for-write:  iso-2022-7bit-dos
REM End:
