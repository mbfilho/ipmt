
all: Main.cpp SuffixArray.o SuffixArray2.o
	g++ -O3 -g Main.cpp SuffixArray.o SuffixArray2.o

SuffixArray.o: SuffixArray.cpp SuffixArray.h Index.h
	g++ -O3 -g -c $<

SuffixArray2.o: SuffixArray2.cpp SuffixArray2.h Index.h
	g++ -O3 -g -c $<
