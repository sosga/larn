g++ -pipe -Wall -Wextra -pedantic -Wpointer-arith -Wshadow -Wmissing-declarations -Wredundant-decls -g -DNIX -DMULTIPLE_SCORE_ENTRY -DTERM_DARK_BACKGROUND ../src/*.cpp -lm -lncurses
mv a.out larn_101d
mv larn_101d ../bin/
