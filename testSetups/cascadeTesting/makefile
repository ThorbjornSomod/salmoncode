CXX=g++
CFLAGS=-g -Wall -std=c++11 `pkg-config opencv --cflags` `pkg-config libvlc --cflags`
SRCS=./main.cpp ./imageRetriever.cpp ./cascadeDetect.cpp
PROG=cascadeDetect

LIBS = `pkg-config opencv --libs` `pkg-config libvlc --libs` -lpthread

$(PROG):$(SRCS)
	$(CXX) -o $(PROG) $(SRCS) $(CFLAGS) $(LIBS)
