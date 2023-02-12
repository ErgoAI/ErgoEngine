#! /bin/sh

# Run dockerized ErgoAI with GUI support
#    run-docker-ergoai.sh  [-admin] [-gui] [ostype]
# ostype must be ubuntu, centos, or nothing (then ubuntu is assumed)

if [ "$1" = "-admin" ]; then
    entrypoint="--entrypoint /bin/bash"
    adminflag=1
    shift
fi
if [ "$1" = "-gui" ]; then
    gui=$1
    shift
else
    gui=
fi

ostype=$1

if [ -z "$ostype" ]; then
    ostype=ubuntu
fi
if [ "$ostype" != "ubuntu" -a "$ostype" != "centos" ]; then
    echo +++ Unsupported OS type: "$ostype"
    echo run-docker-ergoai.sh [-admin] [-gui] [ostype]
# ostype must be ubuntu, centos, or nothing (then ubuntu is assumed)
    exit 1
fi

# mount volume
# To mount external volume, uncomment below. Change paths as needed.
#MOUNTCMD="--volume $HOME/stuff:/home/ergouser/stuff"
MOUNTCMD2="--volume $HOME/docker/ergouser/.xsb:/home/ergouser/.xsb/"

if [ "`uname`" = "Darwin" ]; then
MYDISPLAY=host.docker.internal:0
else
MYDISPLAY=$DISPLAY
fi

if [ -n "$gui" ]; then
    xhost + 127.0.0.1
    # Generally, --privileged is discouraged, but we need it to avoid warnings
    # about connecting to dbus for graphical apps. Our container is pretty
    # safe because one cannot log into root and the default user is non-system
    PRIVILEGED=--privileged
    X11FLAGS="-e DISPLAY=$MYDISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix"
else
    PRIVILEGED=
    X11FLAGS=
fi

TTYFLAGS=--tty

# --net=host works only on Linux
RUNCMD="sudo docker run --net=host $PRIVILEGED $entrypoint $MOUNTCMD $MOUNTCMD2 $X11FLAGS --interactive $TTYFLAGS coherent:ergoai-$ostype$gui"

echo
echo  $RUNCMD
echo
$RUNCMD

if [ -n "$gui" ]; then
    xhost -
fi
