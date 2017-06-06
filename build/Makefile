SOURCES = $(wildcard ../src/*.cpp) $(wildcard ../src/**/*.cpp)
EXEC = larn-next
CXXFLAGS = -pipe -Wall -Wextra -pedantic -Wpointer-arith -Wshadow -Wmissing-declarations -Wredundant-decls -fomit-frame-pointer -DNIX -DMULTIPLE_SCORE_ENTRY -DTERM_DARK_BACKGROUND
CXX = g++
LIBS = -lm -lncurses

all: compile

compile: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SOURCES) $(LIBS)
	mv $(EXEC) ../bin/
