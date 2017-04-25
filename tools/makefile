CXX=g++
CFLAGS=-g -Wall -std=c++11 `pkg-config opencv --cflags`
SRCS=./main.cpp ./tools.cpp ./formatConverter.cpp
PROG=tools

LIBS = `pkg-config opencv --libs`

$(PROG):$(SRCS)
	$(CXX) -o $(PROG) $(SRCS) $(CFLAGS) $(LIBS)
