
This directory contains files used when building Windows GUI 
installers for Ergo, XSB, and Studio using Inno Setup 5.2 (or a later version):

	http://www.jrsoftware.org/isinfo.php

In order to build the installer, perform the following steps.
The description refers to ergo_dev.iss (for building development releases),
but they equally apply to ergo_customer.iss, for building customer releases.

1. Install (or update if necessary) Inno Setup.

2. Compile Ergo and XSB.

   a. Make sure XSB is compiled for 64-bit Windows.
   
   b. Compile ErgoAI

	  makeergo


3. Copy the "ergo_dev.iss" file onto your Desktop folder and open it using
   Inno Setup.

4. The ergo_dev.iss and ergo_customer.iss scripts assume that

    a. Ergo and XSB are compiled and are available in the folders
	   H:\ERGOAI\ErgoAI
	   H:\ERGOAI\XSB
       respectively.
       
    b. The ergoStudio.jar file is in the folder H:\ERGOAI\

       Make sure you have the latest. If you aren't sure, check the Fidji
       repository on Bitbucket (yes, Bitbucket has old "internal" names
       for repositories) and build it yourself.
       The folder Studio_fidji/interprologForJDK has the file
       build.xml. Copy it and edit that copy (NOT build.xml or build-my.xml)
       to adjust for your installation. There are three places to change:

           <property name="XSB_BIN_DIRECTORY" location=.... />
	   <property name="ERGODIR" location=..... />
	   <property name="bin.dir" location=..... />

       Then run

           ant -f your-copy-of-build.xml

       If you see the message "Build Successful" then all is well:
       the newly built copy of ergoStudio.jar should be in the same
       (interprolog_interprologsvn/interprologForJDK) folder.
       If the build fails, bug Miguel.
          
    c. The file H:\ERGOAI\ErgoAI\Studio_scripts\runErgoAI.exe is copied to

	   H:\ERGOAI\

    You can change the "H:\ERGOAI" bit, if this does not match your system,
    but nothing else. For safety, avoid changing anything in the above
    except, maybe, the drive letter in the above paths. To make these changes,
    edit the "ergo_dev.iss" file and change the definition of the variables
    "ErgoBaseDir" and XSBBaseDir to reflect the directory that you are using
    to hold Ergo and XSB. However, subfolders ErgoAI, XSB, and Studio
    must be named exactly as written.

5. (Rarely)
   Update the Ergo release number in ergo_dev.iss (resp., ergo_customer.iss).
   Also check the Ergo copyright information for possible updates.
   Update the XSB release info.

6. In the Inno Setup Compiler application, run the menu command
   "Build > Compile".

7. Assuming no build errors, the generated installer will be found on
   your Desktop inside the "Output" directory.
