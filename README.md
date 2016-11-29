<b>Updates:</b><br\>
Don't raise bugs for this branch.  It's right now purely alpha experimentation and won't be stable for a long long time.

~Gibbon

<b>Future Planned Changes</b><br\>
* Make it harder.  I find Larn to be very easy, in comparison to other games in the genre.
  I also dislike cheaters with the save files, so soon, I am removing the save/load code from the game.
  If you want to complete the game, you can do it in a single sitting (it's only 1 - 2 hours long at best).
 
* Making the beginning of the game easier (better stats) and making the middle to end-game harder (tougher enemies).
  It's not as balanced as I'd like and I think Char Levels 1 - 7 would make a nice 'tutorial' area for beginners.
  Similar to MMO's.  Right now you're dropped in at the deep-end and if you're not careful even a kobold can kill you.
  Advanced players can still use the difficulty settings (which will be balanced also).
  
* Templatize common functions.  Reusable print functions and color functions will be Templatized into C++ templates.
  I dislike heavy template use but for these areas it would make sense, they are used throughout the code and it would
  make the code a bit more readable.

* Builds will be done on 3 systems.  Windows(x64), macOS and FreeBSD(aarch64 - rpi3).  It's 2016, I won't support 32bit in this branch.  Compile it yourselves if you have an outdated architecture / OS.  No Linux builds as my only Linux machine broke.  Naturally I cannot compile on OS's which I don't have :)

* Hunger & Thirst, you will need food to replenish your hunger and water to keep you hydrated.  This won't be like other games which take this to the extreme and end-up damaging the game.  When you start you will have enough food and water for the majority of the game (what adventurer would not pack for a trip?).  Every 1000 steps a hunger notch will dissappear (total of 10), eating food (any food) will replenish 50% of the bar.  Nice and simple and not gameplay damaging.

* Source code reformatting and cleaning.  This will take priority over any new features (until this is finished).  I want a clean slate and not a mess.  BSD formatting will be used.
