#!/bin/sh 

# Create an XSB/ErgoAI/Studio installer for developers

# Run this script as
#  ./ErgoEngine/ErgoAI/Install/ergo_dev.sh [-v Version] [nointeraction]
# in a folder that has ./XSB, and ./ErgoEngine/ErgoAI

# Arguments: none, one, two, or three
# If arg1 is -v version: build that version.
# If arg1 is not -v then build a non-interactive install
# If arg1 is -v and arg3 is given, build the version labeled arg2 using
# non-interactive install


Ergo_base="./ErgoEngine/ErgoAI"
Ergo_base2="ErgoEngine/ErgoAI"
Ergo_parent="/ErgoEngine"


ergo_version=`cat $Ergo_base/version.flh| grep ERGO_VERSION | sed "s|^.*'\(.*\)'.*$|\1|"`

if [ "$1" = "-v" ] ; then
    shift
    VERSION="$1"
    shift
else
    VERSION=$ergo_version
fi

if [ "$1" != "" ]; then
    noninteractive="_ni"
fi

# ErgoAI will be installed in Coherent/$OUTDIR
OUTDIR=ERGOAI_DEV_$VERSION
outfile_suffix=_$VERSION$noninteractive
# $OUTFILE is the name of the file xxxx.run
OUTFILE=ergoAI_dev$outfile_suffix
echo creating $OUTFILE.run

files="./XSB/LICENSE ./XSB/INSTALL \
        ./XSB/README  \
        ./XSB/FAQ ./XSB/Makefile \
        ./XSB/build/ac* ./XSB/build/*.in ./XSB/build/config.guess \
        ./XSB/build/config.sub ./XSB/build/*sh ./XSB/build/*.msg \
        ./XSB/build/configure ./XSB/build/README \
        ./XSB/build/windows* \
        ./XSB/emu ./XSB/syslib ./XSB/cmplib  ./XSB/lib \
	./XSB/gpp \
	./XSB/bin \
	./XSB/prolog_includes \
        ./XSB/etc \
        ./XSB/packages \
        ./XSB/installer \
        ./XSB/InstallXSB.jar \
        ./XSB/examples \
    	$Ergo_base/run* \
        $Ergo_base/make* \
        $Ergo_base/Makefile \
    	$Ergo_base/etc/*.png $Ergo_base/etc/*.eps $Ergo_base/etc/*.ico $Ergo_base/etc/*.icns \
    	$Ergo_base/extensions \
    	$Ergo_base/opt \
        $Ergo_base/*.sh \
        $Ergo_base/rlwrap_keywords.txt \
        $Ergo_base/Tools \
        $Ergo_base/Install \
        $Ergo_base/Studio_scripts \
        $Ergo_base/*.[PH] $Ergo_base/*.xwam $Ergo_base/*.flh\
        $Ergo_base/AT \
        $Ergo_base/ATinc  \
    	$Ergo_base/closure \
    	$Ergo_base/datatypes \
    	$Ergo_base/debugger \
    	$Ergo_base/demos \
    	$Ergo_base/emacs \
    	$Ergo_base/flrincludes \
    	$Ergo_base/genincludes \
    	$Ergo_base/headerinc \
    	$Ergo_base/includes \
    	$Ergo_base/lib \
    	$Ergo_base/libinc \
    	$Ergo_base/ergo_libinc \
    	$Ergo_base/cc \
        $Ergo_base/pkgs/ \
        $Ergo_base/pkgsinc/ \
	$Ergo_base/ergo_pkgs \
	$Ergo_base/ergo_lib \
	$Ergo_base/python \
        $Ergo_base/syslib \
    	$Ergo_base/syslibinc  \
        $Ergo_base/ergo_syslib \
        $Ergo_base/java \
        ./runErgoAI.sh "

## excluded doc files
#        ./XSB/docs/userman/manual?.pdf \
#        $Ergo_base/docs/*.pdf \

if [ -d $Ergo_base -a -d ./XSB ]; then
    flrdir=$currdir/$Ergo_base2
    xsbdir=$currdir/XSB
else
    echo "This script must be run as $Ergo_base/Install/ergo_dev.sh"
    echo "The folders ./XSB and $Ergo_base must reside in the current folder"
    exit 1
fi


EXCLUDEFILE=$Ergo_base/Install/.excludedFiles

cat > $EXCLUDEFILE <<EOF
CVS
*.conf
*.log
.#*
.cvsignore
.svn
.excludedFiles
*.zip
*.tar
*.bz2
*.gz
*.Z
*~
*.bak
*-sv
*-old
.*.tmp
*.tmp
EOF

cp $Ergo_base/Studio_scripts/runErgoAI.sh .
chmod 700 runErgoAI.sh
if [ "`uname`" = "Darwin" ]; then
    tar -X $EXCLUDEFILE -s ,\(^\.$Ergo_parent\|^\.\),Coherent/$OUTDIR, -cf ergo_dev.tar $files || failure=yes
else
    tar cf ergo_dev.tar --exclude-from=$EXCLUDEFILE $files --transform "s,\\(^\.$Ergo_parent\\|^\\.\\),Coherent/$OUTDIR," || failure=yes
fi

if [ "$failure" = "yes" ]; then
    echo ""
    echo "*** Failed to create ergo_dev.tar"
    echo ""
    exit 1
fi

gzip -f ergo_dev.tar

echo ""
echo "*************************************************************"
echo "**  The archive of ErgoAI is now in ./ergo_dev.tar.gz"
echo "**"
echo "**  Remaining steps (performed automatically):"
echo "**"
echo "**     1.  mv ./ergo_dev.tar.gz /tmp"
echo "**         cd /tmp"
echo "**     2.  /bin/rm -rf Coherent"
echo "**         tar xpzf ./ergo_dev.tar.gz"
echo "**     3.  $Ergo_base/Install/makeself/makeself.sh --notemp Coherent $OUTFILE.run 'Installing ErgoAI' 'cd $OUTDIR; $Ergo_base/ergoAI_config.sh'"
echo "**          mv $OUTFILE.run ."
echo "*************************************************************"

TEMPDIR=/tmp
mv ./ergo_dev.tar.gz $TEMPDIR
curdir=`pwd`
cd $TEMPDIR
# this clears out Coherent in /tmp
/bin/rm -rf Coherent
tar xpzf ./ergo_dev.tar.gz

"$curdir/$Ergo_base2/Install/makeself/makeself.sh" --notemp Coherent $OUTFILE.run 'Installing ErgoAI' "cd $OUTDIR; ErgoAI/ergoAI_config$noninteractive.sh" -devel -v $VERSION
mv $OUTFILE.run $curdir
/bin/rm -rf ./ergo_dev.tar.gz
/bin/rm -rf Coherent
cd $curdir
shasum -a 256 $OUTFILE.run > $OUTFILE.run.sha256.sum
gpg --armor --default-key "Coherent Knowledge" --output $OUTFILE.run.sha256.sig --detach $OUTFILE.run.sha256.sum
rm -f $OUTFILE.zip
zip $OUTFILE.zip $OUTFILE.run $OUTFILE.run.sha256.sum $OUTFILE.run.sha256.sig

echo ""
echo "*************************************************************"
echo "**"
echo "**  Installing from self-extracting archive:"
echo "**"
echo "**         ./$OUTFILE.run"
echo "**"
echo "**  Running ErgoAI:"
echo "**"
echo "**         ./Coherent/$OUTDIR/runErgoAI.sh"
echo "**"
echo "**  Running ErgoAI on command line:"
echo "**"
echo "**         ./Coherent/$OUTDIR/ErgoAI/runergo"
echo "**"
echo "*************************************************************"
echo ""
