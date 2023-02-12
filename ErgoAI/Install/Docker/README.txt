I. INSTALLATION

   Linux: just use the package manager to install the Docker version that
          came with your system.

   MacOS: 
     1. Download Docker and install it as explained in
        https://hub.docker.com/editions/community/docker-ce-desktop-mac/
     2. You also need an X server called XQuartz:
        https://techsparx.com/software-development/docker/display-x11-apps.html
        Pay attention to the instructions, especially to the fact that you
        need to open XQuartz, access the Security tab in the menu
        XQuartz > X11 Preferences  and check both boxes there.
     3. Reboot.

    Windows:
      1. Download Docker and install it as explained in
         https://docs.docker.com/docker-for-windows/install/   
      2. Start Docker by right-clicking on its icon AND selecting
         "Run as Administrator."
      3. You also need an X server called VcXrv:
         https://sourceforge.net/projects/vcxsrv/


II. IMPORTING ERGOAI IMAGE INTO DOCKER

To install an Ergo image into Docker:

Suppose the image file is

        ergoai-ubuntu-gui.tar

Here ubuntu means
that the image contains an Ubuntu virtual machine; and gui means that this
image supports ErgoAI GUI. Other possibilities could be (if you have those
images) ergoai_ubuntu.tar, ergoai_centos.tar, etc.

Then in Linux, Mac OS, and Windows do:

1. Make sure Docker is running. In Linux it'll start running right after
   the install. In Mac and Windows, it has to be started manually, as
   explained earlier.

2. Import the image as follows:

          sudo docker load -i ergoai-ubuntu-gui.tar

    To run this command, your account must have sudo privileges and you
    will be asked to enter your password.

    Make sure the last argument here matches the file name of the Docker
    image you've got.

    In Windows:

           docker load -i ergoai-ubuntu-gui.tar

III. RUNNING THE ERGOAI IMAGE

1. In Mac OS, make sure XQuartz X11 server is running. It will probably be
   running after the install and reboot, but do check.

2. In Linux and MacOS, execute:

            run-docker-ergoai.sh -ergo-version -gui

    In Windows:
    
            run-docker-ergoai.bat -ergo-version -gui

    To run this command in Linus and MacOS, your account must have 
    sudo privileges and you will be asked to enter your password most of
    the time.

    In Windows, this command must be run as Administrator.
