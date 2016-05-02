# RL_M
I am the official maintainer of Larn and official maintainer of my XRogue fork (8.0.5 +).

<b>Updates:</b><br\>
Soon some changes will be happening.  I am donating all of my existing computers (with exception to my only Windows machine) to charitable causes.  I will only have a Raspberry Pi 3 as my main computer.  Only ARMv8 GNU/Linux and eventually FreeBSD releases will be done (x86 users will need to compile from source).  Darwin (OS X) is unaffected as this is not my computer.

<b>Code:</b><br \>
Latest SNAPSHOT source code for Larn is in a separate branch (Larn-SNAPSHOT).  There is the full complete code to build releases from, right from my local trunk.  XRogue will have a branch named XRogue-SNAPSHOT (release is when it is ready).

<b>Releases:</b><br \>
I will do a full release for every MAJOR version.  They will be 64bit only except the Windows build, which is a 32bit version.  A full release includes binaries for all my officially supported platforms:

1. GNU/Linux
2. OS X (10.10 +) compatible
3. FreeBSD
4. Windows

These will come with documentation and required files.  These releases will contain major functionality changes and improvements.<br \>

Patch release (MINOR) versions will only get a branch update of the sourcecode with possibly binaries for systems which have had significant modifications, so users can test and report bugs. You can use the source code to build a release yourself for your platform.  These patch releases will have bugfixes and code cleanups only with no major functionality updates.

<b>Bugs:</b><br \>
Raise bugs in the issue tab.  Please tell me your platform, what you were doing and what the bug is.

<b>Building:</b><br \>
On Windows, download TDM-GCC 64bit.  Install it completely into your path and then navigate to the directory where you unpacked the Larn-SNAPSHOT code and do 'mingw32-make -f mingwinrelease.mak'.  PDCurses is included but is compiled only for 64bit.

On GNU/Linux install the ncurses devel package and gcc, make etc.  Then do a 'make -f makefile.nix.debug (or .release)'.

On OSX this is a bit more complicated.  Open a terminal and type gcc.  This will prompt to install the cmdline tools.  Then grab the latest version of ncurses6 and compile it.  Do a make install and then you are ready to make Larn with 'make -f makefile.nix.debug (or .release)'.

On FreeBSD all required dependencies are already installed in the base system.  Simply doing 'make -f makefile.nix.debug (or .release)' will build the binary.

<b>License:</b><br \>
I have found Noah Morgan's original License from 1986 in a usenet archive pertaining to Larn.

See LICENSE.txt in Larn-SNAPSHOT

<b>Contributing:</b><br \>
Feel free to fork, fix a bug or implement a new feature and submit a pull request.  However there are a few things that I respectfully ask of contributors:

1. A good grounding in the C language, code should be clean, correct, portable for all officially supported platforms and not throw warnings on all officially supported platforms (yes your code will be tested before it is merged).
2. Not to radically change gameplay and / or make the game different from it's historic roots.
3. All code submitted and accepted will be under 'Noah's License' as listed in LICENSE.txt in Larn-SNAPSHOT.  By contributing to Larn, you agree to this licensing of your code.  I would love to use a BSD style license, but it is not compatible with Noah's 'not for profit' clause.
4. If fixing or rewriting system-dependent functions, contributors should have a good understanding of Unix-like and Windows specific features and functions.  This is required to keep Larn portable.
