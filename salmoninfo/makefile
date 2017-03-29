CXX=g++
CFLAGS=-g -Wall -std=c++11 `pkg-config opencv --cflags`
SRCS=./main.cpp ./objectDetect.cpp ./LBP/LBP.cpp
PROG=salmoninfo
LIBS = `pkg-config opencv --libs`

$(PROG):$(SRCS)
	$(CXX) -o $(PROG) $(SRCS) $(CFLAGS) $(LIBS)
