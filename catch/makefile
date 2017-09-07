CFLAGS = -O2 -Wall -Wextra -pedantic -std=c++14 -IC:/boost/boost_1_64_0 \
-IC:/_cpplibs/include -LC:/boost/lib_64gcc/lib -static

CXX = g++


SRC = $(wildcard *.cpp)
INCL =
BINS = $(patsubst %.cpp, %.exe, $(SRC))
OBJ = $(patsubst %.cpp, %.o, $(BINS))
DEPS = 


all: $(BINS)


%.exe: %.cpp $(INCL) $(DEPS)
	$(CXX) -o $@ $^ $(CFLAGS)


clean:
	rm -f $(BINS) $(OBJ)


.PHONY: all, clean
