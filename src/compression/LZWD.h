#ifndef LZWD_H
#define LZWD_H
#include "InputBuffer.h"
#include "Decompressor.h"

class LZWD : public Decompressor {
public:
	LZWD(FILE* inputFile, int compressionLevel);
	
	void close();

private:
	InputBuffer input;	
	vector<Byte> buffer;
	vector<pair<int,int> > dictionary;

	int nextAvailableBytePos;

	pair<int,int> lastSequence;
	
	int dictionaryMaxSize;

	void readToken();
	int decodeInt();
	int decompressNextByte();
};
#endif
