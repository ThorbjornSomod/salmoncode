CXX=g++
CFLAGS=-g -Wall -Wno-sign-compare -std=c++11 `pkg-config opencv --cflags` 
SRCS=./main.cpp ./objectDetect.cpp ./lbp.cpp ./svm.cpp 
PROG=salmoninfo
LIBS = `pkg-config opencv --libs` /usr/local/lib/libfftw3.a

$(PROG):$(SRCS)
	$(CXX) -o $(PROG) $(SRCS) $(CFLAGS) $(LIBS)
