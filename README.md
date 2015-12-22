# RL_M
I am the official maintainer of Larn (12.5 +) and official maintainer of my XRogue fork (8.0.5 +).

Code:
Latest SNAPSHOT source code for Larn is in a separate branch (Larn-SNAPSHOT).  There is the full complete code to build releases from, right from my local trunk.  XRogue will have a branch named XRogue-SNAPSHOT (release is when it is ready).

Releases:
I will do a full release for every MAJOR version.  A full release includes OSX, GNU/Linux and Windows binaries with documentation and required files.  These releases will contain major functionality changes and improvements.

Patch release (MINOR) versions will only get a branch update of the sourcecode.  Which you can use to build a release yourself for your platform.  These patch releases will have bugfixes only with no major functionality updates.

Bugs:
Raise bugs in the issue tab.  Please tell me your platform, what you were doing and what the bug is.

Building:
On Windows, download TDM-GCC 64bit.  Install it completely into your path and then navigate to the directory where you unpacked the Larn-SNAPSHOT code and do 'mingw32-make -f mingwinrelease.mak'.  PDCurses is included but is compiled only for 64bit.

On GNU/Linux install the ncurses devel package and gcc, make etc.  Then do a 'make -f gnulinuxrelease.mak'.

On OSX this is a bit more complicated.  Open a terminal and type gcc.  This will prompt to install the cmdline tools.  Then grab the latest version of ncurses6 and compile it.  Do a make install and then you are ready to make Larn with 'make -f darwinrelease.mak'.
