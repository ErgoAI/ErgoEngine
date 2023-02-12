#!/bin/sh

ERGOVERSION=./version.flh

version=`git rev-parse --short=7 HEAD`
perl -i -pe "s/Revision: [^\n]*/Revision: $version'/"  $ERGOVERSION
perl -i -pe "use POSIX qw(strftime); my \$date=strftime \"%F %T\", gmtime; s/Date: [^\n]*/Date: \$date'/"  $ERGOVERSION

