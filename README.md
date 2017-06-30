
# RL_M
I am the official maintainer of Larn.

![Travis CI logo](TravisCI.png)](https://travis-ci.org)

[![Build Status](https://travis-ci.org/atsb/RL_M.svg?branch=master)](https://travis-ci.org/atsb/RL_M)

<b>Master Branch</b></br>
Currently a conversion of the C code of Larn to C++ is underway, in order to de-spaghettifi the
code and make it a good, clean game with a great codebase.

NOTE: A change in accepting commits.  All pull-requests that deal with code changes
will be placed under the Apache 2.0 License.  See [docs/LICENSE.txt](https://github.com/atsb/RL_M/blob/master/docs/LICENSE.txt) or bug #61 for more info.

<b>Future Planned Changes</b>
* Make it harder.
 
* Making the beginning of the game easier (better stats) and making the middle to end-game harder (tougher enemies).
  It's not as balanced as I'd like and I think Char Levels 1 - 7 would make a nice 'tutorial' area for beginners.
  Similar to MMO's.  Right now you're dropped in at the deep-end and if you're not careful even a kobold can kill you.
  Advanced players can still use the difficulty settings (which will be balanced also).
  
* Put common functions into classes.  Reusable print functions will be put into C++ classes and other
well-used functions.

* Increase the use of Religion.  Currently it feels incomplete, I'll be fleshing out the Religon a little more, adding things
that I feel will compliment it's use in the game (like small statues, candles and prayer books).  Perhaps even using a combination of them on a Statue/Altar will give some benefits.

* Hunger & Thirst, you will need food to replenish your hunger and water to keep you hydrated.  This won't be like other games
which take this to the extreme and end-up damaging the game.  When you start you will have enough food and water for the majority of the game (what adventurer would not pack for a trip?).  Every 1000 steps a hunger notch will dissappear (total of 10), eating food (any food) will replenish 50% of the bar.  Nice and simple and not gameplay damaging.

* More portable and more binaries.

# Compile Warnings #
Due to the GNU/Linux distro I use as my primary machine, the officially supported compiler is GCC 7 (minimum).  If Larn throws warnings on earlier versions, I won't fix it, simply because I cannot verify it myself, my distro is now on GCC 7 and that is what will be used for future builds, I don't like staying with ancient versions of packages ;)

# Compiling from Source #
Compiling Larn from source will require GCC 7 as a minimum due to new inlined variables and [[fallthrough]] which is now used.  I realize this may p!!s some people off but the inline variables are important as they function almost like the old #define's did.  Since I am rewriting this so it is clean and up to date, this is the consequence.  If we all held back from being modern we'd still be using 8080 cpu's.
