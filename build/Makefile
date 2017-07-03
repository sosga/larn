SOURCES = $(wildcard ../src/*.cpp) $(wildcard ../src/**/*.cpp)
EXEC = larn-next
CXXFLAGS = -std=c++17 -Winline -fno-elide-constructors -pipe -Wall -Wextra -Werror -pedantic-errors -Wpointer-arith -Woverloaded-virtual -Wuninitialized -Wshadow -Winit-self -Wmissing-declarations -Wredundant-decls -fomit-frame-pointer -DNIX
CXX = g++
LIBS = -lm -lncurses

all: compile

compile: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SOURCES) $(LIBS)
	mv $(EXEC) ../bin/
