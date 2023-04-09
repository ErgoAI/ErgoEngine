#!/bin/sh 

# Create an XSB/ErgoAI/Studio installer 

# Run this script as
#  ErgoEngine/ErgoAI/Install/ergo_customer.sh [-v Version] [nointeraction]
# in a folder that has ./XSB, and ./ErgoEngine/ErgoAI/

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
OUTDIR=ERGOAI_$VERSION
outfile_suffix=_$VERSION$noninteractive
# $OUTFILE is the name of the file xxxx.run
OUTFILE=ergoAI$outfile_suffix
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
        ./XSB/etc/ \
        ./XSB/packages \
        ./XSB/installer \
        ./XSB/InstallXSB.jar \
        ./XSB/examples \
    	$Ergo_base/runflora $Ergo_base/runergo \
        $Ergo_base/ergo_sanity_check.sh \
    	$Ergo_base/etc/*.png $Ergo_base/etc/*.eps $Ergo_base/etc/*.ico $Ergo_base/etc/*.icns \
    	$Ergo_base/opt/*.xwam \
    	$Ergo_base/opt/optstructures.P \
        $Ergo_base/*.sh \
        $Ergo_base/rlwrap_keywords.txt \
        $Ergo_base/Install/LICENSE_ergo \
        $Ergo_base/Install/*-desktop* \
        $Ergo_base/Install/uninstall_ergoAI.sh \
        $Ergo_base/Install/MacOS \
        $Ergo_base/Tools/jena/ErgoOWL/bin/*.sh \
        $Ergo_base/Tools/jena/ErgoSPARQL/bin/*.sh \
        $Ergo_base/Tools/jena/*.jar \
        $Ergo_base/*.H $Ergo_base/*.xwam $Ergo_base/*.flh\
        $Ergo_base/AT/*.flr \
        $Ergo_base/AT/prolog/*.P  $Ergo_base/AT/include/*.flh  \
        $Ergo_base/ATinc/*.flh  \
    	$Ergo_base/closure/*.fli \
    	$Ergo_base/closure/*.flh \
    	$Ergo_base/closure/*.inc \
    	$Ergo_base/datatypes/*.xwam \
    	$Ergo_base/debugger/*.in $Ergo_base/debugger/*.P $Ergo_base/debugger/*.xwam \
    	$Ergo_base/debugger/*.dat \
    	$Ergo_base/demos/*.flr \
	$Ergo_base/demos/*.flh \
    	$Ergo_base/demos/.ergo_aux_files \
    	$Ergo_base/demos/sgml/*.flr $Ergo_base/demos/sgml/expectedoutput \
    	$Ergo_base/demos/xpath/*.flr $Ergo_base/demos/xpath/expectedoutput \
    	$Ergo_base/emacs/ergo.el* \
    	$Ergo_base/emacs/README \
    	$Ergo_base/flrincludes/*.flh \
    	$Ergo_base/genincludes/*.flh \
    	$Ergo_base/headerinc/*.flh \
    	$Ergo_base/includes/*.flh \
    	$Ergo_base/includes/*.fli \
    	$Ergo_base/lib/*.flr \
    	$Ergo_base/lib/.ergo_aux_files \
	$Ergo_base/lib/include/*flh  $Ergo_base/lib/include/*flr \
    	$Ergo_base/libinc/*.flh \
    	$Ergo_base/ergo_libinc/*.flh \
    	$Ergo_base/cc/prolog2hilog.* $Ergo_base/cc/*.P \
    	$Ergo_base/cc/flora_ground.* \
    	$Ergo_base/cc/flrcc_init.P \
    	$Ergo_base/cc/flora_cc_prefix.h \
    	$Ergo_base/cc/C_calling_Ergo/* \
        $Ergo_base/pkgs/*.flr $Ergo_base/pkgs/prolog/*.P \
	$Ergo_base/pkgs/include/*.flh \
        $Ergo_base/pkgs/.ergo_aux_files \
    	$Ergo_base/pkgsinc/*.flh \
	$Ergo_base/ergo_pkgs/.ergo_aux_files \
	$Ergo_base/ergo_pkgs/evidential_probability/*.xwam \
	$Ergo_base/ergo_pkgs/e2dsv/*.xwam \
	$Ergo_base/ergo_lib/.ergo_aux_files \
	$Ergo_base/ergo_lib/ergo2sparql/*.xwam \
	$Ergo_base/ergo_lib/ergo2sparql/*.txt \
	$Ergo_base/ergo_lib/ergo2sparql/java/*.jar \
	$Ergo_base/ergo_lib/ergo2java/*.xwam \
	$Ergo_base/ergo_lib/ergo2java/java/*.jar \
	$Ergo_base/ergo_lib/ergo2json/*.xwam \
	$Ergo_base/ergo_lib/ergo2owl/*.xwam \
	$Ergo_base/ergo_lib/ergo2owl/*.txt \
	$Ergo_base/ergo_lib/ergo2owl/java/*.jar \
	$Ergo_base/ergo_lib/ergo_explain/*.xwam \
	$Ergo_base/ergo_demos/* \
	$Ergo_base/python/*.py \
	$Ergo_base/python/*.sh \
	$Ergo_base/python/pyxsb/*.py \
	$Ergo_base/python/pyergo/*.py \
        $Ergo_base/syslib/*.xwam \
    	$Ergo_base/syslibinc/*.flh  \
        $Ergo_base/ergo_syslib/*.xwam \
        $Ergo_base/java \
        ./runErgoAI.sh "

## excluded doc files
#        ./XSB/docs/userman/manual?.pdf \
#        $Ergo_base/docs/*.pdf \

if [ -d $Ergo_base -a -d ./XSB ]; then
    flrdir=$currdir/$Ergo_base2
    xsbdir=$currdir/XSB
else
    echo "This script must be run as $Ergo_base/Install/ergo_customer.sh"
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
bundle_config.sh
flrcompiler.[PH]
flrparser.[PH]
flrcomposer.[PH]
flrshell.[PH]
flrlibman.[PH]
flora2.[PH]
EOF

cp $Ergo_base/Studio_scripts/runErgoAI.sh .
chmod 700 runErgoAI.sh
if [ "`uname`" = "Darwin" ]; then
    tar -X $EXCLUDEFILE -s ,\(^\.$Ergo_parent\|^\.\),Coherent/$OUTDIR, -cf ergo_cust.tar $files || failure=yes
else
    tar cf ergo_cust.tar --exclude-from=$EXCLUDEFILE $files --transform "s,\\(^\.$Ergo_parent\\|^\\.\\),Coherent/$OUTDIR," || failure=yes
fi

if [ "$failure" = "yes" ]; then
    echo ""
    echo "*** Failed to create ergo_cust.tar"
    echo ""
    exit 1
fi

gzip -f ergo_cust.tar

echo ""
echo "*************************************************************"
echo "**  The archive of ErgoAI is now in ./ergo_cust.tar.gz"
echo "**"
echo "**  Remaining steps (performed automatically):"
echo "**"
echo "**     1.  mv ./ergo_cust.tar.gz /tmp"
echo "**         cd /tmp"
echo "**     2.  /bin/rm -rf Coherent"
echo "**         tar xpzf ./ergo_cust.tar.gz"
echo "**     3.  $Ergo_base/Install/makeself/makeself.sh --notemp Coherent $OUTFILE.run 'Installing ErgoAI' 'cd $OUTDIR; $Ergo_base/ergoAI_config.sh'"
echo "**          mv $OUTFILE.run ."
echo "*************************************************************"

TEMPDIR=/tmp
mv ./ergo_cust.tar.gz $TEMPDIR
curdir=`pwd`
cd $TEMPDIR
# this clears out Coherent in /tmp
/bin/rm -rf Coherent
tar xpzf ./ergo_cust.tar.gz

"$curdir/$Ergo_base2/Install/makeself/makeself.sh" --notemp Coherent $OUTFILE.run 'Installing ErgoAI' "cd $OUTDIR; ErgoAI/ergoAI_config$noninteractive.sh" -v $VERSION
mv $OUTFILE.run $curdir
/bin/rm -rf ./ergo_cust.tar.gz
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
