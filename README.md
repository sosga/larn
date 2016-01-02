# RL_M
I am the official maintainer of Larn (12.5 +) and official maintainer of my XRogue fork (8.0.5 +).

<b>Code:</b><br \>
Latest SNAPSHOT source code for Larn is in a separate branch (Larn-SNAPSHOT).  There is the full complete code to build releases from, right from my local trunk.  XRogue will have a branch named XRogue-SNAPSHOT (release is when it is ready).

<b>Releases:</b><br \>
I will do a full release for every MAJOR version.  A full release includes OSX, GNU/Linux and Windows binaries with documentation and required files.  These releases will contain major functionality changes and improvements.<br \>

Patch release (MINOR) versions will only get a branch update of the sourcecode with possibly binaries for systems which have had significant modifications, so users can test and report bugs. You can use the source code to build a release yourself for your platform.  These patch releases will have bugfixes and code cleanups only with no major functionality updates.

<b>Bugs:</b><br \>
Raise bugs in the issue tab.  Please tell me your platform, what you were doing and what the bug is.

<b>Building:</b><br \>
On Windows, download TDM-GCC 64bit.  Install it completely into your path and then navigate to the directory where you unpacked the Larn-SNAPSHOT code and do 'mingw32-make -f mingwinrelease.mak'.  PDCurses is included but is compiled only for 64bit.<br \>
On GNU/Linux install the ncurses devel package and gcc, make etc.  Then do a 'make -f gnulinuxrelease.mak'.<br \>

On OSX this is a bit more complicated.  Open a terminal and type gcc.  This will prompt to install the cmdline tools.  Then grab the latest version of ncurses6 and compile it.  Do a make install and then you are ready to make Larn with 'make -f darwinrelease.mak'.

<b>License:</b><br \>
I have found Noah Morgan's original License from 1986 in a usenet archive pertaining to Larn.

See LICENSE.txt in Larn-SNAPSHOT

<b>Contributing:</b><br \>
Feel free to fork, fix a bug or implement a new feature and submit a pull request.  However I respectfully ask contributors to:

1. A good grounding in the C language, code should be clean, correct, and not throw warnings.
2. Not to radically change gameplay and / or make the game different from it's historic roots.
3. All code submitted and accepted will be under 'Noah's License' as listed in LICENSE.txt in Larn-SNAPSHOT.  By contributing to Larn, you agree to this licensing of your code.
4. If fixing or rewriting system-dependent functions, contributors should have a good understanding of Unix-like and Windows specific features and functions.  This is required to keep Larn portable.
