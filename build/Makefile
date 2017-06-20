SOURCES = $(wildcard ../src/*.cpp) $(wildcard ../src/**/*.cpp)
EXEC = larn-next
CXXFLAGS = -std=c++1z -Winline -fno-elide-constructors -pipe -Wall -Wextra -Werror -pedantic-errors -Wpointer-arith -Woverloaded-virtual -Wuninitialized -Wshadow -Winit-self -Wmissing-declarations -Wredundant-decls -fomit-frame-pointer -DNIX -DMULTIPLE_SCORE_ENTRY
CXX = g++
LIBS = -lm -lncurses

all: compile

compile: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SOURCES) $(LIBS)
	strip $(EXEC)
	mv $(EXEC) ../bin/
