1. branch QT5_CC7 is installed on Centos7 machine with QT5 installed
2. to install QT5 on cc7 machine, use
  `
   yum install qt5-qtbase
   yum install qt5-qtbase-devel
  `
  use `yum whatprovides "*qt5"` to find out the Qt5 package

3. run make under src/ to build the package

4. this package includes two main functions
   - RadiationTest GUI
   - GBE communication using read.exe, send.exe

In case of upgrading Qt package. go to src/ 

   change the Makefile to backupMakefile

   first use `qmake-qt5 -project` to build myproject.pro , then run `qmake-qt5` to generate a Makefile.  The Makefile from qmake could indicate the correct QT5 libraries for compiling

   modify the backupMakefile according to Makefile in compiling setting and change the backupMakefile back to Makefile, then use make to compile
