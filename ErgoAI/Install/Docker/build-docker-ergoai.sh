#! /bin/sh

# Build dockerized ErgoAI with or without GUI support, for ubuntu or centos

# build-docker-ergoai.sh  [-ergo-version <version-string>] <ostype> [gui]
# ostype: ubuntu or centos

if [ "$1" = "-ergo-version" ]; then
    ergo_version="_$2"
    build_arg="--build-arg ergo_version=$ergo_version"
    shift
    shift
fi

# --squash seems useless now that dockerfiles have only one RUN statement
SQUASHFLAG=--squash
SQUASHFLAG=

ostype=$1
gui=$2

if [ "$ostype" != "ubuntu" -a "$ostype" != "centos" ]; then
    if [ -z "$ostype" ]; then
        echo +++ OS type not given
    else
        echo +++ Unsupported OS type: "$ostype"
    fi
    exit 1
fi

if [ -z "$gui" ]; then
    gui=
else
    gui="-gui"
fi

sudo docker build --rm --tag coherent:ergoai$ergo_version-$ostype$gui $build_arg --file Dockerfile-$ostype$gui . $SQUASHFLAG
