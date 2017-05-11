<b>Larn-NEXT</b></br>
Larn-NEXT is a conversion of the C code of vanilla Larn to C++ (think C with classes style) in order to de-spaghettifi the
code and make it a good, clean game with a great codebase.

NOTE: A change in accepting commits.  All pull-requests that deal with code changes
will be placed under the Apache 2.0 License.  See the 'LICENSE.txt' or bug #61 for more info.

<b>Binaries</b></br>
Binaries are supported in 3 'classes'

* Class 1 binaries: x86, x64 and all MIPS types<i>(thanks to Imagination for the amazing support they give to their CPU's).</i>
* Class 2 binaries: ppc,armel,armhf.
* Class 3 binaries: sparc64 (when it's available).

Different classes have different priority.  So in all builds, class 1 will always be built, class 2 will be built later
and class 3 will only be built if time allows (I'm quite a busy person).  So far they are all built on GNU/Linux but I am
planning also to build on GNU Hurd and a BSD for x86 compatible architectures.</br>

<b>Future Planned Changes</b>
* Make it harder.  I find Larn to be very easy, in comparison to other games in the genre.
  I also dislike cheaters with the save files, so soon, I am removing the save/load code from the game.
  If you want to complete the game, you can do it in a single sitting (it's only 1 - 2 hours long at best).
  
  I may be putting it back in, I'll do some testing and see.  I know some people wouldn't want to sit through a whole setting
  back-back so maybe it's a little overkill..  still a re-write of the code for C++ would be needed.
 
* Making the beginning of the game easier (better stats) and making the middle to end-game harder (tougher enemies).
  It's not as balanced as I'd like and I think Char Levels 1 - 7 would make a nice 'tutorial' area for beginners.
  Similar to MMO's.  Right now you're dropped in at the deep-end and if you're not careful even a kobold can kill you.
  Advanced players can still use the difficulty settings (which will be balanced also).
  
* Put common functions into classes.  Reusable print functions and color functions will be put into C++ classes and other
well-used functions.

* Increase the use of Religion.  Currently it feels incomplete, I'll be fleshing out the Religon a little more, adding things
that I feel will compliment it's use in the game (like small statues, candles and prayer books).  Perhaps even using a combination of them on a Statue/Altar will give some benefits.

* Hunger & Thirst, you will need food to replenish your hunger and water to keep you hydrated.  This won't be like other games
which take this to the extreme and end-up damaging the game.  When you start you will have enough food and water for the majority of the game (what adventurer would not pack for a trip?).  Every 1000 steps a hunger notch will dissappear (total of 10), eating food (any food) will replenish 50% of the bar.  Nice and simple and not gameplay damaging.

* More portable and more binaries.
