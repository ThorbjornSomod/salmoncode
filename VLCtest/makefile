CXX=g++
CFLAGS=-g -Wall -std=c++11 `pkg-config opencv --cflags` `pkg-config libvlc --cflags`
SRCS=./test.cpp
PROG=test

LIBS = `pkg-config opencv --libs` `pkg-config libvlc --libs`

$(PROG):$(SRCS)
	$(CXX) -o $(PROG) $(SRCS) $(CFLAGS) $(LIBS)
