aux = auxiliar
idx = indexes
comp = compression

all: $(idx)/Main.cpp $(aux)/OptionsParsing.cpp SuffixArray.o SuffixTree.o SuffixTreeNode.o DummyCompressor.o DummyDecompressor.o Printer.o LZ78C.o LZ78D.o InputBuffer.o LZWC.o LZWD.o Compressor.o HashTable.o LZ77C.o LZ77D.o IpmtConfiguration.o Decompressor.o Global.o
	g++ -O3 -g -I $(aux) -I $(comp) SuffixArray.o SuffixTree.o SuffixTreeNode.o Printer.o DummyCompressor.o DummyDecompressor.o LZ78C.o LZ78D.o InputBuffer.o LZWC.o LZWD.o HashTable.o LZ77C.o LZ77D.o Compressor.o IpmtConfiguration.o  Decompressor.o Global.o $<

Global.o: $(aux)/Global.cpp $(aux)/Global.h
	g++ -O3 -g -I $(aux) -c $<

Compressor.o: $(comp)/Compressor.cpp $(comp)/Compressor.h $(aux)/Global.h
	g++ -O3 -g -I $(aux) -c $<
Decompressor.o: $(comp)/Decompressor.cpp $(comp)/Decompressor.h $(aux)/Global.h
	g++ -O3 -g -I $(aux) -c $<

IpmtConfiguration.o: $(aux)/IpmtConfiguration.cpp $(aux)/IpmtConfiguration.h $(aux)/Global.h
	g++ -O3 -g -I $(aux) -c $<

SuffixArray.o: $(idx)/SuffixArray.cpp $(idx)/SuffixArray.h $(idx)/Index.h $(aux)/Global.h $(aux)/Printer.h 
	g++ -O3 -g -I $(aux) -I $(comp) -I $(idx) -c $<

SuffixTreeNode.o: $(aux)/SuffixTreeNode.cpp $(aux)/SuffixTreeNode.h 
	g++ -O3  -g -c $< 
SuffixTree.o: $(idx)/SuffixTree.cpp $(idx)/SuffixTree.h $(aux)/SuffixTreeNode.h 
	g++ -O3 -I $(aux) -I $(comp) -g -c $< 

Printer.o: $(aux)/Printer.cpp $(aux)/Printer.h $(aux)/Global.h
	g++ -O3 -g -I $(aux) $(aux)/Printer.cpp -c $<

DummyCompressor.o: $(comp)/DummyCompressor.cpp $(comp)/DummyCompressor.h $(aux)/Global.h $(comp)/Compressor.h
	g++ -O3 -g -I $(aux) $(comp)/DummyCompressor.cpp -c $<
DummyDecompressor.o: $(comp)/DummyDecompressor.cpp $(comp)/DummyDecompressor.h $(aux)/Global.h
	g++ -O3 -g -I $(aux) $(comp)/DummyDecompressor.cpp -c $<

HashTable.o: $(aux)/HashTable.cpp $(aux)/HashTable.h $(aux)/Global.h
	g++ -O3 -g -c $<

InputBuffer.o: $(aux)/InputBuffer.cpp $(aux)/InputBuffer.h $(aux)/Global.h
	g++ -O3 -g $(aux)/InputBuffer.cpp -c $<

LZ78C.o: $(comp)/LZ78C.cpp $(comp)/LZ78C.h $(comp)/Compressor.h $(aux)/Global.h $(aux)/HashTable.h
	g++ -O3 -g -I $(aux) -c $<
LZ78D.o: $(comp)/LZ78D.cpp $(comp)/LZ78D.h $(comp)/Decompressor.h $(aux)/Global.h $(aux)/InputBuffer.h
	g++ -O3 -g -I $(aux) $(comp)/LZ78D.cpp -c $<

LZWC.o: $(comp)/LZWC.cpp $(comp)/LZWC.h $(comp)/Compressor.h $(aux)/Global.h $(aux)/HashTable.h
	g++ -O3 -g -I $(aux) -c $<
LZWD.o: $(comp)/LZWD.cpp $(comp)/LZWD.h $(comp)/Decompressor.h $(aux)/Global.h $(aux)/InputBuffer.h
	g++ -O3 -g -I $(aux)  -c $<

LZ77C.o: $(comp)/LZ77C.cpp $(comp)/LZ77C.h $(aux)/Global.h $(comp)/Compressor.h $(aux)/Treap.h
	g++ -O3 -g -I $(aux)  -c $<
LZ77D.o: $(comp)/LZ77D.cpp $(comp)/LZ77D.h $(aux)/InputBuffer.h $(comp)/Decompressor.h
	g++ -O3 -g -I $(aux)  -c $<

TestCompression: $(comp)/TestCompression.cpp LZ78C.o $(aux)/Global.h LZ78D.o InputBuffer.o LZWC.o LZWD.o Compressor.o LZ77C.o LZ77D.o HashTable.o Global.o Decompressor.o
	g++ -O3 -g -I $(aux) -I $(comp) LZ78C.o LZ78D.o InputBuffer.o LZWC.o LZWD.o  LZ77C.o Compressor.o  LZ77D.o HashTable.o Global.o Decompressor.o -o comp $<
	 


