CFLAGS = -O2 -Wall -Wextra -pedantic -std=c++14 -IC:/boost/boost_1_64_0 -LC:/boost/lib_gcc64/lib -static \
-lboost_system-mt -lboost_regex-mt -lboost_filesystem-mt -lboost_date_time-mt \
-lboost_thread-mt -lws2_32 -lwsock32

CXX = g++


SRC = $(wildcard *.cpp)
INCL = message.hpp
BINS = server.exe client.exe
OBJ = $(patsubst %.cpp, %.o, $(SRC))
DEPS = 


all: $(BINS)

%.o: %.cpp $(INCL) $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

server.exe: server.o
	$(CXX) -o $@ $^ $(CFLAGS)

client.exe: client.o
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(BINS) $(OBJ)


.PHONY: all, clean
