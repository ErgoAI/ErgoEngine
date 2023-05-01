#!/bin/sh 

# This is the post-extraction script run by makeself.
# It configures the extracted ErgoAI files.

# This assumes that XSB is sitting in ./XSB

echo
echo "+++++ Installing ErgoAI -- will take a few minutes"
echo

currdir="`pwd`"
echo "----- Current directory = $currdir"
echo

if [ "$1" = "-devel" ] ; then
    devel_version=yes
    devel_icon_mark="-dev"
    devel_icon_mark2=" (dev)"
    shift
fi

if [ "$1" = "-v" ] ; then
    VERSION=$2
    version_icon_mark="-$VERSION"
    version_icon_mark2=" $VERSION"
    shift
    shift
fi

if [ -d ./ErgoAI -a -d ./XSB ]; then
    flrdir="$currdir/ErgoAI"
    xsbdir="$currdir/XSB"
    tmpxsbdir=/tmp/XSB-`date +"%y-%m-%d-%H_%M_%S"`
    rm -rf $tmpxsbdir || \
	(echo "***** You have no write permission for the /tmp folder: your system is misconfigured"; echo "***** Installation has failed";  exit 1)
else
    echo "***** This script is to be run in a folder that contains ./XSB & ./ErgoAI"
    exit 1
fi

java_version=`java -version 2>&1`
java_version=`echo $java_version | sed 's/JDK.*//'`
java_version=`echo $java_version`   # removes newlines?
java_version_sans_old=`echo $java_version | sed 's/1\.[0-7]//'`

# if called without an argument or just with argument -devel -- ask questions.
# if called with an argument do non-interactive install for Docker, don't ask Qs
if [ "$1" = "" ]; then
    cat ErgoAI/Install/LICENSE_ergo
    echo
    # use /bin/echo, as Mac's shell builtin "echo" does not grok -n
    /bin/echo -n "I accept (y/N): "
    read response
    echo
    if [ "$response" != "Y" -a "$response" != "y" -a "$response" != "Yes" -a "$response" != "yes" -a "$response" != "YES" ]; then
	echo You must accept the terms of the above user license agreements
	echo Exiting ...
	exit 1
    fi

    if [ "$java_version" != "$java_version_sans_old" ]; then
        echo "*** Warning: the installed version of Java is too old!"
        echo
        echo "Please note: to use ErgoAI Studio and the OWL/RDF/SPARQL tools,"
        echo "Java 8 (JRE 1.8) or later must be installed. However, Java"
        echo "is not required to just run the Ergo reasoner on command line,"
        echo "with no GUI or the aforesaid features."
        echo
        /bin/echo -n "Yes, I understand ... "
        read response
        echo
    fi
fi

echo "+++++ Removing old files"
/bin/rm -rf ./XSB/config/*
/bin/rm -rf ./*.app

# start recording uninstall info
rm -f "$currdir/.uninstall_info.data"
echo "base_install_dir=\"$currdir\"" > "$currdir/.uninstall_info.data"

cp "$currdir/ErgoAI/Install/MacOS/mk-mac-alias" "$currdir"
cp "$currdir/ErgoAI/Install/MacOS/del-mac-alias" "$currdir"
cp "$currdir/ErgoAI/Install/uninstall_ergoAI.sh" "$currdir"
chmod 700 "$currdir/mk-mac-alias" "$currdir/del-mac-alias" "$currdir/uninstall_ergoAI.sh"

# Move XSB to /tmp to sidestep the problems with configuring it
# in dirs that have spaces
mv -f "$xsbdir" $tmpxsbdir
cd $tmpxsbdir/build
rm -f "$currdir/ergo-install.log"

echo "+++++ Configuring XSB"
echo
echo "+++++ Configuring XSB" > "$currdir/ergo-install.log"
echo "----- Current directory = $currdir" >> "$currdir/ergo-install.log"
echo "" >> "$currdir/ergo-install.log"

./configure --with-dbdrivers >> "$currdir/ergo-install.log" 2>&1 || \
    (echo :ERRORS:FOUND: >> "$currdir/ergo-install.log"; echo "***** Configuration of XSB failed: see $currdir/ergo-install.log"; exit 1)

grep "configure: error" "$currdir/ergo-install.log" > "$currdir/ergo-misc.log"
grep "Python integration" "$currdir/ergo-install.log" >> "$currdir/ergo-misc.log"

misc_errors=`cat "$currdir/ergo-misc.log"`
if [ -n "$misc_errors" ]; then
    echo
    cat "$currdir/ergo-misc.log"
    echo
fi

echo "+++++ Compiling XSB"
./makexsb >> "$currdir/ergo-install.log" 2>&1 || \
    (echo :ERRORS:FOUND: >> "$currdir/ergo-install.log"; echo "***** Compilation of XSB failed: see $currdir/ergo-install.log"; exit 1)

# Move compiled XSB from /tmp to its intended place
# Splitting mv into cp+rm because of what seems to be a bug in Ubuntu over W10
cp -rf $tmpxsbdir "$xsbdir"
rm -rf $tmpxsbdir

cd "$flrdir"
echo "+++++ Configuring ErgoAI"
echo "+++++ Configuring ErgoAI" >> "$currdir/ergo-install.log"
./ergo_sanity_check.sh "$xsbdir/bin/xsb" >> "$currdir/ergo-install.log" 2>&1 || \
    (echo :ERRORS:FOUND: >> "$currdir/ergo-install.log"; echo "***** Configuration of ErgoAI failed: see $currdir/ergo-install.log"; exit 1)

# setting up the icons
# LINUX
echo "+++++ Setting up icons"
echo "+++++ Setting up icons" >> "$currdir/ergo-install.log"


# key uninstall vars
reasoner_desktop_shortcut="$HOME/Desktop/ErgoReasoner$version_icon_mark$devel_icon_mark.desktop"
studio_desktop_shortcut="$HOME/Desktop/ErgoAI$version_icon_mark$devel_icon_mark.desktop"
reasoner_desktop_shortcut_name="ErgoAI Reasoner $VERSION$devel_icon_mark2"
studio_desktop_shortcut_name="ErgoAI Studio $VERSION$devel_icon_mark2"

if [ "`uname`" = "Linux" -a -d $HOME/Desktop ]; then
    cat "$currdir/ErgoAI/Install/ErgoReasoner$devel_icon_mark-linux-desktop" | sed "s|ERGO_BASE_FOLDER|$currdir|" | sed "s|ERGO_VERSION_NUMBER|$VERSION|" > $reasoner_desktop_shortcut
    cat "$currdir/ErgoAI/Install/ErgoAI$devel_icon_mark-linux-desktop" | sed "s|ERGO_BASE_FOLDER|$currdir|" | sed "s|ERGO_VERSION_NUMBER|$VERSION|" > $studio_desktop_shortcut
    chmod u+x $studio_desktop_shortcut $reasoner_desktop_shortcut
fi

# continue recording uninstall info
echo "reasoner_desktop_shortcut=\"$reasoner_desktop_shortcut\"" >> "$currdir/.uninstall_info.data"
echo "studio_desktop_shortcut=\"$studio_desktop_shortcut\"" >> "$currdir/.uninstall_info.data"

# MAC
if [ "`uname`" = "Darwin" -a -d $HOME/Desktop ]; then
    ergoAI_studio_app_dir="$currdir/runErgoAI$devel_icon_mark.app"
    ergoAI_reasoner_app_dir="$currdir/runErgoReasoner$devel_icon_mark.app"

    if [ "`which Rez`" = "" ]; then
        echo
        echo
        echo "!!! Mac Developer Tools (XCode) do not seem to be installed."
        echo "!!! As a result, desktop icons might not get installed"
        echo "!!! and some ErgoAI packages might be unavailable."
        echo "!!! Make sure you install XCode."
        echo
        /bin/echo -n I understand...
        read response
        echo
    fi

    /bin/rm -f "$HOME/Desktop/ErgoAI Reasoner"
    /bin/rm -f "$HOME/Desktop/ErgoAI Reasoner (dev)"
    /bin/rm -f "$HOME/Desktop/ErgoAI Studio"
    /bin/rm -f "$HOME/Desktop/ErgoAI Studio (dev)"

    # Step 1:  set up runErgoAI.app and its desktop shortcut
    cp -r "$currdir/ErgoAI/Install/MacOS/runErgoAI$devel_icon_mark.app" "$currdir"
    cat "$ergoAI_studio_app_dir/Contents/MacOS/runErgoAI.template" | sed "s|ERGO_BASE_FOLDER|$currdir|" > "$ergoAI_studio_app_dir/Contents/MacOS/runErgoAI"
    cat "$ergoAI_studio_app_dir/Contents/Info.plist.template" | sed "s|ERGO_VERSION_NUMBER|$VERSION|" > "$ergoAI_studio_app_dir/Contents/Info.plist"
    chmod u+x "$ergoAI_studio_app_dir/Contents/MacOS/runErgoAI"

    # make desktop alias for the Studio
    echo "Running del-mac-alias \"$studio_desktop_shortcut_name\"" >> "$currdir/ergo-install.log"
    "$currdir/del-mac-alias" "$studio_desktop_shortcut_name" >> "$currdir/ergo-install.log" 2>&1
    echo "Copying the Studio icon \"$currdir/ErgoAI/etc/ergoAI-desktop-studio.icns\" to the Mac app Resources folder \"$ergoAI_studio_app_dir/Contents/Resources/\"" >> "$currdir/ergo-install.log"
    cp "$currdir/ErgoAI/etc/ergoAI-desktop-studio.icns" "$ergoAI_studio_app_dir/Contents/Resources/"
    echo "Running mk-mac-alias $ergoAI_studio_app_dir \"$studio_desktop_shortcut_name\"" >> "$currdir/ergo-install.log"
    "$currdir/mk-mac-alias" "$ergoAI_studio_app_dir" "$studio_desktop_shortcut_name" >> "$currdir/ergo-install.log" 2>&1

    # Step 2: set up the Reasoner app and its desktop shortcut
    cp -r "$currdir/ErgoAI/Install/MacOS/runErgoReasoner$devel_icon_mark.app" "$currdir"
    cat "$ergoAI_reasoner_app_dir/Contents/MacOS/runErgoReasoner.template" | sed "s|ERGO_BASE_FOLDER|$currdir|" > "$ergoAI_reasoner_app_dir/Contents/MacOS/runErgoReasoner"
    cat "$ergoAI_reasoner_app_dir/Contents/Info.plist.template" | sed "s|ERGO_VERSION_NUMBER|$VERSION|" > "$ergoAI_reasoner_app_dir/Contents/Info.plist"
    chmod u+x "$ergoAI_reasoner_app_dir/Contents/MacOS/runErgoReasoner"

    # make desktop alias for the Reasoner
    echo "Running del-mac-alias \"$reasoner_desktop_shortcut_name\"" >> "$currdir/ergo-install.log"
    "$currdir/del-mac-alias" "$reasoner_desktop_shortcut_name" >> "$currdir/ergo-install.log" 2>&1
    echo "Copying the Reasoner icon \"$currdir/ErgoAI/etc/ergoAI-desktop-reasoner.icns\" to the Mac app Resources folder \"$ergoAI_reasoner_app_dir/Contents/Resources/\"" >> "$currdir/ergo-install.log"
    cp "$currdir/ErgoAI/etc/ergoAI-desktop-reasoner.icns" "$ergoAI_reasoner_app_dir/Contents/Resources/"
    echo "Running mk-mac-alias $ergoAI_reasoner_app_dir \"$reasoner_desktop_shortcut_name\"" >> "$currdir/ergo-install.log"
    "$currdir/mk-mac-alias" "$ergoAI_reasoner_app_dir" "$reasoner_desktop_shortcut_name" >> "$currdir/ergo-install.log" 2>&1
fi

# continue recording uninstall info
echo "studio_desktop_shortcut_name=\"$studio_desktop_shortcut_name\"" >> "$currdir/.uninstall_info.data"
echo "reasoner_desktop_shortcut_name=\"$reasoner_desktop_shortcut_name\"" >> "$currdir/.uninstall_info.data"

install_err_found=`cat "$currdir/ergo-install.log" | grep :ERRORS:FOUND: `

if [ -z "$install_err_found" ]; then
    echo "+++++ Running ErgoAI for the first time"
    echo "+++++ Running ErgoAI for the first time" >> "$currdir/ergo-install.log"
    "$currdir/ErgoAI/runergo" > "$currdir/ergo-initrun.log" 2>&1 <<EOF
\halt.
EOF
fi

touch "$currdir/ErgoAI/lib/.ergo_aux_files/*" >> "$currdir/ergo-initrun.log" 2>&1
touch "$currdir/ErgoAI/pkgs/.ergo_aux_files/*" >> "$currdir/ergo-initrun.log" 2>&1
touch "$currdir/ErgoAI/demos/.ergo_aux_files/*" >> "$currdir/ergo-initrun.log" 2>&1

cat "$currdir/ergo-initrun.log"
cat "$currdir/ergo-initrun.log" >> "$currdir/ergo-install.log"

echo
echo "..... The build log is in \"$currdir/ergo-install.log\""
echo "..... Attach it if filing an installation problem report"
echo

initrun_err_found=`cat "$currdir/ergo-initrun.log" | grep Error`
initrun_abort_found=`cat "$currdir/ergo-initrun.log" | grep Abort `
install_err_found=`cat "$currdir/ergo-install.log" | grep :ERRORS:FOUND: `

if [ -z "$initrun_err_found" -a -z "$initrun_abort_found" -a -z "$install_err_found" ]; then
    echo "+++++ All is well: you can run ErgoAI in terminal mode via the script"
    echo "+++++    \"$currdir/ErgoAI/runergo\""
    echo "+++++ and with the Studio via"
    echo "+++++    \"$currdir/runErgoAI.sh\""
else
    echo "***** ERRORS occurred during installation of ErgoAI"
    echo "***** ERRORS occurred during installation of ErgoAI" >> "$currdir/ergo-install.log"
    echo
fi

echo
#echo "+++++ If desktop icons 'ErgoAI Reasoner' and 'ErgoAI Studio' were installed"
#echo "+++++ successfully, one can conveniently use them to run ErgoAI."
#if [ "`uname`" = "Darwin" -a -d $HOME/Desktop ]; then
#   echo "+++++ On the Mac, one might need to:"
#   echo "+++++   sudo /bin/rm -rf /Library/Cashes/com.apple.iconservices.store"
#   echo "+++++ and then reboot to ensure that ErgoAI icons are displayed."
#fi
#echo

