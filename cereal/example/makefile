CFLAGS = -O2 -Wall -Wextra -pedantic -std=c++14 -IC:/boost/boost_1_64_0 -IC:/_cpplibs/include \
-LC:/boost/lib_64gcc/lib -static


CXX = g++


SRC = example.cpp
INCL = example.hpp
BINS = main.exe
OBJ = $(patsubst %.cpp, %.o, $(SRC))


all: $(BINS)


example.o: example.cpp $(INCL)
	$(CXX) -c -o $@ $< $(CFLAGS)

main.exe: main.cpp $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(BINS) $(OBJ)


.PHONY: all, clean
