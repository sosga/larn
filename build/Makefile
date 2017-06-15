SOURCES = $(wildcard ../src/*.cpp) $(wildcard ../src/**/*.cpp)
EXEC = larn-next
CXXFLAGS = -pipe -O2 -Wall -Wextra -pedantic -Wpointer-arith -Wshadow -Wmissing-declarations -Wredundant-decls -fomit-frame-pointer -DNIX -DMULTIPLE_SCORE_ENTRY
CXX = g++
LIBS = -lm -lncurses

all: compile

compile: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SOURCES) $(LIBS)
	strip $(EXEC)
	mv $(EXEC) ../bin/
