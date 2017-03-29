CXX=g++
CFLAGS=-g -Wall -std=c++11 `pkg-config opencv --cflags`
SRCS=./main.cpp ./objectDetect.cpp
PROG=salmoninfo

#OPENCV=$(pkg-config opencv --libs)
#LIBS=$(OPENCV)
LIBS = `pkg-config opencv --libs`

$(PROG):$(SRCS)
	$(CXX) -o $(PROG) $(SRCS) $(CFLAGS) $(LIBS)
