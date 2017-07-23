SOURCES = $(wildcard ../src/*.cpp) $(wildcard ../src/**/*.cpp)
EXEC = larn-next
CXXFLAGS = -std=c++1z -Winline -fno-elide-constructors -pipe -Wall -Wextra -Werror -pedantic-errors -Wpointer-arith -Woverloaded-virtual -Wuninitialized -Wshadow -Winit-self -Wmissing-declarations -fomit-frame-pointer -DNIX
CXX = g++
LIBS = -lncurses -ljsoncpp

all: compile

compile: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SOURCES) $(LIBS)
	mv $(EXEC) ../bin/
