#! /bin/sh

# This is needed because /usr/ucb/touch is broken, but some people have
# /usr/ucb at the top of their PATH

if test -x /bin/touch ; then
   /bin/touch $*
elif test -x /usr/bin/touch ; then
   /usr/bin/touch $*
else
   echo ""
   echo "Can't find the touch command. Build might fail or be incomplete."
   echo ""
fi
    
