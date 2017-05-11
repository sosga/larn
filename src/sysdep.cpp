/* Copyright 2016 Gibbon aka 'atsb'

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "../includes/larnfunc.h"
#include "../includes/io.h"
#include "../includes/sysdep.h"

#if defined WINDOWS || WINDOWS_VS
	#include <windows.h>
#endif

#if defined NIX
	#include <time.h>
#endif

void nap (int milliseconds)
{
#if defined WINDOWS || WINDOWS_VS
	Sleep ( milliseconds );
#endif
#if defined NIX
	struct timespec tc;
	tc.tv_sec = milliseconds / 1000;
	tc.tv_nsec = ( milliseconds % 1000 ) * 1000000;
	nanosleep ( &tc, NULL );
#endif
}
