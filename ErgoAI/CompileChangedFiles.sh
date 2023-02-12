#! /bin/sh

PROLOG_COMMAND=$*

if test "$PROLOG_COMMAND" = "" ; then
    echo "Prolog command not specified. Bug in a Makefile?"
    exit 1
fi

# If PROLOG variable is not specified to make,
# then $PROLOG_COMMAND will start with `none'.
# Catch it here and tell what to do.
if test "`echo "$PROLOG_COMMAND" | sed 's/^none//'`" != "$PROLOG_COMMAND" ; then
    echo ""
    echo "*** PLEASE USE ./makeflora TO BUILD FLORA-2"
    echo ""
    exit 1
fi

./touch.sh cmd...

split -l 7 cmd... cmd..._

for f in cmd..._*; do
     cat cmd...hdr $f | $PROLOG_COMMAND
done

rm cmd... cmd...hdr cmd..._*
