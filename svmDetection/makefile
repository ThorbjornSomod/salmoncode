CXX=g++
CFLAGS=-g -Wall -Wno-sign-compare -Wno-narrowing -std=c++11 -O2 `pkg-config opencv --cflags` `pkg-config libvlc --cflags`
SRCS=./main.cpp ./imageRetriever.cpp ./objectDetect.cpp ./lbp.cpp ./svm.cpp 
PROG=svmDetect
LIBS = `pkg-config opencv --libs` `pkg-config libvlc --libs` -lpthread /usr/local/lib/libfftw3.a

$(PROG):$(SRCS)
	$(CXX) -o $(PROG) $(SRCS) $(CFLAGS) $(LIBS)
