#ifndef LZ78D_H
#define LZ78D_H
#include "InputBuffer.h"
#include "Decompressor.h"


class LZ78D : public Decompressor {
public:
	LZ78D(FILE* inputFile);
	
	void close();
private:
	InputBuffer input;	

	vector<Byte> buffer;
	vector<pair<int,int> > dictionary;

	int nextAvailableBytePos;

	void readToken();
	int decodeInt();
	int readByte();
};
#endif
