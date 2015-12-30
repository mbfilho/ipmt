aux = auxiliar
idx = indexes
comp = compression

all: $(idx)/Main.cpp $(aux)/OptionsParsing.cpp SuffixArray.o SuffixArray2.o SuffixTree.o SuffixTreeNode.o DummyCompressor.o DummyDecompressor.o Printer.o LZ78C.o LZ78D.o TrieNode.o InputBuffer.o LZWC.o LZWD.o Compressor.o
	g++ -O3 -g -I $(aux) -I $(comp) SuffixArray.o SuffixArray2.o SuffixTree.o SuffixTreeNode.o Printer.o DummyCompressor.o DummyDecompressor.o LZ78C.o LZ78D.o TrieNode.o InputBuffer.o LZWC.o LZWD.o Compressor.o $<

Compressor.o: $(comp)/Compressor.cpp $(comp)/Compressor.h $(aux)/Global.h
	g++ -O3 -g -I $(aux) -c $<

SuffixArray.o: $(idx)/SuffixArray.cpp $(idx)/SuffixArray.h $(idx)/Index.h $(aux)/Global.h $(aux)/Printer.h $(comp)/Compressor.h $(comp)/Decompressor.h
	g++ -O3 -g -I $(aux) -I $(comp) -c $<
SuffixArray2.o: $(idx)/SuffixArray2.cpp $(idx)/SuffixArray2.h $(idx)/Index.h $(comp)/Compressor.h $(comp)/Decompressor.h
	g++ -O3 -g -I $(aux) -I $(comp) -c $<

SuffixTreeNode.o: $(aux)/SuffixTreeNode.cpp $(aux)/SuffixTreeNode.h 
	g++ -O3  -g -c $< 
SuffixTree.o: $(idx)/SuffixTree.cpp $(idx)/SuffixTree.h $(aux)/SuffixTreeNode.h $(comp)/Compressor.h $(comp)/Decompressor.h
	g++ -O3 -I $(aux) -I $(comp) -g -c $< 

Printer.o: $(aux)/Printer.cpp $(aux)/Printer.h $(aux)/Global.h
	g++ -O3 -g -I $(aux) $(aux)/Printer.cpp -c $<

DummyCompressor.o: $(comp)/DummyCompressor.cpp $(comp)/DummyCompressor.h $(aux)/Global.h
	g++ -O3 -g -I $(aux) $(comp)/DummyCompressor.cpp -c $<
DummyDecompressor.o: $(comp)/DummyDecompressor.cpp $(comp)/DummyDecompressor.h $(aux)/Global.h
	g++ -O3 -g -I $(aux) $(comp)/DummyDecompressor.cpp -c $<

TrieNode.o: $(aux)/TrieNode.cpp $(aux)/TrieNode.h $(aux)/Global.h
	g++ -O3 -g $(aux)/TrieNode.cpp -c $<
HashTable.o: $(aux)/HashTable.cpp $(aux)/HashTable.h $(aux)/Global.h
	g++ -O3 -g -c $<

InputBuffer.o: $(aux)/InputBuffer.cpp $(aux)/InputBuffer.h $(aux)/Global.h
	g++ -O3 -g $(aux)/InputBuffer.cpp -c $<

LZ78C.o: $(comp)/LZ78C.cpp $(comp)/LZ78C.h $(comp)/Compressor.h $(aux)/Global.h $(aux)/TrieNode.h
	g++ -O3 -g -I $(aux) $(comp)/LZ78C.cpp -c $<
LZ78D.o: $(comp)/LZ78D.cpp $(comp)/LZ78D.h $(comp)/Decompressor.h $(aux)/Global.h $(aux)/InputBuffer.h
	g++ -O3 -g -I $(aux) $(comp)/LZ78D.cpp -c $<

LZWC.o: $(comp)/LZWC.cpp $(comp)/LZWC.h $(comp)/Compressor.h $(aux)/Global.h $(aux)/HashTable.h
	g++ -O3 -g -I $(aux) -c $<
LZWD.o: $(comp)/LZWD.cpp $(comp)/LZWD.h $(comp)/Decompressor.h $(aux)/Global.h $(aux)/InputBuffer.h
	g++ -O3 -g -I $(aux) $(comp)/LZWD.cpp -c $<

SlidingWindow.o: $(aux)/SlidingWindow.cpp $(aux)/SlidingWindow.h 
	g++ -O3 -g -c $<

LZ77C.o: $(comp)/LZ77C.cpp $(comp)/LZ77C.h $(aux)/Global.h $(comp)/Compressor.h
	g++ -O3 -g -I $(aux)  -c $<
LZ77D.o: $(comp)/LZ77D.cpp $(comp)/LZ77D.h $(aux)/InputBuffer.h $(aux)/SlidingWindow.h $(comp)/Decompressor.h
	g++ -O3 -g -I $(aux)  -c $<

TestCompression: $(comp)/TestCompression.cpp LZ78C.o $(aux)/Global.h TrieNode.o LZ78D.o InputBuffer.o LZWC.o LZWD.o Compressor.o LZ77C.o SlidingWindow.o LZ77D.o HashTable.o
	g++ -O3 -g -I $(aux) -I $(comp) LZ78C.o TrieNode.o LZ78D.o InputBuffer.o LZWC.o LZWD.o LZ77C.o SlidingWindow.o Compressor.o LZ77D.o HashTable.o -o comp $<


