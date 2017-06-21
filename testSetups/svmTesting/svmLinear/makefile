CXX=g++
CFLAGS=-g -Wall -Wno-sign-compare -Wno-narrowing -std=c++11 `pkg-config opencv --cflags` `pkg-config libvlc --cflags` -fopenmp
SRCS=./main.cpp ./imageRetriever.cpp ./objectDetect.cpp ./lbp.cpp ./linear.cpp ./tron.cpp 
PROG=svmDetect
LIBS = `pkg-config opencv --libs` `pkg-config libvlc --libs` -lpthread /usr/local/lib/libfftw3.a blas/blas.a

$(PROG):$(SRCS)
	$(CXX) -o $(PROG) $(SRCS) $(CFLAGS) $(LIBS)
