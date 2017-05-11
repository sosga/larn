g++ -pipe -Wall -Werror -fomit-frame-pointer -DNIX -DMULTIPLE_SCORE_ENTRY -DTERM_DARK_BACKGROUND ../src/*.cpp -lm -lncurses
mv a.out larn_101r
mv larn_101r ../bin/
