CFLAGS = -O2 -Wall -Wextra -pedantic -std=c++14 -IC:/boost/boost_1_64_0 -LC:/boost/lib_64gcc/lib -static \
-lboost_filesystem-mgw72-mt-1_64 -lboost_system-mgw72-mt-1_64 -lboost_system-mgw72-mt-1_64.dll \
-lboost_system-mgw72-mt-d-1_64 -lboost_system-mgw72-mt-d-1_64.dll

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
