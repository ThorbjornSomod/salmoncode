CXX=g++
CFLAGS=-g -Wall -std=c++11 `pkg-config opencv --cflags`
SRCS=./main.cpp ./detectionTest.cpp
PROG=tests

#OPENCV=$(pkg-config opencv --libs)
#LIBS=$(OPENCV)
LIBS = `pkg-config opencv --libs`

$(PROG):$(SRCS)
	$(CXX) -o $(PROG) $(SRCS) $(CFLAGS) $(LIBS)
