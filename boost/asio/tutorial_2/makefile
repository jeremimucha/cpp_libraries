CFLAGS = -O2 -Wall -Wextra -pedantic -std=c++14 -IC:/boost/boost_1_64_0 -LC:/boost/lib_gcc64/lib -static \
-lboost_system-mt -lboost_regex-mt -lboost_filesystem-mt -lboost_date_time-mt \
-lboost_thread-mt -lws2_32 -lwsock32

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
