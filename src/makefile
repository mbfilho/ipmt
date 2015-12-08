aux = auxiliar
idx = indexes

all: $(idx)/Main.cpp SuffixArray.o SuffixArray2.o SuffixTree.o SuffixTreeNode.o
	g++ -O3 -g -I $(aux) $(idx)/Main.cpp SuffixArray.o SuffixArray2.o SuffixTree.o SuffixTreeNode.o

SuffixArray.o: $(idx)/SuffixArray.cpp $(idx)/SuffixArray.h $(idx)/Index.h
	g++ -O3 -g -c $<
SuffixArray2.o: $(idx)/SuffixArray2.cpp $(idx)/SuffixArray2.h $(idx)/Index.h
	g++ -O3 -g -c $<

SuffixTreeNode.o: $(aux)/SuffixTreeNode.cpp $(aux)/SuffixTreeNode.h 
	g++ -O3  -g -c $< 
SuffixTree.o: $(idx)/SuffixTree.cpp $(idx)/SuffixTree.h $(aux)/SuffixTreeNode.h
	g++ -O3 -I $(aux) -g -c $< 
