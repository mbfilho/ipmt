
src=../../src/indexes

all: SuffixArrayTest SuffixArray2Test
	
	
SuffixArrayTest:
	cd $(src) && make SuffixArray.o
	g++ -O3 -g SuffixArrayTest.cpp -I $(src) $(src)/SuffixArray.o -o SuffixArrayTest.out
	echo 'Testando SuffixArray.cpp'
	echo '------------------------------'
	./SuffixArrayTest.out

SuffixArray2Test:
	cd $(src) && make SuffixArray2.o
	g++ -O3 -g SuffixArray2Test.cpp -I $(src) $(src)/SuffixArray2.o -o SuffixArray2Test.out
	echo 'Testando SuffixArray2.cpp'
	echo '------------------------------'
	./SuffixArray2Test.out
