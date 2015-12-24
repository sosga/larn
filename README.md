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

License:
Larn was originally released as open source software and was encouraged to be shared, improved and played.  The 80's never required a concrete license like today and so many of the contributors have never never any details about which license their existing contributions are under.

As maintainer, I want to change this.  Next year I will start to email and track down as many living contributors as possible in a bid to free the majority of the code (yes it isn't really FOSS).  Yesterday in my local trunk I started the process of rewriting Fred Fish's contribution to the termcap routines by modifying a public domain implementation.  This is one of the main blockers because the tgoto code is not free software and it has to go.

I have already scanned the usenet archives and gathered quite a few names.  I even came across a contributor (Alexander Perry) who has released his changes under a BSD-style c clause license for DFSG (Debian Free Software Guidelines) in 2005.

Any help in tracking down and getting contributors to release under the following license would be greatly appreciated.  Let's make Larn free software!  I put this here as it is noticeable.  This will at a later date go into a seperate LICENSE.txt file.

People we need to locate and contact:
James McNamara UNIX install notes, source and patch distribution
Don Kneller Port for DECRainbow and PC compatible
Daniel Kegel Enhanced ansi terminal decoding for DOS
Kevin Routley Integration, enhancement and release

Debian/Linux/GCC porting contributions are under modified BSD:
Copyright (c) 2005 Alexander Perry <alex.perry@ieee.org>
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder(s) nor the names of their
contributors may be used to endorse or promote products derived
from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

All past, present and future bug fixes, cleanups, code refactoring and new feature implementation Contributions are under modified BSD:
Copyright (c) 2015 ATSB (Gibbon) <contact on github>
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder(s) nor the names of their
contributors may be used to endorse or promote products derived
from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
