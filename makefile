src = src
aux = src/auxiliar
idx = src/indexes
comp = src/compression

genObjFile = g++ -O3 -g -I $(aux) -I $(comp) -I $(idx) -c $<

install:
	mkdir -p bin
	make all
	mv ipmt bin/ipmt
	rm *.o

all: $(src)/Ipmt.cpp $(aux)/OptionsParsing.cpp SuffixArray.o SuffixTree.o SuffixTreeNode.o DummyCompressor.o DummyDecompressor.o Printer.o LZ78C.o LZ78D.o InputBuffer.o LZWC.o LZWD.o Compressor.o HashTable.o LZ77C.o LZ77D.o IpmtConfiguration.o Decompressor.o Global.o SuffixTree2.o SuffixTreeAux.o
	g++ -O3 -g -I $(aux) -I $(comp) -I $(idx) SuffixArray.o SuffixTree.o SuffixTreeNode.o Printer.o DummyCompressor.o DummyDecompressor.o LZ78C.o LZ78D.o InputBuffer.o LZWC.o LZWD.o HashTable.o LZ77C.o LZ77D.o Compressor.o IpmtConfiguration.o  Decompressor.o Global.o SuffixTree2.o SuffixTreeAux.o -o ipmt $<

Global.o: $(aux)/Global.cpp $(aux)/Global.h
	$(genObjFile)

Compressor.o: $(comp)/Compressor.cpp $(comp)/Compressor.h $(aux)/Global.h
	$(genObjFile)
Decompressor.o: $(comp)/Decompressor.cpp $(comp)/Decompressor.h $(aux)/Global.h
	$(genObjFile)

IpmtConfiguration.o: $(aux)/IpmtConfiguration.cpp $(aux)/IpmtConfiguration.h $(aux)/Global.h
	$(genObjFile)

SuffixArray.o: $(idx)/SuffixArray.cpp $(idx)/SuffixArray.h $(idx)/Index.h $(aux)/Global.h $(aux)/Printer.h $(comp)/Compressor.h $(comp)/Decompressor.h
	$(genObjFile)

SuffixTreeAux.o: $(aux)/SuffixTreeAux.cpp $(aux)/SuffixTreeAux.h $(aux)/SuffixTreeNode.h $(aux)/Global.h
	$(genObjFile)
SuffixTreeNode.o: $(aux)/SuffixTreeNode.cpp $(aux)/SuffixTreeNode.h 
	$(genObjFile)
SuffixTree.o: $(idx)/SuffixTree.cpp $(idx)/SuffixTree.h $(aux)/SuffixTreeNode.h $(comp)/Compressor.h $(comp)/Decompressor.h $(aux)/Printer.h $(aux)/Global.h $(idx)/Index.h
	$(genObjFile)
SuffixTree2.o: $(idx)/SuffixTree2.cpp $(idx)/SuffixTree2.h $(aux)/SuffixTreeAux.h $(comp)/Compressor.h $(comp)/Decompressor.h $(aux)/Printer.h $(aux)/Global.h $(idx)/Index.h
	$(genObjFile)

Printer.o: $(aux)/Printer.cpp $(aux)/Printer.h $(aux)/Global.h
	$(genObjFile)

DummyCompressor.o: $(comp)/DummyCompressor.cpp $(comp)/DummyCompressor.h $(aux)/Global.h $(comp)/Compressor.h
	$(genObjFile)
DummyDecompressor.o: $(comp)/DummyDecompressor.cpp $(comp)/DummyDecompressor.h $(aux)/Global.h
	$(genObjFile)

HashTable.o: $(aux)/HashTable.cpp $(aux)/HashTable.h $(aux)/Global.h
	$(genObjFile)

InputBuffer.o: $(aux)/InputBuffer.cpp $(aux)/InputBuffer.h $(aux)/Global.h
	$(genObjFile)

LZ78C.o: $(comp)/LZ78C.cpp $(comp)/LZ78C.h $(comp)/Compressor.h $(aux)/Global.h $(aux)/HashTable.h
	$(genObjFile)
LZ78D.o: $(comp)/LZ78D.cpp $(comp)/LZ78D.h $(comp)/Decompressor.h $(aux)/Global.h $(aux)/InputBuffer.h
	$(genObjFile)

LZWC.o: $(comp)/LZWC.cpp $(comp)/LZWC.h $(comp)/Compressor.h $(aux)/Global.h $(aux)/HashTable.h
	$(genObjFile)
LZWD.o: $(comp)/LZWD.cpp $(comp)/LZWD.h $(comp)/Decompressor.h $(aux)/Global.h $(aux)/InputBuffer.h
	$(genObjFile)

LZ77C.o: $(comp)/LZ77C.cpp $(comp)/LZ77C.h $(aux)/Global.h $(comp)/Compressor.h $(aux)/Treap.h
	$(genObjFile)
LZ77D.o: $(comp)/LZ77D.cpp $(comp)/LZ77D.h $(aux)/InputBuffer.h $(comp)/Decompressor.h
	$(genObjFile)


clear:
	rm -f *.o 


