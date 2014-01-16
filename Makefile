
all: fstalign

CXXFLAGS = -msse -msse2 
OPENFSTLIBS=-lfstscript -lfst

LDFLAGS = $(OPENFSTLDFLAGS)
LDLIBS = $(EXTRA_LDLIBS) $(OPENFSTLIBS) -lm -ldl
CC = g++
CXX = g++
AR = ar
AS = as
RANLIB = ranlib

fstalign: main.o FstAligner.o FstLoader.o OneBestFstLoader.o
	$(CXX)  $^ -o $@  $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf *.o fstalign-to-htk

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $<
